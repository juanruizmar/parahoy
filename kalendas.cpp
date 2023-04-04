#include "Cpp_Ficheros.cpp"
#include "Cpp_Kalendas.cpp"

// Los programas que salen "ad hoc" de aquí son el ph, el np, el rph y el prc
class info{
public:
	int n, *v;
	dia *dias[2];
};
info leer_parametros(linea** todos){
	// PRE: recibe una serie de parámetros previamente organizados acorde al sistema de centinelas
	// POS: devuelve una estructura ('info') con todos los parámetros del programa listos para utilizar
	int i,j;
	char *cadena;

	dia *hoy;
	info data;
	linea *actual;

	hoy=crear_dia(fecha());

	data.n=0;
	for(actual=todos[0]->get_siguiente(); actual; actual=actual->get_siguiente()) data.n++;
	data.v=(int*)malloc(data.n*sizeof(int));

	// Lo que hay de aquí en adelante es lo más chapucero del mundo
	// Una función con más de 70 líneas. Tiene el tamaño de un orangután

	for(i=0; i<2; i++){	// Este código es una porquería
		j=0;
		for(actual=todos[i]->get_siguiente(); actual; actual=actual->get_siguiente()) data.v[j]=char_to_int(actual->get_texto());
		insercion(data.v,data.n);

		actual=todos[i]->get_siguiente();
		if(es_dia_valido(actual->get_texto())){
			if(cadlen(actual->get_texto())==2){
				actual=actual->get_siguiente();
				if(actual){
					if(es_mes_valido(actual->get_texto())){
						actual=actual->get_siguiente();
						if(actual){
							if(es_anno_valido(actual->get_texto())) cadena=cadcrr(8);
							else cout << "En cierto momento, el año introducido ha dado errores" << endl;
						}
						else cadena=cadcrr(5);
					}
					else cout << "En cierto momento, el mes introducido ha dado errores" << endl;
				}
				else cadena=cadcrr(2);
			}
			else cadena=cadcrr(3);
		}
		else cout << "En cierto momento, el día introducido ha dado errores" << endl;

		actual=todos[i]->get_siguiente();
		if(es_dia_valido(actual->get_texto())){
			j=0;
			cadins(cadena,&j,actual->get_texto());
			actual=actual->get_siguiente();
			if(actual){
				if(es_mes_valido(actual->get_texto())){
					cadins(cadena,&j,'_');
					cadins(cadena,&j,actual->get_texto());
					actual=actual->get_siguiente();
					if(actual){
						if(es_anno_valido(actual->get_texto())){
							cadins(cadena,&j,'_');
							cadins(cadena,&j,actual->get_texto());
							cadena=cadcrr(8);
						}
						else cout << "En cierto momento, el año introducido ha dado errores (bis)" << endl;
					}
				}
				else cout << "En cierto momento, el mes introducido ha dado errores (bis)" << endl;
			}
		}
		else cout << "En cierto momento, el día introducido ha dado errores (bis)" << endl;

		if(i) data.dias[i]=buscar_coincidencia(data.dias[i-1],cadena);
		else data.dias[0]=buscar_coincidencia(hoy,cadena);
		// Vaya porquería de código
	}
	return data;
} 
char* leer_fecha(char *d, char *m, char *a){
	// PRE: d, m y a representan un día, mes y año válidos. m y a pueden ser NULOS, pero d no
	// POS: devuelve una fecha apta para la función "buscar coincidencia"
	char* c;
	int i, indice(0), len;

	if(!m) c=cadcrr(2);
	else if(!a) c=cadcrr(5);
	else c=cadcrr(8);

	len=cadlen(d);
	if(len<=2){
		cadins(c,&indice,int_0s_izq(char_to_int(d),2));
		if(m){
			len=cadlen(m);
			if(len<=2){
				cadins(c,&indice,'_');
				cadins(c,&indice,int_0s_izq(char_to_int(m),2));
			}
			else if(len==3){
				for(i=0; i<48 && !cadeq(m,(char*)lit_meses+i); i+=4);
				if(i<48){
					cadins(c,&indice,'_');
					cadins(c,&indice,int_0s_izq(1+i/4,2));
				}
			}
			if(a){
				if(cadlen(a)==4) a+=2;
				cadins(c,&indice,'_');
				cadins(c,&indice,int_0s_izq(char_to_int(a),2));
			}
		}
	}
	else if(len==3){
		for(i=0; i<28 && !cadeq(d,(char*)lit_semanas+i); i+=4);
		c=d;
	}

	return c;
}
char* obtener_ruta(char* d_char){
	int i;
	char* nombre;
	
	i=0;
	nombre=cadcrr(33);
	cadins(nombre,&i,(char*)"/usr/lib/parahoy/");
	cadins(nombre,&i,(char*)".tx_");
	cadins(nombre,&i,d_char);
	cadins(nombre,&i,(char*)".txt");

	return nombre;
}
int* ordenar_argv(int argc, char *argv[]){
	int i, *v;

	v=(int*)malloc((argc-1)*sizeof(int));
	for(i=1; i<argc; i++) v[i-1]=char_to_int(argv[i]);
	insercion(v,argc-1);

	return v;
}
// Más concretas
void mostrar_tareas(char *d_char){
	int i;
	linea *actual;

	actual=leer_fichero(obtener_ruta(d_char));
	if(actual) actual->imprimir_texto(2);
}
void leer_plan(dia *d){
	char *hoy, *d_char;
	int i;

	linea *actual;

	d_char=d->get_fecha_amd();
	hoy=fecha();

	actual=leer_fichero(obtener_ruta(d_char));

	if(actual){
		if(cadeq(d_char,hoy)) cout << "Para hoy";
		else cout << "Para el " << d->get_fecha_larga();
		cout << " tienes pendiente: " << endl;

		mostrar_tareas(d_char);

		//for(i=0; actual; actual=actual->get_siguiente()) actual->imprimir(i++,2);
	}
	else{
		if(cadeq(d_char,hoy)) cout << "Para hoy";
		else cout << "Para el " << d->get_fecha_larga();
		cout << " no tienes nada pendiente" << endl;
	}
}
void annadir_tarea(dia *d, char *tarea){
	char *nombre;
	int i;

	linea *actual;
		
	nombre=obtener_ruta(d->get_fecha_amd());

	actual=leer_fichero(nombre,tarea);
	escribir_fichero(nombre,actual);
}
char* extraer_tarea(dia* d, int n_tarea){
	char *nombre, *extraida;
	int i;

	linea *actual;

	nombre=obtener_ruta(d->get_fecha_amd());
	actual=leer_fichero(nombre);
	extraida=extraer_linea(&actual,n_tarea-1);
	escribir_fichero(nombre,actual);

	return extraida;
}
void posponer_todo(dia* origen, dia* destino){
	char *nombre;
	int i;
	linea *l_ori, *l_des;

	nombre=obtener_ruta(origen->get_fecha_amd());
	l_ori=leer_fichero(nombre);
	remove(nombre);

	if(l_ori){
		cout << "Las actividades que tenías para hoy, han sido pospuestas a mañana: " << endl;
		l_ori->imprimir_texto(2);

		//i=45;	// Esta línea es la que debe activarse en la versión temporal
		i=34;	// Esta línea es la que debe activarse en la versión definitiva

		cadins(nombre,&i,destino->get_fecha_amd());
		l_des=leer_fichero(nombre);
		l_des->prolongar(l_ori);
		escribir_fichero(nombre,l_des);
	}
	else cout << "Para hoy no tenías nada pendiente" << endl;
}