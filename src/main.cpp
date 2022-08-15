// #include <Arduino.h>

// void setup() {
//   // put your setup code here, to run once:
//   pinMode(2, OUTPUT);
// }

// void loop() {
//   digitalWrite(2, HIGH);
//   delay(100);
//   digitalWrite(2, LOW);
//   delay(100);
//   // put your main code here, to run repeatedly:
// }

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define FECHADURA 12
#define BOTAO 23

const char *SSID = "Familia2016";
const char *PWD = "09091967";

bool statusFechadura = true;
//0- sem uso 1- botao pressionado 
int flagControle = 0;

// const char *SSID = "Nicolas";
// const char *PWD = "12345678";
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



IPAddress ip(192,168,1,16); //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR. EX: 192.168.1.110 **** ISSO VARIA, NO MEU CASO É: 192.168.0.175
IPAddress gateway(192,168,1,1); //GATEWAY DE CONEXÃO (ALTERE PARA O GATEWAY DO SEU ROTEADOR)
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
  WiFi.config(ip, gateway, subnet); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI SETAR O IP FIXO NO NODEMCU
 
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