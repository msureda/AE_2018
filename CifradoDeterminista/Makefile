all: cifrado_determinista

clean:
	rm -f cifrado_determinista *.o *% *~

cifrado_determinista: cifrado_determinista.cpp
	g++ -O3 -o $@ cifrado_determinista.cpp

run: cifrado_determinista
	./cifrado_determinista ../ProblemInstances/CIFRADO-instances/texto_cifrado01.txt \
	../ProblemInstances/CIFRADO-instances/frecuencias.txt
