#include<stdio.h>
typedef struct{
char nombre[30];
int  vida;
int vida_max;
int ataque;
int defensa;
}Personaje;
void mostrar(const Personaje *p){
 printf("%s Vida: %d/%d Atq: %d Def:%d\n",p->nombre,p->vida,p->vidq_max,p->ataque,p->defensa);


}
int main(){
Personaje heroe = ("Aria", 100, 100, 18, 5);
Personaje *p = &heroe;
printf("%d/n", heroe.vida);
printf("%d\n", (*p).vida);
printf("%d\n", p->vida);
printf("%p %p\n", (void*)p,(void*)&heroe);

mostrar(&heroe);
return 0;

}
