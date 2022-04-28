predicates
sum(real,real,real).
prod(real,real,real).
/* normal recursivity factorial*/
factorial(integer,integer).
/* with last recursivity */
factorial1(integer,integer,integer).
factorial1(integer,integer).
/* power at integer */
power(real,integer,real).
power(real,integer,real,real).
clauses
sum(X,Y,Z):-Z=X+Y.
prod(X,Y,Z):-Z=X*Y.
factorial(0,1):-!.
factorial(1,1):-!.
factorial(X,Y):-Z=X-1,factorial(Z,R),Y=X*R.
factorial1(X,Y,Z):-X>1,!,R2=X*Y,R1=X-1,factorial1(R1,R2,Z).
factorial1(1,Y,Z):-Z=Y,!.
factorial1(X,Z):-factorial1(X,1,Z).
power(0,X,0).
power(1,Y,1).
power(X,Y,Z):-power(X,Y,Z,1).
power(X,Y,Z,R):-Y>0,!,R2=R*X,R1=Y-1,power(X,R1,Z,R2).
power(X,0,Z,R):-Z=R,!.