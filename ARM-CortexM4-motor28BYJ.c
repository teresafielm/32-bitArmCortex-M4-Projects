/* TAREA-EXAMEN NO.4
* Realizado por: Fiel Muñoz Teresa Elpidia
*/
//------------------------------------LIBRERIAS DEL PROGRAMA-----------------------------------
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "inc/tm4c1294ncpdt.h"
volatile uint32_t Count= 0;
volatile uint32_t Termino= 0;
//-----------------------------------------------DEFINICIÓN DE VARIABLES-----------------------
----------------------------
//-------------------TECLADO MATRICIAL--------------------------
#define GPIO_PORTK_DATA_R (*((volatile unsigned long *)0x400613FC)) // Registro de Datos 
Puerto K
#define GPIO_PORTK_DIR_R (*((volatile unsigned long *)0x40061400)) // Registro de 
Dirección Puerto K
#define GPIO_PORTK_DEN_R (*((volatile unsigned long *)0x4006151C)) // Registro de 
Habilitación Puerto K
#define GPIO_PORTK_PDR_R (*((volatile unsigned long *)0x40061514))// Registro de PullDown Puerto K
//------------------DISPLAY 7 SEGMENTOS-------------------------
#define GPIO_PORTM_DATA_R (*((volatile unsigned long *)0x400633FC)) // Registro de Datos 
Puerto M
#define GPIO_PORTM_DIR_R (*((volatile unsigned long *)0x40063400)) // Registro de 
Dirección Puerto M
#define GPIO_PORTM_DEN_R (*((volatile unsigned long *)0x4006351C)) // Registro de 
Habilitación Puerto M
#define GPIO_PORTM_PDR_R (*((volatile unsigned long *)0x40063514))// Registro de PullDown Puerto M
//-------------------HAB. DISPLAY 7 SEGMENTOS-------------------
#define GPIO_PORTE_DATA_R (*((volatile unsigned long *)0x4005C3FC)) // Registro de Datos
Puerto E
#define GPIO_PORTE_DIR_R (*((volatile unsigned long *)0x4005C400)) // Registro de
Dirección Puerto E
#define GPIO_PORTE_DEN_R (*((volatile unsigned long *)0x4005C51C)) // Registro de
Habilitación Puerto E
#define GPIO_PORTE_PUR_R (*((volatile unsigned long *)0x4005C510)) // Registro de PullUp Puerto E
int graditos,muestras; // Variables de
grados en pasos y los valores registrados de muestras
int digmuestra[]={0,0,0,0}; // Digitos de muestra
del teclado
int negativo=0x00; // Valor entero para
saber si es o no negativo el giro
int a,b,c,d;
// RUTINA DE SERVICIO DE INTERRUPCIÓN
void Timer03AIntHandler(void)
{
 //LIMPIA BANDERA
 TIMER3_ICR_R= 0X00000001 ; //LIMPIA BANDERA DE TIMER3
 Termino = Termino + 1; // Se suma 1 a termino a manera de ciclo
 if(Termino==graditos)// Si se han completado los pasos del motor
 {
 negativo=0x00; //Se reinicia el valor negativo a positivo por default
 muestras=0x00; // La muestra de los grados en el programa se reinicia
 }
 if (Termino < graditos) // 32 * 64 = 2048
 {
 if(negativo==0)// Switch case para el caso de giro horario
 {
 Count = Count + 0x01;// Se incrementa la cuenta
 switch (Count&0x0F) {
 case 0x04:
 GPIO_PORTL_DATA_R=0x09; // A,B
GPIO_PORTN_DATA_R = 0x03; // A,B
GPIO_PORTF_AHB_DATA_R = 0x00; //
Count=0x00;// Se reinicia la cuenta
 break;
 case 0x03:
 GPIO_PORTL_DATA_R=0x0C; // A',B
GPIO_PORTN_DATA_R = 0x01; // B
GPIO_PORTF_AHB_DATA_R = 0x10; // A'
break;
 case 0x02:
 GPIO_PORTL_DATA_R=0x06; // A', B'
GPIO_PORTN_DATA_R = 0x00; //
GPIO_PORTF_AHB_DATA_R = 0x11; //A', B'
break;
 case 0x01:
 GPIO_PORTL_DATA_R=0x03; // A, B'
GPIO_PORTN_DATA_R = 0x02; // A
GPIO_PORTF_AHB_DATA_R = 0x01; // B'
 break;
 }
 }
 else// Caso de giro antihorario
 {
 Count = Count + 0x01;//Se incrementa la cuenta en 1
 switch (Count&0x0F) {
 case 0x01:
 GPIO_PORTL_DATA_R=0x09; // A,B
 GPIO_PORTN_DATA_R = 0x03; // A,B
GPIO_PORTF_AHB_DATA_R = 0x00; //
break;
 case 0x02:
 GPIO_PORTL_DATA_R=0x0C; // A',B
 GPIO_PORTN_DATA_R = 0x01; // B
GPIO_PORTF_AHB_DATA_R = 0x10; // A'
break;
 case 0x03:
 GPIO_PORTL_DATA_R=0x06; // A', B'
GPIO_PORTN_DATA_R = 0x00; //
GPIO_PORTF_AHB_DATA_R = 0x11; //A', B'
break;
 case 0x04:
 GPIO_PORTL_DATA_R=0x03; // A, B'
GPIO_PORTN_DATA_R = 0x02; // A
GPIO_PORTF_AHB_DATA_R = 0x01; // B'
Count=0x00;
 break;
 }
 }
 }
}
//--------------------RELOJ INTERNO-----------------------------
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE608)) // Registro de
Habilitación de Reloj de Puertos
#define SYSCTL_PRGPIO_R (*((volatile unsigned long *)0x400FEA08)) // Registro de 
estatus de Reloj de Puerto
// Definición de constantes para operaciones
#define SYSCTL_RCGC2_GPION 0x00001E30 // bit de estado del reloj
main(void)
{
 int i,grados,velocidadr; // Datos de los grados en valor entero y el contador
 int velocidad=0x00; // El valor entero de la velocidad
 int contdig=0x00; // La cuenta que verifica que digito se registra
 int ingresos=0x00; // El valor de ingreso para la velocidad con tecla #
 int digitos[]={0x00,0x00,0x00,0x00}; // Los digitos reales que se registran en 7seg
 int digreal[]={0,0,0,0}; // Arreglo donde se almacenan los dig. reales
 int digvelocidad[4]; // Arreglo de digitos de velocidad
 SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPION; // Activa el reloj
 while ((SYSCTL_PRGPIO_R & 0X1000) == 0){}; // reloj listo?
 GPIO_PORTK_DIR_R |= 0xF0; // Salidas PK4-PK7 y Entradas PK0-PK3
 GPIO_PORTM_DIR_R |= 0xFF; // Salidas de PM0-PM7
 GPIO_PORTE_DIR_R |= 0x0F; // Salidas de PE0-PE3
 GPIO_PORTK_DEN_R |= 0xFF; // Habilita PK0-PK7
 GPIO_PORTM_DEN_R |= 0xFF; // Habilita PM0-PM7
 GPIO_PORTE_DEN_R |= 0x0F; // Habilita PE0-PE3
 GPIO_PORTK_PDR_R |= 0x0F; // Habilita resistencias de pull-down para salidas del teclado
matricial
 GPIO_PORTM_PDR_R |= 0xFF; // Habilita resistencias de pull-down para salidas del display 
7 segmentos
 GPIO_PORTE_PUR_R |= 0x0F; // Habilita resistencias de pull-up para digitos de
habilitación 7 segmentos
 while(1)
 {
 /*Fila 1*/
 GPIO_PORTK_DATA_R=0x80; // Se multiplexa la primera fila del teclado
 if(GPIO_PORTK_DATA_R==0x88) // Presiona tecla 1
 {
 if(contdig==4) // Si ya se ingresaron cuatro digitos del display se
reincia
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
 for(i=0;i<=0x10000;i++){}// Se espera un rato
 /* Obtención de grados en valor entero*/
 if(negativo==1)//Si los grados que se registrarán son negativos
 {
 if(contdig==4)// Si ya se llenó el display se registraron 4 dígitos
 {
 grados=digreal[1]*100+digreal[2]*10+digreal[3];//Conversión a decimal
 graditos=trunc(grados/11.2*64);//Se obtienen esos grados en pasos con
truncamiento
 digmuestra[0]=digitos[0]; // Se transfieren los datos del 7seg
 digmuestra[1]=digitos[1]; // a otro arreglo para mostrarlo en lo que gira
el motor
 digmuestra[2]=digitos[2]; // El dígito 3
 digmuestra[3]=digitos[3]; // Y el dígito 4
 for(i=0;i<=3;i++) // Se reincian los dígitos decimales y 7 seg
 {
 digitos[i]=0x00; // Se reinicia 7 seg
 digreal[i]=0x00; // Se reinicia digitos en decimal
 }
contdig=0x00; // El conteo se reinicia
 }
 }
 else if(contdig==3) // Si el valor es positivo la cuenta llega a 3
 {
 grados=digreal[0]*100+digreal[1]*10+digreal[2]; // Se convierte a decimal
 graditos=trunc(grados/11.2*64); // Se convierten los grados en pasos
 digmuestra[0]=digitos[0]; // Se guardan los dígitos en 7seg para ser mostrados
 digmuestra[1]=digitos[1]; // digito 1
 digmuestra[2]=digitos[2]; // digito 2
 digmuestra[3]=digitos[3]; // digito 3
 for(i=0;i<=3;i++) // Ciclo de reinicio de arreglos
 {
 digitos[i]=0x00; // Se reinician los dígitos en 7seg
 digreal[i]=0x00; // Se reinician los dígitos en decimal
 }
 contdig=0x00;// Se reinicia el contador de dígitos
 }
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
 else if(GPIO_PORTK_DATA_R==0x41)// Se configuró tecla B como # por fallas en mi
teclado
 {
 if(ingresos==0) // Si se teclea una vez se prepara para recibir los datos
 {
 ingresos+=1; // Se incrementa ingresos para saber que ya se tecleo una vez
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 }
 else if(ingresos==1)// Si ya se tecleo una vez se realizará el giro
 {
 muestras=0x01;// La muestra se activa para mostrar el display mientras gira
 ingresos+=1;// Los ingresos se vuelven a incrementar
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 velocidad=digreal[0]*100+digreal[1]*10+digreal[2];// Se convierte la velocidad
a digito real
 velocidadr=trunc(velocidad*16000000/2057); // Conversión de velocidad a RPM
 //habilita PORTN, PORTF, PORTL
 SYSCTL_RCGCGPIO_R |= 0X1E30; // RELOJ PARA EL PUERTO F, L y N
 SYSCTL_RCGCTIMER_R |= 0X08; //RELOJ Y HABILITA TIMER 3 (p.380)
 //retardo para que el reloj alcance el PORTN Y TIMER 3
 while ((SYSCTL_PRGPIO_R & 0X1000) == 0){}; // reloj listo?
 TIMER3_CTL_R=0X00000000; //DESHABILITA TIMER 3 PARA CONFIGURAR (p.986)
 TIMER3_CFG_R= 0X00000000; //CONFIGURA TIMER DE 32 BITS (p. 976)
 //TIMER3_TAMR_R= 0X00000002; //CONFIGURAR PARA MODO PERIODICO CUENTA HACIA 
ABAJO (p. 977)
 TIMER3_TAMR_R= 0X00000012; //CONFIGURAR PARA MODO PERIODICO CUENTA HACIA 
ARRIBA (p. 977)
 TIMER3_TAILR_R= velocidadr; // VALOR DE RECARGA (p.1004)
 // TIMER3_TAILR_R= 0X0004E200; // VALOR DE RECARGA (p.1004)
 TIMER3_TAPR_R= 0X00; // PRESCALADOR DE TIMER A, SOLO PARA MODOS DE 16 BITS 
(p.1008)
 TIMER3_ICR_R= 0X00000001 ; //LIMPIA POSIBLE BANDERA PENDIENTE DE TIMER3 
(p.1002)
 TIMER3_IMR_R |= 0X00000001; //ACTIVA INTRRUPCION DE TIMEOUT (p.993)
 NVIC_EN1_R= 1<<(35-32); //HABILITA LA INTERRUPCION 35 (TIMER3 A)
 TIMER3_CTL_R |= 0X00000001; //HABILITA TIMER 3 (p.986)
 // habilita al Puerto L como salida digital para control de motor
 // PL0,...,PL3 como salidas hacia el ULN2003 (A,A´,B,B´)
 GPIO_PORTL_DIR_R = 0x0F; // Habilita PL0-PL3
 GPIO_PORTL_DEN_R = 0x0F; // Habilita PL0-PL3 como salida
 GPIO_PORTL_DATA_R = 0x09; // Valor de dato
 // habilita PN0 y PN1 como salida digital para monitoreo del programa
 //
 GPIO_PORTN_DIR_R = 0x03; // Habilita PN0-PN1
 GPIO_PORTN_DEN_R = 0x03; // PN0-PN1 como salida
 // habilita PF0 y PF4 como salida digital para monitoreo del programa
 //
 GPIO_PORTF_AHB_DIR_R = 0x11; // PF0 y PF4
 GPIO_PORTF_AHB_DEN_R = 0x11; // PF0 y PF4 como salida
 digitos[0]=0x00; // Se reinician todos los dígitos en 7seg
 digitos[1]=0x00; //dig1
 digitos[2]=0x00; //dig2
 digitos[3]=0x00; //dig3
 contdig=0x00; // El conteo se reinicia
 ingresos=0x00;// Los ingresos se reinician
 Termino=0x00; // El termino del motor se reinicia
 }
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
 if(contdig==0){// Solo se acepta el signo si se ingresa al inicio
 digitos[contdig]=0x01;// Se iguala al signo - en 7seg
 contdig+=1; // Se incrementa el valor en 1
 negativo=0x01;// El valor negativo se hace 1
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 }
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
 if(muestras==1)
 {
 if(negativo==1)// Si el valor negativo es real
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=digmuestra[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=digmuestra[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=digmuestra[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x01;// Dígito 1 es el signo negativo
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 }
 else
 {
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=digmuestra[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=digmuestra[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=digmuestra[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=digmuestra[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 }
 }
 }
}
