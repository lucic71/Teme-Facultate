package angel;

import hero.Knight;
import hero.Pyromancer;
import hero.Rogue;
import hero.Wizard;

public class DamageAngel extends AbstractAngel {
    public DamageAngel(final String name, final int posX, final int posY,
                       String identifier) {
        super(name, posX, posY, identifier);
    }

    @Override
    public void applyAngelAbility(final Pyromancer pyromancer) {
        pyromancer.setAngelMod(AngelConstants.DAMAGEANGEL_PYRO_MOD);
    }

    @Override
    public void applyAngelAbility(final Knight knight) {
        knight.setAngelMod(AngelConstants.DAMAGEANGEL_KNIGHT_MOD);
    }

    @Override
    public void applyAngelAbility(final Rogue rogue) {
        rogue.setAngelMod(AngelConstants.DAMAGEANGEL_ROGUE_MOD);
    }

    @Override
    public void applyAngelAbility(final Wizard wizard) {
        wizard.setAngelMod(AngelConstants.DAMAGEANGEL_WIZARD_MOD);
    }
}
