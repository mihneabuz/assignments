#include "so_stdio.h"
#include "fcntl.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"

#define READ 1
#define WRITE 2
#define APPEND 4
#define CREATE 8
#define TRUNCATE 16

#define BUFFER_SIZE 4096

struct buffer {
	char ptr[BUFFER_SIZE];
	int idx;
	int end;
};

typedef struct _so_file {
	int fd;
	size_t pos;
	struct buffer *buf;
	struct buffer *aux;
	char flags;
	char err;
} SO_FILE;

SO_FILE *so_fopen(const char *pathname, const char *mode)
{
	char flags = 0;

	if (mode[0] == 'w') {
		flags |= WRITE;
		flags |= CREATE;
		flags |= TRUNCATE;
		if (mode[1] == '+')
			flags |= READ;

	} else if (mode[0] == 'r') {
		flags |= READ;
		if (mode[1] == '+')
			flags |= WRITE;

	} else if (mode[0] == 'a') {
		flags |= WRITE;
		flags |= APPEND;
		flags |= CREATE;
		if (mode[1] == '+')
			flags |= READ;

	} else {
		return NULL;
	}

	int open_mode = 0;

	if ((flags & CREATE) != 0)
		open_mode |= O_CREAT;

	if ((flags & TRUNCATE) != 0)
		open_mode |= O_TRUNC;

	if ((flags & WRITE) != 0 && (flags & READ) == 0)
		open_mode |= O_WRONLY;
	else if ((flags & WRITE) == 0 && (flags & READ) != 0)
		open_mode |= O_RDONLY;
	else
		open_mode |= O_RDWR;

	if ((flags & APPEND) != 0)
		open_mode |= O_APPEND;

	int fd = open(pathname, open_mode, 0644);

	if (fd < 0)
		return NULL;

	SO_FILE *tmp = malloc(sizeof(SO_FILE));

	if (tmp == NULL)
		return NULL;

	tmp->fd = fd;
	tmp->flags = flags;
	tmp->pos = 0;
	tmp->err = 0;

	tmp->buf = malloc(sizeof(struct buffer));
	if (tmp->buf == NULL)
		return NULL;

	if ((flags & READ) != 0 && (flags & WRITE) != 0) {
		tmp->aux = malloc(sizeof(struct buffer));
		if (tmp->aux == NULL)
			return NULL;
		tmp->buf->end = 4096;
		tmp->buf->idx = 4096;

		tmp->aux->end = 4096;
		tmp->aux->idx = 0;
	} else {
		tmp->aux = NULL;

		if ((flags & READ) != 0) {
			tmp->buf->end = 4096;
			tmp->buf->idx = 4096;
		} else if ((flags & WRITE) != 0) {
			tmp->buf->end = 4096;
			tmp->buf->idx = 0;
		}
	}

	return tmp;
}

int so_fclose(SO_FILE *stream)
{
	int ret = 0;

	if ((stream->flags & WRITE) != 0) {
		int rc = so_fflush(stream);

		if (rc == -1)
			ret = SO_EOF;
	}

	if (stream->aux != NULL)
		free(stream->aux);
	free(stream->buf);

	if (close(stream->fd) == -1)
		ret = SO_EOF;

	free(stream);

	return ret;
}

int _has_buffer(struct buffer *buf, size_t size)
{
	return (buf->end - buf->idx) >= size;
}

int _refill_buffer(SO_FILE *stream)
{
	stream->buf->end = read(stream->fd, stream->buf->ptr, BUFFER_SIZE);

	if (stream->buf->end == -1)
		stream->err = 1;

	if (stream->buf->end == 0)
		return SO_EOF;

	stream->buf->idx = 0;
	return 0;
}

int so_fileno(SO_FILE *stream)
{
	return stream->fd;
}

int so_fgetc(SO_FILE *stream)
{
	if (!_has_buffer(stream->buf, 1)) {
		int rc = _refill_buffer(stream);

		if (rc == -1)
			return -1;
	}

	char c = stream->buf->ptr[stream->buf->idx];

	stream->buf->idx++;
	stream->pos++;
	return c;
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	int rc;
	size_t remaining = size * nmemb;

	while (!_has_buffer(stream->buf, remaining)) {
		size_t buf_size = stream->buf->end - stream->buf->idx;

		memcpy(ptr, stream->buf->ptr + stream->buf->idx, buf_size);
		ptr += buf_size;
		remaining -= buf_size;
		stream->pos += buf_size;

		rc = _refill_buffer(stream);
		if (rc == -1)
			return (size * nmemb - remaining);
	}

	if (so_ferror(stream) != 0)
		return 0;

	memcpy(ptr, stream->buf->ptr + stream->buf->idx, remaining);
	stream->buf->idx += remaining;
	stream->pos += remaining;

	return nmemb;
}

int so_fflush(SO_FILE *stream)
{
	struct buffer *buf = stream->aux != NULL ? stream->aux : stream->buf;
	int total_wrote = 0;

	while (total_wrote < buf->idx) {
		int wrote = write(stream->fd, buf->ptr + total_wrote, buf->idx - total_wrote);

		if (wrote == -1)
			return -1;

		total_wrote += wrote;
	}

	buf->idx = 0;
	return 0;
}

int so_fputc(int c, SO_FILE *stream)
{
	struct buffer *buf = stream->aux != NULL ? stream->aux : stream->buf;

	if (!_has_buffer(buf, 1)) {
		int rc = so_fflush(stream);

		if (rc == -1)
			return SO_EOF;
	}

	buf->ptr[buf->idx] = c;
	buf->idx++;
	stream->pos++;
	return c;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	struct buffer *buf = stream->aux != NULL ? stream->aux : stream->buf;
	int rc;
	size_t remaining = size * nmemb;

	while (!_has_buffer(buf, remaining)) {
		size_t buf_size = buf->end - buf->idx;

		memcpy(buf->ptr + buf->idx, ptr, buf_size);
		buf->idx += buf_size;
		stream->pos += buf_size;

		rc = so_fflush(stream);
		if (rc == -1) {
			stream->err = 1;
			return 0;
		}

		ptr += buf_size;
		remaining -= buf_size;
	}

	memcpy(buf->ptr + buf->idx, ptr, remaining);
	buf->idx += remaining;
	stream->pos += remaining;
	return nmemb;
}

int so_fseek(SO_FILE *stream, long offset, int whence)
{
	if ((stream->flags & WRITE) != 0)
		so_fflush(stream);

	int rc = lseek(stream->fd, offset, whence);

	if (rc == -1) {
		return SO_EOF;
	} else {
		stream->pos = rc;

		if (stream->aux != NULL) {
			stream->buf->end = 4096;
			stream->buf->idx = 4096;

			stream->aux->end = 4096;
			stream->aux->idx = 0;
		} else {
			if ((stream->flags & READ) != 0) {
				stream->buf->end = 4096;
				stream->buf->idx = 4096;
			} else {
				stream->buf->end = 4096;
				stream->buf->idx = 0;
			}
		}

		return 0;
	}
}

long so_ftell(SO_FILE *stream)
{
	return stream->pos;
}


int so_feof(SO_FILE *stream)
{
	off_t cur = lseek(stream->fd, 0, SEEK_CUR);
	off_t end = lseek(stream->fd, 0, SEEK_END);

	lseek(stream->fd, stream->pos, SEEK_SET);
	if (end == cur && stream->buf->idx >= stream->buf->end)
		return 1;
	else
		return 0;
}

int so_ferror(SO_FILE *stream)
{
	return stream->err;
}

SO_FILE *so_popen(const char *command, const char *type) {}
int so_pclose(SO_FILE *stream) {}
