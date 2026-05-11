#define BLYNK_PRINT Serial 
#define BLYNK_TEMPLATE_ID "TMPL2rSkJhaZ9"
#define BLYNK_TEMPLATE_NAME "dispensador "
#define BLYNK_AUTH_TOKEN "o9Fb57WdJ1RVxCyDz6xE8ghN_0JPYM-F"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

char ssid[] = "SilvaRomero";
char pass[] = "2014.AmbaR!2020.AurelianO?";

Servo miServo;
BlynkTimer timer; 

const int pinServo = 12; 
const int FRENO = 72; 
bool dispensando = false;

// Función para detener el servo
void detenerServo() {
    miServo.write(FRENO);
    dispensando = false;
    Blynk.virtualWrite(V1, 0); // Apaga el botón en la App automáticamente
    Serial.println("Dispensado terminado.");
}

void dispensarComida() {
    if (!dispensando) { // Evita que se active varias veces al mismo tiempo
        dispensando = true;
        miServo.write(180); 
        Serial.println("Dispensando...");
        
        timer.setTimeout(5000L, detenerServo);
    }
}

BLYNK_WRITE(V1) {
    if (param.asInt() == 1) {
        dispensarComida();
    }
}

void setup() {
    Serial.begin(115200);
    
    ESP32PWM::allocateTimer(0);
    miServo.setPeriodHertz(50);
    miServo.attach(pinServo, 500, 2400);
    miServo.write(FRENO);

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
    Blynk.run();
    timer.run(); // Necesario para que funcionen los timers
}