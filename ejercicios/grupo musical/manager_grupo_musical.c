#include <stdio.h>
typedef struct{
char nombre[30];
int popularidad;
int energia;
int energia_max;
int fans;
//mostrar jean







//limitar un rango Oswaldo 








//ensayar nikel


}Idol;


int main(){
Idol integrante1 = {"Jennie",100,100,100,100};
Idol *p= &integrante1;
printf("%s\n", (*p).nombre);
printf("%d\n", (*p).popularidad);
printf("%d\n", p->energia);
printf("%d\n", (*p).energia_max);
printf("%d\n", (*p).fans);



return 0;
}
