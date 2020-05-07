package com.tema1.engine.inspection;

import com.tema1.player.Player;

import java.util.List;

public class ProximityPlayers {
    public Player getLeftPlayer(List<Player> playerList, Player sheriff){
        int pos = sheriffPosition(playerList, sheriff);

        if(pos != -1) {
            if (pos - 1 < 0) {
                return playerList.get(playerList.size() - 1);
            }
            else{
                return playerList.get(pos-1);
            }
        }

        return null;
    }

    public Player getRightPlayer(List<Player> playerList, Player sheriff){
        int pos = sheriffPosition(playerList, sheriff);

        if(pos != -1){
            if(pos + 1 >= playerList.size()){
                return playerList.get(0);
            }
            else{
                return playerList.get(pos+1);
            }
        }

        return null;
    }

    private int sheriffPosition(List<Player> playerList, Player sheriff){
        for(int i = 0; i < playerList.size(); i++){
            if(playerList.get(i) == sheriff){
                return i;
            }
        }

        return -1;
    }
}
