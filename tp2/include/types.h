#ifndef TYPES_H_
#define TYPES_H_

#include <vector>
#include <queue>
#include <tuple>

#define INF -1

typedef unsigned int uint;
typedef uint Vertice;
typedef uint Arco;

typedef struct {
    uint vertices;
    uint aristas;
    std::vector<std::vector<uint>> costos;
} Grafo;

typedef struct s_circuito{
    std::vector<Vertice> vertices;
    uint costo;
    
    bool operator>(const s_circuito& C) const {
        return this->costo > C.costo;
    }

    bool operator<(const s_circuito& C) const {
        return this->costo < C.costo;
    }

    bool operator==(const s_circuito& C) const {
        return this->vertices == C.vertices && this->costo == C.costo;
    }
} Circuito;

// typedef std::priority_queue<Circuito, std::vector<Circuito>, std::greater<Circuito>> Vecindario;

typedef std::tuple<Circuito, uint, uint> Swap;
typedef std::priority_queue<Swap, std::vector<Swap>, std::greater<Swap>> Vecindario;

#endif // TYPES_H_