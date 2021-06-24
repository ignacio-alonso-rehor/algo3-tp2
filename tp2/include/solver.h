#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "types.h"

Circuito nearestNeighbour(Grafo& G);

Circuito farthestInsertion(Grafo G);
Vertice initFarthest(Grafo G, Circuito H);
Vertice fSelect(Grafo G, Circuito H);
void fInsert(Grafo G, Circuito& H, Vertice v);

Circuito christofidesSimplificado(Grafo& G);
Grafo AGM(Grafo& G);
std::vector<Vertice> dfs(Grafo& G);
void dfsVecinos(Vertice v, Grafo& G, std::vector<bool>& visitados, std::vector<Vertice>& orden);

Vecindario _2opt (Circuito& H, Grafo& G, float p);
Circuito swap(Circuito& H, Grafo& G, uint u, uint w);
Circuito tabuSearch(Grafo& G, uint M, uint K, float p);
Circuito tabuSwapSearch(Grafo& G, uint M, uint K, float p);

#endif // _SOLVER_H_