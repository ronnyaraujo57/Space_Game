#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <list>
#include <time.h>
#include "MenuLib.h"
using namespace std;

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

//prototipos
void gotoxy(int x, int y);
void OcultarCursor();
void pintar_limite();
int n_random(int);

//CLASES
class NAVE{
	int x,y;
	int vidas;
public:
	NAVE(int _x, int _y, int _vidas){
		x= _x; y= _y; vidas= _vidas;
	}
	int X(){ return x; };
	int Y(){ return y; };
	int showLife(){ return vidas; };
	void efecto_golpe(){ vidas--; };
	void pintar();
	void borrar();
	void mover();
	void pintar_corazones();
	void pintar_corazones(int _vidas);
	void morir();
};

class BALA{
	int x,y;
public:
	BALA(int _x, int _y){
		x= _x; y=_y;
	}
	int X(){ return x; };
	int Y(){ return y; };
	void mover();
	void mover2();
	bool fuera();
	bool fuera2();
};

class ENEMI{
	int x,y;
	int vida;
public:
	ENEMI(int _x, int _y, int _vida){
		x= _x; y= _y; vida= _vida;
	};
	ENEMI(){
		x=0; y=0;
	};
	int X(){ return x; };
	int Y(){ return y; };
	void X(int _x){	x= _x; };
	void efecto_golpe(){ vida--; };
	void reboot(int _vida){ vida= _vida;};
	void pintar();
	void mover(int p_past);
	void borrar();
	bool aniquilado();
};

//funcion principal
int main(){
	//crear objetos
	int n=9;
	NAVE nave1(15,25,3);
	ENEMI enemi[n];
	enemi[0] = ENEMI(14,8,3);
	enemi[1] = ENEMI(20,10,3);
	enemi[2] = ENEMI(26,8,3);
	enemi[3] = ENEMI(32,10,3);
	enemi[4] = ENEMI(38,8,3);
	enemi[5] = ENEMI(44,10,3);
	enemi[6] = ENEMI(50,8,3);
	enemi[7] = ENEMI(56,10,3);
	enemi[8] = ENEMI(62,8,3);
	//inicializando elementos
	system("mode con cols=85 lines=31");
	system("title Space Game");
	OcultarCursor();
	pintar_limite();
	//crear menus
	string opciones1[] = {"Play","Exit"};
	MENU menu1(2);
	menu1.GetOpciones(opciones1);
	menu1.GetCoordenadas(38,14);
	menu1.GetSelect(false);
	string opciones2[] = {"Reset","Exit"};
	MENU menu2(2);
	menu2.GetOpciones(opciones2);
	menu2.GetCoordenadas(38,16);
	menu2.GetSelect(false);
	//variables
	bool game_over = false;
	int second = 0;
	int cont = 0;
	int cont1 = 0;
	bool m_left = true;
	bool m_right = false;
	int FireEnemi;
	int past=0;
	bool colision=false;
	int Op;
	Op = menu1.Start();
	if(Op==0){
		//pintando objetos
		for(int i=0; i<n; i++){
			enemi[i].pintar();
		}
		nave1.pintar();
		nave1.pintar_corazones();
		list<BALA*> B;
		list<BALA*> BE;
		list<BALA*>::iterator it;
		//bucle principal de juego
		while(!game_over){
			//algoritmo de disparo personaje
			if(kbhit()){
				char tecla = getch();
				if(tecla == 'd'){
					B.push_back(new BALA(nave1.X()+2,nave1.Y()-1));
				}
			}
			for(it =B.begin(); it != B.end(); it++){
				(*it)->mover();
				//colicion bala-enemigo
				for(int i=0; i<n; i++){
					if(!enemi[i].aniquilado() && ((*it)->X() >= enemi[i].X() && (*it)->X() < enemi[i].X()+4) &&  ((*it)->Y() >= enemi[i].Y() && (*it)->Y() <= enemi[i].Y()+2)){
						enemi[i].efecto_golpe();
						if(enemi[i].aniquilado()){
							enemi[i].borrar();
						}else{
							enemi[i].borrar();
							enemi[i].pintar();
						}
						delete(*it);
						it = B.erase(it);
						colision = true;
						break;		
					}	
				}	
				//bala-jugador llega al borde del cuadro		
				if(!colision && (*it)->fuera()){
					gotoxy((*it)->X(),(*it)->Y()); printf(" ");
					delete(*it);
					it = B.erase(it);
				}
				colision = false;
			}
			
			//algoritmo de disparo enemigo
			FireEnemi = n_random(n);
			if(!(past != FireEnemi ) && second==15 && !enemi[FireEnemi].aniquilado()){
				BE.push_back(new BALA(enemi[FireEnemi].X()+1,enemi[FireEnemi].Y()+3));
			}
			past = FireEnemi;
			for(it = BE.begin(); it!=BE.end(); it++ ){
				(*it)->mover2();
				//colision bala-jugador
				if(((*it)->X() >= nave1.X() && (*it)->X() < nave1.X()+5) && ((*it)->Y() >= nave1.Y() && (*it)->Y() < nave1.Y()+2)){
					nave1.efecto_golpe();
					nave1.borrar();
					nave1.pintar();
					nave1.pintar_corazones();
					delete(*it);
					it = BE.erase(it);
				}else{
					//bala-enemigo llega al borde del cuadro
					if((*it)->fuera2()){
						gotoxy((*it)->X(),(*it)->Y()); printf(" ");
						delete(*it);
						it = BE.erase(it);
					}
				}
			}
			//algoritmo de patron de movimiento enemigo
			second++;
			if(second==25 && m_left){
				for(int i=0; i<n; i++){
					if(!enemi[i].aniquilado()){
						enemi[i].borrar();
						enemi[i].X(enemi[i].X()+1);
						enemi[i].pintar();
					}
				}
				cont++;
				second = 1;
				if(cont==3){
					m_left = false;
					m_right = true;
					cont = 0; 
				}
			}else{
				if(second==25 && m_right){
					for(int i=0; i<n; i++){
						if(!enemi[i].aniquilado()){
							enemi[i].borrar();
							enemi[i].X(enemi[i].X()-1);
							enemi[i].pintar();
						}
					}
					cont++;
					second = 1;
					if(cont==3){
						m_left = true;
						m_right = false;
						cont = 0; 
					}
				}
			}
			nave1.morir();
			for(int i=0; i<n; i++){
				if(enemi[i].aniquilado()){
					cont1++;
				}
			}
			//ganaste
			if(cont1==n){
				for(int i=0; i<n; i++){
					enemi[i].borrar();
				}
				nave1.borrar();
				gotoxy(38,14); printf("Winner!");
				Op=menu2.Start();
				if(Op==0){
					gotoxy(38,14); printf("       ");
					main();
				}else{
					game_over=true;					
				}
			}else{
				cont1=0;
			}
			//perdiste game over
			if(nave1.showLife()==0){
				for(int i=0; i<n; i++){
					enemi[i].borrar();
				}
				nave1.borrar();
				gotoxy(38,14); printf("Game Over");
				Op=menu2.Start();
				if(Op==0){
					gotoxy(38,14); printf("         ");
					main();
				}else{
					game_over=true;
				}
			}
			nave1.mover();
			Sleep(30);
		}
	}
	return 0;
}

//cuerpos 
void pintar_limite(){
	gotoxy(2,2); printf("%c",201);
	gotoxy(82,2); printf("%c",187);
	gotoxy(2,28); printf("%c",200);
	gotoxy(82,28); printf("%c",188);
	for(int i=3; i<82;i++){
		gotoxy(i,2); printf("%c",205);
		gotoxy(i,28); printf("%c",205);
	}
	for(int i=3; i<28; i++){
		gotoxy(2,i); printf("%c",186);
		gotoxy(82,i); printf("%c",186);
	}
}

int n_random(int a){
	srand(time(NULL));
	return rand() % a;
}

//CUERPOS DE METODOS DE LA CLASE NAVE
void NAVE::pintar(){
	gotoxy(x,y); printf("  %c",202);
	gotoxy(x,y+1); printf(" %c%c%c",40,207,41);
	gotoxy(x,y+2); printf("%c%c %c%c",203,190,190,203);
}

void NAVE::borrar(){
	gotoxy(x,y);   printf("        ");
	gotoxy(x,y+1); printf("        ");
	gotoxy(x,y+2); printf("        ");
}

void NAVE::mover(){
	if(kbhit()){
		char tecla = getch();
		borrar();
		if(tecla == DERECHA && x+5 < 82)	x++;
		if(tecla == IZQUIERDA && x > 3)		x--;
		if(tecla == ARRIBA && y-1 > 2)		y--; 	
		if(tecla == ABAJO && y+2 < 27)		y++;
		pintar();
		pintar_corazones();
	}
}

void NAVE::pintar_corazones(){
	gotoxy(73,1); printf("Vidas: %d",vidas);	
}

void NAVE::pintar_corazones(int _vidas){
	vidas = _vidas;
	gotoxy(73,1); printf("Vidas: %d",vidas);
}

void NAVE::morir(){
	if(vidas==0){
		borrar();
		gotoxy(x,y);   printf("   **   ");
		gotoxy(x,y+1); printf("  ****  ");
		gotoxy(x,y+2); printf("   **   ");
		Sleep(200);
		
		borrar();
		gotoxy(x,y);   printf("  * ** *  ");
		gotoxy(x,y+1); printf("   ****  ");
		gotoxy(x,y+2); printf("  * ** *  ");
		Sleep(200);
		borrar();
		pintar_corazones();
		pintar();		
	}
}

//CUERPOS DE METODOS DE LA CLASE BALA
void BALA::mover(){
	gotoxy(x,y); printf(" ");
	y--;
	gotoxy(x,y); printf("*");
}

void BALA::mover2(){
	gotoxy(x,y); printf(" ");
	y++;
	gotoxy(x,y); printf("%c",158);
}

bool BALA::fuera(){
	if(y==3){
		return true;
	}else{
		return false;
	}
}

bool BALA::fuera2(){
	if(y==27){
		return true;
	}else
		return false;
	
}
//CUERPOS DE METODOS DE LA CLASE ENEMI
void ENEMI::pintar(){
	gotoxy(x,y);   printf("%c%c%c",95,176,95);
	gotoxy(x,y+1); printf("%c%c%c",186,219,186);
	gotoxy(x,y+2); printf("%c%c%c",200,203,188);
}

void ENEMI::mover(int p_past){
	borrar();
	if(p_past>=x && p_past<x+3){
		x++;
	}else{
		if(p_past<x && p_past>x-3){
			x--;
		}
	}
	pintar();

}

void ENEMI::borrar(){
	gotoxy(x,y);   printf("    ");
	gotoxy(x,y+1); printf("    ");
	gotoxy(x,y+2); printf("    ");
}

bool ENEMI::aniquilado(){
	if(vida==0)
		return true;
	else
		return false;	
}
