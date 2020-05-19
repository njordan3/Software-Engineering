CFLAGS = -I ./include
##LIB    = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lX11 -lGLU -lGL -lm #-lXrandr
GFLAGS = marbienJ.cpp nickolasL.cpp brandonH.cpp nicholasJo.cpp adamO.cpp

all: rainforest

rainforest: rainforest.cpp log.cpp $(GFLAGS)
	g++ $(CFLAGS) $(GFLAGS) rainforest.cpp  log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o rainforest

clean:
	rm -f rainforest
	rm -f *.o

