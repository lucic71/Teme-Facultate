package com.tema1.engine.inspection;

import com.tema1.cards.CardsRestorer;
import com.tema1.converter.Converter;
import com.tema1.player.Player;

import java.util.List;

public class Inspection {
    private Converter converter = new Converter();
    private Payment payment = new Payment();

    public void inspectAsBasic(List<Player> playerList, Player sheriff){

        for(Player player : playerList){
            if(player != sheriff){
                InspectionOutcome outcome = converter.convertStringToFunctionInspectBag(
                        sheriff.getStrategy(), sheriff, player);
                payment.outcomePay(outcome, sheriff, player);
                CardsRestorer.restrictedCards.add(outcome.getRestrictedGoods());
            }
        }
    }

    public void inspectAsGreedy(List<Player> playerList, Player sheriff){

        for(Player player : playerList){

            if(player != sheriff){
                InspectionOutcome outcome = converter.convertStringToFunctionInspectBag(
                        sheriff.getStrategy(), sheriff, player);

                payment.outcomePay(outcome, sheriff, player);
                CardsRestorer.restrictedCards.add(outcome.getRestrictedGoods());
            }

        }
    }

    public void inspectAsBribed(List<Player> playerList, Player sheriff){
        ProximityPlayers proximityPlayers = new ProximityPlayers();
        Player leftPlayer = proximityPlayers.getLeftPlayer(playerList, sheriff);
        Player rightPlayer = proximityPlayers.getRightPlayer(playerList, sheriff);

        assert leftPlayer != null : "leftPlayer could not be found";
        assert rightPlayer != null : "rightPlayer could not be found";

        if(leftPlayer != rightPlayer){
            leftPlayer.setOfferedBribe(0);
            InspectionOutcome outcome = converter.convertStringToFunctionInspectBag(
                    sheriff.getStrategy(), sheriff, leftPlayer);

            payment.outcomePay(outcome, sheriff, leftPlayer);
            CardsRestorer.restrictedCards.add(outcome.getRestrictedGoods());

            rightPlayer.setOfferedBribe(0);
            outcome = converter.convertStringToFunctionInspectBag(
                    sheriff.getStrategy(), sheriff, rightPlayer);

            payment.outcomePay(outcome, sheriff, rightPlayer);
            CardsRestorer.restrictedCards.add(outcome.getRestrictedGoods());
        }
        else{
            leftPlayer.setOfferedBribe(0);

            InspectionOutcome outcome = converter.convertStringToFunctionInspectBag(
                    sheriff.getStrategy(), sheriff, leftPlayer);

            payment.outcomePay(outcome, sheriff, leftPlayer);
            CardsRestorer.restrictedCards.add(outcome.getRestrictedGoods());
        }

        for(Player player : playerList){

            if(player != sheriff && (player != leftPlayer && player != rightPlayer &&
                    player.getOfferedBribe() != 0)){
//                InspectionOutcome outcome = converter.convertStringToFunctionInspectBag(
//                        sheriff.getStrategy(), sheriff, player);

                InspectionOutcome outcome = new InspectionOutcome();

                outcome.setPassedGoods(player.getCards());
                outcome.setMoneyToPayToSheriff(player.getOfferedBribe());

                payment.outcomePay(outcome, sheriff, player);
                CardsRestorer.restrictedCards.add(outcome.getRestrictedGoods());
            }

        }
    }
}
