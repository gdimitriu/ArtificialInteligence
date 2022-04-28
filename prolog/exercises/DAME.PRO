DOMAINS
pereche=integer *
perechi=pereche *
PREDICATES
solutie(perechi)
nuataca(pereche,perechi)
nuataca1(pereche,pereche)
membru(integer,pereche)
CLAUSES
solutie([]).
solutie([[X,Y]|Altele]):-solutie(Altele),membru(Y,[1,2,3,4,5,6,7,8]),nuataca([X,Y],Altele).
nuataca([X,Y],[]).
nuataca([X,Y],[Prima|Celelalte]):-nuataca1([X,Y],Prima),nuataca([X,Y],Celelalte).
nuataca1([X,Y],[X1,Y1]):-X<>X1,Y<>Y1,X+Y<>X1+Y1,X-Y<>X1-Y1.
membru(E,[E|Tail]):-!.
membru(E,[_|Tail]):-membru(E,Tail).