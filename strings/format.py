# Abre un archivo de texto y quita los acentos de las vocales
# Uso: python format.py archivo.txt
import sys

# Mapeo de vocales acentuadas a no acentuadas
reemplazos = {
	'á': 'a', 'é': 'e', 'í': 'i', 'ó': 'o', 'ú': 'u',
	'Á': 'A', 'É': 'E', 'Í': 'I', 'Ó': 'O', 'Ú': 'U'
}

def quitar_acentos(texto):
	return ''.join(reemplazos.get(c, c) for c in texto)

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("Uso: python format.py archivo.txt")
		sys.exit(1)
	archivo = sys.argv[1]
	with open(archivo, encoding='utf-8') as f:
		contenido = f.read()
	sin_acentos = quitar_acentos(contenido)
	# Sobrescribe el archivo original
	with open(archivo, 'w', encoding='utf-8') as f:
		f.write(sin_acentos)