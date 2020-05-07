package angel;

import hero.Knight;
import hero.Pyromancer;
import hero.Rogue;
import hero.Wizard;

public class GoodBoy extends AbstractAngel {
    public GoodBoy(final String name, final int posX, final int posY,
                    String identifier) {
            super(name, posX, posY, identifier);
    }

    @Override
    public void applyAngelAbility(final Pyromancer pyromancer) {
        pyromancer.setAngelMod(AngelConstants.GOODBOY_PYRO_MOD);

        int levelMaxHp = pyromancer.getMaxHp() + pyromancer.getLevel() * pyromancer.getLevelBonusHp();

        if (pyromancer.getHp() >= levelMaxHp) {
            return;
        }

        int newHp = pyromancer.getHp() + AngelConstants.GOODBOY_PYRO_HP;
        if (newHp >= levelMaxHp) {
            newHp = levelMaxHp;
        }
        pyromancer.setHp(newHp);
    }

    @Override
    public void applyAngelAbility(final Knight knight) {
        knight.setAngelMod(AngelConstants.GOODBOY_KNIGHT_MOD);

        int levelMaxHp = knight.getMaxHp() + knight.getLevel() * knight.getLevelBonusHp();

        if (knight.getHp() >= levelMaxHp) {
            return;
        }

        int newHp = knight.getHp() + AngelConstants.GOODBOY_KNIGHT_HP;
        if (newHp >= levelMaxHp) {
            newHp = levelMaxHp;
        }
        knight.setHp(newHp);
    }

    @Override
    public void applyAngelAbility(final Rogue rogue) {
        rogue.setAngelMod(AngelConstants.GOODBOY_ROGUE_MOD);

        int levelMaxHp = rogue.getMaxHp() + rogue.getLevel() * rogue.getLevelBonusHp();

        if (rogue.getHp() >= levelMaxHp) {
            return;
        }

        int newHp = rogue.getHp() + AngelConstants.GOODBOY_ROGUE_HP;
        if (newHp >= levelMaxHp) {
            newHp = levelMaxHp;
        }
        rogue.setHp(newHp);
    }

    @Override
    public void applyAngelAbility(final Wizard wizard) {
        wizard.setAngelMod(AngelConstants.GOODBOY_WIZARD_MOD);

        int levelMaxHp = wizard.getMaxHp() + wizard.getLevel() * wizard.getLevelBonusHp();

        if (wizard.getHp() >= levelMaxHp) {
            return;
        }

        int newHp = wizard.getHp() + AngelConstants.GOODBOY_WIZARD_HP;
        if (newHp >= levelMaxHp) {
            newHp = levelMaxHp;
        }
        wizard.setHp(newHp);
    }
}

