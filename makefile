# This target will compiles everything, as the same command I use from linux terminal.
#all:
#	g++ -std=c++11 -Wall -Werror -pthread -o AnagramsMenu AnagramsMenu.cpp

all: AnagramsMenu

AnagramsMenu: AnagramsMenu.o
	g++ -std=c++11 -Wall -Werror -pthread AnagramsMenu.o -o AnagramsMenu

AnagramsMenu.o: AnagramsMenu.cpp
	g++ -std=c++11 -Wall -Werror -pthread -c AnagramsMenu.cpp

clean:
	rm -rf *o AnagramsMenu
