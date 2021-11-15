#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMA_LINEA                                                           \
	150 // Al leer el archivo el maximo (exagerando) de informacion que puede
// tener cada arista es la informacion de 3 vertices (50 caracteres por
// vetice)

typedef struct sEdge {
	char name[50];
	long id;
	int length;
	struct sEdge *next;
} tEdge;

typedef struct sVertex {
	char name[50];
	long id;
	tEdge *firstEdge;
	struct sVertex *next;
} tVertex;

// Reserva un espacio en memoria
void *chkMalloc(size_t t) {
	void *mem = malloc(
			t); // Intenta reservar un espacio de memoria del tamaño del parametro t
	if (mem == NULL) { // Si el bloque no pudo reservarse, terminar el programa y
		// notificar
		printf("Sin memoria\n");
		exit(1);
	}
	return mem; // regresar el puntero al espacio de memoria reservado
}

// Agrega un vertice al grafo
void addVertex(tVertex **first, char *name, int *numVertices) {
	tVertex *cur = *first;
	while (cur != NULL) { // Si la la lista de adyacencia no esta vacia
		if (strcmp(cur->name, name) ==
				0) {  // Buscar el vertice que tiene por identificador el parametro name
			return; // Si existe el vertice, no agregarlo de nuevo
		}
		cur = cur->next; // Recorrer la lista
	}

	int j = 0, k, n = 0; // indices auxiliares
	int igual = 0;
	char *tab = "\t"; // tabulacion
	char nuevo[51];   // un nombre de 50 caracteres es lo maximo que soporta un
	// vertice

	for (int i = 0; name[i] != '\0';
			i++) { // recorrer la cadena hasta que no haya mas caracteres
		k = i;    // guardar comienzo de la cadena interior

		while (
				name[i] ==
				tab[j]) { // Comparar caracter por caracter que las cadenas sean iguales
			i++;
			j++;
			if (j == strlen(tab)) { // Si ya se ha recorrido toda la cadena interior
				igual = 1;            // Marcar que son iguales
				break;
			}
		}
		j = 0;

		if (igual == 0) { // Si no son iguales
			i = k;          // Regresar a antes de comenzar a comparar
		} else {
			igual = 0; // Si son iguales, devolver la variable a su estado original
		}

		nuevo[n++] = name[i]; // Reintroducir los caracteres a la nueva variable sin
		// la tabulacion
	}
	nuevo[n] = '\0'; // Agregar fin de cadena

	tVertex *newest = chkMalloc(
			sizeof(*newest)); // Reserva un espacio en memoria para un struct sVertex
	strcpy(newest->name, nuevo); // Le asigna el identificador del parametro (sin
	// tabulaciones) al campo name
	newest->next = *first; // El campo next apunta al primer vertice actual del
	// grafo en la lista de adyacencia
	newest->id = (*numVertices)++; // Segun el numero actual de vertices en el
	// grafo, el campo id es uno mas ese numero
	*first = newest; // la lista de adyacencia comienza con el nuevo vertice
}

// Agrega una arista al grafo
void addVertexEdge(tVertex *first, char *name_a, char *name_b, int length_b) {
	tVertex *selected, *pointed; // Vertices de ayuda
	tEdge *newest =
		chkMalloc(sizeof(*newest)); // Se reserva un espacio en la memoria para un
	// struct de tipo sEdge
	while (first != NULL) {         // Si el grafo tiene vertices
		if (strcmp(first->name, name_a) == 0) { // Buscar el vertice que tiene por
			// identificador el parametro name_a
			selected = first; // Asignar ese vertice a la variable selected
		} else if (strcmp(first->name, name_b) ==
				0) {    // Buscar el vertice que tiene por identificador el
			// parametro name_a
			pointed = first; // Asignar ese vertice a la variable pointed
		}
		first = first->next; // Avanzar en la lista
	}

	tEdge *currEdge =
		selected->firstEdge; // Se toma la primer arista del vertice con el
	// identificador del parametro name_a
	int i = 0;
	while (currEdge != NULL) {
		if (strcmp(currEdge->name, name_b) ==
				0) // Se recorre la lista de adyacencia del vertice selected para ver si
			// ya tiene el vertice pointed agregado
			i++; // Si esta, aumentar i
		currEdge = currEdge->next; // Avanzar en la lista
	}

	if (selected->firstEdge == NULL ||
			i == 0) { // Si no tiene vertices adyacentes, o el vertice pointed no esta
		// en su lista
		strcpy(newest->name, name_b); // A la nueva arista se le asigna el
		// identificador del vertice al que apunta
		newest->next =
			selected->firstEdge;   // Se coloca por el frente, apunta al actual
		// comienzo de la lista de adyacencia
		newest->length = length_b; // Se asigna la ponderacion de la arista
		newest->id = pointed->id;  // Se le asigna el identificador entero del
		// vertice al que apunta
		selected->firstEdge =
			newest; // El comienzo de la lista es ahora el nuevo vertice
	}
}

// Funcion de ayuda para crear aristas en grafos dirigidos o no dirigidos, segun
// el parametro dir
void addEdge(tVertex *first, char *name_a, char *name_b, int length_b,
		int dir) {
	if (dir ==
			0) { // Si dir es 0, la aristaen cuestion es no dirigida, y se crea doble
		addVertexEdge(first, name_a, name_b, length_b);
		addVertexEdge(first, name_b, name_a, length_b);
	} else if (dir > 0) { // Si dir es mayor a 0, la arista apunta del vertice
		// name_a a name_b
		addVertexEdge(first, name_a, name_b, length_b);
	} else if (dir < 0) { // Si dir es menor a 0, la arista apunta del vertice
		// name_b a name_a
		addVertexEdge(first, name_b, name_a, length_b);
	}
}

// Elimina una arista del grafo, dada una lista de adyacencia anidada
void delVertexEdge(tEdge **first, char *name) {
	tEdge *cur = *first, *prev = NULL; // Vertices de ayuda
	while (cur != NULL && strcmp(cur->name, name) !=
			0) { // Se recorre la lista buscando un vertice que
		// tenga el nombre del parametro name
		prev = cur;                  // prev se asigna al vertice actual
		cur = cur->next;             // cur se asigna al vertice siguiente
	}

	if (cur) {                  // Si cur existe
		if (prev) {               // Si prev existe
			prev->next = cur->next; // prev apunta al siguiente de cur
		} else {
			*first = cur->next; // Si no, es el comienzo de la lista y esta apunta
			// directamente al siguiente de cur
		}
	}
}

// Eliminar Vertice
void delVertex(tVertex **first, char *name) {
	tVertex *cur = *first, *prev = NULL; // Vertices de ayuda
	while (cur != NULL && strcmp(cur->name, name) !=
			0) { // Se recorre la lista buscando un vertice que
		// tenga el nombre del parametro name
		prev = cur;                  // prev se asigna al vertice actual
		cur = cur->next;             // cur se asigna al vertice siguiente
	}
	if (prev) {               // Si prev existe
		prev->next = cur->next; // prev apunta al siguiente de cur
	} else {
		*first = cur->next; // Si no, es el comienzo de la lista y esta apunta
		// directamente al siguiente de cur
	}

	cur = *first;         // cur apunta de nuevo al comienzo de la lista
	while (cur != NULL) { // Si la la lista de adyacencia no esta vacia
		delVertexEdge(&(cur->firstEdge),
				name); // Eliminar aristas, si existen, que conecten con el
		// vertice con el identificador name en todos los
		// vertices de la lista de adyacencia
		cur = cur->next;     // Recorrer la lista
	}
}

// Funcion de ayuda para eliminar arista, dado todo el grafo
void delEdge(tVertex **first, char *name_a, char *name_b) {
	tVertex *cur = *first; // Cur es el comienzo de la lista
	while (cur != NULL) {  // Recorrer la lista
		if (strcmp(cur->name, name_a) ==
				0) { // Si cur tiene el identificador de name_a
			delVertexEdge(&(cur->firstEdge),
					name_b); // Eliminar la conexion con el vertice con name_b
			// en esa lista de aristas
		}
		if (strcmp(cur->name, name_b) ==
				0) { // Si cur tiene el identificadro de name_b
			delVertexEdge(&(cur->firstEdge),
					name_a); // Eliminar la conexion con el vertice con name_a
			// en esa lista de aristas
		}
		cur = cur->next; // Avanzar en la lista
	}
}

// Imprimir vertices y sus aristas
void dumpDetails(tVertex *currVertex) {
	while (currVertex != NULL) { // Mientras la lista tenga elementos
		printf("%s(%lu) tiene:\n", currVertex->name,
				currVertex->id); // Imprimir el vertice

		tEdge *currEdge =
			currVertex->firstEdge;   // Comenzar al inicio de la lista de aristas
		if (currEdge != NULL) {      // Si hay aristas
			while (currEdge != NULL) { // Mientras haya aristas
				printf("\t[%s(%lu), %d]\n", currEdge->name, currEdge->id,
						currEdge->length);  // Imprimir las aristas
				currEdge = currEdge->next; // Avanzar en la lista de aristas
			}
		}
		currVertex = currVertex->next; // Avanzar en la lista de vertices
	}
}

// Imprimir representacion ASCII del grafo
void printGraph(tVertex *currVertex, int *numVertices) {
	int selected[*numVertices]; // Lista de vertices visitados
	char vertices[*numVertices][50],
	espacios[*numVertices]; // lista de nombres de vertices y de espaciado
	// entre ellos
	int x, y;

	memset(selected, 0,
			sizeof(selected)); // marcar todos los vertices como no visitados
	for (int i = 0; i < *numVertices; i++) {
		espacios[i] = ' '; // agregar un espacio por cada vertice en el grafo
	}

	while (currVertex != NULL) {               // Recorrer lista de vertices
		tEdge *currEdge = currVertex->firstEdge; // Comenzar en la primer arista
		if (currEdge != NULL) {                  // Si hay aristas
			while (currEdge != NULL) {             // Recorrer lista de aristas
				if (!selected[currEdge->id]) { // Si no se ha visitado el vertice al que
					// apunta
					if (currVertex->id >
							currEdge->id) { // Si el indice del vertice es menor al del
						// vertice al que conecta
						x = currEdge->id; // El primero en imprimirse sera la el vertice al
						// que conecta
						y = currVertex->id; // El segundo en imprimirse es el vertice actual
					} else {
						x = currVertex
							->id; // De lo contrario, el primero es el vertice actual
						y = currEdge->id; // Evidentemente, el otro sera el segundo
					}

					for (int i = 0; i < x; i++) { // Mientras sea i menor al primer indice
						// del primer vertice
						printf("%c ", espacios[i]); // Imprimir un espacio de la lista
					}
					printf("+"); // Imprimir una conexion de arista
					for (int i = 0; i < y - x - 1;
							i++) { // Por cada numero en la diferencia de posicion del primer
						// y segundo elemento en imprimir
						printf("--"); // Imprimir una linea que conecte las aristas
					}
					printf("-+ "); // Terminar la conexion
					for (int i = y + 1; i < *numVertices;
							i++) { // Desde el ultimo elemento en imprimir hasta el numero de
						// vertices
						printf("%c ", espacios[i]); // Imprimir un espacio
					}
					printf("\n");

					// Poner una linea que conecte con el elemento en su posicion
					espacios[x] = '|';
					espacios[y] = '|';

					for (int i = 0; i < *numVertices;
							i++) {                   // Por cada vertice en el grafo
						printf("%c ", espacios[i]); // Imprimir espacio
					}

					printf("\n");
				}
				currEdge = currEdge->next; // Pasar a la siguiente arista
			}
		}
		strcpy(vertices[currVertex->id],
				currVertex->name);     // Copiar nombre del vertice actual a la lista
		selected[currVertex->id] = 1; // Marcar vertice como visitado

		currVertex = currVertex->next; // Pasar al siguiente vertice
	}
	printf("\n");

	for (int i = 0; i < *numVertices; i++) { // Por cada vertice
		if (selected[i]) {                     // Si fue visitado
			printf("%d ", i); // Imprimir su indice debajo de su arista
		} else {
			printf("  "); // De lo contrario, omitirlo, pues ha sido eliminado
		}
	}
	printf("\n\n");

	for (int i = 0; i < *numVertices; i++) { // por cada vertice
		if (selected[i]) {                     // Si fue visitado
			printf("(%d) %s\n", i,
					vertices[i]); // Imprimir la leyenda que relaciona su indice con su
			// nombre legible a humanos (el campo name)
		}
	}
	printf("\n");
}

void imprimirCamino(int parent[], int j, char nombres[][50]) {
	if (parent[j] == -1) { // Caso base, si el vertice con indice j es el comienzo
		printf("->(%s)", nombres[j]); // Imprimir el nombre del vertice
		return;                       // terminar la funcion
	}

	imprimirCamino(
			parent, parent[j],
			nombres); // Llamada recursiva con el vertice que antecede al acutal

	printf("->(%s)", nombres[j]); // Imprimir el nombre del vertice
}

float consumoGasolinaItalicaFT125(float distancia) { return distancia / 38000; }

float tiempoEstimadoItalicaFT125(float distancia) { return distancia / 1000; }

// Algoritmo de Dijkstra
void dijkstraAlg(tVertex *currVertex, char *pivot, int *numVertices, int unico,
		char *nombre) {
	tVertex *firstVertex = currVertex; // Vertice de ayuda
	int numEdge = 0, distances[*numVertices], selected[*numVertices],
	caminos[*numVertices]; // indice de vertices, lista de distancias, listade
	// vertices visitados, lista de vertices
	// antecesores a cada vertice
	char nombres[*numVertices][50]; // nombre de cada vertice

	memset(selected, 0,
			sizeof(selected)); // Marcar todos los vertice como no visitados
	for (int i = 0; i < *numVertices; i++) { // Por cada vertice
		distances[i] = INT_MAX; // Marcar su distancia como la distancia maxima
		caminos[i] = -1; // Marcar todos los vertices como que son sus propios
		// antecesores (cambia adelante)
	}

	while (currVertex != NULL) { // Recorrer lista de vertices
		if (strcmp(currVertex->name, pivot) ==
				0) { // Si el vertice actual es igual a nuestra fuente del parametro
			// pivot
			distances[currVertex->id] = 0; // Marcar la distancia a si mismo como 0
			selected[currVertex->id] = 1;  // Marcarlo como visitado
		}
		strcpy(nombres[currVertex->id],
				currVertex->name);      // Copiar su nombre a la lista de nombres
		currVertex = currVertex->next; // Avanzar
	}

	while (numEdge < *numVertices) {    // Por cada vertice en el grafo
		currVertex = firstVertex;         // Comenzar en el primer vertice
		while (currVertex != NULL) {      // Recorrer lista de vertices
			if (selected[currVertex->id]) { // Si el vertice ha sido seleccionado
				tEdge *currEdge =
					currVertex->firstEdge; // Comenzar por la primer arista
				while (currEdge != NULL) { // Recorrer lista de aristas
					if (distances[currEdge->id] > currEdge->length &&
							!selected[currEdge->id]) { // Si la distancia guardada al vertice
						// que apunta es menor a la de la
						// arista acutal y el vertice apuntado
						// no ha sido visitado
						distances[currEdge->id] =
							distances[currVertex->id] +
							currEdge->length; // Actualizar la distancia guardada por la
						// suma de la distancia del vertice actual mas
						// la ponderacion de la arista actual
						caminos[currEdge->id] =
							currVertex->id; // Agregar el vertice actual como el antecesor
						// del vertice al que apunta
					}
					selected[currEdge->id] =
						1;                     // Marcar el vertice apuntado como visitado
					currEdge = currEdge->next; // Avanzar en la lista de aristas
				}
			}
			currVertex = currVertex->next; // Avanzar en la lista de vertices
		}
		numEdge++; // Siguiente pasada
	}

	printf("\n** Caminos más cortos **\n\n");
	for (int i = 0; i < *numVertices; i++) { // Por cada vertice
		if ((unico && strcmp(nombres[i], nombre) == 0) || (!unico)) {
			if (strcmp(nombres[i], "Tacos Chava") == 0)
				continue;
			printf("Camino [");
			imprimirCamino(caminos, i, nombres); // Imprimir camino más corto
			printf("]: -> %d m \n\ta) Consumo de gasolina estimado: %0.3f L\n\tb) "
					"Tiempo de llegada estimado: %0.1f min\n\n",
					distances[i], consumoGasolinaItalicaFT125(distances[i]),
					tiempoEstimadoItalicaFT125(
						distances[i])); // Imprimir distancia del camino
		}
	}
	printf("\n");
}

// Leer un numero de entrada estandar y guardarlo en la variable del puntero
// input, con un tipo segun el primer argumento
void getNumero(int entero, void *input) {
	int length, i = 0;
	char aux[' '];

	do {
		// Obtener valor del usuario con fgets para prevenir desbordamiento
		fgets(aux, ' ', stdin);
		// Eliminar el salto de linea
		aux[strcspn(aux, "\n")] = 0;
		length = strlen(aux);
		if (aux[0] == 'N')
			exit(0);
		if (length != 0) {
			// Recorrer cada caracter de la cadena
			for (i = 0; i < length; ++i) {
				// Si algun caracter no es numero, un punto o el primero un signo de
				// negativo
				if ((!isdigit(aux[i]) && aux[i] != '.') || (i == 0 && aux[i] == '-')) {
					// Romper ciclo para reiniciar y recibir otro valor
					printf("Introduzca correctamente el dato -> ");
					break;
				}
			}
		} else {
			// Forzar la condicion de terminacion del ciclo
			i = length;
		}
	} while (i != length); // Cuando se haya recorrido toda la cadena, salir

	// Si el primer parametro es 0, retornar como flotante
	if (entero == 0) {
		*(float *)input = atof(aux);
		// Si el primer parametro es 2, retornar como long
	} else if (entero == 2) {
		*(long *)input = atol(aux);
		// Si el primer parametro es diferente a lo anterior, retornar como entero
	} else {
		*(int *)input = atoi(aux);
	}
}

void instrucciones() {
	printf("\n\t\t ### SISTEMA TACOS CHAVA ### \n");
	printf("\n\t\t\t ¿Qué opcion desea? :\n"
			"   \t\t 1 agregar colonia\n"
			"   \t\t 2 agregar camino\n"
			"   \t\t 3 eliminar colonia\n"
			"   \t\t 4 eliminar camino\n"
			"   \t\t 5 imprimir mapa\n"
			"   \t\t 6 Encontrar caminos mas cortos a todas las colonia\n"
			"   \t\t 7 Encontrar camino mas corto a una colonia\n"
			"   \t\t 0 salir\n");
}

void cargarGrafo(char *dir, tVertex **grafo, int *numVertices) {
	char line[MAXIMA_LINEA] = {0}, *vertex_a, *vertex_b, *num;
	int dist;

	FILE *file = fopen(dir, "r"); // Abrir archivo

	if (!file) {
		printf("# ERROR: No existe el registro del mapa."); // Imprimir error si no
		// existe el archivo
		return;
	}

	while (fgets(line, MAXIMA_LINEA, file)) { // Recorrer todas las lineas
		// Cortar los campos
		vertex_a = strtok(line, "\t");
		vertex_b = strtok(NULL, "\t");
		num = strtok(NULL, "");
		num[strcspn(num, "\n")] = 0;
		dist = atoi(num);

		// Agregar vertices y aristas al grafo por cada linea
		addVertex(*&grafo, vertex_a, &*numVertices);
		addVertex(*&grafo, vertex_b, &*numVertices);
		addEdge(*grafo, vertex_a, vertex_b, dist, 0);
	}

	if (fclose(file)) { // Cerrrar el archivo
		printf(
				"# ERROR: No puedo cerrarse el registro del mapa."); // Imprimir error,
		// si no se pudo
		// cerrar
		exit(1);
	}
}

void guardarGrafo(char *dir, tVertex *grafo) {
	FILE *file = fopen(dir, "w"); // Abrir archivo

	if (!file) {
		printf(
				"# ERROR: No pudo crearse registro del mapa."); // Imprimir error si no
		// existe el archivo
		exit(1);
	}

	while (grafo != NULL) {               // Recorrer la lista
		tEdge *currEdge = grafo->firstEdge; // Comenzar por la primer arista
		while (currEdge != NULL) {          // Recorrer lista de aristas

			// Imprimir las especificaciones de cada arista al archivo
			fprintf(file, "%s\t%s\t%d\n", grafo->name, currEdge->name,
					currEdge->length);

			currEdge = currEdge->next; // Avanzar en la lista de aristas
		}

		grafo = grafo->next; // Avanzar en la lista
	}

	if (fclose(file)) { // Cerrrar el archivo
		printf(
				"# ERROR: No puedo cerrarse el registro del mapa."); // Imprimir error,
		// si no se pudo
		// cerrar
		exit(1);
	}
}

int main(void) {
	tVertex *firstVertex = NULL;
	int firstGraphVertices = 0;

	cargarGrafo("mapa.txt", &firstVertex, &firstGraphVertices);

	// ############# PIA #############
	/*addVertex(&firstVertex, "Tacos Chava", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "Felipe Carrillo Puerto", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "Nexxus", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "San Genaro", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "Celestino Gasca", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "Ricardo Flores Magon", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "Sector Jardines", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "Infonavit Monterreal", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "Privadas El Sauce", &firstGraphVertices);*/
	/*addVertex(&firstVertex, "La Loma", &firstGraphVertices);*/

	/*addEdge(firstVertex, "Tacos Chava", "Infonavit Monterreal", 1800, 0);*/
	/*addEdge(firstVertex, "Tacos Chava", "Nexxus", 3100, 0);*/
	/*addEdge(firstVertex, "Tacos Chava", "Ricardo Flores Magon", 3400, 0);*/
	/*addEdge(firstVertex, "Tacos Chava", "Privadas El Sauce", 4100, 0);*/
	/*addEdge(firstVertex, "Tacos Chava", "Felipe Carrillo Puerto", 3100, 0);*/
	/*addEdge(firstVertex, "Tacos Chava", "La Loma", 2000, 0);*/

	/*addEdge(firstVertex, "Ricardo Flores Magon", "Infonavit Monterreal", 1600,
	 * 0);*/
	/*addEdge(firstVertex, "Ricardo Flores Magon", "Sector Jardines", 1600, 0);*/

	/*addEdge(firstVertex, "Felipe Carrillo Puerto", "Celestino Gasca", 1400,
	 * 0);*/
	/*addEdge(firstVertex, "Felipe Carrillo Puerto", "Nexxus", 2800, 0);*/
	/*addEdge(firstVertex, "Felipe Carrillo Puerto", "San Genaro", 4700, 0);*/

	/*addEdge(firstVertex, "Privadas El Sauce", "Ricardo Flores Magon", 4400,
	 * 0);*/

	/*addEdge(firstVertex, "Infonavit Monterreal", "Felipe Carrillo Puerto", 2000,
	 * 0);*/

	/*addEdge(firstVertex, "Nexxus", "Celestino Gasca", 1500, 0);*/

	/*addEdge(firstVertex, "Celestino Gasca", "San Genaro", 4600, 0);*/
	// ##################################################

	int op, peso;
	char nombre[50], nombre_b[50];

	do {

		tVertex *minGraph = NULL;

		instrucciones();
		printf("\nOpción -> ");
		getNumero(1, &op);
		switch (op) {
			case 1:
				printf("\n*** Agrega una colonia ***\n");

				printf("Nombre de la colonia -> ");
				fgets(nombre, 50, stdin);
				nombre[strcspn(nombre, "\n")] = 0;

				addVertex(&firstVertex, nombre, &firstGraphVertices);
				break;
			case 2:
				printf("\n*** Agrega un camino ***\n");

				printf("Nombre del primer ubicacion -> ");
				fgets(nombre, 50, stdin);
				nombre[strcspn(nombre, "\n")] = 0;

				printf("Nombre de la segunda ubicacion -> ");
				fgets(nombre_b, 50, stdin);
				nombre_b[strcspn(nombre_b, "\n")] = 0;

				printf("Inserte longitud del camino -> ");
				getNumero(1, &peso);

				addEdge(firstVertex, nombre, nombre_b, peso, 0);
				break;
			case 3:
				printf("\n*** Elimina una colonia ***\n");

				printf("Nombre de la colonia -> ");
				fgets(nombre, 50, stdin);
				nombre[strcspn(nombre, "\n")] = 0;

				delVertex(&firstVertex, nombre);

				break;
			case 4:
				printf("\n*** Elimina camino ***\n");

				printf("Nombre de la primer ubicacion -> ");
				fgets(nombre, 50, stdin);
				nombre[strcspn(nombre, "\n")] = 0;

				printf("Nombre de la segunda ubicacion -> ");
				fgets(nombre_b, 50, stdin);
				nombre_b[strcspn(nombre_b, "\n")] = 0;

				delEdge(&firstVertex, nombre, nombre_b);

				break;
			case 5:
				printf("\n*** Imprime mapa ***\n");

				dumpDetails(firstVertex);
				printf("\n");
				printGraph(firstVertex, &firstGraphVertices);

				break;
			case 6:
				printf("\n*** Encontrar caminos mas cortos a cada colonia ***\n");

				dijkstraAlg(firstVertex, "Tacos Chava", &firstGraphVertices, 0, NULL);

				break;
			case 7:
				printf("\n*** Encontrar camino mas corto a una colonia ***\n");

				printf("Nombre de la colonia -> ");
				fgets(nombre, 50, stdin);
				nombre[strcspn(nombre, "\n")] = 0;

				dijkstraAlg(firstVertex, "Tacos Chava", &firstGraphVertices, 1, nombre);

				break;
		}
	} while (op != 0);

	guardarGrafo("mapa.txt", firstVertex);

	return 0;
}
