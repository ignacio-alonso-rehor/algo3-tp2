import subprocess

def run(
        heuristica: str,
        entrada: str,
        cant_iters: int = None,
        tam_memoria: int = None,
        porcentaje_vecindad: int = None
    ) -> (int, float) :
    
    args = [
        cant_iters,
        tam_memoria,
        porcentaje_vecindad,
    ]

    call = f'../TSP {heuristica}'
    
    for a in args:
        if a is None:
            call += ' 0'
        else:
            call += f' {a}'
    
    result = subprocess.run(f'{call} < {entrada}', shell=True, capture_output=True, text=True)
    
    result.check_returncode()
    costo  = str(result.stdout).split()[1]
    return int(costo), float(result.stderr)

def generar_instancia(archivo_entrada, archivo_salida):
    prob = tl.load(archivo_entrada)
    salida = open(archivo_salida,'w')

    grafo = prob.get_graph()

    print(grafo.number_of_nodes(),grafo.number_of_edges(), file=salida)
    
    if grafo.nodes.get(0) is not None:
        for u,v in grafo.edges:
            if u<v:
                print(u+1,v+1,grafo.get_edge_data(u,v)['weight'], file=salida)
    else:
        for u,v in grafo.edges:
            if u<v:
                print(u,v,grafo.get_edge_data(u,v)['weight'], file=salida)