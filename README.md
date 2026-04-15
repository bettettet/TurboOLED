# fast SSD1306 oled library
a low level library to control the 128x64 SSD1306 OLED display.
## features
- **image display**: display images
- **pixel control**: turn on/off individual pixels on the display
- **fill/clear screen**: make the entire screen on/off
- more features to be added
## using the library

### initializing the OLED
```cpp
#include <TurboOLED.h>

TurboOLED oled;

void setup(){
    oled.oled_init(27,26,1000000); // SDApin, SCLpin, Speed (1Mhz)
}
```
### functions

- **oled_init(SDApin,SCLpin,speed)**
initializes the oled display with the given SDA, SCL pins and speed
```cpp
void setup(){
    oled.oled_init(27,26,1000000); // SDApin, SCLpin, Speed (1Mhz)
}
```

- **update_display()**
sets the screen to values in the buffer (updates the screen)
```cpp
void loop(){
    ...
    oled.update_display();
}
```

- **set_pixel(x,y)**
turns the pixel on coordinates x,y on
```cpp
void loop(){
    oled.set_pixel(10,20);
    oled.update_display();
}
```

- **clear_pixel(x,y)**
turns the pixel on coordinates x,y off
```cpp
void loop(){
    oled.clear_pixel(10,20);
    oled.update_display();
}
```

- **fill_screen()**
makes all pixels be turned on
```cpp
void loop(){
    oled.fill_screen();
    oled.update_display();
}
```

- **clear_screen()**
makes all pixels be turned off
```cpp
void loop(){
    oled.clear_screen();
    oled.update_display();
}
```

- **draw_bitmap(x,y,bitmap[],width,height)**
draws a bitmap (image) on the screen
the bitmap is the image rotated by 90° split into an array of vertical columns
```cpp
//example image
/*
000001111111000
000010000000100
000100000000010
000100001111110
000100010000001
011100010000001
100100010000001
100100001111110
100100000000010
100100000000010
100100000000010
100100000000010
100100000000010
011100011100010
000100010100010
000100010100010
000011100011100
*/
int amogus[15] = { //resulting array of 15 numbers (width = 15)
    0b00001111111000000, //17 bit numbers (height = 17)
    0b00010000000100000,
    0b00010000000100000,
    0b01111111111111100,
    0b10000000000000010,
    0b10000000000000001,
    0b10000000000000001,
    0b01110000001110001,
    0b00010000010001001,
    0b01110000010001001,
    0b10000000010001001,
    0b10000000010001001,
    0b10000000010001010,
    0b01111111110001100,
    0b00000000001110000};
void loop(){
    oled.draw_bitmap(10,20,amogus,15,17);
    oled.update_display();
}
```
