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
typedef struct{
    char nombre[50];
    int id;
    int nvPeligro;
} Especie;

typedef struct{
    char nombre[20];
    int id;
    int nvRiesgo;
} Planeta;
typedef struct{
    char nombre[20];
    int id;
    int nvPeligro;
    int idContrabando;
} Objeto;
typedef struct{
int idObjeto;
int idEspecie;
int permitido;
} Regla;
typedef struct{
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
typedef struct{
char desicion[50];
int id;
int fecha;
int hora;
Viajero *viajero;
Objeto *objeto;
} Incidente;

typedef struct{
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
int crecerArreglo(void **arreglo, int *capacidad, size_t tamElemento){
    int nuevaCapacidad=(*capacidad)*2;
    void *temporal=realloc(*arreglo,(size_t)nuevaCapacidad*tamElemento);
    if(temporal==NULL){
        return 0;
    }
    *arreglo=temporal;
    *capacidad=nuevaCapacidad;
    return 1;
}
Viajero *crearViajero(int capacidadInicial){

    if(capacidadInicial <= 0){
        return NULL;
    }

    Viajero *nuevo = malloc(sizeof(Viajero));

    if(nuevo == NULL){
        return NULL;
    }

    nuevo->equipaje = malloc(
        (size_t)capacidadInicial * sizeof(Objeto*)
    );

    if(nuevo->equipaje == NULL){
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
void liberarViajero(Viajero *v){

    if(v == NULL){
        return;
    }

    free(v->equipaje);
    v->equipaje = NULL;

    free(v);
}
int crearAduana(Aduana *j, int capInicial){
    if(j == NULL || capInicial <= 0){
    return 0;
}
    j->cantEspecies=0;
    j->capEspecies=capInicial;
    j->especies = malloc((size_t)capInicial * sizeof(Especie));
    
    j->cantPlanetas=0;
    j->capPlanetas=capInicial;
    j->planetas = malloc((size_t)capInicial * sizeof(Planeta));
    
    j->cantObjetos=0;
    j->capObjetos=capInicial;
    j->objetos = malloc((size_t)capInicial * sizeof(Objeto));
    
    j->cantRegla=0;
    j->capRegla=capInicial;
    j->reglas = malloc((size_t)capInicial * sizeof(Regla));
    
    j->cantViajeros=0;
    j->capViajeros=capInicial;
    j->viajeros = malloc((size_t)capInicial * sizeof(Viajero));
    
    j->cantIncidentes=0;
    j->capIncidentes=capInicial;
    j->incidentes = malloc((size_t)capInicial * sizeof(Incidente));
        if(!j->especies||!j->planetas||!j->objetos||!j->reglas||!j->viajeros||!j->incidentes){
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
void liberarAduana(Aduana *j){
    if(j == NULL){
    return;
}
    free(j->especies);
    free(j->planetas);
    free(j->objetos);
    free(j->reglas);
    for(int i = 0; i < j->cantViajeros; i++){
    free(j->viajeros[i].equipaje);
    j->viajeros[i].equipaje = NULL;
}
    free(j->viajeros);
    free(j->incidentes);
    j->especies=NULL;
    j->planetas=NULL;
    j->objetos=NULL;
    j->reglas=NULL;
    j->viajeros=NULL;
    j->incidentes=NULL;
}
int agregarObjetoEquipaje(Viajero *v, Objeto *obj){

    if(v == NULL || obj == NULL){
        return 0;
    }

    if(v->numObjetos == v->capacidad){

        if(!crecerArreglo((void**)&v->equipaje,
                          &v->capacidad,
                          sizeof(Objeto*))){

            return 0;
        }
    }

    v->equipaje[v->numObjetos] = obj;

    v->numObjetos++;

    return 1;
}
int quitarObjetoEquipaje(Viajero *v, int idObjeto){

    if(v == NULL || v->equipaje == NULL){
        return 0;
    }

    int posicion = -1;
    for(int i = 0; i < v->numObjetos; i++){

        if(v->equipaje[i]->id == idObjeto){
            posicion = i;
            break;
        }
    }
    if(posicion == -1){
        return 0;
    }
    for(int i = posicion; i < v->numObjetos - 1; i++){

        v->equipaje[i] = v->equipaje[i + 1];
    }

    v->numObjetos--;
    if(v->capacidad > 2 &&
       v->numObjetos <= v->capacidad / 2){

        int nuevaCapacidad = v->capacidad / 2;

        Objeto **temporal = realloc(
            v->equipaje,
            (size_t)nuevaCapacidad * sizeof(Objeto*)
        );

        if(temporal != NULL){

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
void actualizarReferenciasViajeros(Aduana *a, const Especie *baseEsp, const Planeta *basePla, const Objeto *baseObj);
int registrarEspecie(Aduana *a);
int registrarPlaneta(Aduana *a);
int registrarObjeto(Aduana *a);
int registrarRegla(Aduana *a);
int registrarViajero(Aduana *a);void actualizarReferenciasViajeros(Aduana *a, const Especie *baseEsp, const Planeta *basePla, const Objeto *baseObj) {
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
    printf("Especie registrada.\n");
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
    printf("Planeta registrado.\n");
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
    printf("Objeto registrado.\n");
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
    printf("Regla registrada.\n");
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
            }
        }
    } else {
        printf("No hay objetos registrados en el catalogo; el equipaje queda vacio.\n");
    }

    a->viajeros[a->cantViajeros] = *temporal;
    a->cantViajeros++;
    free(temporal);

    printf("Viajero registrado correctamente.\n");
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
    printf("Especie modificada correctamente.\n");
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
    printf("Objeto modificado correctamente.\n");
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
        } else {
            printf("Especie no encontrada; se mantiene la anterior.\n");
        }

        int idPla = leerEntero("Nuevo ID de planeta: ", 1, 100000);
        Planeta *pla = buscarPlaneta(a, idPla, NULL);
        if (pla != NULL) {
            v->planeta = pla;
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
                printf("Objeto agregado al equipaje.\n");
            } else {
                printf("No se pudo agregar el objeto.\n");
            }
        } else if (op == 2) {
            int idObj = leerEntero("ID del objeto a quitar: ", 1, 100000);
            if (quitarObjetoEquipaje(v, idObj)) {
                printf("Objeto quitado del equipaje.\n");
            } else {
                printf("El objeto no estaba en el equipaje.\n");
            }
        }
    }

    printf("Viajero actualizado.\n");
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
int main(){
Aduana unoAduana;

    if(!crearAduana(&unoAduana, 2)){
        printf("Error al crear aduana\n");
        return 1;
    }

printf("=== ADUANA ===\n");
printf("Especies: %d\n", unoAduana.capEspecies);
printf("Planetas: %d\n", unoAduana.capPlanetas);
printf("Objetos: %d\n", unoAduana.capObjetos);
printf("Reglas: %d\n", unoAduana.capRegla);
printf("Viajeros: %d\n", unoAduana.capViajeros);
printf("Incidentes: %d\n", unoAduana.capIncidentes);
printf("\n=== CONTADORES ===\n");
printf("Especies registradas: %d\n", unoAduana.cantEspecies);
printf("Planetas registrados: %d\n", unoAduana.cantPlanetas);
printf("Objetos registrados: %d\n", unoAduana.cantObjetos);
printf("Reglas registradas: %d\n", unoAduana.cantRegla);
printf("Viajeros registrados: %d\n", unoAduana.cantViajeros);
printf("Incidentes registrados: %d\n", unoAduana.cantIncidentes);
    liberarAduana(&unoAduana);
    Viajero *viajero = crearViajero(2);

    if(viajero == NULL){
        printf("Error al crear viajero\n");
        return 1;
    }

    printf("Viajero creado correctamente\n");
    printf("Objetos: %d\n", viajero->numObjetos);
    printf("Capacidad: %d\n", viajero->capacidad);
    Objeto objeto1 = {"Laser", 1, 5, 100};
    Objeto objeto2 = {"Espada", 2, 3, 101};
    Objeto objeto3 = {"Escudo", 3, 2, 102};

    agregarObjetoEquipaje(viajero, &objeto1);
    agregarObjetoEquipaje(viajero, &objeto2);

    printf("=== EQUIPAJE ===\n");

    for(int i = 0; i < viajero->numObjetos; i++){
        printf("%s\n", viajero->equipaje[i]->nombre);
    }

    printf("\nAgregando tercer objeto...\n");

    if(agregarObjetoEquipaje(viajero, &objeto3)){
        printf("Objeto agregado correctamente\n");
    }

    printf("\n=== EQUIPAJE ACTUALIZADO ===\n");

    for(int i = 0; i < viajero->numObjetos; i++){
        printf("%s\n", viajero->equipaje[i]->nombre);
    }

    printf("\nObjetos: %d\n", viajero->numObjetos);
    printf("Capacidad: %d\n", viajero->capacidad);
    printf("=== EQUIPAJE ORIGINAL ===\n");

for(int i = 0; i < viajero->numObjetos; i++){
    printf("%s\n", viajero->equipaje[i]->nombre);
}

if(quitarObjetoEquipaje(viajero, 2)){
    printf("\nObjeto eliminado correctamente\n");
}

printf("\n=== EQUIPAJE ACTUALIZADO ===\n");

for(int i = 0; i < viajero->numObjetos; i++){
    printf("%s\n", viajero->equipaje[i]->nombre);
}

printf("\nObjetos: %d\n", viajero->numObjetos);
printf("Capacidad: %d\n", viajero->capacidad);

    liberarViajero(viajero);

    printf("Memoria liberada correctamente\n");

    Especie *buscarEspecie(Aduana *a, int id, const char *nombre);
Planeta *buscarPlaneta(Aduana *a, int id, const char *nombre);
Objeto *buscarObjeto(Aduana *a, int id, const char *nombre);
Viajero *buscarViajero(Aduana *a, int id, const char *nombre);

void listarEspecies(const Aduana *a);
void listarPlanetas(const Aduana *a);
void listarObjetos(const Aduana *a);
void listarViajeros(const Aduana *a);

int modificarEspecie(Aduana *a);
int modificarObjeto(Aduana *a);
int modificarViajero(Aduana *a);

int eliminarEspecie(Aduana *a);
int eliminarObjeto(Aduana *a);
int eliminarViajero(Aduana *a);
return 0;
}
