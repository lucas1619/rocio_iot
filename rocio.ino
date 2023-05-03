#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char* ssid = "MOVISTAR_04C8"; // Nombre de la red Wi-Fi
const char* password = "m4CHT4K4CW7HB7RG9W7Y"; // Contraseña de la red Wi-Fi

WebSocketsClient webSocket;
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 2000; // En milisegundos

int getHumidity() {
  int humidity = analogRead(A0);
  int humidityValue = map(humidity, 4095, 0, 0, 100);
  return humidityValue; // Devolver el valor de humedad
}
void sendMeasurements() {
  // Crear el objeto JSON
  DynamicJsonDocument jsonDoc(256); // Tamaño máximo del JSON
  JsonObject jsonData = jsonDoc.createNestedObject("data");
  jsonData["humidity"] = getHumidity();
  jsonDoc["event"] = "measureFromIot";

  // Convertir el objeto JSON en una cadena
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Enviar los datos al servidor
  webSocket.sendTXT(jsonString);
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      break;
    case WStype_CONNECTED:
      break;
    case WStype_TEXT:
      break;
  }
}
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Configurar la conexión WebSocket
  webSocket.begin("52.36.218.139", 80, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);

  lastSendTime = millis(); // Inicializar el temporizador
  
}

void loop() {
  webSocket.loop();
  // Enviar los datos cada 2 segundos
  if (millis() - lastSendTime >= sendInterval) {
    sendMeasurements();
    lastSendTime = millis();
  }

}
