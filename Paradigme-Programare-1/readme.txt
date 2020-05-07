I. Structuri folosite
---------------------
Structura principala folosita pentru aceasta tema este state, care inglobeaza
alte structuri cum ar fi bird, pipe, variables si abilities. Valoarea initiala
a acestei structuri se poate obtine prin apelul functiei get-initial-state,
care contine toate datele necesare de la momentul 0 al jocului.

II. Logica jocului
------------------
Pentru a eficientiza timpul si efortul de lucru am impartit scheletul de cod
in mai multe parti care se ocupa cu lucruri diferite ce vor fi puse cap la
cap in final.

1. BIRD
In prima parte a TODO-urilor, fisierul main se ocupa cu definirea getterilor
si logicii miscarii bird-ului, ca in final sa rezulte functia next-state-bird
care poate asigura functionarea bird-ului pe parcursul jocului.

2. PIPES
Logica pentru pipes se asemana cu cea de la bird, elementul nou fiind ca se
definesc functiile de move-pipes, clean-pipes si add-more-pipes care se
integreaza in next-state-pipes. Aici am folosit foarte mult functionale precum
map sau filter si functia struct-copy pentru a obtine structuri noi.

3. SCORE
Pentru aceasta parte a fost nevoie doar de un getter si integrarea variabilei
score in state.

4.COLLISIONS
Pentru verificarea coliziunilor am integrat functiile check-ground-collision
si check-pipe-collision in invalid-state?. Implementarea a fost relativ usoara,
deoarece in schelet a fost data functia check-collision-rectangles care ajuta
la detectarea coliziunilor dintre dreptunghiuri.

5. DRAWING
Pentru punerea pe ecran a imaginilor am definit mai multe functii cum ar fi
place-ground sau place-bird care apelate in draw-frame au facut posibila
afisarea bird-ului, pipe-urilor, etc. Dificultatea aici a fost functia
place-pipes deoarece am incercat o abordare cu place-images care insa n-a mers.
Asa ca am luat fiecare pipe in parte, am calculat coordonatele si le-am pus
peste scena goala.

6. ABILITIES
Pentru abilities am folosit functiile delay si force pentru a intarzia
calcularea lor pana in momentul cand era nevoie, deoarece altfel checkerul
nu-mi dadea punctajul pentru subpunctele respective. Aici nu am reusit sa
implementez afisarea pe ecran a abilitatilor, ci doar logica obtinerii
abilitatilor vizibile.


III. Concluzii si rezultate
---------------------------
Cand am incercat sa testez pe checker am observat ca pentru ultimul test de
la bonus, checkerul da o eroare pe care n-am putut sa o rezolv. El valideaza
toate testele pana la logica punerii abilitatilor pe ecran. Per total a fost
o tema foarte folositoare deoarece mi-a intarit cunostintele despre functionale,
intarzieri, structuri, etc in racket.
