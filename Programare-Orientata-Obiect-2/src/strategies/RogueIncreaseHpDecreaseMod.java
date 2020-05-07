package strategies;

import hero.AbstractHero;

public class RogueIncreaseHpDecreaseMod implements AbstractStrategy {
    @Override
    public void algorithm(AbstractHero hero) {
        int currentHp = hero.getHp();

        int newHp = (int) (currentHp * StrategyConstants.ROGUE_INCREASE_HP);
        hero.setHp(newHp);

        hero.setStrategyMod(StrategyConstants.ROGUE_DECREASE_MOD);
    }
}
