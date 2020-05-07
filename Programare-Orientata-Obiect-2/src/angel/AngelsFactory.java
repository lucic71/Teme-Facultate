package angel;

public final class AngelsFactory {
    private static AngelsFactory instance = null;

    private AngelsFactory() {}

    public static AngelsFactory getInstance() {
        if (instance == null) {
            instance = new AngelsFactory();
        }

        return instance;
    }

    public AbstractAngel getAngel(final String angelName, final int posX, final int posY) {
        AbstractAngel angel = null;

        switch (angelName) {
            case "DamageAngel" :
                angel = new DamageAngel(angelName, posX, posY, "good");
                break;

            case "DarkAngel" :
                angel = new DarkAngel(angelName, posX, posY, "bad");
                break;

            case "Dracula" :
                angel = new Dracula(angelName, posX, posY, "bad");
                break;

            case "GoodBoy" :
                angel = new GoodBoy(angelName, posX, posY, "good");
                break;

            case "LevelUpAngel" :
                angel = new LevelUpAngel(angelName, posX, posY, "good");
                break;

            case "LifeGiver" :
                angel = new LifeGiver(angelName, posX, posY, "good");
                break;

            case "SmallAngel" :
                angel = new SmallAngel(angelName, posX, posY, "good");
                break;

            case "Spawner" :
                angel = new Spawner(angelName, posX, posY, "good");
                break;

            case "TheDoomer" :
                angel = new TheDoomer(angelName, posX, posY, "bad");
                break;

            case "XPAngel" :
                angel = new XPAngel(angelName, posX, posY, "good");
                break;

            default:
                System.err.println("Angel does not exist! Aborting..");
                System.exit(-1);
                break;
        }

        return angel;
    }
}
