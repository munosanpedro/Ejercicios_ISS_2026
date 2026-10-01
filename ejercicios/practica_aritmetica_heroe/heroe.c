#include <stdio.h>

typedef struct {
 char nombre[20];
 int ataque; // bonus al ataque
 int durabilidad; // 0 = rota
} Arma;

typedef struct {
 char nombre[30];
 int vida;
 int vida_max;
 int ataque;
 int defensa;
 Arma *arma; // Apuntador al arma equipada (puede ser NULL)
} Personaje;


int ataque_total(const Personaje *p) {
 if (p->arma != NULL && p->arma->durabilidad > 0) {
    return p->ataque + p->arma->ataque;
 }
 return p->ataque;
}

void mostrar(const Personaje *p){
 printf("%s Vida: %d/%d Atq: %d Def:%d\n",p->nombre,p->vida,p->vida_max,p->ataque,p->defensa);
}

int limitar(int valor, int minimo, int maximo){
    if(valor < minimo) return minimo;
    if(valor > maximo) return maximo;
    return valor;
}

void recibir_damage(Personaje *p, int damage){
    p->vida = limitar(p->vida - damage, 0 , p->vida_max);
}

int estar_vivo(Personaje *p){
    if(p->vida > 0) return 1; // Error corregido: era p->vida, no p--vida
    return 0;
}

// Se agregó la función equipar que faltaba pero era llamada en el main
void equipar(Personaje *p, Arma *a){
    p->arma = a;
}

int atacar(Personaje *atacante, Personaje *defensor){
    if (atacante == NULL || defensor == NULL || atacante == defensor){
        return 0;
    }
    
    
    int damage = ataque_total(atacante) - defensor->defensa;
    
    if(damage < 1) damage = 1;
    recibir_damage(defensor, damage);
    
   
    printf("%s golpea a %s por %d de dano.\n", atacante->nombre, defensor->nombre, damage );
    
    if (atacante->arma != NULL && atacante->arma->durabilidad > 0) {
        atacante->arma->durabilidad--;
        if (atacante->arma->durabilidad == 0) {
            printf("¡El arma %s de %s se ha roto!\n", atacante->arma->nombre, atacante->nombre);
        }
    }
    return damage;
}
void mostrar_equipo(const Personaje *equipo, int n) {
 for (const Personaje *p = equipo; p < equipo + n; p++) {
 mostrar(p);
 }
}
Personaje *mas_debil(Personaje *equipo, int n) {
 Personaje *menor = NULL;
 for (Personaje *p = equipo; p < equipo + n; p++) {
 if (esta_vivo(p) && (menor == NULL || p->vida < menor->vida)) {
 menor = p;
 }
 }
 return menor;
}

int main(){
    Arma armeria[3] = {
     {"Espada vieja", 10, 2}, 
     {"Hacha de guerra", 15, 5},
     {"Daga ligera", 5, 10}
    };
    
    
    Personaje aria = {"Aria", 100, 100, 18, 5, NULL};
    Personaje orco = {"Orco", 120, 120, 12, 3, NULL};
    
    // Equipar armas usando la función (apuntando a elementos de armeria)
    equipar(&aria, &armeria[0]); // Aria se equipa la Espada vieja
    atacar(&aria, &orco);
Personaje *objetivo = mas_debil(monstruos, 3);
if (objetivo != NULL) atacar(&heroes[0], objetivo);

    Personaje heroe = {"Aria", 100, 100, 18, 5, NULL};
    Personaje *p = &heroe;
    printf("%d\n", heroe.vida);
    printf("%d\n", (*p).vida);
    printf("%d\n", p->vida);
    printf("%p %p\n", (void*)p,(void*)&heroe);
    mostrar(&heroe);
    
    Personaje prueba = {"Orco", 100, 100, 18, 5, NULL};
    p = &prueba; 
    printf("%d\n", prueba.vida);
    printf("%d\n", (*p).vida);
    printf("%d\n", p->vida);
    printf("%p %p\n", (void*)p,(void*)&prueba);
    mostrar(&prueba);

    Personaje *turno = &aria;
    Personaje *otro = &orco;
    
    
    while (estar_vivo(turno) && estar_vivo(otro)){
        atacar(turno, otro);
        Personaje *tmp = turno;
        turno = otro;
        otro = tmp;
    }

    mostrar(&heroe);
    recibir_damage(&heroe, 30);
    mostrar(&heroe);
    
    return 0;
}
