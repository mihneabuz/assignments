/*
 * Loader Implementation
 *
 * 2018, Operating Systems
 */

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "exec_parser.h"

#define FAILED_MMAP -2
#define FAILED_READ -3
#define FAILED_PERM -4
#define INVALID_PERM -5

static void(*last_handler)(int, siginfo_t*, void*);
static int page_size;
static so_exec_t *exec;
static int fd;

static inline int check_page_mapped(so_seg_t *segment, int index)
{
	// alloc data if needed
	if (segment->data == NULL)
		segment->data = calloc(segment->mem_size / page_size, sizeof(char));

	// if page is already mapped, SEGV is caused by bad permisions
	return (((char *)segment->data)[index]);
}

static inline void set_page_mapped(so_seg_t *segment, int index)
{
	((char *)segment->data)[index] = 1;
}

static inline int copy_to_page(uintptr_t page_addr, char *map_addr, so_seg_t *segment)
{
	int bytes_read;
	int read_size = segment->file_size - page_addr;

	if (read_size > 0) {
		read_size = read_size > page_size ? page_size : read_size;
		lseek(fd, segment->offset + page_addr, SEEK_SET);

		bytes_read = read(fd, map_addr, read_size);
		if (bytes_read < read_size)
			return -1;

		if (read_size < page_size)
			memset(map_addr + read_size, 0, page_size - read_size);

	} else {
		memset(map_addr, 0, page_size);
	}

	return 0;
}

static inline int translate_permissions(int perm)
{
	return PROT_READ  * ((perm & PERM_R) != 0) |
		   PROT_WRITE * ((perm & PERM_W) != 0) |
		   PROT_EXEC  * ((perm & PERM_X) != 0);
}

int map_page(uintptr_t page_fault_addr, so_seg_t *segment)
{
	int rc;
	char *map_addr;
	int index = (page_fault_addr - segment->vaddr) / page_size;
	uintptr_t page_addr = index * page_size;

	if (check_page_mapped(segment, index))
		return INVALID_PERM;

	// map page in memory (set write permisions so we can copy the file)
	map_addr = mmap((void *)segment->vaddr + page_addr, page_size,
					PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (map_addr == MAP_FAILED)
		return FAILED_MMAP;

	// copy exec data to page
	rc = copy_to_page(page_addr, map_addr, segment);
	if (rc != 0)
		return FAILED_READ;

	set_page_mapped(segment, index);

	// set final permisions of the page
	rc = mprotect(map_addr, page_size, translate_permissions(segment->perm));
	if (rc != 0)
		return FAILED_PERM;

	return 0;
}

so_seg_t *find_segment(uintptr_t page_fault_addr)
{
	so_seg_t *seg;

	for (int i = 0; i < exec->segments_no; i++) {
		seg = &exec->segments[i];
		if (seg->vaddr <= page_fault_addr && page_fault_addr <= seg->vaddr + seg->mem_size)
			return seg;
	}

	return NULL;
}

void so_sigaction(int sig_no, siginfo_t *sig_info, void *context)
{
	int rc;
	so_seg_t *segment;
	uintptr_t page_fault_addr = (uintptr_t)sig_info->si_addr;

	segment = find_segment(page_fault_addr);
	if (segment == NULL)
		goto fail;

	rc = map_page(page_fault_addr, segment);
	if (rc != 0)
		goto fail;

	return;

fail:
	last_handler(sig_no, sig_info, context);
}

static inline void set_segv_handler(void *handler)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_sigaction = handler;
	sigaction(SIGSEGV, &sa, NULL);
}

static inline void *get_segv_handler(void)
{
	struct sigaction old_sa;

	sigaction(SIGSEGV, NULL, &old_sa);
	return old_sa.sa_sigaction;
}

int so_init_loader(void)
{
	// find page size
	page_size = getpagesize();

	// find last handler
	last_handler = get_segv_handler();

	// set new handler
	set_segv_handler(so_sigaction);

	return 0;
}

int so_execute(char *path, char *argv[])
{
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -1;

	exec = so_parse_exec(path);
	if (!exec)
		return -1;

	so_start_exec(exec, argv);

	close(fd);

	return -1;
}
