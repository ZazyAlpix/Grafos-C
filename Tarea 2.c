#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define INFINITO INT_MAX

typedef int **Grafo;
typedef bool *ArregloBool;
typedef int *ArregloInt;

ArregloBool crearArregloBool(int n) {
    ArregloBool a;

    a = calloc(n, sizeof(char));

    if(a == NULL) {
        printf("ERROR: Memoria insuficiente.\n");
        return NULL;
    }

    return a;
}

ArregloInt crearArregloInt(int n) {
    ArregloInt a;

    a = calloc(n, sizeof(int));

    if(a == NULL) {
        printf("ERROR: Memoria insuficiente.\n");
        return NULL;
    }

    return a;
}

Grafo borrarGrafo(Grafo g, int n) {
    int i;

    if(g == NULL) {
        return NULL;
    }

    for(i = 0; i < n; i++) {
        free(g[i]);
    }

    free(g);
    return NULL;
}

Grafo crearGrafo(int n) {
    Grafo g;
    int i, j;

    g = calloc(n, sizeof(int *));

    if(g == NULL) {
        printf("ERROR: Memoria insuficiente.\n");
        return NULL;
    }

    for(i = 0; i < n; i++) {
        g[i] = calloc(n, sizeof(int));

        if(g[i] == NULL) {
            for(j = 0; j < i; j++) {
                free(g[j]);
                free(g);
            }

            printf("ERROR: Memoria insuficiente.\n");
            return NULL;
        }
    }

    return g;
}

Grafo leerArchivo(int *punteroN) {
    char nombreArchivo[50];
    FILE *archivo;
    Grafo g;
    int n, m, i, origen, destino, costo;

    printf("Ingresa el nombre del archivo: ");
    scanf("%s", &nombreArchivo);
    archivo = fopen(nombreArchivo, "r");

    if(archivo == NULL) {
        printf("Archivo no encontrado. Este programa se cerrar%c.", 160);
        exit(EXIT_FAILURE);
    }

    fscanf(archivo, "%i", &n);
    fscanf(archivo, "%i", &m);
    *punteroN = n;
    g = crearGrafo(n);

    if(g == NULL) {
        printf("ERROR: No se pudo asignar memoria. Este programa se cerrar%c.", 160);
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < m; i++) {
        fscanf(archivo, "%i", &origen);
        fscanf(archivo, "%i", &destino);
        fscanf(archivo, "%i", &costo);
        g[origen][destino] = costo;
    }

    fclose(archivo);
    return g;
}

int pedirVerticeInicial(int n) {
    bool valido;
    int v;

    valido = false;

    while(!valido) {
        printf("Ingrese el v%crtice inicial (entre 0 y %d): ", 130, n - 1);

        if(scanf("%d", &v) != 1) {
            printf("Entrada inv%clida.\n", 160);
        } else if(v < 0 || v >= n) {
            printf("Valor fuera de rango.\n");
        } else {
            valido = true;
        }

        while(getchar() != '\n' && getchar() != EOF);
    }

    return v;
}

int pedirOpcionMenu() {
    int opcion = 0;
    int minimo = 1;
    int maximo = 11;
    bool valido = false;

    while(!valido || opcion < minimo || opcion > maximo) {
        printf("\nMen%c de opciones:\n", 163);
        printf(" 1. Mostrar el mapa de la regi%cn (grafo).\n", 162);
        printf(" 2. Ver si el mapa corresponde a un grafo valuado.\n");
        printf(" 3. Ver si el mapa corresponde a un grafo dirigido.\n");
        printf(" 4. Planear ruta usando recorrido en profundidad.\n");
        printf(" 5. Planear ruta usando recorrido en amplitud.\n");
        printf(" 6. Ver si el mapa corresponde a un grafo conexo.\n");
        printf(" 7. Encontrar el camino m%cnimo a locaciones desde un origen.\n", 161);
        printf(" 8. Determinar rutas inexistentes (grafo complemento).\n");
        printf(" 9. Identificar rutas cr%cticas (%crbol de cobertura de costo m%cnimo).\n", 161, 160, 161);
        printf("10. Calcular cantidad m%cxima de Pok%cbolas a transportar.\n", 160, 138);
        printf("11. Eliminar grafos creados y salir.\n\n");
        printf("Ingrese opci%cn (entero entre %d y %d): ", 162, minimo, maximo);

        valido = scanf("%d", &opcion);
        while (getchar() != '\n');

        if(!valido || opcion < minimo || opcion > maximo) {
            printf("Opci%cn no v%clida.\n", 162, 160);
        }
    }

    return opcion;
}

void mostrarGrafo(Grafo g, int n) {
    int i, j;

    if(g == NULL) {
        printf("Grafo no encontrado.\n");
        return;
    }

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            printf("%5d ", g[i][j]);
        }

        printf("\n");
    }
}

bool esValuado(Grafo g, int n) {
    int i, j;

    for(i = 0; i < n; i++) {
        for(j = 1; j < n; j++) {
            if(g[i][j] > 1) {
                return true;
            }
        }
    }

    return false;
}

bool esDirigido(Grafo g, int n) {
    int i, j;

    for(i = 0; i < n; i++) {
        for(j = 1; j < n; j++) {
            if(g[i][j] != g[j][i]) {
                return true;
            }
        }
    }

    return false;
}

void procedimientoProfundidad(Grafo g, int n, int v, ArregloBool visitados) {
    int i;

    visitados[v] = true;
    printf("%i ", v);

    for(i = 0; i < n; i++) {
        if(g[v][i] != 0 && visitados[i] == false) {
            procedimientoProfundidad(g, n, i, visitados);
        }
    }
}

void recorrerProfundidad(Grafo g, int n) {
    ArregloBool visitados;
    int v, i;

    visitados = crearArregloBool(n);

    if(visitados == NULL) {
        printf("ERROR: Memoria insuficiente.\n");
        return;
    }

    v = pedirVerticeInicial(n);
    printf("Recorrido en profundidad desde %i: ", v);
    procedimientoProfundidad(g, n, v, visitados);
    printf("\n\n");
    free(visitados);
}

void procedimientoAmplitud(Grafo g, int n, int v, ArregloBool visitados, bool mostrar) {
    ArregloBool enFila;
    ArregloInt fila;
    int indiceFila, largoFila, i;

    enFila = crearArregloBool(n);
    fila = crearArregloInt(n);

    if(enFila == NULL || fila == NULL) {
        free(enFila);
        free(fila);
        printf("ERROR: Memoria insuficiente.\n");
        return;
    }

    indiceFila = 0;
    largoFila = 0;
    fila[largoFila++] = v;
    enFila[v] = true;

    if(mostrar) {
        printf("Recorrido en amplitud desde %i: ", v);
    }

    while(indiceFila < largoFila) {
        v = fila[indiceFila++];
        enFila[v] = false;
        visitados[v] = true;

        if(mostrar) {
            printf("%i  ", v);
        }

        for(i = 0; i < n; i++) {
            if (g[v][i] != 0 && !visitados[i] && !enFila[i]) {
                fila[largoFila++] = i;
                enFila[i] = true;
            }
        }
    }

    free(fila);
    free(enFila);

    if(mostrar) {
        printf("\n\n");
    }
}

void recorrerAmplitud(Grafo g, int n) {
    int v;
    ArregloBool visitados;

    visitados = crearArregloBool(n);

    if(visitados == NULL) {
        printf("ERROR: Memoria insuficiente.\n");
        return;
    }

    v = pedirVerticeInicial(n);
    procedimientoAmplitud(g, n, v, visitados, true);
    free(visitados);
}

bool esConexo(Grafo g, int n) {
    int v, i;
    ArregloBool visitados;

    if(esDirigido(g, n)) {
        return false;
    }

    visitados = crearArregloBool(n);

    if(visitados == NULL) {
        printf("ERROR: Memoria insuficiente.\n");
        return false;
    }

    procedimientoAmplitud(g, n, 0, visitados, false);

    for(i = 0; i < n; i++) {
        if(!visitados[i]) {
            return false;
        }
    }

    free(visitados);
    return true;
}

int contarSinVisitar(ArregloBool visitados, int n) {
    int i, cantidad;

    cantidad = 0;

    for(i = 0; i < n; i++) {
        if(!visitados[i]) {
            cantidad++;
        }
    }

    return cantidad;
}

int escogerMinimoCostoSinVisitar(ArregloInt costo, ArregloBool visitados, int n) {
    int i, v, minimo;

    minimo = INFINITO;
    v = -1;

    for(i = 0; i < n; i++) {
        if(!visitados[i]) {
            if(costo[i] < minimo) {
                minimo = costo[i];
                v = i;
            }
        }
    }

    return v;
}

void mostrarCamino(ArregloInt predecesor, ArregloInt costo, int v0, int n) {
    ArregloInt camino;
    int v, i, j;

    camino = crearArregloInt(n);

    if(camino == NULL) {
        printf("ERROR: Memoria insuficiente.\n");
        return;
    }

    printf("Camino m%cnimo desde %i:\n", 161, v0);

    for(i = 0; i < n; i++) {
        if(i != v0) {
            if(costo[i] == INFINITO) {
                printf("- De %i a %i (costo: infinito): no existe.\n", v0, i);
            } else {
                camino[0] = i;
                v = predecesor[i];

                for(j = 1; v != v0; j++) {
                    camino[j] = v;
                    v = predecesor[v];
                }

                camino[j] = v;
                printf("- De %i a %i (costo: %i): ", v0, i, costo[i]);

                for(; j >= 0; j--) {
                    printf("%i ", camino[j]);
                }

                printf("\n");
            }
        }
    }

    free(camino);
    printf("\n\n");
}

void mostrarCaminoMinimo(Grafo g, int n) {
    ArregloInt costo, predecesor;
    ArregloBool visitado;
    int v0, v, i;

    visitado = crearArregloBool(n);
    costo = crearArregloInt(n);
    predecesor = crearArregloInt(n);

    if(visitado == NULL || costo == NULL || predecesor == NULL) {
        free(visitado);
        free(costo);
        free(predecesor);
        printf("ERROR: Memoria insuficiente.\n");
        return;
    }

    v0 = pedirVerticeInicial(n);
    visitado[v0] = true;
    predecesor[v0] = -1;
    costo[v0] = 0;

    for(i = 0; i < n; i++) {
        if(i != v0) {
            predecesor[i] = v0;

            if(g[v0][i] != 0) {
                costo[i] = g[v0][i];
            } else {
                costo[i] = INFINITO;
            }
        }
    }

    v = escogerMinimoCostoSinVisitar(costo, visitado, n);

    while(contarSinVisitar(visitado, n) > 1 && v != -1) {
        visitado[v] = true;

        for(i = 0; i < n; i++) {
            if(g[v][i] != 0 && !visitado[i]) {
                if(costo[v] + g[v][i] < costo[i]) {
                    costo[i] = costo[v] + g[v][i];
                    predecesor[i] = v;
                }
            }
        }

        v = escogerMinimoCostoSinVisitar(costo, visitado, n);
    }

    mostrarCamino(predecesor, costo, v0, n);
    free(costo);
    free(predecesor);
    free(visitado);
}

Grafo obtenerComplemento(Grafo g, int n) {
    Grafo complemento;
    int i, j;

    complemento = crearGrafo(n);

    if(complemento == NULL) {
        printf("ERROR: Memoria insuficiente.\n");
        return NULL;
    }

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(i != j) {
                if(g[i][j] == 0) {
                    complemento[i][j] = 1;
                } else {
                    complemento[i][j] = 0;
                }
            }
        }
    }

    return complemento;
}

int obtenerArcoCostoMinimo(ArregloInt costo, ArregloBool visitados, int n) {
    int costoMinimo, indiceMinimo, v;

    costoMinimo = INFINITO;
    indiceMinimo = -1;

    for(v = 0; v < n; v++) {
        if(!visitados[v] && costo[v] < costoMinimo) {
            costoMinimo = costo[v];
            indiceMinimo = v;
        }
    }

    return indiceMinimo;
}

Grafo obtenerArbolCoberturaMinimo(Grafo g, int n) {
    ArregloBool visitados;
    ArregloInt predecesor, costo;
    Grafo arbol;
    int i, u, v, cantidad;

    if(esDirigido(g, n) || !esConexo(g, n)) {
        printf("%crbol de cobertura de costo m%cnimo no definido para grafos dirigidos o desconexos.\n", 181, 161);
        return NULL;
    }

    visitados = crearArregloBool(n);
    predecesor = crearArregloInt(n);
    costo = crearArregloInt(n);
    arbol = crearGrafo(n);

    if(visitados == NULL || predecesor == NULL || costo == NULL || arbol == NULL) {
        printf("ERROR: Memoria insuficiente.\n");
        return NULL;
    }

    for(i = 0; i < n; i++) {
        costo[i] = INT_MAX;
        visitados[i] = false;
    }

    costo[0] = 0;
    predecesor[0] = -1;

    for(cantidad = 0; cantidad < n - 1; cantidad++) {
        u = obtenerArcoCostoMinimo(costo, visitados, n);
        visitados[u] = true;

        for(v = 0; v < n; v++) {
            if(g[u][v] && !visitados[v] && g[u][v] < costo[v]) {
                predecesor[v] = u;
                costo[v] = g[u][v];
            }
        }
    }

    for(i = 1; i < n; i++) {
        arbol[i][predecesor[i]] = g[i][predecesor[i]];
        arbol[predecesor[i]][i] = g[i][predecesor[i]];
    }

    free(visitados);
    free(predecesor);
    free(costo);

    return arbol;
}

int encontrarFuente(Grafo g, int n) {
    int fuente, i, j;
    bool esFuente;

    fuente = -1;

    for(i = 0; i < n; i++) {
        esFuente = true;

        for(j = 0; j < n; j++) {
            if(g[j][i] > 0) {
                esFuente = false;
                break;
            }
        }

        if(esFuente) {
            if(fuente != -1) {
                printf("ERROR: M%cs de una fuente encontrado.\n", 160);
                return -1;
            }

            fuente = i;
        }
    }

    if(fuente == -1) {
        printf("ERROR: No hay un nodo fuente.\n");
    }

    return fuente;
}

int encontrarSumidero(Grafo g, int n) {
    int sumidero, i, j;
    bool esSumidero;

    sumidero = -1;

    for(i = 0; i < n; i++) {
        esSumidero = true;

        for(j = 0; j < n; j++) {
            if(g[i][j] > 0) {
                esSumidero = false;
                break;
            }
        }

        if(esSumidero) {
            if(sumidero != -1) {
                printf("ERROR: M%cs de un nodo sumidero encontrado.\n", 160);
                return -1;
            }

            sumidero = i;
        }
    }

    if(sumidero == -1) {
            printf("ERROR: No hay un sumidero.\n");
    }

    return sumidero;
}

bool buscarCamino(Grafo g, int n, int fuente, int sumidero, ArregloInt predecesor) {
    ArregloBool visitados, enFila;
    ArregloInt fila;
    int indiceFila, largoFila, v, i;

    visitados = crearArregloBool(n);
    enFila = crearArregloBool(n);
    fila = crearArregloInt(n);

    if(visitados == NULL || enFila == NULL || fila == NULL) {
        free(visitados);
        free(enFila);
        free(fila);
        printf("ERROR: Memoria insuficiente.\n");
        return false;
    }

    indiceFila = 0;
    largoFila = 0;
    fila[largoFila++] = fuente;
    enFila[fuente] = true;
    predecesor[fuente] = -1;

    while(indiceFila < largoFila) {
        v = fila[indiceFila++];
        enFila[v] = false;
        visitados[v] = true;

        for(i = 0; i < n; i++) {
            if(g[v][i] > 0 && !visitados[i] && !enFila[i]) {
                fila[largoFila++] = i;
                enFila[i] = true;
                predecesor[i] = v;

                if(i == sumidero) {
                    free(visitados);
                    free(enFila);
                    free(fila);
                    return true;
                }
            }
        }
    }

    free(visitados);
    free(enFila);
    free(fila);
    return false;
}

void mostrarFlujoMaximo(Grafo g, int n) {
    ArregloInt predecesor;
    int fuente, sumidero, flujoMaximo, flujoCamino, u, v;
    Grafo residual;

    if(!esDirigido(g, n)) {
        printf("ERROR: El grafo debe ser dirigido.\n");
        return;
    }

    fuente = encontrarFuente(g, n);
    sumidero = encontrarSumidero(g, n);

    if(fuente == -1 || sumidero == -1) {
        printf("ERROR: El grafo debe tener solo una fuente y un %cnico sumidero.\n", 163);
        return;
    }

    residual = crearGrafo(n);

    if(residual == NULL) {
        printf("ERROR: Memoria insuficiente.\n");
        return;
    }

    for(u = 0; u < n; u++) {
        for(v = 0; v < n; v++) {
            residual[u][v] = g[u][v];
        }
    }

    predecesor = crearArregloInt(n);

    if(predecesor == NULL) {
        printf("ERROR: Memoria insuficiente.\n");
        borrarGrafo(residual, n);
        return;
    }

    flujoMaximo = 0;

    while(buscarCamino(residual, n, fuente, sumidero, predecesor)) {
        flujoCamino = INFINITO;

        for(v = sumidero; v != fuente; v = predecesor[v]) {
            u = predecesor[v];

            if(residual[u][v] < flujoCamino)
                flujoCamino = residual[u][v];
        }

        for(v = sumidero; v != fuente; v = predecesor[v]) {
            u = predecesor[v];
            residual[u][v] -= flujoCamino;
            residual[v][u] += flujoCamino;
        }

        flujoMaximo += flujoCamino;
    }

    printf("Flujo m%cximo: %d\n", 160, flujoMaximo);
    borrarGrafo(residual, n);
    free(predecesor);
}

int main() {
    int n, opcion;
    Grafo g, h;
    bool bandera;

    g = leerArchivo(&n);

    while(opcion != 11) {
        opcion = pedirOpcionMenu();

        switch(opcion) {
            case 1:
                // Mostrar grafo.
                mostrarGrafo(g, n);
                break;
            case 2:
                // Mostrar si el grafo es valuado.
                bandera = esValuado(g, n);

                if(bandera) {
                    printf("Grafo valuado.\n");
                } else {
                    printf("Grafo no valuado.\n");
                }

                break;
            case 3:
                // Mostrar si el grafo es dirigido.
                bandera = esDirigido(g, n);

                if(bandera) {
                    printf("Grafo dirigido.\n");
                } else {
                    printf("Grafo no dirigido.\n");
                }

                break;
            case 4:
                // Recorrido en profundidad.
                recorrerProfundidad(g, n);
                break;
            case 5:
                // Recorrido en amplitud.
                recorrerAmplitud(g, n);
                break;
            case 6:
                // Ver si el grafo es conexo.
                // Mostrar si el grafo es dirigido.
                bandera = esConexo(g, n);

                if(bandera) {
                    printf("Grafo conexo.\n");
                } else {
                    printf("Grafo desconexo.\n");
                }

                break;
            case 7:
                // Camino mínimo.
                mostrarCaminoMinimo(g, n);
                break;
            case 8:
                // Grafo complemento.
                h = obtenerComplemento(g, n);
                mostrarGrafo(h, n);
                h = borrarGrafo(h, n);
                break;
            case 9:
                // Árbol de cobertura de costo mínimo.
                h = obtenerArbolCoberturaMinimo(g, n);
                mostrarGrafo(h, n);
                h = borrarGrafo(h, n);
                break;
            case 10:
                // Flujo máximo.
                mostrarFlujoMaximo(g, n);
                break;
            default:
                break;
        }
    }

    g = borrarGrafo(g, n);
    return 0;
}
