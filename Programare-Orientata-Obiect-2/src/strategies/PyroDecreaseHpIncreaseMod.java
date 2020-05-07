package strategies;

import hero.AbstractHero;

public class PyroDecreaseHpIncreaseMod implements AbstractStrategy {

    @Override
    public void algorithm(final AbstractHero hero) {
        int currentHp = hero.getHp();

        int newHp = (int) (currentHp * StrategyConstants.PYRO_DECREASE_HP);
        hero.setHp(newHp);

        hero.setStrategyMod(StrategyConstants.PYRO_INCREASE_MOD);
    }
}
