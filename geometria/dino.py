import queue
import numpy as np

ROWS, COLS = 2, 4
# ROWS, COLS = 4, 4
EMPTY_SPACE = '' 
CYCAD = 'cycad'
STEGO = 'stego'
BRACHIO = 'brachio'
TYRANNOS = 'tyrannos'

DINOS = [STEGO, BRACHIO, TYRANNOS]

# INITIAL_STATE_LIST = [
#     [STEGO, EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE],
#     [STEGO, EMPTY_SPACE, EMPTY_SPACE, CYCAD]
# ]
# INITIAL_STATE = tuple(map(tuple, INITIAL_STATE_LIST))

# TARGET_STATE_LIST = [
#     [EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE, STEGO],
#     [EMPTY_SPACE, EMPTY_SPACE, STEGO, CYCAD]
# ]
# TARGET_STATE = tuple(map(tuple, TARGET_STATE_LIST))

# INITIAL_STATE_LIST = [
#     [EMPTY_SPACE, BRACHIO, EMPTY_SPACE, CYCAD],
#     [EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE, BRACHIO],
#     [EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE, BRACHIO],
#     [CYCAD, BRACHIO, EMPTY_SPACE, EMPTY_SPACE]
# ]
# INITIAL_STATE = tuple(map(tuple, INITIAL_STATE_LIST))

# TARGET_STATE_LIST = [
#     [EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE, CYCAD],
#     [EMPTY_SPACE, EMPTY_SPACE, BRACHIO, BRACHIO],
#     [EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE, BRACHIO],
#     [CYCAD, EMPTY_SPACE, EMPTY_SPACE, BRACHIO]
# ]
# TARGET_STATE = tuple(map(tuple, TARGET_STATE_LIST))

INITIAL_STATE_LIST = [
    [BRACHIO, EMPTY_SPACE, TYRANNOS, BRACHIO],
    [EMPTY_SPACE, BRACHIO, CYCAD, BRACHIO]
]
INITIAL_STATE = tuple(map(tuple, INITIAL_STATE_LIST))

TARGET_STATE_LIST = [
    [TYRANNOS, BRACHIO, BRACHIO, BRACHIO],
    [EMPTY_SPACE, EMPTY_SPACE, CYCAD, BRACHIO]
]
TARGET_STATE = tuple(map(tuple, TARGET_STATE_LIST))

# Posiciones objetivo de un tipo de dinosaurio
def find_target_positions(target_state, dinosaur_type):
    positions = []
    
    for r in range(ROWS):
        for c in range(COLS):
            if target_state[r][c] == dinosaur_type:
                positions.append((r, c))
    
    return positions

# Heuristica (distancia)
def heuristic(current_state, target_state):
    h = 0
    
    for dino_type in DINOS:
        current_dinos = []

        for r in range(ROWS):
            for c in range(COLS):
                if current_state[r][c] == dino_type:
                    current_dinos.append((r, c))

        target_dinos = find_target_positions(target_state, dino_type)

        available_targets = set(target_dinos)
        
        for r_curr, c_curr in current_dinos:
            min_dist = float('inf')
            best_target = None
            
            # Busca el objetivo disponible más cercano 
            for r_target, c_target in available_targets:
                dist = abs(r_curr - r_target) + abs(c_curr - c_target)
                
                if dist < min_dist:
                    min_dist = dist
                    best_target = (r_target, c_target)
            
            # Sumar la distancia del mejor emparejamiento y remover el objetivo
            if best_target:
                h += min_dist
                available_targets.remove(best_target)

    return h

# Obtiene los vecinos
def get_neighbors(current_state):
    neighbors = []

    empty_positions = []
    
    # Posiciones a las que se puede mover
    for r in range(ROWS):
        for c in range(COLS):
            if current_state[r][c] == EMPTY_SPACE:
                empty_positions.append((r, c))

    # Movimientos posibles
    moves = [(-1, 0), (1, 0), (0, -1), (0, 1)]

    current_list = [list(row) for row in current_state]

    for r0, c0 in empty_positions:
        for dr, dc in moves:
            nr, nc = r0 + dr, c0 + dc  # Nueva posicion

            if 0 <= nr < ROWS and 0 <= nc < COLS:
                # Verifica que no sea CYCAD/obstaculo
                if current_list[nr][nc] != CYCAD:
                    new_list = [row[:] for row in current_list]
                    
                    # Intercambio
                    new_list[r0][c0] = new_list[nr][nc]
                    new_list[nr][nc] = EMPTY_SPACE

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
def aStar_DinoSort(estadoInicial, estadoObjetivo):
    q = queue.PriorityQueue()
    
    costo_inicial_g = 0 
    heuristica_inicial_h = heuristic(estadoInicial, estadoObjetivo)
    costo_total_f = costo_inicial_g + heuristica_inicial_h
    
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

def main_dinosort():
    print(f"DinoSort")
    
    path_states = aStar_DinoSort(INITIAL_STATE, TARGET_STATE)
    
    if path_states:
        print(f"\nSolución en {len(path_states) - 1} pasos\n")
        
        for i, state in enumerate(path_states):
            print(f"Paso {i}:")

            state_array = np.array(state)
            
            for row in state_array:
                row_str = ' '.join([elem if elem != EMPTY_SPACE else '_' for elem in row])
                print(row_str)
             
            print()

    else:
        print("\nNo se encontró solución")

if __name__ == '__main__':
    main_dinosort()