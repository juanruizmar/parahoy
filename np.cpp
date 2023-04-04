#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

#include "../ALEJANDRIA/fecha_hora.h"

using namespace std;

#define NDEBUG

enum{ANNO, MES, DIA, DIA_SEM};

class Cola;
class Fecha;

inline bool es_bisiesto(int);
inline int n_dias_mes(int, int);
inline int leer_dia_mes(string);
inline int leer_dia_semana(string);
inline int leer_nombre_mes(string);
int calcular_dia_semana(int, int, int);

class Fecha{
public:
	Fecha(int argn, char** argc){
		assert(1<=argn && argn<=3);

		int d_sem;

		// Esto no es todo lo eficiente que se podría desear...
		anno(anno_actual());
		mes(mes_actual());
		dia(dia_actual());
		fijar_dia_semana();

		d_sem = dia_sem();
		
		if(argn>1){
			dia_sem(leer_dia_semana(string(argc[1])));
			if(argn==2){											// Formato 'lun' o '4'
				if(dia_sem()==7){									// Formato '4'
					dia(leer_dia_mes(string(argc[1])));
					if(dia_actual()>dia()) avanzar_mes();
					if(dia()<=31) while(dia()>n_dias_mes(anno(),mes())) avanzar_mes(); // Línea optimizable
					fijar_dia_semana();
				}else{												// Formato 'lun'
					avanzar_dias(
						dia_sem() > d_sem ? 
						dia_sem() - d_sem : 
						dia_sem() - d_sem +7
					);
				}
			}else{													// Formato '3 ene'
				dia(leer_dia_mes(string(argc[1])));
				mes(leer_nombre_mes(string(argc[2])));
				if(
					mes_actual()>mes() || 
					(mes_actual()==mes() && dia_actual()>=dia())
				) avanzar_anno();
				fijar_dia_semana();
			}
		}

		
		nombrar_fichero();
	}
	Fecha(int a, int m, int d){
		anno(a);
		mes(m);
		dia(d);

		fijar_dia_semana();
		nombrar_fichero();
	}

	inline int anno() const{
		return datos_[ANNO];
	}
	inline int mes() const{
		return datos_[MES];
	}
	inline int dia() const{
		return datos_[DIA];
	}
	inline int dia_sem() const{
		return datos_[DIA_SEM];
	}
	inline const char *nombre_fichero(){
		return (new string("/usr/lib/parahoy/"+nombre_fichero_))->c_str();
	}

	bool es_valida(){
		return (
			anno()>=23 && 
			mes() && mes()<=12 &&
			dia() && (
				dia()<=Fecha::dias_mes[mes()-1] || 
				(
					es_bisiesto() && 
					mes()==2 && 
					dia()==29
				)
			)
		);
	}

	template <typename OS> inline friend OS &operator <<(OS &os, const Fecha &f){
		os 
			<< nombres_dias_semana[f.datos_[DIA_SEM]] << ", " 
			<< f.datos_[DIA] << " de " 
			<< nombres_meses_largos[f.datos_[MES]-1] << " del '" 
			<< f.datos_[ANNO];
		return os;
	}

	static int dias_mes[12];
	static string nombres_dias_semana[7], nombres_meses[12], nombres_meses_largos[12];

private:
	int datos_[4];
	string nombre_fichero_;

	inline void anno(int i){
		datos_[ANNO]=i;
	}
	inline void mes(int i){
		datos_[MES]=i;
	}
	inline void dia(int i){
		datos_[DIA]=i;
	}
	inline void dia_sem(int i){
		datos_[DIA_SEM]=i;
	}

	inline bool es_bisiesto(){
		return !(anno()%4);
	}
	inline void avanzar_dias(int n){
		datos_[DIA]+=n;
		if(datos_[DIA]>n_dias_mes(datos_[ANNO],datos_[MES])){
			datos_[DIA]-=n_dias_mes(datos_[ANNO],datos_[MES]);
			avanzar_mes();
		}
	}
	inline void avanzar_mes(){
		if(mes()!=12) ++datos_[MES];
		else{
			mes(1);
			++datos_[ANNO];
		}
	}
	inline void avanzar_anno(){
		++datos_[ANNO];
	}
	inline void nombrar_fichero(){
		nombre_fichero_=".tx_"+itos(datos_[ANNO])+"_"+itos(datos_[MES])+"_"+itos(datos_[DIA])+".txt";
	}
	inline string itos(int n){
		if(n<10) return "0"+to_string(n);
		else return to_string(n);
	}

	void fijar_dia_semana(){
		int i, fecha;

		fecha=0;
		for(i=23; i<anno(); ++i){
			fecha+=365;
			if(!(i%4)) ++fecha;		// Aquí la función "es_bisiesto(int)" daba error
		}
		for(i=1; i<mes(); ++i) fecha+=n_dias_mes(anno(),i);
		for(i=1; i<dia(); ++i) ++fecha;

		if(es_bisiesto() && mes()>2) ++i;

		dia_sem((fecha+6)%7);
	}
};

int Fecha::dias_mes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
string Fecha::nombres_dias_semana[7] = {"lun", "mar", "mie", "jue", "vie", "sab", "dom"};
string Fecha::nombres_meses[12] = {"ene", "feb", "mar", "abr", "may", "jun", "jul", "ago", "sep", "oct", "nov", "dic"};
string Fecha::nombres_meses_largos[12] = {"enero", "febrero", "marzo", "abril", "mayo", "junio", "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"};
//Fecha Fecha::hoy;

inline bool es_bisiesto(int i){
	return !(i%4);
}
inline int n_dias_mes(int a, int m){
	assert(0<m && m<=12);
	if(es_bisiesto(a) && m==2) return 29; 
	else return Fecha::dias_mes[m-1];
}
inline int leer_dia_mes(string d){
	if(d.size()==1) return d[0]-'0';
	else if(d.size()==2) return (d[0]-'0')*10+d[1]-'0';
	else return 0;
}
inline int leer_dia_semana(string d){
	int i;
	for(i=0; i<7 && d!=Fecha::nombres_dias_semana[i]; ++i);
	return i;
}
inline int leer_nombre_mes(string m){
	int i;
	for(i=0; i<12 && m!=Fecha::nombres_meses[i]; ++i);
	return ++i%12;
}

int calcular_dia_semana(int a, int m, int d){
	int i,fecha;

	fecha=0;
	for(i=23; i<a; ++i){
		fecha+=365;
		if(es_bisiesto(i)) ++fecha;
	}
	for(i=1; i<m; ++i) fecha+=n_dias_mes(a,i);
	for(i=1; i<=d; ++i) ++fecha;

	if(es_bisiesto(a) && m>2) ++i;

	return (fecha-1)%7;
}

int main(int argn, char **argc){
	#define LEN_BUFFER 512

	char* buffer = new char[LEN_BUFFER];
	Fecha objetivo(argn, argc);

	if(objetivo.es_valida()){
		cout << "Añadir tareas para el " << objetivo << ":" << endl;
		ofstream os(objetivo.nombre_fichero(), ofstream::app);
		
		// Efecto primate
		cin.getline(buffer,LEN_BUFFER);
		while(buffer[0]){
			os << buffer << endl;
			cin.getline(buffer,LEN_BUFFER);
		}
		os.close();
	}
	else cout << "LA FECHA INTRODUCIDA NO ES VÁLIDA" << endl;

	#undef LEN_BUFFER

	delete buffer;

	return 0;
}