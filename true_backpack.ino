/* Design Build Destroy 2021
 *  True and the Rainbow Kingdom Backpack Cosplay Halloween costume
 *  Rotates through rainbow effect on NeoPixel Strings and plays sound clips
 *  based on button press.  
 *
 *  IMPORTANT: I used a modified DFRobotDFPlayerMini library for the boards I use (non genuine)
 *  Library: https://github.com/DesignBuildDestroy/DFRobotDFPlayerMini
 *  Adafruit NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel
 *
 */

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>  //Modified Library for knock off board! https://github.com/DesignBuildDestroy/DFRobotDFPlayerMini


//DFPlayer Serial interface
#define tx_pin 10
#define rx_pin 11

//NeoPixel's
#define chest_pin 6
#define backpack_pin 7
#define NUMPIXELS 3

//Chestpack Buttons
#define red_btn 3
#define blue_btn 4
#define yel_btn 2

//DFPlayer inits
SoftwareSerial df_SoftwareSerial(10, 11); // RX, TX reverse on each side
DFRobotDFPlayerMini df_player;

//NeoPixel inits
Adafruit_NeoPixel chest_led = Adafruit_NeoPixel(NUMPIXELS, chest_pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel backpack_led = Adafruit_NeoPixel(NUMPIXELS, backpack_pin, NEO_GRB + NEO_KHZ800);

void setup() {
  //Button Pinmodes
  pinMode(red_btn, INPUT_PULLUP);
  pinMode(blue_btn, INPUT_PULLUP);
  pinMode(yel_btn, INPUT_PULLUP);

  //Start up DFPlayer communication
  df_SoftwareSerial.begin(9600);
  df_player.begin(df_SoftwareSerial);
  delay(100);

  //Start up NeoPixel strings
  chest_led.begin();
  backpack_led.begin();
}

void loop() {
  //make the rainbows on chest and back packs rotating colors Adafruit style across 2 pixel strings
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < chest_led.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / chest_led.numPixels());
      chest_led.setPixelColor(i, chest_led.gamma32(chest_led.ColorHSV(pixelHue)));
      backpack_led.setPixelColor(i, backpack_led.gamma32(backpack_led.ColorHSV(pixelHue)));
    }
    chest_led.show();
    backpack_led.show();

    //Check if any button has been pressed on chest pack to play sound clip and change light effect
    btn_handler();
    
    //Delay between next rainbow change overrides previous light effect from buttons if any
    delay(5);
  }
}


//Create simple rotating "chase" light effect for both chest and back packs
void chase(byte r=0, byte g=0, byte b=0) {
  byte pixl[] = {0, 1, 2};
  
  for (int i = 0; i < 30; i++) {
    //Show the color rotation by way of circular rotating array containing the string pixel position
    //Light so that of the 3 LED's one is full bright, next is half, last is off, and then rotate them
    //between the so that it makes the appearnace that the lights are chasing them
    chest_led.setPixelColor(pixl[0], r, g, b);
    chest_led.setPixelColor(pixl[1], 0, 0, 0);
    chest_led.setPixelColor(pixl[2], r/2, g/2, b/2);
    //Do the same on the chest backpack
    backpack_led.setPixelColor(pixl[0], r, g, b);
    backpack_led.setPixelColor(pixl[1], 0, 0, 0);
    backpack_led.setPixelColor(pixl[2], r/2, g/2, b/2);
    chest_led.setPixelColor(pixl[0],r,g,b);
    chest_led.setPixelColor(pixl[1],0,0,0);
    chest_led.setPixelColor(pixl[2],r/3, g/3, b/3);

    backpack_led.setPixelColor(pixl[0],r,b,g);
    backpack_led.setPixelColor(pixl[1],0,0,0);
    backpack_led.setPixelColor(pixl[2],r/3,g/3,b/3);
    
    chest_led.show();
    backpack_led.show();
    
    //Circular shift the array
    byte tmp_pixl = pixl[0];
    for (byte i = 0 ; i < 2; i++) pixl[i] = pixl[i + 1];
    pixl[2] = tmp_pixl;

    delay(100);
  }
}

//Make the led's rotate the button color while the mp3 plays
//This gives enough time for the mp3 to fully play and acts as a button debounce
//Since this can't be called again until after the led loop is complete
//approx 20 second to roll through 
void btn_handler() {
  //Lazy method because this is just a halloween costume and not critical
  bool blocking = false;
  
  if (!digitalRead(red_btn)) {
    //Randomize an mp3, skipping 1 and 2 which are the other buttons
    //Play the mp3 sound for the randomized number
    //Files 3-6, this randomizes number from 3-6, 7 will never be chosen
    df_player.play(random(3,7));
    //Chase and debounce
    chase(255,0,0); //Red
  }
  if (!digitalRead(blue_btn)) {
    df_player.play(2);
    //Chase and debounce
    chase(0,0,255); //Blue
  }
  if (!digitalRead(yel_btn)) {
    df_player.play(1);
    //Chase and debounce
    chase(255,255,0); //Yellow
  }
}
