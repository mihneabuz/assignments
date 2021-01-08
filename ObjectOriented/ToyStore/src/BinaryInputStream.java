import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

/** wrapper over InputStream to handle reading binary files **/
public class BinaryInputStream {
    private final InputStream reader;

    public BinaryInputStream(InputStream reader) {
        this.reader = reader;
    }

    public int readInt() throws IOException {
        byte[] bytes = reader.readNBytes(4);
        ByteBuffer wrapped = ByteBuffer.wrap(bytes);
        return wrapped.getInt();
    }

    public String readString() throws IOException {
        int len = readInt();
        StringBuilder stringBuilder = new StringBuilder();
        for (int i = 0; i < len; i ++) {
            stringBuilder.append(new String(reader.readNBytes(2)));
        }
        return stringBuilder.toString();
    }

    public char readChar() throws IOException {
        byte[] bytes = reader.readNBytes(2);
        ByteBuffer wrapped = ByteBuffer.wrap(bytes);
        return wrapped.getChar();
    }

    public double readDouble() throws IOException {
        byte[] bytes = reader.readNBytes(8);
        ByteBuffer wrapped = ByteBuffer.wrap(bytes);
        return wrapped.getDouble();
    }

    public void close() throws IOException {
        reader.close();
    }
}
