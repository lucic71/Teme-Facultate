package map;

import java.util.ArrayList;
import java.util.List;

public final class TerrainMap {
    private static TerrainMap instance = null;
    private List<List<Character>> map;

    private TerrainMap(final int lines, final int cols, final List<String> lands) {
        map = new ArrayList<>();

        // initialize the matrix
        for (int i = 0; i < lines; i++) {
            map.add(new ArrayList<>());
        }

        for (int i = 0; i < lines; i++) {
            for   (int j = 0; j < cols; j++) {
                map.get(i).add(lands.get(i).charAt(j));
            }
        }

    }

    public static List<List<Character>> getTerrainMap(final int lines, final int cols,
                                                      final List<String> lands) {
        if (instance == null) {
            instance = new TerrainMap(lines, cols, lands);
        }

        return instance.map;
    }
}
