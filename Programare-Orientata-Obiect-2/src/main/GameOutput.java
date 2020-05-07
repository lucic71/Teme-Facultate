package main;

import fileio.FileSystem;
import hero.AbstractHero;

import java.io.IOException;
import java.util.List;

public final class GameOutput {
    private final String mInputPath;
    private final String mOutputPath;
    private FileSystem fs;

    public GameOutput(final String inputPath, final String outputPath) {
        this.mInputPath = inputPath;
        this.mOutputPath = outputPath;

        try {
            fs = new FileSystem(mInputPath, mOutputPath);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeDeadPlayer(AbstractHero killedHero, AbstractHero killerHero,
                                String killedHeroName, String killerHeroName) {
        try {
            String output = "Player " + killedHeroName + " " + killedHero.getNumberIdentifier()
                    + " was killed by " + killerHeroName + " " + killerHero.getNumberIdentifier();

            fs.writeWord(output);
            fs.writeNewLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeLevelUp(String heroName, int numberIdentifier, int level) {
        try {
            String output = heroName + " " + numberIdentifier + " reached level " + level;

            fs.writeWord(output);
            fs.writeNewLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeAngelAbility(String angelName, String action, String heroName, int numberIdentifier) {
        try {
            String output = angelName + " " + action + " " + heroName + " " + numberIdentifier;

            fs.writeWord(output);
            fs.writeNewLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeSpawnedAngel(String angelName, int posX, int posY) {
        try {
            String output = "Angel " + angelName + " was spawned at " + posX + " " + posY;

            fs.writeWord(output);
            fs.writeNewLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeRoundNumber(int roundNumber) {
        try {
            String output = "~~ Round " + roundNumber + " ~~";

            if (roundNumber != 1) {
                fs.writeNewLine();
            }
            fs.writeWord(output);
            fs.writeNewLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeLifeStateHero(AbstractHero hero, String heroName) {
        try {
            int numberIdentifier = hero.getNumberIdentifier();
            String output = null;

            if (hero.isAlive()) {
                output = "Player " + heroName + " " + numberIdentifier + " was brought to life"
                        + " by an angel";
            } else {
                output = "Player " + heroName + " " + numberIdentifier + " was killed"
                        + " by an angel";
            }

            fs.writeWord(output);
            fs.writeNewLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    void writeFinalResults(final List<AbstractHero> heroList) {
        try {
            fs.writeNewLine();
            fs.writeWord("~~ Results ~~");
            fs.writeNewLine();

            for (AbstractHero hero : heroList) {
                if (hero.getHp() == 0) {
                    // identifier
                    fs.writeCharacter(hero.getCharIdentifier());
                    fs.writeCharacter(' ');
                    // dead
                    fs.writeWord("dead");
                    fs.writeNewLine();
                } else {
                    // identifier
                    fs.writeCharacter(hero.getCharIdentifier());
                    fs.writeCharacter(' ');
                    // level
                    fs.writeInt(hero.getLevel());
                    fs.writeCharacter(' ');
                    // xp
                    fs.writeInt(hero.getXp());
                    fs.writeCharacter(' ');
                    // hp
                    fs.writeInt(hero.getHp());
                    fs.writeCharacter(' ');
                    // x position
                    fs.writeInt(hero.getPosX());
                    fs.writeCharacter(' ');
                    // y position
                    fs.writeInt(hero.getPosY());
                    fs.writeNewLine();
                }
            }

            fs.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
