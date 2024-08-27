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









