package hero;

import angel.AbstractAngel;
import strategies.PyroDecreaseHpIncreaseMod;
import strategies.PyroIncreaseHpDecreaseMod;
import strategies.StrategySelector;

import java.util.Map;

public final class Pyromancer extends AbstractHero {

    public Pyromancer(final int hp, final int xp, final int posX, final int posY, final int level,
                      final int levelBonusHp, final Character terrain, final float terrainBonus,
                      final Character identifier, final int numberIdentifier) {
        super(hp, xp, posX, posY, level, levelBonusHp, terrain, terrainBonus,
                identifier, numberIdentifier);

        addStrategy(new PyroDecreaseHpIncreaseMod());
        addStrategy(new PyroIncreaseHpDecreaseMod());
    }

    // fireblast
    @Override
    public int firstAbility(final AbstractHero victim) {
        int baseDamage = HeroConstants.FIREBLAST_BASE_DAMAGE;
        int levelBonusDamage = this.getLevel() * HeroConstants.FIREBLAST_LVLBONUS_DAMAGE;

        // base damage
        float totalDamageFloat = baseDamage + levelBonusDamage;

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
                HeroConstants.FIREBLAST_ROGUE_MOD,
                HeroConstants.FIREBLAST_KNIGHT_MOD,
                HeroConstants.FIREBLAST_PYROMANCER_MOD,
                HeroConstants.FIREBLAST_WIZARD_MOD, this);

        totalDamage = Math.round(totalDamage * modTable.get(victim.getCharIdentifier()));

        return totalDamage;
    }

    // ignite
    @Override
    public int secondAbility(final AbstractHero victim) {

        int baseDamage = HeroConstants.IGNITE_BASE_DAMAGE;
        int levelBonusDamage = this.getLevel() * HeroConstants.IGNITE_LVLBONUS_DAMAGE;

        // base damage
        float totalDamageFloat = baseDamage + levelBonusDamage;

        // overtime damage
        float periodicDamageFloat = HeroConstants.IGNITE_OVERTIME_DAMAGE
                + HeroConstants.IGNITE_OVERTIME_DAMAGE_LVLBONUS * this.getLevel();

        // land bonus
        if (this.getCurrentTerrain() == this.getSpecialTerrain()) {
            totalDamageFloat = totalDamageFloat * this.getTerrainBonus();
            periodicDamageFloat = periodicDamageFloat * this.getTerrainBonus();
        }

        // rounded after land bonus was applied
        int totalDamage = Math.round(totalDamageFloat);
        int periodicDamage = Math.round(periodicDamageFloat);

        // damage without race mod
        this.setTotalDamageWithoutRaceMod(totalDamage
                + this.getTotalDamageWithoutRaceMod());

        // race mod
        Map<Character, Float> modTable = RaceModifierTable.getTable(HeroConstants.IGNITE_ROGUE_MOD,
                HeroConstants.IGNITE_KNIGHT_MOD,
                HeroConstants.IGNITE_PYROMANCER_MOD,
                HeroConstants.IGNITE_WIZARD_MOD, this);

        totalDamage = Math.round(totalDamage * modTable.get(victim.getCharIdentifier()));
        periodicDamage = Math.round(periodicDamage * modTable.get(victim.getCharIdentifier()));

        // overtime damage
        victim.setOverTimeDamage(periodicDamage);
        victim.setOverTimeRounds(HeroConstants.IGNITE_OVERTIME_ROUNDS);

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
