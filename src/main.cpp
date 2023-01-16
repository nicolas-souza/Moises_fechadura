
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define FECHADURA 12
#define BOTAO 23

const char *SSID = "";
const char *PWD = "";

bool statusFechadura = true;
int flagControle = 0;


WebServer server(80);
 
char buffer[250];

void setAbrir() {
  statusFechadura = LOW;
  digitalWrite(FECHADURA, statusFechadura);
  server.send(200, "application/json", buffer);
}
 
void setFechar() {
  statusFechadura = HIGH;
  digitalWrite(FECHADURA, statusFechadura);
  server.send(200, "application/json", buffer);
}

void setup_routing() {

  server.on("/abrir", setAbrir);     
     
  server.on("/fechar", setFechar);       
          
  server.begin();    

}

IPAddress ip(192,168,1,16); 
IPAddress gateway(192,168,1,1); 
IPAddress subnet(255,255,255,0);

void verificar(){  
  
  bool botao;  
  botao = digitalRead(BOTAO);

  if(!botao){
    flagControle = 1;
  }else {
    if (flagControle == 1){
      statusFechadura = !statusFechadura;
      digitalWrite(FECHADURA, statusFechadura);
      flagControle = 0;
    }
  }
}

void setup() {    

  pinMode(FECHADURA,OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP);

  Serial.begin(115200);
         
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(SSID, PWD);
  WiFi.config(ip, gateway, subnet); 
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");    
  }
 
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());
  setup_routing();     
  

}    
       
void loop() {    
  server.handleClient();     
  verificar();
}