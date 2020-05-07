package angel;

import hero.Knight;
import hero.Pyromancer;
import hero.Rogue;
import hero.Wizard;

public class Dracula extends AbstractAngel{

    public Dracula(final String name, final int posX, final int posY,
                   String identifier) {
        super(name, posX, posY, identifier);
    }

    @Override
    public void applyAngelAbility(final Pyromancer pyromancer) {
        pyromancer.setAngelMod(-AngelConstants.DRACULA_PYRO_MOD);
        pyromancer.setHp(pyromancer.getHp() - AngelConstants.DRACULA_PYRO_HP);
    }

    @Override
    public void applyAngelAbility(final Knight knight) {
        knight.setAngelMod(-AngelConstants.DRACULA_KNIGHT_MOD);
        knight.setHp(knight.getHp() - AngelConstants.DRACULA_KNIGHT_HP);
    }

    @Override
    public void applyAngelAbility(final Rogue rogue) {
        rogue.setAngelMod(-AngelConstants.DRACULA_ROGUE_MOD);
        rogue.setHp(rogue.getHp() - AngelConstants.DRACULA_ROGUE_HP);
    }

    @Override
    public void applyAngelAbility(final Wizard wizard) {
        wizard.setAngelMod(-AngelConstants.DRACULA_WIZARD_MOD);
        wizard.setHp(wizard.getHp() - AngelConstants.DRACULA_WIZARD_HP);
    }
}
