CC=g++
OBJ_FILES=Game.o TitleScene.o GameplayScene.o DungeonFloor.o PopupManager.o Tile.o Sprite.o Entity.o
OBJ_FILES_WITH_MAIN=main.o $(OBJ_FILES)
FLAGS=-Wall 
SHARED_LIB_FLAGS=-Wall -g  
LIBS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lSDL2 -lSDL2_mixer

all: game 
game: $(OBJ_FILES_WITH_MAIN)
	$(CC) $^ $(FLAGS) $(LIBS) -o $@
Game.o: Game.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
TitleScene.o: TitleScene.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
GameplayScene.o: GameplayScene.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
DungeonFloor.o: DungeonFloor.cpp
	$(CC) $^ $(FLAGS) -c -o $@
PopupManager.o: PopupManager.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
Tile.o: Tile.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
Sprite.o: Sprite.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
Entity.o: Entity.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@
main.o: main.cpp
	$(CC) $^ $(SHARED_LIB_FLAGS) -c -o $@

unit_tests.c: MyTestSuite.h
	cxxtestgen --error-printer -o $@ $^
test: unit_tests.c $(OBJ_FILES)
	$(CC) $^ $(FLAGS) $(LIBS) -o $@

clean:
	rm -f *.o game test unit_tests.c
