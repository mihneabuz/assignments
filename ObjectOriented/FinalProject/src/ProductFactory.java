public class ProductFactory {
    public static ProductBuilder buildProduct(String type) throws InvalidProductException
    {
        if (type.equals("PAINTING"))
            return new ProductBuilder<Painting>();
    //    else if (type.equals("FURNITURE"))
    //        return new FurnitureBuilder();
    //    else if (type.equals("JEWELLERY"))
    //        return new JewelleryBuilder();
        throw new InvalidProductException("Product type invalid");
    }
}
