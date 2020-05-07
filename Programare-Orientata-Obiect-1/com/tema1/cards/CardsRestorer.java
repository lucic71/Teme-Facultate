package com.tema1.cards;

import java.util.ArrayList;
import java.util.List;

public class CardsRestorer {
    public static List<List<Integer>> restrictedCards = new ArrayList<List<Integer>>();
    public static List<List<Integer>> originalCards = new ArrayList<List<Integer>>();

    public void putRestrictedCardsBack(List<Integer> cards){
        for(int i = 0; i < restrictedCards.size(); i++){
            for(Integer card : restrictedCards.get(i)){
                if(originalCards.get(i).contains(card)){
                    cards.add(card);
                    originalCards.get(i).remove(card);
                }
            }
        }
    }
}
