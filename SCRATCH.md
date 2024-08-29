no talking just coding
refactoring in progress...

---------------------------------------

lets do some re-factoring
ive been going thru each object moving code 
into their header files and in some cases
im able to eliminate a .cpp file entirely
this is resulting in a smaller binary and
less LOC so potentially overall good
if i stick to a specific rule:

- if the method is small then header

im working on the Scene class right now
its a rather small class and another good
test case for this reduction method

