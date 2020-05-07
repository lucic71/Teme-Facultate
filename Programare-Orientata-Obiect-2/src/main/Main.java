package main;

import engine.GameEngine;
import hero.AbstractHero;

import java.util.List;

public final class Main {
    public static GameOutput gameOutput;

    public static void main(final String[] args) {
        try {
            GameInputLoader inputLoader = new GameInputLoader(args[0], args[1]);
            GameInput input = inputLoader.load();

            GameOutput gameOutput = new GameOutput(args[0], args[1]);
            Main.gameOutput = gameOutput;

            GameEngine gameEngine = new GameEngine(input);
            List<AbstractHero> heroList = gameEngine.play();

            gameOutput.writeFinalResults(heroList);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
