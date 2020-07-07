#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define PRUEBAS 21
#define TASAENT 0.6
#define EPOCAS 10

using namespace std;

ifstream ds("entrenamiento.txt");
int ts[PRUEBAS][32];
double pesos[3][33];//bias
int esperados[3][3] = { {1,-1,-1},{-1,1,-1},{-1,-1,1} };
char valores[3] = { '0','1','2' };
char resultados[PRUEBAS];

int calcular(int pos,int pes) {
	double resultado = 0;
	for (int i = 0; i < 32; ++i)
		resultado += ts[pos][i] * pesos[pes][i];
	resultado += pesos[pes][32];
	return (resultado>0)?1:-1;
}

int main() {
	srand(time(NULL));
	if (ds.is_open()) {
		for (int i = 0; i < PRUEBAS; ++i)
			for (int j = 0; j < 32; ++j)
				ds >> ts[i][j];//lee el conjunto de entrenamiento
		ds.close();
	}
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 33; j++)
			pesos[i][j] = (double)rand() / (RAND_MAX);//pesos[i][j]= (double)rand() / (RAND_MAX);pesos[i][j] = 0;
	int neuronas[3] = { 0,0,0 };
	for (int i = 0; i < PRUEBAS; ++i)
		resultados[i] = '?';
	bool aun;
	for (int e = 0; e < EPOCAS;++e) {
		for (int i = 0; i < PRUEBAS; ++i) {
			cout << "Entrada " << i << ": ";
			for (int j = 0; j < 3; ++j) {
				neuronas[j] = calcular(i, j);//activación
				cout << neuronas[j] << " ";
			}
			cout << endl;
			for (int m = 0; m < 3; ++m) {
				aun = 1;
				for (int n = 0; n < 3; ++n)
					if (neuronas[n] != esperados[m][n])
						aun &= 0;
				if (!aun) {
					for (int j = 0; j < 3; ++j){
						for (int k = 0; k < 32; ++k)
							pesos[j][k] += TASAENT * esperados[i % 3][j] * ts[i][k];//ajuste de pesos
						pesos[j][32] += TASAENT * esperados[i % 3][j];//ajuste bias
					}
				}
				else
					resultados[i] = valores[m];
			}
		}
	}
	for (int i = 0; i < PRUEBAS; ++i)
		cout << "Entrada " << i << ": " << resultados[i]<<endl;
	return 0;
}

