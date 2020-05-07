package angel;

import hero.Knight;
import hero.Pyromancer;
import hero.Rogue;
import hero.Wizard;

public class SmallAngel extends AbstractAngel {
    public SmallAngel(final String name, final int posX, final int posY,
                     String identifier) {
        super(name, posX, posY, identifier);
    }

    @Override
    public void applyAngelAbility(final Pyromancer pyromancer) {
        pyromancer.setAngelMod(AngelConstants.SMALLANGEL_PYRO_MOD);
        pyromancer.setHp(pyromancer.getHp() + AngelConstants.SMALLANGEL_PYRO_HP);
    }

    @Override
    public void applyAngelAbility(final Knight knight) {
        knight.setAngelMod(AngelConstants.SMALLANGEL_KNIGHT_MOD);
        knight.setHp(knight.getHp() + AngelConstants.SMALLANGEL_KNIGHT_HP);
    }

    @Override
    public void applyAngelAbility(final Rogue rogue) {
        rogue.setAngelMod(AngelConstants.SMALLANGEL_ROGUE_MOD);
        rogue.setHp(rogue.getHp() + AngelConstants.SMALLANGEL_ROGUE_HP);
    }

    @Override
    public void applyAngelAbility(final Wizard wizard) {
        wizard.setAngelMod(AngelConstants.SMALLANGEL_WIZARD_MOD);
        wizard.setHp(wizard.getHp() + AngelConstants.SMALLANGEL_WIZARD_HP);
    }
}

