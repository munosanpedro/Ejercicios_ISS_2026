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







//ensayar nikel






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
