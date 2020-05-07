package great_magician;

public abstract class Observer {
    public abstract void updateRoundNumber();
    public abstract void updateDeadPlayer();
    public abstract void updateSpawnedAngel();
    public abstract void updateAngelAbility();
    public abstract void updateLevelUp();
    public abstract void updateLifeState();
}
