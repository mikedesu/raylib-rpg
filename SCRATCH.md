# scratch notes

- 27 Aug 2024 

...why isn't the position being pulled up for the player?
rather
why isnt the torch spawning at the player position after the change?



found it

i was pulling `last_tile_click_pos` hard-coded in `spawn_torch` instead of pulling from the parameter
lets fix that


discovered another bug
this time it involves the light-reduction when removing a torch
after removing all torches it should be the default lighting of 1

putting 4 in a square created black tiles
putting 3 in line created a black tile in the center
putting 3 in a corner-shape created a black tile right? lol

so it has to do with overlapping light sources

even putting two torches on one tile results in a black out
are we decreasing by too much?
i thought i made them the same...

i dont want to "hack" the solution to this in
for it to be "proper":

every torch addition must properly update the tile lighting
every torch removal must properly update the tile lighting

i thought i might do this by re-calculating the lighting on every tile in the dungeon on every turn
but we should not need to do this if the code is correct
which it isn't....yet
but we will get it

ok
it looks fixed but we have to light up the other directions as well
in the end we have several things to do in the lighting department that i have to figure out still:


1. how to do lighting for entity sprites on the tile
2. how to recursively light the tiles so that we do not light any tiles behind a wall, or dampen the light if it passes thru an entity or such
3. moving light sources

the problem with the black boxes is that we are calling the tile light decrease multiple times on the same tile
we fixed it for some cases but not others
meaning
we still have a more generic version to look for

lol thats one way to solve it
increase the differential between light increments
and set the default lighting to the minimum

lets try a default of 5 or higher......
i know the math wont be right but if it was, after removing all the torches the tiles light level should be the default again

temporarily setting them to else ifs because that worked but resulted in top-left lighting only
cant hardcode this because we need to handle different light increments for future torches, rings, etc
gonna take a break for tonight :D

----------

fixed the main lighting problem kind of...

1. increasing/decreasing light of surrounding tiles requires specific position-checks to make sure we dont double-increment the same tiles
2. have to have a relatively high maximum lighting so that the increment is rather small
3. still can run into problems when adding multiple torches to one tile

we'd also like to have a light source follow entities on screen

1. lighting per-tile must become a function of
    - is there a torch on the tile?
    - does the player have any light-increasing items in their inventory?
    - etc

we must also separate the light_level from the light distance

currently, we are joining the two values together
so, if you lay a torch down and ive set it to update with light level 3, then it updates the 3 surrounding tiles
we can pull out distance and pass it as a parameter to start with, and then we will move on to formalizing entities such that

1. player is an entity subclass called `Humanoid`
2. torch is an entity subclass called `Item`

we also want some guarantee on the order of items returned when iterating over tile entities








