#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define POB 20
#define TAM 10
#define UMBRAL 0.75

using namespace std;

int distancias[TAM][TAM];
vector<int> ciudades, soluciones, fitness;
vector<int>* poblacion[POB];
vector<int>* temp[POB];

ifstream matriz("ciudades.txt");
ofstream salida("rutas.txt");
using namespace std;

int aptitud(vector<int>* v) {
	int suma = 0;
	for (int i = 1; i < v->size(); ++i)
		suma += distancias[v->at(i)][v->at(i - 1)];
	suma += distancias[v->at(v->size() - 1)][v->at(0)];
	return suma;
}

int mejor() {
	int m = 1000000;
	int mejor = 0;
	for (int i = 0; i < POB; i++)
		if (m > fitness[i]) {
			m = fitness[i];
			mejor = i;
		}
	return mejor;
}

float promedio() {
	float prom=0;
	for (int i = 0; i < POB; i++)
			prom+=fitness[i];
	return prom/(float)POB;
}

void verRuta(vector<int>* v) {
	for (int i = 0; i < v->size(); ++i)
		cout << v->at(i) << " ";
}

void cruce(vector<int>* padre1, vector<int>* padre2, vector<int>* hijo) {
	int j;
	/*cout << "Padre1 ";
	for (int k = 0; k < padre1->size(); ++k)
		cout << padre1->at(k) << " ";
	cout <<endl;
	cout << "Padre2 ";
	for (int k = 0; k < padre2->size(); ++k)
		cout << padre2->at(k) << " ";
	cout << endl;*/
	for (int i = 0; i < padre1->size()/2; ++i) {
		j = i;
		while (j < 10 && padre2->at(i) != padre1->at(j))
			j++;
		if (j < 10) {
			if (hijo->at(i) < 0)
				hijo->at(j) = padre1->at(i);
			else
				hijo->at(j) = hijo->at(i);
			hijo->at(i) = padre2->at(i);
		}
		else 
			if (hijo->at(i) < 0)
				hijo->at(i) = padre1->at(i);
	}
	for (int i = padre1->size()/2; i < padre1->size(); ++i) {
		if (hijo->at(i) < 0)
			hijo->at(i) = padre1->at(i);
	}
	/*cout << "Hijo   ";
	for (int k = 0; k < hijo->size(); ++k)
		cout << hijo->at(k) << " ";
	cout << endl;*/
}

void mutacion(vector<int>* v) {
	int cambiar;
	random_shuffle(ciudades.begin(), ciudades.end());
	cambiar = v->at(ciudades[0]);
	v->at(ciudades[0]) = v->at(ciudades[1]);
	v->at(ciudades[1]) = cambiar;
}

int main() {
	srand(time(NULL));
	for (int i = 0; i < TAM; ++i)
		ciudades.push_back(i);
	for (int i = 0; i < POB; ++i)
		soluciones.push_back(i);
	for (int i = 0; i < POB; ++i) {
		vector<int>* vp = new vector<int>;
		random_shuffle(ciudades.begin(), ciudades.end());//poblacion inicial
		for (int j = 0; j < TAM; ++j)
			vp->push_back(ciudades[j]);
		poblacion[i] = vp;
	}
	if (matriz.is_open())
	{
		for (int i = 0; i < TAM; ++i)
			for (int j = 0; j < TAM; ++j)
				matriz >> distancias[i][j];//cargar distancia
		matriz.close();
	}
	int a, b, m;
	float torneo;
	for (int ciclo = 0; ciclo < 50; ++ciclo) {
		/*for (int i = 0; i < POB; ++i) {
			verRuta(poblacion[i]);
			cout << endl;
		}*/
		fitness.clear();
		for (int i = 0; i < POB; ++i)
			fitness.push_back(aptitud(poblacion[i]));
		m = mejor();
		cout << "Mejor ruta: ";
		verRuta(poblacion[m]);
		cout << " - Longitud: " << fitness[m];
		cout <<" Promedio: "<<promedio()<<endl;
		if (salida.is_open())
			salida << fitness[m] << "\t" << promedio() << endl;
		for (int i = 0; i < 17; ++i) {
			random_shuffle(soluciones.begin(), soluciones.end());
			torneo = ((double)rand() / (RAND_MAX));
			if (torneo < UMBRAL) {
				fitness[soluciones[0]] < fitness[soluciones[1]] ? a = 0 : a = 1;//torneo probabilistico
				fitness[soluciones[2]] < fitness[soluciones[3]] ? b = 2 : b = 3;
			}
			else {
				fitness[soluciones[0]] > fitness[soluciones[1]] ? a = 0 : a = 1;
				fitness[soluciones[2]] > fitness[soluciones[3]] ? b = 2 : b = 3;
			}
			//cout << soluciones[a] << "-" << soluciones[b]<<": ";
			temp[i] = new vector<int>;
			for (int k = 0; k < TAM; ++k)
				temp[i]->push_back(-1);
			cruce(poblacion[soluciones[a]], poblacion[soluciones[b]], temp[i]);//cruce
			//verRuta(temp[i]);
			//cout << endl;
		}
		temp[17] = new vector<int>;
		for (int i = 0; i < TAM; ++i)
			temp[17]->push_back(poblacion[m]->at(i));//elitismo
		random_shuffle(soluciones.begin(), soluciones.end());
		temp[18] = poblacion[soluciones[0]];
		temp[19] = poblacion[soluciones[1]];
		mutacion(temp[18]);
		mutacion(temp[19]);
		for (int i = 0; i < POB; ++i)
			poblacion[i] = temp[i];//siguiente generación
	}
	salida.close();
	return 0;
}