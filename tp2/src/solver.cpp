#include <algorithm>
#include <iostream>
#include <stack>

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
    int costoPrev = G.costos[u][w];
    int costoPost = G.costos[u][v] + G.costos[v][w];

    int minCosto = costoPost - costoPrev;
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


Circuito AGM(Grafo G) {
    Circuito H;
    H.costo = 0;

    std::vector<Vertice> recorrido = DFS(G);

    H.vertices.push_back(recorrido.front());
    for (uint i = 1; i < recorrido.size(); ++i){
        Vertice u = recorrido[i-1];
        Vertice w = recorrido[i];
        H.vertices.push_back(w);
        H.costo += G.costos[u][w];
    }
    H.costo += G.costos[recorrido.back()][recorrido.front()];
    
    return H;
}

std::vector<Vertice> DFS(Grafo G) {

    //Hago una pasada de DFS sobre G para obtener el AGM
    std::vector<Vertice> pred(G.vertices + 1, 0);
    std::vector<Vertice> orden(G.vertices + 1, 0);
    int next = 1;
    Vertice r = 1;

    pred[r] = 0;
    orden[r] = next;

    std::stack<Vertice> pila;
    pila.push(r);

    while(!pila.empty()){
        Vertice u = pila.top();
        for (Vertice w = 1; w < G.vertices + 1; ++w) {
            if (u != w && w != r && pred[w] == 0) {
                pred[w] = u;
                next++;
                orden[w] = next;
                pila.push(w);
            }
        }
        pila.pop();
    }
    
    //Marco las aristas de G que estan en el AGM
    std::vector<bool> b(G.vertices + 1, false);
    std::vector<std::vector<bool>> aristasAGM(G.vertices + 1, b);
    for (uint j = 1; j < G.vertices + 1; ++j){
        for (uint k = 1; k < G.vertices + 1; ++k){
            aristasAGM[j][k] = false;
        }
    }

    for (Vertice v = 1; v < pred.size(); ++v){
        aristasAGM[v][pred[v]] = true;
        aristasAGM[pred[v]][v] = true;
    }

    //Vuelvo a hacer DFS considerando solo las aristas del AGM
    std::vector<Vertice> predAGM(G.vertices + 1, 0);
    std::vector<Vertice> ordenAGM(G.vertices + 1, 0);
    int nextAGM = 1;
    Vertice r_AGM = 1;

    predAGM[r_AGM] = 0;
    ordenAGM[r_AGM] = nextAGM;

    std::stack<Vertice> pilaAGM;
    pilaAGM.push(r_AGM);

    while(!pilaAGM.empty()){
        Vertice u = pilaAGM.top();
        for (Vertice w = 1; w < G.vertices + 1; ++w) {
            if (u != w && w != r_AGM && predAGM[w] == 0 && aristasAGM[u][w]) {
                predAGM[w] = u;
                nextAGM++;
                ordenAGM[w] = nextAGM;
                pilaAGM.push(w);
            }
        }
        pilaAGM.pop();
    }

    std::vector<Vertice> recorrido(G.vertices);
    for (uint i = 0; i < ordenAGM.size() - 1; ++i){
        recorrido[i] = ordenAGM[i + 1];
    }
    return recorrido;
}