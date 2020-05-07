package com.tema1.filters;

import com.tema1.goods.GoodsFactory;
import com.tema1.main.GameOutputFormat;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class Filter {
    public void filterIllegalByProfit(List<Integer> illegalCards){
        Comparator<Integer> comparatorProfit = (Integer a, Integer b) -> {
            Integer aProfit = GoodsFactory.getInstance().getGoodsById(a).getProfit();
            Integer bProfit = GoodsFactory.getInstance().getGoodsById(b).getProfit();

            return bProfit.compareTo(aProfit);
        };

        illegalCards.sort(comparatorProfit);
    }

    public void filterLegalByProfitAndId(List<Integer> legalCards){
        Comparator<Integer> comparatorProfit = (Integer a, Integer b) -> {
            Integer aProfit = GoodsFactory.getInstance().getGoodsById(a).getProfit();
            Integer bProfit = GoodsFactory.getInstance().getGoodsById(b).getProfit();

            return bProfit.compareTo(aProfit);
        };

        Comparator<Integer> comparatorId = (Integer a, Integer b) -> {
            Integer aId = GoodsFactory.getInstance().getGoodsById(a).getId();
            Integer bId = GoodsFactory.getInstance().getGoodsById(b).getId();

            return bId.compareTo(aId);
        };

        legalCards.sort(comparatorProfit.thenComparing(comparatorId));
    }

    public void filterLegalByFreqProfitAndId(List<Integer> legalCards){
        Comparator<Integer> comparatorFreq = (Integer a, Integer b) -> {
            Integer aFreq = Collections.frequency(legalCards, a);
            Integer bFreq = Collections.frequency(legalCards, b);

            return bFreq.compareTo(aFreq);
        };

        Comparator<Integer> comparatorProfit = (Integer a, Integer b) -> {
            Integer aProfit = GoodsFactory.getInstance().getGoodsById(a).getProfit();
            Integer bProfit = GoodsFactory.getInstance().getGoodsById(b).getProfit();

            return bProfit.compareTo(aProfit);
        };

        Comparator<Integer> comparatorId = (Integer a, Integer b) -> {
            Integer aId = GoodsFactory.getInstance().getGoodsById(a).getId();
            Integer bId = GoodsFactory.getInstance().getGoodsById(b).getId();

            return bId.compareTo(aId);
        };

        legalCards.sort(comparatorFreq.thenComparing(comparatorProfit).thenComparing(comparatorId));
    }

    public void filterPlayersByScore(List<GameOutputFormat> outputPlayerList){
        Comparator<GameOutputFormat> comparatorScore = (GameOutputFormat a, GameOutputFormat b) -> {
            Integer aScore = a.getScore();
            Integer bScore = b.getScore();

            return bScore.compareTo(aScore);
        };

        outputPlayerList.sort(comparatorScore);
    }
}
