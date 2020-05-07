package hero;

import strategies.AbstractStrategy;

import java.util.ArrayList;
import java.util.List;

public abstract class AbstractHero implements
        VisitableHero, VisitorHero, AngelVisitableHero{
    // player stats
    private int maxHp;
    private boolean alive;

    private int hp;
    private int xp;

    private int posX;
    private int posY;

    private int level;
    private int levelBonusHp;

    private Character specialTerrain;
    private float terrainBonus;
    private Character currentTerrain;

    private Character charIdentifier;
    private Integer numberIdentifier;

    private int overTimeRounds;
    private int overTimeDamage;

    private boolean frozen;
    private int frozenTime;

    private int totalDamageWithoutRaceMod;
    private float angelMod;

    // player abilities
    public abstract int firstAbility(AbstractHero victim);
    public abstract int secondAbility(AbstractHero victim);

    // player strategies
    private List<AbstractStrategy> strategies;
    private AbstractStrategy appliedStrategy;

    public abstract void applyStrategy();

    private float strategyMod;

    // constructor
    public AbstractHero(final int hp, final int xp, final int posX, final int posY, final int level,
                        final int levelBonusHp, final Character terrain, final float terrainBonus,
                        final Character charIdentifier, final int numberIdentifier) {
        this.maxHp = hp;
        this.alive = true;

        this.hp = hp;
        this.xp = xp;
        this.posX = posX;
        this.posY = posY;
        this.level = level;
        this.levelBonusHp = levelBonusHp;
        this.specialTerrain = terrain;
        this.terrainBonus = terrainBonus;
        this.charIdentifier = charIdentifier;
        this.numberIdentifier = numberIdentifier;

        this.overTimeDamage = 0;
        this.overTimeRounds = 0;
        this.frozen = false;
        this.frozenTime = 0;
        this.angelMod = 0;

        this.totalDamageWithoutRaceMod = 0;

        this.strategies = new ArrayList<>();
    }


    // getters and setters
    public final int getHp() {
        return hp;
    }

    public final void setHp(final int hp) {
        // guard for killing a hero
        if (hp < 0) {
           this.hp = 0;
           this.setAlive(false);

           return;
        }

        this.hp = hp;
    }

    public final int getXp() {
        return xp;
    }

    public final void setXp(final int xp) {
        this.xp = xp;
    }

    public final int getPosX() {
        return posX;
    }

    public final void setPosX(final int posX) {
        this.posX = posX;
    }

    public final int getPosY() {
        return posY;
    }

    public final void setPosY(final int posY) {
        this.posY = posY;
    }

    public final int getLevel() {
        return level;
    }

    public final void setLevel(final int level) {
        this.level = level;
    }

    public final int getLevelBonusHp() {
        return levelBonusHp;
    }

    public final void setLevelBonusHp(final int levelBonusHp) {
        this.levelBonusHp = levelBonusHp;
    }

    final Character getSpecialTerrain() {
        return specialTerrain;
    }

    public final void setSpecialTerrain(final char specialTerrain) {
        this.specialTerrain = specialTerrain;
    }

    final float getTerrainBonus() {
        return terrainBonus;
    }

    public final void setTerrainBonus(final int terrainBonus) {
        this.terrainBonus = terrainBonus;
    }

    public final Character getCharIdentifier() {
        return charIdentifier;
    }

    final Character getCurrentTerrain() {
        return currentTerrain;
    }

    public final void setCurrentTerrain(final Character currentTerrain) {
        this.currentTerrain = currentTerrain;
    }

    public final int getOverTimeRounds() {
        return overTimeRounds;
    }

    public final void setOverTimeRounds(final int overTimeRounds) {
        this.overTimeRounds = overTimeRounds;
    }

    public final int getOverTimeDamage() {
        return overTimeDamage;
    }

    public final void setOverTimeDamage(final int overTimeDamage) {
        this.overTimeDamage = overTimeDamage;
    }

    public final boolean isFrozen() {
        return frozen;
    }

    public final void setFrozen(final boolean frozen) {
        this.frozen = frozen;
    }

    public final int getFrozenTime() {
        return frozenTime;
    }

    public final void setFrozenTime(final int frozenTime) {
        this.frozenTime = frozenTime;
    }

    public final int getMaxHp() {
        return maxHp;
    }

    public final void setMaxHp(final int maxHp) {
        this.maxHp = maxHp;
    }

    final int getTotalDamageWithoutRaceMod() {
        return totalDamageWithoutRaceMod;
    }

    final void setTotalDamageWithoutRaceMod(final int totalDamageWithoutRaceMod) {
        this.totalDamageWithoutRaceMod = totalDamageWithoutRaceMod;
    }

    public final boolean isAlive() {
        return alive;
    }

    public final void setAlive(final boolean alive) {
        this.alive = alive;
    }

    public float getAngelMod() {
        return angelMod;
    }

    public void setAngelMod(float angelMod) {
        this.angelMod += angelMod;
    }

    public void addStrategy(AbstractStrategy strategy) {
        strategies.add(strategy);
    }

    public List<AbstractStrategy> getStrategies() {
        return strategies;
    }

    public AbstractStrategy getAppliedStrategy() {
        return appliedStrategy;
    }

    public void setAppliedStrategy(AbstractStrategy appliedStrategy) {
        this.appliedStrategy = appliedStrategy;
    }

    public float getStrategyMod() {
        return strategyMod;
    }

    public void setStrategyMod(float strategyMod) {
        this.strategyMod = strategyMod;
    }

    public Integer getNumberIdentifier() {
        return numberIdentifier;
    }

    @Override
    public String toString() {
        return "AbstractHero{" +
                "maxHp=" + maxHp +
                ", hp=" + hp +
                ", posX=" + posX +
                ", posY=" + posY +
                ", level=" + level +
                ", numberIndentifier=" + numberIdentifier +
                ", overTimeRounds=" + overTimeRounds +
                ", overTimeDamage=" + overTimeDamage +
                ", frozen=" + frozen +
                ", frozenTime=" + frozenTime +
                ", appliedStrategy=" + appliedStrategy +
                ", strategyMod=" + strategyMod +
                '}';
    }
}
