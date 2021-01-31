public class ProductBuilder {
    private String TYPE;
    private int ID;
    private String name;
    private double startPrice;
    private String artistName;
    private String colors;
    private String furnitureType;
    private String material;
    private boolean withGem;

    public ProductBuilder(String TYPE) {
        this.TYPE = TYPE;
    }

    public ProductBuilder withID(int ID) {
        this.ID = ID;
        return this;
    }

    public ProductBuilder withName(String name) {
        this.name = name;
        return this;
    }

    public ProductBuilder withStartPrice(double startPrice) {
        this.startPrice = startPrice;
        return this;
    }

    public ProductBuilder withArtist(String artistName) {
        this.artistName = artistName;
        return this;
    }

    public ProductBuilder withColors(String colors) {
        this.colors = colors;
        return this;
    }

    public ProductBuilder withFurnitureType(String furnitureType) {
        this.furnitureType = furnitureType;
        return this;
    }

    public ProductBuilder withMaterial(String material) {
        this.material = material;
        return this;
    }

    public ProductBuilder setWithGem(boolean withGem) {
        this.withGem = withGem;
        return this;
    }

    public Product build() throws InvalidProductException {
        return ProductFactory.buildProduct(this);
    }

    public String getTYPE() {
        return TYPE;
    }

    public int getID() {
        return ID;
    }

    public String getName() {
        return name;
    }

    public double getStartPrice() {
        return startPrice;
    }

    public String getArtistName() {
        return artistName;
    }

    public String getColors() {
        return colors;
    }

    public String getFurnitureType() {
        return furnitureType;
    }

    public String getMaterial() {
        return material;
    }

    public boolean isWithGem() {
        return withGem;
    }
}
