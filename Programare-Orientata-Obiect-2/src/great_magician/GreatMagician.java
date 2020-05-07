package great_magician;

import angel.AbstractAngel;
import engine.AngelEngine;
import engine.FightingEngine;
import engine.GameEngine;
import hero.AbstractHero;
import main.GameOutput;

public class GreatMagician extends Observer {
    private AbstractHero killedHero;
    private AbstractHero killerHero;

    private FightingEngine deadPlayerSubject;
    private GameEngine     roundNumberSubject;
    private AngelEngine    spawnedAngelSubject;
    private AngelEngine    angelAbilitySubject;
    private FightingEngine levelUpSubject;
    private AngelEngine lifeStateSubject;

    private GameOutput gameOutput;

    public GreatMagician(GameOutput gameOutput) {
        this.gameOutput = gameOutput;
    }

    public void addDeadPlayerSubject(FightingEngine subject) {
       this.deadPlayerSubject = subject;
       if (!subject.getObservers().contains(this)) {
            subject.attach(this);
       }
    }

    public void addRoundNumberSubject(GameEngine subject) {
        this.roundNumberSubject = subject;
        if (!subject.getObservers().contains(this)) {
            subject.attach(this);
        }
    }

    public void addSpawnedAngelSubject(AngelEngine subject) {
        this.spawnedAngelSubject = subject;
        if (!subject.getObservers().contains(this)) {
            subject.attach(this);
        }
    }

    public void addAngelAbilitySubject(AngelEngine subject) {
        this.angelAbilitySubject = subject;
        if (!subject.getObservers().contains(this)) {
            subject.attach(this);
        }
    }

    public void addLevelUpSubject(FightingEngine subject) {
       this.levelUpSubject = subject;
       if (!subject.getObservers().contains(this)) {
            subject.attach(this);
       }
    }

    public void addLifeStateSubject(AngelEngine subject) {
        this.lifeStateSubject = subject;
        if (!subject.getObservers().contains(this)) {
            subject.attach(this);
        }
    }

    @Override
    public void updateRoundNumber() {
        gameOutput.writeRoundNumber(roundNumberSubject.getCurrentRound());
    }

    @Override
    public void updateDeadPlayer() {
        killedHero = deadPlayerSubject.getStateKilledHero();
        killerHero = deadPlayerSubject.getStateKillerHero();

        String killedHeroName = getHeroName(killedHero.getCharIdentifier());
        String killerHeroName = getHeroName(killerHero.getCharIdentifier());

        gameOutput.writeDeadPlayer(killedHero, killerHero, killedHeroName, killerHeroName);
    }

    @Override
    public void updateSpawnedAngel() {
        AbstractAngel angel = spawnedAngelSubject.getSpawnedAngel();
        gameOutput.writeSpawnedAngel(angel.getName(), angel.getPosX(), angel.getPosY());
    }

    @Override
    public void updateAngelAbility() {
        AbstractHero hero = angelAbilitySubject.getAffectedHero();
        AbstractAngel angel = angelAbilitySubject.getSpawnedAngel();

        String action = null;
        if (angel.getIdentifier().compareTo("good") == 0) {
            action = "helped";
        } else if (angel.getIdentifier().compareTo("bad") == 0) {
            action = "hit";
        }

        gameOutput.writeAngelAbility(angel.getName(), action,
                getHeroName(hero.getCharIdentifier()), hero.getNumberIdentifier());
    }

    @Override
    public void updateLevelUp() {
        int newLevel = levelUpSubject.getNewLevel();
        int oldLevel = levelUpSubject.getOldLevel();
        AbstractHero hero = levelUpSubject.getLevelUpHero();

        for (int i = oldLevel + 1; i <= newLevel; i++) {
            gameOutput.writeLevelUp(getHeroName(hero.getCharIdentifier()), hero.getNumberIdentifier(), i);
        }
    }

    @Override
    public void updateLifeState() {
        AbstractHero hero = lifeStateSubject.getChangedLifeStateHero();
        gameOutput.writeLifeStateHero(hero, getHeroName(hero.getCharIdentifier()));
    }

    private String getHeroName(Character identifier) {
        String name = null;
        switch (identifier) {
            case 'W' :
                name = "Wizard";
                break;
            case 'R' :
                name = "Rogue";
                break;
            case 'K' :
                name = "Knight";
                break;
            case 'P' :
                name = "Pyromancer";
                break;
        }

        return name;
    }
}
