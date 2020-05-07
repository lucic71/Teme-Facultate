package com.tema1.strategy;

import com.tema1.cards.CardsChecker;
import com.tema1.engine.inspection.InspectionOutcome;
import com.tema1.filters.Filter;
import com.tema1.filters.GetCards;
import com.tema1.goods.GoodsFactory;
import com.tema1.player.Player;
import com.tema1.strategy.basicutils.BasicUtils;

import java.util.ArrayList;
import java.util.List;

public class Basic {
    public Bag createBag(List<Integer> cards, int money){

        BasicUtils basicUtils = new BasicUtils();
        CardsChecker cardsChecker = new CardsChecker();
        List<Integer> cardsInBag = new ArrayList<Integer>();

        if(!cardsChecker.checkForLegalCards(cards)){
            cardsInBag.add(basicUtils.getMostProfitableIllegalCard(cards));

            if(money - GoodsFactory.getInstance().getGoodsById(cardsInBag.get(0)).getPenalty() < 0){
                cardsInBag.clear();
            }

            return new Bag(cardsInBag, 0);
        }

        GetCards getCards = new GetCards();
        List<Integer> legalCards = getCards.getLegalCards(cards);

        Filter filter = new Filter();
        filter.filterLegalByFreqProfitAndId(legalCards);
        getCards.getBestCardForBasic(legalCards);

        if(cardsInBag.size() > 8) {
            basicUtils.truncate(legalCards);
        }

        return new Bag(legalCards, 0);
    }

    public Integer declareBag(List<Integer> cards){
        CardsChecker cardsChecker = new CardsChecker();

        /* If there is no legal card it means that one illegal card
            was choosen and apples will be declared
         */
        if(!cardsChecker.checkForLegalCards(cards)){
            return 0;
        }
        else{
            return cards.get(0);
        }
    }

    public InspectionOutcome inspectBag(Player sheriff, Player player){
        InspectionOutcome outcome = new InspectionOutcome();

        if(sheriff.getMoney() < 16){
            return outcome;
        }


        Integer declaredCard = player.getDeclaredGood();

        for(Integer card : player.getCards()){

            if(card.equals(declaredCard)){
                outcome.setMoneyToGetFromSheriff(outcome.getMoneyToGetFromSheriff() +
                        GoodsFactory.getInstance().getGoodsById(card).getPenalty());
                outcome.getPassedGoods().add(card);
            }
            else{
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
