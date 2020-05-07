package com.tema1.player;

public class SpecialPlayer {
    private Player player;
    private int howMany;

    public SpecialPlayer() {
        this.player = null;
        this.howMany = 0;
    }

    public Player getPlayer() {
        return player;
    }

    public void setPlayer(Player player) {
        this.player = player;
    }

    public int getHowMany() {
        return howMany;
    }

    public void setHowMany(int howMany) {
        this.howMany = howMany;
    }
}
