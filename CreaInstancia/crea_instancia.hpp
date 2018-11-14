//
// crea_instancia.hpp
//
//  Created on: 13/11/2018
//      Author: marcelo
//

#ifndef CREA_INSTANCIA_HPP_
#define CREA_INSTANCIA_HPP_

#include <string>

enum constantes
{
	CANTIDAD_SIMBOLOS			= 27,

	// Constantes para identificar letras con tildes
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


const char alfabeto[CANTIDAD_SIMBOLOS] =
		{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
		  'j', 'k', 'l', 'm', 'n', ENIE, 'o', 'p', 'q',
		  'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };


#endif // CREA_INSTANCIA_HPP_
