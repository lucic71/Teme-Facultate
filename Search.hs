{-# OPTIONS_GHC -Wall #-}
{-# LANGUAGE MultiParamTypeClasses #-}

module Search where

import ProblemState
import qualified Data.Set as S
import Data.List (sortBy)
import Data.Maybe

{-
    *** TODO ***

    Tipul unei nod utilizat în procesul de căutare. Recomandăm reținerea unor
    informații legate de:

    * stare;
    * acțiunea care a condus la această stare;
    * nodul părinte, prin explorarea căruia a fost obținut nodul curent;
    * adâncime
    * copiii, ce vor desemna stările învecinate
-}

data Node s a = Node s (Maybe a) (Maybe (Node s a)) Int [Node s a]
    deriving Eq

instance (Show s) => Show (Node s a)
    where show (Node s _ _ _ _) = show s

-- Instanta folosita in bidirBFSSearch pentru adaugarea in Set.
instance (Ord s, Eq a) => Ord (Node s a)
    where compare node1 node2 = compare (nodeState node1) (nodeState node2)

{-
    *** TODO ***
    Gettere folosite pentru accesul la câmpurile nodului
-}
nodeState :: Node s a -> s
nodeState (Node s _ _ _ _) = s

nodeAction :: Node s a -> Maybe a
nodeAction (Node _ a _ _ _) = a

nodeParent :: Node s a -> Maybe (Node s a)
nodeParent (Node _ _ p _ _) = p

nodeDepth :: Node s a -> Int
nodeDepth (Node _ _ _ d _) = d

nodeChildren :: Node s a -> [Node s a]
nodeChildren (Node _ _ _ _ c) = c

{-
    *** TODO ***

    Generarea întregului spațiu al stărilor
    Primește starea inițială și creează nodul corespunzător acestei stări,
    având drept copii nodurile succesorilor stării curente.

    ###########################################################################

    Pentru generare se foloseste functia createStateSpaceHelper care primeste
    starea s, actiunea a, parintele p si adancimea d si intoarce un Node s a.

    Functia umple primele 4 campuri cu datele primite la intrare, iar mai apoi
    mapeaza fiecare succesor, generat cu functia successors, la un Node a s.
    Primul camp din noul Node a s va fi starea succesorului, al doilea va fi
    un Maybe actiunea care a dus la starea succesorului, dupa care va urma un
    Maybe referinta la nodul parinte si in final adancimea incrementata.
-}

createStateSpaceHelper 
    :: (ProblemState s a, Eq s) => s -> Maybe a -> Maybe (Node s a) -> Int -> Node s a
createStateSpaceHelper s a p d = Node s a p d
    (map (\x -> createStateSpaceHelper (snd x) 
                                       (Just (fst x)) 
                                       (Just (createStateSpaceHelper s a p d)) 
                                       (d + 1)) 
         (successors s))


createStateSpace :: (ProblemState s a, Eq s) => s -> Node s a
createStateSpace s = createStateSpaceHelper s Nothing Nothing 0

{-
    *** TODO ***
   
    Primește un nod inițial și întoarce un flux de perechi formate din:
    * lista nodurilor adăugate în frontieră la pasul curent
    * frontiera

    ###########################################################################

    Pentru a realiza parcurgerea in adancime voi concatena frontierele nodului
    de start cu rezultatul functiei bfsHelper. 

    In functia bfsHelper algoritmul dupa care functioneaza parcurgerea e
    urmatorul:
        - extrage primul element din @frontier (echivalent cu operatia pop
            pe coada folosita in programarea iterativa a acestui algoritm)
        - creaza un nou tuplu numit level astfel:
            ([toti vecinii nodului extras],
             [lista de la pasul anterior fara primul element] :
             [toti vecinii nodului extras])
        - lega noul tuplu creat la rezultatul functiei
    
    Algoritmul se opreste atunci cand currFrontier sau frontier devin nule.

    Functia init din bfs sterge ultimul nod care e gol.
    Functia sortDepth sorteaza nodurile in functie de depth-ul lor.
-}

sortDepth :: Node s a -> Node s a -> Ordering
sortDepth node1 node2 = compare (nodeDepth node1) (nodeDepth node2)

bfsHelper :: (Ord s, Ord a) => ([Node s a], [Node s a]) -> [([Node s a], [Node s a])]
bfsHelper (currFrontier, frontier) 

    | null currFrontier && null frontier = [([], [])]
    | otherwise = 
        level : bfsHelper level
            where
                frontierHead = head frontier
                nodeChildrenSorted = sortBy sortDepth (nodeChildren frontierHead)
                level = (nodeChildrenSorted, tail frontier ++ nodeChildrenSorted)


bfs :: (Ord s, Ord a) => Node s a -> [([Node s a], [Node s a])]
bfs start = init $ startNode : bfsHelper startNode
    where startNode = ([start], [start])

{-
    *** TODO ***
  
    Primește starea inițială și finală și întoarce o pereche de noduri, reprezentând
    intersecția dintre cele două frontiere.

    ###########################################################################

    Functia bidirBFS se foloseste de functia bidirBFSHelper pentru a intoarece nodurile
    comune din BFS ul e start si de end.

    bidirBFS va primi fluxul BFS pentru nodul de start, fluxul BFS pentru nodul de end,
    un set cu nodurile deja vizitate din start si un set cu nodurile deja vizitate din
    end.

    Va calcula startFrontier si endFrontier care sunt frontierele BFS pentru pasul curent
    al cautarii, apo va calcula currStartVisited si currEndVisited care sunt reuniuni dintre
    nodurile deja vizitate si nodurile adaugate la pasul curent te traversare si in final
    commonVisited va tine nodurile comune care au fost vizitate si de currStartVisited si de
    currEndVisited.

    Mai apoi va decide daca setul de noduri comune e gol, si daca conditia e adevarata atunci
    trece la pasul urmator de recursivitate, avand fluxurile updatate, la fel si seturile.
    In final daca commonVisited nu e gol atunci ia nodul din commonVisited, adica commonNode
    si il cauta in currStartVisited si in currEndVisited pentru a returna o pereche.

    Am folosit functia findMin pentru a extrage un nod din set deoarece la level4 commonVisited
    avea doua elemente si a trebuit sa il extrag pe cel mai mic.
-}

bidirBFSHelper :: (Ord s, Ord a, Show s) => [([Node s a], [Node s a])] -> [([Node s a], [Node s a])]
    -> S.Set (Node s a) -> S.Set (Node s a) -> (Node s a, Node s a)
bidirBFSHelper startBFS endBFS startVisited endVisited
    | S.null commonVisited = 
        bidirBFSHelper (tail startBFS) (tail endBFS) currStartVisited currEndVisited
    | otherwise = 
        let commonNode      = S.findMin commonVisited
            commonNodeStart = S.findMin $ S.filter 
                                (\node -> nodeState node == nodeState commonNode)
                                currStartVisited
            commonNodeEnd   = S.findMin $ S.filter 
                                (\node -> nodeState node == nodeState commonNode) 
                                currEndVisited
            in

            (commonNodeStart, commonNodeEnd)

    where 
        startFrontier = fst $ head startBFS
        endFrontier   = fst $ head endBFS

        currStartVisited = S.union startVisited (S.fromList startFrontier)
        currEndVisited   = S.union endVisited   (S.fromList endFrontier)

        commonVisited    = S.intersection currStartVisited currEndVisited

bidirBFS :: (Ord s, Ord a, Show s) => Node s a -> Node s a -> (Node s a, Node s a)
bidirBFS start end = bidirBFSHelper (bfs start) (bfs end) S.empty S.empty


{-
    *** TODO ***

    Pornind de la un nod, reface calea către nodul inițial, urmând legăturile
    către părinți.

    Întoarce o listă de perechi (acțiune, stare), care pornește de la starea inițială
    și se încheie în starea finală.

    ###########################################################################

    Foloseste functia auxiliara extractPath' care merge din parinte in parinte pana da
    de un nod care are parintele null.

-}


extractPath :: (Eq a, Eq s) => Node s a -> [(Maybe a, s)]
extractPath node = extractPath' node []
    where
        extractPath' node' path
            | nodeParent node' == Nothing = (Nothing, nodeState node') : path
            | otherwise = extractPath' (fromJust $ nodeParent node')
                                       ((nodeAction node', nodeState node') : path)



{-
    *** TODO ***

    Pornind de la o stare inițială și una finală, se folosește de bidirBFS pentru a găsi
    intersecția dintre cele două frontiere și de extractPath pentru a genera calea.

    Atenție: Pentru calea gasită în a doua parcurgere, trebuie să aveți grijă la a asocia
    corect fiecare stare cu acțiunea care a generat-o.

    Întoarce o listă de perechi (acțiune, stare), care pornește de la starea inițială
    și se încheie în starea finală.

    ###########################################################################

    Extrage path-ul de la start pana la mijloc in pathStartMiddle si cel de la mijloc
    pana la end in pathMiddleEnd. Pentru cel din urma trebuie ajustate corect actiunile
    si starile. Pentru asta am folosit zipWith care uneste starea lui (tail pathMiddleEnd)
    cu reverse de actiunea lui pathMiddleEnd.
-}

solve :: (ProblemState s a, Ord s, Show s, Eq a, Ord a, Show a)
      => s          -- Starea inițială de la care se pornește
      -> s          -- Starea finală la care se ajunge
      -> [(Maybe a, s)]   -- Lista perechilor
solve start end = pathStartMiddle ++ pathMiddleEnd'
    where
        startNode = createStateSpace start
        endNode   = createStateSpace end

        bidirSearch = bidirBFS startNode endNode
        pathStartMiddle = extractPath $ fst bidirSearch
        pathMiddleEnd   = reverse $ extractPath $ snd bidirSearch

        pathMiddleEnd' = zipWith (\(_, s1) (a2, s2) -> let justA2 = fromJust a2
                                                           (revA2, _) = reverseAction (justA2, s2) in
                                                           (Just revA2, s1))
                                 (tail pathMiddleEnd) 
                                 pathMiddleEnd



