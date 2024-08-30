no talking just coding
refactoring in progress...

---------------------------------------

implementing code hot swapping

- want to start converting existing classes
into structs so that I can start using 
code hot-swapping (the class mangling
is a big deal)

- convert class into struct
- convert class methods into functions
    - put the reference -OR- `smart_ptr` 
    to the struct as the first argument

For the `Scene` class, because it is a 
parent class, we will need to do:

- `GameplayScene`
    - Copy the methods over into a `.cpp`
    - `Scene`
    - `GameplayScene`

The game will be unplayable/unbuildable
during this period, but once we get it
in place, we will make sure we can 
build and run again once we transition
`Scene` over. 

Once GameplayScene is completed, we will be
able to build and run the game again.
