/*Tarea Examen 4: Motor a pasos y reloj en tiempo real
* Código creado por: Teresa Fiel Muñoz
* Microprocesadores y Microcontroladores 2021-1
*/
//------------------------------------LIBRERIAS DEL PROGRAMA-----------------------------------
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "inc/tm4c1294ncpdt.h"
volatile uint32_t Count= 0;
volatile uint32_t Termino= 0;
//------------------------------------------DEFINICIÓN DE VARIABLES----------------------------------------
//------------------------------------------ADC------------------------------------------------------------
#define GPIO_PORTE_AHB_DIR_R (*((volatile uint32_t *)0x4005C400))
#define GPIO_PORTE_AHB_AFSEL_R (*((volatile uint32_t *)0x4005C420)) //Registro para habilitar funciones alternativas
del de GPIO (p.770)
#define GPIO_PORTE_AHB_DEN_R (*((volatile uint32_t *)0x4005C51C)) //Registro para habilitar funciones digital del
de GPIO (p.781)
#define GPIO_PORTE_AHB_AMSEL_R (*((volatile uint32_t *)0x4005C528))
#define SYSCTL_RCGCADC_R (*((volatile uint32_t *)0x400FE638)) // Registro para habilitar el reloj al ADC(p. 
396)
#define SYSCTL_PRADC_R (*((volatile uint32_t *)0x400FEA38)) // Registro para verificar si el ADC esta listo
(p.515)
#define ADC0_PC_R (*((volatile uint32_t *)0x40038FC4)) // Registro para configurar tasa de muestreo
(p.1159)
#define ADC0_SSPRI_R (*((volatile uint32_t *)0x40038020)) // Registro para configurar la prioridad del
secuenciador (p.1099)
#define ADC0_ACTSS_R (*((volatile uint32_t *)0x40038000)) // Registro para controlar la activación del
secuenciador (p. 1076)
#define ADC0_EMUX_R (*((volatile uint32_t *)0x40038014)) // Registro para seleccionar el evento (trigger) 
que inicia el muestreo en cada secuenciador (p.1091)
#define ADC0_SSEMUX2_R (*((volatile uint32_t *)0x40038098)) // Registro que selecciona entre las entradas
AIN[19:16] o AIN[15:0] (p.1137)
#define ADC0_SSMUX2_R (*((volatile uint32_t *)0x40038080)) // Registro para configurar la entrada analógica
para el Secuenciador 2 (p.1128)
#define ADC0_SSCTL2_R (*((volatile uint32_t *)0x40038084)) // Registro que configura la muestra ejecutada
con el Secuenciador 2 (p.1142)
#define ADC0_IM_R (*((volatile uint32_t *)0x40038008)) // Registro que controla la mascara de
interrupciones en secuenciadores (p. 1081)
#define ADC0_ACTSS_R (*((volatile uint32_t *)0x40038000)) // Registro que controla la activación de los
secuenciadores (p.1077)
#define ADC0_ISC_R (*((volatile uint32_t *)0x4003800C)) //Registro de estatus y para borrar las
condiciones de interrupción del secuenciador (p. 1084)
#define ADC0_PSSI_R (*((volatile uint32_t *)0x40038028)) //Registro que permite al software iniciar el 
muestreo en los secuenciadores (p. 1102)
#define ADC0_RIS_R (*((volatile uint32_t *)0x40038004)) //Registro muestra el estado de la señal de
interrupción de cada secuenciador (p.1079)
#define ADC0_SSFIFO2_R (*((volatile uint32_t *)0x40038088)) //Registro que contiene los resultados de
conversión de las muestras recogidas con el secuenciador (p. 1118)
#define ADC0_SAC_R (*((volatile uint32_t *)0x40038030)) //Registro de control de muestras a promediar (p. 
1105)
#define ADC0_CTL_R (*((volatile uint32_t *)0x40038038))
#define ADC0_SSOP2_R (*((volatile uint32_t *)0x40038090))
#define ADC0_SSTSH2_R (*((volatile uint32_t *)0x4003809C))
#define SYSCTL_PLLFREQ0_R (*((volatile uint32_t *)0x400FE160)) //Registro para configurar el PLL
#define SYSCTL_PLLSTAT_R (*((volatile uint32_t *)0x400FE168)) //Registro muestra el estado de encendido del PLL
#define SYSCTL_PLLFREQ0_PLLPWR 0x00800000 // Valor para encender el PLL
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
#define GPIO_PORTH_DATA_R (*((volatile unsigned long *)0x4005F3FC)) // Registro de Datos Puerto H
#define GPIO_PORTH_DIR_R (*((volatile unsigned long *)0x4005F400)) // Registro de Dirección Puerto H
#define GPIO_PORTH_DEN_R (*((volatile unsigned long *)0x4005F51C)) // Registro de Habilitación Puerto H
#define GPIO_PORTH_PUR_R (*((volatile unsigned long *)0x4005F510)) // Registro de Pull-Up Puerto H
//------------------------------ENTRADAS MOTOR A PASOS----------------------------
#define GPIO_PORTP_DATA_R (*((volatile unsigned long *)0x400653FC)) // Registro de Datos Puerto P
#define GPIO_PORTP_DIR_R (*((volatile unsigned long *)0x40065400)) // Registro de Dirección Puerto P
#define GPIO_PORTP_DEN_R (*((volatile unsigned long *)0x4006551C)) // Registro de Habilitación Puerto P
#define GPIO_PORTP_PDR_R (*((volatile unsigned long *)0x40065514))// Registro de Pull-Down Puerto P
//---------------------------------------------BUZZER PASIVO--------------------------------------------
#define GPIO_PORTQ_DATA_R (*((volatile unsigned long *)0x400663FC)) // Registro de Datos Puerto Q
#define GPIO_PORTQ_DIR_R (*((volatile unsigned long *)0x40066400)) // Registro de Dirección Puerto Q
#define GPIO_PORTQ_DEN_R (*((volatile unsigned long *)0x4006651C)) // Registro de Habilitación Puerto Q
#define GPIO_PORTQ_PDR_R (*((volatile unsigned long *)0x40066514))// Registro de Pull-Down Puerto Q
//-------------------------------------------------------------SENSOR DE PRESENCIA------------------------------------
---------------------------------
#define GPIO_PORTA_DIR_R (*((volatile uint32_t *)0x40058400)) //Registro de Dirección PA
#define GPIO_PORTA_DEN_R (*((volatile uint32_t *)0x4005851C)) //Registro de habilitación PA
#define GPIO_PORTA_PUR_R (*((volatile uint32_t *)0x40058510)) //Registro de pull-up PA
#define GPIO_PORTA_DATA_R (*((volatile uint32_t *)0x40058004)) //Registro de Datos A
#define GPIO_PORTA_IS_R (*((volatile uint32_t *)0x40058404)) //Registro de configuración de detección de nivel
o flanco
#define GPIO_PORTA_IBE_R (*((volatile uint32_t *)0x40058408)) //Registro de configuración de interrupción por
ambos flancos
#define GPIO_PORTA_IEV_R (*((volatile uint32_t *)0x4005840C)) //Registro de configuración de interrupción por
un flanco
#define GPIO_PORTA_ICR_R (*((volatile uint32_t *)0x4005841C)) //Registro de limpieza de interrupcion de flanco
en PA
#define GPIO_PORTA_IM_R (*((volatile uint32_t *)0x40058410)) //Registro de mascara de interrupcion PA (p.764)
#define NVIC_EN0_R (*((volatile uint32_t *)0xE000E100)) // Registro de habilitación de interrupción PA
#define NVIC_PRI0_R (*((volatile uint32_t *)0xE000E400))//Registro de prioridad de interrupción
//-------------------HAB. DISPLAY 7 SEGMENTOS RELOJ-------------------
#define GPIO_PORTE_DATA_R (*((volatile unsigned long *)0x4005C3FC)) // Registro de Datos Puerto E
#define GPIO_PORTE_DIR_R (*((volatile unsigned long *)0x4005C400)) // Registro de Dirección Puerto E
#define GPIO_PORTE_DEN_R (*((volatile unsigned long *)0x4005C51C)) // Registro de Habilitación Puerto E
#define GPIO_PORTE_PUR_R (*((volatile unsigned long *)0x4005C510)) // Registro de Pull-Up Puerto E
#define GPIO_PORTD_DATA_R (*((volatile unsigned long *)0x4005B3FC)) // Registro de Datos Puerto D
#define GPIO_PORTD_DIR_R (*((volatile unsigned long *)0x4005B400)) // Registro de Dirección Puerto D
#define GPIO_PORTD_DEN_R (*((volatile unsigned long *)0x4005B51C)) // Registro de Habilitación Puerto D
#define GPIO_PORTD_PUR_R (*((volatile unsigned long *)0x4005B510)) // Registro de Pull-Up Puerto D
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *)0x4005D3FC)) // Registro de Datos Puerto F
#define GPIO_PORTF_DIR_R (*((volatile unsigned long *)0x4005D400)) // Registro de Dirección Puerto F
#define GPIO_PORTF_DEN_R (*((volatile unsigned long *)0x4005D51C)) // Registro de Habilitación Puerto F
#define GPIO_PORTF_PUR_R (*((volatile unsigned long *)0x4005D510)) // Registro de Pull-Up Puerto F
#define GPIO_PORTG_DATA_R (*((volatile unsigned long *)0x4005E3FC)) // Registro de Datos Puerto G
#define GPIO_PORTG_DIR_R (*((volatile unsigned long *)0x4005E400)) // Registro de Dirección Puerto G
#define GPIO_PORTG_DEN_R (*((volatile unsigned long *)0x4005E51C)) // Registro de Habilitación Puerto G
#define GPIO_PORTG_PUR_R (*((volatile unsigned long *)0x4005E510)) // Registro de Pull-Up Puerto G
#define GPIO_PORTN_DATA_R (*((volatile unsigned long *)0x400643FC)) // Registro de Datos Puerto G
#define GPIO_PORTN_DIR_R (*((volatile unsigned long *)0x40064400)) // Registro de Dirección Puerto G
#define GPIO_PORTN_DEN_R (*((volatile unsigned long *)0x4006451C)) // Registro de Habilitación Puerto G
#define GPIO_PORTN_PUR_R (*((volatile unsigned long *)0x40064510)) // Registro de Pull-Up Puerto G
//REGISTROS DE RELOJ
#define SYSCTL_RCGCGPIO_R (*((volatile uint32_t *)0x400FE608)) //Reloj del puerto
#define SYSCTL_RCGCI2C_R (*((volatile uint32_t *)0x400FE620)) //Reloj de I2C
#define SYSCTL_PRGPIO_R (*((volatile uint32_t *)0x400FEA08)) //Bandera de "Peripherial Ready"
//REGISTROS DEL PUERTO B
#define GPIO_PORTB_DATA_R (*((volatile uint32_t *)0x400593FC)) //Para los datos del puerto
#define GPIO_PORTB_DIR_R (*((volatile uint32_t *)0x40059400)) //Para seleccionar función
#define GPIO_PORTB_AFSEL_R (*((volatile uint32_t *)0x40059420)) //Para seleccionar función alterna
#define GPIO_PORTB_ODR_R (*((volatile uint32_t *)0x4005950C)) //Para activar el Open Drain
#define GPIO_PORTB_DEN_R (*((volatile uint32_t *)0x4005951C)) //Para activar función digital
#define GPIO_PORTB_PCTL_R (*((volatile uint32_t *)0x4005952C)) //Para el control del puerto
//REGISTROS DEL MÓUDLO I2C
#define I2C0_MSA_R (*((volatile uint32_t *)0x40020000)) //I2C Master Slave Adress
#define I2C0_MCS_R (*((volatile uint32_t *)0x40020004)) //I2C Master Control Status
#define I2C0_MDR_R (*((volatile uint32_t *)0x40020008)) //I2C Master Data Register
#define I2C0_MTPR_R (*((volatile uint32_t *)0x4002000C)) //I2C Master Time Period
#define I2C0_MCR_R (*((volatile uint32_t *)0x40020020)) //I2C Master Congirutation Register
/*
El registro I2C Master Control/Status (I2C_MCS_R) tiene:
-Modo READ-ONLY DATUS: los 7 bits menos significativos son:
 7:Clock Time Out Error 6:BUS BUSY 5:IDLE
 4:Arbitration Lost 3:DataAck 2:AdrAck
 1:Error 0:CONTROLLER BUSY
-Modo WRITE-ONLY CONTROL_ Los 6 bits menos significativos son:
 6:BURST 5:QuickCommand 4:High Speed Enable
 3:ACK 2:STOP 1:START
 0:RUN
*/
#define I2C_MCS_ACK 0x00000008 //Transmmitter Acknowledge Enable
#define I2C_MCS_DATACK 0x00000008 // Data Acknowledge Enable
#define I2C_MCS_ADRACK 0x00000004 // Acknowledge Address
#define I2C_MCS_STOP 0x00000004 // Generate STOP
#define I2C_MCS_START 0x00000002 // Generate START
#define I2C_MCS_ERROR 0x00000002 // Error
#define I2C_MCS_RUN 0x00000001 // I2C Master Enable
#define MAXRETRIES 5 // number of receive attempts before giving up
//**Direcciones del DS1307
int AdreDS1307 =0x068;///Dirección del RTC DS1307
int AdreSec= 0x00;
int AdreMin=0x01;
int decenasd[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int unidadesd[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int segmentosd[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int segmentosu[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int dig1=0x00;
int dig2=0x00;
int digdec=0x00;
int dec1=0x00;
int dec2=0x00;
int dec3=0x00;
/*El cálculo del Time Period Register (TPR) se especifica en la página 1284
Asumiendo un reloj de 16 MHz y un modo de operación estándar (100 kbps):
*/
int TPR = 7;
// Variables para manejar los valores del RTC
uint8_t segundos, minutos, horas, dia, fecha, mes, anio;
uint8_t error;
uint32_t i;
//*** Función que inicializa los relojes, el GPIO y el I2C0 ***
void I2C_Init(void){
 //CONFIGURACIÓN DE LOS RELOJ
 SYSCTL_RCGCI2C_R |= 0x0001; // Activamos el reloj de I2C0 [I2C9 I2C8 I2C7 ... I2C0]<--Mapa de RCGCI2C
 SYSCTL_RCGCGPIO_R |= 0X7EFF; // Activamos el reloj GPIO_PORTB mientras se activa el reloj de I2C0
 while((SYSCTL_PRGPIO_R&0X7EB2) == 0){};//Espero a que se active el reloj del puerto B
 //CONFIGURACIÓN DE LOS GPIOS
 /*Acorde con la tabla "Signals by function" de la p. 1808:
 el PIN 2 del puerto B (PB2) es el I2C0SCL del I2C0, y
 el PIN 3 del puerto B (PB3) es el I2C0SDA del I2C0
 */
 GPIO_PORTB_AFSEL_R |= 0x0C; // Activo la función alterna del PB2 y PB3
 GPIO_PORTB_ODR_R |= 0x08; // Activo el OPEN DRAIN para el PB3, ya que el PB2 ya tiene uno por preconfig.
 GPIO_PORTB_DIR_R |= 0x0C; //Activo al PB2 y al PB3 como OUTPUT
 GPIO_PORTB_DEN_R |= 0x0C; //Activo la función digital de PB3 y PB2
 GPIO_PORTE_DIR_R |= 0x0F; // Salidas de PE0-PE3
 GPIO_PORTD_DIR_R |= 0x0F; // Salidas de PD0-PD3
 GPIO_PORTF_DIR_R |= 0x0E; // Salidas de PF1-PF3
 GPIO_PORTG_DIR_R |= 0x03; // Salidas de PG0-PG1
 GPIO_PORTN_DIR_R |= 0x10; // Salidas de PN4
 GPIO_PORTE_DEN_R |= 0x0F; // Habilita PE0-PE3
 GPIO_PORTD_DEN_R |= 0x0F; // Habilita PD0-PD3
 GPIO_PORTF_DEN_R |= 0x0E; // Habilita PF1-PF3
 GPIO_PORTG_DEN_R |= 0x03; // Habilita PG0-PG1
 GPIO_PORTN_DEN_R |= 0x10; // Habilita PN4
 GPIO_PORTE_PUR_R |= 0x0F; // Habilita resistencias de pull-up para digitos de habilitación 7 segmentos
 GPIO_PORTD_PUR_R |= 0x0F; // Habilita resistencias de pull-up para digitos de habilitación 7 segmentos
 GPIO_PORTF_PUR_R |= 0x0E; // Habilita resistencias de pull-up para digitos de habilitación 7 segmentos
 GPIO_PORTG_PUR_R |= 0x03; // Habilita resistencias de pull-up para digitos de habilitación 7 segmentos
 GPIO_PORTN_PUR_R |= 0x10; // Habilita resistencias de pull-up para digitos de habilitación 7 segmentos
 /*
 Así como el registro AFSEL indica que se ejecutará una función externa, en el registro PCTL
 debemos indicar qué función alterna se realizará acorde con la tabla 26-5 de la p.1808 e indicarlo
 en el correspondiente PCMn (uno por cada bit del puerto) del registro PCTL
 */
 GPIO_PORTB_PCTL_R|=0x00002200;
 //CONFIGURACIÓN DEL MODULO I2C0
 I2C0_MCR_R = 0x00000010; // Habilitar función MASTER para el I2C0
 I2C0_MTPR_R = TPR; // Se establece una velocidad estándar de 100kbps
}
// ** Función esperar **
int esperar(){
 while(I2C0_MCS_R&0x00000001){}; //Espero a que la transmisión acabe
 if(I2C0_MCS_R&0x00000002==1){ //¿Hubo error?
 error=1;
 return error;
 };
 return 0;
}
//** Función para configurar al esclavo RTC DS1307 **
void CargarFecha(){
 /*
 Programar: Jueves 31 de octubre del 2019, a las 9:40:00 pm
 El mapa de memoria del DS1207 es el siguiente:
 DIRECCIÓN FUNCIÓN BIT7 BIT6 BIT5 BIT4 BIT3 BIT2 BIT1 BIT0
 00h Segundos 0 0 0 0 0 0 0 0
 01h Minutos 0 0 0 0 0 0 0 0
 02h Horas 0 0 0 1 0 0 0 0
 03h Día 0 0 1 1 0 0 0 1
 04h Fecha 0 0 0 0 0 1 0 0
 05h Mes 0 0 0 1 0 0 0 0
 06h Año 0 0 0 1 1 0 0 1
 07h Control 0 0 0 0 0 0 1 1
 08h-3Fh RAM 56x8
 */
 //Por lo tanto
 int segundos=0x00, minutos=0x40, horas=0x09, dia=0x05, fecha=0x05, mes=0x02, anio=0x21;
 while(I2C0_MCS_R&0x00000001){}; // wait for I2C ready
 //Para transmitir
 I2C0_MSA_R=(AdreDS1307<<1)&0xFE; //Cargo la dirección del DS1307 e indico "SEND", es decir, el Slave va a recibir
 I2C0_MDR_R=AdreSec&0x0FF; //Envio la Subdirección( dirección del registro interno "segundos") al DS1307
 I2C0_MCS_R=(I2C_MCS_RUN|I2C_MCS_START); // Condición de START y corro
 esperar();
 for(i=0;i<300;i++){} //Delay
 I2C0_MDR_R=segundos; //Envio el valor de "segundos"
 I2C0_MCS_R=(I2C_MCS_RUN);
 esperar();
 for(i=0;i<300;i++){} //Delay
 I2C0_MDR_R=minutos; //Envio el valor de "minutos"
 I2C0_MCS_R=(I2C_MCS_RUN); //Inicio la transmisión 1
 esperar();
 for(i=0;i<300;i++){} //Delay
 I2C0_MDR_R=horas; //Envio el valor de "horas"
 I2C0_MCS_R=(I2C_MCS_RUN); //Inicio la transmisión 2
 esperar();
 for(i=0;i<300;i++){} //Delay
 I2C0_MDR_R=dia; //Envio el valor de "dia"
 I2C0_MCS_R=(I2C_MCS_RUN); //Inicio la transmisión 4
 esperar();
 for(i=0;i<300;i++){} //Delay
 I2C0_MDR_R=fecha; //Envio el valor de "fecha"
 I2C0_MCS_R=(I2C_MCS_RUN); //Inicio la transmisión 5
 esperar();
 for(i=0;i<300;i++){} //Delay
 I2C0_MDR_R=mes; //Envio el valor de "mes"
 I2C0_MCS_R=(I2C_MCS_RUN); //Inicio la transmisión 6
 esperar();
 for(i=0;i<300;i++){} //Delay
 I2C0_MDR_R= anio; //Envio el valor de "año"
 I2C0_MCS_R=(I2C_MCS_STOP|I2C_MCS_RUN); //Inicio la ultima transmisión y STOP
 esperar();
 for(i=0;i<300;i++){} //Delay
}
void leerFecha(){
 while(I2C0_MCS_R&0x00000001){}; // wait for I2C ready
 //Para actualizar registro para iniciar la lectura
 I2C0_MSA_R=(AdreDS1307<<1)&0xFE; //Cargo la dirección del DS1207 e indico "SEND", es decir, el Slave va a 
recibir
 I2C0_MDR_R=AdreSec&0x0FF; //Envio la Subdirección( dirección del registro interno "segundos") al DS1307
 I2C0_MCS_R=(I2C_MCS_START|I2C_MCS_RUN);// Condición de START, y corro
 esperar();
 for(i=0;i<300;i++){} //Delay
 //Para recibir información
 I2C0_MSA_R=(AdreDS1307<<1)&0xFE; //La dirección del DS1307 en el Master Slave Adress
 I2C0_MSA_R|=0x01; //Indico "RECIEVE", es decir, el Slave va a transmitir
 I2C0_MCS_R=(I2C_MCS_START|I2C_MCS_RUN|I2C_MCS_ACK); // Condición de RESTART, corro, ack
 esperar();
 for(i=0;i<300;i++){} //Delay
 segundos=(I2C0_MDR_R&0xFF); //El Master lee lo que envia el DS1307
 I2C0_MCS_R=(I2C_MCS_RUN|I2C_MCS_ACK); // corro, ack
 esperar();
 for(i=0;i<300;i++){} //Delay
 minutos=(I2C0_MDR_R&0xFF); //El Master lee lo que envía el DS1307
 I2C0_MCS_R=(I2C_MCS_RUN|I2C_MCS_ACK); //corro, ack
 esperar();
 for(i=0;i<300;i++){} //Delay
 horas=(I2C0_MDR_R&0xFF); //El Master lee lo que envía el DS1307
 I2C0_MCS_R=(I2C_MCS_RUN|I2C_MCS_ACK); // corro, ack
 esperar();
 for(i=0;i<300;i++){} //Delay
 dia=(I2C0_MDR_R&0xFF); //El Master lee lo que envía el DS1307
 I2C0_MCS_R=(I2C_MCS_RUN|I2C_MCS_ACK); // corro, ack
 esperar();
 for(i=0;i<300;i++){} //Delay
 fecha=(I2C0_MDR_R&0xFF); //El Master lee lo que envía el DS1307
 I2C0_MCS_R=(I2C_MCS_RUN|I2C_MCS_ACK); // corro, ack
 esperar();
 for(i=0;i<300;i++){} //Delay
 mes=(I2C0_MDR_R&0xFF); //El Master lee lo que envía el DS1307
 I2C0_MCS_R=(I2C_MCS_RUN|I2C_MCS_ACK); // corro, ack
 esperar();
 for(i=0;i<300;i++){} //Delay
 anio=(I2C0_MDR_R&0xFF); //El Master lee lo que envía el DS1307
 I2C0_MCS_R=(I2C_MCS_STOP|I2C_MCS_RUN); //corro, alto
}
//*** PROGRAMA PRINCIPAL ****
int frec,valor,r,config,TEMP,LUZ; // Datos enteros
int muestraclk=0;
int alarma[]={0,0,0,0};
int setupp=0;
int setupt=0;
int setupl=0;
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
int digmuestra[]={0,0,0,0}; // Digitos de muestra del teclado
int negativo=0x00; // Valor entero para saber si es o no 
negativo el giro
int vueltas,s=0,x=0;
int negativor[5];
int graditosr[5];
int muestra1[4];
int muestra2[4];
int muestra3[4];
int muestra4[4];
int muestra5[4];
int ejecutar=0;
int ingangulo=0;
int angulonegativo=0;
int puertoP=0;
// RUTINA DE SERVICIO DE INTERRUPCIÓN
void Timer03AIntHandler(void)
{
 //LIMPIA BANDERA
 TIMER3_ICR_R= 0X00000001 ; //LIMPIA BANDERA DE TIMER3
 Termino = Termino + 1; // Se suma 1 a termino a manera de ciclo
 if(vueltas<=1){}
 else if(Termino==graditosr[x] && s<=vueltas-2)
 {
 x+=1;
 s+=1;
 Termino=0;
 if(x==1)
 {
 digmuestra[0]=muestra2[0];
 digmuestra[1]=muestra2[1];
 digmuestra[2]=muestra2[2];
 digmuestra[3]=muestra2[3];
 }
 else if(x==2)
 {
 digmuestra[0]=muestra3[0];
 digmuestra[1]=muestra3[1];
 digmuestra[2]=muestra3[2];
 digmuestra[3]=muestra3[3];
 }
 else if(x==3)
 {
 digmuestra[0]=muestra4[0];
 digmuestra[1]=muestra4[1];
 digmuestra[2]=muestra4[2];
 digmuestra[3]=muestra4[3];
 }
 else if(x==4)
 {
 digmuestra[0]=muestra5[0];
 digmuestra[1]=muestra5[1];
 digmuestra[2]=muestra5[2];
 digmuestra[3]=muestra5[3];
 }
 }
 if(Termino==graditosr[x])// Si se han completado los pasos del motor
 {
 negativo=0x00; //Se reinicia el valor negativo a positivo por default
 muestras=0x00; // La muestra de los grados en el programa se reinicia
 graditosr[0]=0;
 graditosr[1]=0;
 graditosr[2]=0;
 graditosr[3]=0;
 graditosr[4]=0;
 }
 if (Termino < graditosr[x]) // 32 * 64 = 2048
 {
 if(negativor[x]==0)// Switch case para el caso de giro horario
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
#define SYSCTL_RCGC2_GPION 0x00007EFF // bit de estado del reloj
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE608)) // Registro de Habilitación de Reloj de
Puertos
volatile uint32_t Flancosdebajada = 0;
void EdgeCounter_Init(void){
 SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPION; //(a) activa el reloj para puerto L
 Flancosdebajada = 0; // (b) inicializa el contador
 GPIO_PORTA_DIR_R &= ~0x10; // (c) PA4 dirección entrada - boton SW1
 GPIO_PORTA_DEN_R |= 0x10; // PA4 se habilita
 GPIO_PORTA_PUR_R |= 0x10; // habilita weak pull-up on PL1
 GPIO_PORTA_IS_R &= ~0x10; // (d) PA4 es sensible por flanco (p.761)
 GPIO_PORTA_IBE_R &= ~0x10; // PA4 no es sensible a dos flancos (p. 762)
 GPIO_PORTA_IEV_R &= ~0x10; // PA4 detecta eventos de flanco de bajada (p.763)
 GPIO_PORTA_ICR_R = 0x10; // (e) limpia la bandera 0 (p.769)
 GPIO_PORTA_IM_R |= 0x10; // (f) Se desenmascara la interrupcion PA4 y se envia al controlador de
interrupciones (p.764)
 NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF)|0x00000000; // (g) prioridad 0 (p. 159)
 NVIC_EN0_R= 1<<(0-32); //(h) habilita la interrupción 0 en NVIC (p. 154)
}
void GPIOPortA_Handler(void)
{
 GPIO_PORTA_ICR_R = 0x01; // bandera0 de confirmación
 Flancosdebajada = Flancosdebajada + 1;
 /*if(setupp==1)
 {
 alarma[0]=0x67;
 alarma[1]=0x05;
 alarma[2]=0x4F;
 alarma[3]=0x5B;
 frecuencia(frequen[0]);
 //setupp=0; //Puede o no quitarse
 }*/
 for(i=0;i<=10000;i++){}
}
//--------------------RELOJ INTERNO-----------------------------
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE608)) // Registro de Habilitación de Reloj de
Puertos
#define SYSCTL_PRGPIO_R (*((volatile unsigned long *)0x400FEA08)) // Registro de estatus de Reloj de Puerto
// Definición de constantes para operaciones
#define SYSCTL_RCGC2_GPION 0x00007EFF // bit de estado del reloj puerto E,L,M,P,H
main(void)
{
 I2C_Init(); //Función que inicializa los relojes, el GPIO y el I2C0
 //Inicializo Slave
 while(I2C0_MCS_R&0x00000001){}; // espera que el I2C esté listo
 //Para transmitir
 CargarFecha(); // Función para configurar al esclavo RTC DS1307
 int i,grados,velocidadr; // Datos de los grados en valor entero y el contador
 int velocidad=0x00; // El valor entero de la velocidad
 int contdig=0x00; // La cuenta que verifica que digito se registra
 int ingresos=0x00; // El valor de ingreso para la velocidad con tecla #
 int digitos[]={0x00,0x00,0x00,0x00}; // Los digitos reales que se registran en 7seg
 int digreal[]={0,0,0,0}; // Arreglo donde se almacenan los dig. reales
 int digvelocidad[4]; // Arreglo de digitos de velocidad
 SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPION; // Activa el reloj
 while ((SYSCTL_PRGPIO_R & 0X1000) == 0){}; // reloj listo?
 GPIO_PORTQ_DIR_R |= 0x01; // Salida PQ0
 GPIO_PORTK_DIR_R |= 0xF0; // Salidas PK4-PK7 y Entradas PK0-PK3
 GPIO_PORTM_DIR_R |= 0xFF; // Salidas de PM0-PM7
 GPIO_PORTH_DIR_R |= 0x0F; // Salidas de PH0-PH3
 GPIO_PORTP_DIR_R |= 0x00; // Entradas PP0-PP3
 GPIO_PORTQ_DEN_R |= 0x01; // Habilita PKQ0
 GPIO_PORTK_DEN_R |= 0xFF; // Habilita PK0-PK7
 GPIO_PORTM_DEN_R |= 0xFF; // Habilita PM0-PM7
 GPIO_PORTH_DEN_R |= 0x0F; // Habilita PH0-PH3
 GPIO_PORTP_DEN_R |= 0x0F; // Habilita PP0-PP3
 GPIO_PORTQ_PDR_R |= 0x01; // Habilita resistencias de pull-down para BUZZER PASIVO
 GPIO_PORTK_PDR_R |= 0x0F; // Habilita resistencias de pull-down para salidas del teclado matricial
 GPIO_PORTP_PDR_R |= 0x0F; // Habilita resistencias de pull-down para botones del motor
 GPIO_PORTM_PDR_R |= 0xFF; // Habilita resistencias de pull-down para salidas del display 7 segmentos
 GPIO_PORTH_PUR_R |= 0x0F; // Habilita resistencias de pull-up para digitos de habilitación 7 segmentos
 r=0; //Incio de configuración
 config=0; //Inicio de configuración
 GPIO_PORTE_AHB_DIR_R = 0x0F; // 2) PE5-4 entradas (analógica)
 GPIO_PORTE_AHB_AFSEL_R |= 0x30; // 3) Habilita Función Alterna en PE5-4 (p. 770)
 GPIO_PORTE_AHB_DEN_R &= ~0x30; // 4) Deshabilita Función Digital en PE5-4 (p 781)
 GPIO_PORTE_AHB_AMSEL_R |= 0x30; // 5) Habilita Función Analógica de PE5-4 (p. 786)
 SYSCTL_RCGCADC_R |= 0x01; // 6) Habilita reloj para ADC0(p. 396)
 while((SYSCTL_PRADC_R & 0x01) == 0); // Se espera a que el reloj se estabilice
 ADC0_PC_R = 0x01; // 7)Configura para 125Ksamp/s (p.1159)
 ADC0_SSPRI_R = 0x0123; // 8)SS3 con la más alta prioridad
 ADC0_ACTSS_R &= ~0x0004; // 9) Deshabilita SS2 antes de cambiar configuración de registros (p. 1076)
 ADC0_EMUX_R = 0x0000; // 10) Se configura SS2 para disparar muestreo por software (default) (p.1091)
 ADC0_SAC_R = 0x0; // 11) Se configura para no tener sobremuestreo por hardware(default)(p. 1105)
 ADC0_CTL_R = 0x0; //12) Se configura con referencias internas (default VDDA and GNDA) (p. 1107)
 ADC0_SSOP2_R = 0x0000; // 13) Se configure para salvar los resultados del ADC en FIFO (default)(p. 1134)
 ADC0_SSTSH2_R = 0x000; // 14) Se configure el ADC para un periodo de 4 S&H (default) (p. 1134)
 ADC0_SSMUX2_R = 0x0089; // 15) Se configura entradas 1°muestra=AIN9 2°muestra=AIN8 (p.1128)
 ADC0_SSEMUX2_R &= ~0x0011; // 16) Canales del SS2 para 1°muestra y 2°muestra en AIN(15:0) (p.1137)
 ADC0_SSCTL2_R = 0x0060; // 17) SI: AIN, Habilitación de INR2; No:muestra diferencial (p.1142)
 ADC0_IM_R &= ~0x0010; // 18) Deshabilita interrupción SS2 (p. 1081)
 ADC0_ACTSS_R |= 0x0004; // 19) Habilita SS2 (p. 1076)
 SYSCTL_PLLFREQ0_R |= SYSCTL_PLLFREQ0_PLLPWR; // encender PLL
 while((SYSCTL_PLLSTAT_R&0x01)==0); // espera a que el PLL fije su frecuencia
 SYSCTL_PLLFREQ0_R &= ~SYSCTL_PLLFREQ0_PLLPWR; // apagar PLL
 ADC0_ISC_R = 0x0004; // Se recomienda Limpia la bandera RIS del ADC0
 // data[0] es el segundo resultado (ADC8 (PE5)) 0 to 10 Sensor de LUZ
 // data[1] es el primer resultado (ADC9 (PE4)) 0 to 10 Sensor de TEMPERATURA
 GPIO_PORTD_DATA_R=0xFF;//Apaga todos los puertos de segmentos para que no se traslape su muestra
 GPIO_PORTF_DATA_R=0xFF;//Apaga todos los puertos de segmentos para que no se traslape su muestra
 GPIO_PORTG_DATA_R=0xFF;//Apaga todos los puertos de segmentos para que no se traslape su muestra
 GPIO_PORTN_DATA_R=0xFF;//Apaga todos los puertos de segmentos para que no se traslape su muestra
 GPIO_PORTE_DATA_R=0xFF;//Apaga todos los puertos de segmentos para que no se traslape su muestra
 while(1)
 {
 leerFecha();
 valor=digreal[3]+digreal[2]*10+digreal[1]*100+digreal[0]*1000; //Calcula dígito total con dígitos ingresados
 puertoP=GPIO_PORTP_DATA_R;
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
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x5B;//Enciende dígito 4 S
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x4F;//Enciende dígito 3 E
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x05;//Enciende dígito 2 R
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x67;//Enciende dígito 1 P
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=fmuestrap[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=fmuestrap[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=fmuestrap[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=fmuestrap[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 4 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 3 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x7E;//Enciende dígito 2 O
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 /*Temperatura*/
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x67;//Enciende dígito 4 P
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x37;//Enciende dígito 3 M
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x4F;//Enciende dígito 2 E
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x0F;//Enciende dígito 1 T
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=fmuestrat[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=fmuestrat[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=fmuestrat[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=fmuestrat[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=vmuestrat[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=vmuestrat[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=vmuestrat[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=vmuestrat[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 4 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 3 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x7E;//Enciende dígito 2 O
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 /*Luz*/
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x6D;//Enciende dígito 4 S
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x0E;//Enciende dígito 3 U
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x3E;//Enciende dígito 2 L
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1 0
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=fmuestral[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=fmuestral[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=fmuestral[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=fmuestral[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=vmuestral[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=vmuestral[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=vmuestral[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=vmuestral[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 4 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 3 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x7E;//Enciende dígito 2 O
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
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
 else if(GPIO_PORTK_DATA_R==0x41)// Se configuró tecla B como # por fallas en mi teclado
 {
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x5B;//Enciende dígito 4 S
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x4F;//Enciende dígito 3 E
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x05;//Enciende dígito 2 R
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x67;//Enciende dígito 1 P
 GPIO_PORTM_DATA_R=0x00;
 }
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
 else if(GPIO_PORTK_DATA_R==0x21)//Tecla C presionada
 {
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x67;//Enciende dígito 4 P
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x37;//Enciende dígito 3 M
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x4F;//Enciende dígito 2 E
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x0F;//Enciende dígito 1 T
 GPIO_PORTM_DATA_R=0x00;
 }
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
 for(i=0;i<=1000000;i++)
{
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 4 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 3 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 2 O
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
frequen[0]=valor; //Frecuencia igual al valor ingresado
 fmuestrap[0]=digitos[0]; //Guarda valor en frecuencia de sensor
 fmuestrap[1]=digitos[1]; //Guarda valor en frecuencia de sensor
 fmuestrap[2]=digitos[2];//Guarda valor en frecuencia de sensor
 fmuestrap[3]=digitos[3];//Guarda valor en frecuencia de sensor
 setupp=1;
 }
 else if(r==2)
 {
 for(i=0;i<=1000000;i++)
{
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 4 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 3 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 2 O
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
frequen[1]=valor;//Frecuencia igual al valor ingresado
 fmuestrat[0]=digitos[0];//Guarda valor en frecuencia de sensor
 fmuestrat[1]=digitos[1];//Guarda valor en frecuencia de sensor
 fmuestrat[2]=digitos[2];//Guarda valor en frecuencia de sensor
 fmuestrat[3]=digitos[3];//Guarda valor en frecuencia de sensor
 }
 else if(r==3)
 {
 for(i=0;i<=1000000;i++)
{
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x47;//Enciende dígito 4 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 3 F
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 2 O
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
frequen[2]=valor;//Frecuencia igual al valor ingresado
 fmuestral[0]=digitos[0];//Guarda valor en frecuencia de sensor
 fmuestral[1]=digitos[1];//Guarda valor en frecuencia de sensor
 fmuestral[2]=digitos[2];//Guarda valor en frecuencia de sensor
 fmuestral[3]=digitos[3];//Guarda valor en frecuencia de sensor
 }
 }
 else if(config==1)
 {
 if(r==2) //TEMPERATURA
 {
 for(i=0;i<=1000000;i++)
{
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x3E;//Enciende dígito 4 S
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 3 U
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 2 L
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1 0
 GPIO_PORTM_DATA_R=0x00;
 }
values[1]=valor;//Valor igual al valor ingresado
 vmuestrat[0]=digitos[0];//Guarda valor en frecuencia de sensor
 vmuestrat[1]=digitos[1];//Guarda valor en frecuencia de sensor
 vmuestrat[2]=digitos[2];//Guarda valor en frecuencia de sensor
 vmuestrat[3]=digitos[3];//Guarda valor en frecuencia de sensor
 setupt=1;
 }
 else if(r==3) //LUZ
 {
 for(i=0;i<=1000000;i++)
{
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x3E;//Enciende dígito 4 S
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 3 U
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 2 L
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1 0
 GPIO_PORTM_DATA_R=0x00;
 }
values[2]=valor;//Valor igual al valor ingresado
 vmuestral[0]=digitos[0];//Guarda valor en frecuencia de sensor
 vmuestral[1]=digitos[1];//Guarda valor en frecuencia de sensor
 vmuestral[2]=digitos[2];//Guarda valor en frecuencia de sensor
 vmuestral[3]=digitos[3];//Guarda valor en frecuencia de sensor
 setupl=1;
 }
 }
 config+=1; //Se suma configuración
 if(r==1)
 {
 if(config==1)
 {
 config=0;
r=0;
 }
 }
 if(config==2)
 {
 config=0;//Si es igual a 2 r se reinicia
 r=0;//R también se reinicia
 }
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 }
 else if(GPIO_PORTK_DATA_R==0x11)//Tecla D presionada
 {
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=0x6D;//Enciende dígito 4 S
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 3
 GPIO_PORTM_DATA_R=0x0E;//Enciende dígito 3 U
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 2
 GPIO_PORTM_DATA_R=0x3E;//Enciende dígito 2 L
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x00;//Enciende dígito 1 0
 GPIO_PORTM_DATA_R=0x00;
 }
 r=3; //Sensor de Luz
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 }
 /* Muestra de datos */
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=digitos[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=digitos[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=digitos[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=digitos[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 /*Lectura ADC*/
 uint32_t data[0];
 ADC0_PSSI_R = 0x0004; //Inicia conversión del SS2
 while((ADC0_RIS_R&0x04)==0); // Espera a que SS2 termine conversión (polling)
 data[1] = (ADC0_SSFIFO2_R&0xFFF);// se lee el primer resultado TEMPERATURA
 data[0] = (ADC0_SSFIFO2_R&0xFFF);// se lee el segundo resultado LUZ
 ADC0_ISC_R = 0x0004; //Limpia la bandera RIS del ADC0
 LUZ=data[0];//Luz se almacena en data0
 TEMP=data[1];//Temp se almacena en data1
 if(setupl==1)
 {
 if(LUZ>=values[2])
 {
 alarma[0]=0x00;
 alarma[1]=0x0E;
 alarma[2]=0x3E;
 alarma[3]=0x6D;
 frecuencia(frequen[2]);
 //setupl=0; //Puede o no quitarse
 }
 }
 if(setupt==1)
 {
 if(TEMP>=values[1])
 {
 alarma[0]=0x0F;
 alarma[1]=0x4F;
 alarma[2]=0x37;
 alarma[3]=0x67;
 frecuencia(frequen[1]);
 //setupt=0; //Puede o no quitarse
 }
 }
 if(muestras==1)
 {
 if(negativo==1)// Si el valor negativo es real
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=digmuestra[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=digmuestra[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=digmuestra[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=0x01;// Dígito 1 es el signo negativo
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 }
 else
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=digmuestra[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=digmuestra[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=digmuestra[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=digmuestra[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 }
 }
 if(GPIO_PORTP_DATA_R==0x01 || angulonegativo==1)
 {
 if(contdig==0){// Solo se acepta el signo si se ingresa al inicio
 digitos[contdig]=0x01;// Se iguala al signo - en 7seg
 contdig+=1; // Se incrementa el valor en 1
 negativo=0x01;// El valor negativo se hace 1
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 }
 }
 if(GPIO_PORTP_DATA_R==0x02 || ingangulo==1)
 {
 for(i=0;i<=0x100000;i++){}// Se espera un rato
 teclaA+=1;
 /* Obtención de grados en valor entero*/
 if(negativo==1)//Si los grados que se registrarán son negativos
 {
 if(contdig==4)// Si ya se llenó el display se registraron 4 dígitos
 {
 p=1;
 grados=digreal[1]*100+digreal[2]*10+digreal[3];//Conversión a decimal
 graditos=trunc(grados/11.2*64);//Se obtienen esos grados en pasos con truncamiento
 digmuestra[0]=digitos[0]; // Se transfieren los datos del 7seg
 digmuestra[1]=digitos[1]; // a otro arreglo para mostrarlo en lo que gira el motor
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
 p=1;
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
 if(p==1)
 {
 if(teclaA==0x01)
 {
 graditosr[0]=graditos;//Guarda grados
 negativor[0]=negativo;//Guarda negativo
 muestra1[0]=digmuestra[0];
 muestra1[1]=digmuestra[1];
 muestra1[2]=digmuestra[2];
 muestra1[3]=digmuestra[3];
 negativo=0;
 }else if(teclaA==0x02)
 {
 graditosr[1]=graditos;//Guarda grados
 negativor[1]=negativo;//Guarda negativo
 muestra2[0]=digmuestra[0];
 muestra2[1]=digmuestra[1];
 muestra2[2]=digmuestra[2];
 muestra2[3]=digmuestra[3];
 negativo=0;
 }else if(teclaA==0x03)
 {
 graditosr[2]=graditos;//Guarda grados
 negativor[2]=negativo;//Guarda negativo
 muestra3[0]=digmuestra[0];
 muestra3[1]=digmuestra[1];
 muestra3[2]=digmuestra[2];
 muestra3[3]=digmuestra[3];
 negativo=0;
 }else if(teclaA==0x04)
 {
 graditosr[3]=graditos;//Guarda grados
 negativor[3]=negativo;//Guarda negativo
 muestra4[0]=digmuestra[0];
 muestra4[1]=digmuestra[1];
 muestra4[2]=digmuestra[2];
 muestra4[3]=digmuestra[3];
 negativo=0;
 }else if(teclaA==0x05)
 {
 graditosr[4]=graditos;//Guarda grados
 negativor[4]=negativo;//Guarda negativo
 muestra5[0]=digmuestra[0];
 muestra5[1]=digmuestra[1];
 muestra5[2]=digmuestra[2];
 muestra5[3]=digmuestra[3];
 negativo=0;
 }else{}
 }
 }
 if(GPIO_PORTP_DATA_R==0x04 || ejecutar==1)
 {
 x=0;
 muestras=0x01;// La muestra se activa para mostrar el display mientras gira
 ingresos+=1;// Los ingresos se vuelven a incrementar
 for(i=0;i<=0x100000;i++){}// Ciclo de espera
 velocidad=40;// Se convierte la velocidad a digito real
 velocidadr=trunc(velocidad*16000000/2057); // Conversión de velocidad a RPM
 //habilita PORTN, PORTF, PORTL
 SYSCTL_RCGCGPIO_R |= 0X7EB0; // RELOJ PARA EL PUERTO F, L y N
 SYSCTL_RCGCTIMER_R |= 0X08; //RELOJ Y HABILITA TIMER 3 (p.380)
 //retardo para que el reloj alcance el PORTN Y TIMER 3
 while ((SYSCTL_PRGPIO_R & 0X1000) == 0){}; // reloj listo?
 TIMER3_CTL_R=0X00000000; //DESHABILITA TIMER 3 PARA CONFIGURAR (p.986)
 TIMER3_CFG_R= 0X00000000; //CONFIGURA TIMER DE 32 BITS (p. 976)
 //TIMER3_TAMR_R= 0X00000002; //CONFIGURAR PARA MODO PERIODICO CUENTA HACIA ABAJO (p. 977)
 TIMER3_TAMR_R= 0X00000012; //CONFIGURAR PARA MODO PERIODICO CUENTA HACIA ARRIBA (p. 977)
 TIMER3_TAILR_R= velocidadr; // VALOR DE RECARGA (p.1004)
// TIMER3_TAILR_R= 0X0004E200; // VALOR DE RECARGA (p.1004)
 TIMER3_TAPR_R= 0X00; // PRESCALADOR DE TIMER A, SOLO PARA MODOS DE 16 BITS (p.1008)
 TIMER3_ICR_R= 0X00000001 ; //LIMPIA POSIBLE BANDERA PENDIENTE DE TIMER3 (p.1002)
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
 GPIO_PORTN_DIR_R = 0xF3; // Habilita PN0-PN1
 GPIO_PORTN_DEN_R = 0xF3; // PN0-PN1 como salida
 // habilita PF0 y PF4 como salida digital para monitoreo del programa
 //
 GPIO_PORTF_AHB_DIR_R = 0x1F; // PF0 y PF4
 GPIO_PORTF_AHB_DEN_R = 0x1F; // PF0 y PF4 como salida
 digitos[0]=0x00; // Se reinician todos los dígitos en 7seg
 digitos[1]=0x00; //dig1
 digitos[2]=0x00; //dig2
 digitos[3]=0x00; //dig3
 contdig=0x00; // El conteo se reinicia
 ingresos=0x00;// Los ingresos se reinician
 Termino=0x00; // El termino del motor se reinicia
 vueltas=teclaA;
 teclaA=0;
 s=0;
 digmuestra[0]=muestra1[0];
 digmuestra[1]=muestra1[1];
 digmuestra[2]=muestra1[2];
 digmuestra[3]=muestra1[3];
 }
 if(GPIO_PORTP_DATA_R==0x08)//Si se presiona PQ3
 {
 for(i=0;i<=100;i++){}
 if(muestraclk==0)
 {
 muestraclk=1;//Iguala a 1 para que se muestre
 }
 else
 {
 muestraclk=0;//Iguala a 0 y ya no se muestra
 }
 }
 if(muestraclk==1)
 {
 conversionD();//Convierte para que se muestre
 }
 }
}
void frecuencia(int frecuenciar)//Funcón frecuencia
 {
 int f;
 f=16000000/frecuenciar;//Convierte frecuencia en ciclos de reloj
 GPIO_PORTK_DATA_R=0x10;//Monitorea valor de la tecla # ingresada
 for(i=0;i<=1000000;i++)
 {
 GPIO_PORTH_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=alarma[3];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=alarma[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=alarma[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTH_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=alarma[0];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 }
 while(GPIO_PORTK_DATA_R!=0x18)//Si se teclea #
 {
 for(i=0;i<=f;i++)//Ciclo encendido
UNIVERSIDAD NACIONAL AUTÓNOMA DE MÉXICO
FACULTAD DE INGENIERÍA
Microprocesadores y Microcontroladores
Semestre: 2021-1
Página 43 de 52 Prof. Dr. Saúl De La Rosa Nieves Grupo 3
 {
 GPIO_PORTQ_DATA_R=0X01;//Enciende buzzer
 }
 for(i=0;i<=f;i++)//Ciclo apagado
 {
 GPIO_PORTQ_DATA_R=0X00;//Apaga buzzer
 }
 }
 }
void conversionD()//Función que convierte lectura de 12C a conversión en 7S para 14 dígitos
{
 int digitoRTC;
 int sevens;
 int sevenar;
 int j=0;
 int k=0;
 int n=0;
 for(j=0;j<=6;j++)
 {
 if(j==0)
 {
 segundos=convertidorsin(segundos);//Convierte dato
 digitoRTC=segundos;//Iguala variable
 }
 if(j==1)
 {
 minutos=convertidorsin(minutos);//Convierte dato
 digitoRTC=minutos;//Iguala variable
 }
 if(j==2)
 {
 horas=convertidorsin(horas);//Convierte dato
 digitoRTC=horas;//Iguala variable
 }
 if(j==3)
 {
 dia=convertidorsin(dia);//Convierte dato
 digitoRTC=dia;//Iguala variable
 }
 if(j==4)
 {
 digitoRTC=fecha;//Iguala variable, como es menor a 9 no se necesita convertir
 }
 if(j==5)
 {
 mes=convertidorsin(mes);//Convierte dato
 digitoRTC=mes;//Iguala variable
 }
 if(j==6)
 {
 anio=convertidorsin(anio);//Convierte dato
 digitoRTC=anio;//Iguala variable
 }
 dig1=digitoRTC&0x0F;//Obtiene digito1
 dig2=digitoRTC&0xF0;//Obtiene dígito2
 digdec=dig2+dig1;//Conversión a decimal
 dec3=digdec/100;//Obtiene unidades
 dec2=(digdec-dec3*100)/10;//Obtiene decenas
 dec1=(digdec-dec3*100-dec2*10);//Obtiene centenas
 unidadesd[n]=dec1;//Almacena unidades
 decenasd[n]=dec2;//Almacena decenas
 n=n+1;//Incremento
 }
 for(j=0;j<=13;j++)//For para ir llenando los arreglos
 {
 for(k=0;k<=1;k++)
 {
 if(k==0)
 {
 sevens=decenasd[j];//Almancena en el arreglo para ser mostrado
 }
 if(k==1)
 {
 sevens=unidadesd[j];//Almancena en el arreglo para ser mostrado
 }
 if(sevens==1)
 {
 sevenar=0x30;//Almancena en el arreglo para ser mostrado
 }
 if(sevens==2)
 {
 sevenar=0x6D;//Almancena en el arreglo para ser mostrado
 }
 if(sevens==3)
 {
 sevenar=0x79;//Almancena en el arreglo para ser mostrado
 }
 if(sevens==4)
 {
 sevenar=0x33;//Almancena en el arreglo para ser mostrado
 }
 if(sevens==5)
 {
 sevenar=0x5B;//Almancena en el arreglo para ser mostrado
 }
 if(sevens==6)
 {
 sevenar=0x5F;//Almancena en el arreglo para ser mostrado
 }
 if(sevens==7)
 {
 sevenar=0x70;//Almancena en el arreglo para ser mostrado
 }
 if(sevens==8)
 {
 sevenar=0x7F;//Almancena en el arreglo para ser mostrado
 }
 if(sevens==9)
 {
 sevenar=0x7B;//Almancena en el arreglo para ser mostrado
 }
 if(sevens==0)
 {
 sevenar=0x7E;//Almancena en el arreglo para ser mostrado
 }
 if(k==0)
 {
 segmentosd[j]=sevenar;//Almancena en el arreglo para ser mostrado
 }
 if(k==1)
 {
 segmentosu[j]=sevenar;//Almancena en el arreglo para ser mostrado
 }
 }
 }
 for(i=0;i<=100;i++)
 {
 GPIO_PORTH_DATA_R=0xFF;//Apaga puerto para que se muestre solo el que se requiere
 GPIO_PORTD_DATA_R=0xFF;//Apaga puerto para que se muestre solo el que se requiere
 GPIO_PORTF_DATA_R=0xFF;//Apaga puerto para que se muestre solo el que se requiere
 GPIO_PORTG_DATA_R=0xFF;//Apaga puerto para que se muestre solo el que se requiere
 GPIO_PORTN_DATA_R=0xFF;//Apaga puerto para que se muestre solo el que se requiere
 GPIO_PORTE_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=segmentosu[0];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=segmentosd[0];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=segmentosu[1];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTE_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=segmentosd[1];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 GPIO_PORTE_DATA_R=0xFF;// Habilita dígito 0
 GPIO_PORTD_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=segmentosu[2];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTD_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=segmentosd[2];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTD_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=segmentosu[3];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTD_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=segmentosd[3];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 GPIO_PORTD_DATA_R=0xFF;//Apaga puerto para que se muestre solo el que se requiere
 GPIO_PORTF_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=segmentosu[4];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTF_DATA_R=0xFB;// Habilita dígito 2
 GPIO_PORTM_DATA_R=segmentosd[4];//Enciende dígito 2
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTF_DATA_R=0xF7;// Habilita dígito 1
 GPIO_PORTM_DATA_R=segmentosu[5];//Enciende dígito 1
 GPIO_PORTM_DATA_R=0x00;
 GPIO_PORTF_DATA_R=0xFF;//Apaga puerto para que se muestre solo el que se requiere
 GPIO_PORTG_DATA_R=0xFE;// Habilita dígito 4
 GPIO_PORTM_DATA_R=segmentosd[5];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTG_DATA_R=0xFD;// Habilita dígito 3
 GPIO_PORTM_DATA_R=segmentosu[6];//Enciende dígito 3
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTG_DATA_R=0xFF;
 GPIO_PORTN_DATA_R=0xEF;// Habilita dígito 4
 GPIO_PORTM_DATA_R=segmentosd[6];//Enciende dígito 4
 GPIO_PORTM_DATA_R=0x00;// Apaga los 7 segmentos
 GPIO_PORTG_DATA_R=0xFF;//Apaga puerto para que se muestre solo el que se requiere
 }
}
int convertidorsin(int digitc)//Función que convierte el ciclo de 0-90 a 0-60
{
 if(digitc<=9)
 {
 digitc=digitc;//Conversión de datos
 }
 else if(digitc<=25)
 {
 digitc=digitc-6;//Conversión de datos
 }
 else if(digitc<=41)
 {
 digitc=digitc-12;//Conversión de datos
 }
 else if(digitc<=57)
 {
 digitc=digitc-18;//Conversión de datos
 }
 else if(digitc<=73)
 {
 digitc=digitc-24;//Conversión de datos
 }
 else if(digitc<=90)
 {
 digitc=digitc-30;//Conversión de datos
 }
 return digitc;
}