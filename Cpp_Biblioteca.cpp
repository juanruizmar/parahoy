#ifndef _H_BIBLIOTECA_
#define _H_BIBLIOTECA_

#define uc unsigned char
#define us unsigned short
#define ui unsigned int
#define lui long unsigned int
//#define min(a,b) ((a)<(b)?(a):(b))
//#define max(a,b) ((a)>(b)?(a):(b))

#define N_NBG 16

#include <algorithm>
#include <iostream>
#include <cstdarg>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

// Básicas
inline int redond(double);
inline void swap(int*,int*);
inline int log_10(int);
inline char* int_to_char(int);
inline int char_to_int(char*);
inline int int_max(int,int);
inline int int_min(int,int);
// Cadenas de caracteres: primera generación
inline char* cadcrr(int);
inline int cadlen(char*);
inline int cadcpy(char*, char*);
inline void cadins(char*,int*,char*);
inline void cadins(char*,int*,char); 
inline bool cadeq(char*,char*);
inline bool cadlex(char*,char*);
// Cadenas de caracteres: segunda generación
void cadscan(char*,char*);
// Pseudo-aleatorios e irracionales
inline int int_rand(int,int);
inline double double_rand(int,int);
double pi_aprox(long int);
// Vectores
char* int_0s_izq(int, int);
void int_0s_izq(int,int,char*);
void imprimir_vector(int,int,int);
bool vector_en_orden(int,int);
void intrcmbio(int,int);
void seleccion(int,int);
void insercion(int,int);

// Básicas
inline int redond(double n_in){
	int n_ot=(int)n_in;
	
	if(n_in-n_ot>=0.5) n_ot++;

	return n_ot;
}
inline void swap(int *a, int *b){
	int aux = *a;
	*a=*b;
	*b=aux;
}
inline int log_10(int n){
	int log=0;
	for(log=0; n; log++) n/=10;
	return log;
}
inline char* int_to_char(int n){
	uc i=log_10(n);
	char* cadena=cadcrr(i);
	for(cadena=cadcrr(i); n; n/=10) cadena[--i]='0'+n%10;
	return cadena;
}
inline int char_to_int(char* c){
	int i, n=0;
	for(i=0; c[i]; i++){
		n*=10;
		n+=c[i]-48;
	}
	return n;
}
inline int int_max(int *v, int n){
	int i, max=v[0];
	for(i=1; i<n; i++) if(v[i]>v[i-1]) max=v[i];
	return max;
}
inline int int_min(int *v, int n){
	int i, min=v[0];
	for(i=1; i<n; i++) if(v[i]<v[i-1]) min=v[i];
	return min;
}
// Cadenas de caracteres: primera generación
inline char* cadcrr(int n){
	char* c=(char*)malloc(n+1);
	c[n]='\0';
	return c;
}
inline int cadlen(char *cadena){
	int i;
	for(i=0; cadena[i]; i++);
	return i;
}
inline int cadcpy(char *destino, char *origen){
	// Seguramente de problemas. Se debería evitar su uso
	int i, len;
	free(destino);
	len=cadlen(origen);

	for(i=0; origen[i]; i++) destino[i]=origen[i];
	return i;
}
inline int cadchn(char *destino, char *origen){
	int i;

	for(i=0; origen[i]; i++) destino[i]=origen[i];
	return i;
}
inline void cadsplit(char* cad, char c){
	int i;
	for(i=0; cad[i]; i++) if(cad[i]==c) cad[i]=0;
}
inline void cadins(char *destino, int *indice, char *origen){
	int i;
	for(i=0; origen[i]; i++) destino[(*indice)++]=origen[i];
}
inline void cadins(char *destino, int *indice, char car){
	destino[(*indice)++]=car;
}
inline bool cadeq(char *cad_a, char *cad_b){
	int i;
	for(i=0; cad_a[i] & cad_b[i] && cad_a[i]==cad_b[i]; i++);
	if(cad_a[i] | cad_b[i]) i=0;
	return i;
}
inline bool cadlex(char* cad_a, char* cad_b){
	int i;
	for(i=0; cad_a[i] & cad_b[i] && cad_a[i]==cad_b[i]; i++);
	if(cad_a[i] & cad_b[i]) i=(cad_a[i]<cad_b[i]);
	else i=0;
	
	return i;
}
// Cadenas de caracteres: segunda generación
void cadscan(char *destino, char *origen){
	// Al igual que cadcpy, podría dar problemas.
	// Lo idóneo sería ir sustituyendo todas estas por una clase
	// PRE: destino ya tiene reservada (como mínimo) la memoria que va a necesitar
	int i;
	for(i=0; origen[i]; i++) destino[i]=origen[i];
	destino[i]=0;
}
// Pseudo-aleatorios e irracionales
inline int int_rand(int a, int b){
	return a+rand()%(b-a+1);
}
inline double double_rand(int a, int b){
	return a+(double)(rand()%0x10000)*(b-a)/0x10000;	// Fracción con 2^16 en el denominador
}
double pi_aprox(long int n){
	long int i;
	double x, sum;

	sum=0;
	for(i=0; i<n; i++){
		x=double_rand(0,1);
		sum+=4*sqrt(1-(x*x));
	}

	return sum/n;
}
// Vectores
char* int_0s_izq(int n, int l){
	char* c=cadcrr(l);
	int i,top;

	top=1;
	for(i=0; i<l; i++) top*=10;

	for(i=0; i<l; i++){
		n%=top;
		top/=10;
		c[i]=n/top+48;
	}

	return c;
}
void int_0s_izq(int n, int l, char *c){
	int i,top;

	top=1;
	for(i=0; i<l; i++) top*=10;

	for(i=0; i<l; i++){
		n%=top;
		top/=10;
		c[i]=n/top+48;
	}
}
void imprimir_vector(int *v, int n, int n_0){
	char *c;
	int i;

	// En esta función ocurre el efecto primate

	c=int_0s_izq(v[0],n_0);
	cout << "( " << c;
	free(c);

	for(i=1; i<n; i++){
		c=int_0s_izq(v[i],n_0);
		cout << " , " << c;
		free(c);
	}
	cout << " )\n";
}
bool vector_en_orden(int *v, int n){
	bool b=true;
	int i; 

	for(i=1; i<n && b; i++) if(v[i]<v[i-1]) b=false;
	return b;
}
void intrcmbio(int *v, int n){
	int i,j;
	for(i=0; i<n; i++) for(j=1; j<n-i; j++) if(v[j-1]>v[j]) swap(v+j-1,v+j);
}
void seleccion(int *v, int n){
	int i,j,i_min;

	for(i=0; i<n-1; i++){
		i_min=i;
		for(j=i+1; j<n; j++) if(v[i_min]>v[j]) swap(v+i,v+j);
	}
}
void insercion(int *v, int n){
	bool bi=false, bj=false;
	int i=0, j=n-1;

	while(i!=j){
		while(i!=j && !bi){
			if(v[i]>v[0]) bi=true;
			else i++;
		}
		while(i!=j && !bj){
			if(v[j]<v[0]) bj=true;
			else j--;
		}
		if(bi && bj){
			swap(v[i],v[j]);
			bi=false;
			bj=false;
		}
	}
	swap(v,v+i-1);
	if(v[n-2]>v[n-1]) swap(v+n-2,v+n-1);

	if(i+1>2) insercion(v,i);
	if(n-j>2) insercion(v+j,n-j);
}

#endif