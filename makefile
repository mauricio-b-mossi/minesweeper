run: link build
	./sfml-app.exe

link: build
	g++ main.o -o sfml-app -LC:/SFMLmingw6.1.0/lib -lsfml-graphics -lsfml-window -lsfml-system

build:
	g++ -c main.cpp -IC:/SFMLmingw6.1.0/include
