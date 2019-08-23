#define ARDUINOJSON_DECODE_UNICODE 1
#define ARDUINOJSON_ENABLE_PROGMEM 0
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#define ARDUINOJSON_ENABLE_INFINITY 1
#define ARDUINOJSON_ENABLE_NAN 1
#include <stdint.h>
#include <inttypes.h>
#include <iterator>
#include <HTTPClient.h>
#include <HardwareSerial.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <WebServer.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include <Update.h>
#include <ArduinoOTA.h>
#include "mbedtls/md.h"
#include <Hash.h>
#include <cstdlib>
#include <codecvt>
#include <string>
#include <cassert>
#include <locale>

TaskHandle_t Task1;
TaskHandle_t serverpage1;
//TaskHandle_t Task2;

byte Gethttp2list = 0;
byte TMstete = 0;
int totalbill = 0;
int a=0;

//////debug error
int Errorjump;
//// error
int ERcode = 1;
///////
//////////////////// time zone
IPAddress timeServerIP;
const char* ntpServerName = "time.cloudflare.com";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];
//// time2daay
String start_date = "";
String end_date = "";
/////////////////////////////////encode sha1HMAC wallet
 char *secretKey = "9LXAVCxcITaABNK48pAVgc4muuTNJ4enIKS5YzKyGZ";
  String mobile = "mobile";
  String l = "|";
  String reference; //= "460282400c1b5fcda72fdd5ccba7b40"
  String deviceId; //  = "f56925389b79a1b"
  String mobileTracking; //  = "yGBeeaO1U3w1Uq9NmJlwUzQfTaVA7Ma7KieRGg2vykqgvmBpj93Rg=="
  String timestamp; // = "156111594752."
  String timestamp2;
  String signature;
  String emailPWsha1;
 const char  *payload = "";
  char signaturechar[200];
  String tokensignature;
  String getTM = "";
 /////// Donate Aleart!!
   String  txid="45489110";   //// idbill
   String  txsendername="CODETEST";  ////donate name
   String  txmessage; ///// donate massage
   String  txsenderphone=""; //// phone sender
   String  txamount="1";  //// amout donate
   String  api_key="0";   //// myphone
/////////////////include TM6 api
//const char *http2api = "https://mobile-api-gateway.truemoney.com";
String check_api = "https://www.tmsteam.me/truetm/api/api_device.php?key="; //0895585545
String check_api_response;
const char *return_api = "https://truetm.tmsteam.me/api/api_true_wallet.php";
String api_host = "https://mobile-api-gateway.truemoney.com/mobile-api-gateway/api/v1"; ////apiv1
String api_endpoint_signin = "/login";  ///login
//String api_endpoint_signout = "/signout/";
//String api_endpoint_profile = "/profile/"; ////profile
//String api_endpoint_balance = "/profile/balance/";
//String api_endpoint_gettran = "/profile/transactions/history/";
String api_endpoint_gettran_new = "https://mobile-api-gateway.truemoney.com/mobile-api-gateway/user-profile-composite/v1/users/transactions/history";
//String api_endpoint_getrepor = "/profile/activities/";
///////////// wallet
//String http2checkmoney = "/mobile-api-gateway/api/v1/profile/";
String http2billist = "/mobile-api-gateway/user-profile-composite/v1/users/transactions/history";
String tokenwallet = "999-999-9999-9999";   ////c1ffd3d-aebe-44a2-9cd9-13c9fc8c919
String URLhttp2path;
String URLhttp2pathB_list;
//String URLhttp2pathB_detail;
String http2bildetail = "/mobile-api-gateway/user-profile-composite/v1/users/transactions/history/detail/";
char TMcheck_api[100];   /////////////// number phone
char sha1encode[100];   
String IDwallet = "";   //////////////// ID wallet
String PSwallet = "";   //////////////// Pass wallet
byte CKstat = 0;
///////////// loop zone get listbalance
////////////// OTAupdate
const byte LEDPIN2 = 2; 
const byte Push_button_fw = 23;
const byte button_program = 22;
const char* host = "esptm";

/////////////////////////////////// TMsteam
byte checkST = 0;
String POSTMessage = "";
////////////////////////////////////////////////// EEPROM
String Essid = "";                  //EEPROM Network SSID
String Epass = "";                 //EEPROM Network Password
String sssid = "";                  //Read SSID From Web Page
String passs = "";                 //Read Password From Web Page
///HardwareSerial Serial2(2);
#define RX_PIN 16  //3 or 16 rx UART2
#define TX_PIN 17  //1 or 17 tx UART2
//////////////////////////////////////////////////esp32 APmode
const char *ssidAP = "TMsteam";
const char *passwordAP = "123456789";
String ssid_scan;
WebServer server(80);

/////////////////////////////////////////////////esp32 scanmode
#define ARRAYSIZE 20
#define MAX_LED_COUNT 10
//input select wifi scanmode
String findwifi[ARRAYSIZE];
int report_id[MAX_LED_COUNT];  ///// real bill
int reportok[MAX_LED_COUNT];
String original_action[MAX_LED_COUNT];
int checkbills = 0;  ///// report id
int billbumber = 0;
//byte checkbalancestat = 0;/// checkbalance

//int getdetailstatz = 0;
String ssidconnect;
String inPass = "";
char SSIDchar[50];
char PWconnect[40];
int selectwifi;
////////////////////////////////////////////////////////////////////////sim900
#define SIM900a Serial2
String ATsucces;


/////////// LEDPIN
const byte LED18 = 18; ////green
const byte LED19 = 19;  ////red
const byte LED21 = 21;  ///yellow
byte BMprogramState;  //// program frimware
//////////////////////////////////////////////
/* Style */
String style PROGMEM = "<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
"#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
"form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

String style1 PROGMEM = "<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#DFBE95;font-family:sans-serif;font-size:14px;color:#777}"
"#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#FF9B34;width:0%;height:10px}"
"form{background:#fff;max-width:300px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
"div{background:#fff;max-width:400px;margin:80px auto;padding:30px;border-radius:5px;text-align:center}"
".w3-btn,.w3-button{border:none;display:inline-block;padding:8px 16px;vertical-align:middle;overflow:hidden;text-decoration:none;color:inherit;background-color:inherit;text-align:center;cursor:pointer;white-space:nowrap}"
".w3-btn,.w3-button{-webkit-touch-callout:none;-webkit-user-select:none;-khtml-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}  "
".w3-disabled,.w3-btn:disabled,.w3-button:disabled{cursor:not-allowed;opacity:0.3}.w3-disabled *,:disabled *{pointer-events:none}"
".w3-dropdown-hover:hover > .w3-button:first-child,.w3-dropdown-click:hover > .w3-button:first-child{background-color:#ccc;color:#000}"
".w3-bar-block .w3-dropdown-hover .w3-button,.w3-bar-block .w3-dropdown-click .w3-button{width:100%;text-align:left;padding:8px 16px}"
".w3-bar .w3-button{white-space:normal}"
".w3-dropdown-hover.w3-mobile,.w3-dropdown-hover.w3-mobile .w3-btn,.w3-dropdown-hover.w3-mobile .w3-button,.w3-dropdown-click.w3-mobile,.w3-dropdown-click.w3-mobile .w3-btn,.w3-dropdown-click.w3-mobile .w3-button{width:100%}}"
".w3-button:hover{color:#000!important;background-color:#ccc!important}"
".w3-deep-orange,.w3-hover-deep-orange:hover{color:#fff!important;background-color:#ff5722!important}"
".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

/* Login page */
String loginIndex PROGMEM =  
"<form name=loginForm>"
"<h2>Firmware TM Update v1.00</h2>"
"<input name=userid placeholder='User ID'> "
"<input name=pwd placeholder=Password type=Password> "
"<input type=submit onclick=check(this.form) class=btn value=Login></form>"
"<script>"
"function check(form) {"
"if(form.userid.value=='admin' && form.pwd.value=='admin')"
"{window.open('/serverIndex')}"
"else"
"{alert('Error Password or Username')}"
"}"
"</script>" + style;
 
/* Server Index Page */
String serverIndex PROGMEM = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
"<label id='file-input' for='file'>   Choose file...</label>"
"<input type='submit' class=btn value='Update'>"
"<br><br>"
"<div id='prg'></div>"
"<br><div id='prgbar'><div id='bar'></div></div><br></form>"
"<script>"
"function sub(obj){"
"var fileName = obj.value.split('\\\\');"
"document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
"};"
"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/update',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"$('#bar').css('width',Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!') "
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>" + style;
//#include <SIM900.h> 
   WiFiUDP udp;

    void setup() {

      pinMode(LEDPIN2, OUTPUT); ///////// LEd frimware
      pinMode(LED18, OUTPUT);
      pinMode(LED19, OUTPUT);
      pinMode(LED21, OUTPUT);
      digitalWrite(LEDPIN2, LOW); ///led frimware biilin
digitalWrite(LED18, LOW);  // green
digitalWrite(LED19, LOW);  // red
digitalWrite(LED21, LOW);  // yellow
      pinMode(Push_button_fw, INPUT);  /////// switch frimware
      pinMode(button_program, INPUT);  /////// switch program
     int buttonState_fw = digitalRead(Push_button_fw);  //// read switch frimware
      BMprogramState = digitalRead(button_program);  //// read switch program
      if(buttonState_fw == 1 && BMprogramState == 1) ESP.restart();
    
      //Serial1.begin(9600,SERIAL_8N1);
                Serial.begin(112500);
                EEPROM.begin(512);
                //WiFi.disconnect();
            //    Serial.println (F ("button_program = "));
               

                  WiFi.mode(WIFI_AP_STA);
/////////// EEPROM  wallet ////////////////     
 for (int i = 96; i < 160; ++i)                               //Reading SSID EEPROM
    {
      IDwallet += char(EEPROM.read(i)); 
    }
      for (int i = 160; i < 192; ++i)                            //Reading Password EEPROM
    {
      PSwallet += char(EEPROM.read(i)); 
    }
  if ( IDwallet.length() > 1 ) {  
                          
////...Serial.println(F("EEPROM PHONE: "));
Serial.println(IDwallet);
Serial.println(PSwallet);
 api_key = (IDwallet.c_str());


////...Serial.println(F("=======================================")); 
 }                     //Print Password
        /////////// EEPROM  wifi ////////////////          
  for (int i = 0; i < 32; ++i)                               //Reading SSID EEPROM
    {
      Essid += char(EEPROM.read(i)); 
    }
      for (int i = 32; i < 96; ++i)                            //Reading Password EEPROM
    {
      Epass += char(EEPROM.read(i)); 
    }
  if ( Essid.length() > 1 ) {  
    Serial.println(Essid.c_str());                             //Print SSID
    Serial.println(Epass.c_str());                            //Print Password
      WiFi.begin(Essid.c_str(), Epass.c_str());   //c_str()
 delay(1500);                                     //Wait for IP to be assigned to Module by Router
      IPAddress ip = WiFi.localIP();           //Get ESP8266 IP Adress
      Serial.print(F("EEPROMIP=")); 
      Serial.println(ip);  
  }
              WiFi.softAP(ssidAP, passwordAP, 6); //////////////////////////////////// APwifi
              IPAddress myIP = WiFi.softAPIP();
              Serial.print(F("AP IP address: "));              
	Serial.println ( WiFi.localIP() );
   ////...Serial.println(F("===============================")); 
    Serial.print(F("AP IP address: ")); 
 Serial.println(myIP);   
  ////...Serial.println(F("============end APMODE==============="));      
/////////////////////////////////////////// Http /////////////////////////////////////

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    ////...Serial.println(F("Error setting up MDNS responder!")); 
    while (1) {
      delay(1000);
    }
  }
  ////...Serial.println(F("mDNS responder started")); 
  
if ( buttonState_fw == 0 ){
   delay(500);
  server.on("/", handleRoot);
}
  else if ( buttonState_fw == 1 )
  {
     server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  }
  server.on("/test", scanfon);
  server.on("/a", wifiscana);
  server.on("/atcom", atcom);
  server.on("/tm6", tm6);
  server.on("/wallet", wallet);
  server.on("/ip", checkip);
  //server.on("/atok", atok);
  server.on("/inline", []() {
  server.send(200, "text/plain", "this works as well");
  });
if ( buttonState_fw == 1 )
  {
/////////////////////////////////ota update Webupload ////////////////
 server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
   /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });


  }
  server.onNotFound(handleNotFound);
  server.begin();
 //Serial.println("HTTP server started");
 ////////////////////////// OTA stard process Error

if ( buttonState_fw == 1 )
 { 
digitalWrite(LEDPIN2, HIGH); 
// Port defaults to 3232
 ArduinoOTA.setPort(3232);
// Hostname defaults to esp3232-[MAC]
//ArduinoOTA.setHostname("myesp32");
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      ////...Serial.println(F("\nEnd"));
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
 delay(1000);
  ////...Serial.println(F("Ready"));
  ////...Serial.println(F("IP address: "));
  //Serial.println(WiFi.localIP());
  //  pinMode(LED_BUILTIN, OUTPUT);

}
    if(buttonState_fw == 1){
    //  xTaskCreatePinnedToCore(
      //            billlistPv ,   /* Task function. */
        //           "Task2",     /* name of task. */
          //         (5000),       /* Stack size of task */
            //        NULL ,        /* parameter of the task */
              //      1,           /* priority of the task */
                //    &Task1,      /* Task handle to keep track of created task */
                  //  0);          /* pin task to core 0 */                                 
  delay(1000);   
////...Serial.println(F("Stat Frimware = ON!!"));
xTaskCreatePinnedToCore(
                    serverpagePV ,   /* Task function. */
                    "serverpagePV",     /* name of task. */
                    (1024*5),       /* Stack size of task */
                    NULL ,        /* parameter of the task */
                           1,           /* priority of the task */
                    &serverpage1,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */  
return;
    }else{
      
///////////////////////////////////////////////////////////////////////////////////
///////////////// rx tx sim900
              //Serial1.begin(115200,SERIAL_8N1, 4, 2);
           
              if(BMprogramState == 0)
              {
                
                digitalWrite(LED21, HIGH); 
                SIM900a.begin(115200,SERIAL_8N1,RX_PIN,TX_PIN);
                ////...Serial.println(F("SIM900 ready..."));
              delay(500);
// ////...Serial.println(F("Setup signal")); 
  SIM900a.print("at+cops?\r"); 
  delay(500);
  // AT command to set SIM900 to SMS mode
 // ////...Serial.println(F("SMS setting")); 
   SIM900a.print("AT+CMGF=0\r"); 
 // delay(3000);
// Set module to send SMS data to serial out upon receipt 
//////...Serial.println(F("SETUP SIM")); 
  SIM900a.print("AT+CNMI=2,2,0,0,0\r");
  // delay(3000);
//////...Serial.println(F("Check number"));
 SIM900a.print("at+cnum\r");
  //delay(3000);
////...Serial.println(F("Check CPIN"));
   SIM900a.print("AT+CPIN?\r");
  }else if(BMprogramState == 1)
  {
    
  onlinetmAPI();   ///getapi from TM

   delay(1000);
  ////...Serial.println(F("===============real api================="));
  //Serial.println(reference);
  //Serial.println(deviceId);
  Serial.println(mobileTracking);
  if (reference == "null" || deviceId == "null" ||  mobileTracking == "null")
  {
    delay(1000);
        xTaskCreatePinnedToCore(
                  errorlooppv ,   /* Task function. */
                   "Taskerror",     /* name of task. */
                   (1000),       /* Stack size of task */
                    NULL ,        /* parameter of the task */
                    5,           /* priority of the task */
                    NULL,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */    
       ///////////////////
    xTaskCreatePinnedToCore(
                    serverpagePV ,   /* Task function. */
                    "serverpagePV",     /* name of task. */
                    (1024*5),        /* Stack size of task */
                    NULL ,        /* parameter of the task */
                          1,           /* priority of the task */
                    &serverpage1,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */  

    return;
  }
////...Serial.println(F("===============real timestamp================="));
   delay(100);
  gettimezone(); ///timestamp
   Ftokensignature(); /// sha1 walletHmac  //// check er ok
////...Serial.println(F("===============login================="));
  POSTtruewallet(); ///// check er ok
  void clear();
  if(Errorjump == 50001)
  {
    digitalWrite(LED19, HIGH);
    
     xTaskCreatePinnedToCore(
                    serverpagePV ,   /* Task function. */
                    "serverpagePV",     /* name of task. */
                    (1024*5),        /* Stack size of task */
                    NULL ,        /* parameter of the task */
                              1,           /* priority of the task */
                    &serverpage1,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */  
                    Serial.println("50001");
   return;
  }

  if(ERcode == 5)
  {
    delay(5000);
    ESP.restart();
  }

  xTaskCreatePinnedToCore(
                    billlistPv ,   /* Task function. */
                    "Task2",     /* name of task. */
                    (5000),       /* Stack size of task */
                    NULL ,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */       
                 
  delay(1000); 
     
   check_api = "";   
   //postwallet = "";   
   timestamp = "";  
         tokensignature = "" ;       
  }
    }
     xTaskCreatePinnedToCore(
                    serverpagePV ,   /* Task function. */
                    "serverpagePV",     /* name of task. */
                    (1024*5),        /* Stack size of task */
                    NULL ,        /* parameter of the task */
                    1,           /* priority of the task */
                    &serverpage1,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */  
   
      return;   
}
         void loop(){
           if(BMprogramState == 0)
    {      
             txtmessage();
             delay(100);
             checkstatus(); 
         if (checkST == 1){
         Serial.println(POSTMessage);
             SMSPOSTAPI(); 
             POSTMessage = "";     
             checkST = 0;
             }
    }
   
                              if(TMstete == 1) 
                                    {
                                     //Serial.println(totalbill);                                
        for(int De = totalbill; De >= 0; De--) {
        
            // if (reportok[De] == 1) {
             billbumber = De;
                  getbil();
                  delay(100);
                tmaleart();
                report_id[De] = NULL;
                void clear();
             //}
        }
         Serial.println("end for Tmsteam = 0");
        TMstete = 0;
                                    } 
   
}

     void checkstatus(){
     
if (POSTMessage != ""){ 
  delay(100);
checkST = 1;
//Serial.println("checkST = 1"); 
     }else{
       delay(100);
checkST = 0;
//Serial.println("checkST = 0"); 
     }
     return;
}

void txtmessage(){ 
  while (SIM900a.available())
  POSTMessage = SIM900a.readString();
  //  Serial.println(textMessage);    
}

void tmaleart()  
{
  int i=0;
 
delay(100); 
txid = String(report_id[billbumber]);   //// 
HTTPClient http;
http.begin(return_api);
http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
http.addHeader("Host", "truetm.tmsteam.me");
http.addHeader("cache-control", "no-cache");
http.addHeader("Connection", "keep-alive");
http.addHeader("accept-encoding", "gzip, deflate");
http.addHeader("Accept", "*/*");
http.POST("txid="+txid+"&txsendername="+txsendername+"&txmessage="+String(txmessage)+"&txsenderphone="+txsenderphone+"&txamount="+txamount+"&api_key="+String(api_key)); 
String response = http.getString();
 http.end();  //Free resources
   response = "";
}

void SMSPOSTAPI() {
HTTPClient http;   
delay(1000);
   http.begin("https://truetm.tmsteam.me/api/api_esp32.php");  //Specify destination for HTTP request
   http.addHeader("Content-Type", "text/plain");             //Specify content-type header
   http.POST(POSTMessage);   //Send the actual POST request
  String response = http.getString();                       //Get the response to the request
   Serial.println(response);           //Print request answer
   http.end();  //Free resources
 return;
}
void onlinetmAPI() {
  //////////////GET
 HTTPClient http;   
    delay(100);
   check_api +=IDwallet;
  check_api.toCharArray(TMcheck_api, 100); //idwallet to char
 //TMcheck_api 
   http.begin(TMcheck_api);  //Specify destination for HTTP request
   Serial.println(TMcheck_api);
   http.addHeader("Content-Type", "application/json");             //Specify content-type header  
   http.addHeader("Host","www.tmsteam.me");
  int httpCode = http.GET();
    delay(100);
   if (httpCode > 0) { //Check the returning code
   String response = http.getString(); //Get the request response payload
 check_api_response = response.substring(1);
 response = "";
 }
 http.end(); //Close connection
 
////////////////string to json
    DynamicJsonDocument doc(700);
deserializeJson(doc, check_api_response);
JsonObject obj = doc.as<JsonObject>();
long useridJson = obj[String("user_id")];
String referencejson = obj["reference_token"];
String deviceIdjson = obj["device_id"];
String mobileTrackingjson = obj["mobile_tracking"];
delay(100);
  reference = referencejson;
  deviceId = deviceIdjson;
  mobileTracking = mobileTrackingjson;
  ///////////pass
delay(100);
//////////////// login PIN
String waa; 
char a1[100],a2[100];
IDwallet.toCharArray(a1,100);
PSwallet.toCharArray(a2,100);
waa = String(a1)+String(a2);
   waa.toCharArray(sha1encode,100);
    Serial.print(F("SHA1:"));
   Serial.println(sha1encode);
    emailPWsha1 = sha1(sha1encode);
    check_api_response = "";
    delay(50);
    void clear();
 return;
  }

void handleRoot() {
  String temp PROGMEM =
  "<html>"
  "<head>"
   "<meta http-equiv='refresh' content='5'/>"
   "<title>TM Config</title>"
  "</head>"
  //"<link rel='stylesheet' href='https://www.w3schools.com/w3css/4/w3.css'>"
  
  "<body>"
 "<div>"
"<h1>TMsteam Donate !</h1>"
"<a href='/test' class='w3-button w3-deep-orange'>SCAN WIFI</a>"
"<p></p>"
"<a href='/wallet' class='w3-button w3-deep-orange'>SETUP WALLET!</a>"
"<p></p>"
"<a href='/atcom' class='w3-button w3-deep-orange'>AT COMMAND</a>"
"<p></p>"
"<a href='/tm6' class='w3-button w3-deep-orange'>REPORT COMMAND</a>"
"<p></p>"
"<a href='/ip' class='w3-button w3-deep-orange'>Check IP</a>"
"</div>"
 "</body>"
"</html>" + style1;
  server.send(200, "text/html", temp);
}
void handleNotFound() {
//  FWfail:
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void scanfon() {
  //String out = "";
  int Tnetwork=0,i=0,len=0;
   String st="",out="";  
   Tnetwork = WiFi.scanNetworks();  
  st = "<select name=\"ssid\">";
  for (int i = 0; i < Tnetwork; ++i)
    {
         /////////////////////////select WIFI
         delay(10);
      st += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*";
     ssid_scan = WiFi.SSID(i);
      st += "<option value=" + String(i) + ">#" + ssid_scan + "( " + (WiFi.RSSI(i)+150) + "\% )</option>";
      findwifi[i] = ssid_scan;
      Serial.println(findwifi[i]);
    }
      st += "</select>";
 IPAddress ip = WiFi.softAPIP(); 
        out = "<div>\n\r\n<!DOCTYPE HTML>\r\n<html><h1>Scan Wifi!</h1> ";
        out += "<form method='post' action='a'><label>SSID: </label>" + st + "<p><label>PASSWORD : </label><input name='pass' length=64 placeholder='Ex : Pass1234'></p><input type='submit'></form>";
        out += "<a href =\"/\"><button>Back</button></a>";
        out += "</div>";
        out += "</html>\r\n\r\n"+ style1; 
   server.send( 200 , "text/html", out);
}
//////////////function clear eeprom
void ClearEeprom(){
        ////...Serial.println(F("Clearing Eeprom"));
        for (int i = 0; i < 96; ++i) { 
          EEPROM.write(i, 0); 
          }
  }
  void ClearEepWALL(){
        Serial.println("ClearROM wallet");
        for (int i = 96; i < 192; ++i) { EEPROM.write(i, 0); }
  }

void wifiscana() {
String message1 = "WIFI_OK\n\n";
delay(200);
  ////////// LOOP ARG
  for (uint8_t i = 0; i < server.args(); i++) {
    message1 += " " + server.argName(i) + ": " + server.arg(i) + "\n"; 
  }   

selectwifi = server.arg(0).toInt(); // stringARG.toInt();    
Serial.println(selectwifi);
///PASSWORD               #inpass
inPass = server.arg(1);
Serial.println(inPass);
//////////////////////////////////////////////
  Serial.print(F("Connect Mode Connecting to "));
ssidconnect = findwifi[selectwifi];
ssidconnect.toCharArray(SSIDchar, 50);
inPass.toCharArray(PWconnect, 50);
Serial.print(F("SSIDchar : "));
  Serial.println(SSIDchar);
Serial.println(PWconnect);
 // WiFi.begin(SSIDchar, PWconnect);
 WiFi.begin(SSIDchar, PWconnect);
      for (int i = 0; i <= 20; i++) {
    if (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(F("."));
    }
    delay(1007);
  }

  ////...Serial.println(F("WiFi connected"));
  ////...Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
delay(200);
/// var to eeprom
   sssid = findwifi[selectwifi];
   passs = PWconnect;
  if(sssid.length()>1 && passs.length()>1){
     ClearEeprom();//First Clear Eeprom
     delay(200);
     for (int i = 0; i < sssid.length(); ++i)
          {
                 EEPROM.write(i, sssid[i]);
          }
        
     for (int i = 0; i < passs.length(); ++i)
          {
            EEPROM.write(32+i, passs[i]);
          }    
     EEPROM.commit();
  }
 Serial.print(F("EEPROM SSid: "));
 Serial.println(sssid);
 Serial.print(F("EEPROM Password: "));
 Serial.println(passs);
 delay(1000);
server.send(200, "text/plain", message1);
}

void atcom(){
  delay(200);
   String atcom="",txtMsg="";  
   String messageat = "ATCommand\n\n";
ATsucces = server.arg(0);
ATsucces += "\r";
//ATsucces += "\n";
SIM900a.println(ATsucces);
////...Serial.println(F("whileloop :"));  
txtMsg = SIM900a.readString();
        atcom = "\n\r\n<!DOCTYPE HTML>\r\n<html><h1>SIM Control</h1> ";
        atcom += "<form method='post' action='atcom'><label>ATCommand: </label><input name='command' length=64><input type='submit'></form>";
       //  for (uint8_t i = 0; i < 5; i++) {
         atcom += "<p></p>";
         atcom += " " + txtMsg + "\n";
 // }
 atcom += "<a href =\"/\"><button>Back</button></a>";
        atcom += "</html>\r\n\r\n";
   server.send( 200 , "text/html", atcom); 
}

void tm6(){
  delay(200);
   String tm6="";  
   String TMsucces="";

   String messageat = "REPORT\n\n";
TMsucces = "Frist connect = : " + reference +" "+ deviceId +" "+mobileTracking +"<br><br>";
TMsucces += "Sha1 = : " + String(sha1encode) +"<br><br>";
TMsucces += "tokenwallet = : " + tokenwallet +"<br><br>";
TMsucces += "Phone = : " + String(IDwallet.c_str()) +"<br><br>";
TMsucces += "PIN = : " + String(PSwallet.c_str()) +"<br><br>";
TMsucces += "List = : " + URLhttp2pathB_list +"<br><br>";

        tm6 = "\n\r\n<!DOCTYPE HTML>\r\n<html><h1>REPORT Control</h1> ";
        tm6 += "<form method='post' action='tm6'><label>INPUT: </label><input name='command' length=64><input type='submit'></form>";
       //  for (uint8_t i = 0; i < 5; i++) {
         tm6 += "<p></p>";
         tm6 += " " + TMsucces + "<br><br>";
         tm6 += "<p></p>";
         tm6 += "<a href =\"/\"><button>Back</button></a>";
        tm6 += "</html>\r\n\r\n";
   server.send( 200 , "text/html", tm6); 
}

void wallet(){
  delay(200);
  if (server.hasArg("Phone") && server.hasArg("PIN")){  
       CKstat = 1;
       ////...Serial.println(F("CKstat = 1"));
   IDwallet=server.arg("Phone");//Get SSID
   PSwallet=server.arg("PIN");//Get Password
  }
  reloadpage :
   String wallet="";  
   wallet = "\n\r\n<!DOCTYPE HTML>\r\n<html><h1>TMsteam Wallet</h1> ";
if (CKstat == 0){
   wallet += "<form method='post' action='#'><label>Phone Number: </label><input name='Phone' length=32><label>PIN: </label><input name='PIN' length=64><input type='submit'></form>";   
    if (CKstat == 1){
     return;
   }
   wallet += "<a href =\"/\"><button>Back</button></a>";
   wallet += "</html>\r\n\r\n";
   server.send( 200 , "text/html", wallet);
}    
if(CKstat == 1){
  if(IDwallet.length()>1 && PSwallet.length()>1){
    CKstat++;
     ClearEepWALL();//First Clear Eeprom wallet
     delay(10);
     for (int i = 0; i < IDwallet.length(); ++i)
          {
            EEPROM.write(96+i, IDwallet[i]);
          }
     for (int i = 0; i < PSwallet.length(); ++i)
          {
            EEPROM.write(160+i, PSwallet[i]);
          }    
     EEPROM.commit();
     goto reloadpage;
     }
     }
     if(CKstat == 2){
  Serial.print(F("EEPROM PHONE: "));
   Serial.println(IDwallet);
 Serial.print(F("EEPROM Wallet PW: "));
 Serial.println(PSwallet);
        wallet += "<h1>Setup WALLET OK!!!</h1>";
        wallet += "<p>Password Saved... ";
        wallet += "<p>Plase Restart to Change password ";
        wallet += "<a href =\"/\"><button>Back</button></a></html>\r\n\r\n";
        server.send(200,"text/html",wallet);
     }
    }

  void checkip(){
    delay(500);
   String checkip="",youip="";  
   String messageat = "ATCommand\n\n";
 youip = WiFi.localIP().toString();
        checkip = "\n\r\n<!DOCTYPE HTML>\r\n<html><h1>Check Your IP...</h1> ";
        checkip += "Your IP in home : ";
         checkip += " " + youip + "\n\n";
 // }
 checkip += "<a href =\"/\"><button>Back</button></a>";
        checkip += "</html>\r\n\r\n";
   server.send( 200 , "text/html", checkip); 
}
    
 //////////zone loginwallet sha1HMAC

 void Ftokensignature(){  ///no delay!!
 signature = mobile + l + reference + l + deviceId + l + mobileTracking + l + timestamp;
 signature.toCharArray(signaturechar, 200);
 payload = signaturechar;
 
 Serial.println(payload);
  byte hmacResult[20];
 
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA1;
 
  const size_t payloadLength = strlen(payload);
  const size_t keyLength = strlen(secretKey);            
 
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
  mbedtls_md_hmac_starts(&ctx, (const unsigned char *) secretKey, keyLength);
  mbedtls_md_hmac_update(&ctx, (const unsigned char *) payload, payloadLength);
  mbedtls_md_hmac_finish(&ctx, hmacResult);
  mbedtls_md_free(&ctx);
 
  Serial.print(F("Hash: "));

  for(int i= 0; i< sizeof(hmacResult); i++){
      char str[3];
      sprintf(str, "%02x", (int)hmacResult[i]);
      Serial.print(str);
      tokensignature += str;
  }
////...Serial.println(F(""));
  Serial.println("tokensignature : "+tokensignature);
  return;
} 

void gettimezone(){
 
  int i;
 WiFi.hostByName(ntpServerName, timeServerIP); 
  delay(1000);
 label:
 sendNTPpacket(timeServerIP);
 int cb = udp.parsePacket();
  if (!cb) {
 
      if(i==20)
        {
          ESP.restart();
        }
        i++;
   goto label;
  }
  else {
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long  epoch = secsSince1900 - seventyYears;

uint32_t frac  = (uint32_t) packetBuffer[44] << 24
               | (uint32_t) packetBuffer[45] << 16
               | (uint32_t) packetBuffer[46] <<  8
               | (uint32_t) packetBuffer[47] <<  0;
               uint16_t mssec = ((uint64_t) frac * 1000) >> 32;
     Serial.println(epoch);
Serial.print(F("millisecondsSinceEpoch : "));
//int second = epoch % 60;
//Serial.println(second);
char buffer[4];
sprintf(buffer,"%03d", mssec); /////millisec 999 = 1sec
 //Serial.println(buffer);
 timestamp = String(epoch);
 timestamp += String(buffer);
 Serial.println(timestamp);
 return;
  }
 return;
}

void POSTtruewallet(){   ////no delay!!
  digitalWrite(LED18, HIGH); 
  char a1[100],a2[100];
IDwallet.toCharArray(a1,100);
PSwallet.toCharArray(a2,100);
 ///createJson true
  StaticJsonDocument<1000> testDocument;
  testDocument["type"] = "mobile";
  testDocument["reference_token"] = reference;
  testDocument["device_id"] = deviceId; 
  testDocument["mobile_tracking"] = mobileTracking;
  testDocument["timestamp"] = timestamp;
  testDocument["signature"] = tokensignature;
char buffer[1000];
  serializeJson(testDocument, buffer);
  //Serial.println(buffer);
////POST loginwallet
  String postwallet;
   postwallet = api_host + api_endpoint_signin;
  HTTPClient http;  

  http.begin(postwallet); ////post wallet api
//////header
http.addHeader("Host", "mobile-api-gateway.truemoney.com");
http.addHeader("Content-Type","application/json");   ///header
http.addHeader("User-Agent", "okhttp/3.12.0");
//http.addHeader("Cache-Control", "no-cache");
//http.addHeader("Accept", "*/*");
http.addHeader("username", a1);
http.addHeader("password", emailPWsha1);
http.POST(buffer); 
  String response = http.getString();   
   //Serial.println(response);  
 http.end();  //Free resources
   ////////////////string to json walletlogin
    DynamicJsonDocument doc(2000);
deserializeJson(doc, response);
JsonObject obj = doc.as<JsonObject>();
int codeJson = obj[String("code")];
//String current_balancejson = obj["data"]["current_balance"];
//String thai_idjson = obj["data"]["thai_id"];
String access_tokenjson = obj["data"]["access_token"];
//String fullnamejson = obj["data"]["fullname"];
//String address_listjson = obj["data"]["address_list"];
ERcode = codeJson;

//check error
Serial.println("error code : "+String(ERcode)); //1
errorloop();   ///ckeck = codeJson 200
if(Errorjump == 50001)
{
  delay(1000);
digitalWrite(LED18, LOW);  // green
 return;
}

//Serial.println(current_balancejson);
//current_balance = current_balancejson;
//Serial.println(thai_idjson);
//Serial.println(address_listjson);
//Serial.println(access_tokenjson);
 tokenwallet = access_tokenjson;
Serial.println(tokenwallet);
response = "";
void clear();
 delay(100 );
////...Serial.println(F("============== get token ============"));
   
   digitalWrite(LED18, LOW); 
delay(500);
   digitalWrite(LED18, HIGH); 
   return;
}


unsigned long sendNTPpacket(IPAddress& address)
{
 ////...Serial.println(F("sending NTP packet..."));
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  
  udp.beginPacket(address, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}


//////////// if 200 login ok //gettoken
//////#////// 20000  ckeck balance //USE token ok
/////////// UPC-200 check today //listreport ok
///#/// 50001 ID phone number Error;   //setup password wallet  call 1240
////// 400  pass failed      nulltoken  [warning!!!]      //setup wallet  & setup TMsteam
///#// 10001 login again
////// MAG-429 signature fail [warning!!!] & login fasted!!!!! ///setup tmsteam & nulltoken //error code0
////// MAS-401  check today listreport fail       
//#/// error code 0 null setuptm
///////////////token
///// 10001 token outtime 
//#// 4 can't resgis tm
//// 1 var not change
//// 5 goto login loop auto programsetting
//#/// 2 token tm = notoken
///// 3 fail get form host balance error != 20000
///// 6 Bill ID = 0 go to checkbalance
void errorloop(){
  while(ERcode != 200){    //////////// if 200
    digitalWrite(LED18, LOW); 
 
    Serial.println("error code : "+ String(ERcode)); //1  Stringnull // 2 token null // 3 connect error not 20000
digitalWrite(LED19, HIGH); 
delay(100);
digitalWrite(LED19, LOW); 
delay(100);
            if(ERcode == 50001)  ////// 50001 ID Error;
            {

             Errorjump = 50001; 
             ////...Serial.println(F("error 50001"));
             delay(10000);
             return;
            }
  }
}
void errorlooppv( void * pvParameters){
  while(1)
  {
    digitalWrite(LED19, HIGH);
    vTaskDelay(200);
    digitalWrite(LED19, LOW);
    vTaskDelay(200);
  }
}
void serverpagePV( void * pvParameters ){
  while(1) server.handleClient();

}
void billlistPv( void * pvParameters ){
    while(1) {
       NoUpdate :
    int watcdog = 0;
    digitalWrite(LED21, HIGH);
  errorget:
  vTaskDelay(200);
  Gethttp2list = 3;
   timedaay(); 
   URLhttp2pathB_list = http2billist + "?start_date=" +  String(start_date) + "&end_date=" + String(end_date) + "&limit=5";
   char parthB_list[150];
   URLhttp2pathB_list.toCharArray(parthB_list, 150);
   Serial.println(parthB_list);
  String url;
 url = "https://mobile-api-gateway.truemoney.com" + String(URLhttp2pathB_list);
//Serial.println(url);
HTTPClient http; 
http.begin(url);
http.addHeader("Host", "mobile-api-gateway.truemoney.com");
http.addHeader("User-Agent", "okhttp/3.12.0");
http.addHeader("Authorization", tokenwallet);
http.addHeader("Content-Type", "application/x-www-form-urlencoded");  
int httpCode = http.GET();
if (httpCode > 0) { 
 String datalisthttp2 = http.getString();
digitalWrite(LED21, LOW);
DynamicJsonDocument doc(3500);
deserializeJson(doc, datalisthttp2);
datalisthttp2 = "";
JsonArray array = doc.as<JsonArray>();
String getcode = doc["code"];
if(getcode != "UPC-200")
            { 
               Serial.println("==listcode != UPC-200=="+getcode);
                               ////LIST fail
                 // Serial.println("==========Bill list GET fail goto check balance]============="); 
               errorloop();
               ESP.restart();
               //vTaskDelete(NULL);
             }           
int totalJson = doc["data"][String("total")];
      if(totalJson == 0)
      {
   ////...Serial.println(F("nobill++ 00:00 PM"));
   vTaskDelay(7000);
 
   Gethttp2list = 4;
    goto NoUpdate;
      }

  a = 0;
////////////// loopbill+
     while(1){
                   digitalWrite(LED18, HIGH);
                   vTaskDelay(500);
                   digitalWrite(LED18, LOW);
                   vTaskDelay(500);
                   //Serial.println("wait0");
                   if(TMstete == 0) break;
                 }
 for(int i = 4; i >= 0; i--)
  {
  reportok[i] = 0;
int reportidJson = doc["data"]["activities"][i]["report_id"];
String original_actionJson = doc["data"]["activities"][i]["original_action"];
    //////sender
    Serial.println(reportidJson);
 if (reportidJson > checkbills && original_actionJson == "creditor")
                {
                   vTaskDelay(50);
                       checkbills = reportidJson;
                       report_id[a] = reportidJson;
                       original_action[a] = original_actionJson;
                       reportok[i] = 1;
                       Serial.println("///////"+String(i) + " : "+String(a)+ " "+ String(report_id[a])+ " "+ String(reportok[i]));    ////// bill num 706998544
                       a++;
                }
  }

void clear();
      Gethttp2list = 1;
      
}else{
  Gethttp2list = 2;
}
waitget :     
     switch (Gethttp2list) {
  case 0:
    vTaskDelay(100);
      goto waitget;
    break;
  case 1:
            ////...Serial.println(F("bill list ok goto NEW"));     
             Gethttp2list = 0;
            
             while(1)
                 {
                   digitalWrite(LED18, HIGH);
                   vTaskDelay(500);
                   digitalWrite(LED18, LOW);
                   vTaskDelay(500);
                  // Serial.println("wait0");
                   if(TMstete == 0) break;
                 }

                 totalbill = a-1;
                 
                // Serial.println("totalill = "+String(totalbill));
                 if(totalbill < 0) 
                 {
                    // Serial.println("total < = 0 ");
                 }else{
                  // Serial.println("total OK !=0 ");
                   TMstete = 1;
                   }
             digitalWrite(LED21, LOW); 
             vTaskDelay(6000);
      goto NoUpdate;
    break;
     case 2:
    // statements
    vTaskDelay(1000);
    ////...Serial.println(F("//////////watchDOG//////////"));
       watcdog++;
              if(watcdog < 3){
                          vTaskDelay(2000);
                         goto errorget;
                            }
                            ////...Serial.println(F("watcdog > 3 RESTART"));
                           
                            ESP.restart();
    break;
     case 3:
    // statements
     vTaskDelay(100);
     goto waitget;
    break;
     case 4:
    // statements
      vTaskDelay(100);
       goto NoUpdate;
    break;
  default:
    // statements
    break;
}                               
                                    
}  /////while
}////void



///////////////////testzone //////
/* 
void tmlist()
{
  digitalWrite(LED18, HIGH);  // green
digitalWrite(LED19, HIGH);  // red
digitalWrite(LED21, LOW);  // yellow
digitalWrite(LEDPIN2, LOW);  // yellow
  timedaay();
delay(100); 
txid = timestamp2;
delay(10); 

String url = return_api;
Serial.println(sentdata);
HTTPClient http;
http.begin("https://postman-echo.com/post");   ////tmsteam post
http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
http.addHeader("cache-control", "no-cache");
http.addHeader("Connection", "keep-alive");
http.addHeader("accept-encoding", "gzip, deflate");
http.addHeader("Accept", "");
http.POST(sentdata); 
Serial.println("=======sender to tm...=========");   
String response = http.getString();
       Serial.println(response);           //Print request answer
delay(10);  
 http.end();  //Free resources
return;
}

*/







void timedaay()
{
 String years,daydate;
 int mont,Emont,Smont,Edaydate,Sdaydate;
  static const char default_format[] = "%a %b %d %Y";
int timezone = 7 * 3600;      
int dst = 0;   
   configTime(timezone, dst, "time.cloudflare.com", "time.navy.mi.th"); 
while (!time(nullptr)) { 
     delay(1000);   
     }  
     do{
       delay(50);
      time_t now = time(nullptr); 
      time_t snow = time(nullptr);   
       snow = now-96400;
      time_t enow = time(nullptr);   
      enow = now+96400; 
              struct tm* p_tm = localtime(&now);    
             // Serial.print(p_tm->tm_mday);  
            daydate = p_tm->tm_mday; 
         
        // Serial.print("-"); 
       //  Serial.print(daydate); 
       // Serial.print("-"); 
             // Serial.print(p_tm->tm_mon + 1);   
              mont = p_tm->tm_mon + 1;
           //   char bufmonth[3];
            //  sprintf(bufmonth,"%02d", mont); /////millisec 999 = 1sec
       // Serial.print(mont);   
      //Serial.print("-"); 
             // Serial.print(p_tm->tm_year + 1900);   
             years = p_tm->tm_year + 1900;
             
      //  Serial.print(years); 
       // Serial.print(" ");   
            //  Serial.print(p_tm->tm_hour);   
            //  Serial.print(":");   
            //  Serial.print(p_tm->tm_min);  
            //  Serial.print(":");   
            //  Serial.print(p_tm->tm_sec);   
            //  Serial.println("");  
         
struct tm* startwalletdate = localtime(&snow);
 Sdaydate = startwalletdate->tm_mday;  ////start date-1day
    char bufSday[3];
             sprintf(bufSday,"%02d", Sdaydate);
 
Smont = startwalletdate->tm_mon + 1;
              char bufSmont[3];
              sprintf(bufSmont,"%02d", Smont); /////month
 

  ///////////////enddate
   struct tm* End1date = localtime(&enow);
        Edaydate = End1date->tm_mday;  ////end date+1day
          char bufEdaydate[3];
          sprintf(bufEdaydate,"%02d", Edaydate); /////day
          

 Emont = End1date->tm_mon + 1;
              char bufEmonth[3];
              sprintf(bufEmonth,"%02d", Emont); /////month
start_date  = years+"-"+bufSmont+"-"+bufSday;
end_date  = years+"-"+bufEmonth+"-"+bufEdaydate;
            timestamp2 = now;
     }while(years == "1970");
   //  Serial.println("startdate = "+start_date);
 //    Serial.println("enddate = "+end_date);
}

void getbil()
{
  digitalWrite(LED21, HIGH);
  int Dsend = 0;
  sendDagain :
String url;
  url = api_endpoint_gettran_new + "/detail/" + report_id[billbumber];
Serial.println(url);
if(report_id[billbumber] == 0) 
{
  delay(100);
return;
}
HTTPClient http; 
http.begin(url);
http.addHeader("Host", "mobile-api-gateway.truemoney.com");
http.addHeader("User-Agent", "okhttp/3.12.0");
http.addHeader("Authorization", tokenwallet);
http.addHeader("Content-Type", "application/json; charset=utf-8");  //text/html;charset=utf-8
int httpCode = http.GET();
if (httpCode > 0) { //Check the returning code
 String responseDetail = (UTF16toUTF8(http.getString()));
 http.end(); //Close connection   
DynamicJsonDocument doc(1300);
deserializeJson(doc, responseDetail);
responseDetail = "";
String checkcode = doc["code"];
txamount = doc["data"]["amount"].as<String>();
txsenderphone = doc["data"]["ref1"].as<String>();
txsendername = doc["data"]["section2"]["column1"]["cell2"]["value"].as<String>();
txmessage = doc["data"]["personal_message"][String("value")].as<String>();
Serial.println(txamount);
Serial.println(txsenderphone);
Serial.println(txsendername);
Serial.println(txmessage);
 if(checkcode != "UPC-200")
                  {
                      // ERcode = 5;
                          Serial.println("=tokenfail!!!!=");
                          delay(1000);
                        return;
                  }                          
 void clear();
}else if(Dsend <3){
     Serial.println("failed = " + String(httpCode));
     Dsend++;
     goto sendDagain;
     }else{
       ////...Serial.println(F("DOWN!!")); 
       delay(1000);
       return;
           }
 digitalWrite(LED21, LOW);
return;
}
 // Serial.println("【 Unicode ( UTF16 ) 】");
  //Serial.println( utf16_str );
  //Serial.println("【 UTF16 → UTF8 】");
  //Serial.println( UTF16toUTF8( utf16_str ) );
//******************************************
  String UTF16toUTF8(String str){
  //str.replace("♥️", "\\2665\\fe0f");
 // str.replace("✨", "\\2763\\fe0f");
  str.replace("\\u","\\");
  str += '\0';
   uint16_t len = str.length();
  char16_t utf16code[len];
 
  int i=0;
  String str4 = "";
  for(int j=0; j<len; j++){
    if(str[j] == 0x5C){ //'\u0e01'D83DDE0AD83DDE19D83DDE1
      j++;
      for(int k=0; k<4; k++){
        str4 += str[j+k];
      }
      utf16code[i] = strtol(str4.c_str(), NULL, 16); //16進文字列を16進数値に変換
      str4 = "";
      j = j+3;
      i++;
    }else if(str[j] == 0x23){ //'#'delete str[j] UTF8encoding
      utf16code[i] = 0xFF03; //encode // "#" // utf16code[i]
      i++;
  }else if(str[j] == 0xe0 ){ //ช for(int z=0; b<2)
               j++;
                if(str[j] == 0xb8){
                   j++;
                   int cod = 0;
                            for(int z=0x80; z<=0xbf; z++){
                                     if(str[j]==z){
                                      utf16code[i] = 3584+cod;  
                                      i++;
                                      }
                      cod++;
                  }

            }else if(str[j] == 0xb9)
            {
                    j++;
                   int cod = 0;
                              for(int z=0x80; z<=0xbf; z++){
                                     if(str[j]==z){
                                      utf16code[i] = 3648+cod;  
                                      i++;
                                      }
                                      cod++;
                              }
            }
  }else if(str[j] == 0xe2 ){// byte1 of 3 byte
          j++;
          int cod1 = 0;   //x80
                 for(int z=0x80; z<=0xbf; z++){   // byte2 of 3 byte //str[j]= 81 cod1=1
                       if(str[j]==z){
                            j++;
                             const int Cbit= 64;
                            int cod2 = 0;
                                 for(int z=0x80; z<=0xbf; z++){   // byte3 of 3 byte //str[j]= 81 cod1=1
                                    if(str[j]==z){
                                              j--;             
                                    utf16code[i] = 8192+(cod2+(cod1*Cbit));
                                    i++;
                                            if(str[j+2]==0xEF){  ///\ufe0f emoji double
                                            j++;
                                                         if(str[j+2]==0xB8){
                                                         j++;
                                                                         if(str[j+2]==0x8F){
                                                                         utf16code[i] = 65039;//0xfe0f
                                                                         i++;
                                                                         j=j+2;
                                                                         }
                                                         }
                                            }
                                      }
                                            cod2++;     
                                  }
                        }
                     cod1++;
                }
  }else{ ////etc emoji
   utf16code[i] = (char16_t)str[j]; 
       i++;
    }
  }
  std::u16string u16str(utf16code);
  std::string u8str = utf16_to_utf8(u16str);
  String ret_str = String(u8str.c_str());
  ret_str.replace("+", "＋");
  ret_str.replace("&", "＆");
  ret_str.replace("\\", "-");
  ret_str.replace("/", " ");
  ret_str.replace("¨", "");
  ret_str.replace("", "");
  ret_str.replace("¸", "");
  ret_str.replace("ï", "");
  ret_str.replace("✜", "");
  ret_str.replace("✜", "");
  ret_str.replace("✏✸", "");
  return ret_str;
}
//*****************************************************
std::string utf16_to_utf8(std::u16string const& src){
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
  return converter.to_bytes(src);
}
