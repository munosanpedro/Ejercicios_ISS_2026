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
Cancion *cancion;
} Idol;

typedef struct {
    char nombre[30];
    Idol miembros[4];
    int n;
    Idol *lider;
} Grupo;


void mostrar (const Idol *p){

printf(" nombre: %s \n Popularidad: %d \n Energia: %d\n Fans: %d\n", p->nombre, p->popularidad, p->energia, p->fans);
}





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

void asignar_cancion(Idol *i, Cancion *c) {
    if (i == NULL) {
        return;
    }

    i->cancion = c;
}

int popularidad_total(const Idol *i){
    if (i == NULL) return 0;
    if (i->cancion != NULL && i->cancion->vigencia > 0) {
        return i->popularidad + i->cancion->bonus;
    }
    return i->popularidad;
}

int cambiar_lider(Grupo *g, Idol *nueva) {
    if (g == NULL || nueva == NULL) return 0;
    for (int i = 0; i < g->n; i++) {
        if (&(g->miembros[i]) == nueva) {
            g->lider = nueva;
            return 1;
        }
    }
    return 0;
}

void mostrar_grupo(const Grupo *g) {
    if (g == NULL) return;
    printf("=== Grupo: %s ===\n", g->nombre);
    Idol *p = (Idol *)g->miembros;
    Idol *fin = g->miembros + g->n;
    while (p < fin) {
        mostrar(p);
        p++;
    }
}

int contar_activas(const Grupo *g) {
    if (g == NULL || g->n == 0) return 0;
    int count = 0;
    Idol *p = g->miembros;
    Idol *fin = g->miembros + g->n;
    while (p < fin) {
        if (p->energia > 0) {
            count++;
        }
        p++;
    }
    return count;
}

Idol *idol_mas_cansada(Grupo *g) {
    if (g == NULL || g->n == 0) return NULL;
    Idol *mas_cansada = &(g->miembros[0]);
    Idol *p = g->miembros + 1;
    Idol *fin = g->miembros + g->n;
    while (p < fin) {
        if (p->energia < mas_cansada->energia) {
            mas_cansada = p;
        }
        p++;
    }
    return mas_cansada;
}

void dar_concierto(Grupo *g) {
    if (g == NULL) return;
    printf("\n--- ¡INICIO DE CONCIERTO PARA EL GRUPO %s! ---\n", g->nombre);
    Idol *p = g->miembros;
    Idol *fin = g->miembros + g->n;
    while (p < fin) {
        if (p->energia > 0) {
            int pop_tot = popularidad_total(p);
            int fans_ganados = pop_tot * 10;
            p->fans += fans_ganados;
            p->energia -= 20;
            p->energia = limitar(p->energia, 0, p->energia_max);
            
            printf("- %s participo en el concierto, gano %d fans y su energia actual es %d.\n", p->nombre, fans_ganados, p->energia);
            if (p->cancion != NULL) {
                if (p->cancion->vigencia > 0) {
                    p->cancion->vigencia--;
                    if (p->cancion->vigencia == 0) {
                        printf("  -> ¡La cancion '%s' ya no esta vigente!\n", p->cancion->nombre);
                    }
                }
            }
        }
        p++;
    }
    printf("--- FIN DEL CONCIERTO ---\n\n");
}

void mostrar_cancion_lider(const Grupo *g) {
    if (g == NULL) {
        printf("Error: El grupo no existe (es NULL).\n");
        return;
    }
    if (g->lider == NULL) {
        printf("El grupo '%s' no tiene una lider asignada actualmente.\n", g->nombre);
        return;
    }
    if (g->lider->cancion == NULL) {
        printf("La lider '%s' no tiene ninguna cancion asignada.\n", g->lider->nombre);
        return;
    }
    printf("La lider '%s' interpreta la cancion: %s\n", g->lider->nombre, g->lider->cancion->nombre);
}

int main(){
    Cancion discos[2]={{"Starlight", 10, 3}, {"Nova", 5, 6}};
    Grupo lumina = {
        "Lumina",
        {
            {"Luna", 80, 50, 100, 90, NULL},
            {"Mina", 85, 20, 100, 95, NULL},
            {"Nei", 100, 100, 100, 100, NULL}
        },3,NULL
    };
    asignar_cancion(&lumina.miembros[0], &discos[0]);
    asignar_cancion(&lumina.miembros[2], &discos[0]);
    
    
    
Idol Jennie = {"Jennie",100,100,100,100,NULL};
Idol Rose = {"Rose",100,100,100,100,NULL};

Idol *p= &Jennie;
printf("%s\n", (*p).nombre);
printf("%d\n", (*p).popularidad);
printf("%d\n", p->energia);
printf("%d\n", (*p).energia_max);
printf("%d\n", (*p).fans);

mostrar(&Jennie);
for(int i=1; i<=20; i++){ensayar(&Jennie);}
mostrar(&Jennie);
esta_activa(&Jennie);
for(int i=1; i<=20; i++){descansar(&Jennie);}
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
printf("%s Cancion: %s\n", Jennie.nombre, Jennie.cancion->nombre);
printf("%s Cancion: %s\n", Rose.nombre, Rose.cancion->nombre);
asignar_cancion(&Jennie, &discos[0]);
asignar_cancion(&Rose, &discos[0]);
printf("%s Cancion: %s\n", Jennie.nombre, Jennie.cancion->nombre);
printf("%s Cancion: %s\n", Rose.nombre, Rose.cancion->nombre);


discos[0].vigencia = 0;
printf("Popularidad total %s (cancion sin vigencia): %d\n",Jennie.nombre, popularidad_total(&Jennie));


if (cambiar_lider(&lumina, &lumina.miembros[0])) {
        printf("Lider asignado correctamente. Fans del lider: %d\n", lumina.lider->fans);
    }
    mostrar_grupo(&lumina);
    printf("Integrantes activas: %d\n", contar_activas(&lumina));


Idol *cansada = idol_mas_cansada(&lumina);
    if (cansada != NULL) {
        printf("Integrante mas cansada: %s (Energia: %d)\n", cansada->nombre, cansada->energia);
        descansar(cansada);
        printf("Energia tras descansar: %d\n", cansada->energia);
    }
    
Grupo grupo_sin_lider = {"Ghost", {{"A", 50, 50, 100, 10, NULL}}, 1, NULL};
mostrar_cancion_lider(&grupo_sin_lider);

cambiar_lider(&lumina, &lumina.miembros[1]); // Jisoo es la líder
lumina.lider->cancion = NULL;
mostrar_cancion_lider(&lumina);
cambiar_lider(&lumina, &lumina.miembros[0]); // Luna es la líder
asignar_cancion(lumina.lider, &discos[0]);
discos[0].vigencia = 3; // Restauramos temporalmente para una pruebita
mostrar_cancion_lider(&lumina);
dar_concierto(&lumina);
dar_concierto(&lumina);
return 0;
}
