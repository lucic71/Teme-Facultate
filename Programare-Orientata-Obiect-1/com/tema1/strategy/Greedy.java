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

public class Greedy extends Basic{
    public Bag createBag(List<Integer> cards, int money, int round){
        Bag createdBag = super.createBag(cards, money);

        for(int i = 0; i < createdBag.getBag().size(); i++){
            cards.remove(createdBag.getBag().get(i));
        }

        if(((round % 2) == 1) && (createdBag.getBag().size() < 8)){
            BasicUtils basicUtils = new BasicUtils();
            GetCards getCards = new GetCards();
            Filter filter = new Filter();

            List<Integer> illegalCards = getCards.getIllegalCards(cards);

            if(illegalCards.size() == 0){
                return createdBag;
            }

            Integer additional = basicUtils.
                    getMostProfitableIllegalCard(illegalCards);

            if(money - GoodsFactory.getInstance().getGoodsById(additional).getPenalty() <= 0){
                return createdBag;
            }

            createdBag.getBag().add(additional);

            return createdBag;


        }

        return createdBag;
    }

    public Integer declareBag(List<Integer> cards){
        CardsChecker cardsChecker = new CardsChecker();

        /* If the player has illegal cand legal cards at the same time
            it means that he played greedy strategy and we return tha
            first element which is the best legal card
         */
        if(cardsChecker.checkForLegalCards(cards) && cardsChecker.
            checkForIllegalCards(cards)){
            return cards.get(0);
        }
        else{
            return super.declareBag(cards);
        }
    }

    public InspectionOutcome inspectBag(Player sheriff, Player player){
        InspectionOutcome outcome = new InspectionOutcome();

        int bribe = player.getOfferedBribe();

        if(bribe != 0){
            outcome.setMoneyToPayToSheriff(bribe);
            outcome.setPassedGoods(new ArrayList<Integer>(player.getCards()));

            return outcome;
        }
        else{
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
}
