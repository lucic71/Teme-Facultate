package strategies;

import hero.AbstractHero;

public class WizardIncreaseHpDecreaseMod implements AbstractStrategy {
    @Override
    public void algorithm(AbstractHero hero) {
        int currentHp = hero.getHp();

        int newHp = (int) (currentHp * StrategyConstants.WIZARD_INCREASE_HP);
        hero.setHp(newHp);

        hero.setStrategyMod(StrategyConstants.WIZARD_DECREASE_MOD);
    }
}
