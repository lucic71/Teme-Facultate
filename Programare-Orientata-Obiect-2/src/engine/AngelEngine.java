package engine;

import angel.AbstractAngel;
import angel.Spawner;
import angel.TheDoomer;
import great_magician.Observer;
import hero.AbstractHero;

import java.util.List;

public class AngelEngine extends AbstractSubject {
    private AbstractAngel spawnedAngel;
    private AbstractHero  affectedHero;
    private AbstractHero changedLifeStateHero;

    public void visitHeroes(List<AbstractAngel> abstractAngels,
                            List<AbstractHero> abstractHeroes) {

        for (AbstractAngel angel : abstractAngels) {
            spawnedAngel = angel;
            notifyObserversSpawnedAngel();

            for (AbstractHero hero : abstractHeroes) {
                affectedHero = hero;

                /* special cases when either the hero may be dead and the angel is
                   a Spawner or the hero may be alive and the hero is a TheDoomer
                */
                if (angel instanceof Spawner && !hero.isAlive()
                        && angel.getPosX() == hero.getPosX() && angel.getPosY()
                        == hero.getPosY()) {
                    changedLifeStateHero = hero;

                    hero.acceptAngelAbility(angel);
                    notifyObserversAngelAbility();
                    notifyLifeState();
                    continue;
                } else if (angel instanceof Spawner && hero.isAlive()
                        && angel.getPosX() == hero.getPosX() && angel.getPosY()
                        == hero.getPosY()) {
                    continue;
                }

                if (angel instanceof TheDoomer && hero.isAlive()
                        && angel.getPosX() == hero.getPosX() && angel.getPosY()
                        == hero.getPosY()) {
                    changedLifeStateHero = hero;

                    hero.acceptAngelAbility(angel);
                    notifyObserversAngelAbility();
                    notifyLifeState();
                    continue;
                } else if (angel instanceof TheDoomer && !hero.isAlive()
                        && angel.getPosX() == hero.getPosX() && angel.getPosY()
                        == hero.getPosY()) {
                    continue;
                }

                // flag that checks if the hero is still alive after a bad
                // angel applied its ability
                boolean heroStillAlive = true;

                if (hero.isAlive() && angel.getPosX() == hero.getPosX()
                    && angel.getPosY() == hero.getPosY()) {
                    notifyObserversAngelAbility();
                    hero.acceptAngelAbility(angel);

                    if (!hero.isAlive()) {
                        heroStillAlive = false;
                    }
                }

                // if the hero was killed by an angel
                if (!heroStillAlive) {
                   changedLifeStateHero = hero;
                   notifyLifeState();
                }
            }
        }

    }

    public AbstractAngel getSpawnedAngel() {
        return spawnedAngel;
    }

    public AbstractHero getAffectedHero() {
        return affectedHero;
    }

    public AbstractHero getChangedLifeStateHero() {
        return changedLifeStateHero;
    }

    @Override
    public void notifyObserversRoundNumber() {

    }

    @Override
    public void notifyObserversDeadHero() {

    }

    @Override
    public void notifyObserversSpawnedAngel() {
        for (Observer o : getObservers()) {
            o.updateSpawnedAngel();
        }
    }

    @Override
    public void notifyObserversAngelAbility() {
        for (Observer o : getObservers()) {
            o.updateAngelAbility();
        }
    }

    @Override
    public void notifyObserverLevelUp() {

    }

    @Override
    public void notifyLifeState() {
        for (Observer o : getObservers()) {
            o.updateLifeState();
        }
    }
}
