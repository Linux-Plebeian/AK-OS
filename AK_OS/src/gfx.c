#include "lib/sprites.h"
#include"io.c"

#define CURSOR_SIZE 2

#define KBD_LEFT  0x4B
#define KBD_RIGHT 0x4D
#define KBD_UP    0x48
#define KBD_DOWN  0x50
#define KBD_SPACE 0x39
#define KBD_ESC 0x01
#define KBD_PgDn 0x51
#define KBD_PgUp 0x49
int cursor_x;
int cursor_y;
int click_l;
int click_r;

void mode_gfx() {
    uint16_t bx = 0x0000;  // BH = 0x07 (light gray on black), BL = don't care

    // Set video mode to 13h (320x240 256 colors)
    asm volatile (
        "mov $0x00, %%ah\n"
        "mov $0x13, %%al\n"
        "int $0x10\n"
        ::: "ax"
    );
    
    // Scroll window up using AH=06h, AL=0, BH=color, CX=0, DX=0x184F
    asm volatile (
        "mov $0x06, %%ah\n"
        "mov $0x00, %%al\n"
        "mov %0, %%bx\n"
        "mov $0x0000, %%cx\n"
        "mov $0x184F, %%dx\n"
        "int $0x10\n"
        :
        : "r"(bx)
        : "ax", "bx", "cx", "dx"
    );
}
void draw_pixel(int x, int y, unsigned char color) {
    unsigned char *vga = (unsigned char*)0xA0000;
    vga[y * 320 + x] = color;
}

void clear_screen(unsigned char color) {
    unsigned char *vga = (unsigned char*)0xA0000;

    unsigned int *vga32 = (unsigned int*)vga;
    unsigned int color32 = color | (color << 8) | (color << 16) | (color << 24);
    
    for (int i = 0; i < 16000; i++) {
        vga32[i] = color32;
    }
}


void draw_sprite(const int sprite[], int xcoord, int ycoord, int width, int height) {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int color = sprite[y * width + x];
            if (color != 0x00){
                unsigned char lower_byte = color >>= 4;
                draw_pixel(xcoord + x, ycoord + y, lower_byte);
            }
            
        }
    }
}
void draw_rectangle(int x, int y, int length, int width, int color){
    for (int i = x; i < width+x; i++){
        for (int j = y; j < length+y; j++){
            draw_pixel(i,j,color);
        }
    }
}
void screensaver(){
    int x=0,y=0,vx=1,vy=2;
    do{clear_screen(1);
        

        x+=vx; y+=vy;
        if (x<=0 || x > 318-SCREENSAVER_W){
            vx *= -1;
        }
        if (y<2 || y > 238-SCREENSAVER_H*4){
            vy *= -1;
        }
        draw_sprite(SCREENSAVER,x,y,SCREENSAVER_W,SCREENSAVER_H);
        for(int w = 0; w < 5000; w++);

}while(1);}
void test(){
    int x, y;
    for (int i = 0; i<320; i++){
        for(int j = 0; j<240; j++){
            draw_pixel(i, j, i);
        }
    }
}
void handle_inputs(){
    char kbd_scancode = getkeycode();
    //using arrow keys to control mouse until mouse support implemented
    if (kbd_scancode == KBD_UP) {
        cursor_y-=CURSOR_SIZE;
    }
    else if (kbd_scancode == KBD_DOWN) {
        cursor_y+=CURSOR_SIZE;
    }
    else if (kbd_scancode== KBD_LEFT) {
        cursor_x-=CURSOR_SIZE;
    }
    else if (kbd_scancode == KBD_RIGHT) {
        cursor_x+=CURSOR_SIZE;
    }
}
