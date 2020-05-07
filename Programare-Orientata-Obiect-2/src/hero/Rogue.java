package hero;

import angel.AbstractAngel;
import strategies.RogueDecreaseHpIncreaseMod;
import strategies.RogueIncreaseHpDecreaseMod;
import strategies.StrategySelector;

import java.util.Map;

public final class Rogue extends AbstractHero {
    private int multipleHitDamage;

    public Rogue(final int hp, final int xp, final int posX, final int posY, final int level,
                 final int levelBonusHp, final Character terrain, final float terrainBonus,
                 final Character identifier, final int numberIdentifier) {
        super(hp, xp, posX, posY, level, levelBonusHp, terrain, terrainBonus,
                identifier, numberIdentifier);

        multipleHitDamage = 0;

        addStrategy(new RogueDecreaseHpIncreaseMod());
        addStrategy(new RogueIncreaseHpDecreaseMod());
    }

    // backstab
    @Override
    public int firstAbility(final AbstractHero victim) {
        int baseDamage = HeroConstants.BACKSTAB_BASE_DAMAGE;
        int levelBonusDamage = this.getLevel() * HeroConstants.BACKSTAB_LVLBONUS_DAMAGE;

        // base damage
        float totalDamageFloat = baseDamage + levelBonusDamage;

        // critical damage
        if (this.multipleHitDamage % HeroConstants.CRITICAL_HIT_CYCLE == 0
                && this.getCurrentTerrain() == this.getSpecialTerrain()) {
            totalDamageFloat = totalDamageFloat * HeroConstants.CRITICAL_HIT_BONUS;
        }
        this.multipleHitDamage++;

        // land bonus
        if (this.getCurrentTerrain() == this.getSpecialTerrain()) {
            totalDamageFloat = totalDamageFloat * this.getTerrainBonus();
        }

        // rounded after land bonus was applied
        int totalDamage = Math.round(totalDamageFloat);

        // damage without race mod
        this.setTotalDamageWithoutRaceMod(totalDamage);

        // race mod
        Map<Character, Float> modTable = RaceModifierTable.getTable(
                HeroConstants.BACKSTAB_ROGUE_MOD,
                HeroConstants.BACKSTAB_KNIGHT_MOD,
                HeroConstants.BACKSTAB_PYROMANCER_MOD,
                HeroConstants.BACKSTAB_WIZARD_MOD, this);

        totalDamage = Math.round(totalDamage * modTable.get(victim.getCharIdentifier()));

        return totalDamage;
    }

    // paralysis
    @Override
    public int secondAbility(final AbstractHero victim) {
        int baseDamage = HeroConstants.PARALYSIS_BASE_DAMAGE;
        int levelBonusDamage = this.getLevel() * HeroConstants.PARALYSIS_LVLBONUS_DAMAGE;
        // base damage
        float totalDamageFloat = baseDamage + levelBonusDamage;

        // land bonus
        if (this.getCurrentTerrain() == this.getSpecialTerrain()) {
            totalDamageFloat = totalDamageFloat * this.getTerrainBonus();
        }

        int totalDamage = Math.round(totalDamageFloat);

        // damage without race mod
        this.setTotalDamageWithoutRaceMod(totalDamage
                + this.getTotalDamageWithoutRaceMod());

        // race mod
        Map<Character, Float> modTable = RaceModifierTable.getTable(
                HeroConstants.PARALYSIS_ROGUE_MOD,
                HeroConstants.PARALYSIS_KNIGHT_MOD,
                HeroConstants.PARALYSIS_PYROMANCER_MOD,
                HeroConstants.PARALYSIS_WIZARD_MOD, this);

        totalDamage = Math.round(totalDamageFloat * modTable.get(victim.getCharIdentifier()));

        // overtime damage and frozen
        victim.setOverTimeDamage(totalDamage);
        victim.setFrozen(true);
        if (this.getCurrentTerrain() == 'W' && victim.getCurrentTerrain() == 'W') {
            victim.setOverTimeRounds(HeroConstants.PARALYSIS_EXTENDED_OVERTIME_ROUNDS);
            victim.setFrozenTime(HeroConstants.PARALYSIS_EXTENDED_OVERTIME_ROUNDS);
        } else {
            victim.setOverTimeRounds(HeroConstants.PARALYSIS_NORMAL_OVERTIME_ROUNDS);
            victim.setFrozenTime(HeroConstants.PARALYSIS_NORMAL_OVERTIME_ROUNDS);
        }

        return totalDamage;
    }

    @Override
    public void applyStrategy() {
        StrategySelector.selectStrategy(this);
        if (getAppliedStrategy() != null) {
            getAppliedStrategy().algorithm(this);
        }
    }

    @Override
    public int acceptDamage(final VisitorHero v) {
        return v.giveDamage(this);
    }

    @Override
    public int giveDamage(final Pyromancer pyromancer) {
        return this.firstAbility(pyromancer)
                + this.secondAbility(pyromancer);
    }

    @Override
    public int giveDamage(final Knight knight) {
        return this.firstAbility(knight)
                + this.secondAbility(knight);
    }

    @Override
    public int giveDamage(final Rogue rogue) {
        return this.firstAbility(rogue)
                + this.secondAbility(rogue);
    }

    @Override
    public int giveDamage(final Wizard wizard) {

        return this.firstAbility(wizard)
                + this.secondAbility(wizard);
    }

    @Override
    public void acceptAngelAbility(final AbstractAngel angel) {
        angel.applyAngelAbility(this);
    }

}
