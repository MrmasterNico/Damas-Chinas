#include "Interfaz.h"
#include <iostream>
#include <string>

int main() {

    // OPCIONAL: si aun quieres UTF-8, activa consola:
    // system("chcp 65001 > nul");

    while (true) {

        MostrarMenuPrincipal();

        std::string op;
        std::getline(std::cin, op);

        if (op == "1") {

            Jugador J1, J2;
            int turno = 1;

            std::cout << "Nombre jugador 1 (x): ";
            std::getline(std::cin, J1.nombre);
            if (J1.nombre.empty()) J1.nombre = "Jugador 1";

            std::cout << "Nombre jugador 2 (o): ";
            std::getline(std::cin, J2.nombre);
            if (J2.nombre.empty()) J2.nombre = "Jugador 2";

            InicializarJuego(J1, J2, turno);
            MostrarControles();

            while (true) {
                MostrarTablero();

                std::cout << "Puntaje -> "
                    << J1.nombre << ": " << J1.score
                    << " | " << J2.nombre << ": " << J2.score << "\n";

                Jugador* cur = (turno == 1 ? &J1 : &J2);
                Jugador* opp = (turno == 1 ? &J2 : &J1);

                std::cout << "Turno: " << cur->nombre << " (" << cur->man << ")\n";

                if (!SeleccionarYMover(*cur, *opp, turno)) {
                    std::cout << "Turno perdido.\n";
                    LimpiarBufferTeclado();
                }

                if (EsGanador(*cur, *opp)) {
                    MostrarTablero();
                    std::cout << "Ganador: " << cur->nombre << "\n";

                    std::cout << "\nPUNTAJES FINALES:\n";
                    std::cout << J1.nombre << ": " << J1.score << "\n";
                    std::cout << J2.nombre << ": " << J2.score << "\n";
                    break;
                }

                turno = (turno == 1 ? 2 : 1);
            }

            std::cout << "\nPresione ENTER para volver al menu...";
            std::string tmp;
            std::getline(std::cin, tmp);
        }

        else if (op == "2") {

            Jugador Hum, IA;
            int turno = 1;

            std::cout << "Nombre (humano) (x): ";
            std::getline(std::cin, Hum.nombre);
            if (Hum.nombre.empty()) Hum.nombre = "Humano";

            IA.nombre = "Computadora";

            Hum.man = 'x'; Hum.king = 'X'; Hum.pieces = 20; Hum.score = 0;
            IA.man = 'o'; IA.king = 'O'; IA.pieces = 20; IA.score = 0;

            InicializarJuego(Hum, IA, turno);
            MostrarControles();

            while (true) {
                MostrarTablero();
                std::cout << "Puntaje -> " << Hum.nombre << ": " << Hum.score
                    << " | " << IA.nombre << ": " << IA.score << "\n";

                if (turno == 1) {
                    std::cout << "Turno: " << Hum.nombre << " (" << Hum.man << ")\n";
                    if (!SeleccionarYMover(Hum, IA, turno)) {
                        std::cout << "Turno perdido.\n";
                        LimpiarBufferTeclado();
                    }
                    if (EsGanador(Hum, IA)) {
                        MostrarTablero();
                        std::cout << "Ganador: " << Hum.nombre << "\n";
                        break;
                    }
                    turno = 2;
                }
                else {
                    std::cout << "Turno: " << IA.nombre << " (" << IA.man << ")\n";
                    MoverIA(IA, Hum, turno);

                    if (EsGanador(IA, Hum)) {
                        MostrarTablero();
                        std::cout << "Ganador: " << IA.nombre << "\n";
                        break;
                    }
                    turno = 1;
                }
            }

            std::cout << "\nPresione ENTER para volver al menu...";
            std::string tmp;
            std::getline(std::cin, tmp);
        }

        else if (op == "3") MostrarReglas();
        else if (op == "4") MostrarControles();
        else if (op == "5") { std::cout << "Saliendo...\n"; return 0; }
        else std::cout << "Opcion invalida.\n";
    }

    return 0;
}
