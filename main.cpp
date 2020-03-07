#include "kimechip.h"
#include <SFML/Window.hpp>
#include <iostream>
#include <chrono>
#include <thread>
int sprite[4][8] =
{
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
};
int main()
{

    KimeChip t;

    int option;
    t.initialise();
    std::cout << "Game list:\n1.Breakout\n2.Maze Demo\n3.Pong (1 Player)\n4.Pong (2 Players)\n5.Tetris\n6.TicTacToe\n";
    std::cout << "Select what game do you want to play: " <<std::endl;
    std::cin >> option;
    switch(option)
    {
    case 1:
        t.loadGame("roms\\breakout.ch8");
        break;
    case 2:
        t.loadGame("roms\\maze.ch8");
        break;
    case 3:
        t.loadGame("roms\\pong.ch8");
        break;
    case 4:
        t.loadGame("roms\\pong2.ch8");
        break;
    case 5:
        t.loadGame("roms\\tetris.ch8");
        break;
    case 6:
        t.loadGame("roms\\tictac.ch8");
        break;
    default:
        std::cout << "Invalid Option! Try again" <<std::endl;
    }
    t.initialiseOpenGL();



    sf::Event windowEvent;
    bool run = true;
    while(run)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            t.key[0] = 1;
        else
            t.key[0] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            t.key[1] = 1;
        else
            t.key[1] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            t.key[2] = 1;
        else
            t.key[2] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            t.key[3] = 1;
        else
            t.key[3] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            t.key[4] = 1;
        else
            t.key[4] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            t.key[5] = 1;
        else
            t.key[5] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            t.key[6] = 1;
        else
            t.key[6] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            t.key[7] = 1;
        else
            t.key[7] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            t.key[8] = 1;
        else
            t.key[8] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            t.key[9] = 1;
        else
            t.key[9] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            t.key[10] = 1;
        else
            t.key[10] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            t.key[11] = 1;
        else
            t.key[11] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            t.key[12] = 1;
        else
            t.key[12] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            t.key[13] = 1;
        else
            t.key[13] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            t.key[14] = 1;
        else
            t.key[14] = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
            t.key[15] = 1;
        else
            t.key[15] = 0;






        while (t.gameWindow.pollEvent(windowEvent))
        {
            switch (windowEvent.type)
            {
            case sf::Event::Closed:
                run = false;
                break;
            }

            // t.drawSprite(10,20,4,sprite);
        }
        t.emulateCycle();
        std::this_thread::sleep_for(std::chrono::microseconds(1200));
    }


}
