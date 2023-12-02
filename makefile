run: link
	./sfml-app.exe

link: main.o 
	g++ main.o AssetManager.o Game.o StateManager.o -o sfml-app -LC:/SFMLmingw6.1.0/lib -lsfml-graphics -lsfml-window -lsfml-system

main.o: src/main.cpp AssetManager.o Game.o StateManager.o
	g++ -c src/main.cpp -IC:/SFMLmingw6.1.0/include

AssetManager.o: src/AssetManager.cpp src/AssetManager.hpp
	g++ -c src/AssetManager.cpp -IC:/SFMLmingw6.1.0/include

Game.o: src/Game.cpp src/Game.hpp
	g++ -c src/Game.cpp -IC:/SFMLmingw6.1.0/include

StateManager.o: src/StateManager.cpp src/StateManager.hpp
	g++ -c src/StateManager.cpp -IC:/SFMLmingw6.1.0/include
