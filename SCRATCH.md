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

we've achieved some binary size reduction
and reduction of total line count
as a result of these methods
we've also sped up compilation time
as some .o files have been eliminated
due to moving all the code into the .h

---------------------------------------

more than once ive had the thought:
"i dont like how my autoformat is formatting my stuff"
so i will look at that during my off-time
it is getting late so lets call it a night 
after we make sure the build is good
