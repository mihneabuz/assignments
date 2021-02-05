package Main;

public class ProductFactory {
    public static Product buildProduct(ProductBuilder builder) throws InvalidProductException {
        switch (builder.getTYPE()) {
            case "PAINTING":
                return new Painting(builder.getID(),
                        builder.getName(),
                        builder.getStartPrice(),
                        builder.getArtistName(),
                        builder.getColors());
            case "FURNITURE":
                return new Furniture(builder.getID(),
                        builder.getName(),
                        builder.getStartPrice(),
                        builder.getFurnitureType(),
                        builder.getMaterial());
            case "JEWELLERY":
                return new Jewellery(builder.getID(),
                        builder.getName(),
                        builder.getStartPrice(),
                        builder.getMaterial(),
                        builder.isWithGem());
        }
        throw new InvalidProductException("Product type invalid");
    }
}
