
// Port addresses for PC speaker control
#define SPEAKER_PORT 0x61
#define TIMER_PORT 0x43
#define TIMER_DATA 0x42

// Function to write to I/O port (inline assembly)
static inline void outb(unsigned short port, unsigned char value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Function to read from I/O port (inline assembly)
static inline unsigned char inb(unsigned short port) {
    unsigned char value;
    asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// Simple delay function
void delay(int milliseconds) {
    volatile int i, j;
    for (i = 0; i < milliseconds; i++) {
        for (j = 0; j < 1000; j++) {
            // Busy wait
        }
    }
}

// Function to make a beep at specified frequency for specified duration
void beep(int frequency, int duration_ms) {
    int divisor = 1193180 / frequency;  // Calculate timer divisor
    
    // Set up the timer
    outb(TIMER_PORT, 0xB6);  // Configure timer 2
    outb(TIMER_DATA, divisor & 0xFF);        // Low byte
    outb(TIMER_DATA, (divisor >> 8) & 0xFF); // High byte
    
    // Enable the speaker
    unsigned char speaker_status = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, speaker_status | 0x03);
    
    // Wait for the specified duration
    delay(duration_ms);
    
    // Disable the speaker
    outb(SPEAKER_PORT, speaker_status & 0xFC);
}

// Classic BIOS-style beep (800Hz for 200ms)
void bios_beep() {
    beep(800, 200);
}