#include "fluxometter.h"


int numero_linea = 0;
volatile int  flow_frequency;  // Measures flow meter pulses
unsigned int  l_hour;          // Calculated litres/hour
int ledOnOff = LOW;
char buf[5];

// valores
unsigned int cantidad; 
const int conversion = 400;
 
void configurarWiFi() {
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  /*
  IPAddress ip(192, 168, 0, 177); 
  IPAddress dns(172, 16, 3, 3);
  IPAddress gw(10, 10, 16, 2);
  IPAddress mask(255, 255, 240, 0);   
  WiFi.config(ip, dns, gw, mask); //, "172.16.3.3", "10.10.16.2", "255.255.240.0");
  */
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  
}

void configurarSerial() {
   Serial.begin(115200);
   delay(10);
   Serial.println("FLUXO-METTER");    
}

void configurarDisplay() {
   LCDInit();
   LCDClear();  
   gotoXY(0, 2);
   LCDString(" EN  ESPERA ");         
}

void configurarBoard() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(FLOW_IN, INPUT);
  digitalWrite(PIN_LED, HIGH);
}


bool CompareString(String myString, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
    if (myString[i] != (char)payload[i])
      return false;
  }
  return true;  
}

// rutina para sctualizar el sensor de flujo
void flow ()                  // Interruot function
{ 
   flow_frequency++;
}

void sensorStart() {
  attachInterrupt(digitalPinToInterrupt(FLOW_IN), flow, RISING); // Setup Interrupt 
  sei();    
  cantidad = 0;
}

void sensorStop() {
  detachInterrupt(digitalPinToInterrupt(FLOW_IN));
}

void blink() {
    if (ledOnOff == LOW) {
      ledOnOff = HIGH;
    } else {
      ledOnOff = LOW;
    }
    digitalWrite(PIN_LED, ledOnOff); 
}

void sensorProcesar() {
  // 400 pulsos = litro
  cantidad = cantidad + flow_frequency;
  char str[16];
  sprintf(str, "%d", cantidad);
  mqttEnviarMensaje("uniajc/fluxo-metter/device/volume", str);
  
  l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour 
  flow_frequency = 0;                   // Reset Counter

  itoa(l_hour, buf, 5);
  gotoXY(0, 4);
  LCDString("            ");
  gotoXY(0, 4);
  LCDString(buf);
  gotoXY(0, 5);
  LCDString("Litros/Hora");

  Serial.print(l_hour, DEC);            // Print litres/hour
  Serial.println(" L/hour");  
}

