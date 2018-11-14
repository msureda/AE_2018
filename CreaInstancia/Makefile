all: crea_instancia

clean:
	rm -f crea_instancia *.o *% *~

crea_instancia: crea_instancia.o
	g++ -g -o $@ crea_instancia.o

crea_instancia.o: crea_instancia.cpp crea_instancia.hpp
	g++ -g -c -o $@ crea_instancia.cpp
