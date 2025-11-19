#include "Interfaz.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <conio.h>    
#include <random>       


char Tablero[10][10];

static inline bool Dentro(int r, int c) { return r >= 0 && r < 10 && c >= 0 && c < 10; }


static std::mt19937& rng() {
    static std::mt19937 gen((unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count());
    return gen;
}


void LimpiarBufferTeclado() {
    while (_kbhit()) {
        _getch();
    }
}


void MostrarMenuPrincipal() {
    std::cout << "===================================\n";
    std::cout << "      DAMAS INTERNACIONALES 10x10  \n";
    std::cout << "===================================\n";
    std::cout << "1. Jugador vs Jugador\n";
    std::cout << "2. Jugador vs IA \n";
    std::cout << "3. Ver reglas\n";
    std::cout << "4. Controles\n";
    std::cout << "5. Salir\n";
    std::cout << "Seleccione una opcion: ";
}

void MostrarReglas() {
    std::cout << "\n========= REGLAS =========\n";
    std::cout << "- Capturas obligatorias.\n";
    std::cout << "- Capturas multiples.\n";
    std::cout << "- Coronacion.\n";
    std::cout << "- Dama mueve en diagonales largas.\n";
}

void MostrarControles() {
    std::cout << "\n========= CONTROLES =========\n";
    std::cout << "Formato: A5, B3, J10\n";
    std::cout << "45 segundos por turno (jugador humano).\n";
    std::cout << "Si se acaba el tiempo -> turno perdido.\n";
}


void InicializarJuego(Jugador& j1, Jugador& j2, int& turno) {
    j1.man = 'x';  j1.king = 'X';  j1.pieces = 20;  j1.score = 0;
    j2.man = 'o';  j2.king = 'O';  j2.pieces = 20;  j2.score = 0;

    turno = 1;

    for (int r = 0; r < 10; r++)
        for (int c = 0; c < 10; c++)
            Tablero[r][c] = ((r + c) % 2 == 1 ? '.' : ' ');

    int placed = 0;
    for (int r = 0; r <= 3; r++)
        for (int c = 0; c < 10; c++)
            if (Tablero[r][c] == '.' && placed < 20)
                Tablero[r][c] = j2.man, placed++;

    placed = 0;
    for (int r = 9; r >= 6; r--)
        for (int c = 0; c < 10; c++)
            if (Tablero[r][c] == '.' && placed < 20)
                Tablero[r][c] = j1.man, placed++;
}


void MostrarTablero() {
    std::cout << "\n   ";
    for (char ch = 'A'; ch <= 'J'; ch++) std::cout << ch << ' ';
    std::cout << "\n";

    for (int r = 0; r < 10; r++) {
        if (r < 9) std::cout << " ";
        std::cout << r + 1 << " ";
        for (int c = 0; c < 10; c++) {
            std::cout << Tablero[r][c] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}


int CoordLetraAIndice(char letra) {
    letra = toupper(letra);
    if (letra < 'A' || letra > 'J') return -1;
    return letra - 'A';
}
int CoordNumeroAIndice(int num) {
    if (num < 1 || num > 10) return -1;
    return num - 1;
}


bool PuedeCapturarDesde(int r, int c, char man, char king) {
    if (!Dentro(r, c)) return false;
    char p = Tablero[r][c];
    if (p != man && p != king) return false;
    const int dr[4] = { 1,1,-1,-1 };
    const int dc[4] = { 1,-1,1,-1 };
    for (int d = 0; d < 4; d++) {
        int mr = r + dr[d], mc = c + dc[d];
        int lr = r + 2 * dr[d], lc = c + 2 * dc[d];
        if (Dentro(mr, mc) && Dentro(lr, lc)) {
            char mid = Tablero[mr][mc];
            if (mid != '.' && mid != ' ' && mid != man && mid != king &&
                Tablero[lr][lc] == '.') return true;
        }
    }
    return false;
}
bool ExisteCapturaParaJugador(Jugador& J) {
    for (int r = 0; r < 10; r++)
        for (int c = 0; c < 10; c++)
            if (Tablero[r][c] == J.man || Tablero[r][c] == J.king)
                if (PuedeCapturarDesde(r, c, J.man, J.king)) return true;
    return false;
}


std::vector<std::pair<int, int>> MovimientosValidosPara(int r, int c, char man, char king, bool onlyCaptures) {
    std::vector<std::pair<int, int>> v;
    char p = Tablero[r][c];
    if (p != man && p != king) return v;
    int forward = (man == 'x') ? -1 : 1;
    const int dr[4] = { 1,1,-1,-1 };
    const int dc[4] = { 1,-1,1,-1 };
    if (!onlyCaptures) {
        if (p == man) {
            for (int k = 0; k < 2; k++) {
                int rr = r + forward, cc = c + (k == 0 ? -1 : 1);
                if (Dentro(rr, cc) && Tablero[rr][cc] == '.')
                    v.push_back({ rr,cc });
            }
        }
        if (p == king) {
            for (int d = 0; d < 4; d++) {
                int rr = r + dr[d], cc = c + dc[d];
                while (Dentro(rr, cc) && Tablero[rr][cc] == '.') {
                    v.push_back({ rr,cc });
                    rr += dr[d]; cc += dc[d];
                }
            }
        }
    }
    for (int d = 0; d < 4; d++) {
        int mr = r + dr[d], mc = c + dc[d];
        int lr = r + 2 * dr[d], lc = c + 2 * dc[d];
        if (Dentro(mr, mc) && Dentro(lr, lc)) {
            char mid = Tablero[mr][mc];
            if (mid != '.' && mid != ' ' && mid != man && mid != king &&
                Tablero[lr][lc] == '.') v.push_back({ lr,lc });
        }
    }
    return v;
}


bool AplicarMovimientoYCapturas(Jugador& J, Jugador& Opp, int sr, int sc, int dr, int dc) {
    char piece = Tablero[sr][sc];
    int distR = dr - sr, distC = dc - sc;
  
    if (abs(distR) == 1 && abs(distC) == 1) {
        Tablero[dr][dc] = piece;
        Tablero[sr][sc] = '.';
        CoronacionSiCorresponde(J, dr, dc);
        return true;
    }

    if (abs(distR) == 2 && abs(distC) == 2) {
        int mr = (sr + dr) / 2, mc = (sc + dc) / 2;
        if (Tablero[mr][mc] == Opp.man || Tablero[mr][mc] == Opp.king) {
            Tablero[mr][mc] = '.';
            Opp.pieces--;
            J.score += 1;
        }
        Tablero[dr][dc] = piece;
        Tablero[sr][sc] = '.';
        CoronacionSiCorresponde(J, dr, dc);
        return true;
    }
    return false;
}


void CoronacionSiCorresponde(Jugador& J, int r, int c) {
    if (Tablero[r][c] == J.man) {
        if (J.man == 'x' && r == 0) Tablero[r][c] = J.king, J.score += 5;
        if (J.man == 'o' && r == 9) Tablero[r][c] = J.king, J.score += 5;
    }
}


bool VerificarTiempoTurno(int limite) {
    using namespace std::chrono;
    auto start = steady_clock::now();
    while (true) {
        auto now = steady_clock::now();
        long segundos = duration_cast<seconds>(now - start).count();
        if (segundos >= limite) {
            std::cout << "\nTiempo agotado. Turno perdido.\n";
            LimpiarBufferTeclado();
            return false;
        }
        if (_kbhit()) {
            LimpiarBufferTeclado();
            return true;
        }
    }
}


bool EscogerFicha(Jugador& J, int& r, int& c, bool forced) {
    while (true) {
        std::cout << J.nombre << " seleccione ficha (ej: B3): ";
        if (!VerificarTiempoTurno(45)) return false;
        std::string s;
        std::getline(std::cin, s);
        if (s.size() < 2) continue;
        int col = CoordLetraAIndice(s[0]);
        int row = atoi(s.substr(1).c_str()) - 1;
        if (!Dentro(row, col)) continue;
        if (Tablero[row][col] != J.man && Tablero[row][col] != J.king) {
            std::cout << "No es tu ficha.\n"; continue;
        }
        if (forced && !PuedeCapturarDesde(row, col, J.man, J.king)) {
            std::cout << "Debes elegir una ficha que capture.\n"; continue;
        }
        r = row; c = col; return true;
    }
}


bool SeleccionarYMover(Jugador& J, Jugador& Opp, int turno) {
    bool forced = ExisteCapturaParaJugador(J);
    int sr, sc;
    if (!EscogerFicha(J, sr, sc, forced)) return false;
    auto movs = MovimientosValidosPara(sr, sc, J.man, J.king, forced);
    if (movs.empty()) return false;
    std::cout << "Movimientos: ";
    for (auto& m : movs) std::cout << (char)('A' + m.second) << m.first + 1 << " ";
    std::cout << "\nDestino: ";
    if (!VerificarTiempoTurno(45)) return false;
    std::string d;
    std::getline(std::cin, d);
    if (d.size() < 2) return false;
    int dc = CoordLetraAIndice(d[0]);
    int dr = atoi(d.substr(1).c_str()) - 1;
    bool valido = false;
    for (auto& m : movs) if (m.first == dr && m.second == dc) valido = true;
    if (!valido) return false;
    AplicarMovimientoYCapturas(J, Opp, sr, sc, dr, dc);
    while (PuedeCapturarDesde(dr, dc, J.man, J.king)) {
        MostrarTablero();
        auto next = MovimientosValidosPara(dr, dc, J.man, J.king, true);
        if (next.empty()) break;
        std::cout << "Continuar captura desde " << (char)('A' + dc) << dr + 1 << ": ";
        if (!VerificarTiempoTurno(45)) return false;
        std::string s;
        std::getline(std::cin, s);
        int ndc = CoordLetraAIndice(s[0]);
        int ndr = atoi(s.substr(1).c_str()) - 1;
        bool ok = false;
        for (auto& m : next) if (m.first == ndr && m.second == ndc) ok = true;
        if (!ok) return false;
        AplicarMovimientoYCapturas(J, Opp, dr, dc, ndr, ndc);
        dr = ndr; dc = ndc;
    }
    return true;
}


void MoverIA(Jugador& IA, Jugador& Hum, int& turno) {
    
    struct Move { int sr, sc, dr, dc; };
    std::vector<Move> captures;
    std::vector<Move> simples;

    for (int r = 0; r < 10; ++r) {
        for (int c = 0; c < 10; ++c) {
            if (Tablero[r][c] != IA.man && Tablero[r][c] != IA.king) continue;
       
            auto caps = MovimientosValidosPara(r, c, IA.man, IA.king, true);
            for (auto& p : caps) captures.push_back({ r, c, p.first, p.second });
            if (captures.empty()) {
                auto mv = MovimientosValidosPara(r, c, IA.man, IA.king, false);
                for (auto& p : mv) simples.push_back({ r, c, p.first, p.second });
            }
        }
    }

    
    if (!captures.empty()) {
        std::uniform_int_distribution<int> dist(0, (int)captures.size() - 1);
        Move m = captures[dist(rng())];
        AplicarMovimientoYCapturas(IA, Hum, m.sr, m.sc, m.dr, m.dc);
       
        int curR = m.dr, curC = m.dc;
        while (PuedeCapturarDesde(curR, curC, IA.man, IA.king)) {
            auto next = MovimientosValidosPara(curR, curC, IA.man, IA.king, true);
            if (next.empty()) break;
            std::uniform_int_distribution<int> dist2(0, (int)next.size() - 1);
            auto p = next[dist2(rng())];
            AplicarMovimientoYCapturas(IA, Hum, curR, curC, p.first, p.second);
            curR = p.first; curC = p.second;
        }
    }
    else if (!simples.empty()) {
        std::uniform_int_distribution<int> dist(0, (int)simples.size() - 1);
        Move m = simples[dist(rng())];
        AplicarMovimientoYCapturas(IA, Hum, m.sr, m.sc, m.dr, m.dc);
        
    }
    
}


bool TieneMovimientos(Jugador& J) {
    for (int r = 0; r < 10; r++)
        for (int c = 0; c < 10; c++)
            if (Tablero[r][c] == J.man || Tablero[r][c] == J.king)
                if (!MovimientosValidosPara(r, c, J.man, J.king, false).empty())
                    return true;
    return false;
}
bool EsGanador(Jugador& J, Jugador& Opp) {
    if (Opp.pieces <= 0) { J.score += 20; return true; }
    if (!TieneMovimientos(Opp)) { J.score += 20; return true; }
    return false;
}
