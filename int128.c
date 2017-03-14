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

/* Shift para direita (aritmetico) */
void int128_shr (Int128 *res, Int128 *v, int n)
{
	int i;
	long temp; //Váriavel para realizar operação de NOT (~)
	long x = 1; //Váriavel utilizada para realizar shift

	for (i=0; i<n; i++) //Realizando shift à direita de um em um até valor desejado
	{
		if ((v->high & 0x1)==0) //Verifica se bit mais à direita da parte alta está desligado
		{
			v->low >>= 1; //Realiza shift à direita na parte baixa
			if ((v->low & x<<63)!=0) //Verifica se bit mais à esquerda está ligado após shift
			{
				temp = ~v->low; //Guarda em temp o complemento de v->low
				temp |= (x<<63); //Liga o bit mais à esquerda do complemento 
				v->low = ~temp; //Dá um NOT para desligar o bit mais à esquerda e guarda em v->low
			}
		}
		else //Caso o bit mais à direita da parte alta esteja ligado 
		{
			v->low >>= 1; //Realiza shift à direita na parte baixa
			v->low |= (x<<63); //Faz um OR (|) com a mascara para manter o bit ligado após shift
		}
		
		if ((v->high & x<<63)==0) //Verifica se bit mais à esquerda da parte alta está desligado
		{
			v->high >>= 1; //Realiza shift à direita na parte alta
			if ((v->high & x<<63)!=0) //Verifica se bit mais à esquerda está ligado após shift
			{
				temp = ~v->high; //Guarda em temp o complemento de v->high
				temp |= (x<<63); //Liga o bit mais à esquerda do complemento
				v->high = ~temp; //Dá um NOT para desligar o bit mais à esquerda e guarda em v->high
			}
		}
		else //Caso o bit mais à esquerda da parte alta esteja ligado
		{
			v->high >>=1; //Realiza shift à direita na parte alta
			v->high |= (x<<63); //Faz um OR (|) com a mascara para manter o bit ligado após shift
		}
	}
	res->low = v->low;  //Guarda então o resultado da operação em res->low
	res->high = v->high; //Guarda então o resultado da operação em res->high
}

/* Grava um Int128 */
int int128_write(Int128 *v, FILE *f)
{
    int c;
    long h,l;
	unsigned char bytes[8]; 
	
	printf("High: ");

	for( c = 0 ; c < 8 ; c++ )
	{
		h = v->high;
		h <<= (8*c);
		h >>= 56;

		bytes[c]=(unsigned char)h;

		printf("%x - %d\n",bytes[c],c);
	}

	if(fwrite(bytes,1,8,f) != 8 ) 
	{
		return -1;
	}

	printf(" Low: ");

	for(c = 0 ; c < 8 ; c++)
	{
		l=v->low;
		l <<= (8*c);
		l >>= 56;

		bytes[c]=(unsigned char)l;

		printf("%x - %d\n",bytes[c],c);
	}

	if(fwrite(bytes,1,8,f) != 8) 
	{
		return -1;
	}

	return 0;
}


/* Lê um Int128 */
int int128_read(Int128 *v, FILE *f)
{
	int count;
	long sum=0,t;
	unsigned char bytes[8];
	
	if(fread(bytes,1,8,f) != 8) 
	{
		return -1;
	}
		
	for ( count = 0 ; count < 8 ; count++)
	{
		t = bytes[count];
		t <<= (8*(7-count));		
		sum |= t;
	}

	v->high=sum;
	printf("%lx - sum\n",sum);  
	sum=0;
	fread(bytes,1,8,f);
		
	for (count = 0 ; count < 8 ; count++)
	{
		t = bytes[count];
		t <<= (8*(7-count));		
		sum |= t;
	}
	
	v->low=sum;
	printf("%lx - sum\n",sum);  
	printf("Fim\n");
	return 0;	
}

