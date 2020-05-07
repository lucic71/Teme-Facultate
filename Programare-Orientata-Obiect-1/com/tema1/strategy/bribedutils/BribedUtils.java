package com.tema1.strategy.bribedutils;

public class BribedUtils {
    public int calculateBribe(final int illegalCards){

        if(illegalCards == 0){
            return 0;
        }
        else if(illegalCards <= 2){
            return 5;
        }
        else if(illegalCards > 2){
            return 10;
        }

        return -1;
    }
}
