package hero;

import angel.AbstractAngel;
import strategies.StrategySelector;
import strategies.WizardDecreaseHpIncreaseMod;
import strategies.WizardIncreaseHpDecreaseMod;

import java.util.Map;

public final class Wizard extends AbstractHero {

    public Wizard(final int hp, final int xp, final int posX, final int posY, final int level,
                  final int levelBonusHp, final Character terrain, final float terrainBonus,
                  final Character identifier, final int numberIdentifier) {
        super(hp, xp, posX, posY, level, levelBonusHp, terrain, terrainBonus,
                identifier, numberIdentifier);

        addStrategy(new WizardDecreaseHpIncreaseMod());
        addStrategy(new WizardIncreaseHpDecreaseMod());
    }

    // drain
    @Override
    public int firstAbility(final AbstractHero victim) {
        final float damagePercent = HeroConstants.DRAIN_BASE_DAMAGE;
        final float levelBonusPercent = HeroConstants.DRAIN_LVLBONUS_DAMAGE;

        // base percent
        float totalPercent = damagePercent + levelBonusPercent * this.getLevel();

        // land bonus
        if (this.getCurrentTerrain() == this.getSpecialTerrain()) {
            totalPercent = totalPercent * this.getTerrainBonus();
        }

        // race mod
        Map<Character, Float> modTable = RaceModifierTable.getTable(
                HeroConstants.DRAIN_ROGUE_MOD,
                HeroConstants.DRAIN_KNIGHT_MOD,
                HeroConstants.DRAIN_PYROMANCER_MOD,
                HeroConstants.DRAIN_WIZARD_MOD, this
        );

        totalPercent = totalPercent * modTable.get(victim.getCharIdentifier());

        int victimMaxLevelHp = victim.getMaxHp() + victim.getLevelBonusHp()
                * victim.getLevel();

        // percent * min(0.3*max_hp, curr_hp)
        int totalDamage = Math.round(totalPercent * Math.min(
                HeroConstants.DRAIN_MAX_HP_PERCENT * victimMaxLevelHp, victim.getHp()));

        return totalDamage;
    }

    // deflect
    @Override
    public int secondAbility(final AbstractHero victim) {
        final float damagePercent = HeroConstants.DEFLECT_BASE_DAMAGE;
        final float levelBonusPercent = HeroConstants.DEFLECT_LVLBONUS_DAMAGE;

        // base percent
        float totalPercent = damagePercent + levelBonusPercent * this.getLevel();

        if (totalPercent > HeroConstants.DEFLECT_MAX_BONUS_PERCENT) {
            totalPercent = HeroConstants.DEFLECT_MAX_BONUS_PERCENT;
        }

        // land bonus
        if (this.getCurrentTerrain() == this.getSpecialTerrain()) {
            totalPercent = totalPercent * this.getTerrainBonus();
        }

        // race mod
        Map<Character, Float> modTable = RaceModifierTable.getTable(
                HeroConstants.DEFLECT_ROGUE_MOD,
                HeroConstants.DEFLECT_KNIGHT_MOD,
                HeroConstants.DEFLECT_PYROMANCER_MOD,
                HeroConstants.DEFLECT_WIZARD_MOD, this
        );

        totalPercent = totalPercent * modTable.get(victim.getCharIdentifier());

        // total damage without race modifiers
        int victimDamage = victim.getTotalDamageWithoutRaceMod();
        int totalDamage = Math.round(victimDamage * totalPercent);

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
