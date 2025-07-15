#include"lib/int.h"
#include"lib/arg.h"

#define VGA_MEMORY (uint16_t*) 0xB8000
uint16_t* video_mem = VGA_MEMORY;
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
uint8_t cursor_row = 0;
uint8_t cursor_col = 0;

//=========INT AND STR CONVERSION==========
int itoa(int value, char *sp){//int to string
    int radix = 10;

    char tmp[16];
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);    
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    if (sign) 
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}
int atoi(const char* str) {//string to int
    int res = 0;
    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    }
    while (*str >= '0' && *str <= '9') {
        res = res * 10 + (*str - '0');
        str++;
    }
    return res * sign;
}
//===========OUTPUT CHARS==================
void putchar(char c) {
    __asm__ volatile (
        "mov $0x0E, %%ah\n"
        "mov %0, %%al\n"
        "int $0x10\n"
        :
        : "r"(c)
        : "ah", "al"
    );
}
void putchar_color(char c, int color) {
    if (c == '\n') {
        cursor_row++;
       
    } else if (c =='\r') {
        cursor_col = 0;
    }else {
        uint16_t pos = cursor_row * VGA_WIDTH + cursor_col;
        video_mem[pos] = (uint16_t)c | (color << 8); // light gray on black
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    // Scroll if needed
    if (cursor_row >= VGA_HEIGHT) {
        for (int row = 1; row < VGA_HEIGHT; row++) {
            for (int col = 0; col < VGA_WIDTH; col++) {
                video_mem[(row - 1) * VGA_WIDTH + col] =
                    video_mem[row * VGA_WIDTH + col];
            }
        }

        // Clear last line
        for (int col = 0; col < VGA_WIDTH; col++) {
            video_mem[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = ' ' | (color << 8);
        }

        cursor_row = VGA_HEIGHT - 1;
        cursor_col = 0;
    }
}
//=======DIRECT PORT I/O FUNCTIONS=========
static inline unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile ("inb %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

static inline void outb(unsigned short port, unsigned char data) {
    __asm__ volatile ("outb %0, %1" : : "a" (data), "dN" (port));
}
//========INPUTS===========================
char getchar() {
    char c;
    __asm__ volatile (
        "xor %%ah, %%ah\n"   // AH = 0 (function 0x00)
        "int $0x16\n"        // BIOS keyboard interrupt
        "mov %%al, %0\n"     // store AL into c
        : "=r"(c)
        :
        : "ah", "al"
    );
    return c;
}
char getkeycode(){
    unsigned char keycode;
    asm volatile (
        "movb $1, %%ah\n\t"
        "int $0x16\n\t"
        "cbw\n\t"
        "int $0x16\n\t"
        "movb %%ah, %0"
        : "=m" (keycode)
        :
        : "ax"
    );
    return keycode;
}
// Check if a key is available in the keyboard buffer
int kbhit() {
    return (inb(0x64) & 0x01);
}
char getch() {
    unsigned char scancode;

    while (!kbhit()) {
        //hang
    }
    
    scancode = inb(0x60); //kbd scancode port
    

    if (scancode & 0x80) {
        return getch(); // Key release, wait for next key
    }
    
    return scancode;
}
void gets(char* buffer, int max_len) {
    int i = 0;
    while (i < max_len - 1) {
        char c = getchar();
        if (c == '\r') {  // Enter key
            putchar('\n');
            break;
        }
        if (c == '\b' && i > 0) {
            putchar('\b');
            putchar(' ');
            putchar('\b');
            i--;
        } else if (c >= 32 && c < 127) {
            putchar(c);
            buffer[i++] = c;
        }
    }
    buffer[i] = '\0';
}
int scanf_cmd(const char* fmt, ...) {//spaces are delimiters for strings
    char buffer[128];
    gets(buffer, 128);
    va_list args;
    va_start(args, fmt);
    const char* buf_ptr = buffer;
    int assigned = 0;
    
    // Skip leading whitespace
    while (*buf_ptr == ' ' || *buf_ptr == '\t') buf_ptr++;
    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'd') {
                int* iptr = va_arg(args, int*);
                *iptr = atoi(buf_ptr);
                while (*buf_ptr && *buf_ptr != ' ') buf_ptr++;
                while (*buf_ptr == ' ') buf_ptr++; // Skip multiple spaces
                assigned++;
            } else if (*fmt == 's') {
                char* sptr = va_arg(args, char*);
                // Read until space or end of string
                while (*buf_ptr && *buf_ptr != ' ') {
                    *sptr++ = *buf_ptr++;
                }
                *sptr = '\0';
                while (*buf_ptr == ' ') buf_ptr++; // Skip multiple spaces
                assigned++;
            }
        }
        fmt++;
    }
    va_end(args);
    return assigned;
}
int scanf(const char* fmt, ...) {//spaces are included in the string
    char buffer[128];
    gets(buffer, 128);
    va_list args;
    va_start(args, fmt);
    const char* buf_ptr = buffer;
    int assigned = 0;
    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'd') {
                int* iptr = va_arg(args, int*);
                *iptr = atoi(buf_ptr);
                while (*buf_ptr && *buf_ptr != ' ') buf_ptr++;
                if (*buf_ptr == ' ') buf_ptr++;
                assigned++;
            } else if (*fmt == 's') {
                char* sptr = va_arg(args, char*);
                // Read the entire remaining string including spaces
                while (*buf_ptr) {
                    *sptr++ = *buf_ptr++;
                }
                *sptr = '\0';
                assigned++;
            }
        }
        fmt++;
    }
    va_end(args);
    return assigned;
}
//=========OUTPUT STRINGS==================
void puts(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}
void puts_color(const char* str, int color) {
    while (*str) {
        putchar_color(*str++,color);
    }
}
void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[32];

    for (const char* p = format; *p; p++) {
        if (*p == '%') {
            p++;
            if (*p == 's') {
                char* str = va_arg(args, char*);
                while (*str) putchar(*str++);
            } else if (*p == 'd') {
                int num = va_arg(args, int);
                itoa(num, buffer);
                char* b = buffer;
                while (*b) putchar(*b++);
            } else if (*p == 'c') {
                char ch = (char)va_arg(args, int);
                putchar(ch);
            } else {
                // unknown format, just print it
                putchar('%');
                putchar(*p);
            }
        } else {
            putchar(*p);
        }
    }

    va_end(args);
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
    // Return the difference of the first differing characters, or 0 if equal
    return (unsigned char)*s1 - (unsigned char)*s2;
}
int memcmp(const void* s1, const void* s2, uint16_t n) {
    const unsigned char* p1 = (const unsigned char*) s1;
    const unsigned char* p2 = (const unsigned char*) s2;

    for (uint16_t i = 0; i < n; i++) {
        if (p1[i] != p2[i])
            return p1[i] - p2[i];
    }
    return 0;
}
char* strcat(char* dest, const char* src) {
    char* ptr = dest;
    
    // Find the end of dest string
    while (*ptr) ptr++;
    
    // Copy src to the end of dest
    while (*src) *ptr++ = *src++;
    
    // Null terminate
    *ptr = '\0';
    
    return dest;
}
//=========CURSOR MANAGEMENT===============
void set_cursorXY(int x, int y) {
    asm volatile (
        "movb $0x02, %%ah\n\t"      // Set cursor position function
        "movb %0, %%dh\n\t"         // Load row position
        "movb %1, %%dl\n\t"         // Load column position
        "movb $0, %%bh\n\t"         // Page number 0
        "int $0x10\n\t"             // BIOS video interrupt
            // BIOS video interrupt
        :                           // No output operands
        :  "m" (y), "m" (x)   // Input operands
        : "ax", "bx", "cx", "dx"    // Clobbered registers
    );
}

void set_cursorXY_and_draw_char(int x, int y, char thing) {
    asm volatile (
        "movb $0x02, %%ah\n\t"      // Set cursor position function
        "movb %0, %%dh\n\t"         // Load row position
        "movb %1, %%dl\n\t"         // Load column position
        "movb $0, %%bh\n\t"         // Page number 0
        "int $0x10\n\t"             // BIOS video interrupt
        "movb $0x0A, %%ah\n\t"      // Write character function
        "movb $0, %%bh\n\t"         // Page number 0
        "movw $1, %%cx\n\t"         // Character count = 1
        "movb %2, %%al\n\t"         // Load player character
        "int $0x10"                 // BIOS video interrupt
        :                           // No output operands
        :  "m" (y), "m" (x), "m" (thing)   // Input operands
        : "ax", "bx", "cx", "dx"    // Clobbered registers
    );
}

int get_cursor_position() {
    int position;
    
    __asm__ volatile (
        "mov $0x03, %%ah\n\t"      // BIOS function 03h - Get cursor position
        "mov $0x00, %%bh\n\t"      // Page number (0 for default)
        "int $0x10\n\t"            // BIOS video interrupt
        "mov %%dh, %%al\n\t"       // Move row to AL
        "mov $80, %%bl\n\t"        // 80 columns per row
        "mul %%bl\n\t"             // Multiply row by 80
        "add %%dl, %%al\n\t"       // Add column to get linear position
        : "=a" (position)           // Output: position in ax
        :                           // No inputs
        : "bl", "dl"               // Clobbered registers
    );
    
    return position;
}

//=====OPERATING MODES=====================
void mode_txt() {
    cursor_row = 0;
    uint16_t bx = 0x0700;  // BH = 0x07 (light gray on black), BL = don't care

    // Set video mode to 03h (80x25 text)
    asm volatile (
        "mov $0x00, %%ah\n"
        "mov $0x03, %%al\n"
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

void mode_bigtxt() {
    cursor_row = 0;
    uint16_t bx = 0x0000;  // BH = 0x07 (light gray on black), BL = don't care

    // Set video mode to 13h (bigger text, no cursor)
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
