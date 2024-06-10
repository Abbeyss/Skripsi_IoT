#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <AccelStepper.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>

#define BOTtoken "5933575452:AAFD1I5gO2KP8f1QpvkTt_PfkWSjGKFMlZ8"
#define pinPakan   16 // Pin D0 = GPIO16
#define pinSHujan A0 // Pin Analog A0
#define ONE_WIRE_BUS 14 // Pin D5 = GPIO 14
#define motorPin1 5 // Pin D1
#define motorPin2 0 // Pin D3
#define motorPin3 4 // Pin D2
#define motorPin4 2 // Pin D4
#define Relay1   12 // Pin D6
#define Relay2   13 // Pin D7

#define idTele "1272187698"
#define MotorInterfaceType 8

//##########   MOTOR STEPPER   #####################
AccelStepper stepper = AccelStepper(MotorInterfaceType, motorPin1,motorPin2,motorPin3,motorPin4);
int SPenutup;

//##########  SERVO   ###############
Servo myservo;

//##########  SENSOR HUJAN   #############
int DataSHujan;
float DataSAir;

//##########  Relay   ############
int SPompa;
int SHeater;

//##############   SENSOR SUHU AIR   #################
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorDS18B20(&oneWire);

//##############  WIFI   ##########################
const char ssid[] = "LAPTOP-BJKN30G3 3843";  //AndroidAP3875   F_ONE
char password[] = "xAbbey06x";    //Takudon02    1dua3  empat

//################   Mode Alat   #######################
bool MAlat = 0; // 0 = manual, 1 = otomatis


//################   Perintah Tele   ######################### 
const char* Perintah_Pakan = "/BeriPakan";

const char* Perintah_Auto = "/ModeManual";
const char* Perintah_Auto1 = "/ModeOtomatis";

const char* Pompa1 = "/NyalakanPompa";
const char* Pompa = "/MatikanPompa";

const char* Heater1 = "/NyalakanHeater";
const char* Heater = "/MatikanHeater";

const char* Penutup = "/BukaPelindung";
const char* Penutup1 = "/TutupPelindung";

const char* STPompa = "/StatusPompa";
const char* STPenutup = "/StatusPenutup";
const char* STKolam = "/StatusKolam";
const char* SRelayOFF = "/MatikanSemuaRelay";

const char* Testt = "/BersihkanPakan";





WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int ON = LOW, OFF = HIGH;

int Bot_mtbs = 1500; 
long Bot_lasttime;   // Pesan Terakhir 

//int ledStatus1 = OFF;
int ledStatus2 = OFF;
int ledStatus3 = OFF;
int ledStatus4 = OFF;
int ledStatus5 = OFF;


void handleNewMessages(int numNewMessages) {
  //Serial.println("handleNewMessage");
  //Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String Pesan = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

// Mode Alat
    if (Pesan == (Perintah_Auto1)){
      Serial.println(Pesan);
      bot.sendMessage(chat_id, "Alat Berjalan Dengan Mode Otomatis");
      MAlat = 1;
    }

    else if (Pesan == (Perintah_Auto)){
      Serial.println(Pesan);
      bot.sendMessage(chat_id, "Alat Berjalan Dengan Mode Manual");
      MAlat = 0;
    }

    else if (Pesan == (Perintah_Pakan)) {
      Serial.println(Pesan);
      myservo.write(180);
      delay(200);
      myservo.write(0);
//      delay(500);
      bot.sendMessage(chat_id, "Pemberian Pakan Berhasil!!!", "");
    }

    else if (Pesan == Pompa1 && MAlat == 1){
      Serial.println(Pesan);
      bot.sendMessage(chat_id, "Saat Ini Alat Dalam Mode Otomatis");}

    else if (Pesan == Pompa1 && MAlat == 0){
      Serial.println(Pesan);
      digitalWrite(Relay2, ON);
      bot.sendMessage(chat_id, "Pompa Dinyalakan Secara Manual");
      SPompa = 1;
      }
  
    else if (Pesan == Pompa && MAlat == 1){
      Serial.println(Pesan);
      bot.sendMessage(chat_id, "Saat Ini Alat Dalam Mode Otomatis");}

    else if (Pesan == Pompa && MAlat == 0){
      Serial.println(Pesan);
      digitalWrite(Relay2, OFF);
      bot.sendMessage(chat_id, "Pompa Dimatikan Secara Manual");
      SPompa = 0;
      }

    else if (Pesan == Heater1 && MAlat == 1){
      Serial.println(Pesan);
      bot.sendMessage(chat_id, "Saat Ini Alat Dalam Mode Otomatis");
      }

    else if (Pesan == Heater1 && MAlat == 0){
      Serial.println(Pesan);
      digitalWrite(Relay1, ON);
      bot.sendMessage(chat_id, "Heater Dinyalakan Secara Manual");
      SHeater = 1;
      }

    else if (Pesan == Heater && MAlat == 1){
      Serial.println(Pesan);
      bot.sendMessage(chat_id, "Saat Ini Alat Dalam Mode Otomatis");
      }

    else if (Pesan == Heater && MAlat == 0){
      Serial.println(Pesan);
      digitalWrite(Relay1, OFF);
      bot.sendMessage(chat_id, "Heater Dimatikan Secara Manual");
      SHeater = 0;
      }

     else if (Pesan == Penutup && MAlat == 1){
      Serial.println(Pesan);
      bot.sendMessage(chat_id, "Saat Ini Alat Dalam Mode Otomatis");}

     else if (Pesan == Penutup && MAlat == 0){
      Serial.println(Pesan);
      stepper.runToNewPosition(13000);
      delay(500);
      bot.sendMessage(chat_id, "Kolam Dibuka Secara Manual");
      SPenutup = 0;
      stepper.disableOutputs();
    }

     else if (Pesan == (Penutup1) && MAlat == 1){
      Serial.println(Pesan);
      bot.sendMessage(chat_id, "Saat Ini Alat Dalam Mode Otomatis");}

     else if (Pesan == (Penutup1) && MAlat == 0){
        Serial.println(Pesan);
        stepper.runToNewPosition(-13000);
        delay(500);
        bot.sendMessage(chat_id, "Kolam Ditutup Secara Manual");
        SPenutup = 1;
        stepper.disableOutputs();
      }
      else if(Pesan == (Testt) && MAlat == 0){
      Serial.println(Pesan);
      myservo.write(90);
      delay(200);
      myservo.write(0);
//      delay(500);
      bot.sendMessage(chat_id, "Pembersihan Pakan Berhasil!!!", "");
      }
    
//#################    UNTUK CHECK STATUS   ###################
   
    else if (Pesan == (STPompa)) {
      if(SPompa == 1){
        bot.sendMessage(chat_id, "Saat Ini Pompa Sedang ON!!!", "");
      } else {
        bot.sendMessage(chat_id, "Saat Ini Pompa Sedang OFF!!!", "");
      }
    }

    else if (Pesan == (STPenutup)) {
        if(SPenutup == 1){
        bot.sendMessage(chat_id, "Saat Ini Kolam Sedang Tertutup!!!", "");
      } else {
        bot.sendMessage(chat_id, "Saat Ini Kolam Sedang Terbuka!!!", "");
      }
    }

    else if (Pesan == (STKolam)){
      Serial.println(Pesan);
      int range1=map(DataSHujan,0,1024,0,100);
      String welcome1 = "# Status Kolam Saat Ini #\n";
      welcome1 += "\n- Mode -\n";
      welcome1 += "Mode Alat : ";
      if (MAlat == 1){
        welcome1 += "Otomatis";
      } else {
        welcome1 += "Manual";
      }
      welcome1 += "\n- Sensor -\n";
      welcome1 += "Suhu Air : " + String(DataSAir);
      welcome1 += "\nItensitas Hujan : " + String(range1);
      welcome1 += "\n- Alat -\n";
      welcome1 += "Pompa : ";
      if (SPompa == 1){
        welcome1 += "ON";
      } else {
        welcome1 += "OFF";
      }
      welcome1 += "\nHeater : ";
      if (SHeater == 1){
        welcome1 += "ON";
      } else {
        welcome1 += "OFF";
      }
      welcome1 += "\nPelindung : ";
      if (SPenutup == 1){
        welcome1 += "Tertutup";
      } else {
        welcome1 += "Terbuka";
      }

      
      

      bot.sendMessage(chat_id, welcome1, "Markdown");
    }

//############ UNTUK MEMATIKAN SEMUA ALAT  ##############
        else if (Pesan == (SRelayOFF)) {
          digitalWrite(Relay1, OFF);
          digitalWrite(Relay2, OFF);
          SPompa = 0;
          SHeater = 0;
          MAlat = 0;
         bot.sendMessage(chat_id, "Semua Relay Sudah Di Matikan");
        }

//##################   UNTUK TEMPLATE PESAN TELEGRAM   #################
    else if (Pesan == "/start") {
      Serial.println(Pesan);
      String welcome = "Selamat Datang, " + from_name + ".\n";
      welcome += "Kontrol Kolam Budidaya Dari Manapun Dan Kapanpun Melalui Telegram\n";
      welcome += "Gunakan Kode Perintah Berikut Untuk Mengontrol Kolam Budidaya :\n\n";
      welcome += "# Perintah Ke Alat :\n";
      welcome += (Perintah_Auto1);
      welcome += " <- Mode Otomatis\n";
      welcome += (Perintah_Auto);
      welcome += " <- Mode Manual\n";
      welcome += (Perintah_Pakan);
      welcome += " <- Memberi Pakan\n";
      welcome += (Pompa1);
      welcome += " <- ON Pompa\n";
      welcome += (Pompa);
      welcome += " <- OFF Pompa\n";
      welcome += (Heater1);
      welcome += " <- ON Heater\n";
      welcome += (Heater);
      welcome += " <- OFF Heater\n";
      welcome += (Penutup1);
      welcome += " <- Buka Kolam\n";
      welcome += (Penutup);
      welcome += " <- Tutup Kolam\n";
      welcome += (Testt);
      welcome += " <- Bersihkan";
      
//      welcome += (Perintah_Off_1);
//      welcome += " <- Tekan OFF\n\n";
      
      //welcome += "# ON/OFF Relay 2:\n";
      //welcome += (Perintah_On_2);
      //welcome += " <- Tekan ON\n";
      //welcome += (Perintah_Off_2);
      //welcome += " <- Tekan OFF\n\n";
      
      
      
      welcome += "\n# Chek Status Realtime:\n";
      welcome += (STKolam);
      welcome += " - Check Status Kolam\n";
      
          
      welcome += "\n# Untuk Mematikan Semua Relay:\n";
      welcome += (SRelayOFF);
      welcome += " - Matikan Semua Relay \n\n\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
   }
  }
}


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  client.setInsecure();              

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(idTele, "Alat Online!!!");

  bot.longPoll = 5;   

  myservo.write(0);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  delay(10);
  
  digitalWrite(Relay1, OFF);
  digitalWrite(Relay2, OFF);


  myservo.attach(pinPakan);
  pinMode(pinSHujan, INPUT);

  stepper.setMaxSpeed(1500);
  stepper.setAcceleration(1500);

  sensorDS18B20.begin();
  
}

void loop() {

  DataSHujan = analogRead(pinSHujan);
  int range=map(DataSHujan,0,1024,0,100);

  sensorDS18B20.requestTemperatures();
  DataSAir = sensorDS18B20.getTempCByIndex(0); 

  Serial.print("Sensor Hujan: ");
  Serial.println(range);
  delay(1000);
  Serial.print("suhu : ");
  Serial.println(DataSAir);
  delay(1000);


  if(range <= 70 && SPenutup == 0 && MAlat == 1){
    stepper.runToNewPosition(-13000);
    delay(2000);
    SPenutup = 1;
    stepper.disableOutputs();
    bot.sendMessage(idTele, "Kolam Ditutup Secara Otomatis!!!");
  }
  else if(range >= 70 && SPenutup == 1 && MAlat == 1){
    stepper.runToNewPosition(13000); 
    delay(2000);
    SPenutup = 0;
    stepper.disableOutputs();
    bot.sendMessage(idTele, "Kolam Dibuka Secara Otomatis!!!");
  }

  else if(DataSAir <= 24 && SHeater == 0 && MAlat == 1){
    digitalWrite(Relay1, ON);
    SHeater = 1;
    bot.sendMessage(idTele, "Pemanas Menyala secara Otomatis!!!");
  }
  else if(DataSAir >= 31 && SPompa == 0 && MAlat == 1){
      digitalWrite(Relay2, ON);
      SPompa = 1;
      bot.sendMessage(idTele, "Pompa Menyala secara Otomatis!!!");
  }
  else if(DataSAir >= 28 && SHeater == 1 && MAlat == 1){
    digitalWrite(Relay1, OFF);
    SHeater = 0;
    bot.sendMessage(idTele, "Pemanas Dimatikan secara Otomatis!!!");
  }
  else if(DataSAir <= 28 && SPompa == 1 && MAlat == 1){
    digitalWrite(Relay2, OFF);
    SPompa = 0;
    bot.sendMessage(idTele, "Pompa Dimatikan secara Otomatis!!!");
  }
  
  //delay(1000);
  
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("## Ada Perintah Masuk!!! ##");
      handleNewMessages(numNewMessages);
      Serial.println("## Terbalas ##");
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
//      bot.sendMessage(idTele, "Saat Ini Kolam Sedang Terbuka!!!");
    }
    Bot_lasttime = millis();
  }
  
}