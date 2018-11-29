//
//  cifrado_determinista.cpp
//
//   Created on: 28/11/2018
//       Author: marcelo
//

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

enum constantes
{
	CANTIDAD_SIMBOLOS			= 27,

	// Codificacion: ISO-8859-1
	ENIE						= 0xf1,			// ñ
};

const unsigned char alfabeto[CANTIDAD_SIMBOLOS] =
		{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
		  'j', 'k', 'l', 'm', 'n', ENIE, 'o', 'p', 'q',
		  'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

// Declara e inicializa matriz ocurrencias para simbolos
unsigned long long int ocurr_sim[CANTIDAD_SIMBOLOS] = { 0ull };

// Declara e inicializa matriz ocurrencias para digramas
unsigned long long int ocurr_di[CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS] = { 0ull };

// Declara e inicializa matriz ocurrencias para trigramas
unsigned long long int ocurr_tri[CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS] = { 0ull };

// Contadores de cantidad total de simbolos, digramas y trigramas
unsigned long long int cnt_sim = 0ull, cnt_di = 0ull, cnt_tri = 0ull;


// Declara e inicializa matriz frecuencias para simbolos
double frec_sim[CANTIDAD_SIMBOLOS] = { 0.0f };
double frec_sim_texto[CANTIDAD_SIMBOLOS] = { 0.0f };

// Declara e inicializa matriz frecuencias para digramas
double frec_di[CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS] = { 0.0f };
double frec_di_texto[CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS] = { 0.0f };

// Declara e inicializa matriz frecuencias para trigramas
double frec_tri[CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS] = { 0.0f };
double frec_tri_texto[CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS] = { 0.0f };

struct Frecuencia_T
{
	unsigned char	simbolo;
	long long int	frecuencia;
};

vector<Frecuencia_T> frecuencia_castellano(CANTIDAD_SIMBOLOS);

bool compara_frecuencia(Frecuencia_T a, Frecuencia_T b)
{
	return (a.frecuencia > b.frecuencia);
}

//
//  indice
//  Devuelve el indice del alfabeto a usar
//  en el array de simbolos.
//
inline int indice(unsigned char c)
{
	if (c >= 'a' && c <= 'n')
	{
		return (int)(c - 'a');
	}
	else if (c == ENIE)
	{
		return (int)('n' - 'a' + 1);
	}
	else if (c >= 'o' && c <= 'z')
	{
		return (int)(c - 'a' + 1);
	}
	else
	{
		return -1;
	}
}

//
//  intercambia_letra
//  Intercambia los elementos en las posiciones
//  indicados por los indices i y j.
//
inline void intercambia_letra(unsigned char *clave, int i, int j)
{
	unsigned char aux = clave[i];
	clave[i] = clave [j];
	clave[j] = aux;
}


//
// leer_texto
// Dado el nombre del archivo carga el text disponible en
// el string texto
//
string leer_texto(const char *archivo)
{
	// Abre Archivo
	ifstream iftexto(archivo, ifstream::in);
	if (iftexto.fail())
	{
		cerr << "Error: Fallo abriendo " << archivo << endl;
		exit(EXIT_FAILURE);
	}

	// Carga frecuencias de simbolos
	string texto, linea;
	while (getline(iftexto, linea))
	{
		texto += linea;
	}
	iftexto.close();
	return texto;
}

//
//  calcula_frecuencia
//  Cuenta la cantidad de ocurrencias de simbolos,
//  digramas y trigramas
//
void calcula_frecuencia(string texto)
{
	int ind_0 = -1, ind_1 = -1, ind_2 = -1;
	for (unsigned int i = 0; i < texto.length(); ++i)
	{
		ind_2 = i > 1 ? ind_1 : -1;
		ind_1 = i > 0 ? ind_0 : -1;
		ind_0 = indice(texto[i]);

		// Simbolos
		if (ind_0 != -1)
		{
			++(ocurr_sim[ind_0]);
			++cnt_sim;
		}

		// Digramas
		if (ind_0 != -1 && ind_1 != -1)
		{
			++ocurr_di[ind_1][ind_0];
			++cnt_di;
		}

		// Trigramas
		if (ind_0 != -1 && ind_1 != -1 && ind_2 != -1)
		{
			++ocurr_tri[ind_2][ind_1][ind_0];
			++cnt_tri;
		}
	}

	// Simbolos
	for(unsigned int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
	{
		frec_sim_texto[i] = (double)ocurr_sim[i] / (double)cnt_sim;
	}

	// Digramas
	for(unsigned int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
	{
		for(unsigned int j = 0; j < CANTIDAD_SIMBOLOS; ++j)
		{
			frec_di_texto[i][j] = (double)ocurr_di[i][j] / (double)cnt_di;
		}
	}

	// Trigramas
	for(unsigned int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
	{
		for(unsigned int j = 0; j < CANTIDAD_SIMBOLOS; ++j)
		{
			for(unsigned int k = 0; k < CANTIDAD_SIMBOLOS; ++k)
			{
				frec_tri_texto[i][j][k] = (double)ocurr_tri[i][j][k] / (double)cnt_tri;
			}
		}
	}
}


//
//  carga_frecuencias
//  Carga en memoria la lista de frecuencias del archivo
//  con el formato:
//  Simbolo<TAB>Frecuencia
//	Digrama<TAB>Frecuencia
//	Trigrama<TAB>Frecuencia
//
void carga_frecuencias(const char *archivo_frecuencias)
{
	// Abre Archivo
	ifstream ifrecuencias(archivo_frecuencias, ifstream::in);
	if (ifrecuencias.fail())
	{
		cerr << "Error: Fallo abriendo " << archivo_frecuencias << endl;
		exit(EXIT_FAILURE);
	}

	// Carga frecuencias de simbolos
	string ifrec_linea;
	while (getline(ifrecuencias, ifrec_linea))
	{
		istringstream ifrec_stream(ifrec_linea);
		string simbolo;
		long long int ocurrencias;
		double frecuencia;
		ifrec_stream >> simbolo >> ocurrencias >> frecuencia;
		switch (simbolo.length())
		{
			// Unigramas o simbolos
			case 1:
				frec_sim[indice(simbolo[0])] = frecuencia;
				frecuencia_castellano[indice(simbolo[0])].frecuencia = ocurrencias;
				frecuencia_castellano[indice(simbolo[0])].simbolo = simbolo[0];
				break;

			// Digramas
			case 2:
				frec_di[indice(simbolo[0])]
				       [indice(simbolo[1])] = frecuencia;
				break;

			// Trigramas
			case 3:
				frec_tri[indice(simbolo[0])]
				        [indice(simbolo[1])]
				        [indice(simbolo[2])] = frecuencia;
				break;

			// Titulos/otros...
			default:
				break;
		}
	}

	if (!ifrecuencias.eof())
	{
		cerr << "Error: Leyendo archivo " << archivo_frecuencias << endl;
		ifrecuencias.close();
		exit(EXIT_FAILURE);
	}
	ifrecuencias.close();
}

//
// imprime_clave
// Imprime clave a pantalla
//
void imprime_clave(const unsigned char *clave)
{
	cout << "Alfabeto: " << string((char *)alfabeto, CANTIDAD_SIMBOLOS) << "\n";
	cout << "Mapeo:    " << string((char *)clave, CANTIDAD_SIMBOLOS) << endl;
}

//
// encripta
// Dado un texto en claro y una clave devuelve el
// texto encriptado con la cifra de sustitucion.
//
string encripta(string texto_claro, const unsigned char *clave)
{
	string texto_encriptado(texto_claro.length(), ' ');

	for (unsigned int i = 0; i < texto_claro.length(); ++i)
	{
		int ind = indice((unsigned)texto_claro[i]);
		if (ind == -1)
		{
			texto_encriptado[i] = texto_claro[i];
		}
		else
		{
			texto_encriptado[i] = clave[ind];
		}
	}
	return texto_encriptado;
}

//
// fitness
// Calcula el fitness del texto segun la clave usada.
//
double fitness()
{
	double fitness = 0.0;
	for(int i=0; i<CANTIDAD_SIMBOLOS;i++)
	{
		fitness += fabs(frec_sim_texto[i] - frec_sim[i]);
	}

	for(int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
	{
		for(int j = 0; j < CANTIDAD_SIMBOLOS; ++j)
		{
			fitness += fabs(frec_di_texto[i][j] - frec_di[i][j]);
		}
	}

	for(int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
	{
		for(int j = 0; j < CANTIDAD_SIMBOLOS; ++j)
		{
			for(int k = 0; k < CANTIDAD_SIMBOLOS; ++k)
			{
				fitness += fabs(frec_tri_texto[i][j][k] - frec_tri[i][j][k]);
			}
		}
	}
	return fitness;
}


int main (int argc, char** argv)
{
	// Valida argumentos
	if (argc < 3)
	{
		cerr << "Error: Faltan parametros.\n\nUso:\n";
		cerr << "\t" << argv[0] << " <archivo_texto_cifrado> <archivo_frecuencias>\n" << endl;
		exit(EXIT_FAILURE);
	}

	// Lee texto
	string texto_cifrado = leer_texto(argv[1]);

	// Calcula frecuencia de texto cifrado
	calcula_frecuencia(texto_cifrado);

	// Carga frecuencias del castellano
	carga_frecuencias(argv[2]);

	// Vector con frecuencia de texto cifrado
	vector<Frecuencia_T> frecuencia_texto_cifrado(CANTIDAD_SIMBOLOS);
	for (unsigned i=0; i<CANTIDAD_SIMBOLOS; ++i)
	{
		frecuencia_texto_cifrado[i].simbolo = alfabeto[i];
		frecuencia_texto_cifrado[i].frecuencia = ocurr_sim[i];
	}

	// Ordena frecuencia de texto cifrado
	sort(frecuencia_texto_cifrado.begin(), frecuencia_texto_cifrado.end(), compara_frecuencia);

	// Ordena frecuencia de castellano
	sort(frecuencia_castellano.begin(), frecuencia_castellano.end(), compara_frecuencia);

	cout << "Frecuencia de texto cifrado\n";
	for (unsigned i=0; i<CANTIDAD_SIMBOLOS; ++i)
	{
		cout << frecuencia_texto_cifrado[i].simbolo << "\t" <<
				frecuencia_texto_cifrado[i].frecuencia << "\n";
	}
	cout << "Frecuencia castellano\n";
	for (unsigned i=0; i<CANTIDAD_SIMBOLOS; ++i)
	{
		cout << frecuencia_castellano[i].simbolo << "\t" <<
				frecuencia_castellano[i].frecuencia << "\n";
	}

	// Clave inicial
	// Mapea mas usadas del texto cifrado a mas usadas del castellano
	unsigned char clave[CANTIDAD_SIMBOLOS];
	for (unsigned i=0; i<CANTIDAD_SIMBOLOS; ++i)
	{
		clave[indice(frecuencia_texto_cifrado[i].simbolo)] = frecuencia_castellano[i].simbolo;
	}
	imprime_clave(clave);

	// Desencripta el texto con la clave
	string texto_claro = encripta(texto_cifrado, clave);
	cout << "Texto Claro:\n" << texto_claro << "\n";

	// Calcual frecuencia
	calcula_frecuencia(texto_claro);
	double mejor_fitness = fitness();
	cout << "Fitness de solucion inicial: " << mejor_fitness << "\n";
	cout << "-----------------\n";

	// Recorre las letras de la clave optimizando mediante
	// sucesivas busquedas locales
	for (unsigned i=0; i<CANTIDAD_SIMBOLOS; ++i)
	{
		unsigned char clave_nueva[CANTIDAD_SIMBOLOS];
		memcpy(clave_nueva, clave, CANTIDAD_SIMBOLOS);

		// Indice de letra pivot
		unsigned int ind_i =  indice(frecuencia_texto_cifrado[i].simbolo);
		cout << "Iteracion externa: " << i << "\tLetra Pivot: " <<
				frecuencia_texto_cifrado[i].simbolo <<
				"\tIndice Letra: " << ind_i << "\n";

		for (unsigned j=i+1; j<CANTIDAD_SIMBOLOS; ++j)
		{

			// Indice letra actual
			unsigned int ind_j =  indice(frecuencia_texto_cifrado[j].simbolo);
			cout << "Iteracion interna: " << j << "\tLetra Actual: " <<
					frecuencia_texto_cifrado[j].simbolo <<
					"\tIndice Letra: " << ind_j << "\n";

			// Copia la clave nueva
			unsigned char clave_actual[CANTIDAD_SIMBOLOS];
			memcpy(clave_actual, clave_nueva, CANTIDAD_SIMBOLOS);

			// Intercambia posicion de letra pivot con letra actual
			cout << "Clave antes intercambio\n";
			imprime_clave(clave_actual);

			intercambia_letra(clave_actual, ind_i, ind_j);

			cout << "Clave despues de intercambio\n";
			imprime_clave(clave_actual);

			// Calcular el fitness de cada intercambio
			texto_claro = encripta(texto_cifrado, clave_actual);
			calcula_frecuencia(texto_claro);
			double fitness_actual = fitness();

			// Elegir la mejor solucion
			if (fitness_actual < mejor_fitness)
			{
				mejor_fitness = fitness_actual;
				memcpy(clave, clave_actual, CANTIDAD_SIMBOLOS);

				// Clave optimizada
				cout << "-----------------\n";
				cout << "Mejora Encontrada\n";
				imprime_clave(clave);
				cout << "Texto Claro:\n" << texto_claro << "\n";
				cout << "Fitness: " << mejor_fitness << "\n";
				cout << "-----------------\n";

			}
		}
	}

	// Clave optimizada
	imprime_clave(clave);

	// Desencripta el texto con la clave
	texto_claro = encripta(texto_cifrado, clave);
	cout << "Texto Claro:\n" << texto_claro << "\n";

	// Calcula frecuencia
	cout << "Fitness de solucion: " << mejor_fitness << "\n";
}
