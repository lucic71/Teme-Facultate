package utils;

import angel.AbstractAngel;
import angel.AngelsFactory;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public final class Utils {
    public static List<String> strBuilderArrToStrArr(final List<StringBuilder> array) {
        List<String> stringArray = new ArrayList<>();

        for (StringBuilder stringBuilder : array) {
            stringArray.add(stringBuilder.toString());
        }

        return stringArray;
    }

    public static List<AbstractAngel> strListToAbstrAngelList(final String angels) {
        List<AbstractAngel> abstractAngels = new ArrayList<>();
        List<String> angelsList = Arrays.asList(angels.split(" "));
        AngelsFactory factory = AngelsFactory.getInstance();

        for (int i = 0; i < angelsList.size(); i++) {
            List<String> angelInfo = Arrays.asList(angelsList.get(i).split(","));
            if (angelInfo.size() == 1) {
                continue;
            }

            String name = angelInfo.get(0);
            int posX = Integer.parseInt(angelInfo.get(1));
            int posY = Integer.parseInt(angelInfo.get(2));

            abstractAngels.add(factory.getAngel(name, posX, posY));
        }

        return abstractAngels;
    }
}
