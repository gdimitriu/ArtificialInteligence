PREDICATES
% nul arrity
run.
% two arrity
run2a(integer,integer).
% infinite loop
run1.
repeat1.
% repeat until
run2.
repeat2.
until2.

CLAUSES
run:-X=3,write(X),nl;X=4,write(X),nl.
run2a(Y,X):-X=3,write(X),nl;X=4,write(X),nl.
run1:-repeat1,nl,readreal(X),write(X),fail.
repeat1.
repeat1:-repeat1.
run2:-repeat2,nl,readreal(X),write(X),fail.
repeat2.
repeat2:-until2,repeat2.
until2:-nl,write("Do you want to repeat?(y/n)"),readchar(Y),Y='y'.
