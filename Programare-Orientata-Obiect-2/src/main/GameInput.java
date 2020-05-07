package main;

import map.TerrainMap;

import java.util.List;

public final class GameInput {

    private final List<List<Character>> map;

    private final int heroesNo;
    private final List<String> heroes;

    private int roundsNo;
    private final List<String> rounds;
    private final List<String> angels;

    GameInput(final int matrixLines, final int matrixCols, final List<String> lands,
              final int heroesNo, final List<String> heroes, final int roundsNo,
              final List<String> rounds, final List<String> angels) {
        this.map = TerrainMap.getTerrainMap(matrixLines, matrixCols, lands);

        this.heroesNo = heroesNo;
        this.heroes = heroes;

        this.roundsNo = roundsNo;
        this.rounds = rounds;
        this.angels = angels;
    }

    public List<List<Character>> getMap() {
        return map;
    }

    public int getHeroesNo() {
        return heroesNo;
    }

    public List<String> getHeroes() {
        return heroes;
    }

    public int getRoundsNo() {
        return roundsNo;
    }

    public List<String> getRounds() {
        return rounds;
    }

    public List<String> getAngels() {
        return angels;
    }

}
