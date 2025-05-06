/*
  Make sure your Firebase project's '.read' and '.write' rules are set to 'true'. 
  Ignoring this will prevent the MCU from communicating with the database. 
  For more details- https://github.com/Rupakpoddar/ESP32Firebase 
*/

#include <ESP32Firebase.h>
#include <ESP32Servo.h> 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>


#define _SSID "C306"          // Your WiFi SSID
#define _PASSWORD "Spkt2022"      // Your WiFi Password
#define REFERENCE_URL "https://esp32-rfid-8db3a-default-rtdb.firebaseio.com/"  // Your Firebase project reference url

Firebase firebase(REFERENCE_URL);

LiquidCrystal_I2C lcd(0x27, 20, 4); 

//Servo
 Servo myServo;
 int servoPin = 2;

 //RFID
int UID[4], i,dem,mode;
#define SS_PIN 5
#define RST_PIN 27
MFRC522 mfrc522(SS_PIN, RST_PIN);
int ID1[4] = {35, 225, 115, 16};
int ID2[4] = {03, 255, 41, 246};
int ID3[4] = {135, 207, 229, 96};
     
// int  CB = 0;  
//Keypad
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char MatrixKey[ROWS][COLS] = 
{
   {'1', '2', '3', 'A'},
   {'4', '5', '6', 'B'},
   {'7', '8', '9', 'C'},
   {' * ', '0', '#', 'D'}
};
   byte rowPins[ROWS] = {32, 33, 25, 26}; 
  byte colPins[COLS] = {14, 12, 27, 2};
   Keypad Mykeys = Keypad (makeKeymap (MatrixKey), rowPins, colPins, ROWS, COLS);
   long Received_Number = 0;
   int digit1 = 0, digit2 = 0, digit3 = 0, digit4=0;
   int otp  =9999; //mk Keypad

void setup() {
   myServo.attach (16); //set chân
    Serial.begin (115200);
    Serial.println ("Dua the vao...");
    Serial.println ();
    lcd.init ();
    lcd.setCursor(0,0);
    lcd.backlight ();
    lcd.clear ();
    SPI.begin (); // Initiate SPI bus
    mfrc522.PCD_Init ();
    myServo.write (0);
  Serial.begin(115200);
  // pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  // digitalWrite(LED_BUILTIN, HIGH);

//================================================================//
//================================================================//

  // Examples of setting String, integer and float values.
  // firebase.setString("Example/setString", "It's Working");
  // firebase.setInt("Example/setInt", 123);
  // firebase.setFloat("Example/setFloat", 45.32);

  // // Examples of pushing String, integer and float values.
  // firebase.pushString("push", "Hello");
  // firebase.pushInt("push", 789);
  // firebase.pushFloat("push", 89.54);

  // // Example of getting a String.
  // String data1 = firebase.getString("Example/setString");
  // Serial.print("Received String:\t");
  // Serial.println(data1);

  // // Example of getting an int.
  // int data2 = firebase.getInt("Example/setInt");
  // Serial.print("Received Int:\t\t");
  // Serial.println(data2);

  // // Example of getting a float.
  // float data3 = firebase.getFloat("Example/setFloat");
  // Serial.print("Received Float:\t\t");
  // Serial.println(data3);

  // // Example of data deletion.
  // firebase.deleteData("Example");
}

void loop() {
  lcd.setCursor (2, 0);
      lcd.print ("MOI QUET THE ");
      char key = Mykeys.getKey ();
      if (key)
      {
         Serial.println (key);
         if (Received_Number > 9999)
         {
            lcd.setCursor (0, 0);
            lcd.print ("*** SAI THE ***");
            lcd.setCursor (0, 1);
            lcd.print ("VUI long THU LAI ");
            //wrong ();
            lcd.clear ();
            delay (5000);
            return;
            i=0;
         }
         i++;
         digit4 = digit3;
         digit3 = digit2;
         digit2 = digit1;
         digit1 = String (key) .toInt () ;
         Received_Number = digit4 * 1000 + digit3 * 100 + digit2 * 10 + digit1;
         Serial.println (Received_Number);
         if (i == 1)
         {
            lcd.setCursor (6, 1);
            lcd.print (key);
            delay (500); // Ký t? hi?n th? trên màn hình LCD trong 0.5s
            lcd.setCursor (6, 1);
            lcd.print ("*"); // Ký t? du?c thay b?i d?u *
         }
         if (i == 2)
         {
            lcd.setCursor (7, 1);
            lcd.print (key);
            delay (500);
            lcd.setCursor (7, 1);
            lcd.print ("*");
         }
         if (i == 3)
         {
            
            lcd.setCursor (8, 1);
            lcd.print (key);
            delay (500);
            lcd.setCursor (8, 1);
            lcd.print ("*");
         }
         if (i == 4)
         {
            lcd.setCursor (9, 1);
            lcd.print (key);
            delay (500);
            lcd.setCursor (9, 1);
            lcd.print ("*");
         }
      }
      if (i == 4)
      {
         
         if (otp == Received_Number)
         {
            Serial.println ("DUNG THE");
            Serial.println ();
            lcd.setCursor (0, 0);
            lcd.print ("*** MO CUA ***");
            lcd.setCursor (0, 1);
            lcd.print ("*** XIN CAM ON ***");
            //right ();
            i = 0;
            myServo.write (90);
           
            delay (5000); // Th?i gian m? c?a
            myServo.write (0);
            lcd.clear ();
             firebase.setString("RFID/ten", "");
            firebase.setString("RFID/UID", "");
            firebase.setString("RFID/nhap mk", "ĐÚNG-MẬT-KHẨU");
            firebase.setString("RFID/quetthe", "");
            return;
         }
         else
         {
            
            lcd.setCursor (0, 0);
            lcd.print ("*** SAI MAT KHAU ***");
            lcd.setCursor (0, 1);
            lcd.print ("VUI long THU LAI ");
            
            //wrong ();
            delay (5000);
            lcd.clear ();
            firebase.setString("RFID/ten", "");
            firebase.setString("RFID/UID", "");
            firebase.setString("RFID/nhap mk", "SAI-MẬT-KHẨU");
            firebase.setString("RFID/quetthe", "");
         }
         i = 0;
      }

      if ( ! mfrc522.PICC_IsNewCardPresent ())
      {
         return;
      }
      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial ())
      {
         return;
      }
      //Show UID on serial monitor
      Serial.print ("UID cua the: ");
      
      for (byte i = 0; i < mfrc522.uid.size; i++)
      {
         Serial.print (mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
         UID[i] = mfrc522.uid.uidByte[i];
         Serial.print (UID[i]);
      }
      Serial.println ();
      Serial.print ("Message : ");
      if (UID[i] == ID1[i]||UID[i] == ID2[i]||UID[i] == ID3[i]) // dán ID c?a th? v?a d?c du?c t? tru?c
      {  
        if (UID[i] == ID1[i])
      {
            dem = 0;
         Serial.println ("DUNG THE");
         Serial.println ();
         lcd.setCursor (0, 0);
         lcd.print ("*** XIN CHAO ***");
         lcd.setCursor (0, 1);
         lcd.print ("*** ANH KHOA ***");
         //right ();
         
         
         
         myServo.write (90);
         delay (5000); // Th?i gian m? c?a
         myServo.write (0);
         lcd.clear ();
      }
        if (UID[i] == ID2[i])
      {
            dem = 0;
         Serial.println ("DUNG THE");
         Serial.println ();
         lcd.setCursor (0, 0);
         lcd.print ("*** XIN CHAO ***");
         lcd.setCursor (0, 1);
         lcd.print ("*** KHANH HUNG ***");
         //right ();
         
         
         
         myServo.write (90);
         delay (5000); // Th?i gian m? c?a
         myServo.write (0);
         lcd.clear ();
      }
      if (UID[i] == ID3[i])
      {
            dem = 0;
         Serial.println ("DUNG THE");
         Serial.println ();
         lcd.setCursor (0, 0);
         lcd.print ("*** XIN CHAO ***");
         lcd.setCursor (0, 1);
         lcd.print ("** THE HOANG **");
         //right ();       
         myServo.write (90);
         delay (5000); // Th?i gian m? c?a
         myServo.write (0);
         lcd.clear ();
      }
      }
          
      else
      {
         dem ++;
         Serial.print ("BIEN DEM: ");
         Serial.println (dem);
         Serial.println ("SAI THE");
         Serial.println ();
         if (dem == 3)
         {
            // t?t dèn led
            
            
            delay (1000);
            mode = 1; delay (1000); mode = 0; dem = 0;
         }
         lcd.setCursor (0, 0);
         lcd.print ("*** SAI THE ***");
         lcd.setCursor (0, 1);
         lcd.print ("VUI long THU LAI ");
         //wrong ();
         delay (5000);
         lcd.clear ();
      }

      
         
    
      if (UID[i] == ID1[i]){
          firebase.setString("RFID/ten", "ANH-KHOA");
          firebase.setString("RFID/UID", "35-225-115-16");
          firebase.setString("RFID/nhap mk", "");
          firebase.setString("RFID/quetthe", "");
          }
      else if(UID[i] == ID2[i])
          { firebase.setString("RFID/ten", "KHANH-HUNG");
            firebase.setString("RFID/nhap mk", "");
            firebase.setString("RFID/UID", "03-255-41-246");
            firebase.setString("RFID/quetthe", "");
          }
      else if(UID[i] == ID3[i])
          { firebase.setString("RFID/ten", "THẾ-HOÀNG");
            firebase.setString("RFID/nhap mk", "");
            firebase.setString("RFID/UID", "135-207-229-96");
            firebase.setString("RFID/quetthe", "");
          }   
       else  { firebase.setString("RFID/ten", "");
            firebase.setString("RFID/nhap mk", "");
            firebase.setString("RFID/UID", "");
            firebase.setString("RFID/quetthe", "SAI-THẺ");
          }    


      // firebase.pushString("push", "Hello");
      // firebase.pushInt("push", 789);
      // firebase.pushFloat("push", 89.54);

      // Examples of setting String, integer and float values.
  // firebase.setString("RFID/ten", "Khoa ngu");
  // firebase.setInt("RFID/UID", 52 220 128 255);
  //firebase.setFloat("Example/setFloat", 45.32);
}
