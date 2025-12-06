/* tecnologias jpc
 *  programa para usar MPU9250 para generar un horizonte artificial (indicador de actitud)
 *  circuito https://drive.google.com/file/d/1qnulgTT39ALBK67G_OotjPK9X4BIcdNE/view?usp=sharing
 *  compatible con tarjeta Arduino Shield, adquierela en http://www.tecnologiasjpc.com/
*/

#define SCREEN_WIDTH    128        // ancho de pantalla OLED en pixeles
#define SCREEN_HEIGHT   64         // alto de pantalla OLED en pixeles  
#define OLED128         0x3C       // direccion para pantalla OLED

#include <Wire.h>                 //libreria para comunicacion I2C
#include <Adafruit_SSD1306.h>     //libreria para OLED
#include <Adafruit_GFX.h>         //libreria para graficar en pantalla OLED
#include <FaBo9Axis_MPU9250.h>    //libreria para usar MPU9250

// crea el objeto para la pantalla OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// crea el objeto para el acelerometro
FaBo9Axis acelerometro;

void setup() {
  Serial.begin(9600);                     // inicializa la comunicación serial
  // inicializa la pantalla OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED128)) {
    Serial.println(F("Error al iniciar OLED. Verifique conexiones"));
    while (1);
  }
  display.clearDisplay();                 // comando para limpiar la pantalla OLED
  // inicializa el sensor MPU9250
  if (!acelerometro.begin()) {
    Serial.println("Error al iniciar MPU. Verifique conexiones");
    while(1);
  }
}

void loop() {
  float ax,ay,az;                           //guarda los valores de aceleración en cada eje
  acelerometro.readAccelXYZ(&ax,&ay,&az);     //obtiene los valores de aceleración en cada eje
  float ang = atan((ay*100)/(ax*100));      //obtiene el angulo de aceleración sobre plano xy
  dibujar_linea(ang + (PI/2));              //dibuja una linea perpendicular a la aceleración
}

void dibujar_linea(float angulo){
  int co = (display.width()/2)*tan(angulo);    //obtiene el cateto opuesto al angulo recibido
  display.clearDisplay();                   // comando para limpiar la pantalla OLED
  //dibuja una linea tomando como cateto adyacente la mitad del largo de pantalla OLED
  //y como cateto opuesto el obtenido a partir del angulo medido
  display.drawLine(0, display.height()/2+co, display.width()-1, display.height()/2-co, SSD1306_WHITE);
  //dibuja un circulo en el centro de la pantalla OLED
  display.drawCircle(display.width()/2, display.height()/2, 5, SSD1306_WHITE);
  display.display();                      // actualiza los graficos mostrados en pantalla OLED
  delay(1);
}
