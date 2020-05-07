package hero;

import angel.AbstractAngel;
import strategies.KnightDecreaseHpIncreaseMod;
import strategies.KnightIncreaseHpDecreaseMod;
import strategies.StrategySelector;

import java.util.Map;

public final class Knight extends AbstractHero{

    public Knight(final int hp, final int xp, final int posX, final int posY, final int level,
                  final int levelBonusHp, final Character terrain, final float terrainBonus,
                  final Character identifier, final int numberIdentifier) {
        super(hp, xp, posX, posY, level, levelBonusHp, terrain, terrainBonus,
                identifier, numberIdentifier);

        addStrategy(new KnightDecreaseHpIncreaseMod());
        addStrategy(new KnightIncreaseHpDecreaseMod());
    }

    // execute
    @Override
    public int firstAbility(final AbstractHero victim) {
        int baseDamage = HeroConstants.EXECUTE_BASE_DAMAGE;
        int levelBonusDamage = HeroConstants.EXECUTE_LVLBONUS_DAMAGE * this.getLevel();

        // base damage
        float totalDamageFloat = baseDamage + levelBonusDamage;

        // compute hp limit for critical damage
        int theoreticalMaxHp = victim.getMaxHp() + victim.getLevel()
                * victim.getLevelBonusHp();

        // 1% per level
        float hpLimitLevelBonusPercent = HeroConstants.EXECUTE_BONUS_HP_LIMIT
                * this.getLevel();

        float totalPercent = HeroConstants.EXECUTE_INITIAL_HP_LIMIT
                + hpLimitLevelBonusPercent;

        // if the percent is greater than 40%
        if (totalPercent > HeroConstants.EXECUTE_UPPER_LIMIT) {
            totalPercent = HeroConstants.EXECUTE_UPPER_LIMIT;
        }

        // (20% + 1% * level) * theoretical max hp
        float hpLimit = totalPercent * theoreticalMaxHp;

        if (victim.getHp() < Math.round(hpLimit)) {
            // land bonus
            if (this.getCurrentTerrain() == this.getSpecialTerrain()) {
                totalDamageFloat = totalDamageFloat * this.getTerrainBonus();
            }

            // damage without race mod
            this.setTotalDamageWithoutRaceMod(Math.round(totalDamageFloat));
            return victim.getHp();
        }

        // compute normal damage

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
                HeroConstants.EXECUTE_ROGUE_MOD,
                HeroConstants.EXECUTE_KNIGHT_MOD,
                HeroConstants.EXECUTE_PYROMANCER_MOD,
                HeroConstants.EXECUTE_WIZARD_MOD, this);

        totalDamage = Math.round(totalDamage * modTable.get(victim.getCharIdentifier()));

        return totalDamage;
    }

    // slam
    @Override
    public int secondAbility(final AbstractHero victim) {
        int baseDamage = HeroConstants.SLAM_BASE_DAMAGE;
        int levelBonusDamage = this.getLevel() * HeroConstants.SLAM_LVLBONUS_DAMAGE;

        // base damage
        float totalDamageFloat = baseDamage + levelBonusDamage;

        // land bonus
        if (this.getCurrentTerrain() == this.getSpecialTerrain()) {
            totalDamageFloat = totalDamageFloat * this.getTerrainBonus();
        }

        // rounded after land bonus was applied
        int totalDamage = Math.round(totalDamageFloat);

        // damage without race mod
        this.setTotalDamageWithoutRaceMod(totalDamage
                + this.getTotalDamageWithoutRaceMod());


        // race mod
        Map<Character, Float> modTable = RaceModifierTable.getTable(
                HeroConstants.SLAM_ROGUE_MOD,
                HeroConstants.SLAM_KNIGHT_MOD,
                HeroConstants.SLAM_PYROMANCER_MOD,
                HeroConstants.SLAM_WIZARD_MOD, this);

        totalDamage = Math.round(totalDamageFloat * modTable.get(victim.getCharIdentifier()));

        // frozen
        victim.setFrozen(true);
        victim.setFrozenTime(1);

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
