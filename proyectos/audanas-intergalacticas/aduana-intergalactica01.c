#include<stdio.h>
#include<stdlib.h>

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
int Permitido;
} Regla;
typedef struct{
char mombre[30];
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

    return 0;
return 0;
}
