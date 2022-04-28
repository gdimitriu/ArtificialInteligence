domains
termen=integer
predicates
sum(termen,termen,termen).
prod(termen,termen,termen).
/* normally factorial*/
factorial(termen,termen).
/* with last recursivity */
factorial(termen,termen,termen).
factorial1(termen,termen).
clauses
sum(X,Y,Z):-Z=X+Y.
prod(X,Y,Z):-Z=X*Y.
factorial(0,1):-!.
factorial(1,1):-!.
factorial(X,Y):-Z=X-1,factorial(Z,R),Y=X*R.
factorial(X,Y,Z):-R2=X*Y,R1=X-1,factorial(R1,R2,Z).
factorial(1,Y,Z):-Z=Y,!.
factorial(1,Z,Z):-!.
factorial1(X,Z):-factorial(X,1,Z).

