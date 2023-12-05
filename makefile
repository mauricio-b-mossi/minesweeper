run: exec
	./sfml-app.exe

runCustom: exec
	./sfml-app.exe -c

runDebug: exec
	./sfml-app.exe -d

runCustomDebug: exec
	./sfml-app.exe -cd

exec: build
	g++ *.o -o sfml-app -LC:/SFMLmingw6.1.0/lib -lsfml-graphics -lsfml-window -lsfml-system

build:
	g++ -c src/*.cpp -IC:/SFMLmingw6.1.0/include

clean:
	del *.o
