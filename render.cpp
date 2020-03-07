#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

// Link statically with GLEW
#define GLEW_STATIC

// Headers
#include "render.h"
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include "kimechip.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
#include <iostream>
// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    uniform mat4 transform;
    void main()
    {
        gl_Position = transform * vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core
    out vec4 outColor;
    void main()
    {
        outColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
)glsl";
 bool exactDraw[120][120] {0};
void KimeChip::drawSprite(int X, int Y,int depth,unsigned short toDraw[][64])
{
        // Create and compile the fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glBindFragDataLocation(shaderProgram, 0, "outColor");
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);

        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Link the vertex and fragment shader into a shader program

        GLint modelLocation = glGetUniformLocation(shaderProgram, "transform");


        memset(exactDraw,0,sizeof(exactDraw[0][0]*120*120));

        int k = 0, o = 0;
        for(int i=Y;i<=Y+depth -1;i++)
        {

            for(int j=X;j<X+8;j++)
                exactDraw[j][i] = toDraw[k][o++];
            o = 0;
            k++;
        }

/*
        std::cout<<std::endl;
        for(int i=0;i<64;i++)
        {
            std::cout <<i<<": "<< std::endl;
            for(int j=0;j<32;j++)
                std::cout <<j <<": " << gfx[j][i] <<" | ";

        }
        */


        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(int i=0;i<64;i++)
            for(int j=0;j<32;j++)
        {
            glm::mat4 trans(1.0f);
            trans = glm::translate(trans, glm::vec3(-0.945 + i/35.0f,0.945 - j/18.0f,0.0f));
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(trans));
            if(gfx[i][j])
                glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        // Draw a triangle from the 3 vertices


        // Swap buffers
        gameWindow.display();
}

void KimeChip::initialiseOpenGL()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 2;

    gameWindow.create(sf::VideoMode(1200, 600, 32), "KimeChip", sf::Style::Titlebar | sf::Style::Close, settings);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {

         -0.01f, -0.01f,
         -0.01f, 0.01f,
         0.01f, 0.01f,

         0.01f,  0.01f,
         0.01f, -0.01f,
        -0.01f, -0.01f,



    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
/*
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);



    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    */
    // Specify the layout of the vertex data



}
void KimeChip::destroy()
{
    /*
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    window.close();
    //*/
}

#endif // RENDER_H_INCLUDED
