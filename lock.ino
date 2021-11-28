#include <Wire.h>              //Библиотека для управления устройствами по I2C 
#include <Keypad.h>            //Библиотека для работы с клавиатуры
#include <LiquidCrystal_I2C.h> //Библиотека для работы с дисплеем LCD1602 по I2C интерфейсу
#include <Servo.h>             //Библиотека для работы с сервоприводом

Servo servo1;        //Объявляем переменную servo

const byte ROWS = 4; //Число строк клавиатуры
const byte COLS = 3; //Число столбцов клавиатуры

//Названия клавиш
char hexaKeys[ROWS][COLS] = {
      {'1','2','3'},
      {'4','5','6'},
      {'7','8','9'},
      {'*','0','#'}
};

byte rowPins[ROWS] = {8, 7, 6, 5}; //К каким выводам подключаем управление строками
byte colPins[COLS] = {4, 3, 2};    //К каким выводам подключаем управление столбцами

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //Иниициализация клавиатуры

int k = 0;                          //Счетчик нажатий
char kod[4] = {'5', '1', '9', '6'}; //Верный код
char buttons[5];                    //Массив нажатых кнопок

LiquidCrystal_I2C lcd(0x27,16,2);   //Указываем I2C адрес

void setup() {
  servo1.attach(10);     //Привязываем сервопривод к аналоговому выходу 10
  Serial.begin(9600);    //Открываем порт для отладки
  pinMode(11, OUTPUT);   //Вывод данных на контакт 11
  pinMode(12, OUTPUT);   //Вывод данных на контакт 12
  pinMode(13, OUTPUT);   //Вывод данных на контакт 13
  digitalWrite(13, LOW); //Подаём нулевой сигнал на вывод 13
  digitalWrite(12, LOW); //Подаём нулевой сигнал на вывод 12
  digitalWrite(11, LOW); //Подаём нулевой сигнал на вывод 11

  lcd.init();            //Инициализация дисплея  
  lcd.backlight();       //Подключение подсветки

}
int a =0,b=0,c=50,p=0,sh=0,g=0, ps=0;
void loop() {

char customKey = customKeypad.getKey();

//Звуковое сопровождение-----------
if(b==c){a=0;b=0;p=0;}
if(a==1){ b++; Serial.println(a);
if(b<c){ 
digitalWrite(11, LOW);
delay(1);
digitalWrite(11, HIGH);}}
//---------------------------------

//Звуковое сопровождение-неверный код-
if(b==c){a=0;b=0;p=0;}
if(a==5){ b++; Serial.println(a);
if(b<c){ 
digitalWrite(11, LOW);
delayMicroseconds(4000);
digitalWrite(11, HIGH);}}
//---------------------------------

if(g==0 || g==12 ){     
  if(a==0){ 
    if(g==0){
      lcd.clear();         //Очистить дисплей
      lcd.setCursor(0,0);  //Установка курсора в начало первой строки
      lcd.print("CLOSED!");//Набор текста на первой строке
      lcd.setCursor(0,1);  //Установка курсора в начало второй строки 
    }

  digitalWrite(13, LOW);  //Подаём нулевой сигнал на вывод 13
  digitalWrite(12, HIGH); //Подаём еденичный сигнал на вывод 12 (загорается красный светодиод)

  delay(1000);            //Задержка на 1 секунду
  lcd.clear();            //Очистить дисплей
  lcd.setCursor(0,0);     //Установка курсора в начало первой строки
  lcd.print("ENTER KOD:");//Набор текста на первой строке
  lcd.setCursor(0,1);     //Установка курсора в начало второй строки 
  lcd.blink();            //Включение мигающего курсора (с частотой около 1 Гц)
  g++;                    //Автоинкремент. Чтобы это условие срабатывало один раз
 }                 
}

if(ps==0){ 
   //Подмена цифры звёздочкой 
   if(b>48 && sh > 0){
          delay(500);
          lcd.print('*');}

   if(customKey){ 
      c=50; 
      p=10;
      a=1;  
      buttons[k] = customKey; //Сохраняем значение кнопки в массиве
      k++;  
         
       //Отображаются все символы кроме "#" И "*"
       if(customKey != '#' && customKey != '*'){
          lcd.print(customKey); 
          lcd.setCursor(sh,1); 
          sh++; //Счётчик нажатий
          }     

     if(servo1.read()!=180){
       //Кнопка сброса [RESET]    
       if(customKey=='*'){
          k=0;
          sh = 0;
          lcd.clear();             //Очистить дисплей
          lcd.setCursor(0,0);      //Установка курсора в начало первой строки
          lcd.print("ENTER KOD:"); //Набор текста на первой строке
          lcd.setCursor(0,1);      //Установка курсора в начало второй строки
     } }

     //Если нажата кнопка-"#" [ENTER]
     if(customKey=='#'){
        if(buttons[0] == kod[0] && buttons[1] == kod[1] && buttons[2] == kod[2] && buttons[3] == kod[3]){
           ps=90;
           Serial.println("Код Верный!");//Отладка
           digitalWrite(13, HIGH);       //Подаём еденичный сигнал на вывод 13 (загорается зелёный светодиод)
           digitalWrite(12, LOW);        //Подаём нулевой сигнал на вывод 12
           
           //СЕРВОПРИВОД
           servo1.write(180); //Ставим угол поворота под 180
           delay(500);        //Ждем 500 миллисекунды 
              
           k=0;                  
           a=1;                  
           c=150;                
           p=10;                
           
           
           sh = 0;
           lcd.clear();        //Очистить дисплей
           lcd.setCursor(0,0); //Установка курсора в начало первой строки
           lcd.print("OPEN!"); //Набор текста на первой строке
           lcd.noBlink();      //Выключение мигающего курсора
        }else{
              Serial.println("Код НЕ верный!");//Отладка
              k=0;                             
              a=5;                             
              c=150;                          
              p=10;                           
           
        sh = 0;
        lcd.clear();                          //Очистить дисплей
        lcd.setCursor(0,0);                   //Установка курсора в начало первой строки
        lcd.print("The code is not");         //Набор текста на первой строке
        lcd.setCursor(0,1);                   //Установка курсора в начало второй строки
        lcd.print("Correct :(");              //Набор текста на второй строке
        g=12;                                 //Включаем текст Enter Kod 
              } 
      }
    }
}


}
