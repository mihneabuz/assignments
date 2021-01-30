public class ProductBuilder<T extends Product> {
    int ID;
    String name;
    double startPrice;
    String artistName;
    String colors;
    String material;
    String furnitureType;
    boolean withGem;

    public ProductBuilder() {
    }

    ProductBuilder<T> withID(int ID) {
        this.ID = ID;
        return this;
    }

    ProductBuilder<T> withName(String name) {
        this.name = name;
        return this;
    }

    ProductBuilder<T> withStartPrice(double startPrice) {
        this.startPrice = startPrice;
        return this;
    }

    ProductBuilder<T> withArtist(String name) {
        this.artistName = name;
        return this;
    }

    ProductBuilder<T> withColors(String colors) {
        this.colors = colors;
        return this;
    }
    //ProductBuilder<T> withMaterial(String material) throws InvalidProductException;
    //ProductBuilder<T> withFurnitureType(String type) throws InvalidProductException;
    ///ProductBuilder<T> withGem(Boolean gem) throws InvalidProductException;

    Product build() {
        return new T(this);
    }
}
