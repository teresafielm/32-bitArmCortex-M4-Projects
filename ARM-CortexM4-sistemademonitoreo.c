/* TAREA-EXAMEN NO.3
* Realizado por: Fiel Muñoz Teresa Elpidia
*/
//------------------------------------LIBRERIAS DEL PROGRAMA-----------------------------------
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
volatile uint32_t Count= 0;
volatile uint32_t Termino= 0;
//-----------------------------------------------DEFINICIÓN DE VARIABLES----------------------------------------------
-----
//-------------------TECLADO MATRICIAL--------------------------
#define GPIO_PORTK_DATA_R (*((volatile unsigned long *)0x400613FC)) // Registro de Datos Puerto K
#define GPIO_PORTK_DIR_R (*((volatile unsigned long *)0x40061400)) // Registro de Dirección Puerto K
#define GPIO_PORTK_DEN_R (*((volatile unsigned long *)0x4006151C)) // Registro de Habilitación Puerto K
#define GPIO_PORTK_PDR_R (*((volatile unsigned long *)0x40061514))// Registro de Pull-Down Puerto K
//------------------DISPLAY 7 SEGMENTOS-------------------------
#define GPIO_PORTM_DATA_R (*((volatile unsigned long *)0x400633FC)) // Registro de Datos Puerto M
#define GPIO_PORTM_DIR_R (*((volatile unsigned long *)0x40063400)) // Registro de Dirección Puerto M
#define GPIO_PORTM_DEN_R (*((volatile unsigned long *)0x4006351C)) // Registro de Habilitación Puerto M
#define GPIO_PORTM_PDR_R (*((volatile unsigned long *)0x40063514))// Registro de Pull-Down Puerto M
//-------------------HAB. DISPLAY 7 SEGMENTOS-------------------
#define GPIO_PORTE_DATA_R (*((volatile unsigned long *)0x4005C3FC)) // Registro de Datos Puerto E
#define GPIO_PORTE_DIR_R (*((volatile unsigned long *)0x4005C400)) // Registro de Dirección Puerto E
#define GPIO_PORTE_DEN_R (*((volatile unsigned long *)0x4005C51C)) // Registro de Habilitación Puerto E
#define GPIO_PORTE_PUR_R (*((volatile unsigned long *)0x4005C510)) // Registro de Pull-Up Puerto E
//---------------------------------------------BUZZER PASIVO--------------------------------------------
#define GPIO_PORTQ_DATA_R (*((volatile unsigned long *)0x400663FC)) // Registro de Datos Puerto K
#define GPIO_PORTQ_DIR_R (*((volatile unsigned long *)0x40066400)) // Registro de Dirección Puerto K
#define GPIO_PORTQ_DEN_R (*((volatile unsigned long *)0x4006651C)) // Registro de Habilitación Puerto K
#define GPIO_PORTQ_PDR_R (*((volatile unsigned long *)0x40066514))// Registro de Pull-Down Puerto K
int i,frec,valor,r,config; // Datos enteros
int frequen[]={0,0,0}; // Datos enteros
int fmuestrap[]={0,0,0,0}; // Datos enteros
int fmuestrat[]={0,0,0,0}; // Datos enteros
int fmuestral[]={0,0,0,0}; // Datos enteros
int vmuestrap[]={0,0,0,0}; // Datos enteros
int vmuestrat[]={0,0,0,0}; // Datos enteros
int vmuestral[]={0,0,0,0}; // Datos enteros
int values[]={0,0,0}; // Datos enteros
int graditos,muestras,p=0,teclaA=0; // Variables de grados en pasos y los
valores registrados de muestras
int digitos[]={0x00,0x00,0x00,0x00}; // Los digitos reales que se registran en
7seg
int digmuestra[]={0,0,0,0}; // Digitos de muestra del teclado 
// Valor entero para saber si es o no negativo el giro
int digreal[]={0,0,0,0}; //Guarda dígitos reales
int contdig=0x00; //Contador de dígitos ingresados
//--------------------RELOJ INTERNO-----------------------------
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE608)) // Registro de Habilitación de Reloj de
Puertos
#define SYSCTL_PRGPIO_R (*((volatile unsigned long *)0x400FEA08)) // Registro de estatus de Reloj de Puerto
// Definición de constantes para operaciones
#define SYSCTL_RCGC2_GPION 0x00005E30 // bit de estado del reloj
main(void)
{
 SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPION; // Activa el reloj
 while ((SYSCTL_PRGPIO_R & 0X1000) == 0){}; // reloj listo?
 GPIO_PORTQ_DIR_R |= 0x01; // Salida PQ0
 GPIO_PORTK_DIR_R |= 0xF0; // Salidas PK4-PK7 y Entradas PK0-PK3
 GPIO_PORTM_DIR_R |= 0xFF; // Salidas de PM0-PM7
 GPIO_PORTE_DIR_R |= 0x0F; // Salidas de PE0-PE3
 GPIO_PORTQ_DEN_R |= 0x01; // Habilita PKQ0
 GPIO_PORTK_DEN_R |= 0xFF; // Habilita PK0-PK7
 GPIO_PORTM_DEN_R |= 0xFF; // Habilita PM0-PM7
 GPIO_PORTE_DEN_R |= 0x0F; // Habilita PE0-PE3
 GPIO_PORTQ_PDR_R |= 0x01; // Habilita resistencias de pull-down para BUZZER PASIVO
 GPIO_PORTK_PDR_R |= 0x0F; // Habilita resistencias de pull-down para salidas del teclado matricial
 GPIO_PORTM_PDR_R |= 0xFF; // Habilita resistencias de pull-down para salidas del display 7 segmentos
 GPIO_PORTE_PUR_R |= 0x0F; // Habilita resistencias de pull-up para digitos de habilitación 7 segmentos
 r=0; //Incio de configuración
 config=0; //Inicio de configuración
 while(1)
 {
 valor=digreal[3]+digreal[2]*10+digreal[1]*100+digreal[0]*1000; //Calcula dígito total con dígitos ingresados
 /*Fila 1*/
 GPIO_PORTK_DATA_R=0x80; // Se multiplexa la primera fila del teclado
 if(GPIO_PORTK_DATA_R==0x88) // Presiona tecla 1
 {
 if(contdig==4) // Si ya se ingresaron cuatro digitos del display se reincia
 {
 contdig=0x00; // Cuenta reiniciada
 }
 if(contdig<=3){ // Si la cuenta es menor a 3 se ingresa el dato
 digitos[contdig]=0x30; // Valor del display 7 segmentos
 digreal[contdig]=1; // El digito real es 1
 contdig+=1; // Se incrementa la cuenta
 for(i=0;i<=0x100000;i++){}// Valor de espera
 }
 }
 else if(GPIO_PORTK_DATA_R==0x84)// Valor 2 leído
 {
 if(contdig==4)// Se reinician los displays
 {
 contdig=0x00;//El contador se reinicia
 }
 if(contdig<=3){// Si la cuenta es menor a 3 se ingresa el dato
 digitos[contdig]=0x6D;// Valor del display 7 segmentos
 digreal[contdig]=2; //El digito real es 2
 contdig+=1;// Se incrementa la cuenta
 for(i=0;i<=0x100000;i++){}// Valor de espera
 }
 }
 else if(GPIO_PORTK_DATA_R==0x82) // Valor 3 del teclado
 {
 if(contdig==4)// Si la cuenta es 4 del display
 {
 contdig=0x00;// Se reinicia el valor de cuenta
 }
 if(contdig<=3){// Si la cuenta es tres o menor
 digitos[contdig]=0x79;// Valor del display 7 segmentos
 digreal[contdig]=3;// El digito real es 3
 contdig+=1; // Se incrementa la cuenta
 for(i=0;i<=0x100000;i++){} // Valor de espera
 }
 }
 else if(GPIO_PORTK_DATA_R==0x81) // Tecla A presionada
 {
 /*Presencia*/
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x5B;//Enciende dígito 4 S
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x4F;//Enciende dígito 3 E
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x05;//Enciende dígito 2 R
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x67;//Enciende dígito 1 P
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=fmuestrap[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=fmuestrap[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=fmuestrap[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=fmuestrap[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 4 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 3 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x7E;//Enciende dígito 2 O
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 /*Temperatura*/
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x67;//Enciende dígito 4 P
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x37;//Enciende dígito 3 M
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x4F;//Enciende dígito 2 E
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x0F;//Enciende dígito 1 T
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=fmuestrat[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=fmuestrat[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=fmuestrat[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=fmuestrat[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=vmuestrat[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=vmuestrat[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=vmuestrat[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=vmuestrat[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 4 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 3 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x7E;//Enciende dígito 2 O
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 /*Luz*/
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x6D;//Enciende dígito 4 S
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x0E;//Enciende dígito 3 U
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x3E;//Enciende dígito 2 L
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1 0
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=fmuestral[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=fmuestral[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=fmuestral[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=fmuestral[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=vmuestral[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=vmuestral[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=vmuestral[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=vmuestral[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 4 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 3 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x7E;//Enciende dígito 2 O
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 frec=valor*10;
 frecuencia(frec);
 }
 /* Fila 2 */
 GPIO_PORTK_DATA_R=0x40; // Se multiplexa la segunda fila
 if(GPIO_PORTK_DATA_R==0x48) // Valor 4 presionado
 {
 if(contdig==4) // Si ya se registraron todos los dígitos el conteo reinicia
 {
 contdig=0x00; // Se reinicia
 }
 if(contdig<=3){ // Si aún se pueden ingresar dígitos
 digitos[contdig]=0x33; // Valor 4 en 7 seg
 digreal[contdig]=4; // Valor 4 en decimal
 contdig+=1; // Se incrementa el conteo en 1
 for(i=0;i<=0x100000;i++){} // Ciclo de espera
 }
 }
 else if(GPIO_PORTK_DATA_R==0x44) // Tecla 5 presionada
 {
 if(contdig==4) // Si ya se ingresaron todos los digitos
 {
 contdig=0x00; // El valor se reinicia
 }
 if(contdig<=3){ // Si aún se pueden ingresar datos
 digitos[contdig]=0x5B; // Valor 7 segmentos de 5
 digreal[contdig]=5; // Valor real 5
 contdig+=1; // Se incrementa el conteo
 for(i=0;i<=0x100000;i++){} // Ciclo de espera
 }
 }
 else if(GPIO_PORTK_DATA_R==0x42) // Valor 6 presionado
 {
 if(contdig==4) // Si ya se ingresaron todos los digitos
 {
 contdig=0x00; // Se reinicia el valor
 }
 if(contdig<=3){// Si aun se pueden ingresar digitos
 digitos[contdig]=0x5F;// Se ingresa el valor 5 7segm
 digreal[contdig]=6;// Se ingresa el valor 6 decimal
 contdig+=1;// Se incrementa en 1 el contador
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 }
 }
 else if(GPIO_PORTK_DATA_R==0x41)// Se configuró tecla B como # por fallas en mi teclado
 {
 r=1; //Presencia
 for(i=0;i<=0x100000;i++){}//Espera
 }
 /* Fila 3 */
 GPIO_PORTK_DATA_R=0x20; // Se multiplexa la fila 3
 if(GPIO_PORTK_DATA_R==0x28)// Valor 7 leído
 {
 if(contdig==4)// Si ya no se pueden ingresar más digitos
 {
 contdig=0x00;// Se reinicia el conteo
 }
 if(contdig<=3){// Si aún se pueden ingresar dígitos
 digitos[contdig]=0x70;// Se ingresan los valores en 7seg
 digreal[contdig]=7;// Se ingresan los valores decimales
 contdig+=1;// Se incrementa el contador
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 }
 }
 else if(GPIO_PORTK_DATA_R==0x24)// Valor 8 leído
 {
 if(contdig==4)// Si ya no se ingresan más datos
 {
 contdig=0x00;// Se reinicia el conteo
 }
 if(contdig<=3){// Si aún se pueden ingresar más datos
 digitos[contdig]=0x7F;// Valor 8 en 7seg
 digreal[contdig]=8;//Valor 8 en decimal
 contdig+=1;// Se incrementa el conteo
 for(i=0;i<=0x100000;i++){}//Ciclo de espera
 }
 }
 else if(GPIO_PORTK_DATA_R==0x22)// Tecla 9 presionada
 {
 if(contdig==4) // Si ya no pueden ingresarse datos
 {
 contdig=0x00;// Se reincia el conteo
 }
 if(contdig<=3){// Si aún se pueden ingresar datos
 digitos[contdig]=0x7B;// Valor 9 7seg
 digreal[contdig]=9;// Valor 9 decimal
 contdig+=1;// Se incrementa el conteo en 1
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 }
 }
 else if (GPIO_PORTK_DATA_R==0x21) // Si se presiona tecla C
 {
 r=2; // Temperatura
 for(i=0;i<=0x100000;i++){}//Aguanta
 }
 /* Fila 4 */
 GPIO_PORTK_DATA_R=0x10;// Multiplexación fila 4
 if(GPIO_PORTK_DATA_R==0x18)// Si se teclea #
 {
 // No se hizo nada porque se implemento en la tecla B
 }
 else if(GPIO_PORTK_DATA_R==0x14)// Si se teclea 0
 {
 if(contdig==4)// Si ya no se pueden ingresar datos
 {
 contdig=0x00;// Se reincia el conteo
 }
 if(contdig<=3){// Si aún se pueden ingresar datos
 digitos[contdig]=0x7E;// Valor 0 en 7segm
 digreal[contdig]=0;// Valor 0 decimal
 contdig+=1;// Se incrementa el conteo
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 }
 }
 else if(GPIO_PORTK_DATA_R==0x12)// Se presiona *
 {
 if(config==0)
 {
 if(r==1)
 {
 frequen[0]=valor; //Frecuencia igual al valor ingresado
 fmuestrap[0]=digitos[0]; //Guarda valor en frecuencia de sensor
 fmuestrap[1]=digitos[1]; //Guarda valor en frecuencia de sensor
 fmuestrap[2]=digitos[2];//Guarda valor en frecuencia de sensor
 fmuestrap[3]=digitos[3];//Guarda valor en frecuencia de sensor
 }
 else if(r==2)
 {
 frequen[1]=valor;//Frecuencia igual al valor ingresado
 fmuestrat[0]=digitos[0];//Guarda valor en frecuencia de sensor
 fmuestrat[1]=digitos[1];//Guarda valor en frecuencia de sensor
 fmuestrat[2]=digitos[2];//Guarda valor en frecuencia de sensor
 fmuestrat[3]=digitos[3];//Guarda valor en frecuencia de sensor
 }
 else if(r==3)
 {
 frequen[2]=valor;//Frecuencia igual al valor ingresado
 fmuestral[0]=digitos[0];//Guarda valor en frecuencia de sensor
 fmuestral[1]=digitos[1];//Guarda valor en frecuencia de sensor
 fmuestral[2]=digitos[2];//Guarda valor en frecuencia de sensor
 fmuestral[3]=digitos[3];//Guarda valor en frecuencia de sensor
 }
 }
 else if(config==1)
 {
 if(r==1)
 {
 values[0]=valor;//Valor igual al valor ingresado
 vmuestrap[0]=digitos[0];//Guarda valor en frecuencia de sensor
 vmuestrap[1]=digitos[1];//Guarda valor en frecuencia de sensor
 vmuestrap[2]=digitos[2];//Guarda valor en frecuencia de sensor
 vmuestrap[3]=digitos[3];//Guarda valor en frecuencia de sensor
 }
 else if(r==2)
 {
 values[1]=valor;//Valor igual al valor ingresado
 vmuestrat[0]=digitos[0];//Guarda valor en frecuencia de sensor
 vmuestrat[1]=digitos[1];//Guarda valor en frecuencia de sensor
 vmuestrat[2]=digitos[2];//Guarda valor en frecuencia de sensor
 vmuestrat[3]=digitos[3];//Guarda valor en frecuencia de sensor
 }
 else if(r==3)
 {
 values[2]=valor;//Valor igual al valor ingresado
 vmuestral[0]=digitos[0];//Guarda valor en frecuencia de sensor
 vmuestral[1]=digitos[1];//Guarda valor en frecuencia de sensor
 vmuestral[2]=digitos[2];//Guarda valor en frecuencia de sensor
 vmuestral[3]=digitos[3];//Guarda valor en frecuencia de sensor
 }
 }
 config+=1; //Se suma configuración
 if(config==2)
 {
 config=0;//Si es igual a 2 r se reinicia
 r=0;//R también se reinicia
 }
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 }
 else if(GPIO_PORTK_DATA_R==0x11)// Se presiona D
 {
 r=3; //Sensor de Luz
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 }
 /* Muestra de datos */
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=digitos[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=digitos[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=digitos[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=digitos[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
}
void frecuencia(int frecuenciar)//Funcón frecuencia
{
 int f;
 f=16000000/frecuenciar;//Convierte frecuencia en ciclos de reloj
 GPIO_PORTK_DATA_R=0x10;//Monitorea valor de la tecla # ingresada
 while(GPIO_PORTK_DATA_R!=0x18)//Si se teclea #
 {
 for(i=0;i<=f;i++)//Ciclo encendido
 {
 GPIO_PORTQ_DATA_R=0X01;//Enciende buzzer
 }
 for(i=0;i<=f;i++)//Ciclo apagado
 {
 GPIO_PORTQ_DATA_R=0X00;//Apaga buzzer
 }
 }
 /*digitos[0]=0x00;
 digitos[1]=0x00;
 digitos[2]=0x00;
 digitos[3]=0x00;*/
 }