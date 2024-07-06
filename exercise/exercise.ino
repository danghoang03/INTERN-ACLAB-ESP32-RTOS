#include "LiquidCrystal_I2C.h";
#include "DHT20.h"

// Define your tasks here
void TaskBlink(void *pvParameters);
void TaskTemperatureHumidity(void *pvParameters);

//Define your components here
LiquidCrystal_I2C lcd(0x21, 16, 2);
DHT20 DHT;


void setup() {

  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  xTaskCreate( TaskBlink, "Task Blink" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskTemperatureHumidity, "Task Temperature" ,2048  ,NULL  ,2 , NULL);
  
  //Now the task scheduler is automatically started.
  Serial.printf("Basic Multi Threading Arduino Example\n");
  
}

void loop() {
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/



void TaskBlink(void *pvParameters) {  // This is a task.
  //uint32_t blink_delay = *((uint32_t *)pvParameters);

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  

  while(1) {                          
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED ON
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED OFF
    delay(200);
  }
}


void TaskTemperatureHumidity(void *pvParameters) {  // This is a task.
  //uint32_t blink_delay = *((uint32_t *)pvParameters);
  lcd.init();
  lcd.backlight();

  while(1) {                          
    DHT.read();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nhiet Do: ");
    lcd.print(DHT.getTemperature());
    lcd.setCursor(0, 1);
    lcd.print("Do Am: ");
    lcd.print(DHT.getHumidity());
    delay(5000);
  }
}
