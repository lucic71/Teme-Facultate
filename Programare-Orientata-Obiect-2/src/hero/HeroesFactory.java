package hero;

public final class HeroesFactory {
    private static HeroesFactory instance = null;
    private static int heroCounter = 0;

    private HeroesFactory() {}

    public static HeroesFactory getInstance() {
        if (instance == null) {
            instance = new HeroesFactory();
        }

        return instance;
    }

    public AbstractHero getHero(final String heroName, final int posX, final int posY) {
        AbstractHero hero = null;

        switch (heroName) {
            case "W" :
                hero = new Wizard(HeroConstants.WIZARDINITHP, 0, posX, posY,
                        0, HeroConstants.WIZARDHPBONUS, HeroConstants.WIZARDTERRAIN,
                        HeroConstants.WIZARDTERRAINBONUS, heroName.charAt(0), heroCounter++);
                break;

            case "R" :
                hero = new Rogue(HeroConstants.ROGUEINITHP, 0, posX, posY,
                        0, HeroConstants.ROGUEHPBONUS, HeroConstants.ROGUETERRAIN,
                        HeroConstants.ROGUETERRAINBONUS, heroName.charAt(0), heroCounter++);
                break;

            case "K" :
                hero = new Knight(HeroConstants.KNIGHTINITHP, 0, posX, posY,
                        0, HeroConstants.KNIGHTHPBONUS, HeroConstants.KNIGHTTERRAIN,
                        HeroConstants.KNIGHTTERRAINBONUS, heroName.charAt(0), heroCounter++);
                break;

            case "P" :
                hero = new Pyromancer(HeroConstants.PYROINITHP, 0, posX, posY,
                            0, HeroConstants.PYROHPBONUS, HeroConstants.PYROTERRAIN,
                            HeroConstants.PYROTERRAINBONUS, heroName.charAt(0), heroCounter++);
                break;

            default :
                System.err.println("Hero does not exist! Aborting..");
                System.exit(-1);
                break;
        }

        return hero;
    }
}
