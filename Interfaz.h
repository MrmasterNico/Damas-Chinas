#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <iostream>
#include <string>
#include <vector>

extern char Tablero[10][10];

struct Jugador {
    char man;     
    char king;    
    int pieces;   
    int score;   
    std::string nombre;
};


void MostrarMenuPrincipal();
void MostrarReglas();
void MostrarControles();


void InicializarJuego(Jugador& j1, Jugador& j2, int& turno);
void MostrarTablero();
bool SeleccionarYMover(Jugador& J, Jugador& Opp, int turno);
bool EsGanador(Jugador& J, Jugador& Opp);


int CoordLetraAIndice(char letra);
int CoordNumeroAIndice(int num);


bool PuedeCapturarDesde(int r, int c, char myMan, char myKing);
bool ExisteCapturaParaJugador(Jugador& J);
std::vector<std::pair<int, int>> MovimientosValidosPara(int r, int c, char myMan, char myKing, bool onlyCaptures);
bool AplicarMovimientoYCapturas(Jugador& J, Jugador& Opp, int sr, int sc, int dr, int dc);
void CoronacionSiCorresponde(Jugador& J, int r, int c);
bool TieneMovimientos(Jugador& J);

bool EscogerFicha(Jugador& J, int& r, int& c, bool forced);


bool VerificarTiempoTurno(int segundos_limite);
void LimpiarBufferTeclado();


void MoverIA(Jugador& IA, Jugador& Hum, int& turno);

#endif
