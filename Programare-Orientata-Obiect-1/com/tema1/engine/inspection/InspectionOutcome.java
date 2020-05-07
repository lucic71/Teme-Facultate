package com.tema1.engine.inspection;

import java.util.ArrayList;
import java.util.List;

public class InspectionOutcome {

    private List<Integer> passedGoods;
    private List<Integer> restrictedGoods;

    private Integer moneyToPayToSheriff;
    private Integer moneyToGetFromSheriff;

    public InspectionOutcome(){
        this.passedGoods = new ArrayList<Integer>();
        this.restrictedGoods = new ArrayList<Integer>();

        this.moneyToPayToSheriff = 0;
        this.moneyToGetFromSheriff = 0;
    }


    public List<Integer> getPassedGoods() {
        return passedGoods;
    }

    public void setPassedGoods(List<Integer> passedGoods) {
        this.passedGoods = passedGoods;
    }

    public List<Integer> getRestrictedGoods() {
        return restrictedGoods;
    }

    public void setRestrictedGoods(List<Integer> restrictedGoods) {
        this.restrictedGoods = restrictedGoods;
    }

    public Integer getMoneyToPayToSheriff() {
        return moneyToPayToSheriff;
    }

    public void setMoneyToPayToSheriff(Integer moneyToPayToSheriff) {
        this.moneyToPayToSheriff = moneyToPayToSheriff;
    }

    public Integer getMoneyToGetFromSheriff() {
        return moneyToGetFromSheriff;
    }

    @Override
    public String toString() {
        return "InspectionOutcome{" +
                "passedGoods=" + passedGoods +
                ", restrictedGoods=" + restrictedGoods +
                ", moneyToPayToSheriff=" + moneyToPayToSheriff +
                ", moneyToGetFromSheriff=" + moneyToGetFromSheriff +
                '}';
    }

    public void setMoneyToGetFromSheriff(Integer moneyToGetFromSheriff) {
        this.moneyToGetFromSheriff = moneyToGetFromSheriff;
    }
}
