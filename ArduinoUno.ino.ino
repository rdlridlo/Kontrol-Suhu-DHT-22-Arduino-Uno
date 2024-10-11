
// pertama masukan lib. untuk project
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

// mendefinisikan pin yang digunakan 
#define DHTPIN 2
#define DHTTYPE DHT22
#define RELAY_PIN 12

DHR_Unified dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int setTemp = 25;
int setTempOn = 30;
bool modeSetOn = false;
bool modeHeating = true;
bool buttonsLocked = false;
bool displayHumidity = false;

unsigned long lastSwitchTime = 0;
const unsigned long switchInterval = 1000;

void setup(){
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  dht.begin();
  lcd.begin(16, 2);
  delay(100);
}

int readButtons(){
  int adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return -1;
  if (adc_key_in < 50)   return 0;
  if (adc_key_in < 250)  return 0;
  if (adc_key_in < 450)  return 0;
  if (adc_key_in < 650)  return 3;
  if (adc_key_in < 850)  return 4;
  return -1;
}

void loop(){
  sensor_event_t event;
  dht.temprature().getEvent(&event);
  float temp = event.temprature;

  sensor_event_t humidityEvent;
  dht.humidity().getEvent(&humidityEvent);
  float humidity = humidityEvent.relative_humidity;

  lcd.setCursor(0, 0);
  lcd.print("T=");
  lcd.print(temp,1);
  lcd.print("C ");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("C ");
  
  lcd.setCursor(7, 0);
  lcd.print(" RH=");
  lcd.print(humidity,1);
  lcd.print("%");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("% ");

lcd.setCursor(11, 1);
if (digitalRead(RELAY_PIN) == HIGH) {
  lcd.print("R:0");
} else {
  lcd.print("R:1");
}

lcd.setCursor(0, 1);
if (modeSetOn) {
  lcd.print("SetOn:");
  lcd.print(setTempOn);
} else {
  lcd.print("SetOff:");
  lcd.print(setTemp);
}
  lcd.print("C");

  lcd.setCursor(15, 1);
  if (modeHeating) {
    lcd.print("H");
  } else {
    lcd.print("C");
  }
  
  if (modeHeating) {
    if(temp < setTempt){
      digitalWrite(RELAY_PIN, HIGH);
    } else {
      digitalWrite(RELAY_PIN, LOW);
    }
  }
    
    int btnState = readButtons();
    if (!buttonLocked || btnState == 0){
      switch(btnState) {
        case 0:
        buttonLocked = !buttonsLocked;
        delay(200);
        break;
      case 1:
        if (modeSetOn) {
          setTempOn++;
        } else {
          setTemp++;
        }
        lcd.clear();
        delay(100);
        break;
      case 2:
        if (modeSetOn) {
          setTempOn--;
        } else {
          setTemp--;
        }
        lcd.clear();
        delay(100);
        break;
      case 3:
        modeHeating = !modeHeating;
        lcd.clear();
        delay(200);
        break;
      case 4:
        modeHeating = !modeSetOn;
        lcd.clear();
        delay(200);
        break;       
    }
  }
  delay (50);
}




