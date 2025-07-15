//#include "beep.c"
#include "gfx.c"
#include "lib/int.h"
#include"lib/fs.h"


char itoa_aoti_buffer[15];
char command[15];
int command_extension1;
char command_extenstion2[15];
char echo_str[15];
int init = true;
char* AK_OS = "\r@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\r@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\r                                                    \n\r       @@@       @@    @@@    @@@@@@@@@    @@@@@@   \n\r     @@@@@@@    @@@@  @@@@@  @@@@@@@@@@@  @@@   @@  \n\r    @@@   @@@   @@@@@@@@@    @@@@@@@@@@@  @@@@      \n\r   @@@@   @@@@  @@@@@@@   @@ @@@@@ @@@@@   @@@@     \n\r   @@@@@@@@@@@  @@@@@ @@@@   @@@@   @@@@     @@@@   \n\r   @@@@@@@@@@@  @@@@  @@@@@  @@@@@ @@@@@      @@@@  \n\r   @@@@   @@@@  @@@@  @@@@@  @@@@@@@@@@@     @@@@@  \n\r   @@@@   @@@@  @@@@   @@@@   @@@@@@@@@  @@@@@@@@   \n\r                                                    \n\r@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\r@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\r";

void sleep(int duration){
    for(long int i = 0; i< 500*duration+1; i++){
        printf("%d\r",i);}
            puts("\r\ndone");
}


void start_gui(){
    mode_gfx();
    clear_screen(0x01);
    do{
        draw_sprite(SPR_CURSOR, cursor_x, cursor_y, 16, 16);
        handle_inputs();
        
    }while(1);
}


void main() {
    if(init == true){//clears the screen if init
        mode_txt();
        puts_color(AK_OS, 0x03);
        for(int j = 0; j <14; j++){
            putchar('\n');
        }
        init = false;
    }
    //bios_beep();
    printf("\rtype 'help' for a list of commands");
    do{
        printf("\r\n%c ",'#');
        scanf_cmd("%s%d%s",command, &command_extension1, &command_extenstion2);
        
        if(strcmp(command, "help")==0){
            puts("\rCOMMANDS: help, clear, echo, ls, read, edit, about\n");
        }
        else if(strcmp(command, "clear")==0){
            mode_txt();
        }
        else if(strcmp(command, "echo")==0){
            write_sector(0x00, 0, 0, FILE_LOCATION_START+command_extension1, command_extenstion2);
            printf("\r%s",command_extenstion2);
        } 
        else if(strcmp(command, "ls")==0){

            uint8_t buffer[512];
            for(int i =FILE_LOCATION_START; i<MAX_FILES+FILE_LOCATION_START; i++){
                
                read_sector(0x00, 0, 0, i, buffer);
                if(buffer[0] != '\0'){//if the first character of the file is null, skip the empty file
                printf("\rFILE %d ",(int)i-FILE_LOCATION_START);
                printf("\n\r%c-%s\n",0xC0, (char*)buffer);
                   }}
        }
        else if(strcmp(command, "read")==0){

            uint8_t buffer[512];
            printf("\rFILE_%d ",command_extension1);
            read_sector(0x00, 0, 0, FILE_LOCATION_START+command_extension1, buffer);
            printf("\n\r%c-%s\n",0xC0, (char*)buffer);
        }
        else if(strcmp(command, "edit")==0 && command_extension1 < 10){
            mode_txt();
            puts_color("type ':quit' on a new line to exit\n\r", 0x0f);
            char buffer_line[512];
            char buffer_write[512];
            int editor_open = 1;
            printf("");
            printf("\r\n\n\n~");
            while(editor_open == 1){
                
                gets(buffer_line, 512);
                if (strcmp(buffer_line, ":quit")==0){
                    editor_open =0;
                    break;
                }
                strcat(buffer_line,"\n\r");
                strcat(buffer_write,buffer_line);
                printf("\r~");
                write_sector(0x00, 0, 0, FILE_LOCATION_START+command_extension1, buffer_write);

                
        }
        }
        else if(strcmp(command, "game")==0){
            mode_txt();
            int x,y;
            while(1==1){
                set_cursorXY(x,y);
                putchar(0x02);
                set_cursorXY(x,y);
                switch (getchar()) {
                    case 'w':
                        y--;
                        break;
                    case 's':
                        y++;
                        break;
                    case 'a':
                        x--;
                        break;
                    case 'd':
                        x++;
                        break;
                    case 27:
                        main();
                }
            }
        }
        else if(strcmp(command, "about")==0){
            mode_txt();
            puts_color(AK_OS, 0x03);
            for(int j = 0; j <14; j++){
                putchar('\n');
            }
            puts("\r\nAK-OS 0.1 by Andrew Khoury");

        }
        else if(strcmp(command, "wait")==0){
            sleep(command_extension1);

        }
        else if(strcmp(command, "mode")==0){
            switch (command_extension1) {
                case 0:
                    mode_txt();
                    break;
                case 1:
                    start_gui();
                    break;
                case 2:
                    screensaver();
                    break;
                case 69:
                    mode_bigtxt();
                    break;
                default:
                    mode_txt();
                    break;
            
            }

        }

        else{
            printf("\r COMMAND NOT VALID");
        }
        
    
    }while (1);

}
