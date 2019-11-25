CPP = g++
OPTS = -lm -lglut -lGL -lGLU -I"assets/soundEngine" -L"/usr/lib"  libIrrKlang.so -pthread && ./korno

all:
	$(CPP) base.cpp -o korno $(OPTS)

clean:
	rm example
