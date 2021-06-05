#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "types.h"

Circuito nearestNeighbour(Grafo G);
Circuito farthestInsertion(Grafo G);
    Vertice initFarthest(Grafo G, Circuito H);
    Vertice fSelect(Grafo G, Circuito H, std::vector<uint> marcados);
    void fInsert(Grafo G, Circuito H, Vertice v);
Circuito AGM(Grafo G);

#endif // _SOLVER_H_