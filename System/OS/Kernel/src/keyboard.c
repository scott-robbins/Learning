#include "keyboard.h"  
#include "source.h"
#include <unistd.h>
#include <sys/io.h>


unsigned char get_scancode(){
    unsigned char inputdata;
    inputdata = inb(0x60);
    return inputdata;
}



void keyboard_handler(){
    unsigned char scancode = 0;
    unsigned int shift_key = 0;
    int clicked = 0;
    int canSend = 0;
    // Get element in console buffer
    scancode = get_scancode();

    char character;
    switch(scancode){
        case 0x0f:
            clicked = 0;
            canSend = 0;

            clear_screen(); // this doesnt seem to return control
            vga_index = 0;
            break;
        case 0x01:
            clicked = 0;
            break;
        case 0x1E:
            if (!clicked){
                character = 'a';
                canSend = 1;
            }else{
                clicked = 1;
            }
            break;
        case 0x30:
            if (!clicked){
                canSend = 1;
                character = 'b';
            }else{
                clicked = 1;
            }
        case 0x2E:
            if (!clicked){
                canSend = 1;
                character = 'c';
            }else{
                clicked = 1;
            }
        case 0x20:
            if (!clicked){
                canSend = 1;
                character = 'd';
            }else{
                clicked = 1;
            }
        case 0x12:
            if (!clicked){
                canSend = 1;
                character = 'e';
            }else{
                clicked = 1;
            }
        case 0x21:
            if (!clicked){
                canSend = 1;
                character = 'f';
            }else{
                clicked = 1;
            }
        case 0x22:
            if (!clicked){
                canSend = 1;
                character = 'g';
            }else{
                clicked = 1;
            }
        case 0x17:
            if (!clicked){
                canSend = 1;
                character = 'i';
            }else{
                clicked = 1;
            }
        case 0x24:
            if (!clicked){
                canSend = 1;
                character = 'j';
            }else{
                clicked = 1;
            }
        case 0x25:
            if (!clicked){
                canSend = 1;
                character = 'k';
            }else{
                clicked = 1;
            }
        case 0x26:
            if (!clicked){
                canSend = 1;
                character = 'l';
            }else{
                clicked = 1;
            }
        case 0x32:
            if (!clicked){
                canSend = 1;
                character = 'm';
            }else{
                clicked = 1;
            }
        case 0x31:
            if (!clicked){
                canSend = 1;
                character = 'n';
            }else{
                clicked = 1;
            }
        case 0x18:
            if (!clicked){
                canSend = 1;
                character = 'o';
            }else{
                clicked = 1;
            }
        case 0x19:
            if (!clicked){
                canSend = 1;
                character = 'p';
            }else{
                clicked = 1;
            }
        case 0x10:
            if (!clicked){
                canSend = 1;
                character = 'q';
            }else{
                clicked = 1;
            }
        case 0x13:
            if (!clicked){
                canSend = 1;
                character = 'r';
            }else{
                clicked = 1;
            }
        case 0x1F:
            if (!clicked){
                canSend = 1;
                character = 's';
            }else{
                clicked = 1;
            }
        case 0x14:
            if (!clicked){
                canSend = 1;
                character = 't';
            }else{
                clicked = 1;
            }
        case 0x16:
            if (!clicked){
                canSend = 1;
                character = 'u';
            }else{
                clicked = 1;
            }
        case 0x2F:
            if (!clicked){
                canSend = 1;
                character = 'v';
            }else{
                clicked = 1;
            }
        case 0x11:
            if (!clicked){
                canSend = 1;
                character = 'w';
            }else{
                clicked = 1;
            }
        case 0x2D:
            if (!clicked){
                canSend = 1;
                character = 'x';
            }else{
                clicked = 1;
            }
        case 0x15:
            if (!clicked){
                canSend = 1;
                character = 'y';
            }else{
                clicked = 1;
            }
        case 0x2C:
            if (!clicked){
                canSend = 1;
                character = 'z';
            }else{
                clicked = 1;
            }
        
    }
    // check if theres a character to send
    if (canSend) {
        vga_index += 1;
        print_char(character, 15); // Make terminal text white
    }
            
    

}

