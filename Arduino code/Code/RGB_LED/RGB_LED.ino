// circuit diagram - https://www.tinkercad.com/things/djXeoY5NZM0

int red_light_pin= 9;
int green_light_pin = 10;
int blue_light_pin = 11;
void setup() {
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
}
void loop() {
  RGB_color(255, 0, 0); // Red
  delay(1000);
  RGB_color(0, 255, 0); // Green
  delay(1000);
  RGB_color(0, 0, 255); // Blue
  delay(1000);
  RGB_color(255, 255, 125); // Raspberry
  delay(1000);
  RGB_color(0, 255, 255); // Cyan
  delay(1000);
  RGB_color(255, 0, 255); // Magenta
  delay(1000);
  RGB_color(255, 255, 0); // Yellow
  delay(1000);
  RGB_color(255, 255, 255); // White
  delay(1000);
  RGB_color(128,0,128); // Purple
  delay(1000);
  RGB_color(255,165,0); // Orange
  delay(1000);
  RGB_color(139,69,19); // Brown
  delay(1000);

// more color codes on https://www.rapidtables.com/web/color/RGB_Color.html

}
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
