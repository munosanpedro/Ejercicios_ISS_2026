#include <stdio.h>
typedef struct{
char nombre[30];
int popularidad;
int energia;
int energia_max;
int fans;
} Idol;

void mostrar (const Idol *p){

printf(" nombre: %s \n Popularidad: %d \n Energia: %d\n Fans: %d\n", p->nombre, p->popularidad, p->energia, p->fans);
}




//limitar un rango Oswaldo 
int limitar(int valor, int minimo, int maximo){
    if(valor < minimo) return minimo;
    if(valor > maximo) return maximo;
    return valor;
}






void ensayar(Idol *p){
    if(p->energia >= 10){
        p->popularidad += 10;
        p->energia -= 10;
        p->popularidad = limitar(p->popularidad, 0, 100);
        p->energia = limitar(p->energia, 0, p->energia_max);
}}







void descansar(Idol *p){
p->energia+=10;
p->energia= limitar(p->energia,0,p->energia_max);
}



void esta_activa(Idol *p){
   if (p->energia>0){
printf("Está activa");
}else{
printf("Está inactiva");
}}


int main(){
Idol integrante1 = {"Jennie",100,100,100,100};
Idol *p= &integrante1;
printf("%s\n", (*p).nombre);
printf("%d\n", (*p).popularidad);
printf("%d\n", p->energia);
printf("%d\n", (*p).energia_max);
printf("%d\n", (*p).fans);

mostrar(&integrante1);
for(int i=0; i<=20; i++){ensayar(&integrante1);}
mostrar(&integrante1);
esta_activa(&integrante1);
for(int i=0; i<=20; i++){descansar(&integrante1);}
mostrar(&integrante1);
esta_activa(&integrante1);

return 0;
}
