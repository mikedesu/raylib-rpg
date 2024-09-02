CC=g++
#OBJ_FILES=Game.o TitleScene.o GameplayScene.o DungeonFloor.o PopupManager.o Tile.o Sprite.o
OBJ_FILES=Game.so TitleScene.so GameplayScene.so DungeonFloor.o PopupManager.so Tile.so Sprite.so Entity.so 
OBJ_FILES_WITH_MAIN=main.o $(OBJ_FILES)
FLAGS=-Wall 
SHARED_LIB_FLAGS=-Wall -g -fPIC 
LIBS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lSDL2 -lSDL2_mixer

all: game 
game: $(OBJ_FILES_WITH_MAIN)
	$(CC) $^ $(FLAGS) $(LIBS) -o $@
Game.so: Game.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
TitleScene.so: TitleScene.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
GameplayScene.so: GameplayScene.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
DungeonFloor.o: DungeonFloor.cpp
	$(CC) $^ $(FLAGS) -c -o $@
PopupManager.so: PopupManager.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
Tile.so: Tile.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
Sprite.so: Sprite.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
Entity.so: Entity.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
main.o: main.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@

unit_tests.c: MyTestSuite.h
	cxxtestgen --error-printer -o $@ $^
test: unit_tests.c $(OBJ_FILES)
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

clean:
	rm -f *.o *.so game test unit_tests.c
