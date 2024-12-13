#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 41 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led azul

const int pino_botao = 18;  // Pino utilizado para o botão
int estado_botao = 0;  // Variável para ler o estado do Botão

const int pino_ldr = 7;  //Pino utilizado para o ldr
int estado_ldr = 0;  //Variável para ler o estado do ldr
int limiar=600; 

unsigned long tempo_inicio = 0;
unsigned long tempo = 0;
unsigned long tempo_delay = 0;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(pino_botao, INPUT_PULLUP); // Inicializa o pino botão como entrada pull_up

  pinMode(pino_ldr, INPUT); // Inicializa o pino botão como entrada

  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int resposta_http = http.GET(); // Código do Resultado da Requisição HTTP

    if (resposta_http>0) {
      Serial.print("HTTP Código de Resposta: ");
      Serial.println(resposta_http);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Código de Erro: ");
      Serial.println(resposta_http);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Desconectado");
  }
}

void loop() {
  
  tempo_inicio = millis();
  estado_botao = digitalRead(pino_botao);

  if (estado_botao == HIGH && millis() - tempo_inicio >= 5000) {
    Serial.println("Botão pressionado!");
    tempo_inicio = 0;
  } else {
    Serial.println("Botão não pressionado!");
  }

  estado_ldr=analogRead(pino_ldr);

  if(estado_ldr<=limiar){
    tempo = millis();
    Serial.print("Modo escuro ativado.");
    Serial.println(estado_ldr);
    if(tempo - tempo_delay>=1000){
      digitalWrite(led_amarelo, HIGH);
    } 

    if (tempo -tempo_delay>=2000){
       digitalWrite(led_amarelo, LOW);

       tempo_delay = tempo;
    }

  }else{
    tempo = millis();
    Serial.print("Modo claro ativado.");
    Serial.println(estado_ldr);
    if(tempo - tempo_delay>=0){
      digitalWrite(led_verde, HIGH);
      digitalWrite(led_vermelho, LOW);
    } 

    if (tempo -tempo_delay>=3000){
       digitalWrite(led_verde, LOW);
       digitalWrite(led_amarelo, HIGH);

    }

    if (tempo -tempo_delay>=5000){
       Serial.print(led_amarelo);
       digitalWrite(led_vermelho, HIGH);
       digitalWrite(led_amarelo, LOW);

    }

    if(tempo - tempo_delay>=1000){
      tempo_delay = tempo;
    }
}
}