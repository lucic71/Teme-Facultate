package angel;

import engine.FightingEngine;
import great_magician.GreatMagician;
import hero.Knight;
import hero.Pyromancer;
import hero.Rogue;
import hero.Wizard;
import main.Main;

public class XPAngel extends AbstractAngel {
    private FightingEngine fightingEngine = new FightingEngine();
    private GreatMagician greatMagician = new GreatMagician(Main.gameOutput);

    public XPAngel(final String name, final int posX, final int posY,
                     String identifier) {
        super(name, posX, posY, identifier);
        greatMagician.addLevelUpSubject(fightingEngine);
    }
    @Override
    public void applyAngelAbility(final Pyromancer pyromancer) {
        pyromancer.setXp(pyromancer.getXp() + AngelConstants.XPANGEL_PYRO_XP);
        fightingEngine.executeLevelGrowth(pyromancer);
    }

    @Override
    public void applyAngelAbility(final Knight knight) {
        knight.setXp(knight.getXp() + AngelConstants.XPANGEL_KNIGHT_XP);
        fightingEngine.executeLevelGrowth(knight);
    }

    @Override
    public void applyAngelAbility(final Rogue rogue) {
        rogue.setXp(rogue.getXp() + AngelConstants.XPANGEL_ROGUE_XP);
        fightingEngine.executeLevelGrowth(rogue);
    }

    @Override
    public void applyAngelAbility(final Wizard wizard) {
        wizard.setXp(wizard.getXp() + AngelConstants.XPANGEL_WIZARD_XP);
        fightingEngine.executeLevelGrowth(wizard);
    }
}
