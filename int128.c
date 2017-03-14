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

/* Subtração */
void int128_sub (Int128 *res, Int128 *v1, Int128 *v2)
{
	signed long a,b,c,d;
	unsigned long k,i;
	
	a = v1->high; 
	b = v2->high; 
	c = v1->low ;
	d = v2->low ;

	if (a >= b) 
	{
		if (c >= d) 
		{
			res->high = v1->high + ~v2->high+1; 
			res->low  = v1->low  + ~v2->low+1;
		}
		else //Se c < d
		{
			res->high = v1->high + ~v2->high; 
			res->low  = v1->low  + ~v2->low+1;
		}
	}
	else // Se a < b
	{
		if (c >= d) 
		{
			res->high = v1->high + ~v2->high+1; 
			res->low  = v1->low  + ~v2->low+1;

			k = 0xFFFFFFFFFFFFFFFF - res->low; //Verifica a diferença 

			for ( i=0xFFFFFFFFFFFFFFFF ; i >= 0 ; i<<=4 ) //Verifica a partir de onde colocar os "F"
			{ //Verifica a partir de onde colocar os "F"
				if(i<k)break;
			}

			res->low |= i;
		}
		else //Se c < d
		{
			res->high = v1->high + ~v2->high;
			res->low  = v1->low  + ~v2->low+1;
		}
	}
}

/* Shift para esquerda */
void int128_shl (Int128 *res, Int128 *v, int n)
{
	int i; 
	long x = 1; //Variável para realizar operação de shift
	
	for (i=0; i<n; i++) //Realizando shift de um em um até o valor desejado n.
	{
		if ((v->low & x<<63)==0) //Verifica se o bit mais à esquerda de v->low está desligado
		{
			v->low <<= 1; //Se estiver realiza o shift à esquerda somente
			v->high <<= 1;
		}
		else //Caso contrário realiza o shift à esquerda e passa o bit aceso para o v->high
		{
			v->low <<= 1; //Realiza shift à esquerda na parte baixa
			v->high <<=1; //Realiza shift à esquerda na parte alta
			v->high |= 0x01; //Realiza um "ou" lógico (|) para acender primeiro bit em v->high
		}
	}
	res->low = v->low; //Atribui o resultado do shift em Int128
	res->high = v->high;
}