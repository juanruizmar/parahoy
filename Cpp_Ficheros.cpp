#ifndef _H_FICHEROS_
#define _H_FICHEROS_

#include <stdlib.h>

#include "Cpp_Biblioteca.cpp"

/*
ESTA BIBLIOTECA PROPORCIONA ELEMENTOS PARA TRABAJAR CON FICHEROS. 
LA IDEA ES HACERLO DE FORMA ÁGIL CON TODO OPTIMIZADO PARA XII
*/

class cadena{
	char* texto;
	int len;

	public:
	cadena(int);
	cadena(char*);
	char* get_texto();
	int get_len();
	void set_len(int);

	void imprimir();
	void imprimir(int, int);
	void imprimir(int, int, char*);
	void escanear(char*);
};
class linea : public cadena{
	linea* siguiente;

	public:
	linea();
	linea(int);
	linea(char*);
	linea* get_siguiente();
	void set_siguiente(linea*);
	void prolongar(linea*);
	void imprimir_texto();
	void imprimir_texto(int);
};
// Constructores
cadena::cadena(int len){
	texto=(char*)malloc(len+1);
	texto[len]=0;
	len=len;
}
cadena::cadena(char* c){
	for(len=0; c[len]; len++);
	texto=(char*)malloc(len+1);
	for(len=0; c[len]; len++) texto[len]=c[len];
	texto[len]=0;
}
linea::linea(int len) : cadena(len){
	siguiente=NULL;
}
linea::linea(char* c) : cadena(c){
	siguiente=NULL;
}
// Getters
char* cadena::get_texto(){
	return texto;
}
int cadena::get_len(){
	return len;
}
linea* linea::get_siguiente(){
	return siguiente;
}
// Setters
void cadena::set_len(int l){
	texto=(char*)malloc(l+1);
	texto[l]=0;
	len=l;
}
void linea::set_siguiente(linea *l){
	siguiente=l;
}
// Cuasi-setters (no lo son exactamente, pero funcionan como tal)
void cadena::escanear(char *c){
	//free(texto);	ESTA LÍNEA DA VIOLACIÓN DE SEGMENTO ¿POR QUÉ?
	for(len=0; c[len]; len++);
	texto=(char*)malloc(len+1);
	for(len=0; c[len]; len++) texto[len]=c[len];
	texto[len]=0;
}
void linea::prolongar(linea *l){
	linea *actual;
	for(actual=this; actual->get_siguiente(); actual=actual->get_siguiente());
	actual->set_siguiente(l);
}
// Otros
void cadena::imprimir(){
	cout << texto << endl;
}
void cadena::imprimir(int n, int l){
	char* c = int_0s_izq(n+1,l);
	cout << c << " : ";
	free(c);
	
	imprimir();
}
void cadena::imprimir(int n, int l, char* c){
	// PRE: c está reservada con longitud l+1, y termina en \0
	int_0s_izq(n+1,l,c);
	cout << c << " : " << texto << endl;
}
void linea::imprimir_texto(){
	linea* actual;

	for(actual=this; actual; actual=actual->get_siguiente()) actual->imprimir();
}
void linea::imprimir_texto(int l){
	int i;
	linea* actual;

	i=0;
	for(actual=this; actual; actual=actual->get_siguiente()) actual->imprimir(i++,l);
}
// Manipular listas enlazadas
char* extraer_linea(linea **primera, int n_extraida){
	// PRE: n_extraida es inferior a la cantidad de elementos en la lista
	// POS: en la lista que comienza en *primera, se ha eliminado la n-ésima línea y se devuelve su texto
	char *txt;
	int i;
	linea *extraida;//, *aux;

	if(n_extraida){
		extraida=*primera;
		for(i=1; i<n_extraida && extraida; i++) extraida=extraida->get_siguiente();
		if(extraida){
			//aux=extraida->get_siguiente();
			txt=extraida->get_siguiente()->get_texto();
			extraida->set_siguiente(extraida->get_siguiente()->get_siguiente());
			//extraida=aux;
		}
		else txt=NULL;
	}
	else{
		txt=(*primera)->get_texto();
		*primera=(*primera)->get_siguiente();
	}

	//for(aux=*primera; aux; aux=aux->get_siguiente()) aux->imprimir();

	return txt;
}
// En ficheros
int len_fichero(FILE *f){
	char c;
	int n(0);
	
	do{
		c=fgetc(f);
		if(c=='\n') n++;
	}while(c!=EOF);
	rewind(f);

	return n;
}
linea* hacer_lineas(linea* primera, FILE *f){
	// Uso muy poco intuitivo.
	// PRE: Ya se han reservado las líneas que se van a ocupar.
	// POS: Se han asignado longitudes y texto con el contenido de un fichero.
	char *txt;
	int i,len;
	linea *actual;

	for(actual=primera; actual; actual=actual->get_siguiente()){
		for(i=0; fgetc(f)!='\n'; i++);
		actual->set_len(i);
	}
	rewind(f);

	for(actual=primera; actual; actual=actual->get_siguiente()){
		len=actual->get_len();
		txt=actual->get_texto();

		for(i=0; i<len; cadins(txt,&i,fgetc(f)));
		fgetc(f);
	}
	rewind(f);

	return primera;
}
linea* leer_fichero(char* nombre){
	char *txt;
	int n,i;

	FILE* f;
	linea *v;

	if(f=fopen(nombre,"r")){
		n=len_fichero(f);
		v=(linea*)malloc(n*sizeof(linea));
		for(i=0; i<n-1; i++) v[i].set_siguiente(v+i+1); v[i].set_siguiente(NULL);

		hacer_lineas(v,f);
		fclose(f);
	}
	else v=NULL;

	return v;
}
linea* leer_fichero(char* nombre, char* nueva_linea){
	// Similar a la anterior, pero en el resultado devuelto hay una nueva línea
	char *txt;
	int n,i;

	FILE* f;
	linea *v;
 
	if(f=fopen(nombre,"r")){
		n=len_fichero(f);
		v=(linea*)malloc((n+1)*sizeof(linea));
		for(i=0; i<n-1; i++) v[i].set_siguiente(v+i+1); v[i].set_siguiente(NULL);

		hacer_lineas(v,f);
		fclose(f);

		v[i].set_siguiente(v+n);
		v[n].set_siguiente(NULL);
		v[n].escanear(nueva_linea);
	}
	else v = new linea(nueva_linea);

	return v;
}
void escribir_fichero(char *nombre, linea *actual){
	FILE *f;

	f=fopen(nombre,"w");
	while(actual){
		fprintf(f,"%s\n",actual->get_texto());
		actual=actual->get_siguiente();
	}
	fclose(f);
}
// Previtrubio
linea** organizar_parametros(int argc, char **argv, int n, char* centinelas...){
	// PRE: recibe unos parámetros que previamente fueron pasados a una función.
	// POS: Los parámetros han sido organizados acorde a unos valores centinela que eran buscados
	int i,j;

	linea **todos, *actual;	
	va_list valist;

	todos=(linea**)malloc((n+1)*sizeof(linea*));

	va_start(valist,centinelas);
	// Esto NO es efecto primate
	todos[0]=new linea(argv[0]);
	for(i=1; i<=n; i++) todos[i] = new linea(va_arg(valist,char*));
	va_end(valist);

	j=0;
	actual=todos[0];
	for(i=1; i<=argc; i++){
		if(!cadeq(argv[i],todos[j]->get_texto())){
			actual->set_siguiente(new linea(argv[i]));
			actual=actual->get_siguiente();
		}
		else actual=new linea(todos[j++]->get_texto());
	}

	return todos;
}

#endif