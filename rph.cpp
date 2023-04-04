#include "kalendas.cpp"

int main(int argc, char *argv[]){
	char *ruta, *tarea;
	int i, j, *v;
	linea *txt;
	dia *hoy;

	hoy=crear_dia(fecha());
	ruta=obtener_ruta(fecha());
	txt=leer_fichero(ruta);

	if(argc>=2){
		v=ordenar_argv(argc, argv);

		if(txt) {
			for(i=1; i<argc; i++) if(v[i-1]){
			// No está previsto y bien organizado que el cenutrio usuario pida eliminar la actividad 0
				tarea=extraer_tarea(hoy,v[i-1]-i+1);
				if(tarea) cout << "Se ha eliminado la tarea: \" " << tarea << " \"" << endl;
				else cout << "No hay una tarea " << v[i-1] <<  " para hoy" << endl;
			}
		}
		else cout << "Para hoy no tenías nada pendiente" << endl;
	}
	else{
		if(txt){
			cout << "Para hoy tenías pendiente: " << endl;
			txt->imprimir_texto(2);
			remove(ruta);
		}
		else cout << "Para hoy no tenías nada pendiente" << endl;
	}

	// Esta función podría hacerse de manera que pueda aceptar fechas como parámetros
	// Estructurar antes el vitrubio

	return 0;
}