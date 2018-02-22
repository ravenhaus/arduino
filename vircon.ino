
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


int LED_RED = 11;
int LED_GREEN = 12;

int brightness = 0;
int fadeAmount = 5;

int PIN_LATCH = 3;
int PIN_CLOCK = 4;
int PIN_DATA = 2;

long iteration = 0;
int error_flag = 1;
int screen_flag = 1;

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);
  
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();

  display.clearDisplay();
}

void loop() {
    delay(10);
    iteration++;

    if ((iteration * 10) % (random(18, 25) * 10) == 0) {
        if (screen_flag == 1) {
            print_garbage();
        } else {
            print_bars();          
        }
        screen_flag = -screen_flag;
        display.display();
        display.clearDisplay();
    }

    if ((iteration * 10) % 360 == 0) {
        writeError(error_flag);
        error_flag = -error_flag;
    }

    if ((iteration * 10) % 30 == 0) {
        if (brightness < fadeAmount) {
            analogWrite(LED_RED, 5);
        } else {
            analogWrite(LED_RED, brightness);
        }
      
        brightness = brightness + fadeAmount;
        if (brightness <= 0 || brightness >= 255) {
          fadeAmount = -fadeAmount;
        }
        
        if (random(100) < 5) {
          digitalWrite(LED_GREEN, LOW);
        } else {
          digitalWrite(LED_GREEN, HIGH);
        }
    }

}

void writeError(int flag) {
  digitalWrite(PIN_LATCH, LOW);
  if (flag == -1) {
    shiftOut(PIN_DATA, PIN_CLOCK, LSBFIRST, B00000001);
  } else {
    shiftOut(PIN_DATA, PIN_CLOCK, LSBFIRST, B10011110);      
  }
  digitalWrite(PIN_LATCH, HIGH);  
}

void print_bars(void) {
  for (int16_t i=0; i<display.width(); i+=3) {
    display.drawLine(i, random(0, 22), i, 32, WHITE);
  }
  display.display();
  delay(1);
}

void print_garbage(void) {
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(random(168));
  }    

  display.display();
  delay(1);
}
