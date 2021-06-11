#include <algorithm>
#include <iostream>
#include <stack>
#include <queue>
#include <set>
#include <random>

#include "solver.h"
#include "types.h"

Circuito nearestNeighbour(Grafo& G) {
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

Circuito AGM(Grafo& G) {
    Circuito H;
    H.costo = 0;

    Grafo T = Prim(G);

    std::vector<Vertice> recorrido = DFS(T);

    H.vertices.push_back(recorrido.front());
    for (uint i = 1; i < recorrido.size(); ++i) {
        Vertice u = recorrido[i-1];
        Vertice w = recorrido[i];
        H.vertices.push_back(w);
        H.costo += G.costos[u][w];
    }
    H.costo += G.costos[recorrido.back()][1];
    
    return H;
}

Grafo Prim(Grafo& G) {
    Grafo T;
    T.aristas  = 0;
    uint n = G.vertices;
    std::vector<std::vector<uint>> costosAGM(n+1, std::vector<uint>(n+1, INF));
    T.costos = costosAGM;

    std::vector<uint> min(n+1, INF);
    std::vector<bool> visitados(n+1, false);
    std::vector<Vertice> padres(n+1, 0);

    min[1] = 0;
    T.vertices = 1;

    while(T.aristas < n - 1) {
        Vertice u = 1;
        uint minCosto = INF;
        for (Vertice v = 1; v < n + 1; ++v){
            if (!visitados[v] && min[v] < minCosto) {
                minCosto = min[v];
                u = v;
            }
        }
        visitados[u] = true;
        for (Vertice v = 1; v < n + 1; ++v){
            if (!visitados[v] && G.costos[u][v] < min[v]) {
                padres[v] = u;
                min[v] = G.costos[u][v];
            }
        }
        T.vertices++;
        T.aristas++;
    }

    for (uint j = 1; j < padres.size(); ++j) {
        Vertice u = j;
        Vertice v = padres[j];
        if (u != 0 && v != 0 && u != v )
        T.costos[u][v] = G.costos[u][v];
        T.costos[v][u] = G.costos[v][u];
    }
    return T;
}

std::vector<Vertice> DFS(Grafo& G) {
    std::vector<Vertice> orden(G.vertices, 0);
    std::vector<bool> verticesAgregados(G.vertices + 1, false);
    
    Vertice raiz = 1;
    verticesAgregados[raiz] = true;

    uint k = 1;
    orden[raiz - 1] = k;
    k++;

    std::stack<Vertice> pila;
    pila.push(raiz);

    while(!pila.empty()) {
        Vertice actual = pila.top();
        Vertice v = 1;

        while(v < G.vertices + 1 && (verticesAgregados[v] || G.costos[actual][v] == (uint) INF)) ++v;  
        
        if (v == G.vertices + 1) {
            pila.pop();
            continue;
        }

        pila.push(v);
        verticesAgregados[v] = true;
        orden[v-1] = k;
        ++k;
    }

    return orden;
}

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

// PRE = {0 <= p <= 1}
Vecindario _2opt(Circuito& H, Grafo& G, float p) {
    Vecindario vecinos2opt;
    std::vector<Swap> swaps2opt;
    uint n = G.vertices;

    for (uint i = 1; i < n; ++i) {
        for (uint j = i+1; j < n; ++j) {
            swaps2opt.push_back(std::make_pair(swap(H, G, i, j), std::make_pair(i, j)));
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

Circuito tabooSearch(Grafo& G, uint M, uint K, float p) {
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

Circuito tabooSwapSearch(Grafo& G, uint M, uint K, float p) {
    Circuito H = nearestNeighbour(G);
    Circuito S = H;

    std::deque<Indices> memoriaTabu;

    for (uint k = 0; k < K; ++k) {

        Vecindario v2opt = _2opt(H, G, p);        


        // Vemos si los swap asociados a los circuitos de la priority_queue ya
        // fueron visitados y no cumplen la función de aspiración.
        // f_asp(H) : Exceptuamos de la restricción tabú si tiene costo menor al
        // mínimo encontrado hasta ahora.

        while ( !v2opt.empty() &&
                std::count(memoriaTabu.begin(), memoriaTabu.end(), std::get<1>(v2opt.top())) != 0 &&
                S.costo < std::get<0>(v2opt.top()).costo ) {
            v2opt.pop();
        }

        // Si todos los swaps del sub-vecindario seleccionado ya fueron visitados,
        // se sigue con la siguiente iteración. 
        
        if (v2opt.empty()) continue;
        
        H = std::get<0>(v2opt.top());
        Indices I = std::get<1>(v2opt.top());

        // Nos fijamos si la memoria tabú está a máxima capacidad, y en ese caso
        // removemos el swap más viejo y agregamos el más reciente.

        if (memoriaTabu.size() == M)
            memoriaTabu.pop_back();


        // Notar que en caso de que el swap seleccionado cumpla la función de
        // aspiración, este se encontrará dos veces en la memoria tabú. Esto
        // implicaría que ese swap no podrá ser seleccionado (siempre y cuando 
        // no cumpla nuevamente la función de aspiración) únicamente cuando
        // esta última inserción haya sido removida de la memoria.
        
        memoriaTabu.push_front(I);

        if (H.costo < S.costo) S = H;
    }

    return S;
}