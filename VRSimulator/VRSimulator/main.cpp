//
//  main.cpp
//  VRSimulator
//
//  Created by Pramod S on 18/08/18.
//  Copyright © 2018 Studio73. All rights reserved.
//
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <stb_image.h>

#include "GLUtils.hpp"
#include "InputSystem.hpp"

#define TEXTURE_VS "texture.vs"
#define TEXTURE_FS "texture.fs"

#define CYLINDERPROJ_VS "CylinderProj.vs"
#define CYLINDERPROJ_FS "CylinderProj.fs"

// vao and vbo handle
GLuint vao, vbo[2], ibo[2];

GLUtils *utils = new GLUtils();
InputSystem input;
Camera *camera = new Camera();

void ProcessInput(GLFWwindow *window)
{
    // Use the GLFW function to check for the user pressing the Escape button, as well as a window close event.
    if ( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window) != 0 )
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    
    // Below we check if the UP, DOWN, LEFT, RIGHT or W,A,S,D keys are pressed, and send the InputCode
    // down to our InputManager so we can know when to move the camera.
    if ( glfwGetKey(window, GLFW_KEY_UP) || glfwGetKey(window, GLFW_KEY_W) )
        input.KeyPressed(InputCodes::Up);
    if ( glfwGetKey(window, GLFW_KEY_DOWN) || glfwGetKey(window, GLFW_KEY_S) )
        input.KeyPressed(InputCodes::Down);
    if ( glfwGetKey(window, GLFW_KEY_LEFT) || glfwGetKey(window, GLFW_KEY_A) )
        input.KeyPressed(InputCodes::Left);
    if ( glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D) )
        input.KeyPressed(InputCodes::Right);
    
    // Create some variables to store the current mouse position
    double mouseX, mouseY;
    
    // Grab the current mouse position from our window
    glfwGetCursorPos(window, &mouseX, &mouseY);
    
    // If the mouse moved then send it to our InputManager to tell the camera
    if ( mouseX != 0 && mouseY != 0 )
    {
        // Send the updated mouse position to our InputManager
        input.MouseMoved((float)mouseX, (float)mouseY);
    }
    
    // Set the window's cursor position back to 0,0 (top left corner) so we keep getting a delta
    //glfwSetCursorPos(window, 0, 0);
    
    // Poll the input events to see if the user quit or closed the window
    glfwPollEvents();
}

unsigned char *GetTextureData(const char *filename)
{
    int width, height, n;
    unsigned char * data = stbi_load(filename, &width, &height, &n, 0);
    if(!data)
    {
        printf("Error, while loading texture: %s\n", stbi_failure_reason());
    }
    
    return data;
}

GLuint InitializeTexture(GLfloat *vertexData, GLuint ibo, int width, int height, const char *filename, int index)
{
    // fill with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 5, vertexData, GL_STATIC_DRAW);

    // set up generic attrib pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (char*) 0 + 0 * sizeof(GLfloat));
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (char*) 0 + 3 * sizeof(GLfloat));
    
    
    // generate and bind the index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    
    GLuint indexData[] = {
        0,1,2, // first triangle
        2,1,3, // second triangle
    };
    
    // fill with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 2 * 3, indexData, GL_STATIC_DRAW);
    
    // "unbind" vao
    glBindVertexArray(0);
    
    // texture handle
    GLuint texture;
    
    // generate texture
    glGenTextures(1, &texture);
    
    // bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);
    
    //Load Image
    unsigned char *imgData = GetTextureData(filename);
    
    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // set texture content
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
    
    return texture;
}

// helper to check and display for shader compiler errors
bool check_shader_compile_status(GLuint obj) {
    GLint status;
    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        GLint length;
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}

// helper to check and display for shader linker error
bool check_program_link_status(GLuint obj) {
    GLint status;
    glGetProgramiv(obj, GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        GLint length;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetProgramInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}

int main() {
    int width = 1440;
    int height = 1440;
    
    if(glfwInit() == GL_FALSE) {
        std::cerr << "failed to init GLFW" << std::endl;
        return 1;
    }
    
    // select opengl version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // create a window
    GLFWwindow *window;
    if((window = glfwCreateWindow(1200, 800, "03texture", NULL, NULL)) == 0) {
        std::cerr << "failed to open window" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    GLuint program = utils->CreateProgramFromFiles(CYLINDERPROJ_VS, CYLINDERPROJ_FS);
    // get texture uniform location
    GLint texture_location = glGetUniformLocation(program, "tex");
    GLint projMatrixLocation = glGetUniformLocation(program, "projMatrix");
    GLint modelViewMatrixLocation = glGetUniformLocation(program, "mvMatrix");
    GLint mvpMatrixLocation = glGetUniformLocation(program, "mvpMatrix");
    GLint mvrMatrixLocation = glGetUniformLocation(program, "mvrMatrix");
    
    // generate and bind the vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
//    glGenVertexArrays(1, &vao[1]);
//    glBindVertexArray(vao[1]);
    
    // generate and bind the vertex buffer object
    glGenBuffers(1, &vbo[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    
    glGenBuffers(1, &vbo[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    
    // data for a quad (this time with texture coords)
    GLfloat leftVertexData[] = {
        //  X     Y     Z           U     V
        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, // vertex 0
        -1.0f, 0.5f, 0.0f,       1.0f, 0.0f, // vertex 1
        0.0f,-0.5f, 0.0f,       0.0f, 1.0f, // vertex 2
        -1.0f,-0.5f, 0.0f,       1.0f, 1.0f, // vertex 3
    }; // 4 vertices with 5 components (floats) each
    
    GLfloat rightVertexData[] = {
        //  X     Y     Z           U     V
        1.0f, 0.5f, 0.0f,       0.0f, 0.0f, // vertex 0
        0.0f, 0.5f, 0.0f,       1.0f, 0.0f, // vertex 1
        1.0f,-0.5f, 0.0f,       0.0f, 1.0f, // vertex 2
        0.0f,-0.5f, 0.0f,       1.0f, 1.0f, // vertex 3
    }; // 4 vertices with 5 components (floats) each
    
    GLuint leftTexture = InitializeTexture( leftVertexData, ibo[0], width, height, "left.png", 0 );
    //GLuint rightTexture = InitializeTexture( rightVertexData, ibo[1], width, height, "right.png", 1 );
    
    camera->SetPerspective(glm::radians(60.0f), 1440 / (float)1440, 0.01f, 1000);
    camera->PositionCamera(0, 0, 6, 0, 0);
    input.SetCamera(camera);
    
    glm::mat4 projection = camera->GetProjectionMatrix();
    glm::mat4 viewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 view = viewTranslate;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 modelView = model * view;
    glm::mat4 mvp = projection * view * model;
    glm::mat4 mvr = modelView * camera->GetRotationMatrix();
    
    
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ProcessInput(window);
        
        // clear first
        glClear(GL_COLOR_BUFFER_BIT);
        
        // use the shader program
        glUseProgram(program);
        
        glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, &modelView[0][0]);
        glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(mvrMatrixLocation, 1, GL_FALSE, &mvr[0][0]);
        
        // bind texture to texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, leftTexture);
        
        // set texture uniform
        glUniform1i(texture_location, 0);
        
        // bind the vao
        glBindVertexArray(vao);
        
        // draw
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        //glBindTexture(GL_TEXTURE_2D, rightTexture);
        
        // set texture uniform
        //glUniform1i(texture_location, 0);
        
        //glBindVertexArray(vao[1]);
        
        // draw
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
            std::cerr << error << std::endl;
            break;
        }
        
        // finally swap buffers
        glfwSwapBuffers(window);
    }
    
    // delete the created objects
    
    glDeleteTextures(1, &leftTexture);
    //glDeleteTextures(1, &rightTexture);
    
    glDeleteVertexArrays(1, &vao);
//    glDeleteVertexArrays(1, &vao[1]);
    glDeleteBuffers(1, &vbo[0]);
    glDeleteBuffers(1, &ibo[0]);
    glDeleteBuffers(1, &vbo[1]);
    glDeleteBuffers(1, &ibo[1]);
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
