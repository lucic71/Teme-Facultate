package com.tema1.strategy;

import java.util.List;

public class Bag {
    private List<Integer> bag;
    private int offeredBribe;

    public Bag(List<Integer> bag, int offeredBribe) {
        this.bag = bag;
        this.offeredBribe = offeredBribe;
    }

    public List<Integer> getBag() {
        return bag;
    }

    public void setBag(List<Integer> bag) {
        this.bag = bag;
    }

    public int getOfferedBribe() {
        return offeredBribe;
    }

    public void setOfferedBribe(int offeredBribe) {
        this.offeredBribe = offeredBribe;
    }

    @Override
    public String toString() {
        return "Bag{" +
                "bag=" + bag +
                ", offeredBribe=" + offeredBribe +
                '}';
    }
}
