#include <stdio.h>
typedef struct{
char nombre[30];
int bonus;
int vigencia;
} Cancion;

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



int esta_activa(const Idol *p){
   if (p->energia>0){
printf("Está activa\n");
return 1;
}else{
printf("Está inactiva\n");
return 0;
 }
}


int colaboracion(Idol *a, Idol *b){
    if(a==NULL || b==NULL|| a == b){
        return 0;
    }else{
       a->popularidad +=10; 
       b->popularidad +=10;
       a->energia -=15; 
       b->energia -=15;
       a->energia= limitar(a->energia,0,a->energia_max);
       b->energia= limitar(b->energia,0,b->energia_max);
       printf("%s colaboro con %s\n", a->nombre,b->nombre);
       return 1;
    }
    
    
}

int main(){
    Cancion discos[2]={{"Starlight", 10, 3}, {"Nova", 5, 6}};
    
    
    
Idol Jennie = {"Jennie",100,100,100,100};
Idol Rose = {"Rose",100,100,100,100};
Idol *p= &Jennie;
printf("%s\n", (*p).nombre);
printf("%d\n", (*p).popularidad);
printf("%d\n", p->energia);
printf("%d\n", (*p).energia_max);
printf("%d\n", (*p).fans);

mostrar(&Jennie);
for(int i=0; i<=20; i++){ensayar(&Jennie);}
mostrar(&Jennie);
esta_activa(&Jennie);
for(int i=0; i<=20; i++){descansar(&Jennie);}
mostrar(&Jennie);
esta_activa(&Jennie);
mostrar(&Rose);
colaboracion(&Jennie, &Jennie);
colaboracion(NULL, &Rose);
colaboracion(&Jennie, &Rose);
mostrar(&Jennie),mostrar(&Rose);

Idol *turno= &Jennie;
Idol *otra= &Rose;

while(esta_activa(turno) && esta_activa(otra)&& turno->energia >= 10 && otra->energia >= 10){
    printf("%s está ensayando\n", turno->nombre);
    ensayar(turno);
    Idol *tmp=turno;
    turno=otra;
    otra=tmp;
}

const Idol *ganador = esta_activa(turno) ? turno : otra;
    printf("\n%s conserva energia\n", ganador->nombre);
    printf("\n--- FIN DEL ENSAYO ---\n");

if (Jennie.energia > 0 && Rose.energia == 0) {
    printf("Jennie conserva energía\n");
} else if (Rose.energia > 0 && Jennie.energia == 0) {
    printf("Rose conserva energía\n");
} else if (Jennie.energia > 0 && Rose.energia > 0) {
    printf("Ambas conservan energía, pero una ya no puede ensayar\n");
} else {
    printf("Ninguna conserva energía\n");
}

mostrar(&Jennie);
mostrar(&Rose);



return 0;
}
