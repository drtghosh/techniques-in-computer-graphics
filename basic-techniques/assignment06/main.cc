/*===========================================================================*/
/*                                                                           *
 * Copyright (c) 2020, Computer Graphics Group RWTH Aachen University        *
 *                            All rights reserved                            *
 *                                                                           *
 * Basic Techniques in Computer Graphics Exercise                            *
 *                            DO NOT EDIT THIS FILE!                         *
 *                                                                           */
/*===========================================================================*/

#include "assignment.hh"

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>

/// decide whether to build solution or task. only works if you have the solution file :^)
#if VIEW_SOLUTIONS
using namespace solution;
#else
using namespace task;
#endif


namespace
{
auto windowWidth = 512u;
auto windowHeight = 512u;
GLFWwindow* window;

GLuint vs, fs, prog;
const char *vsrc = "#version 150\n in vec2 aPosition; \n void main() { gl_Position = vec4(aPosition.x, aPosition.y, -0.5, 1.0); }\n";
const char *fsrc = "#version 150\n out vec4 oColor; \n uniform vec3 color; void main() { oColor = vec4(color, 1.0); }\n";

GLuint array_buffer_triangles;
GLuint vao_triangles;
GLuint array_buffer_triangle_edges;
GLuint vao_triangle_edges;
GLuint array_buffer_polygon_edges;
GLuint vao_polygon_edges;

std::vector<glm::vec2> vertices;
std::vector<int> triangles;
}

/**********************************************************************************************************************
 * Returns true if a window with the desired context could get created.
 * Requested OpenGL version gets set by ACGL defines.
 */
bool createWindow( bool forceOpenGL32 )
{
    /////////////////////////////////////////////////////////////////////////////////////
    // Configure OpenGL context
    //
    if (forceOpenGL32)
    {
		// request OpenGL 3.2, will return a 4.1 context on Mavericks as well
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	}

    // activate multisampling (second parameter is the number of samples):
    glfwWindowHint(GLFW_SAMPLES, 16 );

    // request an OpenGL debug context:
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    // define whether the window can get resized:
    glfwWindowHint(GLFW_RESIZABLE, false);

    /////////////////////////////////////////////////////////////////////////////////////
    // try to create an OpenGL context in a window and check the supported OpenGL version:
    //                                                  R,G,B,A, Depth,Stencil
    window = glfwCreateWindow(windowWidth, windowHeight, "Basic Techniques in Computer Graphics", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to open a GLFW window" << std::endl;
        return false;
    }
    glfwMakeContextCurrent(window);

    return true;
}

void initializeOpenGL()
{
    if (!gladLoadGL())
    {
        std::cerr << "[Error] Init of glad failed. Terminating." << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glDebugMessageCallback(common::glDebugOutput, NULL);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}



void resizeCallback( GLFWwindow *, int newWidth, int newHeight )
{
    windowWidth  = newWidth;
    windowHeight = newHeight;

    // define the part of the screen OpenGL should draw to (in pixels):
    glViewport(0, 0, newWidth, newHeight);
}

double polygon_area(const std::vector<glm::vec2>& vertices)
{
    const size_t n = vertices.size();
    double area = 0.0;
    for (int i = 0; i < n; ++i)
        area += 0.5 * (vertices[i][0] * vertices[(i+1) % n][1] - vertices[i][1] * vertices[(i+1) % n][0]);

    return area;
}

double triangulation_area(const std::vector<glm::vec2>& vertices, const std::vector<int>& triangles)
{
    double area = 0.0;
    for (int i = 0; i < triangles.size(); i+=3)
    {
        glm::vec2 r = vertices[triangles[i+2]] - vertices[triangles[i+1]];
        glm::vec2 l = vertices[triangles[i]] - vertices[triangles[i+1]];
        area += 0.5 * (r[0] * l[1] - r[1] * l[0]);
    }

    return area;
}

bool prepareExercise() {
    // prepare the shaders:
    vs = glCreateShader( GL_VERTEX_SHADER );
    fs = glCreateShader( GL_FRAGMENT_SHADER );
    prog = glCreateProgram();
    
    // at least OpenGL 3.2
    glShaderSource( vs, 1, &vsrc, NULL );
    glShaderSource( fs, 1, &fsrc, NULL );
    
    glCompileShader(vs);
    common::checkCompileErrors(vs);
    glCompileShader(fs);
    common::checkCompileErrors(fs);
    
    glAttachShader( prog, vs );
    glAttachShader( prog, fs );
    
    glLinkProgram( prog );
    common::checkLinkErrors(prog);

    glUseProgram( prog );
    
    if (glGetError() != GL_NO_ERROR)
    {
        std::cerr << "could not prepare shaders" << std::endl;
        return false;
    }

    glGenVertexArrays( 1, &vao_triangles );
    glGenBuffers( 1, &array_buffer_triangles );

    glGenVertexArrays( 1, &vao_triangle_edges );
    glGenBuffers( 1, &array_buffer_triangle_edges );

    glGenVertexArrays( 1, &vao_polygon_edges );
    glGenBuffers( 1, &array_buffer_polygon_edges );

    if (glGetError() != GL_NO_ERROR)
    {
        std::cerr << "could not prepare geometry" << std::endl;
        return false;
    }
    
    return true;
}

void setUpScene(int scene_idx)
{
    vertices.clear();
    triangles.clear();

    // Init vertices
    if (scene_idx == 1)
    {
        vertices.resize(6);
        int i = 0;
        vertices[i++] = glm::vec2(-0.8, 0.5);
        vertices[i++] = glm::vec2(-0.8, -0.5);
        vertices[i++] = glm::vec2(0.0, -0.2);
        vertices[i++] = glm::vec2(0.8, -0.5);
        vertices[i++] = glm::vec2(0.8, 0.5);
        vertices[i++] = glm::vec2(0.0, 0.2);
    }
    else if (scene_idx == 2)
    {
        const int n = 12; // even
        vertices.resize(n);

        for (int i = 0; i < n; ++i)
        {
            const double angle = (double)i / (double)n * 2.0 * M_PI;
            const double r = i % 2 == 0 ? 0.3 : 0.8;
            vertices[i] = glm::vec2(r * cos(angle), r * sin(angle));
        }
    }
    else if (scene_idx == 3)
    {
        vertices.resize(8);
        int i = 0;
        vertices[i++] = glm::vec2(-0.8, 0.8);
        vertices[i++] = glm::vec2(-0.8, -0.8);
        vertices[i++] = glm::vec2(0.8, -0.8);
        vertices[i++] = glm::vec2(0.8, 0.8);
        vertices[i++] = glm::vec2(0.5, 0.8);
        vertices[i++] = glm::vec2(0.5, -0.5);
        vertices[i++] = glm::vec2(-0.5, -0.5);
        vertices[i++] = glm::vec2(-0.5, 0.8);
    }
    else if (scene_idx == 4)
    {
        const int n = 7;
        vertices.resize(n);
        vertices[0] = glm::vec2(0.0, 0.8);
        for (int i = 1; i < n; ++i)
        {
            const double angle = M_PI / 4.0 + (double)i / (double)n * M_PI / 2.0;
            vertices[i] = glm::vec2(-1.75 * cos(angle), 1.75 * sin(angle)) + glm::vec2(0.0, -2.0);
        }
    }
    else if (scene_idx == 5)
    {
        const int n = 64;
        const int p = 4;
        const double b = 0.6;

        vertices.resize(2*n + 1);

        for (int i = 0; i < n; ++i)
        {
            const double angle = (double)i / (double)n * (double)p * 2.0 * M_PI;
            const double r = pow(b, (double)i / (double)n * (double)p);
            vertices[i] = glm::vec2(r * cos(angle), r * sin(angle));
        }

        vertices[n] = glm::vec2(pow(b, p + 0.25), 0.0);

        for (int i = 0; i < n; ++i)
        {
            const double angle = (double)(n-i-1) / (double)n * (double)p * 2.0 * M_PI;
            const double r = pow(b, (double)(n-i-1) / (double)n * (double)p + 0.5);
            vertices[n+i+1] = glm::vec2(r * cos(angle), r * sin(angle));
        }
    }

    // Call student code to triangulate polygon
    std::cout << "Scene " << scene_idx << std::endl;
    triangulate(vertices, triangles);

    // Check result
    if (triangles.size() % 3 != 0)
    {
        std::cerr << "ERROR: size of the triangles vector has to be a multiple of 3." << std::endl;
        exit(1);
    }

    if (triangles.size() != 3 * (vertices.size() - 2))
        std::cerr << "WARNING: Wrong number of triangles." << std::endl;

    for (int i = 0; i < triangles.size(); ++i)
    {
        if (triangles[i] < 0 || triangles[i] > vertices.size())
        {
            std::cerr << "ERROR: Invalid vertex index in triangle vector. Index: " << triangles[i] << std::endl;
            exit(1);
        }
    }

    const double area_polygon = polygon_area(vertices);
    const double area_triangles = triangulation_area(vertices, triangles);
    if (std::abs(area_polygon - area_triangles) > 1e-6)
        std::cerr << "WARNING: Wrong total area of triangulation. "
                  << "Expected " << area_polygon << ", but you generated " << area_triangles << std::endl;

    GLint attributeLocation;
    attributeLocation = glGetAttribLocation( prog, "aPosition" );

    { // Prepare triangle rendering
        int componentsPerVertex = 2;
        std::vector<float> data(componentsPerVertex * triangles.size());
        int data_idx = 0;
        for (int i = 0; i < triangles.size(); ++i)
        {
            data[data_idx++] = vertices[triangles[i]][0];
            data[data_idx++] = vertices[triangles[i]][1];
        }

        glBindVertexArray( vao_triangles );
        glBindBuffer( GL_ARRAY_BUFFER, array_buffer_triangles );
        glBufferData( GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW );
        glVertexAttribPointer( attributeLocation, componentsPerVertex, GL_FLOAT, GL_FALSE, componentsPerVertex*sizeof(float), 0 );
        glEnableVertexAttribArray( attributeLocation );
    }

    { // Prepare triangle edge rendering
        int componentsPerVertex = 2;
        std::vector<float> data(componentsPerVertex * 2 * 3 * triangles.size());
        int data_idx = 0;
        for (int i = 0; i < triangles.size(); i += 3)
        {
            data[data_idx++] = vertices[triangles[i]][0];
            data[data_idx++] = vertices[triangles[i]][1];
            data[data_idx++] = vertices[triangles[i+1]][0];
            data[data_idx++] = vertices[triangles[i+1]][1];

            data[data_idx++] = vertices[triangles[i+1]][0];
            data[data_idx++] = vertices[triangles[i+1]][1];
            data[data_idx++] = vertices[triangles[i+2]][0];
            data[data_idx++] = vertices[triangles[i+2]][1];

            data[data_idx++] = vertices[triangles[i+2]][0];
            data[data_idx++] = vertices[triangles[i+2]][1];
            data[data_idx++] = vertices[triangles[i]][0];
            data[data_idx++] = vertices[triangles[i]][1];
        }

        glBindVertexArray( vao_triangle_edges );
        glBindBuffer( GL_ARRAY_BUFFER, array_buffer_triangle_edges );
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer( attributeLocation, componentsPerVertex, GL_FLOAT, GL_FALSE, componentsPerVertex*sizeof(float), 0 );
        glEnableVertexAttribArray( attributeLocation );
    }

    { // Prepare polygon edge rendering
        int componentsPerVertex = 2;
        const size_t n = vertices.size();
        std::vector<float> data(componentsPerVertex * 2 * n);
        for (int i = 0; i < n; ++i)
        {
            data[componentsPerVertex * 2 * i] = vertices[i][0];
            data[componentsPerVertex * 2 * i + 1] = vertices[i][1];
            data[componentsPerVertex * 2 * i + 2] = vertices[(i + 1) % n][0];
            data[componentsPerVertex * 2 * i + 3] = vertices[(i + 1) % n][1];
        }

        glBindVertexArray( vao_polygon_edges );
        glBindBuffer( GL_ARRAY_BUFFER, array_buffer_polygon_edges );
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer( attributeLocation, componentsPerVertex, GL_FLOAT, GL_FALSE, componentsPerVertex*sizeof(float), 0 );
        glEnableVertexAttribArray( attributeLocation );
    }
}

void cleanupExercise()
{
    glDeleteBuffers( 1, &array_buffer_triangles );
    glDeleteVertexArrays( 1, &vao_triangles );
    glDeleteBuffers( 1, &array_buffer_polygon_edges );
    glDeleteVertexArrays( 1, &vao_polygon_edges );
    glDeleteBuffers( 1, &array_buffer_triangle_edges );
    glDeleteVertexArrays( 1, &vao_triangle_edges );

    glDeleteProgram( prog );
    glDeleteShader( vs );
    glDeleteShader( fs );
}

void draw( int sceneToDraw, double runTime )
{
    glDisable(GL_DEPTH_TEST);

    { // Render triangles
        GLint uniformLocation = glGetUniformLocation( prog, "color" );
        glUniform3f( uniformLocation, 232.0f / 255.0f, 241.0f / 255.0f, 250.0f / 255.0f );
        glBindVertexArray(vao_triangles);
        glDrawArrays( GL_TRIANGLES, 0, triangles.size() );
    }

    { // Render triangle outline
        glEnable(GL_LINE_SMOOTH);
        GLint uniformLocation = glGetUniformLocation( prog, "color" );
        glUniform3f( uniformLocation, 0.0f / 255.0f, 84.0f / 255.0f, 159.0f / 255.0f );
        glBindVertexArray(vao_triangle_edges);
        glDrawArrays( GL_LINES, 0, 2 * triangles.size() );
    }

    { // Render polygon outline
        glEnable(GL_LINE_SMOOTH);
        glPointSize(10);
        GLint uniformLocation = glGetUniformLocation( prog, "color" );
        glUniform3f( uniformLocation, 0.0f / 255.0f, 84.0f / 255.0f, 159.0f / 255.0f );
        glBindVertexArray(vao_polygon_edges);
        glDrawArrays( GL_LINES, 0, 2 * vertices.size() );
        glDrawArrays( GL_POINTS, 0, 2 * vertices.size() );
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int main( int argc, char *argv[] )
{
    // Initialise GLFW
    if (!glfwInit())
    {
        std::cerr << "[Error] Init of GLFW failed. Terminating." << std::endl;
        glfwTerminate();
        return -1;
    }

    window = common::createWindow(windowWidth, windowHeight, "Basic Techniques in Computer Graphics - Assignment 4");

    if (!window)
        std::cerr << "[Error] Window could not be created!" << std::endl;

    if (!common::init(window))
    {
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    glfwSetWindowSizeCallback(window, resizeCallback);

    // Enable vertical sync (on cards that support it)
    // vertical sync
    int vSync = 1;
    glfwSwapInterval(vSync);

    bool exitProgram = false;
    bool holdDownAKey = false;
    bool holdDownBKey = false;
    bool holdDownCKey = false;
    bool holdDownDKey = false;
    bool holdDownEKey = false;
    int sceneToDraw = 1;
    glGetError(); // clear errors

    if (!prepareExercise())
        exit(-1);

    initCustomResources();

    setUpScene(sceneToDraw);
	
    double startTimeInSeconds = glfwGetTime();
    do {
        glfwPollEvents();
        
        if ((glfwGetKey(window, 'A') == GLFW_PRESS) && (!holdDownAKey)) {
            holdDownAKey = true;
            sceneToDraw  = 1;
            setUpScene(sceneToDraw);
        }
        if ((glfwGetKey(window, 'A') == GLFW_RELEASE) && (holdDownAKey)) holdDownAKey = false;
        
        if ((glfwGetKey(window, 'B') == GLFW_PRESS) && (!holdDownBKey)) {
            holdDownBKey = true;
            sceneToDraw  = 2;
            setUpScene(sceneToDraw);
        }
        if ((glfwGetKey(window, 'B') == GLFW_RELEASE) && (holdDownBKey)) holdDownBKey = false;
        
        if ((glfwGetKey(window, 'C') == GLFW_PRESS) && (!holdDownCKey)) {
            holdDownCKey = true;
            sceneToDraw  = 3;
            setUpScene(sceneToDraw);
        }
        if ((glfwGetKey(window, 'C') == GLFW_RELEASE) && (holdDownCKey)) holdDownCKey = false;

        if ((glfwGetKey(window, 'D') == GLFW_PRESS) && (!holdDownDKey)) {
            holdDownDKey = true;
            sceneToDraw  = 4;
            setUpScene(sceneToDraw);
        }
        if ((glfwGetKey(window, 'D') == GLFW_RELEASE) && (holdDownDKey)) holdDownDKey = false;

        if ((glfwGetKey(window, 'E') == GLFW_PRESS) && (!holdDownEKey)) {
            holdDownEKey = true;
            sceneToDraw  = 5;
            setUpScene(sceneToDraw);
        }
        if ((glfwGetKey(window, 'E') == GLFW_RELEASE) && (holdDownEKey)) holdDownEKey = false;

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        double runTime = glfwGetTime() - startTimeInSeconds;
        draw( sceneToDraw, runTime );

        // Swap buffers
        glfwSwapBuffers( window );
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS) exitProgram = true;
        
    } // Check if the window was closed
    while( !glfwWindowShouldClose( window ) && !exitProgram );
    
    // clean up:
    deleteCustomResources();
    cleanupExercise();
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    exit(0);
}
