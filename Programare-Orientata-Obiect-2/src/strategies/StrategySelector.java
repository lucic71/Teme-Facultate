package strategies;

import hero.AbstractHero;

public class StrategySelector {
    public static void selectStrategy(final AbstractHero hero) {
        float lower_limit = 0;
        float upper_limit = 0;

        switch (hero.getCharIdentifier()) {
            case 'K' :
                lower_limit = StrategyConstants.KNIGHT_LOWER_LIMIT;
                upper_limit = StrategyConstants.KNIGHT_UPPER_LIMIT;
                break;

            case 'P' :
                lower_limit = StrategyConstants.PYRO_LOWER_LIMIT;
                upper_limit = StrategyConstants.PYRO_UPPER_LIMIT;
                break;

            case 'R' :
                lower_limit = StrategyConstants.ROGUE_LOWER_LIMIT;
                upper_limit = StrategyConstants.ROGUE_UPPER_LIMIT;
                break;

            case 'W' :
                lower_limit = StrategyConstants.WIZARD_LOWER_LIMIT;
                upper_limit = StrategyConstants.WIZARD_UPPER_LIMIT;
                break;
        }

        float max_level_hp = hero.getMaxHp() + hero.getLevel() * hero.getLevelBonusHp();
        if (lower_limit * max_level_hp < (float)hero.getHp()
            && (float)hero.getHp() < upper_limit * max_level_hp) {
            hero.setAppliedStrategy(hero.getStrategies().get(0));
            return;
        }

        if ((float)hero.getHp() < lower_limit * max_level_hp) {
            hero.setAppliedStrategy(hero.getStrategies().get(1));
            return;
        }

        // default case where none of the strategies was chosen
        hero.setAppliedStrategy(null);
    }
}
