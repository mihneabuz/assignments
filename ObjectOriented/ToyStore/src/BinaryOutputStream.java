import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;

/** wrapper over OutputStream to handle writing binary files **/
public class BinaryOutputStream {
    private final OutputStream writer;

    public BinaryOutputStream(OutputStream writer) {
        this.writer = writer;
    }

    public void write(String s) throws IOException {
        int len = s.length();
        write(len);
        for (int i = 0; i < len; i ++)
            write(s.charAt(i));
    }

    public void write(char c) throws IOException {
        byte[] bytes = ByteBuffer.allocate(2).putChar(c).array();
        writer.write(bytes);
    }

    public void write(int x) throws IOException {
        byte[] bytes = ByteBuffer.allocate(4).putInt(x).array();
        writer.write(bytes);
    }

    public void write(double x) throws IOException {
        byte[] bytes = ByteBuffer.allocate(8).putDouble(x).array();
        writer.write(bytes);
    }

    public void close() throws IOException {
        writer.close();
    }
}
