package angel;

import engine.FightingEngine;
import great_magician.GreatMagician;
import hero.*;
import main.Main;

public class LevelUpAngel extends AbstractAngel {
    private FightingEngine fightingEngine = new FightingEngine();
    private GreatMagician greatMagician = new GreatMagician(Main.gameOutput);

    public LevelUpAngel(final String name, final int posX, final int posY,
                   String identifier) {
        super(name, posX, posY, identifier);
        greatMagician.addLevelUpSubject(fightingEngine);
    }

    @Override
    public void applyAngelAbility(final Pyromancer pyromancer) {
        levelUpHero(pyromancer);
        pyromancer.setAngelMod(AngelConstants.LEVELUPANGEL_PYRO_MOD);
    }

    @Override
    public void applyAngelAbility(final Knight knight) {
        levelUpHero(knight);
        knight.setAngelMod(AngelConstants.LEVELUPANGEL_KNIGHT_MOD);
    }

    @Override
    public void applyAngelAbility(final Rogue rogue) {
        levelUpHero(rogue);
        rogue.setAngelMod(AngelConstants.LEVELUPANGEL_ROGUE_MOD);
    }

    @Override
    public void applyAngelAbility(final Wizard wizard) {
        levelUpHero(wizard);
        wizard.setAngelMod(AngelConstants.LEVELUPANGEL_WIZARD_MOD);
    }

    private void levelUpHero(final AbstractHero hero) {
        // if xp is lower than 250 then set xp to 250 and level up
        if (hero.getXp() < HeroConstants.LEVEL_GROWTH_BASE) {
            hero.setXp(HeroConstants.LEVEL_GROWTH_BASE);
            fightingEngine.executeLevelGrowth(hero);
        } else {
            // find next multiple of 50 and level up
            int neededXp = ((hero.getXp() + HeroConstants.LEVEL_GROWTH_EXPONENT - 1)
                    / HeroConstants.LEVEL_GROWTH_EXPONENT) * HeroConstants.LEVEL_GROWTH_EXPONENT;
            if (neededXp == hero.getXp()) {
                neededXp += HeroConstants.LEVEL_GROWTH_EXPONENT;
            }
            hero.setXp(neededXp);
            fightingEngine.executeLevelGrowth(hero);
        }
    }
}

