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

the next classes wont be as easy as Game...
a lot of other classes depend on each other
Game is at the top of the hierarchy so it 
was easiest

For the `Scene` class, because it is a 
parent class, we will need to do:

- `TitleScene`
- `GameplayScene`

skip `GameoverScene`, just remove it from the project for now.
