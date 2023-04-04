#ifndef _H_KALENDAS_
#define _H_KALENDAS_

#include <ctime>

#include "Cpp_Biblioteca.cpp"

/*
ESTA BIBLIOTECA PROPORCIONA ELEMENTOS PARA TRABAJAR CON FECHAS. 
HABRÁ PROBLEMAS CON AQUELLAS QUE ESCAPEN AL PERIODO:
		1-ENERO-2018 a 31-DICIEMBRE-2099
		 ¡ Que viva nuestro siglo XXI !
*/

// Esta biblioteca (entera) es muy optimizable para trabajar con enteros en lugar de cadenas de caracteres
// Postproducción: algunos experimentos dicen que automáticamente se pasa al siglo XXII al terminar. Qué guapo!

using namespace std;

class dia;
class mes;
class anno;

class unidad_tiempo{
	int numero;

	public:
	unidad_tiempo(int);
	int get_numero();
	char* get_n_char();
};
class anno : public unidad_tiempo{
	anno *siguiente=NULL;
	mes **meses=(mes**)calloc(12,sizeof(mes*));
	bool bisiesto;

	public:
	anno(int);
	void describir();
	char* get_fecha_amd();
	bool es_bisiesto();
	anno* get_siguiente();
	void set_siguiente(anno*);
	void set_mes(mes*, int);
	mes* get_mes(int);
};
class mes : public unidad_tiempo{
	anno *superior;
	dia **dias;
	int q_dias;

	public:
	mes(int, anno*);
	void describir();
	char* get_fecha_amd();
	int get_q_dias();
	const char* get_nombre();
	anno* get_anno();
	mes* get_siguiente();
	dia* get_dia(int);
	void set_dia(dia*, int);
};
class dia : public unidad_tiempo{
	mes *superior;
	bool festivo;
	unsigned int fecha=0;
	float evaluacion;

	public:
	dia(int, mes*);
	void describir();
	char* get_fecha_amd();
	char* get_fecha_completa();
	char* get_fecha_larga();
	bool es_festivo();
	const char* get_dia_semana();
	mes* get_mes();
	dia* get_siguiente();
	int get_fecha();
	void set_fecha();
};
dia* crear_dia(char*);
char* fecha();
char* hora();
const char* lit_meses="ene\0feb\0mar\0abr\0may\0jun\0jul\0ago\0sep\0oct\0nov\0dic\0";
const char* lit_semanas="lun\0mar\0mie\0jue\0vie\0sab\0dom\0";
const char* lit_meses_extenso="enero\0febrero\0marzo\0abril\0mayo\0junio\0julio\0agosto\0septiembre\0octubre\0noviembre\0diciembre\0";
const char* lit_semanas_extenso="lunes\0martes\0miércoles\0jueves\0viernes\0sábado\0domingo\0";
anno *semilla = new anno(2018);
anno *fin_siglo = new anno(2100); // No se usa, aunque podría ser útil para evitar cortocircuitos más adelante
// Generalidades de la unidad de tiempo
unidad_tiempo::unidad_tiempo(int n){
	numero=n;
}
char* unidad_tiempo::get_n_char(){
	return int_0s_izq(numero,2);
}
int unidad_tiempo::get_numero(){
	return numero;
}
// Constructores
anno::anno(int n) : unidad_tiempo::unidad_tiempo(n){
	if(n%4) bisiesto=false;
	else bisiesto=true;

	//cout << "Se construye un año: " << get_n_char() << endl;
}
mes::mes(int n, anno* a) : unidad_tiempo::unidad_tiempo(n){
	superior=a;
	switch(n){
		case 1: case 3: case 5: case 7: case 8: case 10: case 12: q_dias=31; break;
		case 4: case 6: case 9: case 11: q_dias=30; break;
		case 2: 
			if(superior->es_bisiesto()) q_dias=29; 
			else q_dias=28; 
			break;
	}
	dias=(dia**)calloc(q_dias,sizeof(dia*));
	superior->set_mes(this,n-1);

	//cout << "Se construye un mes: " << superior->get_n_char() << "_" << get_n_char() << endl;
}
dia::dia(int n, mes* m) : unidad_tiempo::unidad_tiempo(n){
	superior=m;
	superior->set_dia(this,n-1);

	//cout << "Se construye un dia: " << get_fecha_amd() << endl;

	set_fecha();
}
// Descriptores
void anno::describir(){
	int i;

	cout << "Año: " << get_fecha_amd() << endl;
	cout << "Siguiente: " << siguiente << endl;
	
	for(i=0; i<12; i++){
		if(meses[i]) cout << meses[i] << " , ";
		else cout << "NULL , ";
	}
	cout << endl;

	if(bisiesto) cout << "Este año SI es bisiesto" << endl;
	else cout << "Este año NO es bisiesto" << endl;

	cout << endl;
}
void mes::describir(){
	int i;
	cout << "Mes: " << get_fecha_amd() << endl;
	cout << "Superior: " << superior << endl;
	cout << "Tiene dias: " << q_dias << endl;
	for(i=0; i<q_dias; i++){
		if(dias[i]) cout << dias[i] << " , ";
		else cout << "NULL , ";
	}
	cout << endl;

	cout << endl;
}
void dia::describir(){
	cout << "Día: " << get_fecha_amd() << endl;
	cout << "Superior: " << superior << endl;
	cout << "Fecha contada: " << fecha << endl;
	cout << "Dia de la semana: " << get_dia_semana() << endl;

	if(festivo) cout << "Este dia SI es festivo" << endl;
	else cout << "Este dia NO es festivo" << endl;

	cout << endl;
}
char* anno::get_fecha_amd(){
	char *c;
	int i;

	c=cadcrr(2);
	i=0;

	cadins(c,&i,get_n_char());

	return c;
}
char* mes::get_fecha_amd(){
	char *c;
	int i;

	c=cadcrr(5);
	i=0;

	cadins(c,&i,get_anno()->get_n_char());
	cadins(c,&i,'_');
	cadins(c,&i,get_n_char());

	return c;
}
char* dia::get_fecha_amd(){
	char *c;
	int i;

	c=cadcrr(8);
	i=0;

	cadins(c,&i,superior->get_anno()->get_n_char());
	cadins(c,&i,'_');
	cadins(c,&i,superior->get_n_char());
	cadins(c,&i,'_');
	cadins(c,&i,get_n_char());

	return c;
}
char* dia::get_fecha_completa(){
	char* c=cadcrr(14);
	int i(0);

	cadins(c,&i,(char*)get_dia_semana());							// lun		(3)
	cadins(c,&i,' ');												//			(4)
	cadins(c,&i,get_n_char());										// 01		(6)
	cadins(c,&i,' ');												// 			(7)
	cadins(c,&i,(char*)get_mes()->get_nombre());					// ene		(10)
	cadins(c,&i,(char*)" '");										//			(11)
	cadins(c,&i,get_mes()->get_anno()->get_n_char());				// '18		(14)

	return c;
}
char* dia::get_fecha_larga(){
	char* c;
	int i, j, indice, n, d_semana, d_mes;

	n=24;
	indice=0;
	d_semana=fecha%7;
	d_mes=get_mes()->get_numero()-1;

	//cout << "Semana: " << d_semana << " y mes: " << d_mes << endl;

	switch(d_semana){			// (+5) como mínimo (lunes)
		case 1:	n+=1; break;
		case 2:	n+=4; break;
		case 3:	n+=2; break;
		case 4:	n+=2; break;
		case 5:	n+=1; break;
		case 6:	n+=2; break;
	}
	if(get_numero()>=10) n++;	// (+1) como mínimo
	switch(d_mes){				// (+4) como mínimo (mayo)
		case  0: n+=1; break;
		case  1: n+=2; break;
		case  2: n+=1; break;
		case  3: n+=1; break;
		case  5: n+=1; break;
		case  6: n+=1; break;
		case  7: n+=2; break;
		case  8: n+=6; break;
		case  9: n+=3; break;
		case 10: n+=5; break;
		case 11: n+=5; break;
	}

	for(i=0; d_semana; i++) if(!lit_semanas_extenso[i]) d_semana--;
	for(j=0; d_mes; j++) if(!lit_meses_extenso[j]) d_mes--;

	c=cadcrr(n);
	cadins(c,&indice,(char*)lit_semanas_extenso+i);						// longitud variable
	cadins(c,&indice,(char*)", ");										// (+2)
	cadins(c,&indice,int_to_char(get_numero()));						// longitud variable
	cadins(c,&indice,(char*)" de ");									// (+4)
	cadins(c,&indice,(char*)lit_meses_extenso+j);						// longitud variable
	cadins(c,&indice,(char*)" del '");									// (+6)
	cadins(c,&indice,get_mes()->get_anno()->get_n_char());				// (+2)

	return c;
}
// Obtener siguientes
anno* anno::get_siguiente(){
	if(!siguiente) siguiente=new anno(get_numero()+1);
	return siguiente;
}
mes* mes::get_siguiente(){
	mes* siguiente;
	if(get_numero()==12) siguiente=superior->get_siguiente()->get_mes(0);
	else{
		if(!superior->get_mes(get_numero())) new mes(get_numero()+1,superior);
		siguiente=superior->get_mes(get_numero());
	}
	return siguiente;
}
dia* dia::get_siguiente(){
	dia* siguiente;
	if(get_numero()==superior->get_q_dias()) siguiente=superior->get_siguiente()->get_dia(0);
	else{
		if(!superior->get_dia(get_numero())) new dia(get_numero()+1,superior);
		siguiente=superior->get_dia(get_numero());
	}
	return siguiente;
}
// Getters superiores
anno* mes::get_anno(){
	return superior;
}
mes* dia::get_mes(){
	return superior;
}
// Getters inferiores
mes* anno::get_mes(int n){
	if(!meses[n]) meses[n]=new mes(n+1,this);
	return meses[n];
}
dia* mes::get_dia(int n){
	if(!dias[n]) dias[n]=new dia(n+1,this);
	return dias[n];
}
// Setters
void anno::set_siguiente(anno* nuevo){
	if(siguiente) cout << "Deberías llamar a un destructor (set_siguiente)" << endl;
	siguiente=nuevo;
}
void anno::set_mes(mes* nuevo, int n){
	//cout << "Se llama al constructor del mes" << endl;
	if(meses[n]){
		cout << "Deberías llamar a un destructor (set_mes) para: " << endl;
		cout << get_n_char() << "_" << meses[n]->get_n_char() << endl;
	}
	meses[n]=nuevo;
}
void mes::set_dia(dia* nuevo, int n){
	//cout << "Se llama al constructor del dia" << endl;
	if(dias[n]){
		cout << "Deberías llamar a un destructor (set_dia) para: " << endl;
		cout << superior->get_n_char() << "_" << get_n_char() << "_" << dias[n]->get_n_char() << endl;
	}
	dias[n]=nuevo;
}
// Otros
bool anno::es_bisiesto(){
	return bisiesto;
}
bool dia::es_festivo(){
	return festivo;
}
int mes::get_q_dias(){
	return q_dias;
}
const char* mes::get_nombre(){
	return lit_meses+4*(get_numero()-1);
}
const char* dia::get_dia_semana(){
	return lit_semanas+4*(fecha%7);
}
int dia::get_fecha(){
	return fecha;
}
void dia::set_fecha(){
	anno* a_actual;
	mes* m_actual;
	dia* d_actual;

	a_actual=semilla;
	while(a_actual!=superior->get_anno()){
		fecha+=365;
		if(a_actual->es_bisiesto()) fecha++;
		a_actual=a_actual->get_siguiente();
	}

	m_actual=a_actual->get_mes(0);
	while(m_actual!=superior){
		fecha+=m_actual->get_q_dias();
		m_actual=m_actual->get_siguiente();
	}

	d_actual=m_actual->get_dia(0);
	while(d_actual!=this){
		fecha++;
		d_actual=d_actual->get_siguiente();
	}
}
// Funciones sin clase
dia* crear_dia(char* c){
	int a,m,d, i;
	anno* a_actual;
	mes* m_actual;
	dia* d_actual;

	a=10*c[0]+c[1]-528;
	m=10*c[3]+c[4]-528;
	d=10*c[6]+c[7]-528;

	a_actual=semilla;
	for(i=18; i<a; i++) a_actual=a_actual->get_siguiente();

	m_actual=a_actual->get_mes(0);
	for(i=1; i<m; i++) m_actual=m_actual->get_siguiente();

	d_actual=m_actual->get_dia(0);
	for(i=1; i<d; i++) d_actual=d_actual->get_siguiente();

	return d_actual;
}
dia* buscar_coincidencia(dia *d, char*c){
	// Esta función recibe una cadena que representa una fecha. Puede estar en varios formatos:
	// -Tipo "lun" (el próximo lunes)
	// -Tipo "06" (el próximo día 6)
	// -Tipo "03_24" (el próximo 24 de marzo)
	// -Tipo "22_12_03" (el 3 de diciembre del '22) 
	// Y devuelve el (objeto) día correspondiente a partir del día 'd' que se toma como referencia
	// (NOTA sobre la fecha indicando las últimas cifras del año: 
	// en ocasiones funcionará para el s.XXII y de ahí en adelante, pero el resto del programa no está pensado para ello)

	int len;
	dia* nuevo;

	len=(cadlen(c));
	cadsplit(c,'_');
	nuevo=d->get_siguiente();

	switch(len){
		case 2: while(!cadeq(nuevo->get_n_char(),c)) nuevo=nuevo->get_siguiente(); break;
		case 3:	while(!cadeq((char*)nuevo->get_dia_semana(),c)) nuevo=nuevo->get_siguiente(); break;
		case 5: while(!cadeq((char*)nuevo->get_n_char(),c) || !cadeq((char*)nuevo->get_mes()->get_n_char(),c+3)) nuevo=nuevo->get_siguiente(); break;
		case 8: while(!cadeq((char*)nuevo->get_n_char(),c) || !cadeq((char*)nuevo->get_mes()->get_n_char(),c+3) || !cadeq((char*)nuevo->get_mes()->get_anno()->get_n_char(),c+6)) nuevo=nuevo->get_siguiente(); break;
	}

	return nuevo;
}
// Cadenas de caracteres
char* fecha(){
	time_t t=time(NULL);
	char* f=cadcrr(8);
	strftime(f,9,"%2y_%2m_%2d",localtime(&t));
	return f;
}
char* hora(){
	time_t t=time(NULL);
	char* f=cadcrr(8);
	strftime(f,9,"%2H:%2M:%2S",localtime(&t));
	return f;
}
bool es_dia_valido(char *c){
	bool b;
	int i, len;
	
	len=cadlen(c);
	if(len<3){
		b=true;
		for(i=0; b && c[i]; i++) b=(c[i]>=48 && c[i]<=57);
		if(b) b=(char_to_int(c)>0);
	}
	else if(len==3){
		b=false;
		for(i=0; !b && i<28; i+=4) if(cadeq(c,(char*)lit_semanas+i)) b=true;
	}
	else b=false;
	
	return b;
}
bool es_mes_valido(char *c){
	bool b;
	int i, len;
	
	len=cadlen(c);
	if(len<3){
		b=true;
		for(i=0; b && c[i]; i++) b=(c[i]>=48 && c[i]<=57);
		if(b){
			i=char_to_int(c);
			b=(i && i<=12);
		}
	}
	else if(len==3){
		b=false;
		for(i=0; !b && i<48; i+=4) if(cadeq(c,(char*)lit_meses+i)) b=true;
	}
	else b=false;
	
	return b;
}
bool es_anno_valido(char *c){
	bool b;
	int i, len;
	
	len=cadlen(c);
	if(len==2) b=(c[0]>=48 && c[0]<=57 && c[1]>=48 && c[1]<=57);
	else if(len==4) b=(c[0]==50 && c[1]==48 && c[2]>=48 && c[2]<=57 && c[3]>=48 && c[3]<=57);
	else b=false;
	
	return b;
}

#endif