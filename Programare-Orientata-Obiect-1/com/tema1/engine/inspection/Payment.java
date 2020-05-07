package com.tema1.engine.inspection;

import com.tema1.player.Player;

public class Payment {
    public void outcomePay(InspectionOutcome outcome, Player sheriff, Player player){
        sheriff.setMoney(sheriff.getMoney() - outcome.getMoneyToGetFromSheriff());
        sheriff.setMoney(sheriff.getMoney() + outcome.getMoneyToPayToSheriff());

        player.setMoney(player.getMoney() - outcome.getMoneyToPayToSheriff());
        player.setMoney(player.getMoney() + outcome.getMoneyToGetFromSheriff());

        player.setCards(outcome.getPassedGoods());
    }
}
