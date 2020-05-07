package angel;

public abstract class AbstractAngel implements VisitorAngel{
    private String name;
    private int posX;
    private int posY;
    private String identifier;

    public AbstractAngel(final String name, final int posX, final int posY,
                         String identifier) {
        this.name = name;
        this.posX = posX;
        this.posY = posY;
        this.identifier = identifier;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getPosX() {
        return posX;
    }

    public void setPosX(int posX) {
        this.posX = posX;
    }

    public int getPosY() {
        return posY;
    }

    public void setPosY(int posY) {
        this.posY = posY;
    }

    public String getIdentifier() {
        return identifier;
    }

}
