package hero;

import java.util.HashMap;
import java.util.Map;

final class RaceModifierTable {
    static Map<Character, Float> getTable(final float rogueMod, final float knightMod,
                                          final float pyromancerMod, final float wizardMod,
                                          final AbstractHero hero) {
        final Map<Character, Float> table = new HashMap<>();

        table.put('R', rogueMod + (rogueMod != 1 ? hero.getAngelMod() : 0)
                + (rogueMod != 1 ? hero.getStrategyMod() : 0));

        table.put('K', knightMod + (knightMod != 1 ? hero.getAngelMod() : 0)
                + (knightMod != 1 ? hero.getStrategyMod() : 0));

        table.put('P', pyromancerMod + (pyromancerMod != 1 ? hero.getAngelMod() : 0)
                + (pyromancerMod != 1 ? hero.getStrategyMod() : 0));

        table.put('W', wizardMod + (wizardMod != 1 ? hero.getAngelMod() : 0)
                + (pyromancerMod != 1 ? hero.getStrategyMod() : 0));

        return table;
    }
}
