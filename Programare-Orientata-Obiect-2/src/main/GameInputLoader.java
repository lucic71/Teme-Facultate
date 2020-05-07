package main;

import fileio.FileSystem;
import utils.Utils;

import java.util.ArrayList;
import java.util.List;

final class GameInputLoader {
    private final String mInputPath;
    private final String mOutputPath;

    GameInputLoader(final String inputPath, final String outputPath) {
        this.mInputPath = inputPath;
        this.mOutputPath = outputPath;
    }

    GameInput load() {
        int mMatrixLines = 0;
        int mMatrixCols = 0;

        List<String> lineLands = new ArrayList<>();

        int heroesNo = 0;
        List<StringBuilder> lineHeroes = new ArrayList<>();

        int roundsNo = 0;
        List<String> lineRounds = new ArrayList<>();
        List<String> lineAngels = new ArrayList<>();

        try {
            final FileSystem fs = new FileSystem(mInputPath, mOutputPath);

            // first line
            mMatrixLines = fs.nextInt();
            mMatrixCols = fs.nextInt();

            // next mMatrixLines lines
            for (int i = 0; i < mMatrixLines; i++) {
                lineLands.add(fs.nextWord());
            }

            // heroes number line
            heroesNo = fs.nextInt();

            // next heroesNo lines
            for (int i = 0; i < heroesNo; i++) {
                lineHeroes.add(new StringBuilder());

                for (int j = 0; j < 3; j++) {
                    lineHeroes.get(i).append(fs.nextWord() + " ");
                }
            }

            // rounds number line
            roundsNo = fs.nextInt();

            // next roundsNo lines reading rounds info
            for (int i = 0; i < roundsNo; i++) {
                lineRounds.add(fs.nextWord());
            }

            // next roundsNo lines reading angel info
            for (int i = 0; i < roundsNo; i++) {
                // read the first number from a line
                int angelPerRound = fs.nextInt();
                StringBuilder round = new StringBuilder();

                // read the angels
                for (int j = 0; j < angelPerRound; j++) {
                    round.append(fs.nextWord() + " ");
                }

                lineAngels.add(i, round.toString());
            }

            fs.close();

        } catch (final Exception e) {
            e.printStackTrace();
        }

        assert mMatrixLines != 0 : "Error reading number of lines";
        assert mMatrixCols != 0 : "Error reading number of columns";

        assert lineLands.size() != 0 : "Error reading lands";

        assert heroesNo != 0 : "Error reading number of heroes";
        assert lineHeroes.size() != 0 : "Error reading heroes stats";

        assert roundsNo != 0 : "Error reading number of rounds";
        assert lineRounds.size() != 0 : "Error reading rounds stats";

        return new GameInput(mMatrixLines, mMatrixCols, lineLands,
                heroesNo, Utils.strBuilderArrToStrArr(lineHeroes), roundsNo,
                lineRounds, lineAngels);
    }

}
