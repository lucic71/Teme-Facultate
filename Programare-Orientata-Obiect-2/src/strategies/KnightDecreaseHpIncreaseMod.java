package strategies;

import hero.AbstractHero;

public class KnightDecreaseHpIncreaseMod implements AbstractStrategy{
    @Override
    public void algorithm(final AbstractHero hero) {
        int currentHp = hero.getHp();

        int newHp = (int) (currentHp * StrategyConstants.KNIGHT_DECREASE_HP);
        hero.setHp(newHp);

        hero.setStrategyMod(StrategyConstants.KNIGHT_INCREASE_MOD);
    }
}
