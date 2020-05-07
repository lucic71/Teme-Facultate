package com.tema1.main;

import com.tema1.filters.Filter;
import com.tema1.player.Player;

import fileio.implementations.FileWriter;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class GameOutput {
    public void writeToFile(List<Player> playerList, String outputFile) throws IOException {
        List<GameOutputFormat> outputPlayerList = new ArrayList<GameOutputFormat>();

        for(int i = 0; i < playerList.size(); i++) {
            outputPlayerList.add(new GameOutputFormat(i, playerList.get(i).getStrategy().
                toUpperCase(), playerList.get(i).getFinalScore()));
        }

        Filter filter = new Filter();
        filter.filterPlayersByScore(outputPlayerList);

        for(GameOutputFormat player : outputPlayerList){
            System.out.printf("%d %s %d\n", player.getNumber(), player.getStrategy(),
                    player.getScore());
        }

    }
}
