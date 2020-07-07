#include <iostream>
#include <algorithm>
#include <utility>
#include <stack>
#include <vector>
#include <ctime>

const int TAM = 20;
const int TAM2 = TAM * TAM;
const int TAM20 = TAM2 / 5;
int grafo[TAM][TAM];
int poss[8][2] = { {1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1} };

struct elem {
    int posix;
    int f;
    int g;
    elem(int p, int ff, int gg) : posix(p), f(ff), g(gg) {}
    bool operator< (const elem& e) {
        if (f < e.f)
            return 1;
        else {
            if (f == e.f)
                return (g > e.g);
        }
        return 0;
    }
};

using namespace std;

bool inVector(vector<elem*>& v, int i) {
    vector<elem*>::iterator it;
    it = v.begin();
    while (it != v.end()) {
        if ((*it)->posix == i)
            return 1;
        it++;
    }
    return 0;
}

void dfs(int x0, int y0, int x1, int y1) {
    pair<int, int>* elem = new pair<int, int>(x0, y0);//valor inicial
    stack<pair<int, int>* > pila;
    pila.push(elem);
    grafo[elem->first][elem->second] = 0;//ya recorrido
    int x, y;
    do {
        //grafo[elem->first][elem->second] = -1;
        cout << elem->first << " " << elem->second << endl;
        pila.pop();
        for (int i = 0; i < 8; ++i) {
            x = elem->first + poss[i][0];
            y = elem->second + poss[i][1];
            if (x > -1 && y > -1 && x < TAM && y < TAM && grafo[x][y]>0) {
                pila.push(new pair<int, int>(x, y));
                grafo[x][y] = 0;
            }
        }
        elem = pila.top();
    } while (elem->first != x1 || elem->second != y1);
    cout << elem->first << " " << elem->second << endl;
}

void aStar(int x0, int y0, int x1, int y1) {
    vector<elem*> open;
    vector<elem*>::iterator it;
    elem* e0 = new elem(x0 * TAM + y0, 0, 0);
    open.push_back(e0);
    grafo[x0][y0] = 0;
    int x, y, h;
    do {
        cout << e0->posix / TAM << " " << e0->posix % TAM << endl;
        open.erase(open.begin());
        for (int i = 0; i < 8; ++i) {
            x = e0->posix / TAM + poss[i][0];
            y = e0->posix % TAM + poss[i][1];
            if (x > -1 && y > -1 && x < TAM && y < TAM && grafo[x][y]>0 && !inVector(open, x * TAM + y)) {
                h = max(abs(x - x1), abs(y - y1));
                //open.emplace(new elem(x*TAM+y,h+e0->g+1,e0->g+1);
                if (open.empty())
                    open.push_back(new elem(x * TAM + y, h + e0->g + 1, e0->g + 1));
                else {
                    it = open.begin();
                    while (it!=open.end() && h + e0->g + 1 > (*it)->f)
                        it++;
                    open.insert(it, new elem(x * TAM + y, h + e0->g + 1, e0->g + 1));
                }
            }
        }
        grafo[e0->posix / TAM][e0->posix % TAM] = 0;
        e0 = *(open.begin());
    } while (e0->posix / TAM != x1 || e0->posix % TAM != y1);
    cout << e0->posix / TAM << " " << e0->posix % TAM << endl;
}

int main()
{
    srand(time(NULL));
    int vals1[TAM2], vals2[TAM2];
    for (int i = 0; i < TAM; ++i) {
        for (int j = 0; j < TAM; ++j) {
            grafo[i][j] = 1;
            vals1[i * TAM + j] = i;
            vals2[i * TAM + j] = i;
        }
    }
    random_shuffle(vals1, vals1 + TAM2);
    random_shuffle(vals2, vals2 + TAM2);
    for (int k = 0; k < TAM20; ++k)
        grafo[vals1[k]][vals2[k]] = -1;//impasables
    //dfs(vals1[TAM20+1],vals2[TAM20+1],vals1[TAM20+2],vals2[TAM20+2]);
    aStar(vals1[TAM20 + 1], vals2[TAM20 + 1], vals1[TAM20 + 2], vals2[TAM20 + 2]);
}
