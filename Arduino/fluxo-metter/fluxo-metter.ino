/**
 * FLUXO-METTER
 * UIAJC 2017-01
 */
#include "fluxometter.h"

WiFiClient wifi;
PubSubClient mqtt(wifi);

// ejecta tareas sin usar delays()
long lastReconnectAttempt = 0;
unsigned long previousMillis = 0;
const long interval = 1000;

// Estdos de control del device
bool deviceOnOff = false;

void setup() {
  
  configurarSerial();
  configurarDisplay();
  configurarWiFi();
  configurarBoard();
  
  mqtt.setServer(MQTT_SERVER, MQTT_SERVERPORT);
  mqtt.setCallback(callback);

  lastReconnectAttempt = 0;
  previousMillis = millis();
}

void loop() {
  if (!mqtt.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 20000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // Client connected
    mqtt.loop();
  }
  
  // workload 
  if (deviceOnOff) {
    unsigned long currentMillis = millis();
    // hacer las tareas de control
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      sensorProcesar();
      blink();
    }
  } else {
    
  }
  
}

boolean reconnect() {
  if (mqtt.connect(MQTT_DEVICENAME)) {
    Serial.println("Conectado con MQTT Server: "MQTT_SERVER);

    // settear el dipositivo en el servidor    
    mqtt.publish("uniajc/fluxo-metter/device/onOff","OFF");
    mqtt.publish("uniajc/fluxo-metter/device/volume","0");
    mqtt.publish("uniajc/fluxo-metter/device/value","0");
    // ... and resubscribe
    mqtt.subscribe("uniajc/fluxo-metter/app/#");
  } else {
    Serial.println("No hay conexión MQTT Server: "MQTT_SERVER);    
  }
  return mqtt.connected();
}

void deviceStart() {
  mqtt.publish("uniajc/fluxo-metter/device/onOff","ON");
  deviceOnOff = true;
  sensorStart();
  Serial.println("Device Start");
  LCDClear();
  gotoXY(0, 2);
  LCDString("EN OPERACION");
}

void deviceStop() {
  mqtt.publish("uniajc/fluxo-metter/device/onOff", "OFF");
  deviceOnOff = false;
  sensorStop();
  
  digitalWrite(PIN_LED, HIGH);
  Serial.println("Device Stop");
  LCDClear();
  gotoXY(0, 2);
  LCDString(" EN  ESPERA ");
}

// Rutina que se ejecuta cuando recibimos datos
void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  String path = String(topic);
  
  if (path == "uniajc/fluxo-metter/app/onOff") {
    if (CompareString("ON", payload, length)) {
      deviceStart();
    }
    else {
      deviceStop();
    }
  }  
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void mqttEnviarMensaje(char* topic, char* message) {
   mqtt.publish(topic, message);
}

