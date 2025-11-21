import numpy as np
import queue
import random

ROWS, COLS = 3, 3

INITIAL_STATE_LIST = [
    [3, 2, 0],
    [7, 1, 4],
    [6, 5, 8]
]

INITIAL_STATE = tuple(map(tuple, INITIAL_STATE_LIST))

TARGET_STATE_LIST = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 0]
]

TARGET_STATE = tuple(map(tuple, TARGET_STATE_LIST))

# Heuristica (distancia)
def heuristic(current_state, target_state):
    h = 0
    # Posiciones objetivo
    target_positions = {}

    for r in range(ROWS):
        for c in range(COLS):
            target_positions[target_state[r][c]] = (r, c)

    for r in range(ROWS):
        for c in range(COLS):
            tile = current_state[r][c]
            
            if tile != 0:
                # Obtener la posicion objetivo del tile
                tr, tc = target_positions[tile]
                
                h += abs(r - tr) + abs(c - tc)
    
    return h

# Obtiene los vecinos
def get_neighbors(current_state):
    neighbors = []
    
    # Posicion de la casilla _
    r0, c0 = -1, -1
    for r in range(ROWS):
        try:
            c = current_state[r].index(0)
            r0, c0 = r, c
            break
        except ValueError:
            continue
    
    # Movimientos posibles
    moves = [(-1, 0), (1, 0), (0, -1), (0, 1)] 
    
    current_list = [list(row) for row in current_state]
    
    for dr, dc in moves:
        nr, nc = r0 + dr, c0 + dc # Nueva posicion
        
        if 0 <= nr < ROWS and 0 <= nc < COLS:
            new_list = [row[:] for row in current_list]
            
            # Intercambio
            new_list[r0][c0] = new_list[nr][nc]
            new_list[nr][nc] = 0
            
            new_state = tuple(map(tuple, new_list))
            neighbors.append(new_state)
            
    return neighbors

# Reconstruye el path
def reconstruct_path(came_from, current):
    path = []
    
    while current is not None:
        path.append(current)
        current = came_from.get(current) 
    
    path.reverse()
    
    return path

# A*
def aStar_nPuzzle(estadoInicial, estadoObjetivo):
    q = queue.PriorityQueue()
    
    costo_inicial_g = 0
    heuristica_inicial_h = heuristic(estadoInicial, estadoObjetivo) # h(n)
    costo_total_f = costo_inicial_g + heuristica_inicial_h # f(n) = g(n) + h(n)
    
    q.put((costo_total_f, estadoInicial)) 
    
    came_from = {estadoInicial: None}
    cost_so_far = {estadoInicial: 0}
    
    while not q.empty():
        estado = q.get()[1]
        
        if estado == estadoObjetivo:
            return reconstruct_path(came_from, estadoObjetivo)
        
        vecinos = get_neighbors(estado)
        
        for v in vecinos:
            new_cost = cost_so_far[estado] + 1
            
            if v not in cost_so_far or new_cost < cost_so_far[v]:
                cost_so_far[v] = new_cost
                priority_h = heuristic(v, estadoObjetivo)
                priority_f = new_cost + priority_h
                
                q.put((priority_f, v))
                came_from[v] = estado
                
    return [] # No existe camino


def main():
    print("N-Puzzle")
    print("\nEstado Inicial:")
    print(np.array(INITIAL_STATE_LIST))
    print("\nEstado Objetivo:")
    print(np.array(TARGET_STATE_LIST))
    
    path_states = aStar_nPuzzle(INITIAL_STATE, TARGET_STATE)
    
    if path_states:
        print(f"\nSolución en {len(path_states) - 1} pasos")
        
        for i, state in enumerate(path_states):
            print(f"Paso {i}:")
            
            state_array = np.array(state)
            
            # 0 -> _
            state_str = str(state_array).replace(' 0', ' _').replace('0', ' _')
            print(state_str)
            print()
    
    else:
        print("\nNo se encontró solución")

if __name__ == '__main__':
    main()