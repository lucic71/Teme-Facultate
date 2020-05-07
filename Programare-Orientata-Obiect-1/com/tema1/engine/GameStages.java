package com.tema1.engine;

import com.tema1.converter.Converter;
import com.tema1.engine.inspection.Inspection;
import com.tema1.engine.utils.Utils;
import com.tema1.player.FinalScoreCalculator;
import com.tema1.player.Player;
import com.tema1.player.PlayerRole;
import com.tema1.strategy.Bag;

import java.util.List;

public class GameStages {
    private Converter converter = new Converter();

    public void createBag(List<Player> playerList, final int round){

        for(Player player : playerList){
            if(player.getRole() == PlayerRole.Trader){

               Bag createdBag = converter.convertStringToFunctionCreateBag(player.getStrategy(),
                       player.getCards(), player.getMoney(), round);

               player.setCards(createdBag.getBag());
               player.setOfferedBribe(createdBag.getOfferedBribe());

            }
        }
    }

    public void declareBag(List<Player> playerList){

        for(Player player : playerList){
            if(player.getRole() == PlayerRole.Trader){

                Integer declaredCard = converter.convertStringToFunctionDeclareBag(player.getStrategy(),
                        player.getCards());

                player.setDeclaredGood(declaredCard);

            }
        }
    }

    public void inspectBag(List<Player> playerList){
        Utils utils = new Utils();
        Inspection inspection = new Inspection();
        Player sheriff = utils.getSheriff(playerList);

        switch (sheriff.getStrategy()) {
            case "basic":
                inspection.inspectAsBasic(playerList, sheriff);
                break;
            case "greedy":
                inspection.inspectAsGreedy(playerList, sheriff);
                break;
            case "bribed":
                inspection.inspectAsBribed(playerList, sheriff);
                break;
        }

    }

    public void putOnStall(List<Player> playerList){
        for(Player player : playerList){
            if(player.getCards() != null){
                player.addToStall(player.getCards());
            }
        }
    }

    public void calculateFinalScore(List<Player> playerList){
        FinalScoreCalculator calculator = new FinalScoreCalculator();

        for(Player player : playerList){
            calculator.convertBonusToGoods(player);

            player.addToFinalScore(player.getMoney());
            player.addToFinalScore(calculator.getTotalLegalProfit(player));
            player.addToFinalScore(calculator.getTotalIllegalProfit(player));

        }

        calculator.getKingQueenBonus(playerList);
    }
}
