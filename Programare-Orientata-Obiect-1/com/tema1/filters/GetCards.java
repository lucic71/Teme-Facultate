package com.tema1.filters;

import com.tema1.goods.GoodsFactory;
import com.tema1.goods.GoodsType;

import java.util.ArrayList;
import java.util.List;

public class GetCards {
    public List<Integer> getIllegalCards(List<Integer> cards){
        List<Integer> illegalCards = new ArrayList<Integer>(cards);
        illegalCards.removeIf(filter -> GoodsFactory.getInstance().getGoodsById(filter).
                    getType() == GoodsType.Legal);

        return illegalCards;
    }

    public List<Integer> getLegalCards(List<Integer> cards){
        List<Integer> legalCards = new ArrayList<Integer>(cards);
        legalCards.removeIf(filter -> GoodsFactory.getInstance().getGoodsById(filter).
                getType() == GoodsType.Illegal);

        return legalCards;
    }

    public void getBestCardForBasic(List<Integer> legalCards){
        Integer bestCard = legalCards.get(0);
        legalCards.removeIf(filter -> filter != bestCard);
    }
}
