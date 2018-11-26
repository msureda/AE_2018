#ifndef INC_REQ_newGA
#define INC_REQ_newGA
#include "newGA.hh"
#include <math.h>
#include <sstream>

skeleton newGA
{

	// Problem ---------------------------------------------------------------

	Problem::Problem ():_dimension(CANTIDAD_SIMBOLOS),
						pos_primera_frec(0),
						pos_segunda_frec(0),
						pos_tercera_frec(0),
						pos_cuarta_frec(0)
	{
		inicializa_frecuencias(frec_sim_castellano, frec_di_castellano, frec_tri_castellano);
	}

	ostream& operator<< (ostream& os, const Problem& pbm)
	{
		os << endl << endl << "Number of Variables " << pbm._dimension
		   << endl;
		return os;
	}

	istream& operator>> (istream& is, Problem& pbm)
	{
		char buffer[MAX_BUFFER];

		// Leo nombre de archivo de frecuencias del castellano
		is.getline(buffer,MAX_BUFFER,'\n');
		pbm.carga_frecuencias(buffer);

		// Leo nombre de archivo de texto cifrado
		is.getline(buffer,MAX_BUFFER,'\n');
		pbm._texto_cifrado = pbm.leer_texto(buffer);


		// Declara e inicializa matriz ocurrencias para simbolos
		unsigned long long int ocurr_sim[CANTIDAD_SIMBOLOS] = { 0ull };
		for (unsigned int i = 0; i < pbm._texto_cifrado.length(); ++i)
		{
			int ind = indice(pbm._texto_cifrado[i]);
			if (ind != -1)
			{
				++(ocurr_sim[ind]);
			}
		}

		//	4 LETRAS MAS FRECUENTES Y POSICIONES
		//	Basado en los 'nombres' definidos de las funciones en 'calculo_frecuencia'
		//	Ordeno (de mayor a menor) y tomo los 4 primeros.
		int frecuencias_ordenadas[CANTIDAD_SIMBOLOS]; // ocurr_sim , array con frencuencias de los simbolos
		for (int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
		{
			frecuencias_ordenadas[i] = ocurr_sim[i];
		}

		for (int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
			for (int j = i + 1; j < CANTIDAD_SIMBOLOS; ++j)
            {
                if (frecuencias_ordenadas[i] < frecuencias_ordenadas[j])
                {
                    int aux =  frecuencias_ordenadas[i];
                    frecuencias_ordenadas[i] = frecuencias_ordenadas[j];
                    frecuencias_ordenadas[j] = aux;
                }

            }
        for (int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
        {
        	if(ocurr_sim[i] == frecuencias_ordenadas[0]) // simbolo con mayor frecuencia en texto cifrado
        	{
        		pbm.pos_primera_frec = i;
        	}
        	else if(ocurr_sim[i] == frecuencias_ordenadas[1]) // simbolo con 2da mayor frecuencia en texto cifrado
        	{
        		pbm.pos_segunda_frec = i;
        	}
        	else if(ocurr_sim[i] == frecuencias_ordenadas[2]) // simbolo con 3era mayor frecuencia en texto cifrado
        	{
        		pbm.pos_tercera_frec = i;
        	}
        	else if(ocurr_sim[i] == frecuencias_ordenadas[3]) // simbolo con 4ta mayor frecuencia en texto cifrado
        	{
        		pbm.pos_cuarta_frec = i;
        	}
        }

		return is;
	}

	bool Problem::operator== (const Problem& pbm) const
	{
		if (_dimension!=pbm.dimension()) return false;
		return true;
	}

	bool Problem::operator!= (const Problem& pbm) const
	{
		return !(*this == pbm);
	}

	Direction Problem::direction() const
	{
		//return maximize;
		return minimize;
	}

	int Problem::dimension() const
	{
		return _dimension;
	}

	int Problem::get_pos_frecuencia_primera() const
	{
		return pos_primera_frec;
	}
	int Problem::get_pos_frecuencia_segunda() const
	{
		return pos_segunda_frec;
	}
	int Problem::get_pos_frecuencia_tercera() const
	{
		return pos_tercera_frec;
	}
	int Problem::get_pos_frecuencia_cuarta() const
	{
		return pos_cuarta_frec;
	}

	string Problem::get_texto_cifrado() const
	{
		return _texto_cifrado;
	}

	double Problem::get_frec_sim_castellano(int i) const
	{
		return frec_sim_castellano[i];
	}

	double Problem::get_frec_di_castellano(int i, int j) const
	{
		return frec_di_castellano[i][j];
	}

	double Problem::get_frec_tri_castellano(int i, int j, int k) const
	{
		return frec_tri_castellano[i][j][k];
	}

	//
	// encripta
	// Dado un texto en claro y una clave devuelve el
	// texto encriptado con la cifra de sustitucion.
	//
	string Problem::desencripta(const string texto_claro, const unsigned char *clave) const
	{
		string texto(texto_claro.length(), ' ');

		for (unsigned int i = 0; i < texto_claro.length(); ++i)
		{
			int ind = indice((unsigned)texto_claro[i]);
			if (ind == -1)
			{
				texto[i] = texto_claro[i];
			}
			else
			{
				texto[i] = clave[ind];
			}
		}
		return texto;
	}


	//
	//  carga_frecuencias
	//  Carga en memoria la lista de frecuencias del archivo
	//  con el formato:
	//  Simbolo<TAB>Frecuencia
	//	Digrama<TAB>Frecuencia
	//	Trigrama<TAB>Frecuencia
	//
	void Problem::carga_frecuencias(const char *archivo_frecuencias)
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
					frec_sim_castellano[indice(simbolo[0])] = frecuencia;
					break;

				// Digramas
				case 2:
					frec_di_castellano[indice(simbolo[0])]
					                   [indice(simbolo[1])] = frecuencia;
					break;

				// Trigramas
				case 3:
					frec_tri_castellano[indice(simbolo[0])]
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
	string Problem::leer_texto(const char *archivo)
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
	//  digramas y trigramas y calcula la frecuencia
	//
	Frecuencias_Texto_T Problem::calcula_frecuencia(string texto) const
	{
		Frecuencias_Texto_T frecuencias_texto;

		// Declara e inicializa matriz ocurrencias para simbolos
		unsigned long long int ocurr_sim[CANTIDAD_SIMBOLOS] = { 0ull };

		// Declara e inicializa matriz ocurrencias para digramas
		unsigned long long int ocurr_di[CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS] = { 0ull };

		// Declara e inicializa matriz ocurrencias para trigramas
		unsigned long long int ocurr_tri[CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS][CANTIDAD_SIMBOLOS] = { 0ull };

		// Contadores de cantidad total de simbolos, digramas y trigramas
		unsigned long long int cnt_sim = 0ull, cnt_di = 0ull, cnt_tri = 0ull;

		int ind_0 = -1, ind_1 = -1, ind_2 = -1;
		//for (unsigned int i = 0; i < texto.length(); ++i)
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

		for(unsigned int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
		{
			frecuencias_texto.frec_sim[i] = (double)ocurr_sim[i] / (double)cnt_sim;
		}

		// Digramas
		for(unsigned int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
		{
			for(unsigned int j = 0; j < CANTIDAD_SIMBOLOS; ++j)
			{
				frecuencias_texto.frec_di[i][j] = (double)ocurr_di[i][j] / (double)cnt_di;
			}
		}

		// Trigramas
		for(unsigned int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
		{
			for(unsigned int j = 0; j < CANTIDAD_SIMBOLOS; ++j)
			{
				for(unsigned int k = 0; k < CANTIDAD_SIMBOLOS; ++k)
				{
					frecuencias_texto.frec_tri[i][j][k] = (double)ocurr_tri[i][j][k] / (double)cnt_tri;
				}
			}
		}
		return frecuencias_texto;
	}


	Problem::~Problem()
	{
	}


	// Solution --------------------------------------------------------------

	Solution::Solution (const Problem& pbm):_pbm(pbm)
	{}

	unsigned int Solution::_grupos_frec = 0;

	const Problem& Solution::pbm() const
	{
		return _pbm;
	}

	Solution::Solution(const Solution& sol):_pbm(sol.pbm())
	{
		*this=sol;
	}

	istream& operator>> (istream& is, Solution& sol)
	{
		for (int i=0;i<sol.pbm().dimension();++i)
			is >> sol._var[i];
		return is;
	}

	ostream& operator<< (ostream& os, const Solution& sol)
	{
		for (int i=0;i<sol.pbm().dimension();i++)
			os << sol._var[i];
		return os;
	}

	NetStream& operator << (NetStream& ns, const Solution& sol)
	{
		for (int i=0;i<CANTIDAD_SIMBOLOS; ++i)
			ns << sol._var[i];
		return ns;
	}

	NetStream& operator >> (NetStream& ns, Solution& sol)
	{
		for (int i=0;i<CANTIDAD_SIMBOLOS; ++i)
			ns >> sol._var[i];
		return ns;
	}

 	Solution& Solution::operator= (const Solution &sol)
	{
		for(int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
			_var[i] = sol._var[i];
		return *this;
	}

	bool Solution::operator== (const Solution& sol) const
	{
		if (sol.pbm() != _pbm) return false;
		for(int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
			if(_var[i] != sol._var[i]) return false;
		return true;
	}

	bool Solution::operator!= (const Solution& sol) const
	{
		return !(*this == sol);
	}

	void Solution::initialize()
	{
		char _permutacion[CANTIDAD_SIMBOLOS];
		bool letras_usadas[CANTIDAD_SIMBOLOS] = {false}; // letras usadas.

		// Posición (en base 27) de las letras más frecuentes que aparecen en el texto
		int primera_letra_frecuente = _pbm.get_pos_frecuencia_primera();
		int segunda_letra_frecuente = _pbm.get_pos_frecuencia_segunda();
		int tercera_letra_frecuente = _pbm.get_pos_frecuencia_tercera();
		int cuarta_letra_frecuente =  _pbm.get_pos_frecuencia_cuarta();

		int letra_e, letra_a, letra_o, letra_s;

		if(_grupos_frec < 20) // 1er grupo: E A O S
		{
			for (int i = 0; i < _pbm.dimension(); i++) // 27
			{
				// Generamos la permutación, asignando un valor aleatorio a cada celda.
				int letra_aleatoria = rand_int(0,CANTIDAD_SIMBOLOS - 1);
				while(letras_usadas[letra_aleatoria])
				{
					letra_aleatoria = (letra_aleatoria + 1) % CANTIDAD_SIMBOLOS;
				}
				_permutacion[i] = alfabeto[letra_aleatoria];
				letras_usadas[letra_aleatoria] = 1;

				/*
				Permutacion por SWAP
				int index1 = rand_int(0,27);
				int index2 = rand_int(0,27);
				int aux = _permutacion[index1];
				_permutacion[index1] = _permutacion[index2];
				_permutacion[index2] = aux;
				*/

				// Guardo indices para luego, acomodar la permutacion
				switch(_permutacion[i])
				// a0 b1 c2 d3 e4 f5 g6 h7 i8 j9 k10 l11 m12 n13 ñ14 o15 p16 q17 r18 s19 t20 u21 v22 w23 x24 y25 z26
				{
					case 'e': // E
						letra_e = i;
						break;
					case 'a': // A
						letra_a = i;
						break;
					case 'o': // O
						letra_o = i;
						break;
					case 's': // S
						letra_s = i;
						break;
					default:
						break;
				};
			} /* END FOR */

			for(int i = 0; i < _pbm.dimension(); i++)
			{
				_var[i] = _permutacion[i];
			}
			// EJ: si la a mapea a la z, en la pos de a va un 26
			for (int i = 0; i < _pbm.dimension(); i++) // ACOMODO LETRAS haciendo swap
			{
				if(_var[i] != 'e' && i == primera_letra_frecuente)
				//en _var[i], debería ir la letra más frecuente (la letra "e")
				{
					char aux_letra = _var[letra_e];
					_var[letra_e] = _var[i];
					_var[i] = aux_letra;
				}
				else if(_var[i] != 'a' && i == segunda_letra_frecuente)
				//en _var[i], debería ir la segunda letra más frecuente (la letra "a")
				{
					char aux_letra = _var[letra_a];
					_var[letra_a] = _var[i];
					_var[i] = aux_letra;
				}
				else if(_var[i] != 'o' && i == tercera_letra_frecuente)
				//en _var[i], debería ir la tercera letra más frecuente (la letra "o")
				{
					char aux_letra = _var[letra_o];
					_var[letra_o] = _var[i];
					_var[i] = aux_letra;
				}
				else if(_var[i] != 's' && i == cuarta_letra_frecuente)
				//en _var[i], debería ir la cuarta letra más frecuente (la letra "s")
				{
					char aux_letra = _var[letra_s];
					_var[letra_s] = _var[i];
					_var[i] = aux_letra;
				}
			}
		} /* END IF GRUPO 1 */

		else if(_grupos_frec >= 20 && _grupos_frec < 40) // A E O S
		{
			for (int i = 0; i < _pbm.dimension(); i++) // 27
			{
				int letra_aleatoria = rand_int(0,CANTIDAD_SIMBOLOS - 1);
				while(letras_usadas[letra_aleatoria])
				{
					letra_aleatoria = (letra_aleatoria + 1) % CANTIDAD_SIMBOLOS;
				}
				_permutacion[i] = alfabeto[letra_aleatoria];
				letras_usadas[letra_aleatoria] = 1;

				switch(_permutacion[i])
				{
					case 'e': // E
						letra_e = i;
						break;
					case 'a': // A
						letra_a = i;
						break;
					case 'o': // O
						letra_o = i;
						break;
					case 's': // S
						letra_s = i;
						break;
					default:
						break;
				};
			}

			for(int i = 0; i < _pbm.dimension(); i++)
			{
				_var[i] = _permutacion[i];
			}

			for (int i = 0; i < _pbm.dimension(); i++)
			{
				if(_var[i] != 'a' && i == primera_letra_frecuente)
				//en _var[i], debería ir la letra más frecuente (la letra "a")
				{
					char aux_letra = _var[letra_e];
					_var[letra_e] = _var[i];
					_var[i] = aux_letra;
				}
				else if(_var[i] != 'e' && i == segunda_letra_frecuente)
				//en _var[i], debería ir la segunda letra más frecuente (la letra "e")
				{
					char aux_letra = _var[letra_a];
					_var[letra_a] = _var[i];
						_var[i] = aux_letra;
				}
				else if(_var[i] != 'o' && i == tercera_letra_frecuente)
				//en _var[i], debería ir la tercera letra más frecuente (la letra "o")
				{
					char aux_letra = _var[letra_o];
					_var[letra_o] = _var[i];
					_var[i] = aux_letra;
				}
				else if(_var[i] != 's' && i == cuarta_letra_frecuente)
				//en _var[i], debería ir la cuarta letra más frecuente (la letra "s")
				{
					char aux_letra = _var[letra_s];
					_var[letra_s] = _var[i];
					_var[i] = aux_letra;
				}
			}
		}
		else if(_grupos_frec >= 40 && _grupos_frec < 60) // E A S O
		{
			for (int i = 0; i < _pbm.dimension(); i++) // 27
			{
				int letra_aleatoria = rand_int(0,CANTIDAD_SIMBOLOS - 1);
				while(letras_usadas[letra_aleatoria])
				{
					letra_aleatoria = (letra_aleatoria + 1) % CANTIDAD_SIMBOLOS;
				}
				_permutacion[i] = alfabeto[letra_aleatoria];
				letras_usadas[letra_aleatoria] = 1;
				switch(_permutacion[i])
				{
					case 'e': // E
						letra_e = i;
						break;
					case 'a': // A
						letra_a = i;
						break;
					case 'o': // O
						letra_o = i;
						break;
					case 's': // S
						letra_s = i;
						break;
					default:
						break;
				};
			}

			for(int i = 0; i < _pbm.dimension(); i++)
			{
				_var[i] = _permutacion[i];
			}

			for (int i = 0; i < _pbm.dimension(); i++)
			{
				if(_var[i] != 'e' && i == primera_letra_frecuente)
				//en _var[i], debería ir la letra más frecuente (la letra "a")
				{
					char aux_letra = _var[letra_e];
					_var[letra_e] = _var[i];
					_var[i] = aux_letra;
				}
				else if(_var[i] != 'a' && i == segunda_letra_frecuente)
				//en _var[i], debería ir la segunda letra más frecuente (la letra "e")
				{
					char aux_letra = _var[letra_a];
					_var[letra_a] = _var[i];
					_var[i] = aux_letra;
				}
				else if(_var[i] != 's' && i == tercera_letra_frecuente)
				//en _var[i], debería ir la tercera letra más frecuente (la letra "o")
				{
					char aux_letra = _var[letra_o];
					_var[letra_o] = _var[i];
					_var[i] = aux_letra;
				}
				else if(_var[i] != 'o' && i == cuarta_letra_frecuente)
				//en _var[i], debería ir la cuarta letra más frecuente (la letra "s")
				{
					char aux_letra = _var[letra_s];
					_var[letra_s] = _var[i];
					_var[i] = aux_letra;
				}
			}
		}
		else if(_grupos_frec >= 60 && _grupos_frec < 80) // A E S O
		{
			for (int i = 0; i < _pbm.dimension(); i++) // 27
			{
				int letra_aleatoria = rand_int(0,CANTIDAD_SIMBOLOS - 1);
				while(letras_usadas[letra_aleatoria])
				{
					letra_aleatoria = (letra_aleatoria + 1) % CANTIDAD_SIMBOLOS;
				}
				_permutacion[i] = alfabeto[letra_aleatoria];
				letras_usadas[letra_aleatoria] = 1;
				switch(_permutacion[i])
				{
					case 'e': // E
						letra_e = i;
						break;
					case 'a': // A
						letra_a = i;
						break;
					case 'o': // O
						letra_o = i;
						break;
					case 's': // S
						letra_s = i;
						break;
					default:
						break;
				};
			}

			for(int i = 0; i < _pbm.dimension(); i++)
			{
				_var[i] = _permutacion[i];
			}

			for (int i = 0; i < _pbm.dimension(); i++)
			{
				if(_var[i] != 'a' && i == primera_letra_frecuente)
				//en _var[i], debería ir la letra más frecuente (la letra "a")
				{
					char aux_letra = _var[letra_e];
					_var[letra_e] = _var[i];
					_var[i] = aux_letra;
				}
				else if(_var[i] != 'e' && i == segunda_letra_frecuente)
				//en _var[i], debería ir la segunda letra más frecuente (la letra "e")
				{
					char aux_letra = _var[letra_a];
					_var[letra_a] = _var[i];
					_var[i] = aux_letra;
				}
				else if(_var[i] != 'o' && i == tercera_letra_frecuente)
				//en _var[i], debería ir la tercera letra más frecuente (la letra "o")
				{
					char aux_letra = _var[letra_o];
					_var[letra_o] = _var[i];
					_var[i] = aux_letra;
				}
				else if(_var[i] != 's' && i == cuarta_letra_frecuente)
				//en _var[i], debería ir la cuarta letra más frecuente (la letra "s")
				{
					char aux_letra = _var[letra_s];
					_var[letra_s] = _var[i];
					_var[i] = aux_letra;
				}
			}
		}
		else // GRUPO RANDOM
		{
			for (int i = 0; i < _pbm.dimension(); i++) // 27
			{	
				int letra_aleatoria = rand_int(0,CANTIDAD_SIMBOLOS - 1);
				while(letras_usadas[letra_aleatoria])
				{
					letra_aleatoria = (letra_aleatoria + 1) % CANTIDAD_SIMBOLOS;
				}
				_permutacion[i] = alfabeto[letra_aleatoria];
				letras_usadas[letra_aleatoria] = 1;
			}
			for(int i = 0; i < _pbm.dimension(); i++)
			{
				_var[i] = _permutacion[i];
			}
		}
//		cout << "Solution::initialize(): Solucion creada [" << _grupos_frec <<
//				"] = " << (*this) << "\n";
		_grupos_frec = (_grupos_frec + 1) % 100;
	}

	double Solution::fitness()
	{
		string texto_desencriptado = _pbm.desencripta(_pbm.get_texto_cifrado(), _var);
//		cout << "Solution::fitness: Solucion despues de _pbm.desencripta:" << (*this) << "\n";
		Frecuencias_Texto_T frec_texto_desencriptado =  _pbm.calcula_frecuencia(texto_desencriptado);

		double fitness = 0.0;
		for(int i=0; i<CANTIDAD_SIMBOLOS;i++)
		{
			fitness += fabs(_pbm.get_frec_sim_castellano(i) - frec_texto_desencriptado.frec_sim[i]);
		}

		for(int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
		{
			for(int j = 0; j < CANTIDAD_SIMBOLOS; ++j)
			{
				fitness += fabs(_pbm.get_frec_di_castellano(i,j) - frec_texto_desencriptado.frec_di[i][j]);
			}
		}

		for(int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
		{
			for(int j = 0; j < CANTIDAD_SIMBOLOS; ++j)
			{
				for(int k = 0; k < CANTIDAD_SIMBOLOS; ++k)
				{
					fitness += fabs(_pbm.get_frec_tri_castellano(i,j,k) - frec_texto_desencriptado.frec_tri[i][j][k]);
				}
			}
		}
//		cout << "Solution::fitness: Solucion " << (*this) << " >> Fitnes: " << fitness << "\n";
		return fitness;
	}

	char *Solution::to_String() const
	{
		return (char *)_var;
	}

	void Solution::to_Solution(char *_string_)
	{
		char *ptr=_string_;
		for (int i=0;i<_pbm.dimension();i++)
		{
			_var[i]=*ptr;
			ptr++;
		}
	}

	unsigned int Solution::size() const
	{
		return (_pbm.dimension() * sizeof(unsigned char));
	}


	unsigned char& Solution::var(const int index)
	{
		return _var[index];
	}


//	Rarray<unsigned char>& Solution::array_var()
//	{
//		// NO USAR: alcance de temp es array_var
//		Rarray<unsigned char> temp(CANTIDAD_SIMBOLOS);
//		for(int i = 0; i < CANTIDAD_SIMBOLOS; ++i)
//			temp[i] = _var[i];
//		return temp;
//	}

	Solution::~Solution()
	{}

	// UserStatistics -------------------------------------------------------

	UserStatistics::UserStatistics ()
	{}

	ostream& operator<< (ostream& os, const UserStatistics& userstat)
	{
		os << "\n---------------------------------------------------------------" << endl;
		os << "                   STATISTICS OF TRIALS                   	 " << endl;
		os << "------------------------------------------------------------------" << endl;

		for (int i=0;i< userstat.result_trials.size();i++)
		{
			os << endl
			   << userstat.result_trials[i].trial
			   << "\t" << userstat.result_trials[i].best_cost_trial
			   << "\t\t" << userstat.result_trials[i].worst_cost_trial
			   << "\t\t\t" << userstat.result_trials[i].nb_evaluation_best_found_trial
			   << "\t\t\t" << userstat.result_trials[i].nb_iteration_best_found_trial
			   << "\t\t\t" << userstat.result_trials[i].time_best_found_trial
			   << "\t\t" << userstat.result_trials[i].time_spent_trial;
		}
		os << endl << "------------------------------------------------------------------" << endl;
		return os;
	}

	UserStatistics& UserStatistics::operator= (const UserStatistics& userstats)
	{
		result_trials=userstats.result_trials;
		return (*this);
	}

	void UserStatistics::update(const Solver& solver)
	{
		if( (solver.pid()!=0) || (solver.end_trial()!=true)
		  || ((solver.current_iteration()!=solver.setup().nb_evolution_steps())
		       && !terminateQ(solver.pbm(),solver,solver.setup())))
			return;

		struct user_stat *new_stat;

		if ((new_stat=(struct user_stat *)malloc(sizeof(struct user_stat)))==NULL)
			show_message(7);
		new_stat->trial         		 		 = solver.current_trial();
		new_stat->nb_evaluation_best_found_trial = solver.evaluations_best_found_in_trial();
		new_stat->nb_iteration_best_found_trial  = solver.iteration_best_found_in_trial();
		new_stat->worst_cost_trial     		 	 = solver.worst_cost_trial();
		new_stat->best_cost_trial     		 	 = solver.best_cost_trial();
		new_stat->time_best_found_trial		 	 = solver.time_best_found_trial();
		new_stat->time_spent_trial 		 		 = solver.time_spent_trial();

		result_trials.append(*new_stat);
	}

	void UserStatistics::clear()
	{
		result_trials.remove();
	}

	UserStatistics::~UserStatistics()
	{
		result_trials.remove();
	}

// Intra_operator  --------------------------------------------------------------

	Intra_Operator::Intra_Operator(const unsigned int _number_op):_number_operator(_number_op),probability(NULL)
	{}

	unsigned int Intra_Operator::number_operator() const
	{
		return _number_operator;
	}

	Intra_Operator *Intra_Operator::create(const unsigned int _number_op)
	{
		switch (_number_op)
		{
			case 0: return new Crossover;break;
			case 1: return new Mutation();break;
		}
	}

	ostream& operator<< (ostream& os, const Intra_Operator& intra)
	{
		switch (intra.number_operator())
		{
			case 0: os << (Crossover&)intra;break;
			case 1: os << (Mutation&)intra;break;
		}
		return os;
	}

	Intra_Operator::~Intra_Operator()
	{}

//  Crossover:Intra_operator -------------------------------------------------------------

	Crossover::Crossover():Intra_Operator(0)
	{
		probability = new float[1];
	}

	void Crossover::cross(Solution& sol1,Solution& sol2) const // dadas dos soluciones de la poblacion, las cruza
	{
//		cout << "Crossover::cross: Solucion 1: " << sol1 << "\n";
//		cout << "Crossover::cross: Solucion 2: " << sol2 << "\n";

		int i=0;
		unsigned char aux[CANTIDAD_SIMBOLOS];
		for (i = 0; i < CANTIDAD_SIMBOLOS; ++i)
		{
			aux[i] = sol2.var(i);
		}

		// Crossover 1 punto
		int limit = rand_int(0,sol1.pbm().dimension() - 1);

		for (i = 0; i < limit; ++i)
		{
			sol2.var(i) = sol1.var(i);
			sol1.var(i) = aux[i];
		}

//		cout << "Crossover::cross: Solucion 1 [Crossover 1 pt]: " << sol1 << "\n";
//		cout << "Crossover::cross: Solucion 2 [Crossover 1 pt]: " << sol2 << "\n";

		// Después del crossover, marco usados para cada sol
		int _usados_sol1 [CANTIDAD_SIMBOLOS] = {0};
		int _usados_sol2 [CANTIDAD_SIMBOLOS] = {0};
		for (i = 0; i < sol1.pbm().dimension(); ++i)
		{
			_usados_sol1[indice(sol1.var(i))]++;
			_usados_sol2[indice(sol2.var(i))]++;
		}

		int j = -1;
		//
		// Cambio el valor de los repetidos
		// Para Sol 1 
		//
		for (i = 0; i < limit; ++i)
		{
			if (_usados_sol1[indice(sol1.var(i))] > 1) // Letra repetida en individuo
			{
				while(_usados_sol1[++j] && j < CANTIDAD_SIMBOLOS); // Busco letra no usada
				sol1.var(i) = alfabeto[j];
				_usados_sol1[indice(sol1.var(i))]--;
				_usados_sol1[j]++;
			}
		}

		//
		// Cambio el valor de los repetidos
		// Para Sol 2 
		//
		j = -1;
		for (i = 0; i < limit; ++i)
		{
			if (_usados_sol2[indice(sol2.var(i))] > 1) // Letra repetida en individuo
			{
				while(_usados_sol2[++j] && j < CANTIDAD_SIMBOLOS); // Busco letra no usada
				sol2.var(i) = alfabeto[j];
				_usados_sol2[indice(sol2.var(i))]--;
				_usados_sol2[j]++;
			}
		}
		
//		cout << "Crossover::cross: POST Solucion 1 " << sol1 << "\n";
//		cout << "Crossover::cross: POST Solucion 2 " << sol2 << "\n";
	}

	void Crossover::execute(Rarray<Solution*>& sols) const
	{
		for (int i=0;i+1<sols.size();i=i+2)
		 	if (rand01()<=probability[0]) cross(*sols[i],*sols[i+1]);
	}

	ostream& operator<< (ostream& os, const Crossover&  cross)
	{
		 os << "Crossover." << " Probability: "
                    << cross.probability[0]
		    << endl;
		 return os;
	}

	void Crossover::RefreshState(const StateCenter& _sc) const
	{
		_sc.set_contents_state_variable("_crossover_probability",(char *)probability,1,sizeof(float));
	}

	void Crossover::UpdateFromState(const StateCenter& _sc)
	{
		 unsigned long nbytes,length;
		 _sc.get_contents_state_variable("_crossover_probability",(char *)probability,nbytes,length);
	}

	void Crossover::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f ",&op,&probability[0]);
		assert(probability[0]>=0);
	}

	Crossover::~Crossover()
	{
		delete [] probability;
	}

	//  Mutation: Sub_operator -------------------------------------------------------------

	Mutation::Mutation():Intra_Operator(1)
	{
		probability = new float[2];
	}

	void Mutation::mutate(Solution& sol) const
	{
		if (rand01() <= probability[1])
		{
			int indice1 = rand_int(0,sol.pbm().dimension() - 1);
			int indice2 = rand_int(0,sol.pbm().dimension() - 1);
			if(indice2 == indice1)
			{
			 indice2 = (indice2 + 1) % sol.pbm().dimension();			
			}
			char aux = sol.var(indice1);
			sol.var(indice1) = sol.var(indice2);
			sol.var(indice2) = aux;
		}
	}

	void Mutation::execute(Rarray<Solution*>& sols) const
	{
		for (int i=0;i<sols.size();i++)
			if(rand01() <= probability[0])	mutate(*sols[i]);
	}

	ostream& operator<< (ostream& os, const Mutation&  mutation)
	{
		os << "Mutation." << " Probability: " << mutation.probability[0]
		   << " Probability1: " << mutation.probability[1]
		   << endl;
		return os;
	}

	void Mutation::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f %f ",&op,&probability[0],&probability[1]);
		assert(probability[0]>=0);
		assert(probability[1]>=0);
	}

	void Mutation::RefreshState(const StateCenter& _sc) const
	{
		_sc.set_contents_state_variable("_mutation_probability",(char *)probability,2,sizeof(probability));
	}

	void Mutation::UpdateFromState(const StateCenter& _sc)
	{
		unsigned long nbytes,length;
		_sc.get_contents_state_variable("_mutation_probability",(char *)probability,nbytes,length);
	}

	Mutation::~Mutation()
	{
		delete [] probability;
	}

// StopCondition_1 -------------------------------------------------------------------------------------

	StopCondition_1::StopCondition_1():StopCondition()
	{}

	bool StopCondition_1::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)
	{
		return ((int)solver.best_cost_trial() == pbm.dimension());
	}

	StopCondition_1::~StopCondition_1()
	{}

	//------------------------------------------------------------------------
	// Specific methods ------------------------------------------------------
	//------------------------------------------------------------------------

	bool terminateQ (const Problem& pbm, const Solver& solver,
			 const SetUpParams& setup)
	{
		StopCondition_1 stop;
		return stop.EvaluateCondition(pbm,solver,setup);
	}
}
#endif

