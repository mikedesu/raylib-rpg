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

im working on the Sprite class right now
while doing this off-stream i accidentally
messed up a function and caused the sprite
to not draw with the proper scale
so im going to be slow and methodical about
this
