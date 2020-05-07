package com.tema1.player;

import java.util.ArrayList;
import java.util.List;

public class Player {
    private String strategy;
    private PlayerRole role;
    private List<Integer> cards;
    private Integer offeredBribe;
    private Integer declaredGood;
    private int money;
    private List<Integer> stall;
    private int finalScore;

    public Player(final String strategy, PlayerRole role, final List<Integer> cards){
        this.strategy = strategy;
        this.role = role;
        this.cards = cards;

        this.offeredBribe = 0;
        this.declaredGood = 0;
        this.money = 80;

        this.stall = new ArrayList<Integer>();
        this.finalScore = 0;
    }

    public String getStrategy() {
        return strategy;
    }

    public void setStrategy(String strategy) {
        this.strategy = strategy;
    }

    public PlayerRole getRole() {
        return role;
    }

    public void setRole(PlayerRole role) {
        this.role = role;
    }

    public List<Integer> getCards() {
        return cards;
    }

    public void setCards(List<Integer> cards) {
        this.cards = cards;
    }

    public Integer getOfferedBribe() {
        return offeredBribe;
    }

    public void setOfferedBribe(Integer offeredBribe) {
        this.offeredBribe = offeredBribe;
    }

    public Integer getDeclaredGood() {
        return declaredGood;
    }

    public void setDeclaredGood(Integer declaredGood) {
        this.declaredGood = declaredGood;
    }

    public int getMoney() {
        return money;
    }

    public List<Integer> getStall() {
        return stall;
    }

    public void addToStall(List<Integer> stall) {
        this.stall.addAll(stall);
    }

    public void setMoney(int money) {
        this.money = money;
    }

    public int getFinalScore() {
        return finalScore;
    }

    public void addToFinalScore(int finalScore) {
        this.finalScore += finalScore;
    }

    @Override
    public String toString() {
        return "Player{" +
                "strategy='" + strategy + '\'' +
                ", role=" + role +
                ", cards=" + cards +
                ", offeredBribe=" + offeredBribe +
                ", declaredGood=" + declaredGood +
                ", money=" + money +
                ", stall=" + stall +
                ", finalScore=" + finalScore +
                '}';
    }

}
