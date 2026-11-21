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
    int decicion;
} Viajero;
typedef struct{
    char decision[50];
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
    
return 0;
}
