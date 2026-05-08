#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// --- Configuración ---
const char* ssid = "SilvaRomero";
const char* password = "2014.AmbaR!2020.AurelianO?";

Servo miServo;
const int pinServo = 12; 
const int FRENO = 72; 
const long INTERVALO_ESPERA = 5000;

unsigned long ultimoTiempoFinalizado = 0;
int contadorPorciones = 0;

// Prototipo de función (Requerido en C++ puro / PIO)
void dispensarComida();

void setup() {
    Serial.begin(115200);

    // Configuración específica de ESP32Servo
    ESP32PWM::allocateTimer(0);
    miServo.setPeriodHertz(50);
    miServo.attach(pinServo, 500, 2400);
    
    miServo.write(FRENO);

    // Intento de conexión WiFi
    WiFi.begin(ssid, password);
    
    // Espera máxima de 5 segundos para no bloquear el inicio
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 5000) {
        delay(500);
    }

}

void loop() {
    unsigned long tiempoActual = millis();

    if (tiempoActual - ultimoTiempoFinalizado >= INTERVALO_ESPERA) {

        dispensarComida();
        
        ultimoTiempoFinalizado = millis();
    }
}

void dispensarComida() {
    miServo.write(180); 
    delay(5000); 
    
    miServo.write(FRENO);
}