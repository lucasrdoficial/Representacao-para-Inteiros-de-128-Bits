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

/* Soma */
void int128_add (Int128 *res, Int128 *v1, Int128 *v2)
{
	int a,b,c,d; //Váriaveis para type cast
	long soma1, soma2; //Variáveis para somar com type cast
	long temp1, temp2; //Variáveis para realizar shift e reduzir 64 para 32 bits
	long x=1; //Variável para manipulação de shift

	temp1 = v1->low >> 32; //Reduzindo v1->low de 64 para 32 bits
	temp2 = v2->low >> 32; //Reduzindo v2->low de 64 para 32 bits 

	a = (int) v1->low; 
	b = (int) v2->low; //Type cast com truncamento em 32 bits
	c = (int) temp1;
	d = (int) temp2;

	soma1 = (long) a + b; //Somando as segundas metades int c/ type cast para ver overflow
	if ((soma1 & x<<32)==0) //Verifica se não houve overflow em soma1
	{
		soma2 = (long) c + d; //Soma as primeiras metades int c/ type cast para ver overflow
		if ((soma2 & x<<32)==0) //Verifica se não houve overflow em soma2
		{
			res->high = v1->high + v2->high; //Guarda em res a soma de v1 + v2
			res->low  = v1->low  + v2->low;
		}
		else //Se houve overflow em soma2
		{
			res->high = v1->high + v2->high + 1; //Incrementa a parte alta de res
			res->low  = v1->low  + v2->low;
		}
	}
	else //Se houve overflow em soma1
	{
		//Incrementa na soma das primeiras metades int c/ type cast long
		soma2 = (long) c + d + 1; 
		if ((soma2 & x<<32)==0) //Verifica se não houve overflow em soma2
		{
			res->high = v1->high + v2->high; //Guarda em res a soma de v1 + v2
			res->low  = v1->low  + v2->low;
		}
		else //Se houve overflow em soma2
		{
			res->high = v1->high + v2->high + 1; //Incrementa a parte alta de res
			res->low  = v1->low  + v2->low;
		}
	}
}