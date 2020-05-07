package com.tema1.strategy;

import com.tema1.cards.CardsChecker;
import com.tema1.engine.inspection.InspectionOutcome;
import com.tema1.filters.Filter;
import com.tema1.filters.GetCards;
import com.tema1.goods.GoodsFactory;
import com.tema1.player.Player;
import com.tema1.strategy.bribedutils.BribedUtils;

import java.util.ArrayList;
import java.util.List;

public class Bribed extends Basic {
    @Override
    public Bag createBag(List<Integer> cards, int money){

        CardsChecker cardsChecker = new CardsChecker();

        /* If there are no illegal cards play as basic */
        if(!cardsChecker.checkForIllegalCards(cards) || money < 5){
            Basic basicPlayer = new Basic();
            return basicPlayer.createBag(cards, money);
        }

        GetCards getCards = new GetCards();
        List<Integer> illegalCards = getCards.getIllegalCards(cards);

        Filter filter = new Filter();
        filter.filterIllegalByProfit(illegalCards);


        /* Put illegal cards in the bag */
        List<Integer> cardsInBag = new ArrayList<Integer>();
        int totalPenalty = 0;

        BribedUtils bribedUtils = new BribedUtils();

        for(Integer card : illegalCards){

            if(cardsInBag.size() < 8){
                cardsInBag.add(card);
                int bribe = bribedUtils.calculateBribe(cardsInBag.size());

                int penalty = GoodsFactory.getInstance().getGoodsById(card).getPenalty();
                totalPenalty += penalty;

                if(money - bribe <= 0 || money - totalPenalty <= 0){
                    cardsInBag.remove(card);
                    totalPenalty -= penalty;
                }
            }
        }

        /* If the cardsInBag is empty it means that the bribe player
            could not afford any illegal good so he will try to play base
            strategy
         */
        if(cardsInBag.isEmpty()){
            Basic basicPlayer = new Basic();
            return basicPlayer.createBag(cards, money);
        }

       /* If there is remaining space try to add some legal cards too */
       int finalBribe = bribedUtils.calculateBribe(cardsInBag.size());

       /* Add legal cards until limit 8 is reached or the player has no more money
          Add them by profit and id
        */

       if(cardsInBag.size() < 8){
           List<Integer> legalCards = getCards.getLegalCards(cards);
           filter.filterLegalByProfitAndId(legalCards);
           for(Integer card : legalCards){

               if(cardsInBag.size() < 8){
                   cardsInBag.add(card);

                   int penalty = GoodsFactory.getInstance().getGoodsById(card).getPenalty();
                   totalPenalty += penalty;

                   /* If the card is apple it means that the bribed player has to pay no
                      penalty because he will declare apples
                    */
//                   if(GoodsFactory.getInstance().getGoodsById(card).getId() == 0){
//                       totalPenalty -= penalty;
//                       penalty = 0;
//                   }

                   if(money - finalBribe < 0 || money - totalPenalty <= 0){
                      cardsInBag.remove(card);
                      totalPenalty -= penalty;
                   }
               }

           }
       }

       return new Bag(cardsInBag, finalBribe);
    }

    public Integer declareBag(List<Integer> cards){
        /* If there is no legal good it means that the bribe strategy
            was not applied so we return the declaration for basic strategy
         */
        CardsChecker cardsChecker = new CardsChecker();
        if(!cardsChecker.checkForIllegalCards(cards)){
            return super.declareBag(cards);
        }

        /* Otherwise return apples */
        return 0;
    }

    public InspectionOutcome inspectBag(Player sheriff, Player player) {
        InspectionOutcome outcome = new InspectionOutcome();

        if(sheriff.getMoney() < 16 ){
            outcome.setPassedGoods(new ArrayList<Integer>(player.getCards()));
            return outcome;
        }

        int bribe = player.getOfferedBribe();

        if (bribe != 0) {
            outcome.setMoneyToPayToSheriff(bribe);
            outcome.setPassedGoods(new ArrayList<Integer>(player.getCards()));

            return outcome;
        } else {
            Integer declaredCard = player.getDeclaredGood();

            for (Integer card : player.getCards()) {

                if (card.equals(declaredCard)) {
                    outcome.setMoneyToGetFromSheriff(outcome.getMoneyToGetFromSheriff() +
                            GoodsFactory.getInstance().getGoodsById(card).getPenalty());
                    outcome.getPassedGoods().add(card);
                } else {
                    outcome.setMoneyToPayToSheriff(outcome.getMoneyToPayToSheriff() +
                            GoodsFactory.getInstance().getGoodsById(card).getPenalty());
                    outcome.getRestrictedGoods().add(card);
                }

            }

            /* Check if the trader was honest or not */
            if(!outcome.getRestrictedGoods().isEmpty() && outcome.getMoneyToPayToSheriff() != 0){
                outcome.setMoneyToGetFromSheriff(0);
            }

            return outcome;
        }
    }
}
