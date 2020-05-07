package com.tema1.cards;

import java.util.List;

public class CardsRemover {
    public void removeTenCards(List<Integer> cards){
        final int toRemove = 10;
        cards.subList(0, toRemove).clear();
    }
}
