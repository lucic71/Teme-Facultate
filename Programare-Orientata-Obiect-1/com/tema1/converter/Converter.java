package com.tema1.converter;

import com.tema1.engine.inspection.InspectionOutcome;
import com.tema1.player.Player;
import com.tema1.strategy.Bag;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.List;

public class Converter {
    public Bag convertStringToFunctionCreateBag(String className, List<Integer> cards,
                                                int money, final int round){
        String classNameCopy = className;

        className = classNameCopy.substring(0, 1).toUpperCase() + className.substring(1);
        className = "com.tema1.strategy." + className;

        final String methodName = "createBag";

        Class<?> strategyClass = null;
        try {
            strategyClass = Class.forName(className);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }

        Object strategy = null;
        try {
            assert strategyClass != null;
            strategy = strategyClass.getDeclaredConstructor().newInstance();
        } catch (InstantiationException | IllegalAccessException |
                InvocationTargetException | NoSuchMethodException e) {
            e.printStackTrace();
        }

        if(classNameCopy.equals("greedy")){
            Class<?>[] paramTypes = {List.class, int.class, int.class};
            Method createBag = null;
            try {
                assert strategy != null;
                createBag = strategy.getClass().getMethod(methodName, paramTypes);
            } catch (NoSuchMethodException e) {
                e.printStackTrace();
            }

            try {
                assert createBag != null;
                return (Bag) createBag.invoke(strategy, cards, money, round);
            } catch (IllegalAccessException | InvocationTargetException e) {
                e.printStackTrace();
            }
        }

        Class<?>[] paramTypes = {List.class, int.class};
        Method createBag = null;
        try {
            assert strategy != null;
            createBag = strategy.getClass().getMethod(methodName, paramTypes);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }

        try {
            assert createBag != null;
            return (Bag) createBag.invoke(strategy, cards, money);
        } catch (IllegalAccessException | InvocationTargetException e) {
            e.printStackTrace();
        }

        return null;
    }

    public Integer convertStringToFunctionDeclareBag(String className, List<Integer> cards){
        String classNameCopy = className;

        className = classNameCopy.substring(0, 1).toUpperCase() + className.substring(1);
        className = "com.tema1.strategy." + className;

        final String methodName = "declareBag";

        Class<?> strategyClass = null;
        try {
            strategyClass = Class.forName(className);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }

        Object strategy = null;
        try {
            assert strategyClass != null;
            strategy = strategyClass.getDeclaredConstructor().newInstance();
        } catch (InstantiationException | IllegalAccessException |
                InvocationTargetException | NoSuchMethodException e) {
            e.printStackTrace();
        }

        Class<?>[] paramTypes = {List.class};
        Method declareBag = null;
        try {
            assert strategy != null;
            declareBag = strategy.getClass().getMethod(methodName, paramTypes);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }

        try {
            assert declareBag != null;
            return (Integer) declareBag.invoke(strategy, cards);
        } catch (IllegalAccessException | InvocationTargetException e) {
            e.printStackTrace();
        }

        return null;

    }

    public InspectionOutcome convertStringToFunctionInspectBag(String className, Player sheriff,
                                                               Player player){

        String classNameCopy = className;

        className = classNameCopy.substring(0, 1).toUpperCase() + className.substring(1);
        className = "com.tema1.strategy." + className;

        final String methodName = "inspectBag";

        Class<?> strategyClass = null;
        try {
            strategyClass = Class.forName(className);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }

        Object strategy = null;
        try {
            assert strategyClass != null;
            strategy = strategyClass.getDeclaredConstructor().newInstance();
        } catch (InstantiationException | IllegalAccessException |
                InvocationTargetException | NoSuchMethodException e) {
            e.printStackTrace();
        }

        Class<?>[] paramTypes = {Player.class, Player.class};
        Method inspectBag = null;
        try {
            assert strategy != null;
            inspectBag = strategy.getClass().getMethod(methodName, paramTypes);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }

        try {
            assert inspectBag != null;
            return (InspectionOutcome) inspectBag.invoke(strategy, sheriff, player);
        } catch (IllegalAccessException | InvocationTargetException e) {
            e.printStackTrace();
        }

        return null;

    }
}
