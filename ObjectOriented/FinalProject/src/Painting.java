public class Painting extends Product {
    public enum Colors {
        OIL,
        ACRYLIC,
        TEMPERA
    }
    private String artistName;
    private Colors colors;

    public Painting() {
        super();
    }

    public Painting(int ID, String name, double startPrice, String artistName, String colors) {
        super(ID, name, startPrice);
        this.artistName = artistName;
        this.colors = Colors.valueOf(colors);
    }

    public void setArtistName(String artistName) {
        this.artistName = artistName;
    }

    public void setColors(String colors) {
        this.colors = Colors.valueOf(colors);
    }

    public String toString() {
        return String.format("Painting by %s in %s\n", artistName, colors.toString()) + super.toString();
    }
}
