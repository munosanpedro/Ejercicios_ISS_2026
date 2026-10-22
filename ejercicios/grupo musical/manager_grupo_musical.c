#include <stdio.h>
typedef struct{
char nombre[30];
int popularidad;
int energia;
int energia_max;
int fans;

} Idol;
//mostrar jean
void mostrar (const Idol *p)
void ensayar(Idol *i);{

printf(" nombre: %s \n Popularidad: %d \n Energia: %d\n Fans: %d\n", p->nombre, p->popularidad, p->energia, p->fans);
}




//limitar un rango Oswaldo 
int limitar(int valor, int minimo, int maximo){
    if(valor < minimo) return minimo;
    if(valor > maximo) return maximo;
    return valor;
}







void ensayar(Idol *i) {
    i->popularidad = limitar(i->popularidad + 10, 0, 100);

    printf("\n%s ha ensayado.\n", i->nombre);
    printf("Nueva popularidad: %d\n", i->popularidad);
}

int main() {
    Idol integrante1 = {"Jennie", 100, 100, 100, 100};
    Idol *p = &integrante1;





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
