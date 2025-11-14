import matplotlib.pyplot as plt

def lee_archivo(archivo):
	f = open(archivo, "r")
	contenido = f.read()
	f.close()

	lines = contenido.split("\n")
	n = int(lines[0])
	aux = [list(map(float, lines[i].split("\t")))  for i in range(1, len(lines)-1)]

	points = []

	for a in aux: 
		points.append(Punto(a[0], a[1]))

	return n, points


class Punto:
	def __init__ ( self, x, y ):
		self.x = x
		self.y = y

	def show(self):
		return [self.x, self.y]


class Segmento:
	def __init__ ( self, A, B ):
		self.A = A
		self.B = B

	def show(self):
		return [self.A.x,self.A.y, self.B.x,self.B.y]


class Triangulo:
	def __init__ ( self, puntos ):
		self.puntos = puntos
		s1 = Segmento( puntos[0],  puntos[1])
		s2 = Segmento( puntos[1],  puntos[2])
		s3 = Segmento( puntos[2],  puntos[0])
		self.lados = [s1, s2, s3]

		# darle valores a centro y radio es el problema 2
		self.centro = None
		self.radio = None 


def create_triangulo(puntos):
	min_x = min(puntos, key=lambda p: p.x).x
	max_x = max(puntos, key=lambda p: p.x).x
	min_y = min(puntos, key=lambda p: p.x).y
	max_y = max(puntos, key=lambda p: p.x).y

	dx = max_x - min_x
	dy = max_y - min_y

	dmax = max(dx, dy)

	centro_x = min_x + dx/2
	centro_y = min_y + dy/2

	p1 = Punto(centro_x - 2 * dmax, centro_y - dmax * 2)
	p2 = Punto(centro_x, centro_y + 2 * dmax)
	p3 = Punto(centro_x + 2 * dmax, centro_y - dmax * 2)

	return Triangulo([p1, p2, p3])


def get_circumcircle(triangulo):
    A = triangulo.puntos[0]
    B = triangulo.puntos[1]
    C = triangulo.puntos[2]

    D = 2 * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y))

    if D == 0:
        return None, None

    Ux = ((A.x**2 + A.y**2) * (B.y - C.y) + (B.x**2 + B.y**2) * (C.y - A.y) + (C.x**2 + C.y**2) * (A.y - B.y)) / D
    Uy = ((A.x**2 + A.y**2) * (C.x - B.x) + (B.x**2 + B.y**2) * (A.x - C.x) + (C.x**2 + C.y**2) * (B.x - A.x)) / D

    centro = Punto(Ux, Uy)
    radio = ((centro.x - A.x)**2 + (centro.y - A.y)**2)**0.5

    return centro, radio


def punto_en_circuncirculo(punto, triangulo):
    centro, radio = get_circumcircle(triangulo)

    if centro is None:
        return False

    distancia = ((punto.x - centro.x)**2 + (punto.y - centro.y)**2)**0.5

    return distancia < radio


def compararSegmentos(seg1: Segmento, seg2: Segmento):
    return (seg1.A == seg2.A and seg1.B == seg2.B) or (seg1.A == seg2.B and seg1.B == seg2.A)


def lados_no_compartidos(lista_triangulos):
    general, repetidos = [], []

    for tri in lista_triangulos:
        for seg in tri.lados:
            found = False
            
            for i in range(len(general)):
                if compararSegmentos(seg, general[i]):
                    repetidos.append(general.pop(i))
                    found = True
                    break
            
            if not found:
                if not any(compararSegmentos(seg, r) for r in repetidos):
                    general.append(seg)
    
    return general


def bowyer_watson(points):
    triangulo_superior = create_triangulo(points)
    triangulos = [triangulo_superior]

    for punto in points:
        lados_a_revisar = []

        for tri in triangulos:
            if punto_en_circuncirculo(punto, tri):
                lados_a_revisar.extend(tri.lados)

        triangulos = [tri for tri in triangulos if not punto_en_circuncirculo(punto, tri)]

        lados_unicos = lados_no_compartidos([Triangulo([seg.A, seg.B, punto]) for seg in lados_a_revisar])

        for lado in lados_unicos:
            nuevo_triangulo = Triangulo([lado.A, lado.B, punto])
            triangulos.append(nuevo_triangulo)

    triangulos_finales = [tri for tri in triangulos if not any(p in triangulo_superior.puntos for p in tri.puntos)]

    return triangulos_finales


def plot_triangulation(triangulos, puntos):
    for tri in triangulos:
        x = [p.x for p in tri.puntos] + [tri.puntos[0].x]
        y = [p.y for p in tri.puntos] + [tri.puntos[0].y]
        plt.plot(x, y, 'b-')

    px = [p.x for p in puntos]
    py = [p.y for p in puntos]
    plt.plot(px, py, 'ro')

    plt.show()


def main():
    n, puntos = lee_archivo("puntos-n10.txt")
    triangulos = bowyer_watson(puntos)
    plot_triangulation(triangulos, puntos)


main()