#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <TM1638plus.h>

// TM1638 setup
#define STROBE_TM 4     // strobe = GPIO connected to strobe line of module
#define CLOCK_TM 5      // clock = GPIO connected to clock line of module
#define DIO_TM 7        // data = GPIO connected to data line of module
bool high_freq = false; // default false,, If using a high freq CPU > ~100 MHZ set to true.
TM1638plus tm(STROBE_TM, CLOCK_TM, DIO_TM, high_freq);

struct DataPacket
{
  float rpm;
  float gear;
  float spd;
  float max_rpm;
};

#define BUFFER_SIZE 16 // 4 * 4 floats

// WS2812 setup
const int ledPin = 2;
const int numLeds = 8; 
const int delta = 256 / numLeds;
int lastNum = 0;
const int fps = 16;
#define RED_ZONE 8
#define YELLOW_ZONE 6
#define GREEN_ZONE 4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

float rpm;
uint16_t last_gear;
float led;
int max_rpm = 7000;

void tachometer(int rpm);
uint32_t getColor(int ledPos);

void setup()
{
  strip.begin();
  strip.setBrightness(20); // 1/3 brightness
  Serial.begin(115200);
  tm.displayBegin();
}

void loop()
{
  DataPacket data;

  if (Serial.available() >= BUFFER_SIZE)
  {
    uint8_t buffer[BUFFER_SIZE];

    Serial.readBytes(buffer, BUFFER_SIZE);

    memcpy(&data, buffer, BUFFER_SIZE);

    float toDisplay = int(data.rpm) + int(data.spd) * 100000;
    tm.displayIntNum(toDisplay, true, TMAlignTextRight); 
    
    uint16_t gear = uint16_t(data.gear);
    if (last_gear != gear)
    {
      tm.setLEDs(gear);
      last_gear = gear;
    }

    max_rpm = int(data.max_rpm) - 500;
    float k = (((data.rpm - max_rpm * 0.8) * numLeds) / (max_rpm * 0.2)) + 0;
    tachometer(int(k));


    while (Serial.read() > 0)
    {
    }
  }
}

void tachometer(int rpm)
{
  if (rpm < 0)
  {
    return;
  }
  if (rpm > numLeds)
  {
    return;
  }
  if (rpm < lastNum)
  {
    for (int x = strip.numPixels() - 1; x >= 0; x--)
    {
      if (x < rpm)
      {
        strip.setPixelColor(x, getColor(x));
      }
      else
      {
        strip.setPixelColor(x, 0);
      }
      strip.show();
    }
  }
  else
  {
    for (uint8_t x = 0; x < strip.numPixels(); x++)
    {
      if (x < rpm)
      {
        strip.setPixelColor(x, getColor(x));
      }
      else
      {
        strip.setPixelColor(x, 0);
      }
      strip.show();
    }
  }
  lastNum = rpm;
}

uint32_t getColor(int ledPos)
{
  
  if (ledPos < GREEN_ZONE)
  {
    return strip.Color(0, 255, 0); // green
  }
  else if (ledPos >= GREEN_ZONE && ledPos < YELLOW_ZONE)
  {
    return strip.Color(255, 255, 0); // yellow
  }
  else
  {
    return strip.Color(255, 0, 0); // red
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
