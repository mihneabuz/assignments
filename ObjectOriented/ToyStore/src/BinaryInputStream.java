import java.io.IOException;
import java.io.InputStream;

public class BinaryInputStream {
    InputStream reader;

    public BinaryInputStream(InputStream reader) {
        this.reader = reader;
    }

    public int readInt() throws IOException {
        return reader.read();
    }

    public String readString() throws IOException {
        int len = reader.read();
        StringBuilder stringBuilder = new StringBuilder();
        for (int i = 0; i < len; i ++) {
            stringBuilder.append(reader.readNBytes(1).toString());
        }
        return stringBuilder.toString();
    }

    public double readDouble() throws IOException {
        return Double.parseDouble(readString());
    }

    public void close() throws IOException {
        reader.close();
    }
}
