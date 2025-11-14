import math
import matplotlib.pyplot as plt
import csv

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def show(self):
        return [self.x, self.y]
    

class Pump:
    def __init__(self, name, coords):
        self.name = name
        self.coords = coords
        self.cell = []

    def show(self):
        return [self.name, self.coords.x, self.coords.y]
    

class Death:
    def __init__(self, quantity, coords):
        self.quantity = quantity
        self.coords = coords

    def show(self):
        return [self.quantity, self.coords.x, self.coords.y]
    

def getPumps(pumps_csv):
    pumps = []

    with open(pumps_csv, newline='', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        
        for row in reader:
            name = row['Pump Name']
            x = float(row['X coordinate'])
            y = float(row['Y coordinate'])
            pumps.append(Pump(name, Point(float(x), float(y))))

    return pumps


def getDeaths(deaths_csv):
    deaths = []

    with open(deaths_csv, newline='', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        
        for row in reader:
            quantity = int(row['Death'])
            x = float(row['X coordinate'])
            y = float(row['Y coordinate'])
            deaths.append(Death(quantity, Point(float(x), float(y))))

    return deaths

# Caja que encierra a los puntos, parecido al triangulo gigante de delaunay
def sorroundingSquare(pumps, padding):
    min_x = min(pump.coords.x for pump in pumps)
    max_x = max(pump.coords.x for pump in pumps)
    min_y = min(pump.coords.y for pump in pumps)
    max_y = max(pump.coords.y for pump in pumps)

    square = [
        Point(min_x - padding, min_y - padding),
        Point(max_x + padding, min_y - padding),
        Point(max_x + padding, max_y + padding),
        Point(min_x - padding, max_y + padding)
    ]

    return square

# Bisector de puntos p y q
def bisector(p, q):
    x0 = (p.x + q.x) / 2.0
    y0 = (p.y + q.y) / 2.0

    a = q.x - p.x
    b = q.y - p.y
    c = -(a * x0 + b * y0)

    return a, b, c

# Asegura que el punto esta mas cerca de p que de q
def point_in_half_plane(point, p, q):
    d1 = (point.x - p.x) ** 2 + (point.y - p.y) ** 2
    d2 = (point.x - q.x) ** 2 + (point.y - q.y) ** 2

    return d1 <= d2 + 1e-9

# Interseccion entre segmento AB de la celda y el bisector ax + by + c = 0
def intersect(A, B, a, b, c):
    fa = a * A.x + b * A.y + c
    fb = a * B.x + b * B.y + c

    denom = fa - fb

    # Segmento y bisector paralelos
    if abs(denom) < 1e-9:
        return None
    
    t = fa / denom
    
    # La interseccion no esta dentro del segmento
    if t < -1e-9 or t > 1 + 1e-9:
        return None
    
    ix = A.x + t * (B.x - A.x)
    iy = A.y + t * (B.y - A.y)
    
    return Point(ix, iy)

# Crea una nueva celda recortando el poligono original con el bisector entre p y q
def clip_cell(polygon, p, q):
    if not polygon:
        return []
    
    # Bisector
    a, b, c = bisector(p, q)
    
    new_poly = []
    
    m = len(polygon)
    
    for i in range(m):
        # Segmentos de la celda
        A = polygon[i]
        B = polygon[(i + 1) % m]
        
        inside_A = point_in_half_plane(A, p, q)
        inside_B = point_in_half_plane(B, p, q)
        
        # Ambos puntos mas cerca de p
        if inside_A and inside_B:
            new_poly.append(Point(B.x, B.y))

        # A mas cerca de p, B mas cerca de q
        elif inside_A and not inside_B:
            # Buscar interseccion
            I = intersect(A, B, a, b, c)
            
            if I:
                new_poly.append(I)

        # A mas cerca de q, B mas cerca de p
        elif not inside_A and inside_B:
            # Buscar interseccion
            I = intersect(A, B, a, b, c)
            
            if I:
                new_poly.append(I)

            new_poly.append(Point(B.x, B.y))
        
        else:
            pass

    
    # Eliminar puntos duplicados consecutivos
    cleaned = []
    for point in new_poly:
        if not cleaned:
            cleaned.append(point)
        else:
            last = cleaned[-1]
            if abs(last.x - point.x) > 1e-12 or abs(last.y - point.y) > 1e-12:
                cleaned.append(point)

    if len(cleaned) > 1:
        first = cleaned[0]
        last = cleaned[-1]
        if abs(first.x - last.x) < 1e-12 and abs(first.y - last.y) < 1e-12:
            cleaned.pop()
    return cleaned


def voronoi(pumps, padding):
    box = sorroundingSquare(pumps, padding)

    cells = []

    for pump in pumps:
        cell = [Point(p.x, p.y) for p in box]

        for other_pump in pumps:
            if other_pump != pump:
                cell = clip_cell(cell, pump.coords, other_pump.coords)

            if not cell:
                break 

        pump.cell = cell
        cells.append(cell)

    return cells
        
# Verifica si un punto esta dentro de una celda
def point_in_cell(point, cell):
    n = len(cell)
    inside = False

    x = point.x
    y = point.y

    p1x = cell[0].x
    p1y = cell[0].y

    for i in range(n + 1):
        p2x = cell[i % n].x
        p2y = cell[i % n].y

        if y > min(p1y, p2y):
            if y <= max(p1y, p2y):
                if x <= max(p1x, p2x):
                    if p1y != p2y:
                        xinters = (y - p1y) * (p2x - p1x) / (p2y - p1y) + p1x
                    if p1x == p2x or x <= xinters:
                        inside = not inside
        p1x, p1y = p2x, p2y

    return inside

def main():
    pumps = getPumps("pumps.csv")
    deaths = getDeaths("deaths.csv")

    voronoi(pumps, 0.001)

    plt.figure()

    for pump in pumps:
        cell = pump.cell
        if cell:
            x_coords = [p.x for p in cell] + [cell[0].x]
            y_coords = [p.y for p in cell] + [cell[0].y]
            plt.plot(x_coords, y_coords, 'b-')

        plt.plot(pump.coords.x, pump.coords.y, 'ro')
        plt.text(pump.coords.x, pump.coords.y, pump.name, fontsize=9, ha='right')

    cmap = plt.cm.get_cmap('tab20', max(1, len(pumps)))
    colors = [cmap(i) for i in range(len(pumps))]

    for death in deaths:
        color = (0.5, 0.5, 0.5, 1.0)  # gris por defecto

        # Revisa cada celda
        for idx, pump in enumerate(pumps):
            # Verifica si el punto de muerte esta dentro de la celda
            if pump.cell and point_in_cell(death.coords, pump.cell):
                color = colors[idx]
                break
        
        plt.scatter(death.coords.x, death.coords.y, c=[color], s=40, marker='x', linewidths=1.5)

    plt.show()


if __name__ == "__main__":
    main()