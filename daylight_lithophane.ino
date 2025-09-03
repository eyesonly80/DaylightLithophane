#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define LEDLIGHT_PIN    2

// How many NeoPixels are attached to the Arduino?
#define LEDLIGHT_COUNT 12

int lightBrightness = 0;

// Declare our NeoPixel objects:
Adafruit_NeoPixel stripLight(LEDLIGHT_COUNT, LEDLIGHT_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


//Smoothing of the readings from the light sensor so it is not too twitchy
const int numReadings = 12;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
long total = 0;                  // the running total
long average = 0;                // the average



void setup() {

  Serial.begin(9600);

  stripLight.begin();           // INITIALIZE NeoPixel stripClock object (REQUIRED)
  stripLight.show();            // Turn OFF all pixels ASAP
  stripLight.setBrightness(100); // Set inital BRIGHTNESS (max = 255)

  //smoothing
    // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  
}

void loop() {
  //Record a reading from the light sensor and add it to the array
  readings[readIndex] = analogRead(A7); //get an average light level from previouse set of samples
  Serial.print("Light sensor value added to array = ");
  Serial.println(readings[readIndex]);
  readIndex = readIndex + 1; // advance to the next position in the array:

  // if we're at the end of the array move the index back around...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  //now work out the sum of all the values in the array
  int sumBrightness = 0;
  for (int i=0; i < numReadings; i++) {
    sumBrightness += readings[i];
  }
  Serial.print("Sum of the brightness array = ");
  Serial.println(sumBrightness);

  // and calculate the average: 
  int lightSensorValue = sumBrightness / numReadings;
  Serial.print("Average light sensor value = ");
  Serial.println(lightSensorValue);


  //set the brightness based on ambiant light levels
  lightBrightness = map(constrain(lightSensorValue, 200, 700), 200, 700, 255, 50); 
  stripLight.setBrightness(lightBrightness); // Set brightness value of the LEDs
  Serial.print("Mapped brightness value = ");
  Serial.println(lightBrightness);
  
  //(red * 65536) + (green * 256) + blue ->for 32-bit merged colour value so 16777215 equals white
  stripLight.fill(0xFFFFFF, 0, LEDLIGHT_COUNT);
  stripLight.show();

  delay(5000);   //this 5 second delay to slow things down during testing

}


