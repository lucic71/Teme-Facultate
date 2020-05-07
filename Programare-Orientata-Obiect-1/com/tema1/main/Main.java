package com.tema1.main;


import com.tema1.engine.GameEngine;
import com.tema1.player.Player;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.List;

public class Main {

    public static void main(final String[] args) throws ClassNotFoundException, NoSuchMethodException,
            InvocationTargetException, InstantiationException, IllegalAccessException, IOException {
        GameInputLoader gameInputLoader = new GameInputLoader(args[0], args[1]);
        GameInput gameInput = gameInputLoader.load();
        //TODO implement homework logic

        List<Player> playerList;

        GameEngine gameEngine = new GameEngine(gameInput.getPlayerNames(), gameInput.getAssetIds());
        playerList = gameEngine.play(gameInput.getRounds());

        GameOutput gameOutput = new GameOutput();
        gameOutput.writeToFile(playerList, args[1]);
    }
}
