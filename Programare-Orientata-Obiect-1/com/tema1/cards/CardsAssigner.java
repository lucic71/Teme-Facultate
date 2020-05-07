package com.tema1.cards;

import java.util.ArrayList;
import java.util.List;

public class CardsAssigner {
    public List<List<Integer>> divideCards(List<Integer> initialList){
        final int chunkLength = 10;

        /* Split the cards in chunks of 10 that
            well be assigned to each player
         */
        List<List<Integer>> chunks = new ArrayList<List<Integer>>();
        final int N = initialList.size();
        for(int i = 0; i < N; i += chunkLength){
            chunks.add(
                    new ArrayList<Integer>(initialList.subList(i, Math.min(N, i + chunkLength)))
            );
        }

        return chunks;
    }
}
