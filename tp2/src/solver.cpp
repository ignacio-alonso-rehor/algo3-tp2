#include <algorithm>

#include "solver.h"
#include "types.h"

Circuito nearestNeighbour(Grafo G)  {
    Circuito H;

    // Empezamos el circuito con el vértice 1
    H.vertices.push_back(1);
    H.costo = 0;

    while (H.vertices.size() < G.vertices) {
        uint vecinoMin;
        uint costoMin = INF;
 
        Vertice ultimo = H.vertices.back();
        for (Vertice w = 1; w < G.vertices + 1; ++w) {

            // Iteramos sobre todos los vértices todavía no incluidos en H.
            auto it = std::find(H.vertices.begin(), H.vertices.end(), w);
            if (it != H.vertices.end()) continue;

            if (costoMin > G.costos[ultimo][w]) {
                costoMin = G.costos[ultimo][w];
                vecinoMin = w;
            }
        }

        H.vertices.push_back(vecinoMin);
        H.costo += costoMin;
    }

    // Agregamos el costo de la arista (v_n->1)
    H.costo += G.costos[H.vertices.back()][1];
    return H;
}

// Circuito farthestInsertion(Grafo G) {
// }

// Circuito AGM(Grafo G) {
// }