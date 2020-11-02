#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

#define ARRIBA 72
#define ABAJO 80
#define ENTER 13

struct OPTIONS{
	int cant;
	string *op;	
	int *X;
	int *Y;
	bool select;
};

class MENU{
	OPTIONS opciones;
public:
	MENU(int _cant){
		opciones.cant = _cant;
	};
	void GetOpciones(string a[]);
	void GetCoordenadas(int _X, int _Y);
	void GetSelect(bool _p){ opciones.select = _p; };
	int *Set_X(){ return opciones.X; };
	int *Set_Y(){ return opciones.Y; };
	string *Set_Op(){ return opciones.op; };
	int Set_cant(){ return opciones.cant; };
	bool Select(){ return opciones.select; };
	int Start();	
};

class PUNTERO{
	int x,y;
	int posicion;
public:	
	PUNTERO(int _x, int _y, int _pos){
		x=_x; y=_y;	posicion=_pos;
	};
	void pintar();
	void borrar();
	int showPos(){ return posicion;	};
	OPTIONS mover(OPTIONS a);	
};

void gotoxy(int x, int y){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;	
	SetConsoleCursorPosition(hCon,dwPos);
}

void OcultarCursor(){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 50;
	cci.bVisible = FALSE;	
	SetConsoleCursorInfo(hCon,&cci);
}

//CUERPOS DE LA CLASE PUNTERO
void PUNTERO::pintar(){
	gotoxy(x,y); cout<<"->";
}

void PUNTERO::borrar(){
	gotoxy(x,y); cout<<"  ";	
}

OPTIONS PUNTERO::mover(OPTIONS a){
	if(kbhit()){
		char tecla= getch();
		borrar();
		if(tecla == ARRIBA){			
			if(posicion == 0){
				y = a.Y[a.cant-1];
				posicion = a.cant-1;
			}else{
				if(posicion > 0 && posicion <= a.cant-1){
					posicion--;
					y = a.Y[posicion];
				}
			}
		}	
		if(tecla == ABAJO){			
			if(posicion == a.cant-1){
				y = a.Y[0];
				posicion = 0;
			}else{
				if(posicion >= 0 && posicion < a.cant-1){
					posicion++;
					y = a.Y[posicion];
				}
			}
		}
		if(tecla == ENTER){
			a.select=true;
		}
		pintar();
	}
	return a;
}
//CUERPOS DE LA CLASE MENU
void MENU::GetOpciones(string a[]){
	opciones.op = new string[opciones.cant];
	for(int i=0; i<opciones.cant; i++){
		opciones.op[i] = a[i];
	}
}

void MENU::GetCoordenadas(int _X, int _Y){
	opciones.X = new int[opciones.cant];
	opciones.Y = new int[opciones.cant];
	for(int i=0; i<opciones.cant; i++){
		opciones.X[i] = _X;
		opciones.Y[i] = _Y + i;
	}	
}

int MENU::Start(){
	PUNTERO cursor(opciones.X[0]-3,opciones.Y[0],0);
	cursor.pintar();
	while(!opciones.select==true){
		for(int i=0; i<opciones.cant; i++){
			gotoxy(opciones.X[i],opciones.Y[i]); cout<<opciones.op[i];
		}
		opciones = cursor.mover(opciones);	
	}
	cursor.borrar();
	for(int i=0; i<opciones.cant; i++){
		gotoxy(opciones.X[i],opciones.Y[i]); cout<<"               ";
	}
	return cursor.showPos();
}

