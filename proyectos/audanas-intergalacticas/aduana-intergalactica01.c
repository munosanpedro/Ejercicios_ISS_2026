#include<stdio.h>
#include<stdlib.h>

typedef struct{
    char nombre[50];
    int id;
    int nvPeligro;
} Especie;

typedef struct{
    char nombre[20];
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
] Regla;
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

int main()
{

return 0;
}
