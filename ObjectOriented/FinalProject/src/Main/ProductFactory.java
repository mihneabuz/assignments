package Main;

public class ProductFactory {
    public static Product buildProduct(ProductBuilder builder) throws InvalidProductException {
        if (builder.getTYPE().equals("PAINTING"))
            return new Painting(builder.getID(),
                                builder.getName(),
                                builder.getStartPrice(),
                                builder.getArtistName(),
                                builder.getColors());
        else if (builder.getTYPE().equals("FURNITURE"))
            return new Furniture(builder.getID(),
                                 builder.getName(),
                                 builder.getStartPrice(),
                                 builder.getFurnitureType(),
                                 builder.getMaterial());
        else if (builder.getTYPE().equals("JEWELLERY"))
            return new Jewellery(builder.getID(),
                                 builder.getName(),
                                 builder.getStartPrice(),
                                 builder.getMaterial(),
                                 builder.isWithGem());
        throw new InvalidProductException("Main.Product type invalid");
    }
}
