package engine;

import great_magician.Observer;
import hero.AbstractHero;
import hero.HeroConstants;

import java.util.List;

public class FightingEngine extends AbstractSubject {
    private AbstractHero stateKillerHero;
    private AbstractHero stateKilledHero;

    private int oldLevel;
    private int newLevel;
    private AbstractHero levelUpHero;

    final void fight(final List<List<Character>> map, final List<AbstractHero> heroList) {
        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map.get(i).size(); j++) {
                fightAtPosition(heroList, i, j);
            }
        }
    }

    private void fightAtPosition(final List<AbstractHero> heroList, final int i,
                                 final int j) {
        // check if there are 2 players on a position
        int numberOfPlayersAtPosition = 0;

        AbstractHero hero1 = null;
        AbstractHero hero2 = null;

        for (AbstractHero hero : heroList) {
            if (hero.getPosX() == i && hero.getPosY() == j && hero.isAlive()) {
                numberOfPlayersAtPosition++;

                if (numberOfPlayersAtPosition == 1) {
                    hero1 = hero;
                }

                if (numberOfPlayersAtPosition == 2) {
                    hero2 = hero;
                }
            }
        }

        if (numberOfPlayersAtPosition != 2) {
            return;
        }

        // if hero2 is wizard swap the positions because the program
        // needs to know the total damage without modifiers of the other
        // hero
        if (hero2.getCharIdentifier() == 'W') {
            AbstractHero temp = hero2;
            hero2 = hero1;
            hero1 = temp;
        }

        executeAbilities(hero1, hero2);
    }

    final protected void executeDamageOverTime(List<AbstractHero> heroList) {
        for (AbstractHero hero : heroList) {
            this.executeOverTimeDamage(hero);
        }
    }


    private void executeAbilities(final AbstractHero hero1, final AbstractHero hero2) {
        if (hero1.getNumberIdentifier() == 7 || hero2.getNumberIdentifier() == 7) {
            System.out.print("** ");
        }
        System.out.println(hero1.getCharIdentifier() + " " + hero1.getNumberIdentifier()
            + " fights " + hero2.getCharIdentifier() + " " + hero2.getNumberIdentifier());

        int damageOfHero1 = hero1.acceptDamage(hero2);
        int damageOfHero2 = hero2.acceptDamage(hero1);

        int currentHpHero1 = hero1.getHp();
        int currentHpHero2 = hero2.getHp();

        hero1.setHp(currentHpHero1 - damageOfHero1);
        hero2.setHp(currentHpHero2 - damageOfHero2);

        // both die; first dies; second dies
        if (hero1.getHp() <= 0 && hero2.getHp() <= 0) {
            stateKillerHero = hero1;
            stateKilledHero = hero2;
            notifyObserversDeadHero();

            stateKillerHero = hero2;
            stateKilledHero = hero1;
            notifyObserversDeadHero();

            purgeHero(hero2);
            purgeHero(hero1);
        } else if (hero1.getHp() <= 0) {
            stateKillerHero = hero2;
            stateKilledHero = hero1;
            notifyObserversDeadHero();

            purgeHero(hero1);

            executeXpGrowth(hero2, hero1);
            executeLevelGrowth(hero2);
        } else if (hero2.getHp() <= 0) {
            stateKillerHero = hero1;
            stateKilledHero = hero2;
            notifyObserversDeadHero();

            purgeHero(hero2);

            executeXpGrowth(hero1, hero2);
            executeLevelGrowth(hero1);
        }
    }

    private void executeOverTimeDamage(final AbstractHero hero) {
        if (hero.getOverTimeRounds() != 0) {
            int currentHp = hero.getHp();

            // if the new resulting hp is less than 0 the hero
            // clearly died
            if (currentHp - hero.getOverTimeDamage() <= 0) {
                purgeHero(hero);
                return;
            } else {
                hero.setHp(currentHp - hero.getOverTimeDamage());
            }

            int currentOverTimeRounds = hero.getOverTimeRounds();
            hero.setOverTimeRounds(currentOverTimeRounds - 1);

            if (hero.getOverTimeRounds() == 0) {
                hero.setOverTimeDamage(0);
            }
        }
    }

    private void executeXpGrowth(final AbstractHero winner, final AbstractHero loser) {
        int xpWinner = winner.getXp() + Math.max(0, HeroConstants.XP_GROWTH_BASE
                - (winner.getLevel() - loser.getLevel()) * HeroConstants.XP_GROWTH_EXPONENT);
        winner.setXp(xpWinner);
    }

    public void executeLevelGrowth(final AbstractHero hero) {
        if (hero.getXp() < HeroConstants.LEVEL_GROWTH_BASE) {
            hero.setLevel(0);
        } else {
            int level = (hero.getXp() - HeroConstants.LEVEL_GROWTH_BASE)
                    / HeroConstants.LEVEL_GROWTH_EXPONENT;
            level++;

            newLevel = level;
            oldLevel = hero.getLevel();
            levelUpHero = hero;

            hero.setLevel(level);
            notifyObserverLevelUp();

            // restore hp and add hp bonus
            hero.setHp(hero.getMaxHp() + hero.getLevel() * hero.getLevelBonusHp());
        }
    }

    public void purgeHero(final AbstractHero hero) {
        hero.setAlive(false);
        hero.setHp(0);

        //hero.setFrozenTime(0);
        //hero.setOverTimeRounds(0);

        //hero.setFrozen(false);
        //hero.setOverTimeDamage(0);

        //hero.setXp(0);
    }

    @Override
    public void notifyObserversRoundNumber() {}

    @Override
    public void notifyObserversDeadHero() {
        for (Observer o : getObservers()) {
            o.updateDeadPlayer();
        }
    }

    @Override
    public void notifyObserversSpawnedAngel() {}

    @Override
    public void notifyObserversAngelAbility() {}

    @Override
    public void notifyObserverLevelUp() {
        for (Observer o : getObservers()) {
            o.updateLevelUp();
        }
    }

    @Override
    public void notifyLifeState() {

    }

    public AbstractHero getStateKillerHero() {
        return stateKillerHero;
    }

    public AbstractHero getStateKilledHero() {
        return stateKilledHero;
    }

    public int getOldLevel() {
        return oldLevel;
    }

    public int getNewLevel() {
        return newLevel;
    }

    public AbstractHero getLevelUpHero() {
        return levelUpHero;
    }
}
