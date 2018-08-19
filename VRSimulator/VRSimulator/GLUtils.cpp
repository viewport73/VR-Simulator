//
//  GLUtils.cpp
//  VRSimulator
//
//  Created by Pramod S on 18/08/18.
//  Copyright © 2018 Studio73. All rights reserved.
//

#include "GLUtils.hpp"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SHADER_LENGTH 262144

bool GLUtils::ParseFileString( const char *filename, char *shaderString, int maxLength )
{
    shaderString[0] = '\0';
    FILE *file = fopen( filename, "r" );
    if ( !file )
    {
        printf( "ERROR: opening file for reading: %s\n", filename );
        return false;
    }
    int current_len = 0;
    char line[2048];
    strcpy( line, "" );
    while ( !feof( file ) )
    {
        if ( NULL != fgets( line, 2048, file ) )
        {
            current_len += strlen( line );
            if ( current_len >= maxLength )
            {
                printf( "ERROR: shader length is longer than string buffer length %i\n", maxLength );
            }
            strcat( shaderString, line );
        }
    }
    if ( EOF == fclose( file ) )
    {
        printf( "ERROR: closing file from reading %s\n", filename );
        return false;
    }
    return true;
}

void GLUtils::PrintShaderInfoLog( GLuint shaderIndex )
{
    int maxlength = 2048;
    int actuallength = 0;
    char log[2048];
    glGetShaderInfoLog( shaderIndex, maxlength, &actuallength, log );
    printf( "shader info log for GL index %i:\n%s\n", shaderIndex, log );
}

bool GLUtils::CreateShader( const char *filename, GLuint *shader, GLenum type )
{
    printf( "creating shader from %s...\n", filename );
    char shaderString[MAX_SHADER_LENGTH];
    ParseFileString( filename, shaderString, MAX_SHADER_LENGTH );
    *shader = glCreateShader( type );
    const GLchar *p = (const GLchar *)shaderString;
    glShaderSource( *shader, 1, &p, NULL );
    glCompileShader( *shader );
    int params = -1;
    glGetShaderiv( *shader, GL_COMPILE_STATUS, &params );
    if ( GL_TRUE != params )
    {
        printf( "ERROR: GL shader index %i did not compile\n", *shader );
        PrintShaderInfoLog( *shader );
        return false;
    }
    printf( "shader compiled. index %i\n", *shader );
    return true;
}

bool GLUtils::IsProgramValid( GLuint sp )
{
    glValidateProgram( sp );
    GLint params = -1;
    glGetProgramiv( sp, GL_VALIDATE_STATUS, &params );
    if ( GL_TRUE != params )
    {
        printf( "program %i GL_VALIDATE_STATUS = GL_FALSE\n", sp );
        return false;
    }
    printf( "program %i GL_VALIDATE_STATUS = GL_TRUE\n", sp );
    return true;
}

bool GLUtils::CreateProgram( GLuint vertex, GLuint fragment, GLuint *program )
{
    int infoLogLength = 0;
    *program = glCreateProgram();
    printf( "created programme %u. attaching shaders %u and %u...\n", *program, vertex, fragment );
    glAttachShader( *program, vertex );
    glAttachShader( *program, fragment );
    glLinkProgram( *program );
    GLint params = -1;
    glGetProgramiv( *program, GL_LINK_STATUS, &params );
    if ( GL_TRUE != params )
    {
        glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &infoLogLength);
        char programLog[infoLogLength];
        glGetProgramInfoLog(*program, infoLogLength, NULL, &programLog[0]);
        printf( "Pprogram Link Error: %s", &programLog[0]);
        
        printf( "ERROR: could not link shader programme GL index %u\n", *program);
        return false;
    }
    IsProgramValid( *program );
    glDeleteShader( vertex );
    glDeleteShader( fragment );
    return true;
}

GLuint GLUtils::CreateProgramFromFiles( const char *vertexFilename, const char *fragmentFilename )
{
    GLuint vertex, fragment, program;
    CreateShader( vertexFilename, &vertex, GL_VERTEX_SHADER );
    CreateShader( fragmentFilename, &fragment, GL_FRAGMENT_SHADER );
    CreateProgram( vertex, fragment, &program );
    return program;
}
