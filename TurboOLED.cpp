#include "TurboOLED.h"
void TurboOLED::oled_init(int speed) {
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)SDA_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t)SCL_PIN;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = speed;
    conf.clk_flags = 0;
    
    i2c_param_config(I2C_PORT, &conf);
    i2c_driver_install(I2C_PORT, conf.mode, 0, 0, 0);
    
    static const uint8_t init_cmds[] = {
        0xAE,          // Display OFF
        0xD5, 0x80,    // Set Display Clock Divide Ratio
        0xA8, 0x3F,    // Set Multiplex Ratio (64 lines)
        0xD3, 0x00,    // Set Display Offset
        0x40,          // Set Display Start Line
        0x8D, 0x14,    // Charge Pump
        0x20, 0x00,    // Memory Addressing Mode (Horizontal)
        0xA1,          // Segment Remap (rotation of the screen)
        0xC8,          // COM Output Scan Direction
        0xDA, 0x12,    // Set COM Pins Hardware Configuration
        0x81, 0xCF,    // Set Contrast Control
        0xD9, 0xF1,    // Set Pre-charge Period
        0xDB, 0x40,    // Set VCOMH Deselect Level
        0xA4,          // Entire Display ON (Follow RAM content)
        0xA6,          // Normal Display (Not Inverted)
        0xAF           // Display ON
    };
    
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x00, true); // Control byte: Následují jen příkazy
    i2c_master_write(cmd, (uint8_t*)init_cmds, sizeof(init_cmds), true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
}

esp_err_t TurboOLED::update_display() {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    //set ram address of display
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x00, true); // Příkazový režim
    i2c_master_write_byte(cmd, 0x21, true); // Column addr
    i2c_master_write_byte(cmd, 0, true);
    i2c_master_write_byte(cmd, 127, true);
    i2c_master_write_byte(cmd, 0x22, true); // Page addr
    i2c_master_write_byte(cmd, 0, true);
    i2c_master_write_byte(cmd, 7, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(10));
    i2c_cmd_link_delete(cmd);
    
    //send buffer
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x40, true);
    i2c_master_write(cmd, TurboOLED::buffer, 1024, true);
    i2c_master_stop(cmd);
    
    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);
    return ret;
}
void TurboOLED::draw_bitmap(uint8_t x, uint8_t y, int bitmap[],uint8_t width, uint8_t height,bool overwrite){
    x = (x)<(0)?(0):(x)>(128-width)?(128-width):(x);
    y = (y)<(0)?(0):(y)>(64-height)?(64-height):(y);
    int i = 0;
    switch(overwrite){
        case 0:
            while(i < width){
                buffer[x+i+((y>>3)<<7)] |= (bitmap[i]&(~(1<<(9-(y&7)))))<<(y&7);
                int j = 0;
                while(j < ((height-(y&7))<<3)+((height-(y&7))&7)?(1):(0)){
                    if(8+(j<<3)-(y&7)>height) break;
                    buffer[x+i+(((y>>3)+1+j)<<7)] |= (bitmap[i]>>(8+(j<<3)-(y&7)))&255;
                    j++;
                }
                i++;
            }
            break;
        case 1:
            while(i < width){
                buffer[x+i+((y>>3)<<7)] &= 255>>(y&7);
                buffer[x+i+((y>>3)<<7)] |= (bitmap[i]&(~(1<<(9-(y&7)))))<<(y&7);
                int j = 0;
                while(j < ((height-(y&7))<<3)+((height-(y&7))&7)?(1):(0)){
                    if(8+(j<<3)-(y&7)>height) break;
                    if((bitmap[i]>>(8+(j<<3)-(y&7)))&255 < 127){
                        buffer[x+i+(((y>>3)+1+j)<<7)] &= ~(1<<(height-8-(j<<3)-(y&7)));
                        buffer[x+i+(((y>>3)+1+j)<<7)] |= (bitmap[i]>>(8+(j<<3)-(y&7)))&255;
                    } else {
                    buffer[x+i+(((y>>3)+1+j)<<7)] = (bitmap[i]>>(8+(j<<3)-(y&7)))&255;
                    }
                    j++;
                }
                i++;
            }
            break;
    }
}
void TurboOLED::set_pixel(uint8_t x, uint8_t y){
    buffer[x + ((y >> 3) << 7)] |= 1 << (y & 7);
}
void TurboOLED::clear_pixel(uint8_t x, uint8_t y){
    buffer[x + ((y >> 3) << 7)] &= ~(1 << (y & 7));
}
void TurboOLED::fill_screen(){
    memset(buffer, 0xFF, 1024);
}
void TurboOLED::clear_screen(){
    memset(buffer, 0, 1024);
}