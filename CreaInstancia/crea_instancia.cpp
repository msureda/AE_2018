//
// crea_instancia.cpp
//
//  Created on: 13/11/2018
//      Author: marcelo
//
#include "crea_instancia.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

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
// imprime_clave
// Imprime clave a pantalla
//
inline void imprime_clave(const char *clave)
{
	cout << "Alfabeto: " << string((char *)alfabeto, CANTIDAD_SIMBOLOS) << "\n";
	cout << "Mapeo:    " << string((char *)clave, CANTIDAD_SIMBOLOS) << endl;
}

//
// invierte_clave
// Dada una clave devuelve la clave_inversa
//
inline void invierte_clave(const char *clave, char *clave_inversa)
{
	for (unsigned int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
	{
		clave_inversa[indice(clave[i])] = alfabeto[i];
	}
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
// genera_clave
// Genera una clave aleatoria
//
void genera_clave(char *clave)
{
	srand(time(NULL));
	bool usadas[CANTIDAD_SIMBOLOS] = {false};
	for(unsigned int i=0; i<CANTIDAD_SIMBOLOS; ++i)
	{
		int mapeo = rand() % CANTIDAD_SIMBOLOS;
		while (usadas[mapeo])
		{
			mapeo = (mapeo + 1) % CANTIDAD_SIMBOLOS;
		}
		clave[i] = alfabeto[mapeo];
		usadas[mapeo] = true;
	}
}

//
// preprocesa_texto
// Sustituye mayusculas y tildes
//
void preprocesa_texto(string &texto)
{
	for (string::iterator it=texto.begin(); it!=texto.end(); ++it)
	{
		switch ((unsigned int)*it)
		{
			case A_MINUSCULA_ACENTO_AGUDO:
			case A_MINUSCULA_ACENTO_GRAVE:
			case A_MAYUSCULA_ACENTO_AGUDO:
			case A_MAYUSCULA_ACENTO_GRAVE:
				*it = 'a';
				break;

			case E_MINUSCULA_ACENTO_AGUDO:
			case E_MINUSCULA_ACENTO_GRAVE:
			case E_MAYUSCULA_ACENTO_AGUDO:
			case E_MAYUSCULA_ACENTO_GRAVE:
				*it = 'e';
				break;

			case I_MINUSCULA_ACENTO_AGUDO:
			case I_MINUSCULA_ACENTO_GRAVE:
			case I_MAYUSCULA_ACENTO_AGUDO:
			case I_MAYUSCULA_ACENTO_GRAVE:
				*it = 'i';
				break;

			case O_MINUSCULA_ACENTO_AGUDO:
			case O_MINUSCULA_ACENTO_GRAVE:
			case O_MAYUSCULA_ACENTO_AGUDO:
			case O_MAYUSCULA_ACENTO_GRAVE:
				*it = 'o';
				break;

			case U_MINUSCULA_ACENTO_AGUDO:
			case U_MINUSCULA_ACENTO_GRAVE:
			case U_MAYUSCULA_ACENTO_AGUDO:
			case U_MAYUSCULA_ACENTO_GRAVE:
				*it = 'u';
				break;

			case ENIE_MAYUSCULA:
				*it = ENIE;
				break;

			default:
				*it = tolower(*it);
				break;
		}
	}
}

//
// encripta
// Dado un texto en claro y una clave devuelve el
// texto encriptado con la cifra de sustitucion.
//
string encripta(string texto_claro, const char *clave)
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
// desencripta
// Dado un texto en claro y una clave devuelve el
// texto desencriptado con el cifrado de sustitucion.
//
string desencripta(string texto_claro, const char *clave)
{
	string texto_encriptado;
	char clave_inversa[CANTIDAD_SIMBOLOS];
	invierte_clave(clave, clave_inversa);
	return encripta(texto_claro, clave_inversa);
}


int main(int argc, char *argv[])
{
	// Valida argumentos
	if (argc < 3)
	{
		cerr << "Error: Faltan parametros.\n\nUso:\n";
		cerr << "\t" << argv[0] << " <archivo_texto> <salida_encriptada>\n" << endl;
		exit(EXIT_FAILURE);
	}

	// Lee texto
	string texto_claro = leer_texto(argv[1]);
	//cout << "Texto Claro:\n" << texto_claro << "\n";

	// Genera clave de encripcion
	char clave[CANTIDAD_SIMBOLOS];
	genera_clave(clave);
	cout << "Clave de Cifrado\n";
	imprime_clave(clave);

	// Pre-procesa el texto: elimina ñ, tildes y mayusculas
	preprocesa_texto(texto_claro);
	//cout << "Texto Preprocesado:\n" << texto_claro << "\n";

	// Encripta el texto
	string texto_cifrado = encripta(texto_claro, clave);
	ofstream of_cifrado(argv[2]);
	if (of_cifrado.fail())
	{
		cerr << "Error: Fallo abriendo " << argv[2] << endl;
		exit(EXIT_FAILURE);
	}
	of_cifrado << "Texto Cifrado:\n" << texto_cifrado << "\n";

	// Valida que la funcion inversa funciona
	string texto_descifrado = desencripta(texto_cifrado, clave);
	if (texto_descifrado.compare(texto_claro))
	{
		cout << "Error en Descifrado:\n" << texto_descifrado << "\n";
		exit(EXIT_FAILURE);
	}

	// Obtiene clave de desencripcion
	char clave_inversa[CANTIDAD_SIMBOLOS];
	invierte_clave(clave, clave_inversa);
	cout << "Clave de Descifrado\n";
	imprime_clave(clave_inversa);

	return 0;
}
