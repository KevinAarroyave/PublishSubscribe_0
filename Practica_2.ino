//Incluir librerias para enviar datos a la nube
#include <UbiConstants.h>
#include <UbiTypes.h>
#include <UbidotsEsp32Mqtt.h>
//Incluir librerías del TTGO
#include <TFT_eSPI.h>
#include <SPI.h>
//Incluir librerías del MPU6050
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
TFT_eSPI tft = TFT_eSPI(); // Invocar la librería del TTGO
Adafruit_MPU6050 mpu = Adafruit_MPU6050(); //Invocar librería del MPU6050
//Definir macros para los colores
#define TFT_BLACK 0x0000 /* 0, 0, 0 */
#define TFT_NAVY 0x000F /* 0, 0, 128 */
#define TFT_DARKGREEN 0x03E0 /* 0, 128, 0 */
#define TFT_DARKCYAN 0x03EF /* 0, 128, 128 */
#define TFT_MAROON 0x7800 /* 128, 0, 0 */
#define TFT_PURPLE 0x780F /* 128, 0, 128 */
#define TFT_OLIVE 0x7BE0 /* 128, 128, 0 */
#define TFT_LIGHTGREY 0xD69A /* 211, 211, 211 */
#define TFT_DARKGREY 0x7BEF /* 128, 128, 128 */
#define TFT_BLUE 0x001F /* 0, 0, 255 */
#define TFT_GREEN 0x07E0 /* 0, 255, 0 */
#define TFT_CYAN 0x07FF /* 0, 255, 255 */
#define TFT_RED 0xF800 /* 255, 0, 0 */
#define TFT_MAGENTA 0xF81F /* 255, 0, 255 */
#define TFT_YELLOW 0xFFE0 /* 255, 255, 0 */
#define TFT_WHITE 0xFFFF /* 255, 255, 255 */
#define TFT_ORANGE 0xFDA0 /* 255, 180, 0 */
#define TFT_GREENYELLOW 0xB7E0 /* 180, 255, 0 */
#define TFT_PINK 0xFE19 /* 255, 192, 203 */ //Lighter pink, was 0xFC9F 
#define TFT_BROWN 0x9A60 /* 150, 75, 0 */
#define TFT_GOLD 0xFEA0 /* 255, 215, 0 */
#define TFT_SILVER 0xC618 /* 192, 192, 192 */
#define TFT_SKYBLUE 0x867D /* 135, 206, 235 */
#define TFT_VIOLET 0x915C /* 180, 46, 226 */
const char *UBIDOTS_TOKEN = "BBUS-jJBSm7aUTrG4VUYbS1lvuOm6LAAdz1"; // Put here your Ubidots TOKEN
const char *WIFI_SSID = "Arroyave"; // Put here your Wi-Fi SSID
const char *WIFI_PASS = "3042486753"; // Put here your Wi-Fi password
const char *PUBLISH_DEVICE_LABEL = "TTGESP32"; // Put here your Device label to which data will be published
const char *PUBLISH_VARIABLE_LABEL_1 = "Rx"; // Put here your Variable label to which data will be published
const char *PUBLISH_VARIABLE_LABEL_2 = "Ry"; // Put here your Variable label to which data will be published
const char *PUBLISH_VARIABLE_LABEL_3 = "T"; // Put here your Variable label to which data will be published
const char *SUBSCRIBE_DEVICE_LABEL = "TTGESP32"; // Put here your Device label to which data will be published
const char *SUBSCRIBE_VARIABLE_LABEL = "SW1"; // Put here your Variable label to which data will be published

const uint8_t LED = 27; // Pin used to write data based on 1's and 0's coming from Ubidots
bool POINT = 0; 

float gx, gy, temp, value_1, value_2, value_3; //Definir Variables en las que se capturarán las medidas.

Ubidots ubidots(UBIDOTS_TOKEN);
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    if ((char)payload[0] == '1')
    {
      
      digitalWrite(LED, HIGH);
      POINT = true;
    }
    else
    {

      digitalWrite(LED, LOW);
      POINT = false;
    }
  }
  Serial.println();
}

void setup() {
 
 Serial.begin(115200); //Inicializar puerto serial
 pinMode(LED, OUTPUT);
 pinMode(POINT, INPUT);
 tft.init(); //Inicializar la librería
 tft.setRotation(1); //Rotar los caracterees en el display
 mpu.begin(); // Inicializar el MPU6050
 mpu.setAccelerometerRange(MPU6050_RANGE_8_G); //Selecionar rango de acelerometro del MPU
 mpu.setGyroRange(MPU6050_RANGE_500_DEG); //Selecionar rango de giroscopio del MPU
 mpu.setFilterBandwidth(MPU6050_BAND_21_HZ); //Elegir un hancho de bnada para el filtro de la librería
 //Inicialización del modulo de publicación de Ubidot
 
 ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
 ubidots.setCallback(callback);
 ubidots.setup();
 ubidots.reconnect();

}
void loop() {
 
 tft.fillScreen(TFT_BLACK); //Definir un color de fondo por la pantalla
 tft.setCursor(0, 0, 2); // Ubicar el cursor en una posición inicial
 tft.setTextColor(TFT_WHITE); //Elegir un color para la letra
 tft.setTextSize(2); //Definir un tamaño para la letra
 tft.println("Hola!"); //Imprimir texto
 
 sensors_event_t a, g, t;//Definir variables para obtener medidas
 mpu.getEvent(&a, &g, &t); //Obtener medidas
 //tft.setTextSize(1);
 
 
 tft.print("Rx = "), tft.print(g.gyro.x), tft.println(" rad/s "); // Mostrar velocidad angular en el eje x a través del display del TTGO 
 tft.print("Ry = "), tft.print(g.gyro.y), tft.println(" rad/s"); // Mostrar velocidad angular en el eje y a través del display del TTGO
 tft.print("T = "), tft.print(t.temperature), tft.println(" °C"); //Mostrar temperatura medida por el MPU
 
 gx = g.gyro.x;
 gy = g.gyro.y;
 temp = t.temperature;
 //Envío de datos a la nube
 if (!ubidots.connected())
 {
 ubidots.reconnect();
 }
 //if (timer == PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
 //{
 
 value_1 = gx;
 value_2 = gy;
 value_3 = temp;
 ubidots.add(PUBLISH_VARIABLE_LABEL_1, value_1); // Insert your variable Labels and the value to be sent
 ubidots.add(PUBLISH_VARIABLE_LABEL_2, value_2); // Insert your variable Labels and the value to be sent
 ubidots.add(PUBLISH_VARIABLE_LABEL_3, value_3); // Insert your variable Labels and the value to be sent
 ubidots.publish(PUBLISH_DEVICE_LABEL);
 ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL, SUBSCRIBE_VARIABLE_LABEL);
 if(POINT){
  tft.fillCircle(80, 15, 10,TFT_GREEN);
 } else{
  tft.fillCircle(80, 15, 10,TFT_DARKGREY);
 }
 
 //}
 ubidots.loop();
 delay(5000);
}
