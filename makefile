run: exec
	./sfml-app.exe

exec: build
	g++ out/*.o -o sfml-app -LC:/SFMLmingw6.1.0/lib -lsfml-graphics -lsfml-window -lsfml-system

build:
	g++ -c src/*.cpp -IC:/SFMLmingw6.1.0/include


link: out/main.o out/AssetManager.o out/Game.o out/StateManager.o out/WelcomeScreen.o out/GameScreen.o out/Board.o out/Cell.o
	g++ out/main.o out/AssetManager.o out/Game.o out/StateManager.o out/WelcomeScreen.o out/GameScreen.o out/Board.o out/Cell.o -o sfml-app -LC:/SFMLmingw6.1.0/lib -lsfml-graphics -lsfml-window -lsfml-system

out/main.o: src/main.cpp
	g++ -c src/main.cpp -o$@ -IC:/SFMLmingw6.1.0/include

out/AssetManager.o: src/AssetManager.cpp src/AssetManager.hpp
	g++ -c src/AssetManager.cpp -o$@ -IC:/SFMLmingw6.1.0/include

out/Game.o: src/Game.cpp src/Game.hpp
	g++ -c src/Game.cpp -o$@ -IC:/SFMLmingw6.1.0/include

out/StateManager.o: src/StateManager.cpp src/StateManager.hpp src/State.hpp
	g++ -c src/StateManager.cpp -o$@ -IC:/SFMLmingw6.1.0/include

out/WelcomeScreen.o: src/WelcomeScreen.cpp src/WelcomeScreen.hpp src/State.hpp src/StateManager.hpp
	g++ -c src/WelcomeScreen.cpp -o$@ -IC:/SFMLmingw6.1.0/include

out/GameScreen.o: src/GameScreen.cpp src/GameScreen.hpp src/State.hpp src/StateManager.hpp src/Board.cpp
	g++ -c src/GameScreen.cpp -o$@ -IC:/SFMLmingw6.1.0/include

out/Board.o: src/Board.cpp src/Board.hpp src/Cell.cpp src/Cell.hpp
	g++ -c src/Board.cpp -o$@ -IC:/SFMLmingw6.1.0/include

out/Cell.o: src/Cell.cpp src/Cell.hpp
	g++ -c src/Cell.cpp -o$@ -IC:/SFMLmingw6.1.0/include
