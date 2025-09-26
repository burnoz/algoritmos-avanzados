// Ordenamiento topológico
void topologicalSort(int N, int **matrix) {
	int *inDegree = new int[N]();
	for (int u = 0; u < N; u++) {
		for (int v = 0; v < N; v++) {
			if (matrix[u][v] == 1) {
				inDegree[v]++;
			}
		}
	}

	Queue queue(N);
	for (int i = 0; i < N; i++) {
		if (inDegree[i] == 0) {
			queue.enqueue(i);
		}
	}

	int count = 0;
	int *order = new int[N];

	while (!queue.isEmpty()) {
		int u = queue.dequeue();
		order[count++] = u;

		for (int v = 0; v < N; v++) {
			if (matrix[u][v] == 1) {
				inDegree[v]--;
				if (inDegree[v] == 0) {
					queue.enqueue(v);
				}
			}
		}
	}

	if (count != N) {
		cout << "El grafo tiene ciclos, no se puede ordenar topológicamente." << endl;
	} 
	
	else{
		cout << "Orden topologico: ";
		for (int i = 0; i < N; i++) {
			cout << order[i] << " ";
		}
		cout << endl;
	}
}