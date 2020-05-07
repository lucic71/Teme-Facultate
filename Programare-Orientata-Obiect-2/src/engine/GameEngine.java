package engine;

import angel.AbstractAngel;
import great_magician.GreatMagician;
import great_magician.Observer;
import hero.AbstractHero;
import hero.HeroesFactory;
import main.GameInput;
import main.Main;
import utils.Utils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class GameEngine extends AbstractSubject {
    private List<AbstractHero> heroList;
    private List<String> angelString;

    private List<List<Character>> map;
    private List<String> rounds;
    private int currentRound;

    private final GreatMagician greatMagician = new GreatMagician(Main.gameOutput);

    /**
        GameEngine constructor stores a list of heroes that will
        be later used in @play method. Also it saves the map and
        the number of rounds. This info is parsed from a GameInput
        object declared in #main.GameInput.
     */
    public GameEngine(final GameInput input) {
        heroList = new ArrayList<>();
        HeroesFactory factory = HeroesFactory.getInstance();

        // set heroes for the game
        for (String hero : input.getHeroes()) {
            List<String> heroAsStrList = Arrays.asList(hero.split(" "));

            assert heroAsStrList.size() == 3 : "Not enough info to create a hero!";

            String name = heroAsStrList.get(0);
            int posX = Integer.parseInt(heroAsStrList.get(1));
            int posY = Integer.parseInt(heroAsStrList.get(2));

            heroList.add(factory.getHero(name, posX, posY));
        }

        // receive the angels as strings
        this.angelString = input.getAngels();

        // set the map for the game
        map = input.getMap();

        // set rounds for the game
        rounds = input.getRounds();
    }

    /**
     * The #GameEngine.play method simulates each round of the game,
     * moving the players to their desired position and then calling
     * the fight method from Fighting class. This class mainly focuses
     * on handling the battles on each position in the map.
     *
     * @return: a list of type AbstractHero that contains all info
     *      needed at the end of the game.
     */
    public List<AbstractHero> play() {
        MovementEngine movementEngine = new MovementEngine();
        FightingEngine fightingEngine = new FightingEngine();
        AngelEngine angelEngine = new AngelEngine();

        greatMagician.addDeadPlayerSubject(fightingEngine);
        greatMagician.addRoundNumberSubject(this);
        greatMagician.addSpawnedAngelSubject(angelEngine);
        greatMagician.addAngelAbilitySubject(angelEngine);
        greatMagician.addLevelUpSubject(fightingEngine);
        greatMagician.addLifeStateSubject(angelEngine);


        // move the players
        for (int roundIt = 0; roundIt < rounds.size(); roundIt++) {
            currentRound = roundIt + 1;
            notifyObserversRoundNumber();

            // dot
            fightingEngine.executeDamageOverTime(heroList);

            // movement
            for (int i = 0; i < rounds.get(roundIt).length(); i++) {
                char direction = rounds.get(roundIt).charAt(i);
                AbstractHero hero = heroList.get(i);

                // skip if the hero is dead
                if (!hero.isAlive()) {
                    continue;
                }

                // if the hero is frozen do not move
                if (!hero.isFrozen()) {
                    movementEngine.move(hero, direction);

                    int newPosX = hero.getPosX();
                    int newPosY = hero.getPosY();

                    hero.setCurrentTerrain(map.get(newPosX).get(newPosY));

                    // apply strategy
                    hero.applyStrategy();
                } else {
                    int currentFrozenTime = hero.getFrozenTime();
                    hero.setFrozenTime(currentFrozenTime - 1);

                    if (hero.getFrozenTime() == 0) {
                        hero.setFrozen(false);
                    }

                }
            }

            // fight
            fightingEngine.fight(map, heroList);

            // set angels for this round
            List<AbstractAngel> angelList =
                    Utils.strListToAbstrAngelList(angelString.get(roundIt));

            // angels visit heroes
            angelEngine.visitHeroes(angelList, heroList);
        }

        return this.heroList;
    }

    public int getCurrentRound() {
        return currentRound;
    }

    @Override
    public void notifyObserversRoundNumber() {
        for (Observer o : getObservers()) {
            o.updateRoundNumber();
        }
    }

    @Override
    public void notifyObserversDeadHero() {

    }

    @Override
    public void notifyObserversSpawnedAngel() {

    }

    @Override
    public void notifyObserversAngelAbility() {

    }

    @Override
    public void notifyObserverLevelUp() {

    }

    @Override
    public void notifyLifeState() {

    }
}
