package engine;

import great_magician.Observer;

import java.util.ArrayList;
import java.util.List;

public abstract class AbstractSubject {
    private List<Observer> observers = new ArrayList<>();

    public void attach(Observer o) {
        observers.add(o);
    }

    public List<Observer> getObservers() {
        return observers;
    }

    public abstract void notifyObserversRoundNumber();
    public abstract void notifyObserversDeadHero();
    public abstract void notifyObserversSpawnedAngel();
    public abstract void notifyObserversAngelAbility();
    public abstract void notifyObserverLevelUp();
    public abstract void notifyLifeState();
}
