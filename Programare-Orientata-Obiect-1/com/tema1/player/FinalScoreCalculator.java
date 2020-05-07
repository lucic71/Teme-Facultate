package com.tema1.player;

import com.tema1.goods.Goods;
import com.tema1.goods.GoodsFactory;
import com.tema1.goods.GoodsType;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class FinalScoreCalculator {

    public void convertBonusToGoods(Player player){
        List<Integer> bonusList = new ArrayList<Integer>();

        for (Integer good : player.getStall()) {
            if(GoodsFactory.getInstance().getGoodsById(good).getType() == GoodsType.Illegal) {

                for(Map.Entry<Goods, Integer> bonus : GoodsFactory.getInstance().getIllegalGoodsById(good).
                        getIllegalBonus().entrySet()){

                    int times = bonus.getValue();

                    for(int i = 0; i < times; i++){
                        bonusList.add(bonus.getKey().getId());
                    }

                }

            }
        }

        player.addToStall(bonusList);
    }

    public int getTotalLegalProfit(Player player) {
        int totalProfit = 0;

        for (Integer good : player.getStall()) {
            if(GoodsFactory.getInstance().getGoodsById(good).getType() == GoodsType.Legal) {
                totalProfit += GoodsFactory.getInstance().getGoodsById(good).getProfit();
            }
        }

        return totalProfit;
    }

    public int getTotalIllegalProfit(Player player){
        int totalProfit = 0;

        for (Integer good : player.getStall()) {
            if(GoodsFactory.getInstance().getGoodsById(good).getType() == GoodsType.Illegal) {
                totalProfit += GoodsFactory.getInstance().getGoodsById(good).getProfit();
            }
        }

        return totalProfit;
    }

    public void getKingQueenBonus(List<Player> playerList){
        Map<Integer, Goods> allGoods = GoodsFactory.getInstance().getAllGoods();

        for(Map.Entry<Integer, Goods> entry : allGoods.entrySet()){
            SpecialPlayer kingPlayer = new SpecialPlayer();
            SpecialPlayer queenPlayer =  new SpecialPlayer();

            if(entry.getValue().getType() == GoodsType.Legal){

                for(Player player : playerList){
                    if(calculateHowManyOfOneGood(player, entry.getKey()) > kingPlayer.getHowMany()){

                        queenPlayer.setHowMany(kingPlayer.getHowMany());
                        queenPlayer.setPlayer(kingPlayer.getPlayer());

                        kingPlayer.setPlayer(player);
                        kingPlayer.setHowMany(calculateHowManyOfOneGood(player, entry.getKey()));

                    }
                    else if(calculateHowManyOfOneGood(player, entry.getKey()) > queenPlayer.getHowMany()
                        && calculateHowManyOfOneGood(player, entry.getKey()) <= kingPlayer.getHowMany()){

                        queenPlayer.setHowMany(calculateHowManyOfOneGood(player, entry.getKey()));
                        queenPlayer.setPlayer(player);

                    }
                }

            }

            if(kingPlayer.getPlayer() != null) {
                kingPlayer.getPlayer().addToFinalScore(GoodsFactory.getInstance().getLegalGoodsById(entry.getKey()).
                        getKingBonus());
            }

            if(queenPlayer.getPlayer() != null){
                queenPlayer.getPlayer().addToFinalScore(GoodsFactory.getInstance().getLegalGoodsById(entry.getKey()).
                        getQueenBonus());
            }
        }
    }

    public int calculateHowManyOfOneGood(Player player, int good){
        int counter = 0;

        for(Integer stallGood : player.getStall()){
            if(stallGood.equals(good)){
                counter++;
            }
        }

        return counter;
    }

}
