#ifndef TYPES_H_
#define TYPES_H_

#include <vector>


#define INF -1

typedef unsigned int uint;
typedef uint Vertice;

typedef struct {
    uint vertices;
    uint aristas;
    std::vector<std::vector<uint>> costos;
} Grafo;

typedef std::vector<Vertice> Circuito;

#endif // TYPES_H_