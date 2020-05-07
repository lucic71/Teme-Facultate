package com.tema1.cards;

import com.tema1.goods.GoodsFactory;
import com.tema1.goods.GoodsType;

import java.util.List;

public class CardsChecker {
    public boolean checkForIllegalCards(List<Integer> cards){
        for(Integer card : cards){
            if(GoodsFactory.getInstance().getGoodsById(card).
                getType() == GoodsType.Illegal){
                return true;
            }
        }

        return false;
    }

    public boolean checkForLegalCards(List<Integer> cards){
        for(Integer card : cards){
            if(GoodsFactory.getInstance().getGoodsById(card).
                    getType() == GoodsType.Legal){
                return true;
            }
        }

        return false;
    }
}
