package angel;

import hero.Knight;
import hero.Pyromancer;
import hero.Rogue;
import hero.Wizard;

public class Spawner extends AbstractAngel {
    public Spawner(final String name, final int posX, final int posY,
                      String identifier) {
        super(name, posX, posY, identifier);
    }
    @Override
    public void applyAngelAbility(final Pyromancer pyromancer) {
        pyromancer.setAlive(true);
        pyromancer.setHp(AngelConstants.SPAWNER_PYRO_HP);
    }

    @Override
    public void applyAngelAbility(final Knight knight) {
        knight.setAlive(true);
        knight.setHp(AngelConstants.SPAWNER_KNIGHT_HP);
    }

    @Override
    public void applyAngelAbility(final Rogue rogue) {
        rogue.setAlive(true);
        rogue.setHp(AngelConstants.SPAWNER_ROGUE_HP);
    }

    @Override
    public void applyAngelAbility(final Wizard wizard) {
        wizard.setAlive(true);
        wizard.setHp(AngelConstants.SPAWNER_WIZARD_HP);
    }
}
