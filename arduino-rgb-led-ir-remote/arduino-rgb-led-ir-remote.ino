#include "IRremote.h"

typedef enum {
  ONLY_RED,
  ONLY_GREEN,
  ONLY_BLUE,
  ONLY_YELLOW,
  ONLY_CYAN,
  ONLY_MAGENTA,
  ONLY_WHITE,
  FADING
} rgb_led_mode_t;

const int led_change_intensity = 32;
int pin_receiver = 11;
int pin_red_led = 10;
int pin_green_led = 6;
int pin_blue_led = 9;
int red_value = 128;
int green_value = 128;
int blue_value = 128;

bool is_led_red_on = true;
bool is_led_green_on = true;
bool is_led_blue_on = true;

rgb_led_mode_t led_state = ONLY_RED;

IRrecv irrecv(pin_receiver);
decode_results results;

uint8_t calculate_brightness_limits(int led_value, int intensity_change) {
  uint8_t result = 0;
  int temp_intensity = led_value + intensity_change;

  if (temp_intensity < 0) {
    result = 0;
  } else if (temp_intensity > 255) {
    result = 255;
  } else {
    result = temp_intensity;
  }

  return result;
}
void change_brightness(int intensity_change) {
  if (is_led_red_on == true) {
    red_value = calculate_brightness_limits(red_value, intensity_change);
  }
  if (is_led_blue_on == true) {
    blue_value = calculate_brightness_limits(blue_value, intensity_change);
  }
  if (is_led_green_on == true) {
    green_value = calculate_brightness_limits(green_value, intensity_change);
  }
  Serial.print(red_value);
  Serial.print(",");
  Serial.print(green_value);
  Serial.print(",");
  Serial.println(blue_value);
}
void translateIR() {
  switch (results.value) {
    case 0xFFA25D:
      Serial.println("power");
      is_led_red_on = !is_led_red_on;
      is_led_green_on = !is_led_green_on;
      is_led_blue_on = !is_led_blue_on;
      break;
    case 0xFF6897:
      Serial.println("0");
      break;
    case 0xFF30CF:
      Serial.println("1");
      led_state = ONLY_RED;
      is_led_red_on = true;
      is_led_green_on = false;
      is_led_blue_on = false;
      red_value = 255;
      green_value = 0;
      blue_value = 0;
      break;
    case 0xFF18E7:
      Serial.println("2");
      led_state = ONLY_GREEN;
      is_led_red_on = false;
      is_led_green_on = true;
      is_led_blue_on = false;
      red_value = 0;
      green_value = 255;
      blue_value = 0;
      break;
    case 0xFF7A85:
      Serial.println("3");
      led_state = ONLY_BLUE;
      is_led_red_on = false;
      is_led_green_on = false;
      is_led_blue_on = true;
      red_value = 0;
      green_value = 0;
      blue_value = 255;
      break;
    case 0xFF10EF:
      Serial.println("4");
      led_state = ONLY_YELLOW;
      is_led_red_on = true;
      is_led_green_on = true;
      is_led_blue_on = false;
      red_value = 255;
      green_value = 255;
      blue_value = 0;
      break;
    case 0xFF38C7:
      Serial.println("5");
      led_state = ONLY_CYAN;
      is_led_red_on = false;
      is_led_green_on = true;
      is_led_blue_on = true;
      red_value = 0;
      green_value = 255;
      blue_value = 255;
      break;
    case 0xFF5AA5:
      Serial.println("6");
      led_state = ONLY_MAGENTA;
      is_led_red_on = true;
      is_led_green_on = false;
      is_led_blue_on = true;
      red_value = 255;
      green_value = 0;
      blue_value = 255;
      break;
    case 0xFF42BD:
      Serial.println("7");
      led_state = ONLY_WHITE;
      is_led_red_on = true;
      is_led_green_on = true;
      is_led_blue_on = true;
      red_value = 255;
      green_value = 255;
      blue_value = 255;
      break;
    case 0xFF4AB5:
      Serial.println("8");
      break;
    case 0xFF52AD:
      Serial.println("9");
      break;
    case 0xFF629D:
      Serial.println("VOL+");
      change_brightness(led_change_intensity);
      break;
    case 0xFFA857:
      Serial.println("VOL-");
      change_brightness(-led_change_intensity);
      break;
    default:
      Serial.println(" other button : ");
      Serial.println(results.value);
  }
  delay(100);
}

void control_leds() {
  if (is_led_red_on == true) {
    analogWrite(pin_red_led, red_value);
  } else {
    analogWrite(pin_red_led, 0);
  }
  if (is_led_green_on == true) {
    analogWrite(pin_green_led, green_value);
  } else {
    analogWrite(pin_green_led, 0);
  }
  if (is_led_blue_on == true) {
    analogWrite(pin_blue_led, blue_value);
  } else {
    analogWrite(pin_blue_led, 0);
  }
}

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  // Check if a remote command has been received
  if (irrecv.decode(&results)) {
    translateIR();
    irrecv.resume();
  }
  // Control LED
  control_leds();
}