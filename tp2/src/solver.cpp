#include <algorithm>
#include <limits>

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

 Circuito farthestInsertion(Grafo G) {
     Circuito H;
     std::vector<uint> agregados(G.vertices + 1, 0);

     H.vertices.push_back(1);
     agregados[1] = 1;

     Vertice w = initFarthest(G, H);
     H.vertices.push_back(w);
     agregados[w] = 1;

     while(H.vertices.size() < G.vertices) {

         Vertice v = fSelect(G, H, agregados);
         agregados[v] = 1;
         fInsert(G, H, v);
     }

     return H;
 }

 Vertice initFarthest(Grafo G, Circuito H) {
     Vertice w = 0;
     uint costoMax = 0;
     for (Vertice u = 1; u < G.vertices + 1; ++u) {
         if (G.costos[1][u] > costoMax) {
             costoMax = G.costos[1][u];
             w = u;
         }
     }
     H.costo += costoMax;
     return w;
 }

 Vertice fSelect(Grafo G, Circuito H, std::vector<uint> ag) {
     Vertice v = 0;
     uint costoMax = 0;

     for (Vertice u : H.vertices){
         uint costoMinParcial = std::numeric_limits<int>::max();
         Vertice min_v = 0;

         for (Vertice w = 1; w < G.vertices + 1; ++w){
             if (u != w && ag[w] == 0) {
                 if (G.costos[u][w] < costoMinParcial) {
                     costoMinParcial = G.costos[u][w];
                     min_v = w;
                 }
             }
         }

         if (costoMinParcial > costoMax) {
             costoMax = costoMinParcial;
             v = min_v;
         }
     }
     H.costo += costoMax;
     return v;
}

void fInsert(Grafo G, Circuito H, Vertice v){
    
}

// Circuito AGM(Grafo G) {
// }