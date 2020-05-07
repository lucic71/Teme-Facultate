package com.tema1.engine;

import com.tema1.cards.CardsAssigner;
import com.tema1.cards.CardsRestorer;
import com.tema1.engine.utils.Utils;
import com.tema1.player.Player;
import com.tema1.player.PlayerRole;
import com.tema1.strategy.Basic;
import com.tema1.strategy.Bribed;
import com.tema1.strategy.Greedy;

import java.util.ArrayList;
import java.util.List;

public class GameEngine {
    private List<Player> playerList = new ArrayList<Player>();
    private List<Integer> cards;

    public GameEngine(List<String> playerStrategy, List<Integer> assetsId){
        this.cards = assetsId;

        for(int i = 0; i < playerStrategy.size(); i++){
            this.playerList.add(new Player(playerStrategy.get(i), PlayerRole.Trader, null));
        }
    }

    public List<Player> play(final int rounds){
       final int subrounds = playerList.size();
        Utils gameEngineUtils = new Utils();

        CardsAssigner cardsAssigner = new CardsAssigner();
        List<List<Integer>> cardChunks;

        for(int i = 0; i < rounds; i++){
            //System.out.println("ROUND NUMBER:" + (i+1));

            for(int j = 0; j < subrounds; j++){
             //   System.out.println("Subround Number: " + j);

                cardChunks = cardsAssigner.divideCards(cards);

                gameEngineUtils.setRolesToTrader(playerList);
                gameEngineUtils.setRoleToSheriff(playerList.get(j));
                gameEngineUtils.assignCards(playerList, cardChunks, cards);

                this.playSubround(playerList, i);

            }

            /* At the end of the subround put the restricted cards back in place */
            CardsRestorer cardsRestorer = new CardsRestorer();
            cardsRestorer.putRestrictedCardsBack(cards);
        }

        GameStages gameStages = new GameStages();
        gameStages.calculateFinalScore(playerList);

        return this.playerList;
    }

    public void playSubround(List<Player> playerList, final int round){
        Basic basic = new Basic();
        Greedy greedy = new Greedy();
        Bribed bribed = new Bribed();
        GameStages gameStages = new GameStages();

//        for(int i = 0; i < playerList.size(); i++){
//            System.out.println(i + " " + playerList.get(i).getStrategy().toUpperCase() + " " + playerList.get(i).getMoney());
//        }
//        System.out.println();
//        System.out.println();

        gameStages.createBag(playerList, round);
//        for(int i = 0; i < playerList.size(); i++){
//            System.out.println(i + " " + playerList.get(i).getStrategy().toUpperCase() + " sack: "
//                    + playerList.get(i).getCards());
//        }
//        System.out.println();
        gameStages.declareBag(playerList);
        gameStages.inspectBag(playerList);
        gameStages.putOnStall(playerList);
//        for(int i = 0; i < playerList.size(); i++){
//            System.out.println(i + " " + playerList.get(i).getStrategy().toUpperCase() + " table: "
//                    + playerList.get(i).getStall());
//        }
//        System.out.println();
    }
}
