#include "driver/i2c.h"
#include <string.h>
#define I2C_PORT               I2C_NUM_0
#define SDA_PIN                27
#define SCL_PIN                26
#define OLED_ADDR              0x3C

#define SET_PIXEL(x, y, buf) (buf[x + ((y >> 3) << 7)] |= (1 << (y & 7)))
#define CLR_PIXEL(x, y, buf) (buf[x + ((y >> 3) << 7)] &= ~(1 << (y & 7)))
#define CLEAR_BUFFER(buf) memset(buf, 0, 1024)
#define FILL_BUFFER(buf) memset(buf, 0xFF, 1024)
class TurboOLED{
    public:
        void oled_init(int speed);
        esp_err_t update_display();
        void draw_bitmap(uint8_t x, uint8_t y, int bitmap[],uint8_t width, uint8_t height,bool overwrite=0);
        void set_pixel(uint8_t x,uint8_t y);
        void clear_pixel(uint8_t x,uint8_t y);
        void fill_screen();
        void clear_screen();
    private:
        uint8_t buffer[1024];
};