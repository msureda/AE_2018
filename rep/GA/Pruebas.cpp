//============================================================================
// Name        : Pruebas.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <cstdio>
#include <cstdlib>
//#include <clocale>
//#include <cwchar>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

enum constantes
{
	CANTIDAD_SIMBOLOS			= 27,

	// Constantes para identificar diferentes caracteres
	// Codificacion: ISO-8859-1
	A_MINUSCULA_ACENTO_AGUDO	= 0xffffffe1,	// á
	A_MINUSCULA_ACENTO_GRAVE	= 0xffffffe0,	// à
	A_MAYUSCULA_ACENTO_AGUDO	= 0xffffffc1,	// Á
	A_MAYUSCULA_ACENTO_GRAVE	= 0xffffffc0,	// À

	E_MINUSCULA_ACENTO_AGUDO	= 0xffffffe9,	// é
	E_MINUSCULA_ACENTO_GRAVE	= 0xffffffe8,	// è
	E_MAYUSCULA_ACENTO_AGUDO	= 0xffffffc9,	// É
	E_MAYUSCULA_ACENTO_GRAVE	= 0xffffffc8,	// È

	I_MINUSCULA_ACENTO_AGUDO	= 0xffffffed,	// í
	I_MINUSCULA_ACENTO_GRAVE	= 0xffffffec,	// ì
	I_MAYUSCULA_ACENTO_AGUDO	= 0xffffffcd,	// Í
	I_MAYUSCULA_ACENTO_GRAVE	= 0xffffffcc,	// Ì

	O_MINUSCULA_ACENTO_AGUDO	= 0xfffffff3,	// ó
	O_MINUSCULA_ACENTO_GRAVE	= 0xfffffff2,	// ò
	O_MAYUSCULA_ACENTO_AGUDO	= 0xffffffd3,	// Ó
	O_MAYUSCULA_ACENTO_GRAVE	= 0xffffffd2,	// Ò

	U_MINUSCULA_ACENTO_AGUDO	= 0xfffffffa,	// ú
	U_MINUSCULA_ACENTO_GRAVE	= 0xfffffff9,	// ù
	U_MAYUSCULA_ACENTO_AGUDO	= 0xffffffda,	// Ú
	U_MAYUSCULA_ACENTO_GRAVE	= 0xffffffd9,	// Ù

	ENIE_MINUSCULA				= 0xfffffff1,	// ñ
	ENIE						= 0xf1,			// ñ
	ENIE_MAYUSCULA				= 0xffffffd1	// Ñ
};

const unsigned char alfabeto[] =
		{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
		  'j', 'k', 'l', 'm', 'n', ENIE, 'o', 'p', 'q',
		  'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

// Declara e inicializa matriz para simbolos
double frec_sim[CANTIDAD_SIMBOLOS] = { 0.0f };

// Declara e inicializa matriz para digramas
double frec_di[CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS] = { 0.0f };

// Declara e inicializa matriz para trigramas
double frec_tri[CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS] = { 0.0f };

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
void imprime_clave(const char *clave)
{
	cout << "Alfabeto: " << string((char *)alfabeto, CANTIDAD_SIMBOLOS) << "\n";
	cout << "Mapeo:    ";
	for(unsigned int i=0; i<CANTIDAD_SIMBOLOS; ++i)
	{
		cout << clave[i];
	}
	cout << endl;
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
		string simbolo, ocurrencias;
		double frecuencia;
		ifrec_stream >> simbolo >> ocurrencias >> frecuencia;
		switch (simbolo.length())
		{
			// Unigramas o simbolos
			case 1:
				frec_sim[indice(simbolo[0])] = frecuencia;
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
// Dado un texto lo convierte a minusculas
// sin tildes
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


int main()
{

	// Abre Archivo
	carga_frecuencias("/home/marcelo/workspaceAE/Pruebas/Debug/frecuencias.txt");

	// Unigramas
	cout << "Simbolo\tFrecuencia\n";
	for(unsigned int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
	{
		cout << alfabeto[i] << "\t" <<
					frec_sim[i] << "\n";
	}

	// Digramas
	cout << "Digrama\tFrecuencia\n";
	for(unsigned int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
	{
		for(unsigned int j = 0; j < CANTIDAD_SIMBOLOS; ++j)
		{
			cout << alfabeto[i] << alfabeto[j] << "\t"
				<< frec_di[i][j] << "\n";
		}
	}

	// Trigramas
	cout << "Trigrama\tFrecuencia\n";
	for(unsigned int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
	{
		for(unsigned int j = 0; j < CANTIDAD_SIMBOLOS; ++j)
		{
			for(unsigned int k = 0; k < CANTIDAD_SIMBOLOS; ++k)
			{
				cout << alfabeto[i] << alfabeto[j] << alfabeto[k] << "\t"
					<< frec_tri[i][j][k] << "\n";
			}
		}
	}

	string texto_claro = leer_texto("/home/marcelo/Documentos/FIng/Algoritmos Evolutivos/Obligatorios/Obligatorio 2/Instancias/100 años de soledad.txt");
	cout << "Texto Claro:\n" << texto_claro << "\n";

	char clave[CANTIDAD_SIMBOLOS];
	genera_clave(clave);
	imprime_clave(clave);

	preprocesa_texto(texto_claro);
	cout << "Texto Preprocesado:\n" << texto_claro << "\n";

	string texto_cifrado = encripta(texto_claro, clave);
	cout << "Texto Cifrado:\n" << texto_cifrado << "\n";

	string texto_descifrado = desencripta(texto_cifrado, clave);
	cout << "Texto Descifrado:\n" << texto_descifrado << "\n";

	return 0;
}


// int aux()
// {
//	setlocale(LC_ALL, "es_UY:es");
//	printf("ó á");
//
//	wstring str;
//	str = L"aáàÁÀeéèÉÈiíìÍÌoóòÓÒuúùÚÙñÑ";
//	for (int i=0; str[i] != 0; i++)
//	{
//		//printf("%c = %u | %x\n", str[i], str[i], str[i]);
//		wcout << str[i] << " | " <<
//				std::dec << (unsigned int)str[i] << "|" <<
//				std::hex << (int)str[i] << endl;
//	}
//	PRUEBA
//	iftexto.sync_with_stdio(false);
//	iftexto.imbue(locale("es_UY.UTF-8"));
//	ios_base::sync_with_stdio(false);
//	wcout.imbue(locale("es_UY.UTF-8"));
//	wcout << L"áéíóú";
//
//	for (auto const&t : wstring (L"áéíóú"))
//	{
//		wcout << t;
//	}
	// Bad Idea
//	for (int i = 0; i < 15; i++)
//	{
//	  std::cout << i << std::endl;
//	}
	// Good Idea
//	for (int i = 0; i < 15; ++i)
//	{
//	  std::cout << i << std::endl;
//	}
// }
