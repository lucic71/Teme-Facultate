package com.tema1.engine.utils;

import com.tema1.cards.CardsRemover;
import com.tema1.cards.CardsRestorer;
import com.tema1.player.Player;
import com.tema1.player.PlayerRole;

import java.util.List;

public class Utils {
    public void setRolesToTrader(List<Player> playerList){
        for(Player player : playerList){
            player.setRole(PlayerRole.Trader);
        }
    }

    public void setRoleToSheriff(Player sheriff){
        sheriff.setRole(PlayerRole.Sheriff);
        sheriff.setCards(null);
        sheriff.setOfferedBribe(0);
        sheriff.setDeclaredGood(0);
    }

    public Player getSheriff(List<Player> playerList){
        for(Player player : playerList){
            if(player.getRole() == PlayerRole.Sheriff){
                return player;
            }
        }

        return null;
    }

    public void assignCards(List<Player> playerList, List<List<Integer>> cardChunks, List<Integer> cards){
        final int firstChunk = 0;
        CardsRemover cardsRemover = new CardsRemover();

        for(Player player : playerList){

            if(player.getRole() == PlayerRole.Trader){
                player.setCards(cardChunks.get(firstChunk));
                CardsRestorer.originalCards.add(cardChunks.get(firstChunk));
                cardChunks.remove(firstChunk);
                cardsRemover.removeTenCards(cards);
            }

        }
    }
}
