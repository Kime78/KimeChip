#include "kimechip.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <cstring>
#include <stdio.h>
void KimeChip::initialise()
{
    pc     = 0x200;  // Program counter starts at 0x200
    opcode = 0;      // Reset current opcode
    I      = 0;      // Reset index register
    sp     = 0;      // Reset stack pointer

    // Clear display
    // Clear stack
    // Clear registers V0-VF
    // Clear memory
    delay_timer = 0;
    // Load fontset
    for(int i = 0; i < 80; ++i)
        memory[i] = chip8_fontset[i];
    for(int i=0;i<16;i++)
    {
        V[i] = 0;
        stack[i] = 0;
    }
    for(int i=0; i<64; i++)
        for(int j=0; j<64; j++)
            gfx[i][j] = 0;

   // drawSprite(0,0,32,gfx);

   int zz;
    // Reset timers
}
void KimeChip::loadGame(std::string path)
{
    //std::ifstream fin {"game.ch8",std::ios::binary};
    //std::copy(std::istream_iterator<char>{fin}, std::istreambuf_iterator<char>{}, memory.begin() + 512);
    char16_t test[10000];
    memset(test,0,sizeof(test));
    long lSize;
    FILE *f = fopen(path.c_str(), "r+b");
    fseek (f, 0, SEEK_END);
    lSize = ftell (f);
    rewind (f);
    fread(memory + 512, 1, sizeof(memory) - 512,f);
    //for(int i=512;i<4096;i++)
    //std::cout << memory[i];
    fclose(f);
    std::cout <<( memory[pc] << 8 | memory[pc + 1]);
}
void KimeChip::emulateCycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1];
    //std::cout << (opcode & 0xFFFF) <<std::endl;
    //pc += 2;
    // Decode opcode
    switch(opcode & 0xF000)
    {

    // Some opcodes //

    // V[x] = V[(opcode & 0x0F00) >> 8]
    // V[y] = V[(opcode & 0x00F0) >> 4]
    // NN = 0x00FF
    // NNN = 0x0FFF


    case 0x0000:
    {
        switch(opcode & 0x000F)
        {
            case 0x0000://00E0 Clear the screen
            {
                for(int i=0; i<64; i++)
                    for(int j=0; j<64; j++)
                        gfx[i][j] = 0;
                    drawSprite(0,0,32,gfx);

                pc += 2;
                break;

            }
            case 0x000E: //00EE Return from subroutine
            {
                --sp;
                pc = stack[sp];
                pc += 2;
                break;
            }
        }
        break;
    }
    case 0x1000: //1NNN Jump to NNN
    {
        pc = opcode & 0x0FFF;
        break;
    }


    case 0x2000: //2NNN Call NNN
    {
        stack[sp++] = pc;
        pc = opcode & 0x0FFF;
        break;
    }


    case 0x3000: //3XNN if Vx == NN then jump
    {
        if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
        break;
    }


    case 0x4000: //4XNN if Vx != NN then jump
    {
        if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
        break;
    }

    case 0x5000: //5XY0 if Vx == Vy then jump
    {
        if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
                pc += 4;
            else
                pc += 2;
        break;
    }


    case 0x6000: //6XNN set v[x] to NN
    {
        V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        pc += 2;
        break;
    }

    case 0x7000: //7XNN Adds NN to VX.
    {
        V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
        pc += 2;
        break;
    }

    case 0x8000:
    {
        switch(opcode & 0x000F)
        {

                // 8XY0 - Sets VX to the value of VY.
                case 0x0000:
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                // 8XY1 - Sets VX to (VX OR VY).
                case 0x0001:
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                // 8XY2 - Sets VX to (VX AND VY).
                case 0x0002:
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                // 8XY3 - Sets VX to (VX XOR VY).
                case 0x0003:
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                // 8XY4 - Adds VY to VX. VF is set to 1 when there's a carry,
                // and to 0 when there isn't.
                case 0x0004:
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
                        V[0xF] = 1; //carry
                    else
                        V[0xF] = 0;
                    pc += 2;
                    break;

                // 8XY5 - VY is subtracted from VX. VF is set to 0 when
                // there's a borrow, and 1 when there isn't.
                case 0x0005:
                    if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
                        V[0xF] = 0; // there is a borrow
                    else
                        V[0xF] = 1;
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                // 0x8XY6 - Shifts VX right by one. VF is set to the value of
                // the least significant bit of VX before the shift.
                case 0x0006:
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    pc += 2;
                    break;

                // 0x8XY7: Sets VX to VY minus VX. VF is set to 0 when there's
                // a borrow, and 1 when there isn't.
                case 0x0007:
                    if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])	// VY-VX
                        V[0xF] = 0; // there is a borrow
                    else
                        V[0xF] = 1;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;

                // 0x8XYE: Shifts VX left by one. VF is set to the value of
                // the most significant bit of VX before the shift.
                case 0x000E:
                {
                  //std::cout<<std::endl;
                  //std::cout <<"Before shift " << (int)V[0xF] << " " <<(int) V[(opcode & 0x0F00) >> 8] << std::endl;
                  V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
                  V[(opcode & 0x0F00) >> 8] <<= 1;
                  //std::cout <<"After shift " <<  (int)V[0xF] << " " <<(int) V[(opcode & 0x0F00) >> 8] << std::endl;
                  pc += 2;
                  break;
                }
        }break;
    }

    case 0x9000: //9XY0 Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
    {

        if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
            pc += 4;
        else
            pc += 2;
        break;

    }
    case 0xA000: //ANNN: Sets I to the address NNN
    {

        I = opcode & 0x0FFF;
        pc += 2;
        break;
    }
    case 0xB000: //BNNN Jumps to the address NNN plus V0.
    {

        pc = opcode & 0x0FFF + V[0];
        break;
    }
    case 0xC000: //CNNN Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
    {

        V[(opcode & 0x0F00) >> 8] =  floor((rand() % 0xFF) & (opcode & 0x00FF));
        pc += 2;
        break;
    }

    case 0xD000: //DXYN Draw a sprite
    {

        unsigned short x = V[(opcode & 0x0F00) >> 8];
        unsigned short y = V[(opcode & 0x00F0) >> 4];
        unsigned short height = opcode & 0x000F;
        unsigned short pixel;
        //std::cout<<x<<" "<<y<<std::endl;
        V[0xF] = 0;
        for(int yline = 0; yline < height; yline++)
        {
            pixel = memory[I + yline];
            for(int xline = 0; xline < 8; xline++)
            {
                if((pixel & (0x80 >> xline)) != 0)
                {
                    if(gfx[xline + x][yline + y] == 1)
                    {
                        V[0xF] = 1;
                    }
                    gfx[xline + x][yline + y] ^= 1;
                }
            }
        }



        drawSprite(x,y,height,gfx);
        // Clear GFX each draw....
        pc += 2;
        break;

    }

    case 0xE000:
    {

        switch(opcode & 0x00FF)
        {

        case 0x009E: //EX9E Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)

            if(key[V[(opcode & 0x0F00) >> 8]] == 1)
                pc += 4;
            else
                pc += 2;
            break;

        case 0x00A1: //EXA1 Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)

            if(key[V[(opcode & 0x0F00) >> 8]] != 1)
                pc += 4;
            else
                pc += 2;
            break;
        }
        break;
    }

    case 0xF000:
    {

        switch(opcode & 0x00FF)
        {
            case 0x0007: //FX07 Sets VX to the value of the delay timer.
            {

                V[(opcode & 0x0F00) >> 8] = delay_timer;
                //std::cout << (int)delay_timer <<std::endl;
                pc += 2;
                break;

            }
            case 0x000A: //FX0A A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
            {
             //   std::cout << "Testing keys...0" << std::endl;

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
                    {
                        V[(opcode & 0x0F00) >> 8] = 0;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
                    {
                        V[(opcode & 0x0F00) >> 8] = 1;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
                    {
                        V[(opcode & 0x0F00) >> 8] = 2;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
                    {
                        V[(opcode & 0x0F00) >> 8] = 3;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                    {
                        V[(opcode & 0x0F00) >> 8] = 4;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                    {
                        V[(opcode & 0x0F00) >> 8] = 5;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                    {
                        V[(opcode & 0x0F00) >> 8] = 6;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                    {
                        V[(opcode & 0x0F00) >> 8] = 7;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    {
                        V[(opcode & 0x0F00) >> 8] = 8;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    {
                        V[(opcode & 0x0F00) >> 8] = 9;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    {
                        V[(opcode & 0x0F00) >> 8] = 10;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
                    {
                        V[(opcode & 0x0F00) >> 8] = 11;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                    {
                        V[(opcode & 0x0F00) >> 8] = 12;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                    {
                        V[(opcode & 0x0F00) >> 8] = 13;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                    {
                        V[(opcode & 0x0F00) >> 8] = 14;
                        pc += 2;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
                    {
                        V[(opcode & 0x0F00) >> 8] = 15;
                        pc += 2;
                    }


                break;
            }
            case 0x0015: //FX15 Sets the delay timer to VX.
            {

                delay_timer = V[(opcode & 0x0F00) >> 8];
                pc += 2;
                break;
            }

            case 0x0018: //FX18 Sets the sound timer to VX.
            {

                sound_timer = V[(opcode & 0x0F00) >> 8];
                pc += 2;
                break;
            }

            case 0x001E: //FX1E Adds VX to I. VF is set to 1 when there is a range overflow (I+VX>0xFFF), and to 0 when there isn't.
            {

                I += V[(opcode & 0x0F00) >> 8];
                if(I + V[(opcode & 0x0F00) >> 8] > 0xFFF)
                    V[0xF] = 1;
                else
                {
                    V[0xF] = 0;

                }
                pc += 2;
                break;
            }

            case 0x0029: //FX29 Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
            {
                I = V[(opcode & 0x0F00) >> 8] * 0x5;
                pc += 2;
                break;
            }

            /*
            case 0x0033:
            memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
            memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
            memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
            pc += 2;
            break;
            */
            case 0x0033: //FX33 Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2.
            {

                unsigned char digit = V[(opcode & 0x0F00) >> 8];
                for(int Index = 3; Index > 0; --Index)
                {
                    memory[I + Index - 1] = digit % 10;
                    digit /= 10;
                }
                pc += 2;
                break;
            }
                case 0x0055:
                    {
                        for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
                            memory[I + i] = V[i];

                        // On the original interpreter, when the
                        // operation is done, I = I + X + 1.
                        //I += ((opcode & 0x0F00) >> 8) + 1;
                        pc += 2;
                        break;
                    }


                case 0x0065:
                    {

                        for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
                            V[i] = memory[I + i];

                        // On the original interpreter,
                        // when the operation is done, I = I + X + 1.
                        //I += ((opcode & 0x0F00) >> 8) + 1;
                        pc += 2;
                        break;

                    }
        }break;
    }



    // More opcodes //

    default:
        exit(0);
        break;
    }
        if (delay_timer > 0)
            --delay_timer;
}
