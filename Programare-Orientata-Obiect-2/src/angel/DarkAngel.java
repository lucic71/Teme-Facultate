package angel;

import hero.Knight;
import hero.Pyromancer;
import hero.Rogue;
import hero.Wizard;

public class DarkAngel extends AbstractAngel{
    public DarkAngel(final String name, final int posX, final int posY,
                        String identifier) {
            super(name, posX, posY, identifier);
    }

    @Override
    public void applyAngelAbility(final Pyromancer pyromancer) {
        pyromancer.setHp(pyromancer.getHp() - AngelConstants.DARKANGEL_PYRO_HP);
    }

    @Override
    public void applyAngelAbility(final Knight knight) {
        knight.setHp(knight.getHp() - AngelConstants.DARKANGEL_KNIGHT_HP);
    }

    @Override
    public void applyAngelAbility(final Rogue rogue) {
        rogue.setHp(rogue.getHp() - AngelConstants.DARKANGEL_ROGUE_HP);
    }

    @Override
    public void applyAngelAbility(final Wizard wizard) {
        wizard.setHp(wizard.getHp() - AngelConstants.DARKANGEL_WIZARD_HP);
    }
}
