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
