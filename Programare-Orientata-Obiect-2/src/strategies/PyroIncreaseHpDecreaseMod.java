package strategies;

import hero.AbstractHero;

public class PyroIncreaseHpDecreaseMod implements AbstractStrategy {
    @Override
    public void algorithm(final AbstractHero hero) {
        int currentHp = hero.getHp();

        int newHp = (int) (currentHp * StrategyConstants.PYRO_INCREASE_HP);
        hero.setHp(newHp);

        hero.setStrategyMod(StrategyConstants.PYRO_DECREASE_MOD);
    }
}
