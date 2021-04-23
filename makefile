test: SFMLCamera.o test.o
	g++ -g -Wall test.o SFMLCamera.o -o test -L /home/lucas/Headers/SFML-2.5.1/lib/ -lsfml-graphics -lsfml-window -lsfml-system -Wl,-rpath=/home/lucas/Headers/SFML-2.5.1/lib/

test.o: test.cpp
	g++ -g -Wall test.cpp -c -I /home/lucas/Headers/SFML-2.5.1/include/

SFMLCamera.o: SFMLCamera.cpp SFMLCamera.hpp
	g++ -g -Wall SFMLCamera.cpp -c -I /home/lucas/Headers/SFML-2.5.1/include/