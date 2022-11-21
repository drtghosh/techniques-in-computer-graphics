/*===========================================================================*/
/*                                                                           *
 * Copyright (c) 2020, Computer Graphics Group RWTH Aachen University        *
 *                            All rights reserved                            *
 *                                                                           *
 * Basic Techniques in Computer Graphics Exercise                            *
 *                                                                           *
 *                                                                           */
/*===========================================================================*/

#include "assignment.hh"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <iostream>

namespace
{
glm::mat4 viewMatrix(1.f);
glm::mat4 projectionMatrix(1.f);

glm::mat4 getRotationMatrixZAxis(float angle)
{
    glm::mat4 r(1.f);
    r[0][0] = std::cos(angle);
    r[1][0] = -std::sin(angle);
    r[0][1] = std::sin(angle);
    r[1][1] = std::cos(angle);
    r[2][2] = 1.0;
    r[3][3] = 1.0;
    return r;
}

glm::mat4 getRotationMatrixYAxis(float angle)
{
    glm::mat4 r(1.f);
    r[0][0] = std::cos(angle);
    r[2][0] = std::sin(angle);
    r[1][1] = 1.0;
    r[0][2] = -std::sin(angle);
    r[2][2] = std::cos(angle);
    r[3][3] = 1.0;
    return r;
}


/*
 * A white circle represents the track center line
 */
void drawTrackMarks(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    drawCircle(glm::vec3(1.0, 1.0, 1.0), model, view, projection);
}

/*
 * A Blue circle represents the tracks outer boundary
 */
void drawOuterTrackMarks(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    drawCircle(glm::vec3(0.0, 0.0, 1.0), model, view, projection);
}

/*
 * A gray circle representing the grandstand
 */
void drawGrandstand(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    drawCircle(glm::vec3(0.4, 0.4, 0.4), model, view, projection);
}


void drawTrack()
{
    for (unsigned int i = 0; i < 30; ++i)
    {
        // Compute rotation for the marks
        float angle = float(i) * 4.0f * M_PI / 60.0f;

        float next_angle = float(i + 1) * 4.0f * M_PI / 60.0f;

        float height = std::sin(angle * 2.0) * 0.1;

        float next_height = std::sin(float(i + 1) * 4.0f * M_PI / 60.0f * 2.0) * 0.1;

        glm::mat4 markRotation = getRotationMatrixZAxis(angle);

        // =====================================================
        // Track Center
        // =====================================================

        // Scaling
        glm::mat4 scaleTrack(1.f);
        scaleTrack[0][0] = 0.03;
        scaleTrack[1][1] = 0.01;
        scaleTrack[2][2] = 0.01;

        // translate
        glm::mat4 translateTrack(1.f);
        translateTrack[3][1] = 0.8;    // Radius of the circle
        translateTrack[3][2] = height; // Height

        // rotate around y axis to follow track height change
        glm::mat4 heightRotation = getRotationMatrixYAxis(-(M_PI / 2.0 - std::acos((height - next_height) / (2.0 * std::sin((next_angle - angle) / 2.0)))));

        drawTrackMarks(markRotation * translateTrack * heightRotation * scaleTrack, viewMatrix, projectionMatrix);

        // =====================================================
        // Track Boundaries
        // =====================================================

        // Outer Circle
        translateTrack[3][1] = 0.9; // Radius of the circle

        drawOuterTrackMarks(markRotation * translateTrack * heightRotation * scaleTrack, viewMatrix, projectionMatrix);

        // Inner Circle
        translateTrack[3][1] = 0.7; // Radius of the circle

        drawOuterTrackMarks(markRotation * translateTrack * heightRotation * scaleTrack, viewMatrix, projectionMatrix);
    }
}

void drawGrandstand()
{
    glm::mat4 transformStand(1.f);

    // Scale circle to form grandstand
    transformStand[0][0] = 0.05;
    transformStand[1][1] = 0.6;
    transformStand[2][2] = 0.07;

    // Place on the left of the track
    transformStand[3][0] = -0.95;

    drawGrandstand(transformStand, viewMatrix, projectionMatrix);
}

void drawFinish()
{
    glm::mat4 transformFinish(1.f);

    // Scaling
    transformFinish[0][0] = 0.005;
    transformFinish[1][1] = 0.03;
    transformFinish[2][2] = 0.01;

    // Initialize translation (Left boundary of track)
    transformFinish[3][0] = -0.9;

    // Draw nine marks from left to right
    for (unsigned int i = 0; i < 9; ++i)
    {
        // Translate to next mark
        transformFinish[3][0] += 0.02;

        drawTrackMarks(transformFinish, viewMatrix, projectionMatrix);
    }
}

void drawCar(float angle, int lane, glm::vec3 color)
{
    // Start with identity
    glm::mat4 transformCar(1.f);
    float height = std::sin(angle * 2.0) * 0.1;

    // Scale second car to fit onto track
    transformCar[0][0] *= 0.04;
    transformCar[1][1] *= 0.1;
    transformCar[2][2] *= 0.03;
    transformCar[3][2] = -height;

    // Transform second car to position 0 (x-direction)
    if (lane == 1)
    {
        transformCar[3][0] = -0.85;
    }
    else
    {
        transformCar[3][0] = -0.75;
    }

    glm::mat4 rotation = getRotationMatrixZAxis(angle);
    rotation *= transformCar;

    drawCircle(color, rotation, viewMatrix, projectionMatrix);
}


glm::mat4 buildFrustum(float phiInDegree, float aspectRatio, float near, float far)
{
    glm::mat4 fm(1.f);

    // ====================================================================
    // buildFrustum function for programming exercise part b:
    // Add your code here:
    // ====================================================================
    float top = near * tan((M_PI / 180.0f) * (phiInDegree / 2.0f));
    float bottom = -1.0f * top;
    float right = aspectRatio * top;
    float left = -1.0f * right;

    fm[0][0] = 2.0f * near / (right - left);
    fm[1][1] = 2.0f * near / (top - bottom);
    fm[2][2] = -1.0f * (far + near) / (far - near);
    fm[3][3] = 0.0f;
    fm[2][0] = (right + left) / (right - left);
    fm[2][1] = (top + bottom) / (top - bottom);
    fm[2][3] = -1.0f;
    fm[3][2] = -2.0f * far * near / (far - near);

    // ====================================================================
    // End Exercise code
    // ====================================================================

    return fm;
}

glm::mat4 lookAt(const glm::vec3& camPos, const glm::vec3& viewDirection, const glm::vec3& up)
{
    // ====================================================================
    // Lookat for programming exercise part a:
    // Add your code here:
    // ====================================================================
    float norm_viewDirection = sqrt((viewDirection.x * viewDirection.x) + (viewDirection.y * viewDirection.y) + (viewDirection.z * viewDirection.z));
    glm::vec3 normalized_viewDirection = glm::vec3(viewDirection.x / norm_viewDirection, viewDirection.y / norm_viewDirection, viewDirection.z / norm_viewDirection);
    glm::vec3 rightVector = glm::vec3((viewDirection.y * up.z) - (viewDirection.z * up.y), (viewDirection.z * up.x) - (viewDirection.x * up.z), (viewDirection.x * up.y - viewDirection.y * up.x));
    float norm_rightVector = sqrt((rightVector.x * rightVector.x) + (rightVector.y * rightVector.y) + (rightVector.z * rightVector.z));
    glm::vec3 normalized_rightVector = glm::vec3(rightVector.x / norm_rightVector, rightVector.y / norm_rightVector, rightVector.z / norm_rightVector);
    glm::vec3 modifiedUp = glm::vec3((rightVector.y * viewDirection.z - rightVector.z * viewDirection.y), (rightVector.z * viewDirection.x - rightVector.x * viewDirection.z), (rightVector.x * viewDirection.y - rightVector.y * viewDirection.x));
    float norm_modifiedUp = sqrt((modifiedUp.x * modifiedUp.x) + (modifiedUp.y * modifiedUp.y) + (modifiedUp.z * modifiedUp.z));
    glm::vec3 normalized_modifiedUp = glm::vec3(modifiedUp.x / norm_modifiedUp, modifiedUp.y / norm_modifiedUp, modifiedUp.z / norm_modifiedUp);

    float dot_rightCamera = (normalized_rightVector.x * camPos.x) + (normalized_rightVector.y * camPos.y) + (normalized_rightVector.z * camPos.z);
    float dot_upCamera = (normalized_modifiedUp.x * camPos.x) + (normalized_modifiedUp.y * camPos.y) + (normalized_modifiedUp.z * camPos.z);
    float dot_directionCamera = (normalized_viewDirection.x * camPos.x) + (normalized_viewDirection.y * camPos.y) + (normalized_viewDirection.z * camPos.z);

    return glm::mat4(normalized_rightVector.x, normalized_modifiedUp.x, -1.0f * normalized_viewDirection.x, 0.0f, normalized_rightVector.y, normalized_modifiedUp.y, -1.0f * normalized_viewDirection.y, 0.0f, normalized_rightVector.z, normalized_modifiedUp.z, -1.0f * normalized_viewDirection.z, 0.0f, -dot_rightCamera, -dot_upCamera, dot_directionCamera, 1.0f);

    // ====================================================================
    // End Exercise code
    // ====================================================================
}

}


void task::resizeCallback(int newWidth, int newHeight)
{
    glViewport(0, 0, newWidth, newHeight);

    // ====================================================================
    // projection matrix setup for programming exercise part d:
    // Add your code here:
    // ====================================================================
    float aspectRatio = (1.0f * newWidth) / newHeight;
    projectionMatrix = buildFrustum(90.0f, aspectRatio, 0.01f, 20.0f);

    // ====================================================================
    // End Exercise code
    // ====================================================================
}

void task::drawScene(int scene, float runTime)
{
    float angle1 = -2.0f * M_PI * runTime / 60.0f;

    if (scene != 4)
    {
        // =====================================================
        // static camera for programming exercise part c:
        // Add your code here:
        // =====================================================
        glm::vec3 cameraPosition_c = glm::vec3(0.0f, -1.0f, 1.0f);
        glm::vec3 viewDirection_c = glm::vec3(0.0f, 0.0f, 0.0f) - cameraPosition_c;
        glm::vec3 upDirection_c = glm::vec3(0.0f, 1.0f, 0.0f);
        viewMatrix = lookAt(cameraPosition_c, viewDirection_c, upDirection_c);

        // =====================================================
        // End Exercise code
        // =====================================================
    }
    else
    {
        // rotate around track for the other parts, looking at the center:
        glm::vec3 pos = glm::vec3(1.5f * std::sin(runTime), 1.5f * std::cos(runTime), 1.0f);
        viewMatrix = lookAt(pos, glm::vec3(0, 0, 0) - pos, glm::cross(glm::cross(glm::vec3(0, 0, 0) - pos, glm::vec3(0, 0, 1)), glm::vec3(0, 0, 0) - pos));
    }

    if (scene == 5)
    {
        float height = -std::sin(angle1 * 2.0) * 0.1;


        // =====================================================
        // Moving camera for programming exercise part e:
        // Add your code here:
        // =====================================================

        glm::vec3 camera_position = glm::vec3(-0.8f, 0.0f, 0.0f);
        glm::vec3 view_direction = glm::vec3(-sin(angle1), cos(angle1), 0.0f);
        glm::vec3 up_direction = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::mat4 viewMatrix = lookAt(camera_position, view_direction, up_direction);

        // =====================================================
        // End Exercise code
        // =====================================================
    }


    // Draw Track (Border and inner track marks)
    drawTrack();

    // Draw finish line
    drawFinish();

    // Draw Grandstand on the left
    drawGrandstand();


    drawCar(angle1, 1, glm::vec3(0, 1, 0));

    float angle2 = 0.5f * -2.0f * M_PI * runTime / 60.0f;
    drawCar(angle2, 2, glm::vec3(1, 1, 0));

    if (scene == 3)
    {
        // optional bonus fun :-D

        // state:
        static float carPosition = 0.0f;
        static float lastRunTime = runTime;
        static float carSpeed = 0.0f;
        static int carLane = 1;

        static float spectatorPos = 0.9f;
        static bool spectatorAlive = true;
        static float spectatorDirection = -1.0f;

        float timeDiff = runTime - lastRunTime;

        // car control:
        if (arrowKeyUpPressed)
            carSpeed += timeDiff;
        if (arrowKeyDownPressed)
            carSpeed -= 2.0f * timeDiff;
        carSpeed = glm::clamp(carSpeed, 0.0f, 1.0f);

        if (arrowKeyRightPressed)
            carLane = 2;
        if (arrowKeyLeftPressed)
            carLane = 1;

        carPosition += -2.0f * M_PI * timeDiff * 0.5 * carSpeed;
        if (carPosition < -2.0 * M_PI)
            carPosition += 2.0 * M_PI;

        // car rendering:
        drawCar(carPosition, carLane, glm::vec3(1, 0, 0));

        // car crash detection:
        float distToCar;
        float crashPos;
        if (carLane == 1)
        {
            distToCar = std::abs(carPosition - angle1);
            crashPos = 0.5 * (carPosition + angle1);
        }
        else
        {
            distToCar = std::abs(carPosition - angle2);
            crashPos = 0.5 * (carPosition + angle2);
        }
        if (distToCar < 0.24f)
        {
            // crash!
            carSpeed = 0.0f;

            // Start with identity
            glm::mat4 crash(1.f);

            // Scale second car to fit onto track
            crash[0][0] *= 0.02 + 0.5 * (0.24f - distToCar);
            crash[1][1] *= 0.02 + 0.5 * (0.24f - distToCar);
            crash[2][2] *= 0.02 + 0.5 * (0.24f - distToCar);

            if (carLane == 1)
            {
                crash[3][0] = -0.85;
            }
            else
            {
                crash[3][0] = -0.75;
            }
            glm::mat4 rotation = getRotationMatrixZAxis(crashPos);
            rotation *= crash;

            drawCircle(glm::vec3(0.8, 0.8, 0.8), rotation, viewMatrix, projectionMatrix);
        }

        float spectatorSize = 0.025;
        float distToSpectator = std::abs(carPosition);
        if (distToSpectator < 0.12f)
        {
            if ((carLane == 1) && (std::abs(spectatorPos + 0.85) < 0.75 * spectatorSize))
            {
                spectatorAlive = false;
            }
            if ((carLane == 2) && (std::abs(spectatorPos + 0.75) < 0.75 * spectatorSize))
            {
                spectatorAlive = false;
            }
        }


        glm::mat4 transformSpectator(1.f);

        // spectator rendering:
        glm::vec3 spectatorColor = glm::vec3(0.8, 0.2, 1.0);
        if (spectatorAlive)
        {
            spectatorPos += 0.03f * timeDiff * spectatorDirection;
            if (spectatorPos <= -0.9f)
            {
                spectatorPos = -0.9f;
                spectatorDirection = 1.0f;
            }
            if (spectatorPos >= -0.7f)
            {
                spectatorPos = -0.7f;
                spectatorDirection = -1.0f;
            }
            transformSpectator[2][2] = 0.06;
        }
        else
        {
            spectatorColor = glm::vec3(0.8, 0.0, 0.0);
            spectatorSize *= 2.0f;
            transformSpectator[2][2] = 0.005;
        }


        transformSpectator[0][0] = spectatorSize;
        transformSpectator[1][1] = spectatorSize;
        transformSpectator[3][0] = spectatorPos;

        drawCircle(spectatorColor, transformSpectator, viewMatrix, projectionMatrix);

        lastRunTime = runTime;
    }
}

void task::initCustomResources() {}

void task::deleteCustomResources() {}
