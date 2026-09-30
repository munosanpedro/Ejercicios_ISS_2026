#include<stdio.h>
typedef struct{
int numerador;
int denominador;
}Fraccion;
Fraccion suma(Fraccion x, Fraccion y);
Fraccion resta(Fraccion x, Fraccion y);
Fraccion multiplicacion(Fraccion x, Fraccion y);
Fraccion division(Fraccion x, Fraccion y);
Fraccion simplifica(Fraccion x, Fraccion y);
int main(){
Fraccion f1, f2;
printf("Ingresa el nunerador de la primer fraccion");
scanf("%d",&f1.numerador);
printf("Ingresa el denominador  de la primer fraccion");
scanf("%d",&f1.denominador);
printf("Ingresa el nunerador de la 2da fraccion");
scanf("%d",&f2.numerador);
printf("Ingresa el denominador  de la 2da fraccion");
scanf("%d",&f2.denominador);

Fraccion resultado=suma(f1,f2);

printf("la suma es: %d / %d\n", resultado.numerador,resultado.denominador); 

resultado=resta(f1,f2);
printf("La resta es %d/%d\n", resultado.numerador,resultado.denominador);

 resultado=multiplicacion(f1,f2);
printf("La multiplicación es %d/%d\n", resultado.numerador,resultado.denominador);


 resultado=division(f1,f2);
printf("La división es %d/%d\n", resultado.numerador,resultado.denominador);


return 0;
/* 
planificacion
 historia de usuarii
 eztimacion de tiempo
Analisis
 diagrama de fkuji x funcion
Codificacion
 github 
Pruebas
 probar en criteriis de aceptacion 
post morten

*/

}
Fraccion suma(Fraccion a,Fraccion b){
Fraccion resultado;
resultado.numerador= (a.numerador * b.denominador)+(b.numerador*a.denominador);
resultado.denominador= (a.denominador*b.denominador);
return resultado;
}
Fraccion resta(Fraccion a,Fraccion b){
Fraccion resultado;
resultado.numerador= (a.numerador *b.denominador)-(b.numerador*a.denominador);
resultado.denominador= (a.denominador*b.denominador);
return resultado;
}
Fraccion multiplicacion(Fraccion a,Fraccion b){
Fraccion resultado;
resultado.numerador= (a.numerador * b.numerador);
resultado.denominador= (a.denominador*b.denominador);
return resultado;
}
Fraccion division(Fraccion a,Fraccion b){
Fraccion resultado;
resultado.numerador= (a.numerador * b.denominador);
resultado.denominador= (a.denominador*b.numerador);
return resultado;
}
Fraccion simplifica(Fraccion a,Fraccion b){
Fraccion resultado;
resultado.numerador= (a.numerador * b.denominador)+(b.numerador*a.denominador);
resultado.denominador= (a.denominador*b.denominador);
return resultado;
}
