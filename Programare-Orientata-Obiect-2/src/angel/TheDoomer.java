package angel;

import engine.FightingEngine;
import hero.Knight;
import hero.Pyromancer;
import hero.Rogue;
import hero.Wizard;

public class TheDoomer extends AbstractAngel {
    private FightingEngine fightingEngine = new FightingEngine();

    public TheDoomer(final String name, final int posX, final int posY,
                   String identifier) {
        super(name, posX, posY, identifier);
    }

    @Override
    public void applyAngelAbility(final Pyromancer pyromancer) {
        fightingEngine.purgeHero(pyromancer);
    }

    @Override
    public void applyAngelAbility(final Knight knight) {
        fightingEngine.purgeHero(knight);
    }

    @Override
    public void applyAngelAbility(final Rogue rogue) {
        fightingEngine.purgeHero(rogue);
    }

    @Override
    public void applyAngelAbility(final Wizard wizard) {
        fightingEngine.purgeHero(wizard);
    }
}
