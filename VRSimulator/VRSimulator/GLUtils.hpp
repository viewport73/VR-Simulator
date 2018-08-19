//
//  GLUtils.hpp
//  VRSimulator
//
//  Created by Pramod S on 18/08/18.
//  Copyright © 2018 Studio73. All rights reserved.
//

#ifndef GLUtils_hpp
#define GLUtils_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdarg.h>

extern int g_gl_width;
extern int g_gl_height;

class GLUtils
{
public:
    GLuint CreateProgramFromFiles( const char *vertexFilename, const char *fragmentFilename );
private:
    bool ParseFileString( const char *filename, char *shaderString, int maxLength );
    void PrintShaderInfoLog( GLuint shaderIndex );
    bool CreateShader( const char *filename, GLuint *shader, GLenum type );
    bool IsProgramValid( GLuint sp );
    bool CreateProgram( GLuint vertex, GLuint fragment, GLuint *program );
    };

#endif /* GLUtils_hpp */
