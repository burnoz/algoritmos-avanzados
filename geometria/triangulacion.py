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


def show_triangle(triangulo):
	print(triangulo.puntos[0].show())
	print(triangulo.puntos[1].show())
	print(triangulo.puntos[2].show())


def plot_everything(triangulo, puntos):
	plt.figure()

	# triangulo
	xs = [triangulo.puntos[i].x for i in range(3)] + [triangulo.puntos[0].x]
	ys = [triangulo.puntos[i].y for i in range(3)] + [triangulo.puntos[0].y]
	plt.plot(xs, ys, 'b-')

	# puntos
	pxs = [p.x for p in puntos]
	pys = [p.y for p in puntos]
	plt.plot(pxs, pys, 'ro')

	plt.show()


base_name = "puntos-n"
nums = [10, 11, 15, 16, 20, 50, 100]

for n in nums:
	n, puntos = lee_archivo(f"{base_name}{n}.txt")
	triangulo = create_triangulo(puntos)

	# show_triangle(triangulo)
	plot_everything(triangulo, puntos)
