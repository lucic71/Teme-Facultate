package com.tema1.goods;

import java.util.HashMap;
import java.util.Map;

public class GoodsFactory {
    private static GoodsFactory instance = null;

    private final Map<Integer, Goods> goodsById;

    private final Map<Integer, IllegalGoods> illegalGoodsById;

    private final Map<Integer, LegalGoods> legalGoodsById;

    private GoodsFactory() {
        goodsById = new HashMap<Integer, Goods>();
        illegalGoodsById = new HashMap<Integer, IllegalGoods>();
        legalGoodsById = new HashMap<Integer, LegalGoods>();

        initLegalGoods();
        initIllegalGoods();
    }

    private final void initLegalGoods() {
        // create the types of legal goods
        Goods good0 = new LegalGoods(0, 2, 2, 20, 10);
        Goods good1 = new LegalGoods(1, 3, 2, 19, 9);
        Goods good2 = new LegalGoods(2, 4, 2, 18, 9);
        Goods good3 = new LegalGoods(3, 4, 2, 17, 8);
        Goods good4 = new LegalGoods(4, 3, 2, 16, 7);
        Goods good5 = new LegalGoods(5, 2, 2, 15, 6);
        Goods good6 = new LegalGoods(6, 3, 2, 14, 5);
        Goods good7 = new LegalGoods(7, 5, 2, 13, 4);
        Goods good8 = new LegalGoods(8, 2, 2, 12, 3);
        Goods good9 = new LegalGoods(9, 3, 2, 11, 2);

        // insert legal goods into a hashMap
        goodsById.put(0, good0);
        goodsById.put(1, good1);
        goodsById.put(2, good2);
        goodsById.put(3, good3);
        goodsById.put(4, good4);
        goodsById.put(5, good5);
        goodsById.put(6, good6);
        goodsById.put(7, good7);
        goodsById.put(8, good8);
        goodsById.put(9, good9);

        legalGoodsById.put(0, (LegalGoods) good0);
        legalGoodsById.put(1, (LegalGoods) good1);
        legalGoodsById.put(2, (LegalGoods) good2);
        legalGoodsById.put(3, (LegalGoods) good3);
        legalGoodsById.put(4, (LegalGoods) good4);
        legalGoodsById.put(5, (LegalGoods) good5);
        legalGoodsById.put(6, (LegalGoods) good6);
        legalGoodsById.put(7, (LegalGoods) good7);
        legalGoodsById.put(8, (LegalGoods) good8);
        legalGoodsById.put(9, (LegalGoods) good9);
    }

    private final void initIllegalGoods() {
        // create LegalGoods - quantity hashMaps
        Map<Goods, Integer> bonus0 = new HashMap<Goods, Integer>();
        Map<Goods, Integer> bonus1 = new HashMap<Goods, Integer>();
        Map<Goods, Integer> bonus2 = new HashMap<Goods, Integer>();
        Map<Goods, Integer> bonus3 = new HashMap<Goods, Integer>();
        Map<Goods, Integer> bonus4 = new HashMap<Goods, Integer>();

        bonus0.put(goodsById.get(1), 3);
        bonus1.put(goodsById.get(3), 2);
        bonus2.put(goodsById.get(2), 2);
        bonus3.put(goodsById.get(7), 4);
        bonus4.put(goodsById.get(3), 1);
        bonus4.put(goodsById.get(4), 2);
        bonus4.put(goodsById.get(6), 3);

        // create the types of illegal goods
        IllegalGoods good0 = new IllegalGoods(20, 9, 4, bonus0);
        IllegalGoods good1 = new IllegalGoods(21, 8, 4, bonus1);
        IllegalGoods good2 = new IllegalGoods(22, 7, 4, bonus2);
        IllegalGoods good3 = new IllegalGoods(23, 6, 4, bonus3);
        IllegalGoods good4 = new IllegalGoods(24, 12, 4, bonus4);

        // insert legal goods into a hashMap
        goodsById.put(20, good0);
        goodsById.put(21, good1);
        goodsById.put(22, good2);
        goodsById.put(23, good3);
        goodsById.put(24, good4);

        illegalGoodsById.put(20, good0);
        illegalGoodsById.put(21, good1);
        illegalGoodsById.put(22, good2);
        illegalGoodsById.put(23, good3);
        illegalGoodsById.put(24, good4);
    }

    public static GoodsFactory getInstance() {
        if(instance == null) {
            instance = new GoodsFactory();
        }

        return instance;
    }

    public final Goods getGoodsById(final int id) {
        return goodsById.get(id);
    }

    public final IllegalGoods getIllegalGoodsById(final int id) {
        return illegalGoodsById.get(id);
    }

    public final LegalGoods getLegalGoodsById(final int id) {
        return legalGoodsById.get(id);
    }

    public final Map<Integer, Goods> getAllGoods() {
        return goodsById;
    }

    public final Map<Integer, IllegalGoods> getAllIllegalGoods(){
        return illegalGoodsById;
    }
}
