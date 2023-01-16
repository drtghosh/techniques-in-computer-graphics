/*===========================================================================*/
/*                                                                           *
 * Copyright (c) 2020, Computer Graphics Group RWTH Aachen University        *
 *                            All rights reserved                            *
 *                                                                           *
 * Basic Techniques in Computer Graphics Exercise                            *
 *                            DO NOT EDIT THIS FILE!                         *
 *                                                                           */
/*===========================================================================*/

#pragma once

#include <glad/glad.h>
/// make sure inclusion of glad.h does not come after glfw

#include <GLFW/glfw3.h>

// OpenGL Math:
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Tools/ABReader.hh"
#include "Tools/GenericCamera.hh"
#include "Tools/HelperFunctions.hh"
#include "Tools/PNGReader.hh"
#include "Tools/TextureData.hh"
#include "Tools/VertexArrayObject.hh"

// Resources
extern ShaderProgram* g_shader;

extern GenericCamera g_camera;

extern VertexArrayObject* g_vaoBunny;
extern VertexArrayObject* g_vaoEarth;


glm::mat4 buildFrustum(float phiInDegree, float aspectRatio, float near, float far);

void resizeCallback(int newWidth, int newHeight);

void setupShaderUniforms(GLuint _program, int _scene);

void createShaderProgram(GLuint& vs, GLuint& fs, GLuint& prog, const std::string& vsFileName, const std::string& fsFileName);

void printGLSLCompileLog(GLuint shaderHandle);
void printGLSLLinkLog(GLuint progHandle);
void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);
