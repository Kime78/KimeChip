#ifndef KIMECHIP_H_INCLUDED
#define KIMECHIP_H_INCLUDED
#include <SFML/Window.hpp>



class KimeChip
{
    public:
        void initialiseOpenGL();
        void loadGame(std::string path);
        void emulateCycle();
        void renderFrame();
        void drawSprite(int X, int Y,int depth, unsigned short toDraw[][64]);
        void initialise();
        void destroy();
        sf::Window gameWindow;
        bool key[16];
    private:
        unsigned short pc; // current procedure call
        unsigned short opcode;
        unsigned short I; // index register
        unsigned short sp;
        unsigned short stack[16];
        unsigned char delay_timer;
        unsigned char sound_timer;
        unsigned char V[16] = {0};
        unsigned short gfx[64][64];

        unsigned char chip8_fontset[80] =
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, //0
            0x20, 0x60, 0x20, 0x20, 0x70, //1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
            0x90, 0x90, 0xF0, 0x10, 0x10, //4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
            0xF0, 0x10, 0x20, 0x40, 0x40, //7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
            0xF0, 0x90, 0xF0, 0x90, 0x90, //A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
            0xF0, 0x80, 0x80, 0x80, 0xF0, //C
            0xE0, 0x90, 0x90, 0x90, 0xE0, //D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
            0xF0, 0x80, 0xF0, 0x80, 0x80  //F
        };
        unsigned char memory[4096] = {0};
};

#endif // KIMECHIP_H_INCLUDED
