#include <iostream>
#include <algorithm>
#include <time.h>
#include <chrono>
#include <vector>
#include <map>

using namespace std;

class Producto {
  public:
    int w;
    int r;
    Producto(int peso, int resistencia) {
      w = peso;
      r = resistencia;
    }
};

typedef unsigned int Vertice;
typedef unsigned int Costo;
typedef vector< vector<Vertice> > Grafo;

int INVALIDO = 0;
int N_INDEX = 1;
int M_INDEX = 2;
int n; // Cantidad de vertices
int m; // Cantidad de aristas
int optimo = 0;
Grafo G;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cerr << "Parámetros faltantes: " << endl << "Cantidad de vertices, aristas del grafo" << endl;
    return 1;
  }
  cout << "Cantidad de vertices: " << argv[N_INDEX] << endl;
  n = stoi(argv[N_INDEX]);
  cout << "Cantidad de aristas: " << argv[M_INDEX] << endl;
  m = stoi(argv[M_INDEX]);
  cout << endl << "Aristas:" << endl;
  G = vector< vector<Vertice> >(n, vector<Vertice>(n, 0));
  for (int i = M_INDEX + 1; i < argc; i += 3) {
    Vertice v = stoi(argv[i]);
    Vertice w = stoi(argv[i+1]);
    Costo c = stoi(argv[i+2]);
    cout << "Arista: (" << v << "->" << w << ") con costo: " << c << endl;
    G[v][w] = c;
  };
  cout << endl;

  cout << "Arista (1,2) = " << G[1][2] << endl;

  return 0;
}
