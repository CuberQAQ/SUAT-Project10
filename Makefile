# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g
LDFLAGS = -lncurses

# Targets
all: project10

project10: main.o Game.o Gui.o GameObject.o Obstacle.o Weapon.o Item.o Ship.o BattleShip.o EnemyShip.o GunBoat.o Destroyer.o Cruiser.o Bomber.o
	$(CXX) $(CXXFLAGS) -o main main.o Game.o Gui.o GameObject.o Obstacle.o Weapon.o Item.o Ship.o BattleShip.o EnemyShip.o GunBoat.o Destroyer.o Cruiser.o Bomber.o $(LDFLAGS)

main.o: main.cpp Game.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Game.o: Game.cpp Game.h
	$(CXX) $(CXXFLAGS) -c Game.cpp

Gui.o: Gui.C Gui.h
	$(CXX) $(CXXFLAGS) -c Gui.C

GameObject.o: GameObject.cpp GameObject.h Gui.h Types.h
	$(CXX) $(CXXFLAGS) -c GameObject.cpp

Obstacle.o: Obstacle.cpp Obstacle.h GameObject.h
	$(CXX) $(CXXFLAGS) -c Obstacle.cpp

Weapon.o: Weapon.cpp Weapon.h GameObject.h
	$(CXX) $(CXXFLAGS) -c Weapon.cpp

Item.o: Item.cpp Item.h GameObject.h
	$(CXX) $(CXXFLAGS) -c Item.cpp

Ship.o: Ship.cpp Ship.h GameObject.h
	$(CXX) $(CXXFLAGS) -c Ship.cpp

BattleShip.o: BattleShip.cpp BattleShip.h Ship.h Obstacle.h Weapon.h
	$(CXX) $(CXXFLAGS) -c BattleShip.cpp

EnemyShip.o: EnemyShip.cpp EnemyShip.h Ship.h Obstacle.h Weapon.h BattleShip.h
	$(CXX) $(CXXFLAGS) -c EnemyShip.cpp

GunBoat.o: GunBoat.cpp GunBoat.h EnemyShip.h
	$(CXX) $(CXXFLAGS) -c GunBoat.cpp

Destroyer.o: Destroyer.cpp Destroyer.h EnemyShip.h
	$(CXX) $(CXXFLAGS) -c Destroyer.cpp

Cruiser.o: Cruiser.cpp Cruiser.h EnemyShip.h
	$(CXX) $(CXXFLAGS) -c Cruiser.cpp

Bomber.o: Bomber.cpp Bomber.h GameObject.h Weapon.h
	$(CXX) $(CXXFLAGS) -c Bomber.cpp

clean:
	rm -f project10 *.o
