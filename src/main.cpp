// Uso de blynk wifi por medio de un boton en la app, para activar un servo que dispensa comida a una mascota. El servo se detiene automáticamente después de 5 segundos.
#define BLYNK_PRINT Serial 
#define BLYNK_TEMPLATE_ID "TU ID"
#define BLYNK_TEMPLATE_NAME "NOMBRE DASHBOARD"
#define BLYNK_AUTH_TOKEN "TOKEN"
// Asegúrate de reemplazar los valores anteriores con los de tu proyecto en Blynk.

// Librerías necesarias
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// Configuración de WiFi
char ssid[] = "NOMBRE RED";
char pass[] = "CONTRASEÑA";

// Configuración del servo
Servo miServo;
BlynkTimer timer; 
// Configuración del pin del servo y posición de frenado
const int pinServo = 12; 
const int FRENO = 72; 
bool dispensando ;

// Función para detener el servo
void detenerServo() {
    miServo.write(FRENO);
    dispensando = false;
    Blynk.virtualWrite(V1, 0); // Apaga el botón en la App automáticamente
    Serial.println("Dispensado terminado.");
}

// Función para dispensar comida
void dispensarComida() {
   // Evita que se active varias veces al mismo tiempo
        // dispensando = true;
        miServo.write(180); 
        Serial.println("Dispensando...");
        
        timer.setTimeout(5000L, detenerServo);
    
}

// Función que se ejecuta cuando se presiona el botón en la App
BLYNK_WRITE(V1) {
    int valor = param.asInt();
    Serial.print("Señal recibida: ");
    Serial.println(valor);

    if (valor > 0) { // Ahora funcionará si mandas 1, 255 o cualquier número positivo
        dispensarComida();
    }
}

// Configuración inicial
void setup() {
    Serial.begin(115200);
    delay(1000); 
    
    Serial.println("Iniciando WiFi...");
    WiFi.begin(ssid, pass);

    // Esperar máximo 10 segundos
    int contador = 0;
    while (WiFi.status() != WL_CONNECTED && contador < 20) {
        delay(500);
        Serial.print(".");
        contador++;
    }
    // Verificar si se conectó al WiFi
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n¡WiFi Conectado!");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nError: No se pudo conectar al WiFi.");
    }

    // Luego intentas Blynk
    Blynk.config(BLYNK_AUTH_TOKEN);
    Blynk.connect();
}

// Bucle principal
void loop() {
    Blynk.run();
    timer.run(); // Necesario para que funcionen los timers
}