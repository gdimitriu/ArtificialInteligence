predicates
prim(integer)
prim(integer,integer,integer)
clauses
prim(1). prim(2). prim(3).
prim(N):-N>=5,RadN=trunc(sqrt(N)),prim(N,2,RadN).
prim(N,RadN,RadN):-N/RadN<>trunc(N/RadN),!.
prim(N,Doi,RadN):-N/Doi<>trunc(N/Doi),Trei=Doi+1,prim(N,Trei,RadN).
