import java.io.IOException;
import java.io.OutputStream;

public class BinaryOutputStream {
    OutputStream writer;

    public BinaryOutputStream(OutputStream writer) {
        this.writer = writer;
    }

    public void write(String s) throws IOException {
        int len = s.length();
        writer.write(len);
        for (int i = 0; i < len; i ++)
            writer.write(s.charAt(i));
    }

    public void write(int x) throws IOException {
        writer.write(x);
    }

    public void write(double x) throws IOException {
        String valueString = String.format("%f", x);
        write(valueString);
    }

    public void close() throws IOException {
        writer.close();
    }
}
