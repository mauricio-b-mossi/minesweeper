run: link
	./sfml-app.exe

link: out/main.o out/AssetManager.o out/Game.o out/StateManager.o out/WelcomeScreen.o out/GameScreen.o
	g++ out/main.o out/AssetManager.o out/Game.o out/StateManager.o out/WelcomeScreen.o out/GameScreen.o -o sfml-app -LC:/SFMLmingw6.1.0/lib -lsfml-graphics -lsfml-window -lsfml-system

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

out/GameScreen.o: src/GameScreen.cpp src/GameScreen.hpp src/State.hpp src/StateManager.hpp
	g++ -c src/GameScreen.cpp -o$@ -IC:/SFMLmingw6.1.0/include
