#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOMBRE 50
#define MAX_NOMBRE_CORTO 20

#define UMBRAL_RETENIDO 40
#define UMBRAL_RECHAZADO 75

#define ARCHIVO_ESPECIES   "especies.txt"
#define ARCHIVO_PLANETAS   "planetas.txt"
#define ARCHIVO_OBJETOS    "objetos.txt"
#define ARCHIVO_REGLAS     "reglas.txt"
#define ARCHIVO_VIAJEROS   "viajeros.txt"
#define ARCHIVO_INCIDENTES "incidentes.txt"

typedef enum {
    AUTORIZADO = 0,
    RETENIDO   = 1,
    RECHAZADO  = 2
} Decision;

typedef enum {
    MOTIVO_NINGUNO = 0,
    MOTIVO_OBJETO_PROHIBIDO,
    MOTIVO_CONTRABANDO,
    MOTIVO_CONFLICTO_EQUIPAJE,
    MOTIVO_RIESGO_ALTO
} MotivoIncidente;

typedef struct {
    char nombre[50];
    int id;
    int nvPeligro;
} Especie;

typedef struct {
    char nombre[20];
    int id;
    int nvRiesgo;
} Planeta;

typedef struct {
    char nombre[20];
    int id;
    int nvPeligro;
    int idContrabando;
} Objeto;

typedef struct {
    int idObjeto;
    int idEspecie;
    int permitido;
} Regla;

typedef struct {
    char nombre[30];
    int id;
    Especie *especie;
    Planeta *planeta;
    Objeto **equipaje;
    int numObjetos;
    int capacidad;
    int puntajeRiesgo;
    int desicion;
} Viajero;

typedef struct {
    char desicion[50];
    int id;
    int motivo;
    time_t fechaHora;
    int fecha;
    int hora;
    Viajero *viajero;
    Objeto *objeto;
} Incidente;

typedef struct {
    Especie *especies;
    int cantEspecies;
    int capEspecies;
    Planeta *planetas;
    int cantPlanetas;
    int capPlanetas;
    Objeto *objetos;
    int cantObjetos;
    int capObjetos;
    Regla *reglas;
    int cantRegla;
    int capRegla;
    Viajero *viajeros;
    int cantViajeros;
    int capViajeros;
    Incidente *incidentes;
    int cantIncidentes;
    int capIncidentes;
} Aduana;

int crecerArreglo(void **arreglo, int *capacidad, size_t tamElemento) {
    int nuevaCapacidad = (*capacidad) * 2;
    void *temporal = realloc(*arreglo, (size_t)nuevaCapacidad * tamElemento);
    if (temporal == NULL) {
        return 0;
    }
    *arreglo = temporal;
    *capacidad = nuevaCapacidad;
    return 1;
}

Viajero *crearViajero(int capacidadInicial) {
    if (capacidadInicial <= 0) {
        return NULL;
    }

    Viajero *nuevo = malloc(sizeof(Viajero));
    if (nuevo == NULL) {
        return NULL;
    }

    nuevo->equipaje = malloc((size_t)capacidadInicial * sizeof(Objeto *));
    if (nuevo->equipaje == NULL) {
        free(nuevo);
        return NULL;
    }

    nuevo->numObjetos = 0;
    nuevo->capacidad = capacidadInicial;
    nuevo->puntajeRiesgo = 0;
    nuevo->desicion = 0;

    nuevo->especie = NULL;
    nuevo->planeta = NULL;

    return nuevo;
}

void liberarViajero(Viajero *v) {
    if (v == NULL) {
        return;
    }

    free(v->equipaje);
    v->equipaje = NULL;

    free(v);
}

int crearAduana(Aduana *j, int capInicial) {
    if (j == NULL || capInicial <= 0) {
        return 0;
    }
    j->cantEspecies = 0;
    j->capEspecies = capInicial;
    j->especies = malloc((size_t)capInicial * sizeof(Especie));

    j->cantPlanetas = 0;
    j->capPlanetas = capInicial;
    j->planetas = malloc((size_t)capInicial * sizeof(Planeta));

    j->cantObjetos = 0;
    j->capObjetos = capInicial;
    j->objetos = malloc((size_t)capInicial * sizeof(Objeto));

    j->cantRegla = 0;
    j->capRegla = capInicial;
    j->reglas = malloc((size_t)capInicial * sizeof(Regla));

    j->cantViajeros = 0;
    j->capViajeros = capInicial;
    j->viajeros = malloc((size_t)capInicial * sizeof(Viajero));

    j->cantIncidentes = 0;
    j->capIncidentes = capInicial;
    j->incidentes = malloc((size_t)capInicial * sizeof(Incidente));

    if (!j->especies || !j->planetas || !j->objetos || !j->reglas || !j->viajeros || !j->incidentes) {
        free(j->especies);
        free(j->planetas);
        free(j->objetos);
        free(j->reglas);
        free(j->viajeros);
        free(j->incidentes);
        return 0;
    }
    return 1;
}

void liberarAduana(Aduana *j) {
    if (j == NULL) {
        return;
    }
    free(j->especies);
    free(j->planetas);
    free(j->objetos);
    free(j->reglas);
    for (int i = 0; i < j->cantViajeros; i++) {
        free(j->viajeros[i].equipaje);
        j->viajeros[i].equipaje = NULL;
    }
    free(j->viajeros);
    free(j->incidentes);
    j->especies = NULL;
    j->planetas = NULL;
    j->objetos = NULL;
    j->reglas = NULL;
    j->viajeros = NULL;
    j->incidentes = NULL;
}

int agregarObjetoEquipaje(Viajero *v, Objeto *obj) {
    if (v == NULL || obj == NULL) {
        return 0;
    }

    if (v->numObjetos == v->capacidad) {
        if (!crecerArreglo((void **)&v->equipaje, &v->capacidad, sizeof(Objeto *))) {
            return 0;
        }
    }

    v->equipaje[v->numObjetos] = obj;
    v->numObjetos++;

    return 1;
}

int quitarObjetoEquipaje(Viajero *v, int idObjeto) {
    if (v == NULL || v->equipaje == NULL) {
        return 0;
    }

    int posicion = -1;
    for (int i = 0; i < v->numObjetos; i++) {
        if (v->equipaje[i]->id == idObjeto) {
            posicion = i;
            break;
        }
    }
    if (posicion == -1) {
        return 0;
    }
    for (int i = posicion; i < v->numObjetos - 1; i++) {
        v->equipaje[i] = v->equipaje[i + 1];
    }

    v->numObjetos--;
    if (v->capacidad > 2 && v->numObjetos <= v->capacidad / 2) {
        int nuevaCapacidad = v->capacidad / 2;

        Objeto **temporal = realloc(v->equipaje, (size_t)nuevaCapacidad * sizeof(Objeto *));

        if (temporal != NULL) {
            v->equipaje = temporal;
            v->capacidad = nuevaCapacidad;
        }
    }

    return 1;
}

void leerCadena(const char *mensaje, char *destino, int tam) {
    if (destino == NULL || tam <= 0) {
        return;
    }

    do {
        printf("%s", mensaje);
        if (fgets(destino, tam, stdin) == NULL) {
            destino[0] = '\0';
            return;
        }
        destino[strcspn(destino, "\n")] = '\0';

        if (strlen(destino) == 0) {
            printf("El texto no puede estar vacio.\n");
        }
    } while (strlen(destino) == 0);
}

int leerEntero(const char *mensaje, int min, int max) {
    int valor = 0;
    int ok = 0;

    do {
        printf("%s", mensaje);
        if (scanf("%d", &valor) != 1) {
            printf("Entrada invalida. Ingrese un numero.\n");
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
            if (valor < min || valor > max) {
                printf("El valor debe estar entre %d y %d.\n", min, max);
            } else {
                ok = 1;
            }
        }
    } while (!ok);

    return valor;
}

int validarNombre(const char *cadena) {
    if (cadena == NULL) {
        return 0;
    }
    int len = (int)strlen(cadena);
    if (len < 2 || len >= MAX_NOMBRE) {
        return 0;
    }
    return 1;
}

int existeId(const Aduana *a, char tipo, int id) {
    if (a == NULL) {
        return 0;
    }

    if (tipo == 'E') {
        for (int i = 0; i < a->cantEspecies; i++) {
            if (a->especies[i].id == id) return 1;
        }
    } else if (tipo == 'P') {
        for (int i = 0; i < a->cantPlanetas; i++) {
            if (a->planetas[i].id == id) return 1;
        }
    } else if (tipo == 'O') {
        for (int i = 0; i < a->cantObjetos; i++) {
            if (a->objetos[i].id == id) return 1;
        }
    } else if (tipo == 'V') {
        for (int i = 0; i < a->cantViajeros; i++) {
            if (a->viajeros[i].id == id) return 1;
        }
    } else if (tipo == 'I') {
        for (int i = 0; i < a->cantIncidentes; i++) {
            if (a->incidentes[i].id == id) return 1;
        }
    }

    return 0;
}

int validarIdUnico(const Aduana *a, char tipo, int id) {
    if (id <= 0) {
        return 0;
    }
    return !existeId(a, tipo, id);
}

int confirmarAccion(const char *mensaje) {
    char resp[10];
    printf("%s (s/n): ", mensaje);
    scanf("%9s", resp);
    while (getchar() != '\n');

    if (resp[0] == 's' || resp[0] == 'S') {
        return 1;
    }
    return 0;
}

void actualizarReferenciasViajeros(Aduana *a, const Especie *baseEsp, const Planeta *basePla, const Objeto *baseObj) {
    if (a == NULL || a->viajeros == NULL) {
        return;
    }

    for (int i = 0; i < a->cantViajeros; i++) {
        Viajero *v = &a->viajeros[i];

        if (v->especie != NULL && baseEsp != NULL && a->especies != NULL) {
            int idx = (int)(v->especie - baseEsp);
            if (idx >= 0 && idx < a->cantEspecies) {
                v->especie = &a->especies[idx];
            }
        }

        if (v->planeta != NULL && basePla != NULL && a->planetas != NULL) {
            int idx = (int)(v->planeta - basePla);
            if (idx >= 0 && idx < a->cantPlanetas) {
                v->planeta = &a->planetas[idx];
            }
        }

        if (v->equipaje != NULL && baseObj != NULL && a->objetos != NULL) {
            for (int j = 0; j < v->numObjetos; j++) {
                if (v->equipaje[j] != NULL) {
                    int idx = (int)(v->equipaje[j] - baseObj);
                    if (idx >= 0 && idx < a->cantObjetos) {
                        v->equipaje[j] = &a->objetos[idx];
                    }
                }
            }
        }
    }
}

int registrarEspecie(Aduana *a) {
    if (a == NULL) {
        return 0;
    }

    Especie *anterior = a->especies;
    if (a->cantEspecies == a->capEspecies) {
        if (!crecerArreglo((void **)&a->especies, &a->capEspecies, sizeof(Especie))) {
            printf("Error: no se pudo ampliar memoria para especies.\n");
            return 0;
        }
        actualizarReferenciasViajeros(a, anterior, NULL, NULL);
    }

    Especie nueva;
    do {
        nueva.id = leerEntero("ID de especie: ", 1, 100000);
        if (!validarIdUnico(a, 'E', nueva.id)) {
            printf("Ese ID de especie ya existe o no es valido.\n");
        }
    } while (!validarIdUnico(a, 'E', nueva.id));

    do {
        leerCadena("Nombre de especie: ", nueva.nombre, MAX_NOMBRE);
        if (!validarNombre(nueva.nombre)) {
            printf("Nombre invalido.\n");
        }
    } while (!validarNombre(nueva.nombre));

    nueva.nvPeligro = leerEntero("Nivel de peligro base (0-100): ", 0, 100);

    a->especies[a->cantEspecies] = nueva;
    a->cantEspecies++;
    printf("[OK] Especie '%s' registrada (ID: %d | Peligro base: +%d sospecha).\n",
           nueva.nombre, nueva.id, nueva.nvPeligro);
    return 1;
}

int registrarPlaneta(Aduana *a) {
    if (a == NULL) {
        return 0;
    }

    Planeta *anterior = a->planetas;
    if (a->cantPlanetas == a->capPlanetas) {
        if (!crecerArreglo((void **)&a->planetas, &a->capPlanetas, sizeof(Planeta))) {
            printf("Error: no se pudo ampliar memoria para planetas.\n");
            return 0;
        }
        actualizarReferenciasViajeros(a, NULL, anterior, NULL);
    }

    Planeta nuevo;
    do {
        nuevo.id = leerEntero("ID de planeta: ", 1, 100000);
        if (!validarIdUnico(a, 'P', nuevo.id)) {
            printf("Ese ID de planeta ya existe o no es valido.\n");
        }
    } while (!validarIdUnico(a, 'P', nuevo.id));

    do {
        leerCadena("Nombre de planeta: ", nuevo.nombre, MAX_NOMBRE_CORTO);
        if (!validarNombre(nuevo.nombre)) {
            printf("Nombre invalido.\n");
        }
    } while (!validarNombre(nuevo.nombre));

    nuevo.nvRiesgo = leerEntero("Nivel de riesgo del planeta (0-100): ", 0, 100);

    a->planetas[a->cantPlanetas] = nuevo;
    a->cantPlanetas++;
    printf("[OK] Planeta '%s' registrado (ID: %d | Riesgo base: +%d sospecha).\n",
           nuevo.nombre, nuevo.id, nuevo.nvRiesgo);
    return 1;
}

int registrarObjeto(Aduana *a) {
    if (a == NULL) {
        return 0;
    }

    Objeto *anterior = a->objetos;
    if (a->cantObjetos == a->capObjetos) {
        if (!crecerArreglo((void **)&a->objetos, &a->capObjetos, sizeof(Objeto))) {
            printf("Error: no se pudo ampliar memoria para objetos.\n");
            return 0;
        }
        actualizarReferenciasViajeros(a, NULL, NULL, anterior);
    }

    Objeto nuevo;
    do {
        nuevo.id = leerEntero("ID de objeto: ", 1, 100000);
        if (!validarIdUnico(a, 'O', nuevo.id)) {
            printf("Ese ID de objeto ya existe o no es valido.\n");
        }
    } while (!validarIdUnico(a, 'O', nuevo.id));

    do {
        leerCadena("Nombre de objeto: ", nuevo.nombre, MAX_NOMBRE_CORTO);
        if (!validarNombre(nuevo.nombre)) {
            printf("Nombre invalido.\n");
        }
    } while (!validarNombre(nuevo.nombre));

    nuevo.nvPeligro = leerEntero("Nivel de peligro del objeto (0-100): ", 0, 100);
    nuevo.idContrabando = leerEntero("Es contrabando? (1 = Si, 0 = No): ", 0, 1);

    a->objetos[a->cantObjetos] = nuevo;
    a->cantObjetos++;
    printf("[OK] Objeto '%s' registrado (Peligro: +%d%s).\n",
           nuevo.nombre,
           nuevo.nvPeligro,
           nuevo.idContrabando ? " | CONTRABANDO: +20 sospecha extra" : "");
    return 1;
}

int registrarRegla(Aduana *a) {
    if (a == NULL) {
        return 0;
    }

    if (a->cantObjetos == 0 || a->cantEspecies == 0) {
        printf("Debe registrar al menos un objeto y una especie antes de crear reglas.\n");
        return 0;
    }

    if (a->cantRegla == a->capRegla) {
        if (!crecerArreglo((void **)&a->reglas, &a->capRegla, sizeof(Regla))) {
            printf("Error: no se pudo ampliar memoria para reglas.\n");
            return 0;
        }
    }

    Regla nueva;
    do {
        nueva.idObjeto = leerEntero("ID de objeto para la regla: ", 1, 100000);
        if (!existeId(a, 'O', nueva.idObjeto)) {
            printf("No existe un objeto con ese ID.\n");
        }
    } while (!existeId(a, 'O', nueva.idObjeto));

    do {
        nueva.idEspecie = leerEntero("ID de especie para la regla: ", 1, 100000);
        if (!existeId(a, 'E', nueva.idEspecie)) {
            printf("No existe una especie con ese ID.\n");
        }
    } while (!existeId(a, 'E', nueva.idEspecie));

    nueva.permitido = leerEntero("Permitido para esta especie? (1 = Si, 0 = No): ", 0, 1);

    a->reglas[a->cantRegla] = nueva;
    a->cantRegla++;
    printf("[OK] Regla registrada: Objeto ID %d -> %s para Especie ID %d.\n",
           nueva.idObjeto,
           nueva.permitido ? "PERMITIDO" : "PROHIBIDO (+25 sospecha si lo porta)",
           nueva.idEspecie);
    return 1;
}

int registrarViajero(Aduana *a) {
    if (a == NULL) {
        return 0;
    }

    if (a->cantEspecies == 0 || a->cantPlanetas == 0) {
        printf("Debe registrar al menos una especie y un planeta antes de registrar viajeros.\n");
        return 0;
    }

    Especie *antEsp = a->especies;
    Planeta *antPla = a->planetas;
    Objeto *antObj = a->objetos;

    if (a->cantViajeros == a->capViajeros) {
        if (!crecerArreglo((void **)&a->viajeros, &a->capViajeros, sizeof(Viajero))) {
            printf("Error: no se pudo ampliar memoria para viajeros.\n");
            return 0;
        }
        actualizarReferenciasViajeros(a, antEsp, antPla, antObj);
    }

    Viajero *temporal = crearViajero(2);
    if (temporal == NULL) {
        printf("Error al reservar memoria para el viajero.\n");
        return 0;
    }

    do {
        temporal->id = leerEntero("ID de viajero: ", 1, 100000);
        if (!validarIdUnico(a, 'V', temporal->id)) {
            printf("Ese ID de viajero ya existe o no es valido.\n");
        }
    } while (!validarIdUnico(a, 'V', temporal->id));

    do {
        leerCadena("Nombre del viajero: ", temporal->nombre, sizeof(temporal->nombre));
        if (!validarNombre(temporal->nombre)) {
            printf("Nombre invalido.\n");
        }
    } while (!validarNombre(temporal->nombre));

    int idEsp = 0;
    do {
        idEsp = leerEntero("ID de especie del viajero: ", 1, 100000);
        if (!existeId(a, 'E', idEsp)) {
            printf("Especie no encontrada.\n");
        }
    } while (!existeId(a, 'E', idEsp));

    for (int i = 0; i < a->cantEspecies; i++) {
        if (a->especies[i].id == idEsp) {
            temporal->especie = &a->especies[i];
            break;
        }
    }
    printf("  -> Especie asignada: %s (+%d sospecha base)\n",
           temporal->especie->nombre, temporal->especie->nvPeligro);

    int idPla = 0;
    do {
        idPla = leerEntero("ID de planeta de origen: ", 1, 100000);
        if (!existeId(a, 'P', idPla)) {
            printf("Planeta no encontrado.\n");
        }
    } while (!existeId(a, 'P', idPla));

    for (int i = 0; i < a->cantPlanetas; i++) {
        if (a->planetas[i].id == idPla) {
            temporal->planeta = &a->planetas[i];
            break;
        }
    }
    printf("  -> Planeta asignado: %s (+%d sospecha base)\n",
           temporal->planeta->nombre, temporal->planeta->nvRiesgo);

    if (a->cantObjetos > 0) {
        int cantObj = leerEntero("Cuantos objetos lleva en su equipaje?: ", 0, 100);
        for (int i = 0; i < cantObj; i++) {
            int idObj = leerEntero("ID del objeto a agregar: ", 1, 100000);
            Objeto *encontrado = NULL;

            for (int j = 0; j < a->cantObjetos; j++) {
                if (a->objetos[j].id == idObj) {
                    encontrado = &a->objetos[j];
                    break;
                }
            }

            if (encontrado == NULL) {
                printf("Objeto no encontrado, se omite.\n");
            } else if (!agregarObjetoEquipaje(temporal, encontrado)) {
                printf("No se pudo agregar el objeto al equipaje.\n");
            } else {
                printf("  + Agregado '%s' (Peligro: +%d%s)\n",
                       encontrado->nombre,
                       encontrado->nvPeligro,
                       encontrado->idContrabando ? " | CONTRABANDO: +20 sospecha" : "");
            }
        }
    } else {
        printf("No hay objetos registrados en el catalogo; el equipaje queda vacio.\n");
    }

    int sospechaBase = temporal->especie->nvPeligro + temporal->planeta->nvRiesgo;
    for (int i = 0; i < temporal->numObjetos; i++) {
        if (temporal->equipaje[i] != NULL) {
            sospechaBase += temporal->equipaje[i]->nvPeligro;
            if (temporal->equipaje[i]->idContrabando) {
                sospechaBase += 20;
            }
        }
    }

    a->viajeros[a->cantViajeros] = *temporal;
    a->cantViajeros++;
    free(temporal);

    printf("[OK] Viajero registrado correctamente. Sospecha preliminar acumulada: +%d pts (pendiente de evaluacion).\n",
           sospechaBase);
    return 1;
}

Especie *buscarEspecie(Aduana *a, int id, const char *nombre) {
    if (a == NULL || a->especies == NULL) {
        return NULL;
    }

    Especie *fin = a->especies + a->cantEspecies;
    for (Especie *p = a->especies; p < fin; p++) {
        if (id > 0 && p->id == id) {
            return p;
        }
        if (nombre != NULL && nombre[0] != '\0' && strcmp(p->nombre, nombre) == 0) {
            return p;
        }
    }
    return NULL;
}

Planeta *buscarPlaneta(Aduana *a, int id, const char *nombre) {
    if (a == NULL || a->planetas == NULL) {
        return NULL;
    }

    Planeta *fin = a->planetas + a->cantPlanetas;
    for (Planeta *p = a->planetas; p < fin; p++) {
        if (id > 0 && p->id == id) {
            return p;
        }
        if (nombre != NULL && nombre[0] != '\0' && strcmp(p->nombre, nombre) == 0) {
            return p;
        }
    }
    return NULL;
}

Objeto *buscarObjeto(Aduana *a, int id, const char *nombre) {
    if (a == NULL || a->objetos == NULL) {
        return NULL;
    }

    Objeto *fin = a->objetos + a->cantObjetos;
    for (Objeto *p = a->objetos; p < fin; p++) {
        if (id > 0 && p->id == id) {
            return p;
        }
        if (nombre != NULL && nombre[0] != '\0' && strcmp(p->nombre, nombre) == 0) {
            return p;
        }
    }
    return NULL;
}

Viajero *buscarViajero(Aduana *a, int id, const char *nombre) {
    if (a == NULL || a->viajeros == NULL) {
        return NULL;
    }

    Viajero *fin = a->viajeros + a->cantViajeros;
    for (Viajero *p = a->viajeros; p < fin; p++) {
        if (id > 0 && p->id == id) {
            return p;
        }
        if (nombre != NULL && nombre[0] != '\0' && strcmp(p->nombre, nombre) == 0) {
            return p;
        }
    }
    return NULL;
}

void listarEspecies(const Aduana *a) {
    if (a == NULL || a->cantEspecies == 0) {
        printf("No hay especies registradas.\n");
        return;
    }

    printf("\n=== ESPECIES ===\n");
    const Especie *fin = a->especies + a->cantEspecies;
    for (const Especie *p = a->especies; p < fin; p++) {
        printf("ID: %d | Nombre: %s | Peligro base: %d\n", p->id, p->nombre, p->nvPeligro);
    }
}

void listarPlanetas(const Aduana *a) {
    if (a == NULL || a->cantPlanetas == 0) {
        printf("No hay planetas registrados.\n");
        return;
    }

    printf("\n=== PLANETAS ===\n");
    const Planeta *fin = a->planetas + a->cantPlanetas;
    for (const Planeta *p = a->planetas; p < fin; p++) {
        printf("ID: %d | Nombre: %s | Riesgo: %d\n", p->id, p->nombre, p->nvRiesgo);
    }
}

void listarObjetos(const Aduana *a) {
    if (a == NULL || a->cantObjetos == 0) {
        printf("No hay objetos registrados.\n");
        return;
    }

    printf("\n=== OBJETOS ===\n");
    const Objeto *fin = a->objetos + a->cantObjetos;
    for (const Objeto *p = a->objetos; p < fin; p++) {
        printf("ID: %d | Nombre: %s | Peligro: %d | Contrabando: %s\n",
               p->id, p->nombre, p->nvPeligro, p->idContrabando ? "Si" : "No");
    }
}

void listarViajeros(const Aduana *a) {
    if (a == NULL || a->cantViajeros == 0) {
        printf("No hay viajeros registrados.\n");
        return;
    }

    printf("\n=== VIAJEROS ===\n");
    const Viajero *fin = a->viajeros + a->cantViajeros;
    for (const Viajero *p = a->viajeros; p < fin; p++) {
        const char *nomEsp = (p->especie != NULL) ? p->especie->nombre : "Sin especie";
        const char *nomPla = (p->planeta != NULL) ? p->planeta->nombre : "Sin planeta";
        printf("ID: %d | Nombre: %s | Especie: %s | Planeta: %s | Objetos: %d | Riesgo: %d | Decision: %d\n",
               p->id, p->nombre, nomEsp, nomPla, p->numObjetos, p->puntajeRiesgo, p->desicion);
    }
}

int modificarEspecie(Aduana *a) {
    if (a == NULL || a->cantEspecies == 0) {
        printf("No hay especies para modificar.\n");
        return 0;
    }

    int id = leerEntero("ID de especie a modificar: ", 1, 100000);
    Especie *esp = buscarEspecie(a, id, NULL);
    if (esp == NULL) {
        printf("Especie no encontrada.\n");
        return 0;
    }

    char nuevoNombre[MAX_NOMBRE];
    do {
        leerCadena("Nuevo nombre de especie: ", nuevoNombre, MAX_NOMBRE);
        if (!validarNombre(nuevoNombre)) {
            printf("Nombre invalido.\n");
        }
    } while (!validarNombre(nuevoNombre));

    int nuevoPeligro = leerEntero("Nuevo nivel de peligro base (0-100): ", 0, 100);

    strcpy(esp->nombre, nuevoNombre);
    esp->nvPeligro = nuevoPeligro;
    printf("[OK] Especie '%s' actualizada (Peligro base: +%d sospecha).\n",
           esp->nombre, esp->nvPeligro);
    return 1;
}

int modificarObjeto(Aduana *a) {
    if (a == NULL || a->cantObjetos == 0) {
        printf("No hay objetos para modificar.\n");
        return 0;
    }

    int id = leerEntero("ID de objeto a modificar: ", 1, 100000);
    Objeto *obj = buscarObjeto(a, id, NULL);
    if (obj == NULL) {
        printf("Objeto no encontrado.\n");
        return 0;
    }

    char nuevoNombre[MAX_NOMBRE_CORTO];
    do {
        leerCadena("Nuevo nombre de objeto: ", nuevoNombre, MAX_NOMBRE_CORTO);
        if (!validarNombre(nuevoNombre)) {
            printf("Nombre invalido.\n");
        }
    } while (!validarNombre(nuevoNombre));

    int nuevoPeligro = leerEntero("Nuevo nivel de peligro (0-100): ", 0, 100);
    int nuevoContra = leerEntero("Es contrabando? (1 = Si, 0 = No): ", 0, 1);

    strcpy(obj->nombre, nuevoNombre);
    obj->nvPeligro = nuevoPeligro;
    obj->idContrabando = nuevoContra;
    printf("[OK] Objeto '%s' actualizado (Peligro: +%d%s).\n",
           obj->nombre,
           obj->nvPeligro,
           obj->idContrabando ? " | CONTRABANDO: +20 sospecha" : "");
    return 1;
}

int modificarViajero(Aduana *a) {
    if (a == NULL || a->cantViajeros == 0) {
        printf("No hay viajeros para modificar.\n");
        return 0;
    }

    int id = leerEntero("ID de viajero a modificar: ", 1, 100000);
    Viajero *v = buscarViajero(a, id, NULL);
    if (v == NULL) {
        printf("Viajero no encontrado.\n");
        return 0;
    }

    char nuevoNombre[30];
    do {
        leerCadena("Nuevo nombre del viajero: ", nuevoNombre, sizeof(nuevoNombre));
        if (!validarNombre(nuevoNombre)) {
            printf("Nombre invalido.\n");
        }
    } while (!validarNombre(nuevoNombre));
    strcpy(v->nombre, nuevoNombre);

    if (confirmarAccion("Desea cambiar especie y planeta?")) {
        int idEsp = leerEntero("Nuevo ID de especie: ", 1, 100000);
        Especie *esp = buscarEspecie(a, idEsp, NULL);
        if (esp != NULL) {
            v->especie = esp;
            printf("  -> Especie actualizada a %s (+%d sospecha base)\n",
                   esp->nombre, esp->nvPeligro);
        } else {
            printf("Especie no encontrada; se mantiene la anterior.\n");
        }

        int idPla = leerEntero("Nuevo ID de planeta: ", 1, 100000);
        Planeta *pla = buscarPlaneta(a, idPla, NULL);
        if (pla != NULL) {
            v->planeta = pla;
            printf("  -> Planeta actualizado a %s (+%d sospecha base)\n",
                   pla->nombre, pla->nvRiesgo);
        } else {
            printf("Planeta no encontrado; se mantiene el anterior.\n");
        }
    }

    if (confirmarAccion("Desea editar el equipaje del viajero?")) {
        int op = leerEntero("1) Agregar objeto  2) Quitar objeto  0) Nada: ", 0, 2);
        if (op == 1) {
            int idObj = leerEntero("ID del objeto a agregar: ", 1, 100000);
            Objeto *obj = buscarObjeto(a, idObj, NULL);
            if (obj != NULL && agregarObjetoEquipaje(v, obj)) {
                printf("  + Objeto '%s' agregado al equipaje (Peligro: +%d%s).\n",
                       obj->nombre,
                       obj->nvPeligro,
                       obj->idContrabando ? " | CONTRABANDO: +20 sospecha" : "");
            } else {
                printf("No se pudo agregar el objeto.\n");
            }
        } else if (op == 2) {
            int idObj = leerEntero("ID del objeto a quitar: ", 1, 100000);
            Objeto *objPrev = buscarObjeto(a, idObj, NULL);
            if (quitarObjetoEquipaje(v, idObj)) {
                if (objPrev != NULL) {
                    printf("  - Objeto '%s' retirado del equipaje (-%d peligro%s).\n",
                           objPrev->nombre,
                           objPrev->nvPeligro,
                           objPrev->idContrabando ? " y -20 contrabando" : "");
                } else {
                    printf("Objeto quitado del equipaje.\n");
                }
            } else {
                printf("El objeto no estaba en el equipaje.\n");
            }
        }
    }

    printf("[OK] Viajero actualizado (recuerde volver a evaluarlo si cambio su perfil o equipaje).\n");
    return 1;
}

int eliminarEspecie(Aduana *a) {
    if (a == NULL || a->cantEspecies == 0) {
        printf("No hay especies para eliminar.\n");
        return 0;
    }

    int id = leerEntero("ID de especie a eliminar: ", 1, 100000);
    int pos = -1;
    for (int i = 0; i < a->cantEspecies; i++) {
        if (a->especies[i].id == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Especie no encontrada.\n");
        return 0;
    }

    for (int i = 0; i < a->cantViajeros; i++) {
        if (a->viajeros[i].especie != NULL && a->viajeros[i].especie->id == id) {
            printf("No se puede eliminar: hay viajeros asignados a esta especie.\n");
            return 0;
        }
    }

    for (int i = 0; i < a->cantRegla; i++) {
        if (a->reglas[i].idEspecie == id) {
            printf("No se puede eliminar: hay reglas asociadas a esta especie.\n");
            return 0;
        }
    }

    if (!confirmarAccion("Seguro que desea eliminar esta especie?")) {
        printf("Operacion cancelada.\n");
        return 0;
    }

    Especie *anterior = a->especies;
    for (int i = pos; i < a->cantEspecies - 1; i++) {
        a->especies[i] = a->especies[i + 1];
    }
    a->cantEspecies--;
    actualizarReferenciasViajeros(a, anterior, NULL, NULL);

    printf("Especie eliminada.\n");
    return 1;
}

int eliminarObjeto(Aduana *a) {
    if (a == NULL || a->cantObjetos == 0) {
        printf("No hay objetos para eliminar.\n");
        return 0;
    }

    int id = leerEntero("ID de objeto a eliminar: ", 1, 100000);
    int pos = -1;
    for (int i = 0; i < a->cantObjetos; i++) {
        if (a->objetos[i].id == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Objeto no encontrado.\n");
        return 0;
    }

    for (int i = 0; i < a->cantRegla; i++) {
        if (a->reglas[i].idObjeto == id) {
            printf("No se puede eliminar: hay reglas asociadas a este objeto.\n");
            return 0;
        }
    }

    for (int i = 0; i < a->cantViajeros; i++) {
        for (int j = 0; j < a->viajeros[i].numObjetos; j++) {
            if (a->viajeros[i].equipaje[j] != NULL && a->viajeros[i].equipaje[j]->id == id) {
                printf("No se puede eliminar: un viajero lleva este objeto en su equipaje.\n");
                return 0;
            }
        }
    }

    if (!confirmarAccion("Seguro que desea eliminar este objeto?")) {
        printf("Operacion cancelada.\n");
        return 0;
    }

    Objeto *anterior = a->objetos;
    for (int i = pos; i < a->cantObjetos - 1; i++) {
        a->objetos[i] = a->objetos[i + 1];
    }
    a->cantObjetos--;
    actualizarReferenciasViajeros(a, NULL, NULL, anterior);

    printf("Objeto eliminado.\n");
    return 1;
}

int eliminarViajero(Aduana *a) {
    if (a == NULL || a->cantViajeros == 0) {
        printf("No hay viajeros para eliminar.\n");
        return 0;
    }

    int id = leerEntero("ID de viajero a eliminar: ", 1, 100000);
    int pos = -1;
    for (int i = 0; i < a->cantViajeros; i++) {
        if (a->viajeros[i].id == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Viajero no encontrado.\n");
        return 0;
    }

    if (!confirmarAccion("Seguro que desea eliminar este viajero?")) {
        printf("Operacion cancelada.\n");
        return 0;
    }

    free(a->viajeros[pos].equipaje);
    a->viajeros[pos].equipaje = NULL;

    for (int i = pos; i < a->cantViajeros - 1; i++) {
        a->viajeros[i] = a->viajeros[i + 1];
    }
    a->cantViajeros--;

    printf("Viajero eliminado correctamente.\n");
    return 1;
}

int registrarIncidenteInterno(Aduana *a, Viajero *v, Objeto *obj, int motivo, Decision dec) {
    if (a == NULL || v == NULL) {
        return 0;
    }

    if (a->cantIncidentes == a->capIncidentes) {
        if (!crecerArreglo((void **)&a->incidentes, &a->capIncidentes, sizeof(Incidente))) {
            return 0;
        }
    }

    Incidente inc;
    inc.id = a->cantIncidentes + 1;
    inc.viajero = v;
    inc.objeto = obj;
    inc.motivo = motivo;
    inc.fechaHora = time(NULL);

    struct tm *infoTiempo = localtime(&inc.fechaHora);
    if (infoTiempo != NULL) {
        inc.fecha = (infoTiempo->tm_year + 1900) * 10000 +
                    (infoTiempo->tm_mon + 1) * 100 +
                    infoTiempo->tm_mday;
        inc.hora = infoTiempo->tm_hour * 100 + infoTiempo->tm_min;
    } else {
        inc.fecha = 0;
        inc.hora = 0;
    }

    if (dec == AUTORIZADO) {
        strcpy(inc.desicion, "AUTORIZADO");
    } else if (dec == RETENIDO) {
        strcpy(inc.desicion, "RETENIDO");
    } else {
        strcpy(inc.desicion, "RECHAZADO");
    }

    a->incidentes[a->cantIncidentes] = inc;
    a->cantIncidentes++;
    return 1;
}

int generarIncidente(Aduana *a, Viajero *v, Objeto *obj, int motivo, Decision dec) {
    return registrarIncidenteInterno(a, v, obj, motivo, dec);
}

void listarIncidentes(const Aduana *a) {
    if (a == NULL || a->cantIncidentes == 0) {
        printf("No hay incidentes registrados.\n");
        return;
    }

    int filtroId = leerEntero("Filtrar por ID de viajero (0 para ver todos): ", 0, 100000);

    printf("\n=== INCIDENTES ===\n");
    int mostrados = 0;
    for (int i = 0; i < a->cantIncidentes; i++) {
        const Incidente *inc = &a->incidentes[i];
        if (filtroId > 0 && (inc->viajero == NULL || inc->viajero->id != filtroId)) {
            continue;
        }

        const char *nomViajero = (inc->viajero != NULL) ? inc->viajero->nombre : "Desconocido";
        const char *nomObjeto = (inc->objeto != NULL) ? inc->objeto->nombre : "Ninguno";
        printf("ID: %d | Viajero: %s | Objeto: %s | Motivo: %d | Decision: %s | Fecha: %d | Hora: %04d\n",
               inc->id, nomViajero, nomObjeto, inc->motivo, inc->desicion, inc->fecha, inc->hora);
        mostrados++;
    }

    if (mostrados == 0) {
        printf("No hay incidentes para ese viajero.\n");
    }
}

void reporteViajero(const Aduana *a) {
    if (a == NULL || a->cantViajeros == 0) {
        printf("No hay viajeros registrados.\n");
        return;
    }

    int id = leerEntero("ID de viajero para reporte: ", 1, 100000);
    Viajero *v = buscarViajero((Aduana *)a, id, NULL);
    if (v == NULL) {
        printf("Viajero no encontrado.\n");
        return;
    }

    printf("\n=== REPORTE DE VIAJERO ===\n");
    printf("ID: %d | Nombre: %s\n", v->id, v->nombre);
    printf("Especie: %s\n", (v->especie != NULL) ? v->especie->nombre : "Sin especie");
    printf("Planeta: %s\n", (v->planeta != NULL) ? v->planeta->nombre : "Sin planeta");
    printf("Puntaje de riesgo: %d | Decision: %d\n", v->puntajeRiesgo, v->desicion);

    printf("Equipaje (%d objetos):\n", v->numObjetos);
    for (int i = 0; i < v->numObjetos; i++) {
        if (v->equipaje[i] != NULL) {
            printf("  - %s (ID %d, Peligro %d)\n",
                   v->equipaje[i]->nombre, v->equipaje[i]->id, v->equipaje[i]->nvPeligro);
        }
    }

    printf("Incidentes del viajero:\n");
    int countInc = 0;
    for (int i = 0; i < a->cantIncidentes; i++) {
        if (a->incidentes[i].viajero != NULL && a->incidentes[i].viajero->id == v->id) {
            printf("  - Incidente #%d: Motivo %d | Decision %s\n",
                   a->incidentes[i].id, a->incidentes[i].motivo, a->incidentes[i].desicion);
            countInc++;
        }
    }
    if (countInc == 0) {
        printf("  Sin incidentes registrados.\n");
    }
}

void reporteGeneral(const Aduana *a) {
    if (a == NULL) {
        return;
    }

    int totAutorizados = 0;
    int totRetenidos = 0;
    int totRechazados = 0;

    for (int i = 0; i < a->cantViajeros; i++) {
        if (a->viajeros[i].desicion == AUTORIZADO) totAutorizados++;
        else if (a->viajeros[i].desicion == RETENIDO) totRetenidos++;
        else if (a->viajeros[i].desicion == RECHAZADO) totRechazados++;
    }

    printf("\n=== REPORTE GENERAL ===\n");
    printf("Total viajeros: %d (Autorizados: %d | Retenidos: %d | Rechazados: %d)\n",
           a->cantViajeros, totAutorizados, totRetenidos, totRechazados);
    printf("Total incidentes: %d\n", a->cantIncidentes);

    printf("\nIncidentes por especie:\n");
    for (int i = 0; i < a->cantEspecies; i++) {
        int cont = 0;
        for (int j = 0; j < a->cantIncidentes; j++) {
            if (a->incidentes[j].viajero != NULL &&
                a->incidentes[j].viajero->especie != NULL &&
                a->incidentes[j].viajero->especie->id == a->especies[i].id) {
                cont++;
            }
        }
        printf("  - %s: %d\n", a->especies[i].nombre, cont);
    }

    printf("\nObjetos detectados en incidentes:\n");
    for (int i = 0; i < a->cantObjetos; i++) {
        int cont = 0;
        for (int j = 0; j < a->cantIncidentes; j++) {
            if (a->incidentes[j].objeto != NULL &&
                a->incidentes[j].objeto->id == a->objetos[i].id) {
                cont++;
            }
        }
        printf("  - %s: %d\n", a->objetos[i].nombre, cont);
    }
}

int exportarReporte(const Aduana *a) {
    if (a == NULL) {
        return 0;
    }

    FILE *f = fopen("reporte_aduana.txt", "w");
    if (f == NULL) {
        printf("Error al crear reporte_aduana.txt.\n");
        return 0;
    }

    fprintf(f, "=== REPORTE GENERAL DE ADUANA ===\n");
    fprintf(f, "Especies: %d | Planetas: %d | Objetos: %d | Reglas: %d\n",
            a->cantEspecies, a->cantPlanetas, a->cantObjetos, a->cantRegla);
    fprintf(f, "Viajeros: %d | Incidentes: %d\n\n",
            a->cantViajeros, a->cantIncidentes);

    fprintf(f, "--- VIAJEROS ---\n");
    for (int i = 0; i < a->cantViajeros; i++) {
        const Viajero *v = &a->viajeros[i];
        fprintf(f, "ID:%d | Nombre:%s | Riesgo:%d | Decision:%d | Objetos:%d\n",
                v->id, v->nombre, v->puntajeRiesgo, v->desicion, v->numObjetos);
    }

    fprintf(f, "\n--- INCIDENTES ---\n");
    for (int i = 0; i < a->cantIncidentes; i++) {
        const Incidente *inc = &a->incidentes[i];
        fprintf(f, "ID:%d | Viajero:%s | Objeto:%s | Motivo:%d | Decision:%s\n",
                inc->id,
                (inc->viajero != NULL) ? inc->viajero->nombre : "N/A",
                (inc->objeto != NULL) ? inc->objeto->nombre : "N/A",
                inc->motivo,
                inc->desicion);
    }

    fclose(f);
    printf("Reporte exportado a reporte_aduana.txt\n");
    return 1;
}

int objetoPermitido(const Aduana *a, const Objeto *obj, const Especie *esp) {
    if (a == NULL || obj == NULL) {
        return 0;
    }

    if (esp == NULL || a->reglas == NULL) {
        return obj->idContrabando ? 0 : 1;
    }

    for (int i = 0; i < a->cantRegla; i++) {
        if (a->reglas[i].idObjeto == obj->id && a->reglas[i].idEspecie == esp->id) {
            return a->reglas[i].permitido ? 1 : 0;
        }
    }

    return obj->idContrabando ? 0 : 1;
}

int detectarObjetosProhibidos(const Aduana *a, const Viajero *v, Objeto **prohibidos, int *cantProhibidos) {
    if (prohibidos == NULL || cantProhibidos == NULL) {
        return 0;
    }

    *prohibidos = NULL;
    *cantProhibidos = 0;

    if (a == NULL || v == NULL || v->equipaje == NULL || v->numObjetos <= 0) {
        return 0;
    }

    Objeto *lista = malloc((size_t)v->numObjetos * sizeof(Objeto));
    if (lista == NULL) {
        return 0;
    }

    int encontrados = 0;
    Objeto **fin = v->equipaje + v->numObjetos;
    for (Objeto **ptr = v->equipaje; ptr < fin; ptr++) {
        if (*ptr != NULL && !objetoPermitido(a, *ptr, v->especie)) {
            lista[encontrados] = **ptr;
            encontrados++;
        }
    }

    if (encontrados == 0) {
        free(lista);
        return 0;
    }

    Objeto *reducido = realloc(lista, (size_t)encontrados * sizeof(Objeto));
    *prohibidos = (reducido != NULL) ? reducido : lista;
    *cantProhibidos = encontrados;
    return encontrados;
}

int detectarConflictos(const Viajero *v) {
    if (v == NULL || v->equipaje == NULL || v->numObjetos < 2) {
        return 0;
    }

    int tieneAgujeroNegro = 0;
    int tieneMascotaInvisible = 0;

    Objeto **fin = v->equipaje + v->numObjetos;
    for (Objeto **ptr = v->equipaje; ptr < fin; ptr++) {
        if (*ptr != NULL) {
            if (strcmp((*ptr)->nombre, "Agujero negro") == 0 ||
                strcmp((*ptr)->nombre, "agujero negro") == 0) {
                tieneAgujeroNegro = 1;
            }
            if (strcmp((*ptr)->nombre, "Mascota invisible") == 0 ||
                strcmp((*ptr)->nombre, "mascota invisible") == 0) {
                tieneMascotaInvisible = 1;
            }
        }
    }

    if (tieneAgujeroNegro && tieneMascotaInvisible) {
        return 35;
    }
    return 0;
}

int calcularRiesgo(const Aduana *a, const Viajero *v) {
    if (a == NULL || v == NULL) {
        return 0;
    }

    int puntaje = 0;

    if (v->especie != NULL) {
        puntaje += v->especie->nvPeligro;
    }

    if (v->planeta != NULL) {
        puntaje += v->planeta->nvRiesgo;
    }

    if (v->equipaje != NULL && v->numObjetos > 0) {
        Objeto **fin = v->equipaje + v->numObjetos;
        for (Objeto **ptr = v->equipaje; ptr < fin; ptr++) {
            if (*ptr != NULL) {
                puntaje += (*ptr)->nvPeligro;
                if ((*ptr)->idContrabando) {
                    puntaje += 20;
                }
                if (!objetoPermitido(a, *ptr, v->especie)) {
                    puntaje += 25;
                }
            }
        }
    }

    puntaje += detectarConflictos(v);

    if (puntaje < 0) {
        puntaje = 0;
    }

    return puntaje;
}

Decision clasificarViajero(const Aduana *a, const Viajero *v, int puntaje) {
    if (a != NULL && v != NULL && v->equipaje != NULL) {
        Objeto **fin = v->equipaje + v->numObjetos;
        for (Objeto **ptr = v->equipaje; ptr < fin; ptr++) {
            if (*ptr != NULL) {
                int prohibido = !objetoPermitido(a, *ptr, v->especie);
                if ((*ptr)->nvPeligro >= 80 || ((*ptr)->idContrabando && prohibido)) {
                    return RECHAZADO;
                }
            }
        }
    }

    if (puntaje >= UMBRAL_RECHAZADO) {
        return RECHAZADO;
    }
    if (puntaje >= UMBRAL_RETENIDO) {
        return RETENIDO;
    }
    return AUTORIZADO;
}

int evaluarViajero(Aduana *a) {
    if (a == NULL || a->cantViajeros == 0) {
        printf("No hay viajeros registrados para evaluar.\n");
        return 0;
    }

    int id = leerEntero("ID de viajero a evaluar: ", 1, 100000);
    Viajero *v = buscarViajero(a, id, NULL);
    if (v == NULL) {
        printf("Viajero no encontrado.\n");
        return 0;
    }

    Objeto *prohibidos = NULL;
    int cantProhibidos = 0;
    detectarObjetosProhibidos(a, v, &prohibidos, &cantProhibidos);

    int puntaje = calcularRiesgo(a, v);
    Decision dec = clasificarViajero(a, v, puntaje);

    v->puntajeRiesgo = puntaje;
    v->desicion = (int)dec;

    int baseEsp = (v->especie != NULL) ? v->especie->nvPeligro : 0;
    int basePla = (v->planeta != NULL) ? v->planeta->nvRiesgo : 0;
    int bonusConf = detectarConflictos(v);

    printf("\n=== DESGLOSE DE EVALUACION ===\n");
    printf("Viajero: %s (ID %d)\n", v->nombre, v->id);
    printf("  + Especie (%s): +%d sospecha\n",
           (v->especie != NULL) ? v->especie->nombre : "Sin especie", baseEsp);
    printf("  + Planeta (%s): +%d sospecha\n",
           (v->planeta != NULL) ? v->planeta->nombre : "Sin planeta", basePla);

    if (v->equipaje != NULL && v->numObjetos > 0) {
        for (int i = 0; i < v->numObjetos; i++) {
            Objeto *obj = v->equipaje[i];
            if (obj != NULL) {
                int extraContra = obj->idContrabando ? 20 : 0;
                int extraProhib = !objetoPermitido(a, obj, v->especie) ? 25 : 0;
                printf("  + Objeto '%s': +%d peligro", obj->nombre, obj->nvPeligro);
                if (extraContra > 0) printf(" | +20 contrabando");
                if (extraProhib > 0) printf(" | +25 prohibido por regla");
                printf("\n");
            }
        }
    } else {
        printf("  + Equipaje vacio: +0 sospecha\n");
    }

    if (bonusConf > 0) {
        printf("  + ALERTA DE CONFLICTO en equipaje: +%d sospecha\n", bonusConf);
    }

    if (prohibidos != NULL && cantProhibidos > 0) {
        for (int i = 0; i < cantProhibidos; i++) {
            Objeto *objReal = buscarObjeto(a, prohibidos[i].id, NULL);
            int motivo = prohibidos[i].idContrabando ? MOTIVO_CONTRABANDO : MOTIVO_OBJETO_PROHIBIDO;
            generarIncidente(a, v, objReal, motivo, dec);
        }
        printf("  -> Se generaron %d incidente(s) por objetos prohibidos/contrabando.\n", cantProhibidos);
        free(prohibidos);
    }

    if (bonusConf > 0) {
        generarIncidente(a, v, NULL, MOTIVO_CONFLICTO_EQUIPAJE, dec);
        printf("  -> Se genero 1 incidente por conflicto en equipaje.\n");
    }

    if (dec == RECHAZADO && cantProhibidos == 0 && bonusConf == 0) {
        generarIncidente(a, v, NULL, MOTIVO_RIESGO_ALTO, dec);
        printf("  -> Se genero 1 incidente por nivel de riesgo critico.\n");
    }

    printf("----------------------------------------\n");
    printf("Puntaje total de riesgo: %d (Retenido >= %d | Rechazado >= %d)\n",
           v->puntajeRiesgo, UMBRAL_RETENIDO, UMBRAL_RECHAZADO);
    printf("Decision final: %s\n",
           (dec == AUTORIZADO) ? "AUTORIZADO" : (dec == RETENIDO) ? "RETENIDO" : "RECHAZADO");

    return 1;
}

int guardarEspecies(const Aduana *a) {
    if (a == NULL) return 0;
    FILE *f = fopen(ARCHIVO_ESPECIES, "w");
    if (f == NULL) return 0;

    for (int i = 0; i < a->cantEspecies; i++) {
        fprintf(f, "%d|%s|%d\n",
                a->especies[i].id,
                a->especies[i].nombre,
                a->especies[i].nvPeligro);
    }
    fclose(f);
    return 1;
}

int guardarPlanetas(const Aduana *a) {
    if (a == NULL) return 0;
    FILE *f = fopen(ARCHIVO_PLANETAS, "w");
    if (f == NULL) return 0;

    for (int i = 0; i < a->cantPlanetas; i++) {
        fprintf(f, "%d|%s|%d\n",
                a->planetas[i].id,
                a->planetas[i].nombre,
                a->planetas[i].nvRiesgo);
    }
    fclose(f);
    return 1;
}

int guardarObjetos(const Aduana *a) {
    if (a == NULL) return 0;
    FILE *f = fopen(ARCHIVO_OBJETOS, "w");
    if (f == NULL) return 0;

    for (int i = 0; i < a->cantObjetos; i++) {
        fprintf(f, "%d|%s|%d|%d\n",
                a->objetos[i].id,
                a->objetos[i].nombre,
                a->objetos[i].nvPeligro,
                a->objetos[i].idContrabando);
    }
    fclose(f);
    return 1;
}

int guardarReglas(const Aduana *a) {
    if (a == NULL) return 0;
    FILE *f = fopen(ARCHIVO_REGLAS, "w");
    if (f == NULL) return 0;

    for (int i = 0; i < a->cantRegla; i++) {
        fprintf(f, "%d|%d|%d\n",
                a->reglas[i].idObjeto,
                a->reglas[i].idEspecie,
                a->reglas[i].permitido);
    }
    fclose(f);
    return 1;
}

int guardarViajeros(const Aduana *a) {
    if (a == NULL) return 0;
    FILE *f = fopen(ARCHIVO_VIAJEROS, "w");
    if (f == NULL) return 0;

    for (int i = 0; i < a->cantViajeros; i++) {
        const Viajero *v = &a->viajeros[i];
        int idEsp = (v->especie != NULL) ? v->especie->id : 0;
        int idPla = (v->planeta != NULL) ? v->planeta->id : 0;

        fprintf(f, "%d|%s|%d|%d|%d|%d|%d",
                v->id, v->nombre, idEsp, idPla,
                v->puntajeRiesgo, v->desicion, v->numObjetos);

        for (int j = 0; j < v->numObjetos; j++) {
            int idObj = (v->equipaje[j] != NULL) ? v->equipaje[j]->id : 0;
            fprintf(f, "|%d", idObj);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 1;
}

int guardarIncidentes(const Aduana *a) {
    if (a == NULL) return 0;
    FILE *f = fopen(ARCHIVO_INCIDENTES, "w");
    if (f == NULL) return 0;

    for (int i = 0; i < a->cantIncidentes; i++) {
        const Incidente *inc = &a->incidentes[i];
        int idVia = (inc->viajero != NULL) ? inc->viajero->id : 0;
        int idObj = (inc->objeto != NULL) ? inc->objeto->id : 0;

        fprintf(f, "%d|%d|%d|%ld|%d|%d|%d|%s\n",
                inc->id, idVia, idObj, (long)inc->fechaHora,
                inc->fecha, inc->hora, inc->motivo, inc->desicion);
    }
    fclose(f);
    return 1;
}

int cargarEspecies(Aduana *a) {
    if (a == NULL) return 0;
    FILE *f = fopen(ARCHIVO_ESPECIES, "r");
    if (f == NULL) return 0;

    char linea[256];
    while (fgets(linea, sizeof(linea), f) != NULL) {
        linea[strcspn(linea, "\n")] = '\0';
        if (strlen(linea) == 0) continue;

        Especie esp;
        if (sscanf(linea, "%d|%49[^|]|%d", &esp.id, esp.nombre, &esp.nvPeligro) == 3) {
            if (a->cantEspecies == a->capEspecies) {
                if (!crecerArreglo((void **)&a->especies, &a->capEspecies, sizeof(Especie))) break;
            }
            a->especies[a->cantEspecies++] = esp;
        }
    }
    fclose(f);
    return 1;
}

int cargarPlanetas(Aduana *a) {
    if (a == NULL) return 0;
    FILE *f = fopen(ARCHIVO_PLANETAS, "r");
    if (f == NULL) return 0;

    char linea[256];
    while (fgets(linea, sizeof(linea), f) != NULL) {
        linea[strcspn(linea, "\n")] = '\0';
        if (strlen(linea) == 0) continue;

        Planeta pla;
        if (sscanf(linea, "%d|%19[^|]|%d", &pla.id, pla.nombre, &pla.nvRiesgo) == 3) {
            if (a->cantPlanetas == a->capPlanetas) {
                if (!crecerArreglo((void **)&a->planetas, &a->capPlanetas, sizeof(Planeta))) break;
            }
            a->planetas[a->cantPlanetas++] = pla;
        }
    }
    fclose(f);
    return 1;
}

int cargarObjetos(Aduana *a) {
    if (a == NULL) return 0;
    FILE *f = fopen(ARCHIVO_OBJETOS, "r");
    if (f == NULL) return 0;

    char linea[256];
    while (fgets(linea, sizeof(linea), f) != NULL) {
        linea[strcspn(linea, "\n")] = '\0';
        if (strlen(linea) == 0) continue;

        Objeto obj;
        if (sscanf(linea, "%d|%19[^|]|%d|%d", &obj.id, obj.nombre, &obj.nvPeligro, &obj.idContrabando) == 4) {
            if (a->cantObjetos == a->capObjetos) {
                if (!crecerArreglo((void **)&a->objetos, &a->capObjetos, sizeof(Objeto))) break;
            }
            a->objetos[a->cantObjetos++] = obj;
        }
    }
    fclose(f);
    return 1;
}

int cargarReglas(Aduana *a) {
    if (a == NULL) return 0;
    FILE *f = fopen(ARCHIVO_REGLAS, "r");
    if (f == NULL) return 0;

    char linea[256];
    while (fgets(linea, sizeof(linea), f) != NULL) {
        linea[strcspn(linea, "\n")] = '\0';
        if (strlen(linea) == 0) continue;

        Regla r;
        if (sscanf(linea, "%d|%d|%d", &r.idObjeto, &r.idEspecie, &r.permitido) == 3) {
            if (a->cantRegla == a->capRegla) {
                if (!crecerArreglo((void **)&a->reglas, &a->capRegla, sizeof(Regla))) break;
            }
            a->reglas[a->cantRegla++] = r;
        }
    }
    fclose(f);
    return 1;
}

int cargarViajeros(Aduana *a) {
    if (a == NULL) return 0;
    FILE *f = fopen(ARCHIVO_VIAJEROS, "r");
    if (f == NULL) return 0;

    char linea[512];
    while (fgets(linea, sizeof(linea), f) != NULL) {
        linea[strcspn(linea, "\n")] = '\0';
        if (strlen(linea) == 0) continue;

        int id = 0, idEsp = 0, idPla = 0, riesgo = 0, decision = 0, cantObj = 0;
        char nombre[30];

        char *token = strtok(linea, "|");
        if (!token) continue;
        id = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(nombre, token, sizeof(nombre) - 1);
        nombre[sizeof(nombre) - 1] = '\0';

        token = strtok(NULL, "|"); if (!token) continue; idEsp = atoi(token);
        token = strtok(NULL, "|"); if (!token) continue; idPla = atoi(token);
        token = strtok(NULL, "|"); if (!token) continue; riesgo = atoi(token);
        token = strtok(NULL, "|"); if (!token) continue; decision = atoi(token);
        token = strtok(NULL, "|"); if (!token) continue; cantObj = atoi(token);

        Viajero *vTemp = crearViajero(cantObj > 2 ? cantObj : 2);
        if (vTemp == NULL) continue;

        vTemp->id = id;
        strcpy(vTemp->nombre, nombre);
        vTemp->especie = buscarEspecie(a, idEsp, NULL);
        vTemp->planeta = buscarPlaneta(a, idPla, NULL);
        vTemp->puntajeRiesgo = riesgo;
        vTemp->desicion = decision;

        for (int i = 0; i < cantObj; i++) {
            token = strtok(NULL, "|");
            if (token != NULL) {
                int idObj = atoi(token);
                Objeto *obj = buscarObjeto(a, idObj, NULL);
                if (obj != NULL) {
                    agregarObjetoEquipaje(vTemp, obj);
                }
            }
        }

        if (a->cantViajeros == a->capViajeros) {
            if (!crecerArreglo((void **)&a->viajeros, &a->capViajeros, sizeof(Viajero))) {
                liberarViajero(vTemp);
                break;
            }
        }
        a->viajeros[a->cantViajeros++] = *vTemp;
        free(vTemp);
    }
    fclose(f);
    return 1;
}

int cargarIncidentes(Aduana *a) {
    if (a == NULL) return 0;
    FILE *f = fopen(ARCHIVO_INCIDENTES, "r");
    if (f == NULL) return 0;

    char linea[256];
    while (fgets(linea, sizeof(linea), f) != NULL) {
        linea[strcspn(linea, "\n")] = '\0';
        if (strlen(linea) == 0) continue;

        int id = 0, idVia = 0, idObj = 0, fecha = 0, hora = 0, motivo = 0;
        long ts = 0;
        char dec[50];

        if (sscanf(linea, "%d|%d|%d|%ld|%d|%d|%d|%49[^\n]",
                   &id, &idVia, &idObj, &ts, &fecha, &hora, &motivo, dec) == 8) {
            Incidente inc;
            inc.id = id;
            inc.viajero = buscarViajero(a, idVia, NULL);
            inc.objeto = buscarObjeto(a, idObj, NULL);
            inc.fechaHora = (time_t)ts;
            inc.fecha = fecha;
            inc.hora = hora;
            inc.motivo = motivo;
            strcpy(inc.desicion, dec);

            if (a->cantIncidentes == a->capIncidentes) {
                if (!crecerArreglo((void **)&a->incidentes, &a->capIncidentes, sizeof(Incidente))) break;
            }
            a->incidentes[a->cantIncidentes++] = inc;
        }
    }
    fclose(f);
    return 1;
}

void guardarTodo(const Aduana *a) {
    if (a == NULL) return;
    guardarEspecies(a);
    guardarPlanetas(a);
    guardarObjetos(a);
    guardarReglas(a);
    guardarViajeros(a);
    guardarIncidentes(a);
    printf("[OK] Todos los datos fueron guardados en sus archivos .txt.\n");
}

void cargarTodo(Aduana *a) {
    if (a == NULL) return;
    cargarEspecies(a);
    cargarPlanetas(a);
    cargarObjetos(a);
    cargarReglas(a);
    cargarViajeros(a);
    cargarIncidentes(a);
}

void mostrarMenu(void) {
    printf("\n=== SISTEMA DE ADUANA INTERGALACTICA ===\n");
    printf("1) Registro\n");
    printf("2) Consultas y listados\n");
    printf("3) Modificar\n");
    printf("4) Eliminar\n");
    printf("5) Evaluar viajero\n");
    printf("6) Reportes e incidentes\n");
    printf("7) Guardar datos ahora\n");
    printf("0) Guardar y salir\n");
}

void menuRegistro(Aduana *a) {
    if (a == NULL) return;

    int op = -1;
    do {
        printf("\n--- MENU REGISTRO ---\n");
        printf("1) Registrar especie\n");
        printf("2) Registrar planeta\n");
        printf("3) Registrar objeto\n");
        printf("4) Registrar regla\n");
        printf("5) Registrar viajero\n");
        printf("0) Volver\n");
        op = leerEntero("Opcion: ", 0, 5);

        switch (op) {
            case 1: registrarEspecie(a); break;
            case 2: registrarPlaneta(a); break;
            case 3: registrarObjeto(a); break;
            case 4: registrarRegla(a); break;
            case 5: registrarViajero(a); break;
            case 0: break;
        }
    } while (op != 0);
}

void menuConsultas(Aduana *a) {
    if (a == NULL) return;

    int op = -1;
    do {
        printf("\n--- MENU CONSULTAS ---\n");
        printf("1) Listar especies\n");
        printf("2) Listar planetas\n");
        printf("3) Listar objetos\n");
        printf("4) Listar viajeros\n");
        printf("5) Buscar viajero por ID\n");
        printf("0) Volver\n");
        op = leerEntero("Opcion: ", 0, 5);

        if (op == 1) {
            listarEspecies(a);
        } else if (op == 2) {
            listarPlanetas(a);
        } else if (op == 3) {
            listarObjetos(a);
        } else if (op == 4) {
            listarViajeros(a);
        } else if (op == 5) {
            int id = leerEntero("ID de viajero: ", 1, 100000);
            Viajero *v = buscarViajero(a, id, NULL);
            if (v != NULL) {
                printf("Encontrado: %s (ID %d) | Riesgo: %d | Decision: %d\n",
                       v->nombre, v->id, v->puntajeRiesgo, v->desicion);
            } else {
                printf("Viajero no encontrado.\n");
            }
        }
    } while (op != 0);
}

void menuModificar(Aduana *a) {
    if (a == NULL) return;

    int op = -1;
    do {
        printf("\n--- MENU MODIFICAR ---\n");
        printf("1) Modificar especie\n");
        printf("2) Modificar objeto\n");
        printf("3) Modificar viajero\n");
        printf("0) Volver\n");
        op = leerEntero("Opcion: ", 0, 3);

        switch (op) {
            case 1: modificarEspecie(a); break;
            case 2: modificarObjeto(a); break;
            case 3: modificarViajero(a); break;
            case 0: break;
        }
    } while (op != 0);
}

void menuEliminar(Aduana *a) {
    if (a == NULL) return;

    int op = -1;
    do {
        printf("\n--- MENU ELIMINAR ---\n");
        printf("1) Eliminar especie\n");
        printf("2) Eliminar objeto\n");
        printf("3) Eliminar viajero\n");
        printf("0) Volver\n");
        op = leerEntero("Opcion: ", 0, 3);

        switch (op) {
            case 1: eliminarEspecie(a); break;
            case 2: eliminarObjeto(a); break;
            case 3: eliminarViajero(a); break;
            case 0: break;
        }
    } while (op != 0);
}

void menuReportes(const Aduana *a) {
    if (a == NULL) return;

    int op = -1;
    do {
        printf("\n--- MENU REPORTES ---\n");
        printf("1) Listar incidentes\n");
        printf("2) Reporte por viajero\n");
        printf("3) Reporte general\n");
        printf("4) Exportar reporte a .txt\n");
        printf("0) Volver\n");
        op = leerEntero("Opcion: ", 0, 4);

        switch (op) {
            case 1: listarIncidentes(a); break;
            case 2: reporteViajero(a); break;
            case 3: reporteGeneral(a); break;
            case 4: exportarReporte(a); break;
            case 0: break;
        }
    } while (op != 0);
}

int main(void) {
    Aduana aduana;

    if (!crearAduana(&aduana, 2)) {
        printf("Error al inicializar la aduana.\n");
        return 1;
    }

    cargarTodo(&aduana);
    printf("\n");
    printf("============================================================\n");
    printf("          Bienvenido al SISTEMA DE ADUANAS INTERGALACTICAS\n");
    printf("============================================================\n");
    printf("Datos cargados -> Especies: %d | Planetas: %d | Objetos: %d | Reglas: %d | Viajeros: %d | Incidentes: %d\n",
           aduana.cantEspecies, aduana.cantPlanetas, aduana.cantObjetos,
           aduana.cantRegla, aduana.cantViajeros, aduana.cantIncidentes);

    int opcion = -1;
    do {
        mostrarMenu();
        opcion = leerEntero("Seleccione una opcion: ", 0, 7);

        switch (opcion) {
            case 1: menuRegistro(&aduana); break;
            case 2: menuConsultas(&aduana); break;
            case 3: menuModificar(&aduana); break;
            case 4: menuEliminar(&aduana); break;
            case 5: evaluarViajero(&aduana); break;
            case 6: menuReportes(&aduana); break;
            case 7: guardarTodo(&aduana); break;
            case 0:
                guardarTodo(&aduana);
                printf("Saliendo del sistema...\n");
                break;
        }
    } while (opcion != 0);

    liberarAduana(&aduana);
    return 0;
}
