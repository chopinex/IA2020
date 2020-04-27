#include <iostream>
#include <sstream>
#include <set>
#include <algorithm>

using namespace std;

char tablero[3][3] = { {' ',' ',' '},{' ',' ',' '},{' ',' ',' '} };
int ganadores[8][3] = { {0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6} };
set<int> posibles;
int maxnivel;

int ganador(char player) {
    int i = 0;
    while (i < 8) {
        if (tablero[ganadores[i][0] / 3][ganadores[i][0] % 3] != ' ' && tablero[ganadores[i][0] / 3][ganadores[i][0] % 3] == tablero[ganadores[i][1] / 3][ganadores[i][1] % 3] && tablero[ganadores[i][1] / 3][ganadores[i][1] % 3] == tablero[ganadores[i][2] / 3][ganadores[i][2] % 3]) {
            if (tablero[ganadores[i][0] / 3][ganadores[i][0] % 3] == player)
                return 1;
            else
                return -1;
        }
        i++;
    }
    return 0;
}

bool lleno() {
    int ocupados = 0;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (tablero[i][j] != ' ')
                ocupados++;
    return ocupados == 9 ? 1 : 0;
}

bool terminado() {
    int gano;
    gano = ganador('X');
    if (gano == 1) {
        cout << "Ganaste!" << endl;
        return 1;
    }
    if (gano == -1) {
        cout << "Te gane!" << endl;
        return 1;
    }
    if (lleno()) {
        cout << "Empate" << endl;
        return 1;
    }
    return 0;
}

void humano() {
    stringstream ss;
    int opcion = -1;
    set<int>::iterator it;
    ss.clear();
    for (it = posibles.begin(); it != posibles.end(); ++it) {
        ss << *it << ",";
    }
    do {
        cout << "Escoja una de las opciones: (" << ss.str() << " ): ";
        cin >> opcion;
        it = posibles.find(opcion);
    } while (it == posibles.end());
    tablero[opcion / 3][opcion % 3] = 'X';
    posibles.erase(it);
}

int minimax(int nivel, bool isMax) {
    if (nivel > maxnivel)
        return 0;
    int salida = ganador('O');
    int mejor;
    if (salida == 1 || salida == -1)
        return salida;
    if (lleno())
        return 0;
    if (isMax) {
        mejor = -10;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (tablero[i][j] == ' ') {
                    tablero[i][j] = 'O';
                    mejor = max(mejor, minimax(nivel + 1, !isMax));
                    tablero[i][j] = ' ';
                }
            }
        }
        return mejor;
    }
    else {
        mejor = 10;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (tablero[i][j] == ' ') {
                    tablero[i][j] = 'X';
                    mejor = min(mejor, minimax(nivel + 1, !isMax));
                    tablero[i][j] = ' ';
                }
            }
        }
        return mejor;
    }
}

void maquina() {
    int x, y, act, mejor = -20;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (tablero[i][j] == ' ') {
                tablero[i][j] = 'O';
                act = minimax(0, 0);
                tablero[i][j] = ' ';
                if (act > mejor) {
                    mejor = act;
                    x = i;
                    y = j;
                }
            }
        }
    }
    tablero[x][y] = 'O';
    posibles.erase(x * 3 + y);
}

void impTablero() {
    for (int i = 0; i < 3; ++i) {
        cout << "          -------\n          |";
        for (int j = 0; j < 3; ++j)
            cout << tablero[i][j] << "|";
        cout << endl;
    }
    cout << "          -------\n";
}

int main() {
    for (int i = 0; i < 9; ++i)
        posibles.insert(i);
    bool quien, fin = false;
    cout << "Quien inicia?( 0: jugador | 1: maquina): ";
    cin >> quien;
    cout << "Maximo nivel de busqueda?(1 a 8): ";
    cin >> maxnivel;
    while (!fin) {
        if (quien) {
            maquina();
            impTablero();
            fin = terminado();
        }
        else {
            humano();
            impTablero();
            fin = terminado();
        }
        quien = !quien;
    }
    return 0;
}
