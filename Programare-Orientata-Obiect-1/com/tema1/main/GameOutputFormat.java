package com.tema1.main;

public class GameOutputFormat {
    private int number;
    private String strategy;
    private int score;

    public GameOutputFormat(int number, String strategy, int score) {
        this.number = number;
        this.strategy = strategy;
        this.score = score;
    }

    public int getNumber() {
        return number;
    }

    public void setNumber(int number) {
        this.number = number;
    }

    public String getStrategy() {
        return strategy;
    }

    public void setStrategy(String strategy) {
        this.strategy = strategy;
    }

    public int getScore() {
        return score;
    }

    public void setScore(int score) {
        this.score = score;
    }

    @Override
    public String toString() {
        return "GameOutputFormat{" +
                "number=" + number +
                ", strategy='" + strategy + '\'' +
                ", score=" + score +
                '}';
    }
}
