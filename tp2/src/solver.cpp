#include <algorithm>
#include <limits>
#include <iostream>

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
     H.costo = 0;

     H.vertices.push_back(1);

     Vertice w = initFarthest(G, H);
     H.vertices.push_back(w);
     H.costo += G.costos[1][w]; 

     while(H.vertices.size() < G.vertices) {
         Vertice v = fSelect(G, H);         
         fInsert(G, &H, v);
     }

     H.costo += G.costos[H.vertices.back()][H.vertices.front()];

     return H;
 }

 Vertice initFarthest(Grafo G, Circuito H) {
     Vertice w = 0;
     uint costoMax = 0;
     for (Vertice u = 2; u < G.vertices + 1; ++u) {
         if (G.costos[1][u] > costoMax) {
             costoMax = G.costos[1][u];
             w = u;
         }
     }
     return w;
 }

 Vertice fSelect(Grafo G, Circuito H) {
     Vertice v = 1;
     uint costoMax = 0;  
     for (Vertice u = 1; u < G.vertices + 1; ++u){

        // Iteramos sobre todos los vértices todavía no incluidos en H.
        auto it = std::find(H.vertices.begin(), H.vertices.end(), u); 
        if (it != H.vertices.end()) continue;           
        
        Vertice w = H.vertices.front();
        uint costoMinParcial = G.costos[u][w];//2-1...3-1
        
        // Buscamos la menor arista hacia un vertice incluido en H.
        for (uint j = 1; j < H.vertices.size(); ++j){
            w = H.vertices[j];//4
            if (G.costos[u][w] < costoMinParcial){//2-4...3-4
                costoMinParcial = G.costos[u][w];
            }            
        }

        // Nos quedamos con la mas lejana
        if (costoMinParcial > costoMax) {
            costoMax = costoMinParcial;
            v = u;
        }
     }
     return v;
}

void fInsert(Grafo G, Circuito* H, Vertice v){

    Vertice u = H->vertices.back();
    Vertice w = H->vertices.front();
    uint costoPrev = G.costos[u][w];
    uint costoPost = G.costos[u][v] + G.costos[v][w];

    uint minCosto = costoPost - costoPrev;
    Vertice pos = w;

    for (uint j = 0; j < H->vertices.size() - 1 ; ++j){
        u = H->vertices[j];
        w = H->vertices[j + 1];
        costoPrev = G.costos[u][w];
        costoPost = G.costos[u][v] + G.costos[v][w];
        if (costoPost - costoPrev < minCosto){
            minCosto = costoPost - costoPrev;
            pos = w;
        }
    }

    auto it = std::find(H->vertices.begin(), H->vertices.end(), pos);
    H->vertices.insert(it, v);
    H->costo += minCosto;
}


// Circuito AGM(Grafo G) {
// }