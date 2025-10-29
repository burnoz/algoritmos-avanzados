import matplotlib.pyplot as plt

def lee_archivo(archivo):
	f = open(archivo, "r")
	contenido = f.read()
	f.close()

	lines = contenido.split("\n")
	n = int(lines[0])
	points = [ list(map(float, lines[i].split("\t")))  for i in range(1, len(lines)-1 )]

	return points

def get_val(p1, p2, p3):
    val = (p2[1] - p1[1]) * (p3[0] - p2[0]) - (p2[0] - p1[0]) * (p3[1] - p2[1])
    
    return val

#  0 =  son colineales
#  1 = p3 esta en el sentido del reloj
# -1 = p3 esta counterclockwise
def orientation(p1, p2, p3):
    val = get_val(p1, p2, p3)

    if val == 0:
        return 0
    
    elif val > 0:
        return 1
    
    else:
        return -1

# Distancia del punto p al la linea formada por a y b
def dist_to_line(a, b, p):
    return abs(get_val(a, b, p))

# a - punto inicial de la linea
# b - punto final de la linea
# points_set - conjunto de puntos a considerar
# out - lista donde se almacenan los puntos del convex hull
def find_hull(a, b, points_set, out):
    # Caso base
    if not points_set:
        out.append(b)
        return

    far = points_set[0]
    maxd = dist_to_line(a, b, far)

    # Buscar el punto mas lejano de la linea a - b 
    for p in points_set[1:]:
        d = dist_to_line(a, b, p)
        if d > maxd:
            maxd = d
            far = p

    # Divide el conjunto de puntos en dos subconjuntos (los que quedan fuera del triangulo)
    s1 = [p for p in points_set if p != far and orientation(a, far, p) > 0]
    s2 = [p for p in points_set if p != far and orientation(far, b, p) > 0]

    # Llamada recursiva
    find_hull(a, far, s1, out)
    find_hull(far, b, s2, out)

def quickhull(points):
    n = len(points)

    if n == 0:
        return []
    
    if n == 1:
        return [points[0]]

    # Punto mas a la izquierda
    left_most = min(points, key=lambda p: (p[0], p[1]))
    # Punto mas a la derecha
    right_most = max(points, key=lambda p: (p[0], p[1]))

    left_set = []
    right_set = []

    for pt in points:
        s = orientation(left_most, right_most, pt)

        if s > 0:
            # pt esta a la izquierda de la linea
            left_set.append(pt)
        
        elif s < 0:
            # pt esta a la derecha de la linea
            right_set.append(pt)

    hull = []

    hull.append(left_most)
    find_hull(left_most, right_most, left_set, hull)
    find_hull(right_most, left_most, right_set, hull) 

    # Elimina duplicados
    res = []
    for p in hull:
        if not res or res[-1] != p:
            res.append(p)
            
    # Asegura que el primer y ultimo punto no sean iguales
    if len(res) > 1 and res[0] == res[-1]:
        res.pop()

    return res


base_name = "puntos-n"
n_files = [8, 10, 11, 15, 16, 20, 50, 100]

for num in n_files:
    file_name = f"{base_name}{num}.txt"
    points = lee_archivo(file_name)

    hull = quickhull(points)

    print(f"Convex hull para {file_name} (n={num}):")

    if hull:
        for p in hull:
            print(tuple(p))
    
    else:
        print(f"No hay puntos en el hull para {file_name}")
    
    print()

    xs = [p[0] for p in points]
    ys = [p[1] for p in points]

    plt.scatter(xs, ys, s=20)

    if hull:
        hx = [p[0] for p in hull] + [hull[0][0]]
        hy = [p[1] for p in hull] + [hull[0][1]]
        plt.plot(hx, hy, '-r')

    plt.title(f'QuickHull n={num}')
    plt.show()