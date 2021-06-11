#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "types.h"

Circuito nearestNeighbour(Grafo& G);

Circuito farthestInsertion(Grafo G);
Vertice initFarthest(Grafo G, Circuito H);
Vertice fSelect(Grafo G, Circuito H);
void fInsert(Grafo G, Circuito& H, Vertice v);

Circuito AGM(Grafo& G);
std::vector<Vertice> DFS(Grafo& G);
Grafo Prim(Grafo& G);


Vecindario _2opt (Circuito& H, Grafo& G, float p);
Circuito swap(Circuito& H, Grafo& G, uint u, uint w);
Circuito tabooSearch(Grafo& G, uint M, uint K, float p);
Circuito tabooSwapSearch(Grafo& G, uint M, uint K, float p);

#endif // _SOLVER_H_