/* AUTOR : LUCAS RODRIGUES */

#include "int128.h"

/* Atribuição */
void int128_attr (Int128 *res, long l) //Extensão de long 64 bits para 128 bits
{
	long x=1; //Váriável temporária para realizar máscara para left shift
	res->low = l; //Atribui o valor de l para a parte baixa do int128
	if((l & x<<63)==0) //Verifica se o bit mais significativo está desligado
		res->high &= 0x0; //Caso caso esteja completa a parte alta com 0 (número positivo)
	else
		res->high |= 0xFFFFFFFFFFFFFFFF; //Caso contrário completa com 1 (número negativo)
}