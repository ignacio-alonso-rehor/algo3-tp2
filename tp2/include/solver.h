#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "types.h"

Circuito nearestNeighbour(Grafo G);

Circuito farthestInsertion(Grafo G);
Vertice initFarthest(Grafo G, Circuito H);
Vertice fSelect(Grafo G, Circuito H);
void fInsert(Grafo G, Circuito& H, Vertice v);

Circuito AGM(Grafo G);
std::vector<Vertice> DFS(Grafo G, uint op);

Vecindario _2opt (Circuito& H, Grafo& G, float p, std::deque<Circuito>& tabu);
Circuito swap(Circuito& H, Grafo& G, uint u, uint w);

#endif // _SOLVER_H_