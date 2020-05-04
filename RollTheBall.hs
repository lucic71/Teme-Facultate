{-# OPTIONS_GHC -Wall #-}
{-# LANGUAGE EmptyDataDecls, MultiParamTypeClasses,
             TypeSynonymInstances, FlexibleInstances,
             InstanceSigs #-}
module RollTheBall where
import Pipes
import ProblemState

import Data.Array
import Data.Maybe

{-
    Direcțiile în care se poate mișca o piesa pe tablă
-}

data Directions = North | South | West | East
    deriving (Show, Eq, Ord)

{-
    Sinonim tip de date pentru reprezetarea unei perechi (Int, Int)
    care va reține coordonatele celulelor de pe tabla de joc
-}

type Position = (Int, Int)

{-
    Sinonim pentru fixarea unui Cell.
-}

type Fixed = Bool

{-
    Sinonim pentru continutul unui Cell.
-}

type Pipe = Char


{-
    Folosite in connection pentru a determina conectivitatea dintre doua pipe-uri.

    Sunt declarate aici pentru ca daca le definesc in Pipes.hs checkerul de pe 
    vmchecker o sa imi supraincarce fisierul si n-o sa le mai citeasca corect.

-}

leftOriented :: [Char]
leftOriented = [botRight, topRight, startLeft, winLeft, horPipe]

rightOriented :: [Char]
rightOriented = [botLeft, topLeft, startRight, winRight, horPipe]

upOriented :: [Char]
upOriented = [botLeft, botRight, startUp, winUp, verPipe]

downOriented :: [Char]
downOriented = [topLeft, topRight, startDown, winDown, verPipe]

{-
    Tip de date pentru reprezentarea celulelor tablei de joc. O celula
    contine caracterul ce va reprezenta forma celulei si un boolean
    care va decide daca celula poate fi mutata sau nu.
-}

data Cell = Cell Pipe Fixed deriving (Show, Eq, Ord)

{-
    Getters pentru campurile din Cell.
-}

getPipe :: Cell -> Pipe
getPipe (Cell p _) = p

getFixed :: Cell -> Fixed
getFixed (Cell _ f) = f

{-
    Implementarea unor Cell-urilor goale si a unei functii care
    decide daca un Cell e gol sau nu.

-}

emptySpaceCell :: Cell
emptySpaceCell = Cell emptySpace False

emptyCellCell :: Cell
emptyCellCell = Cell emptyCell False

isEmptyCell :: Cell -> Bool
isEmptyCell cell
    | cell == emptySpaceCell || cell == emptyCellCell = True
    | otherwise = False

{-
    Tip de date pentru reprezentarea nivelului curent. Un Level contine matricea
    ce contine Cell-uri si dimensiunile matricei: n - numar linii, m - numar
    coloane. La matrice va fi adaugata in plus o coloana noua ce va contine
    endl pentru a face afisarea mai usoara. In plus la pozitia n, m + 1 va fi
    un char \0000 utilizat pentru a nu mai adauga inca un endl la finalul matricei.
-}
data Level = Level (Array (Int, Int) Cell) Int Int
    deriving (Eq, Ord)

getLvlArr :: Level -> (Array (Int, Int) Cell)
getLvlArr (Level lvlArr _ _) = lvlArr

getLvlN :: Level -> Int
getLvlN (Level _ n _) = n

getLvlM :: Level -> Int
getLvlM (Level _ _ m) = m

{-
    *** Optional *** 
  
    Dacă aveți nevoie de o funcționalitate particulară,
    instantiați explicit clasele Eq și Ord pentru Level.
    În cazul acesta, eliminați deriving (Eq, Ord) din Level.
-}

{-
    *** TODO ***

    Instanțiati Level pe Show. 
    Atenție! Fiecare linie este urmată de \n (endl in Pipes).

    ###########################################################################

    Pentru afisare ia toate valorile de tip Pipe din lista de elemente
    ale matrice si afiseaza-le.
-}

instance Show Level 
    where show (Level lvlArr _ _) = "\n" ++ map getPipe (elems lvlArr)

{-
    *** TODO ***
    Primește coordonatele colțului din dreapta jos a hărții.
    Intoarce un obiect de tip Level în care tabla este populată
    cu EmptySpace. Implicit, colțul din stânga sus este (0,0)

    ###########################################################################

    Pe langa elementele de tip Pipe normale, mai adauga o coloana cu
    Pipe-uri endl pentru a face afisarea mai usoara. Pe pozitia n, m + 1
    adauga un caracter 0 pentru a nu afisa un new line degeaba.

    lvlArr     - matricea normala fara endl sau zero la final
    lvlArrEndl - matricea cu o coloana de endl adaugata
-}

emptyLevel :: Position -> Level
emptyLevel (x, y)
    | x <= 0 || y <= 0 = error "Pozitia dorita e invalida."

    | otherwise = Level lvlArrEndl x y
        where
            lvlArr = array ((0, 0), (x, y + 1))
                [((i, j), Cell emptySpace False) | i <- [0..x], j <- [0..y + 1]]
            lvlArrEndl = lvlArr     // [((i, y + 1), Cell endl False) | i <- [0..x]]

{-
    *** TODO ***

    Adaugă o celulă de tip Pipe în nivelul curent.
    Parametrul char descrie tipul de tile adăugat: 
        verPipe -> pipe vertical
        horPipe -> pipe orizontal
        topLeft, botLeft, topRight, botRight -> pipe de tip colt
        startUp, startDown, startLeft, startRight -> pipe de tip initial
        winUp, winDown, winLeft, winRight -> pipe de tip final

    Parametrul Position reprezintă poziția de pe hartă la care va fi adaugată
    celula, dacă aceasta este liberă (emptySpace).

    ###########################################################################

    Daca pipe-ul nou ce vrea sa fie adaugat e de tip start sau win atunci el
    trebuie setat ca fixat, altfel el trebuie setat ca nefixat.
-}

addCell :: (Char, Position) -> Level -> Level
addCell (pipe, (x, y)) (Level lvlArr n m)
    | x < 0 || y < 0 || x > n || y > m = (Level lvlArr n m)

    | elem pipe startCells || elem pipe winningCells =
        Level (lvlArr // [((x, y), Cell pipe True)]) n m

    | otherwise = Level (lvlArr // [((x, y), Cell pipe False)]) n m



{-
    *** TODO *** 

    Primește coordonatele colțului din dreapta jos al hărții și o listă de 
    perechi de tipul (caracter_celulă, poziția_celulei).
    Întoarce un obiect de tip Level cu toate celeule din listă agăugate pe
    hartă.
    Observatie: Lista primită ca parametru trebuie parcursă de la dreapta 
    la stanga.

    ###########################################################################

    Adauga fiecare pipe in initLevel folosind foldr.
-}
 
createLevel :: Position -> [(Char, Position)] -> Level
createLevel dimension pipes = foldr addCell initLevel pipes
    where initLevel = emptyLevel dimension


{-
    *** TODO ***

    Mișcarea unei celule în una din cele 4 direcții 
    Schimbul se poate face doar dacă celula vecină e goală (emptySpace).
    Celulele de tip start și win sunt imutabile.

    Hint: Dacă nu se poate face mutarea puteți lăsa nivelul neschimbat.

    ###########################################################################

    Pentru fiecare caz trebuie verificat daca celula actuala poate sa fie
    mutata in directia dorita(verificam daca e fixata si daca nu se afla la
    extremele matrice) si deasemenea verificam daca veciul ei poate fi mutat
    (trebuie sa fie un emptySpace).
-}

moveCell :: Position -> Directions -> Level -> Level
moveCell (x, y) direction (Level lvlArr n m)
    
    | direction == North && getFixed cell == False && x > 0 =
        let upperCell = lvlArr ! (x - 1, y) in
            case getPipe upperCell == emptySpace of
                True  -> Level (lvlArr // [((x, y), upperCell), ((x - 1, y), cell)]) n m
                False -> Level lvlArr n m

    | direction == South && getFixed cell == False && x < n =
        let lowerCell = lvlArr ! (x + 1, y) in
            case getPipe lowerCell == emptySpace of
                True  -> Level (lvlArr // [((x, y), lowerCell), ((x + 1, y), cell)]) n m
                False -> Level lvlArr n m

    | direction == West && getFixed cell == False && y > 0 =
        let leftCell = lvlArr ! (x, y - 1) in
            case getPipe leftCell == emptySpace of
                True  -> Level (lvlArr // [((x, y), leftCell), ((x, y - 1), cell)]) n m
                False -> Level lvlArr n m

    | direction == East && getFixed cell == False && y < n =
        let rightCell = lvlArr ! (x, y + 1) in
            case getPipe rightCell == emptySpace of
                True  -> Level (lvlArr // [((x, y), rightCell), ((x, y + 1), cell)]) n m
                False -> Level lvlArr n m

    | otherwise = Level lvlArr n m

        where cell = lvlArr ! (x, y)

{-
    *** TODO ***

    Verifică dacă două celule se pot conecta.
    Atenție: Această funcție se aplică de la stânga la 
    dreapta(nu este comutativă).

    ex: connection botLeft horPipe = True (╚═)
        connection horPipe botLeft = False (═╚)

    ###########################################################################

    Verifica daca Pipe urile p1 si p2 se indreapta una catre alta din directii
    diferite de pe aceeasi axa si returneaza True. Altfel daca amandoua se 
    indreapta in aceeasi directie sau directile sunt pe axe diferite returneaza False.
-}
connection :: Cell -> Cell -> Directions -> Bool
connection c1 c2 d
    | elem p1 leftOriented  == True && elem p2 rightOriented == True && d == East  = True
    | elem p1 rightOriented == True && elem p2 leftOriented  == True && d == West  = True
    | elem p1 upOriented    == True && elem p2 downOriented  == True && d == North = True
    | elem p1 downOriented  == True && elem p2 upOriented    == True && d == South = True

    | otherwise = False
        where
            p1 = getPipe c1
            p2 = getPipe c2

{-
    *** TODO ***

    Va returna True dacă jocul este câștigat, False dacă nu.
    Va verifica dacă celulele cu Pipe formează o cale continuă de la celula
    de tip inițial la cea de tip final.
    Este folosită în cadrul Interactive.

    ###########################################################################

    Gaseste mai intai pozitia Cell-ului de start, Daca sunt mai multe Cell-uri
    de start sau nu e niciunul atunci returneaza False.

    Mergi in wonLevelHelper si incearca fiecare vecin(North, West, South, East)
    sa vezi daca contine un pipe prin care se poate muta bila, daca da mergi
    recursiv prin fiecare pipe pana ajungi la un winningPipe. Altfel daca niciun
    vecin nu e valid inseamna ca nivelul nu are o configuratie de castig valida
    inca. Mai mult de atat trebuie tinut cont de pipe-urile prin care deja am mers
    deci e tinuta o lista de pozitii vizitate pentru a evita ciclarea la infinit.
    wonLevelHelper va intoarce True atunci cand Cell-ul d ela pozitia curenta
    contine un winningPipe.

-}
wonLevelHelper :: Level -> (Int, Int) -> [(Int, Int)] -> Bool
wonLevelHelper (Level lvlArr n m) (x, y) visited
    -- Am ajuns la winningCell.
    | elem (getPipe $ lvlArr ! (x, y)) winningCells = True

    -- Verifica vecinul de la North.
    | x - 1 >= 0 && isEmptyCell (lvlArr ! (x - 1, y)) == False && 
    elem (x - 1, y) visited == False && 
    connection (lvlArr ! (x, y)) (lvlArr ! (x - 1, y)) North == True = 
        wonLevelHelper (Level lvlArr n m) (x - 1, y) ((x, y) : visited)

    -- Verifica vecinul de la West.
    | y <= m && isEmptyCell (lvlArr ! (x, y + 1)) == False &&
    elem (x, y + 1) visited == False &&
    connection (lvlArr ! (x, y)) (lvlArr ! (x, y + 1)) West == True =
        wonLevelHelper (Level lvlArr n m) (x, y + 1) ((x, y) : visited)

    -- Verifica vecinul de la South.
    | x + 1 <= n && isEmptyCell (lvlArr ! (x + 1, y)) == False &&
    elem (x + 1, y) visited == False &&
    connection (lvlArr ! (x, y)) (lvlArr ! (x + 1, y)) South == True =
        wonLevelHelper (Level lvlArr n m) (x + 1, y) ((x, y) : visited)

    -- Verifica vecinul de la East.
    | y - 1 >= 0 && isEmptyCell (lvlArr ! (x, y - 1)) == False &&
    elem (x, y - 1) visited == False &&
    connection (lvlArr ! (x, y)) (lvlArr ! (x, y - 1)) East == True = 
        wonLevelHelper (Level lvlArr n m) (x, y - 1) ((x, y) : visited)

    -- Daca niciunul dintre vecini nu e valid, returneaza False.
    | otherwise = False

getStartCellPos :: Level -> Maybe (Int, Int)
getStartCellPos (Level lvlArr _ _)
    | length posList /= 1 = Nothing
    | otherwise           = Just (head posList)

    -- Creaza o lista cu elementele din Array si cauta Cellul care contine
    -- un pipe de tip Start.
    where posList = [fst x | x <- (assocs $ lvlArr), elem (getPipe $ snd x) startCells]


wonLevel :: Level -> Bool
wonLevel (Level lvlArr n m)
    | startPosJust == Nothing = False
    | otherwise = wonLevelHelper (Level lvlArr n m) startPos []

    where 
        startPosJust = getStartCellPos (Level lvlArr n m)
        startPos = fromJust startPosJust
        
{-
    ###########################################################################

    successors - Ia fiecare pozitie in parte si incearca sa vezi daca o mutare
    in orice directie va schimba configuratia nivelului. Daca o schimba inseamna
    ca e o mutare ce o adaugam in lista de succesori, altfel ignora.

    isGoal - E egal cu functia wonLevel.

    reverseAction - Daca actiunea pentru care vrem sa facem reverse e una valida
    (adica miscarea nu scoate pipe-ul din matricea nivelului) atunci inversam
    miscarea, altfel intoarce configuratia de input.
-}

instance ProblemState Level (Position, Directions) where
    successors lvl = [(((i, j), d), moveCell (i, j) d lvl) | i <- [0 .. getLvlN lvl], 
        j <- [0 .. getLvlM lvl], d <- [North, West, South, East], 
        (moveCell (i, j) d lvl == lvl) == False]

    isGoal = wonLevel

    reverseAction (((x, y), d), (Level lvlArr n m))
        | x > 0 && d == North = (((x - 1, y), South), moveCell (x, y) North (Level lvlArr n m))

        | x < n && d == South = (((x + 1, y), North), moveCell (x, y) South (Level lvlArr n m))

        | y > 0 && d == West  = (((x, y - 1), East),  moveCell (x, y) East (Level lvlArr n m))

        | y < m && d == East  = (((x, y + 1), West),  moveCell (x, y) West (Level lvlArr n m))

        | otherwise = (((x, y), d), Level lvlArr n m)
