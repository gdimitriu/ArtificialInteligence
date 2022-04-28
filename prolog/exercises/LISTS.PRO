DOMAINS
elem=integer
list=elem*

PREDICATES
% sum of the elements of a list
sum(list,elem).
sum(list,elem,elem).
% sum of two lists
sum2l(list,list,list).
% add one element to a list 
sumN(list,elem,list).
% product of tho lists
prod2l(list,list,list).
% elimination of one element from a list
eliml(list,elem,list).
% find the minimum in a list
min(list,elem).
min(list,elem,elem).
% find the maximum in a list
max(list,elem).
max(list,elem,elem).
% create a list from a list by adding the two conssecutive elements
conssec(list,list).
% create a list from a list by adding the odd and even elements
sumoe(list,list).
sumoe(list,list,list).
% extract from a list the evens elements into a new list
even(list,list).
% extract from a list the odds elements into a new list
odd(list,list).
% replace an element from a list with another element
replace(list,elem,list,elem).

CLAUSES
% sum of the elements of a list 
sum(X,R):-sum(X,0,R).
sum([H|T],B,R):-B1=H+B,sum(T,B1,R).
sum([],B,R):-R=B,!.
% sum of two lists
sum2l([H1|T1],[H2|T2],[H3|T3]):-H3=H1+H2,sum2l(T1,T2,T3).
sum2l([],L,L):-!.
sum2l(L,[],L):-!.
% add one integer to a list 
sumN([H1|T1],N,[H2|T2]):-H2=H1+N,sumN(T1,N,T2).
sumN([],N,[]):-!.
% product of two list
prod2l([H1|T1],[H2|T2],[H3|T3]):-H3=H2*H1,prod2l(T1,T2,T3).
prod2l([],L,L):-!.
prod2l(L,[],L):-!.
% elimination of one element from a list
eliml([H1|T1],X,[H3|T3]):-H1<>X,H3=H1,eliml(T1,X,T3).
eliml([H1|T1],X,Y):-H1=X,eliml(T1,X,Y).
eliml([],X,[]):-!.
% find the minimum in a list
min([H|T],X):-min(T,H,X).
min([H|T],X,R):-H<X,X1=H,min(T,X1,R).
min([H|T],X,R):-H>=X,min(T,X,R).
min([],X,X):-!.
% find the maximum in a list
max([H|T],X):-max(T,H,X).
max([H|T],X,R):-H>X,X1=H,max(T,X1,R).
max([H|T],X,R):-H<=X,max(T,X,R).
max([],X,X):-!.
% create a list from a list by adding the two conssecutive elements
conssec([H1|[H2|T1]],[H3|T3]):-H3=H1+H2,conssec(T1,T3).
conssec([],[]):-!.
% create a list form a list by adding the odd and even elements
sumoe(X,Y):-sumoe(X,Y,[0,0]).
sumoe([H1|[H2|T1]],[O|[E|T2]],[Ob|[Eb|T3]]):-O1=Ob+H1,E1=Eb+H2,sumoe(T1,[O|[E|T2]],[O1|[E1|T3]]).
sumoe([],L,L):-!.
% extract from a list the evens elements into a new list
even([H1|[H2|T1]],[H|T]):-H=H2,even(T1,T).
even([],[]):-!.
% extract from a list the odds elements into a new list
odd([H1|[H2|T1]],[H|T]):-H=H1,odd(T1,T).
odd([],[]):-!.
% replace an element from a list with another element
replace([H1|T1],X,[H2|T2],Y):-H1<>X,H2=H1,replace(T1,X,T2,Y).
replace([H1|T1],X,[H2|T2],Y):-H1=X,H2=Y,replace(T1,X,T2,Y).
replace([],X,[],Y):-!.