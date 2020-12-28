all: main

main: main.o
	gcc -o main main.o graphisme.o operation.o -Wall -std=c99  `sdl2-config --cflags --libs` -lm -lSDL2_mixer

main.o: main.c graphisme.o operation.o
	gcc -c main.c -o main.o -Wall -std=c99 `sdl2-config --cflags --libs` -lm -lSDL2_mixer

graphisme.o: graphisme.h graphisme.c operation.o
	gcc -c graphisme.c -o graphisme.o -Wall -std=c99 `sdl2-config --cflags --libs` -lm -lSDL2_mixer

operation.o: type_cst.h operation.h operation.c
	gcc -c operation.c -o operation.o -Wall -std=c99 `sdl2-config --cflags --libs` -lm -lSDL2_mixer

clean:
	rm -rf main.o
