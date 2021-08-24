#include <String.h>

float lm35 = A0; //temperature sensor at pin A0
float realtemp;
const int pir = 4;
const int fan = 11;

const int th1 = 32;
const int th2 = 40;

int motion=0;

String mobile = "+91**********";
String msg;
String condition;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String incomingString ="";
int startIndex = 0;
int endIndex = 0;

    
void setup()  
{
  pinMode(13, OUTPUT);
  pinMode(pir, INPUT);
  pinMode(fan, OUTPUT);
  
   Serial.begin(38400); //gsm baud rate
    delay(1000);
 
/////////////////GSM INIT//////////////////
 // reserve 200 bytes for the inputString:
      inputString.reserve(200);
      //--Start: Send SMS --
      Serial.print("AT+CMGF=1\r");    
      delay(1000);

      //Serial.print("AT+CMGD=1,4\r");  // Deletes all SMS saved in SIM memory
      Serial.print("AT+CMGDA=\""); 
      Serial.println("DEL ALL\"");
        delay(1000);
      Serial.println("AT+CMGS=\"" + mobile + "\"" + "\r");
        delay(1000);
      Serial.print("Baby Monitoring System is Turned On");   //The text of the message to be sent
      delay(1000);
      Serial.write(0x1A);    // command to send the msg
      delay(1000);
      Serial.print("AT+CNMI=2,2,0,0,0\r"); 
      delay(1000);
     //--End: SMS--
}


void loop()                     // run over and over again
{
digitalWrite(13, LOW);

//Serial.println(vibration);


  realtemp = analogRead(lm35);
  delay(10);
  realtemp = realtemp * 0.48828125; //converting volts to degrees celsius ----- 0.48828125 = [(5V*1000)/1024]10
  delay(10);

  motion = digitalRead(pir);
  digitalWrite(13, motion);
  delay(10);



String temp = String (realtemp);
  String movement = String (motion);
  condition = " Temp= " + temp + ", " + "Movement= " + movement;



 if (realtemp >= th1  && motion == 1 )
  {
  
    sendalertsms();
    delay(3000);
    sendconditionsms();
    delay(3000);
  
  }  

if (stringComplete && inputString!="") 
      {

        inputString.toLowerCase();


        if(inputString=="@on#")

        {
          sendfanonsms();
          digitalWrite(fan, HIGH);// Turn On fan
          delay(5000); 

        }

        if(inputString=="@data#")

        {

          sendconditionsms();
          delay(3000);

        }

       Serial.print("AT+CMGDA=\""); 
       Serial.println("DEL ALL\""); // To Delete Messages from SIM Memory
       delay(1000);
       inputString = "";
       stringComplete = false;
      }

if (realtemp >= th2)
  {
  digitalWrite(fan, HIGH);
  }  
delay(10);

if (realtemp <= th1)
    {
    digitalWrite(fan, LOW); 
    }
}


void sendconditionsms()
{
  
  digitalWrite(13, HIGH);
  Serial.println("AT+CMGF=1");
  delay(100);
  Serial.println("AT+CMGS=\"" + mobile + "\"" + "\r");
  delay(100);
  Serial.println(condition);
  delay(500);
  Serial.println((char)26); 
  
  }

  void sendalertsms()
{
  
  digitalWrite(13, HIGH);
  Serial.println("AT+CMGF=1");
  delay(100);
  Serial.println("AT+CMGS=\"" + mobile + "\"" + "\r");
    delay(100);
  Serial.println("Temperature is rising in car. Any action?");
  delay(500);
  Serial.println((char)26); 
  
  }


void sendfanonsms()
{
  
  digitalWrite(13, HIGH);
  Serial.println("AT+CMGF=1");
  delay(100);
  Serial.println("AT+CMGS=\"" + mobile + "\"" + "\r");
    delay(100);
  Serial.println("Fan Turned ON");
  delay(500);
  Serial.println((char)26); 
  
  }

void serialEvent() 
    {
       if(stringComplete == false)
       { 
            incomingString = Serial.readString();
            if(incomingString!="")
            {
              startIndex = incomingString.indexOf("@");
              endIndex = incomingString.indexOf("#");
              if(startIndex>0 && endIndex>0)
              {
                 inputString = incomingString.substring(startIndex,endIndex+1);
                 stringComplete = true;
              }
       }
    
      }
    }  
