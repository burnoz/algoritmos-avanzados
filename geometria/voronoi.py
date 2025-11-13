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


def sorroundingSquare(pumps, padding=1.0):
    min_x = min(pump.coords.x for pump in pumps)
    max_x = max(pump.coords.x for pump in pumps)
    min_y = min(pump.coords.y for pump in pumps)
    max_y = max(pump.coords.y for pump in pumps)

    # create box corners in (clockwise or ccw) order
    square = [
        Point(min_x - padding, min_y - padding),
        Point(max_x + padding, min_y - padding),
        Point(max_x + padding, max_y + padding),
        Point(min_x - padding, max_y + padding)
    ]

    return square

# Helper: coefficients (a, b, c) of bisector line ax + by + c = 0
def bisector_line_coeffs(s, o):
    a = 2 * (o.x - s.x)
    b = 2 * (o.y - s.y)
    c = (s.x * s.x + s.y * s.y) - (o.x * o.x + o.y * o.y)
    return a, b, c

# Helper: check if point p is in half-plane closer to site s than to other o
def point_in_half_plane(p, s, o, eps=1e-9):
    d1 = (p.x - s.x) ** 2 + (p.y - s.y) ** 2
    d2 = (p.x - o.x) ** 2 + (p.y - o.y) ** 2
    return d1 <= d2 + eps

# Helper: intersection between segment AB and line ax + by + c = 0
# returns Point or None
def intersect_segment_line(A, B, a, b, c, eps=1e-12):
    fa = a * A.x + b * A.y + c
    fb = a * B.x + b * B.y + c
    denom = fa - fb
    if abs(denom) < eps:
        # segment parallel to line or both endpoints lie on line (no unique intersection or infinite)
        return None
    t = fa / denom  # t in [0,1] if intersection lies within segment
    if t < -eps or t > 1 + eps:
        return None
    ix = A.x + t * (B.x - A.x)
    iy = A.y + t * (B.y - A.y)
    return Point(ix, iy)

# Clip polygon (list of Point) keeping the half-plane of points closer to s than o
def clip_polygon_by_bisector(polygon, s, o):
    if not polygon:
        return []
    a, b, c = bisector_line_coeffs(s, o)
    new_poly = []
    m = len(polygon)
    for i in range(m):
        A = polygon[i]
        B = polygon[(i + 1) % m]
        inside_A = point_in_half_plane(A, s, o)
        inside_B = point_in_half_plane(B, s, o)

        if inside_A and inside_B:
            # keep B
            new_poly.append(Point(B.x, B.y))
        elif inside_A and not inside_B:
            # A in, B out -> add intersection
            I = intersect_segment_line(A, B, a, b, c)
            if I:
                new_poly.append(I)
        elif not inside_A and inside_B:
            # A out, B in -> add intersection then B
            I = intersect_segment_line(A, B, a, b, c)
            if I:
                new_poly.append(I)
            new_poly.append(Point(B.x, B.y))
        else:
            # both out -> maybe the segment lies on the bisector; ignore
            pass

    # Remove duplicate consecutive points (or close ones)
    cleaned = []
    for p in new_poly:
        if not cleaned:
            cleaned.append(p)
        else:
            last = cleaned[-1]
            if abs(last.x - p.x) > 1e-12 or abs(last.y - p.y) > 1e-12:
                cleaned.append(p)
    # Also ensure polygon isn't closed with repeated first point
    if len(cleaned) > 1:
        first = cleaned[0]
        last = cleaned[-1]
        if abs(first.x - last.x) < 1e-12 and abs(first.y - last.y) < 1e-12:
            cleaned.pop()
    return cleaned

def voronoiDiagram(pumps, padding=0.001):
    # initialize bounding box
    box = sorroundingSquare(pumps, padding)

    cells = []
    for i, pump in enumerate(pumps):
        # start cell as the bounding box (copy points)
        cell = [Point(p.x, p.y) for p in box]

        for j, other_pump in enumerate(pumps):
            if i == j:
                continue
            # clip current cell by half-plane closer to pump.coords than other_pump.coords
            cell = clip_polygon_by_bisector(cell, pump.coords, other_pump.coords)
            if not cell:
                break

        # assign and collect
        pump.cell = cell
        cells.append(cell)

    return cells, box


def main():
    pumps = getPumps("pumps.csv")
    deaths = getDeaths("deaths.csv")

    voronoiDiagram(pumps)

    # Plotting
    plt.figure()

    for pump in pumps:
        cell = pump.cell
        if cell:
            x_coords = [p.x for p in cell] + [cell[0].x]
            y_coords = [p.y for p in cell] + [cell[0].y]
            plt.plot(x_coords, y_coords, 'b-')
        plt.plot(pump.coords.x, pump.coords.y, 'ro')
        plt.text(pump.coords.x, pump.coords.y, pump.name, fontsize=9, ha='right')


    for death in deaths:        
        plt.plot(death.coords.x, death.coords.y, 'kx')



if __name__ == "__main__":
    main()