#include <stdio.h>
typedef struct{
char nombre[30];
int popularidad;
int energia;
int energia_max;
int fans;
} Idol;
//mostrar jean
void mostrar (const Idol *p){

printf(" nombre: %s \n Popularidad: %d \n Energia: %d\n Fans: %d\n", p->nombre, p->popularidad, p->energia, p->fans);
}




//limitar un rango Oswaldo 
int limitar(int valor, int minimo, int maximo){
    if(valor < minimo) return minimo;
    if(valor > maximo) return maximo;
    return valor;
}






void ensayar(Idol *i){
    if(i->energia >= 10){
        i->popularidad += 10;
        i->energia -= 10;
        i->popularidad = limitar(i->popularidad, 0, 100);
        i->energia = limitar(i->energia, 0, i->energia_max);






//descansar oswaldo
void descansar(idol *p, int horas){
p->energia = limitar(p->energia + horas * 10, 0, p->energia_max);
printf("%s descanso %d horas\n", p->nombre, horas);
}









int main(){
Idol integrante1 = {"Jennie",100,100,100,100};
Idol *p= &integrante1;
printf("%s\n", (*p).nombre);
printf("%d\n", (*p).popularidad);
printf("%d\n", p->energia);
printf("%d\n", (*p).energia_max);
printf("%d\n", (*p).fans);

mostrar(&integrante1);

return 0;
}
