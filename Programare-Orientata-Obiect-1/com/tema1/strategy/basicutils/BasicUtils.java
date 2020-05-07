package com.tema1.strategy.basicutils;

import com.tema1.filters.GetCards;
import com.tema1.goods.GoodsFactory;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class BasicUtils {
    public Integer getMostProfitableIllegalCard(List<Integer> cards){
        if(cards.isEmpty()){
            return -1;
        }

        GetCards getCards = new GetCards();
        List<Integer> illegalCards = getCards.getIllegalCards(cards);

        Comparator<Integer> comparatorProfit = (Integer a, Integer b) ->{
            Integer aProfit = GoodsFactory.getInstance().getGoodsById(a).getProfit();
            Integer bProfit = GoodsFactory.getInstance().getGoodsById(b).getProfit();

            return aProfit.compareTo(bProfit);
        };

        return Collections.max(illegalCards, comparatorProfit);
    }

    public void truncate(List<Integer> legalCards){
        legalCards = legalCards.subList(0, 8);
    }
}
