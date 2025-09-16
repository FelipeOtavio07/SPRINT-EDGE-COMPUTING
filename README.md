# Projeto IoT – Passa a Bola

## Integrantes  
- Felipe Otávio Garcia Madeira – RM: 563521  
- Murilo Macedo Pina – RM: 563397  
- Diego Bondezan Yonamine – RM: 562013  
- Alexandre Martins Lucas – RM: 561732  
- Vitor Carvalho Alexandre – RM: 562298  

---

## 1. Introdução  

O nosso projeto consiste em utilizar o sistema (IoT) para monitorar o desempenho de jogadoras do futebol feminino em tempo real.  
O ESP32 que utilizamos foi programado para simular dados das jogadoras, como número de jogos, gols e assistências, enviando essas informações para o Node-RED através do protocolo MQTT.  

Essa aplicação mostra como o IoT agrega valor em contextos esportivos:  
- Monitoramento em tempo real das atletas.  
- Automação da coleta de dados, sem depender de anotações manuais.  
- Análise rápida para auxiliar em estratégias e relatórios de desempenho.  

### Conceitos de IoT  
A IoT conecta objetos à internet para que possam enviar e receber informações automaticamente.  
No nosso caso:  
- O ESP32 é o dispositivo IoT.  
- O HiveMQ atua como broker MQTT.  
- O Node-RED recebe e exibe os dados em tempo real.  

### Arquitetura Proposta  
ESP32 (Dispositivo IoT) → MQTT Broker (HiveMQ) → Node-RED (Gerenciamento e Visualização)  

---

## 2. Desenvolvimento  

### Instalação da Plataforma IoT  
- O ESP32 foi programado no Wokwi (simulador online).  
- O Node-RED foi instalado no terminal do Windows (PowerShell) e configurado localmente.  

### Configuração Básica  
- Dispositivo IoT: ESP32.  
- Protocolo: MQTT.  
- Broker: HiveMQ (público).  
- Serviços: envio dos dados pelo ESP32 e recepção/monitoramento no Node-RED.  

---

## 3. Demonstração  

### Coleta de Dados  
No Wokwi, o ESP32 gera dados simulados de jogadoras (nome, jogos, gols e assistências).  
Esses dados são publicados no tópico MQTT.  

### Health Check  
Foi testada a conexão entre ESP32, HiveMQ e Node-RED.  
As mensagens aparecem no painel de debug do Node-RED em tempo real, confirmando o funcionamento do nosso sistema IoT.  

### Entidade Lógica  
Cada jogadora é tratada como uma entidade, com atributos:  
- Nome  
- Jogos  
- Gols  
- Assistências  

Isso facilita a organização dos dados e futuras análises dos scout que querem contratar as jogadoras para tal clube.  

---

## 4. Conclusão  

O nosso projeto demonstra de forma simples como o IoT pode ser aplicado no esporte para monitoramento e análise em tempo real.  
Com a integração entre ESP32, MQTT e Node-RED, foi possível criar uma solução prática que coleta, transmite e exibe dados automaticamente das jogadoras, trazendo eficiência e novas possibilidades para o acompanhamento do desempenho esportivo no geral.


Link do vídeo do nosso projeto no youtube:
https://www.youtube.com/watch?v=IR94m_3z8Ys


Representação do nosso diagrama no NODE-RED:


<img width="655" height="197" alt="image" src="https://github.com/user-attachments/assets/4a609b08-97bd-41df-bfe8-3bee15df2eb1" />




Nosso Código realizado no Wokwi (Programa utilizado para fazer a simulação).

#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

struct Jogadora {
  const char* nome;
  int jogos;
  int gols;
  int assistencias;
};

Jogadora jogadoras[] = {
  {"Ana", 0, 0, 0},
  {"Bianca", 0, 0, 0},
  {"Camila", 0, 0, 0},
  {"Daniela", 0, 0, 0},
  {"Elisa", 0, 0, 0}
};

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32FutFem")) {
      client.publish("futFem/status", "ESP32 conectado!");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  for (int i = 0; i < 5; i++) {
    jogadoras[i].jogos = random(0, 20);
    jogadoras[i].gols = random(0, 10);
    jogadoras[i].assistencias = random(0, 10);

    char msg[100];
    sprintf(msg, "%s - Jogos: %d, Gols: %d, Assistencias: %d",
            jogadoras[i].nome,
            jogadoras[i].jogos,
            jogadoras[i].gols,
            jogadoras[i].assistencias);

    Serial.println(msg);
    client.publish("futFem/jogadoras", msg);
    delay(500);
  }

  delay(2000);
}


