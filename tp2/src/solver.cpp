#include <algorithm>
#include <iostream>
#include <stack>
#include <queue>
#include <set>
#include <random>

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
    H.costo += 2 * G.costos[1][w]; 

    while(H.vertices.size() < G.vertices) {
        Vertice v = fSelect(G, H);         
        fInsert(G, H, v);
    }

    //H.costo += G.costos[H.vertices.back()][H.vertices.front()];

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

    for (Vertice u = 1; u < G.vertices + 1; ++u) {
        // Iteramos sobre todos los vértices todavía no incluidos en H.
        auto it = std::find(H.vertices.begin(), H.vertices.end(), u); 
        if (it != H.vertices.end()) continue;

        uint costoMinParcial = INF;

        // Buscamos la menor arista hacia un vertice incluido en H.
        for (Vertice w : H.vertices) {
            if (G.costos[u][w] < costoMinParcial){
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

void fInsert(Grafo G, Circuito& H, Vertice v) {
    Vertice u = H.vertices.back();
    Vertice w = H.vertices.front();
    int costoPrev = G.costos[u][w];
    int costoPost = G.costos[u][v] + G.costos[v][w];

    int minCosto = costoPost - costoPrev;
    Vertice pos = w;

    for (uint j = 0; j < H.vertices.size() - 1 ; ++j){
        u = H.vertices[j];
        w = H.vertices[j + 1];
        costoPrev = G.costos[u][w];
        costoPost = G.costos[u][v] + G.costos[v][w];
        if (costoPost - costoPrev < minCosto){
            minCosto = costoPost - costoPrev;
            pos = w;
        }
    }

    auto it = std::find(H.vertices.begin(), H.vertices.end(), pos);
    if (it == H.vertices.begin()) H.vertices.push_back(v);
    else H.vertices.insert(it, v);
    H.costo += minCosto;
}

Circuito AGM(Grafo G) {
    Circuito H;
    H.costo = 0;

    Grafo T;
    uint n = G.vertices;
    T.vertices = n;

    std::vector<Vertice> aristasArbol = DFS(G, 0); 

    //Reconstruyo el AGM con los valores de pred del DFS
    std::vector<std::vector<uint>> costosAGM(n+1, std::vector<uint>(n+1, INF));
    T.costos = costosAGM;
    
    for (Vertice v = 2; v < aristasArbol.size(); ++v){
        Vertice w = aristasArbol[v];
        T.costos[v][w] = G.costos[v][w];
        T.costos[w][v] = G.costos[w][v];
        T.aristas++;
    }

    std::vector<Vertice> recorrido = DFS(T, 1);

    H.vertices.push_back(recorrido.front());
    for (uint i = 1; i < recorrido.size(); ++i) {
        Vertice u = recorrido[i-1];
        Vertice w = recorrido[i];
        H.vertices.push_back(w);
        H.costo += G.costos[u][w];
    }
    H.costo += G.costos[recorrido.back()][recorrido.front()];
    
    return H;
}

std::vector<Vertice> DFS(Grafo G, uint op) { //op=0 devuelve los padres, sino devuelve el orden.
    std::vector<Vertice> pred(G.vertices + 1, 0);
    std::vector<Vertice> orden(G.vertices, 0);
    Vertice r = 1;
    int next = 1;
    orden[r - 1] = next;

    std::stack<Vertice> pila;
    pila.push(r);

    while(!pila.empty()){
        Vertice u = pila.top();
        for (Vertice w = 1; w < G.vertices + 1; ++w) {
            if (u != w && w != r && pred[w] == 0 && G.costos[u][w] != INF) {
                pred[w] = u;
                next++;
                orden[w - 1] = next;
                pila.push(w);
            }
        }
        pila.pop();
    }

    if (op == 0) return pred;
    else return orden;
}

// Circuito tabuSearchSoluciones(Grafo G, uint k) {
//     Circuito H = nearestNeighbour(G);
//     std::queue<Circuito> mem;

//     while(true) {
        
//         for (uint i = 0; i < k; ++i){
//             for (uint j = i; j < k + 1; ++j){
//                 nuevoCircuito = 2opt()
//             }
//         }
//     }
// }

// Circuito tabuSearchEstructura() {
//     return Circuito;
// };

Circuito swap(Circuito& H, Grafo& G, uint i, uint j) {
    Circuito N = H;
    N.costo = 0;

    auto begin = std::next(N.vertices.begin(), i);
    auto end = std::next(N.vertices.begin(), j);

    std::reverse(begin, ++end);

    for (uint k = 0; k < N.vertices.size(); ++k) {
        Vertice v = N.vertices[k];
        Vertice w = N.vertices[(k+1) % N.vertices.size()];
        N.costo += G.costos[v][w];
    }

    return N;
}

/*// PRE = {0 <= p <= 1}
Vecindario _2opt (Circuito& H, Grafo& G, float p) {
    Vecindario vecinos2opt;
    std::vector<Circuito> circuitosSwaps;
    uint n = G.vertices;

    for (uint i = 1; i < n; ++i) {
        for (uint j = i+1; j < n; ++j) {
            circuitosSwaps.push_back(swap(H, G, i, j));
        }
    }

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(circuitosSwaps.begin(), circuitosSwaps.end(), rng);

    auto itVecino = circuitosSwaps.begin();
    while ((float) vecinos2opt.size() / ((n-1)*(n-2)/2) < p && itVecino != circuitosSwaps.end()) {
        vecinos2opt.push(*itVecino);
        itVecino++;
    }

    return vecinos2opt;
}*/

Vecindario _2opt(Circuito& H, Grafo& G, float p) {
    Vecindario vecinos2opt;
    std::vector<Swap> swaps2opt;
    uint n = G.vertices;

    for (uint i = 1; i < n; ++i) {
        for (uint j = i+1; j < n; ++j) {
            swaps2opt.push_back(std::make_tuple(swap(H, G, i, j), i, j));
        }
    }

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(swaps2opt.begin(), swaps2opt.end(), rng);

    auto itVecino = swaps2opt.begin();
    
    while ((float) vecinos2opt.size() / ((n-1)*(n-2)/2) < p && itVecino != swaps2opt.end()) {
        vecinos2opt.push(*itVecino);
        itVecino++;
    }

    return vecinos2opt;
}


Circuito tabuSearch(Grafo& G, uint M, uint K, float p) {
    Circuito H = nearestNeighbour(G);
    Circuito S = H;

    std::deque<Circuito> memoriaTabu;

    for (uint k = 0; k < K; ++k) {

        Vecindario v2opt = _2opt(H, G, p);
         
        // Como v2opt es una priority_queue que tiene al circuito de menor costo
        // como raíz, hacemos pop() hasta que encontremos un Circuito que no hayamos
        // visitado, o hasta que no tengamos más circuitos en el sub-vecindario
        // seleccionado.
        

        while (!v2opt.empty() && std::count(memoriaTabu.begin(), memoriaTabu.end(), std::get<0>(v2opt.top())) != 0)
            v2opt.pop();

        // Si todos los circuitos del sub-vecindario seleccionado ya fueron visitados,
        // se sigue con la siguiente iteración. 
        
        if (v2opt.empty()) continue;
        
        H = std::get<0>(v2opt.top());

        // Nos fijamos si la cola de circuitos ya visitados está a máxima capacidad, y
        // en ese caso removemos el circuito más viejo y agregamos el más reciente.

        if (memoriaTabu.size() == M)
            memoriaTabu.pop_back();

        memoriaTabu.push_front(H);

        // En caso de que el circuito que estamos considerando actualmente tenga
        // menor costo que el minimo hasta el momento, lo actualizamos.

        if (H.costo < S.costo) S = H;
    }

    return S;
}



/*
//2opt
Circuito 2opt(Circuito H)
Circuito H = AGM(G); //lo buscaria al circuito aca? o lo hago antes en tabusearch?
mejorCircuito = H;
int improvements = 0;

While (improvements < 20) {
    best_distance = calculateTotalDistance(existing_route) //Aca hay que calcular de vuelta el costo.
    start_again:
    for (i = 0; i <= length(G) - 1 && encontro == false; i++) {
        for (k = i + 1; k <= length(g); k++) {
            Circuito new_route = 2optSwap(existing_route, i, k)
            int new_distance = new_route.costo(); // aca tendria que ser algo como new_distance.costo()
            if (new_distance < best_distance) {
                existing_route = new_route;
                best_distance = new_distance
                encontro == true;
                goto start_again // esto lo que hace es empezar los ciclos for de vuelta
            }
            improvements++;
        }
    }
}

Circuito 2optSwap(route, i, k) {
    Circuito new_route;
    1. take route[0] to route[i-1] and add them in order to new_route
    for (int l = 0; l < i; l++){
        new_route->vertices.push_back(route->vertices[l])
    }
    2. take route[i] to route[k] and add them in reverse order to new_route
    int copy_k = k;
    for (int p = i; p < k + 1; p++){
        new_route->vertices.push_back(route->vertices[copy_k]);
        copy_k--;
    }
    3. take route[k+1] to end and add them in order to new_route
    for (int b = k+1; b < length(new_route->vertices); b++){
        new_route->vertices.push_back(route->vertices[b]);
    }

    H.costo -
    return new_route;
}


... v1 --- w2 ...
... v2 --- w1 ...

Costo(H) = (v1, w2) + (v2, w1) + Costo(H*)

...  v1  w2 ...
       \/
       /\
... v2    w1 ...


Costo(2optH) = Costo(H) - (v1, w2) - (v2, w1) + (v1, w1) + (v2, w2)
             = Costo(H*) + 

Ejemplo para 2optSwap
Example route: A → B → C → D → E → F → G → H → A
Example parameters: i = 4, k = 7 (starting index 1)
Contents of new_route by step:
(A → B → C)
A → B → C → (G → F → E → D)
A → B → C → G → F → E → D → (H → A)

}*/