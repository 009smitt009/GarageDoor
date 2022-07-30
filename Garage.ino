#define BTT_STOP   A6
#define BTT_OPEN  5
#define BTT_CLOSE 4
#define LED_DOOR 6
#define L_DOOR_PWM 11
#define L_DIR_DOOR 9
#define L_GERCON_DOOR 8
#define R_DOOR_PWM 10
#define R_DIR_DOOR 12
#define R_GERCON_DOOR 7
#define CLK 2
#define DIO 3
int state   = 1;
unsigned long last_tame_led; 
//unsigned long last_tame_door_open;
unsigned long last_tame_door_close;  
boolean flag_led = true;
// файл лежит в одной папке со скетчем
#include "GyverTM1637.h"
GyverTM1637 disp(CLK, DIO);

//bool flagGercL = false;
//uint32_t gercTimerL = 0;
//
//bool flagGercR = false;
//uint32_t gercTimerR = 0;

//-------Конфигурация------------//
int timeDelayDoorClose = 1000*10;
//-------------------------------//

#include "button.h"
button btnGerL(L_GERCON_DOOR); // указываем пин
button btnGerR(R_GERCON_DOOR);
button btnOpen(BTT_OPEN);
button btnCls(BTT_CLOSE);


void setup() {
  Serial.begin(9600); 
  pinMode(BTT_STOP, INPUT);
  pinMode(BTT_OPEN, INPUT);
  pinMode(BTT_CLOSE, INPUT);
  pinMode(LED_DOOR,OUTPUT);
  pinMode(L_DOOR_PWM,OUTPUT);
  pinMode(L_DIR_DOOR,OUTPUT);
  pinMode(L_GERCON_DOOR,INPUT_PULLUP);
  pinMode(R_DOOR_PWM,OUTPUT);
  pinMode(R_DIR_DOOR,OUTPUT);
  pinMode(R_GERCON_DOOR,INPUT_PULLUP);
  pinMode(CLK,OUTPUT);
  pinMode(DIO,OUTPUT);
  
  disp.clear();
  disp.brightness(7);  // яркость, 0 - 7 (минимум - максимум)

  if (analogRead(BTT_STOP) > 800){state = 2;}
  

//  if ( digitalRead(BTT_OPEN == LOW) ){
//    scrolls();
//  }
  //процедура изменения времени

}

void loop() {
  // put your main code here, to run repeatedly:

switch (state) {
    case 1:
      //выполняется, когда опрос кнопок
      if(btnOpen.click(false)){ //false-pullDW/ true-pullUP
        openDoor();
      }

      if( btnCls.click(false)){
        closeDoor();
       }
      
//      if (digitalRead(BTT_OPEN) == HIGH){openDoor();}
//      if (digitalRead(BTT_CLOSE) == HIGH){closeDoor();}

      break;
    case 2:
      //выполняется когда  var равно 2
      disp.clear();
      disp.displayByte(_S, _E, _t, _empty);
      delay(3000);
      disp.clear();
      state = 1;
      break;
//    default:
      // выполняется, если не выбрана ни одна альтернатива
      // default необязателен
  }



}

//------------------Процедуры----------------------//
void openDoor(){
  digitalWrite(R_DIR_DOOR,HIGH);
  digitalWrite(L_DIR_DOOR,HIGH);
  disp.displayByte(_O, _P, _E, _N);
  delay(1000);
  checkGercon(0);
}


void closeDoor(){
  digitalWrite(R_DIR_DOOR,LOW);
  digitalWrite(L_DIR_DOOR,LOW);
  disp.displayByte(_C, _L, _O, _S);
  delay(1000);
  checkGercon(timeDelayDoorClose);
  
}

boolean checkStorButton(){
  if (analogRead(BTT_STOP) > 800){
          digitalWrite(R_DOOR_PWM,LOW);
          digitalWrite(L_DOOR_PWM,LOW);
          digitalWrite(R_DIR_DOOR,LOW);
          digitalWrite(L_DIR_DOOR,LOW);
          disp.clear();
          disp.displayByte(_S, _t, _O,_P);
          delay(1000);
          disp.clear();
          return true;
  }else return false;
}

boolean checkGercon(int DoorDelay){
  boolean  doorL  = false;
  boolean  doorR  = false;
  boolean flagR = false;
  boolean flagL = false;
  
//  for (int i=0; i<=255; i++){
//    analogWrite(R_DOOR_PWM,i);
//    analogWrite(L_DOOR_PWM,i);
////    if ((digitalRead(R_GERCON_DOOR) == LOW)||(digitalRead(L_GERCON_DOOR) == LOW)){ // проверка на препятствие плавного пуска
////          digitalWrite(R_DOOR_PWM,LOW);
////          digitalWrite(L_DOOR_PWM,LOW);
////          digitalWrite(R_DIR_DOOR,LOW);
////          digitalWrite(L_DIR_DOOR,LOW);
////          disp.clear();
////          disp.displayByte(_E, _r, _o,_r);
////          delay(1000);
////          disp.clear();
////          ledSwhich(false);
////          return;
////      }
//          ledSwhich(true);
//         if(checkStorButton()){ledSwhich(false); return;}
//     
//    delay(10);

    
    
//  }
    
//    delay(1000);
// analogWrite(R_DOOR_PWM,pwmDoorDelay);
 last_tame_door_close = millis(); 
 while ( !doorR || !doorL ) {

  if(millis()-last_tame_door_close > DoorDelay){
//          last_tame_door_close = millis();
          if (doorL == false && flagL == false ){ analogWrite(L_DOOR_PWM,255);  flagL = true; }


    if (btnGerL.hold(true)){
//        if (digitalRead(L_GERCON_DOOR) == LOW){
          digitalWrite(L_DOOR_PWM,LOW);
          doorL = true;
        }
     }

     
      if (doorR == false && flagR == false ){ analogWrite(R_DOOR_PWM,255);  flagR = true; }
      
      if (btnGerR.hold(true)){
//        if (digitalRead(R_GERCON_DOOR) == LOW){
          digitalWrite(R_DOOR_PWM,LOW);
          doorR = true;
      }  
      
       
      ledSwhich(true); 
      if(checkStorButton()){ledSwhich(false); return;}
  }
          disp.clear();
          disp.displayByte(_Y, _E, _S,_empty);
          ledSwhich(false);
          delay(1000);
          disp.clear();
}


void ledSwhich(boolean state){
  if(state){
    if(millis()-last_tame_led > 700){
          last_tame_led = millis();
          flag_led = !flag_led;
          digitalWrite(13,flag_led);
          digitalWrite(LED_DOOR,flag_led);
        }
  }else {digitalWrite(13,LOW);digitalWrite(LED_DOOR,LOW);}
  
}

//void scrolls() {
  // прокрутка массив ЦИФР
//  byte digs[4] = {3, 5, 7, 1};
//  disp.scroll(digs, 100);     // скорость прокрутки 100
//  delay(1000);

  // прокрутка прицельно (ячейка, ЦИФРА, скорость)
//  disp.scroll(0, 8, 200);
//  delay(1000);
//
//  disp.clear();
//  delay(1000);
//  for (byte i = 0; i < 10; i++) {
//    disp.scroll(3, i, 50);
//    delay(400);
//  }

  // прокрутка массива БАЙТ
//  byte troll[4] = {_t, _r, _o, _l};
//  disp.scrollByte(troll, 100);
//  delay(1000);

//  // прицельная прокрутка БАЙТА (ячейка, байт, скорость)
//  disp.scrollByte(2, _G, 50);
//  delay(1000);
//}
