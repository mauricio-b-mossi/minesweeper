run: link
	./sfml-app.exe

link: out/main.o out/AssetManager.o out/Game.o out/StateManager.o out/Welcome.o
	g++ out/main.o out/AssetManager.o out/Game.o out/StateManager.o out/Welcome.o -o sfml-app -LC:/SFMLmingw6.1.0/lib -lsfml-graphics -lsfml-window -lsfml-system

out/main.o: src/main.cpp
	g++ -c src/main.cpp -o$@ -IC:/SFMLmingw6.1.0/include

out/AssetManager.o: src/AssetManager.cpp src/AssetManager.hpp
	g++ -c src/AssetManager.cpp -o$@ -IC:/SFMLmingw6.1.0/include

out/Game.o: src/Game.cpp src/Game.hpp
	g++ -c src/Game.cpp -o$@ -IC:/SFMLmingw6.1.0/include

out/StateManager.o: src/StateManager.cpp src/StateManager.hpp
	g++ -c src/StateManager.cpp -o$@ -IC:/SFMLmingw6.1.0/include

out/Welcome.o: src/Welcome.cpp src/Welcome.hpp
	g++ -c src/Welcome.cpp -o$@ -IC:/SFMLmingw6.1.0/include
