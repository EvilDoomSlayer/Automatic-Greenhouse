/*================================================================================
Programa de la Incubadora
*Este prgograma controla:
-Shield Temp
-Shield L293D_V2
-Shield LCD
Este programa tiene como funcion automatizar todos los procesos de este sistema
================================================================================*/



/*
██╗░░░░░██╗██████╗░██████╗░███████╗██████╗░██╗░█████╗░░██████╗
██║░░░░░██║██╔══██╗██╔══██╗██╔════╝██╔══██╗██║██╔══██╗██╔════╝
██║░░░░░██║██████╦╝██████╔╝█████╗░░██████╔╝██║███████║╚█████╗░
██║░░░░░██║██╔══██╗██╔══██╗██╔══╝░░██╔══██╗██║██╔══██║░╚═══██╗
███████╗██║██████╦╝██║░░██║███████╗██║░░██║██║██║░░██║██████╔╝
╚══════╝╚═╝╚═════╝░╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝╚═╝╚═╝░░╚═╝╚═════╝░
*/
//Se incluyen la libreria de la LCD y los numeros de pines de esta
#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);




/*
██╗░░░██╗░█████╗░██████╗░██╗░█████╗░██████╗░██╗░░░░░███████╗░██████╗
██║░░░██║██╔══██╗██╔══██╗██║██╔══██╗██╔══██╗██║░░░░░██╔════╝██╔════╝
╚██╗░██╔╝███████║██████╔╝██║███████║██████╦╝██║░░░░░█████╗░░╚█████╗░
░╚████╔╝░██╔══██║██╔══██╗██║██╔══██║██╔══██╗██║░░░░░██╔══╝░░░╚═══██╗
░░╚██╔╝░░██║░░██║██║░░██║██║██║░░██║██████╦╝███████╗███████╗██████╔╝
░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚═╝╚═╝╚═╝░░╚═╝╚═════╝░╚══════╝╚══════╝╚═════╝░
*/
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//Se definen los pines y las variables--------------------------------------------
  /*
  ▀█▀ █▀▀ █▀▄▀█ █▀█ █▀▀ █▀█ ▄▀█ ▀█▀ █░█ █▀█ ▄▀█
  ░█░ ██▄ █░▀░█ █▀▀ ██▄ █▀▄ █▀█ ░█░ █▄█ █▀▄ █▀█
  */
  //TEMP==========================================================================
  #define FOCO 12 //Pin del foco
  #define ABANICO 13  //Pin del abanico
  #define TEMPERATURA_ANALIZADA 100000000.0 //Temperatura que se debe de alcanzar en la incubadora
  int SENSOR;  //Se define la variable donde se va almacenar el valor leido en el AnalogRead del sensor
  float TEMP;  //Se define la variable en la cual se va a almacenar el valor de temperatura
  float SUMA; //Se define la variable en donde se va a lamacenar toda la suma de las tempearuras para sacar una promedio
  float TEMPPROM = 0.0; //Se define la variable que sera la temperatura promedio
  float tempPrint = 0.0; //Se define la variable en la cual se va a almacenar la temperatura medida de la funcion para imprimir la temperatura
  float tempCntrl = 0.0; //Se define la variable en la cual se va a almacenar la temperatura medida de la funcion para controlar la temperatura
  //==============================================================================


  /* 
  █▀▄▀█ █▀█ ▀█▀ █▀█ █▀█
  █░▀░█ █▄█ ░█░ █▄█ █▀▄
  */
  //MOTOR=========================================================================
  #define MOTOREBDER 9  //Pin del Motor para girar a la derecha 
  #define MOTOREBIZQ 8  //Pin del Motor para girar a la izquierda 
  #define PWM 10  //Pin del Motor para controlar la potencia de giro 
  #define POTENCIA_DE_GIRO 110 //Declara con que potencia se girara el motor (Valor entre 0-255)
  #define TIEMPO_DE_GIRO 5000 //Es el tiempo que va a girar el motor en milisegundos
  bool x = false; //Variable toogle para iniciar el giro del motor
  //==============================================================================


 /*  
  █▀▀ █▀█ █▀█ █▄░█ █▀█ █▀▄▀█ █▀▀ ▀█▀ █▀█ █▀█
  █▄▄ █▀▄ █▄█ █░▀█ █▄█ █░▀░█ ██▄ ░█░ █▀▄ █▄█
  */
  //CRONOMETRO====================================================================
  #define TIEMPO_DE_INCUBAR 35 //Tiempo de incubacion en segundos
  bool INICIO = false; //Variabe booleana la cual es la que inicia o no la incubadora
  unsigned int SEGUNDOS = TIEMPO_DE_INCUBAR; // Es la variable que va a cambiar sobre tiempo va disminuyendo respecto al tiempo de incubacion definido
  unsigned int thoras = 0; //Es la variable que almacena el tiempo restante en horas
  unsigned int tminutos = 0;  //Es la variable que almacena el tiempo restante en minutos
  unsigned int tsegundos = 0;  //Es la variable que almacena el tiempo restante en segundos
  unsigned int TIEMP = 0;  //Es la variable en la cual se va a almacenar el tiempo que resta desde la funcion del temporizador
  //==============================================================================


  /*
  █░░ █▀▀ █▀▄
  █▄▄ █▄▄ █▄▀
  */
  //LCD===========================================================================
  #define BTN_INICIO 0 //Es el numero matricial de el boton
  #define BTN_TEMPERATURA 1  //Es el numero matricial de el boton
  #define BTN_TIEMPO 2  //Es el numero matricial de el boton
  #define BTN_REINICIAR 3  //Es el numero matricial de el boton
  
  uint8_t button[4] = { //Es el arreglo en el cual se guardan los pines de los botones
  19,
  18,
  17,
  16
  };

  uint8_t button_estate[4]; //Define la cantidad de botones en el arreglo

  #define S_DeFault 0 //Define cual es el estado en el que se encuentra la incuvadora (Default)
  #define S_INICIO 1  //Define cual es el estado en el que se encuentra la incuvadora (INICIO)
  #define S_TEMPERATURA 2 //Define cual es el estado en el que se encuentra la incuvadora (TEMPERATURA)
  #define S_TIEMPO 3  //Define cual es el estado en el que se encuentra la incuvadora (TIEMPO)
  #define S_REINICIAR 4 //Define cual es el estado en el que se encuentra la incuvadora (REINICIO)

  uint8_t estado = S_DeFault; //Define el estado base como el Default
  int Reinicio_Count = 0; //Es la variable en la cual se va a almacenar la cantidad de veces que se ha presionado el boton reinicio con el objetivo de que se deba de presionar 3 veces para reiniciar e sistema
  int contador = 0;  //Nos sirve para hacer parpadear la funcion Screen0
  //==============================================================================



  /*
  █▀▀ █░█ █▀ ▀█▀ █▀█ █▀▄▀█ █▀▀ █░█ ▄▀█ █▀█
  █▄▄ █▄█ ▄█ ░█░ █▄█ █░▀░█ █▄▄ █▀█ █▀█ █▀▄
  */
  //CUSTOMCHAR====================================================================
  byte POINT[] = { //Es la representacion de un punto en la LCD (.)
  B00000,
  B00000,
  B00000,
  B00100,
  B01110,
  B11111,
  B01110,
  B00100
  };

  byte DEGREE[] = { //Es la representacion de un grado en la LCD (°)
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
  };
  
  byte HUEVOIZQ[] = { //Es la representacion de el lado izquierdo de un huevo
  B00011,
  B00111,
  B00111,
  B01111,
  B01111,
  B01111,
  B00111,
  B00011
  };

byte HUEVODER[] = {  //Es la representacion de el lado derecho de un huevo
  B10000,
  B11000,
  B11000,
  B11100,
  B11100,
  B11100,
  B11000,
  B10000
  };
  //==============================================================================
  //------------------------------------------------------------------------------
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>














/*
░██████╗███████╗████████╗██╗░░░██╗██████╗░
██╔════╝██╔════╝╚══██╔══╝██║░░░██║██╔══██╗
╚█████╗░█████╗░░░░░██║░░░██║░░░██║██████╔╝
░╚═══██╗██╔══╝░░░░░██║░░░██║░░░██║██╔═══╝░
██████╔╝███████╗░░░██║░░░╚██████╔╝██║░░░░░
╚═════╝░╚══════╝░░░╚═╝░░░░╚═════╝░╚═╝░░░░░
*/
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// CONFIGURACIONES----------------------------------------------------------------
  void setup() {
    /*
    ▀█▀ █▀▀ █▀▄▀█ █▀█ █▀▀ █▀█ ▄▀█ ▀█▀ █░█ █▀█ ▄▀█
    ░█░ ██▄ █░▀░█ █▀▀ ██▄ █▀▄ █▀█ ░█░ █▄█ █▀▄ █▀█
    */
    //TEMP========================================================================
    pinMode(FOCO, OUTPUT); //Se define el pin del foco como output
    pinMode(ABANICO, OUTPUT);  //Se define el pin del abanico como output
    //============================================================================


    /* 
    █▀▄▀█ █▀█ ▀█▀ █▀█ █▀█
    █░▀░█ █▄█ ░█░ █▄█ █▀▄
   */
    //MOTOR=======================================================================
    pinMode(MOTOREBDER, OUTPUT); //Se define el pin del Motor hacia la derecha como output
    pinMode(MOTOREBIZQ, OUTPUT); //Se define el pin del Motor hacia la izquierda como output
    pinMode(PWM, OUTPUT);  //Se define el pin de la potencia como output
    MOFF(); //Se le llama a la funcion de apagar el motor para que el programa arranque con el motor apagado
    //============================================================================


    /*
    █░░ █▀▀ █▀▄
    █▄▄ █▄▄ █▄▀
    */
    //LCD=========================================================================
    //Inicia las configuraciones de la impresion
    lcd.begin(16, 2); 
    lcd.setCursor(3, 0);
    lcd.print("Bienvenido");
    //Imprime Bienvenido 
    pinMode(button[BTN_INICIO], INPUT_PULLUP); //Se define el pin del boton 1 como input tipo pullup
    pinMode(button[BTN_TEMPERATURA], INPUT_PULLUP);  //Se define el pin del boton 2 como input tipo pullup
    pinMode(button[BTN_TIEMPO], INPUT_PULLUP);  //Se define el pin del boton 3 como input tipo pullup
    pinMode(button[BTN_REINICIAR], INPUT_PULLUP);  //Se define el pin del boton 4 como input tipo pullup

    //Define los estados de los botones como High
    button_estate[0] = HIGH; 
    button_estate[1] = HIGH;
    button_estate[2] = HIGH;
    button_estate[3] = HIGH;
    //============================================================================
  

    /*
    █▀▀ █░█ █▀ ▀█▀ █▀█ █▀▄▀█ █▀▀ █░█ ▄▀█ █▀█
    █▄▄ █▄█ ▄█ ░█░ █▄█ █░▀░█ █▄▄ █▀█ █▀█ █▀▄
    */
    //CUSTOMCHAR==================================================================
    lcd.createChar(0, POINT); //Crea el caracter con lo anteriormente declarado
    lcd.createChar(1, DEGREE); //Crea el caracter con lo anteriormente declarado
    lcd.createChar(2, HUEVOIZQ); //Crea el caracter con lo anteriormente declarado
    lcd.createChar(3, HUEVODER); //Crea el caracter con lo anteriormente declarado
    Serial.begin(9600); //Define el monitor serial por si se llega a ocupar.
    //============================================================================
    //----------------------------------------------------------------------------
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

















/*
███╗░░░███╗░█████╗░██╗███╗░░██╗
████╗░████║██╔══██╗██║████╗░██║
██╔████╔██║███████║██║██╔██╗██║
██║╚██╔╝██║██╔══██║██║██║╚████║
██║░╚═╝░██║██║░░██║██║██║░╚███║
╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝╚═╝░░╚══╝
*/
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//PROGRAMA PRINCIPAL////////////////////////////////////////////////////////////////
void loop(){


  /*
  █ █▄░█ █ █▀▀ █ ▄▀█ █▀█
  █ █░▀█ █ █▄▄ █ █▀█ █▀▄
  */
  //Checa si se inicia o no la incubadora
  if (INICIO == true){
    float tempCntrl = MEDTEMP(); //Extralle la temperatura promedio
    TEMPCONTRL(tempCntrl); //Usa la temperatura extraida para controlar el foco
    TIEMP = TEMPORIZADOR(); //Saca el tiempo e inicia el temporizador
    if (TIEMP == 0){ //Checa si se acaba el tiempo
      SEGUNDOS = TIEMPO_DE_INCUBAR;
      x = false; // Si se acaba el tiempo se reinicia todo
    }
    if ( x == false){ //Si se inica o se acaba el tiempo empieza a girar el motor
      PANTALLA_HUEVO(); //Imprime el mensaje de que el huevo se esta girando
      x = GIRO(x); //Se llama a la funcion de girado del huevo
      lcd.clear(); //Se limpia la LCD para la siguiente pantalla
    }
  }


  /* 
  █▀ █░█░█ █ ▀█▀ █▀▀ █░█
  ▄█ ▀▄▀▄▀ █ ░█░ █▄▄ █▀█
  */
  switch(estado) { //Switch que chequea todos los botones y en que estado esta el menu


  /*
  █▀▀ ▄▀█ █▀ █▀█   █▀▄ █▀▀ █▀▀ ▄▀█ █░█ █░░ ▀█▀
  █▄▄ █▀█ ▄█ █▄█   █▄▀ ██▄ █▀░ █▀█ █▄█ █▄▄ ░█░
  */
    case S_DeFault: //Es el caso en que no se presione nada o se reinicie o se acaba de iniciar la incubadora 
      SCREEN_DeFault();
      if(flancoSubida(BTN_INICIO)) { //Checa si se oprimio el boton 1 (Boton de inicio)
        estado = S_INICIO; //Si se presiono se cambia el estado de la pantalla al de inicio
          lcd.clear(); //Se limpia los mensajes de la LCD
         INICIO = true; //Se inicia la incubadora
          ABANICENC(); //Se enciende el abanico
        break; //Acaba el if
      }
    break; //Acaba el caso DeFault


    /*
    █▀▀ ▄▀█ █▀ █▀█   █ █▄░█ █ █▀▀ █ █▀█
    █▄▄ █▀█ ▄█ █▄█   █ █░▀█ █ █▄▄ █ █▄█
    */
    case S_INICIO: //Es el estado en el que se muestra que se esta incubando
      SCREEN_MENU(); //Llama la funcion de la pantalla de inicio
      SCREEN_MENU_POINTS(); //Lama la funcion de la pantalla de inicio que imprime los 3 puntos suspensivos


      if(flancoSubida(BTN_TEMPERATURA)) { //Checa si se oprimio el boton 2 (Boton de temperatura)
        estado = S_TEMPERATURA; //Si se presiono se cambia el estado de la pantalla al de temperatura
        lcd.clear(); //Se limpia los mensajes de la LCD
        break;  //Acaba el if
      }
      
      if(flancoSubida(BTN_TIEMPO)) {  //Checa si se oprimio el boton 3 (Boton de tiempo)
        estado = S_TIEMPO;  //Si se presiono se cambia el estado de la pantalla al de tiempo
        lcd.clear(); //Se limpia los mensajes de la LCD
        break;  //Acaba el if
      }

      if(flancoSubida(BTN_REINICIAR)) {  //Checa si se oprimio el boton 4 (Boton de reinicio)
        estado = S_REINICIAR;  //Si se presiono se cambia el estado de la pantalla al de reinicio
        Reinicio_Count ++; //Suma un numero para el boton de reinicio
        lcd.clear();  //Se limpia los mensajes de la LCD
        SCREEN_REINICIO(); //Se imprime la pantalla de Reinicio
        break;  //Acaba el if
      }
    break; //Acaba el caso INICIO


    /*
    █▀▀ ▄▀█ █▀ █▀█   ▀█▀ █▀▀ █▀▄▀█ █▀█ █▀▀ █▀█ ▄▀█ ▀█▀ █░█ █▀█ ▄▀█
    █▄▄ █▀█ ▄█ █▄█   ░█░ ██▄ █░▀░█ █▀▀ ██▄ █▀▄ █▀█ ░█░ █▄█ █▀▄ █▀█
    */
    case S_TEMPERATURA:  //Es el estado en el que muestra la temperatura
      SCREEN_TEMPERATURA(); //Llama a la funcion de la impresion de la pantalla de temperatura


      if(flancoSubida(BTN_INICIO)) { //Checa si se oprimio el boton 1 (Boton de inicio)
        estado = S_INICIO;  //Si se presiono se cambia el estado de la pantalla al de inicio
          lcd.clear(); //Se limpia los mensajes de la LCD
        break;  //Acaba el if
      }


      if(flancoSubida(BTN_TIEMPO)) { //Checa si se oprimio el boton 3 (Boton de tiempo)
        estado = S_TIEMPO;  //Si se presiono se cambia el estado de la pantalla al de tiempo
        lcd.clear(); //Se limpia los mensajes de la LCD
        break;  //Acaba el if
      }


      if(flancoSubida(BTN_REINICIAR)) {  //Checa si se oprimio el boton 4 (Boton de reinicio)
        estado = S_REINICIAR;  //Si se presiono se cambia el estado de la pantalla al de reinicio
        Reinicio_Count ++;  //Suma un numero para el boton de reinicio
        lcd.clear();  //Se limpia los mensajes de la LCD
        SCREEN_REINICIO();  //Se imprime la pantalla de Reinicio
        break;  //Acaba el if
      }
    break;  //Acaba el caso TEMPERATURA

    /*
    █▀▀ ▄▀█ █▀ █▀█   ▀█▀ █ █▀▀ █▀▄▀█ █▀█ █▀█
    █▄▄ █▀█ ▄█ █▄█   ░█░ █ ██▄ █░▀░█ █▀▀ █▄█
    */
    case S_TIEMPO:
      TIEMP = TEMPORIZADOR(); //Variable que guarda el tiempo que resta del temporizador
      SCREEN_TIEMPO(TIEMP); //Imprime la pantalla tiempo y usa la variable TIEMP para imprimir el tiempo que resta


      if(flancoSubida(BTN_INICIO)) { //Checa si se oprimio el boton 1 (Boton de inicio)
        estado = S_INICIO;  //Si se presiono se cambia el estado de la pantalla al de inicio
          lcd.clear();  //Se limpia los mensajes de la LCD
        break;  //Acaba el if
      }

      if(flancoSubida(BTN_TEMPERATURA)) { //Checa si se oprimio el boton 2 (Boton de temperatura)
        estado = S_TEMPERATURA; //Si se presiono se cambia el estado de la pantalla al de temperatura
        lcd.clear(); //Se limpia los mensajes de la LCD
        break;  //Acaba el if
      }

      if(flancoSubida(BTN_REINICIAR)) {  //Checa si se oprimio el boton 4 (Boton de reinicio)
        estado = S_REINICIAR;  //Si se presiono se cambia el estado de la pantalla al de reinicio
        Reinicio_Count ++;  //Suma un numero para el boton de reinicio
        lcd.clear();  //Se limpia los mensajes de la LCD
        SCREEN_REINICIO();  //Se imprime la pantalla de Reinicio
        break;  //Acaba el if
      }
    break; //Acaba el caso TIEMPO

    /*
    █▀▀ ▄▀█ █▀ █▀█   █▀█ █▀▀ █ █▄░█ █ █▀▀ █ ▄▀█ █▀█
    █▄▄ █▀█ ▄█ █▄█   █▀▄ ██▄ █ █░▀█ █ █▄▄ █ █▀█ █▀▄
    */
    case S_REINICIAR:
      if(flancoSubida(BTN_INICIO)) { //Checa si se oprimio el boton 1 (Boton de inicio)
        Reinicio_Count = 0;  //Suma un numero para el boton de reinicio
        estado = S_INICIO;  //Si se presiono se cambia el estado de la pantalla al de inicio
          lcd.clear();  //Se limpia los mensajes de la LCD
        break;  //Acaba el if
      }

      if(flancoSubida(BTN_TEMPERATURA)) { //Checa si se oprimio el boton 2 (Boton de temperatura)
        Reinicio_Count = 0;  //Suma un numero para el boton de reinicio
        estado = S_TEMPERATURA; //Si se presiono se cambia el estado de la pantalla al de temperatura
        lcd.clear(); //Se limpia los mensajes de la LCD
        break;  //Acaba el if
      }
      
      if(flancoSubida(BTN_TIEMPO)) { //Checa si se oprimio el boton 3 (Boton de tiempo)
        Reinicio_Count = 0;
        estado = S_TIEMPO;  //Si se presiono se cambia el estado de la pantalla al de tiempo
        lcd.clear(); //Se limpia los mensajes de la LCD
        break;  //Acaba el if
      }

      if(flancoSubida(BTN_REINICIAR)) {  //Checa si se oprimio el boton 4 (Boton de reinicio) de nuevo
      Reinicio_Count ++;  //Suma un numero para el boton de reinicio
        if(Reinicio_Count >= 3){ //Si se ha presionado 3 veces seguidas el boton de REINICIAR:
        ACCION_DE_REINICIO(); //Hace las acciones de reinicio
        break; //Acaba el if
      }
      break; //Acaba el if
    }
    break; //Acaba el caso TIEMPO
 }
}
///////////////////////////////////////////////////////////////////////////////////
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>















/*
░██████╗██╗░░░██╗██████╗░░░░░░░███████╗██╗░░░██╗███╗░░██╗░█████╗░██╗░█████╗░███╗░░██╗███████╗░██████╗
██╔════╝██║░░░██║██╔══██╗░░░░░░██╔════╝██║░░░██║████╗░██║██╔══██╗██║██╔══██╗████╗░██║██╔════╝██╔════╝
╚█████╗░██║░░░██║██████╦╝█████╗█████╗░░██║░░░██║██╔██╗██║██║░░╚═╝██║██║░░██║██╔██╗██║█████╗░░╚█████╗░
░╚═══██╗██║░░░██║██╔══██╗╚════╝██╔══╝░░██║░░░██║██║╚████║██║░░██╗██║██║░░██║██║╚████║██╔══╝░░░╚═══██╗
██████╔╝╚██████╔╝██████╦╝░░░░░░██║░░░░░╚██████╔╝██║░╚███║╚█████╔╝██║╚█████╔╝██║░╚███║███████╗██████╔╝
╚═════╝░░╚═════╝░╚═════╝░░░░░░░╚═╝░░░░░░╚═════╝░╚═╝░░╚══╝░╚════╝░╚═╝░╚════╝░╚═╝░░╚══╝╚══════╝╚═════╝░
*/
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/*
░█▀▀█ ─█▀▀█ ░█▀▀▀█ ▀█▀ ░█▀▀█ ─█▀▀█ ░█▀▀▀█ 
░█▀▀▄ ░█▄▄█ ─▀▀▀▄▄ ░█─ ░█─── ░█▄▄█ ─▀▀▀▄▄ 
░█▄▄█ ░█─░█ ░█▄▄▄█ ▄█▄ ░█▄▄█ ░█─░█ ░█▄▄▄█
*/
/*
▀█▀ █▀▀ █▀▄▀█ █▀█ █▀▀ █▀█ ▄▀█ ▀█▀ █░█ █▀█ ▄▀█
░█░ ██▄ █░▀░█ █▀▀ ██▄ █▀▄ █▀█ ░█░ █▄█ █▀▄ █▀█
*/
// Procesos de TEMP---------------------------------------------------------------
  //FOCO==========================================================================
  void FOCOENC() {  //Enciende el foco
    digitalWrite(FOCO,HIGH);
  }

  void FOCOOFF() {  //Apaga el foco
    digitalWrite(FOCO,LOW);
  }
  //==============================================================================

  //ABANICO=======================================================================
  void ABANICENC() { //Enceinde el Abaico
    digitalWrite(ABANICO,HIGH);
  }

  void ABANICOFF() { //Apaga el abanico
    digitalWrite(ABANICO,LOW);
  }
  //==============================================================================
  //------------------------------------------------------------------------------


/* 
█▀▄▀█ █▀█ ▀█▀ █▀█ █▀█
█░▀░█ █▄█ ░█░ █▄█ █▀▄
*/
// Procesos de los motores--------------------------------------------------------
  //MOTOR=========================================================================
  void MDER() {  //Hace girar el motor hacia la derecha
    analogWrite(PWM, POTENCIA_DE_GIRO); //Declara la potencia de giro
    digitalWrite(MOTOREBDER,PWM);
    digitalWrite(MOTOREBIZQ,LOW);
    }

  void MIZQ() { //Hace girar el motor a la izquierda
    analogWrite(PWM, POTENCIA_DE_GIRO); //Declara la potencia de giro
    digitalWrite(MOTOREBDER,LOW);
    digitalWrite(MOTOREBIZQ,HIGH);
    }
 
   void MOFF() {  //Apaga el motor
    digitalWrite(MOTOREBDER,LOW);
    digitalWrite(MOTOREBIZQ,LOW);
    }
  //==============================================================================
  //------------------------------------------------------------------------------
 /*
░█▀▀▀█ ░█─░█ ░█▀▀█ ░█▀▀█ ░█▀▀█ ░█▀▀▀█ ░█▀▀█ ░█▀▀▀ ░█▀▀▀█ ░█▀▀▀█ ░█▀▀▀█ 
─▀▀▀▄▄ ░█─░█ ░█▀▀▄ ░█▄▄█ ░█▄▄▀ ░█──░█ ░█─── ░█▀▀▀ ─▀▀▀▄▄ ░█──░█ ─▀▀▀▄▄ 
░█▄▄▄█ ─▀▄▄▀ ░█▄▄█ ░█─── ░█─░█ ░█▄▄▄█ ░█▄▄█ ░█▄▄▄ ░█▄▄▄█ ░█▄▄▄█ ░█▄▄▄█
*/

/*
▀█▀ █▀▀ █▀▄▀█ █▀█ █▀█ █▀█ █ ▀█ ▄▀█ █▀▄ █▀█ █▀█
░█░ ██▄ █░▀░█ █▀▀ █▄█ █▀▄ █ █▄ █▀█ █▄▀ █▄█ █▀▄
*/
// Temporizador GLOBAL------------------------------------------------------------ 
  //Temporizador==================================================================
  int TEMPORIZADOR(){
  static long ultimo_cambio = 0;
  long hora = millis();
    while (SEGUNDOS > 0){
      if (hora - ultimo_cambio > 1000){
	      ultimo_cambio = hora;
        SEGUNDOS --;
    }
    return SEGUNDOS;
    }
  }  
  //==============================================================================
  //------------------------------------------------------------------------------

/*
▀█▀ █▀▀ █▀▄▀█ █▀█ █▀▀ █▀█ ▄▀█ ▀█▀ █░█ █▀█ ▄▀█
░█░ ██▄ █░▀░█ █▀▀ ██▄ █▀▄ █▀█ ░█░ █▄█ █▀▄ █▀█
*/
// Procesos de TEMP--------------------------------------------------------------- 
  //MEDICION DEL SENSOR DE TEMPERATURA============================================
  float MEDTEMP(){ //Funcion que mide la temperatura
    static long ultimo_cambio = 0; //Variable para aplicar el delay
    static int Estado = 0; //Variable para hacer que la LCD cambie de estado de imprimir a borrar para crear el efecto de parpadeo
    long hora = millis(); //Es la variable en la que se va a almacenar el tiempo para sustituir al delay
    SUMA = 0; //Es la variable donde se va a ir sumando las temperaturas cada ciclo
    if (hora - ultimo_cambio > 500){  //Esta funcion sustituye al delay de 0.1 segundos
      ultimo_cambio = hora; //Cambia para reiniciar el delay
          if (Estado == 1){
          for (int y=0; y<10; y++){
          SENSOR= analogRead(A0);
          TEMP= ((SENSOR * 5000.0) / 1023.0)/10.0;
          SUMA = TEMP + SUMA;
          }
        }
        else {  //SI NO SE ESTA EJECUTANDO LA IMPRESION
        Estado = 1; //El estado pasa a 1 para reiniciar el bucle
        }
      TEMPPROM = SUMA/10.0; //Es la suma de todas las temperaturas entre el numero de ciclos que se hizo
    }
     return TEMPPROM;
  }
  //==============================================================================


  //CONTROL DE PROCESOS DEPENDIENDO DE LA TEMPERATURA=============================
  void TEMPCONTRL(float promediotem){ //Funcion que toma deciciones respecto a la temperatura medida
    if (promediotem < TEMPERATURA_ANALIZADA){ //Si la temperatura promedio de la incubadora es menor que la que debe de ser:
      FOCOENC(); //Se enciende el foco
    }
    else { //Si no es menor la temperatura promedio, por lo tanto es mayor:
      FOCOOFF(); //Se apaga el foco
    }
  }
  //==============================================================================
  //------------------------------------------------------------------------------


/* 
█▀▄▀█ █▀█ ▀█▀ █▀█ █▀█
█░▀░█ █▄█ ░█░ █▄█ █▀▄
*/
// Procesos de los motores--------------------------------------------------------
  //Tiempo de giro de motor=======================================================
  bool GIRO(bool y){ //Es la funcion que hace girar el motor y despues hace que se apague
    MDER(); //Hace que el motor gire hacia la derecha
    delay(TIEMPO_DE_GIRO); //Pone un delay para que no se pueda ejecutar nada mas que el giro del motor por la cantidad de tiempo de giro
    MOFF(); //Apaga el motor
    y = true; //Cambia la variable a verdadero para dejar de correr este proceso
    return y; //Devuelve la variable cambiada a verdadero para acabar con el proceso
  }

  /* Funcion de giro que no funciono, no se porque :(
  bool GIRAR(bool y){
    static long ultimo_cambio = 0;
    long hora = millis();
    MDER();
    if (hora - ultimo_cambio > 5000){
	    ultimo_cambio = hora;
      MOFF();
      y = true;
    }
    return y;
  }
  */
  //==============================================================================
  //------------------------------------------------------------------------------


/*
█▀█ █▀▀ █ █▄░█ █ █▀▀ █ █▀█
█▀▄ ██▄ █ █░▀█ █ █▄▄ █ █▄█
*/
// Procesos de REINICIO-----------------------------------------------------------
  //Accion de reinicio============================================================
  void ACCION_DE_REINICIO(){ //Accion que pone todos los parametros como si se arracnara el programa de 0
    estado = S_DeFault; //Reinicia el estado a DeFault
    SEGUNDOS = TIEMPO_DE_INCUBAR; //Reinicia los segundos de la incubacion
    INICIO = false; //Declara que no se inicie el programa
    x = false; //Reinicia la variable para que gire el motor
    Reinicio_Count = 0; //Pone el boton de reinicio en 0
    FOCOOFF(); //Apaga el foco
    ABANICOFF();  //Apaga el foco
    SCREEN_DeFault(); //Pone la pantalla de DeFault
  }
  //==============================================================================
  //------------------------------------------------------------------------------


/*
░█─── ░█▀▀█ ░█▀▀▄ 
░█─── ░█─── ░█─░█ 
░█▄▄█ ░█▄▄█ ░█▄▄▀
*/
// Procesos de LCD---------------------------------------------------------------
  /*
  █▄▄ █▀█ ▀█▀ █▀█ █▄░█ █▀▀ █▀
  █▄█ █▄█ ░█░ █▄█ █░▀█ ██▄ ▄█
  */
//BOTONES========================================================================
  uint8_t flancoSubida(int btn) { //Funcion que se fija si se presiono un boton de la matriz de botones ya definida
  uint8_t valor_nuevo = digitalRead(button[btn]); //Define como el valor actual del boton el que se lee
  uint8_t result = button_estate[btn]!=valor_nuevo && valor_nuevo == 1; //Guarda si el estado del boton es diferente al valor nuevo y el valor nuevo es 1:
  button_estate[btn] = valor_nuevo; //El estado del boton cambia al estado nuevo
  return result; //Regresa si se presiono o no el boton
  }
  //==============================================================================

  
/*
█▀▄▀█ █▀▀ █▄░█ █▀ ▄▀█ ░░█ █▀▀ █▀
█░▀░█ ██▄ █░▀█ ▄█ █▀█ █▄█ ██▄ ▄█
*/
//MENSAJES DE LAS PANTALLAS||||||||||||||||||||||||||||||||||||||||||||||||||||||

  //MENSAJE DE TEMPERATURA========================================================
  void MSGTEMP(float tprom){ //Es la funcion que define el mensaje que se va a imprimir en la pantalla de temperatura
    static long ultimo_cambio = 0; //Variable para aplicar el delay
    static int Estado = 0; //Variable para hacer que la LCD cambie de estado de imprimir a borrar para crear el efecto de parpadeo
    long hora = millis(); //Es la variable en la que se va a almacenar el tiempo para sustituir al delay
    if (hora - ultimo_cambio > 100){ //Esta funcion sustituye al delay de 0.1 segundos
      ultimo_cambio = hora; //Cambia para reiniciar el delay
      if (Estado == 1){ //Si se pone en estado imprimir:
        lcd.setCursor(0, 0);  //Se fija el cursor en cierta casilla
        lcd.print("TEMP=:");  //Se imprime el mensaje en la LCD
        lcd.setCursor(6, 0);  //Se fija el cursor en cierta casilla
        lcd.print(tprom);  //Se imprime la temperatura en la LCD
        lcd.setCursor(11, 0);  //Se fija el cursor en cierta casilla
        lcd.write(byte(1));  //Se imprime el caracter customizado que era el grado (°)
        lcd.setCursor(12, 0);  //Se fija el cursor en cierta casilla
        lcd.print("C");  //Se imprime el mensaje en la LCD
      }
      else {  //SI NO SE ESTA EJECUTANDO LA IMPRESION
        Estado = 1; //El estado pasa a 1 para reiniciar el bucle
      }
    }
  }
  //==============================================================================

  //MENSAJE DE TIEMPO=============================================================
  void MSGTIEMP(unsigned int segtot){  //Es la funcion que va a definir el mensaje que se imprimira en la pantalla de tiempo
    static long ultimo_cambio = 0; //Variable para aplicar el delay
    long hora = millis(); //Es la variable en la que se va a almacenar el tiempo para sustituir al delay
    if (hora - ultimo_cambio > 1000){ //Esta funcion sustituye al delay de 1 segundo
	    ultimo_cambio = hora; //Cambia para reiniciar el delay
      thoras = ((segtot/60)/60); //Divide el tiempo que queda para ponerlo en formato de horas
      tminutos = (segtot/60)%60; //Divide el tiempo que queda para ponerlo en formato de minutos
      tsegundos = segtot%60; //Pone el tiempo que queda en formato de segundos

      lcd.setCursor(0, 0);  //Se fija el cursor en cierta casilla
      lcd.print("TIEMPO RESTANTE:");  //Se imprime el mensaje en la LCD
      lcd.setCursor(0, 1);  //Se fija el cursor en cierta casilla
      if(thoras < 10) lcd.print("0");  //Si las horas son menores que 10 se imprime un 0 a la izquierda
      lcd.print(thoras); //Se imprime el mensaje en la LCD
      lcd.print(":"); //Se imprime el mensaje en la LCD
      if(tminutos < 10) lcd.print("0");  //Si los minutos son menores que 10 se imprime un 0 a la izquierda
      lcd.print(tminutos); //Se imprime el mensaje en la LCD
      lcd.print(":"); //Se imprime el mensaje en la LCD
      if(tsegundos < 10) lcd.print("0"); //Si los segundos son menores que 10 se imprime un 0 a la izquierda
      lcd.print(tsegundos); //Se imprime el mensaje en la LCD
    } 
  }
  //==============================================================================


/*
█▀█ ▄▀█ █▄░█ ▀█▀ ▄▀█ █░░ █░░ ▄▀█ █▀
█▀▀ █▀█ █░▀█ ░█░ █▀█ █▄▄ █▄▄ █▀█ ▄█
*/
// PANTALLAS||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


  /*
  █▀█ ▄▀█ █▄░█ ▀█▀ ▄▀█ █░░ █░░ ▄▀█   █░█ █░█ █▀▀ █░█ █▀█
  █▀▀ █▀█ █░▀█ ░█░ █▀█ █▄▄ █▄▄ █▀█   █▀█ █▄█ ██▄ ▀▄▀ █▄█
  */
  //Pantalla DeFault==============================================================
  void PANTALLA_HUEVO(){
    lcd.clear();
    lcd.print("Girando huevo");
    lcd.setCursor(14, 0);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(3));
  }
  //==============================================================================


  /*
  █▀█ ▄▀█ █▄░█ ▀█▀ ▄▀█ █░░ █░░ ▄▀█   █▀▄ █▀▀ █▀▀ ▄▀█ █░█ █░░ ▀█▀
  █▀▀ █▀█ █░▀█ ░█░ █▀█ █▄▄ █▄▄ █▀█   █▄▀ ██▄ █▀░ █▀█ █▄█ █▄▄ ░█░
  */
  //Pantalla DeFault==============================================================
  void SCREEN_DeFault(){ //Funcion de imprimir la pantalla DeFault
    static long ultimo_cambio = 0; //Variable para aplicar el delay
    static int Estado = 0; //Variable para hacer que la LCD cambie de estado de imprimir a borrar para crear el efecto de parpadeo
    long hora = millis(); //Es la variable en la que se va a almacenar el tiempo para sustituir al delay
    int estado = digitalRead(19); //Lee para ver si se presiona el boton de inicio o no
    if (estado == 1){ //Si se presiona el boton de inicio:
      contador = contador + 1; //Se Sube el contador y se acaba la funcion de imprimir la pantalla DeFault
    }
    Serial.println(contador); //Es para ver en el monor serial que no se esta presionando el boton
    if (contador == 0){ //Si no se presiona el boton de iniciar:
      if (hora - ultimo_cambio > 1700){ //Esta funcion sustituye al delay de 1.7segundos
	      ultimo_cambio = hora; //Cambia para reiniciar el delay
        if (Estado == 1){ //Si se pone en estado imprimir:
          lcd.clear(); //Se limpia la lcd
          lcd.setCursor(5, 0); //Se fija el cursor en cierta casilla
          lcd.print("PULSE 1"); //Se imprime el mensaje en la LCD
          lcd.setCursor(2, 1); //Se fija el cursor en cierta casilla
          lcd.print("PARA INICIAR"); //Se imprime el mensaje en la LCD
         Estado = 0; //Se cambia el estado para que ahora se limpie la LCD para dar el efecto de parpadeo
        } 
        else { //Si no esta en el estado de imprimir, por lo tanto esta en el estado de limpiar y por lo tanto:
          lcd.setCursor(0, 0); //Se fija el cursor en cierta casilla
          lcd.print("                "); //Se imprimen espacios vacios porque asi parece que no hay nada
          lcd.setCursor(0, 1); //Se fija el cursor en cierta casilla
          lcd.print("                "); //Se imprimen espacios vacios porque asi parece que no hay nada
        Estado = 1; //Se cambia el estado para que ahora imprima el mensaje en la LCD para dar el efecto de parpadeo
        }
      }
    }
  }
  //==============================================================================


  /*
  █▀█ ▄▀█ █▄░█ ▀█▀ ▄▀█ █░░ █░░ ▄▀█   █▀▄▀█ █▀▀ █▄░█ █░█
  █▀▀ █▀█ █░▀█ ░█░ █▀█ █▄▄ █▄▄ █▀█   █░▀░█ ██▄ █░▀█ █▄█
  */
  //Pantalla MENU=================================================================
  void SCREEN_MENU(){ //Es la funcion que va a imprimir la pantalla del menu
    lcd.setCursor(2, 0); //Se fija el cursor en cierta casilla
    lcd.print("INCUBANDO");  //Se imprime el mensaje en la LCD
  }
  //==============================================================================


  //PUNTOS SUSPENSIVOS============================================================
  void SCREEN_MENU_POINTS(){ //Es la funcion que va a imprimir los puntos suspensivos de la pantalla de inicio
    static long ultimo_cambio = 0; //Variable para aplicar el delay
    static int Estado = 0; //Variable para hacer que la LCD cambie de estado de imprimir a borrar para crear el efecto de parpadeo
    long hora = millis(); //Es la variable en la que se va a almacenar el tiempo para sustituir al delay
    if (hora - ultimo_cambio > 1500){  //Esta funcion sustituye al delay de 1.7segundos
	    ultimo_cambio = hora;  //Cambia para reiniciar el delay
      if (Estado == 1){ //Si esta en el estado de imprimir el primer punto:
        lcd.setCursor(11, 0); //Se fija el cursor en cierta casilla
        lcd.write(byte(0)); //Se imprime el caracter customizado que era el punto
        Estado = Estado + 1; //Sube de estado para pasar al siguiente
      }
      else if (Estado == 2){ //Si esta en el estado de imprimir el segundo punto:
        lcd.setCursor(12, 0);  //Se fija el cursor en cierta casilla
        lcd.write(byte(0));  //Se imprime el caracter customizado que era el punto
        Estado = Estado + 1;  //Sube de estado para pasar al siguiente
      }
      else if (Estado == 3){  //Si esta en el estado de imprimir el tercer punto:
        lcd.setCursor(13, 0);  //Se fija el cursor en cierta casilla
        lcd.write(byte(0));  //Se imprime el caracter customizado que era el punto
        Estado = Estado + 1;  //Sube de estado para pasar al siguiente
      }
      else if (Estado == 4){ //Si esta en el estado de poner todo en blanco:
        lcd.setCursor(11, 0); //Se fija el cursor en cierta casilla
        lcd.print("                "); //Se imprimen espacios vacios porque asi parece que no hay nada
        Estado = 1; //El estado pasa a 1 para reiniciar el bucle
      }
      else {  //Por si algo falla y no se reinicia:
        Estado = 1; //El estado pasa a 1 para reiniciar el bucle
      }
    }
  }
  //==============================================================================


  /*
  █▀█ ▄▀█ █▄░█ ▀█▀ ▄▀█ █░░ █░░ ▄▀█   ▀█▀ █▀▀ █▀▄▀█ █▀█ █▀▀ █▀█ ▄▀█ ▀█▀ █░█ █▀█ ▄▀█
  █▀▀ █▀█ █░▀█ ░█░ █▀█ █▄▄ █▄▄ █▀█   ░█░ ██▄ █░▀░█ █▀▀ ██▄ █▀▄ █▀█ ░█░ █▄█ █▀▄ █▀█
  */
  //PANTALLA TEMPERATURA==========================================================
  void SCREEN_TEMPERATURA(){ //Es la funcion que va a imprimir la pantalla de TEMPERATURA
    static long change = 0; //Variable para aplicar el delay
    long time = millis();  //Es la variable en la que se va a almacenar el tiempo para sustituir al delay
    if (time - change > 700){  //Esta funcion sustituye al delay de 2 segundos
      change = time; //Cambia para reiniciar el delay
      float tempPrint = MEDTEMP(); //Saca en una variable la temperatura
      MSGTEMP(tempPrint); //Imprime la temperatura
    }
  }


  /*
  █▀█ ▄▀█ █▄░█ ▀█▀ ▄▀█ █░░ █░░ ▄▀█   ▀█▀ █ █▀▀ █▀▄▀█ █▀█ █▀█
  █▀▀ █▀█ █░▀█ ░█░ █▀█ █▄▄ █▄▄ █▀█   ░█░ █ ██▄ █░▀░█ █▀▀ █▄█
  */
  //PANTALLA TIEMPO===============================================================
  void SCREEN_TIEMPO(unsigned int tiempo_que_queda){ //Es la funcion que va a imprimir la pantalla de TIEMPO
    unsigned int tiempo = tiempo_que_queda; //Se saca en una variable el tiempo que queda
    MSGTIEMP(tiempo); //Se imprime el tiempo que queda
  }   
  //==============================================================================


  /*
  █▀█ ▄▀█ █▄░█ ▀█▀ ▄▀█ █░░ █░░ ▄▀█   █▀█ █▀▀ █ █▄░█ █▀▀ █ █▀█
  █▀▀ █▀█ █░▀█ ░█░ █▀█ █▄▄ █▄▄ █▀█   █▀▄ ██▄ █ █░▀█ █▄▄ █ █▄█
  */
  //PANTALLA REINICIO============================================================
  void SCREEN_REINICIO(){  //Es la funcion que va a imprimir la pantalla de reinicio
      lcd.setCursor(3, 0);  //Se fija el cursor en cierta casilla
      lcd.print("REINICIAR ?");  //Se imprime el mensaje en la LCD
      lcd.setCursor(0, 1);  //Se fija el cursor en cierta casilla
      lcd.print("SI(4x3) NO(ELSE)");  //Se imprime el mensaje en la LCD
  }
  //==============================================================================

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>