/*===========================================================================*/
/*                                                                           *
 * Copyright (c) 2020, Computer Graphics Group RWTH Aachen University        *
 *                            All rights reserved                            *
 *                                                                           *
 * Basic Techniques in Computer Graphics Exercise                            *
 *                                                                           */
/*===========================================================================*/

#include "assignment.hh"

void task::drawScene(int scene, float runTime) 
{ 
	/*
	a) Drawing the track using circles of diiferent radius
	*/
	drawCircle(glm::vec3(0.0f, 0.0f, 1.0f), glm::mat4(0.9f, 0.0f, 0.0f, 0.0f, 0.0f, 0.9f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)); //creating blue_bigger_matrix
	drawCircle(glm::vec3(0.25f, 0.25f, 0.25f), glm::mat4(0.88f, 0.0f, 0.0f, 0.0f, 0.0f, 0.88f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)); //creating grey_inner_matrix
	drawCircle(glm::vec3(0.0f, 0.0f, 1.0f), glm::mat4(0.75f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)); //creating blue_smaller_matrix
	drawCircle(glm::vec3(0.0f, 0.0f, 0.0f), glm::mat4(0.73f, 0.0f, 0.0f, 0.0f, 0.0f, 0.73f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)); //creating black_innermost_matrix

	/*
	b) Drawing a stand for spectators on the left
	*/
	glm::mat4 translate_stand = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -0.94f, 0.0f, 0.0f, 1.0f);
	glm::mat4 stand = translate_stand * glm::mat4(0.03f, 0.0f, 0.0f, 0.0f, 0.0f, 0.66f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	drawCircle(glm::vec3(0.5f, 0.5f, 0.5f), stand);

	/*
	Drawing a start/finish line as nine white ellipses
	*/
	for (int l = 0; l < 9; l++) 
	{
		glm::mat4 translate_line = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -0.865f + (float(l) / 75), 0.0f, 0.0f, 1.0f);
		glm::mat4 line = translate_line * glm::mat4(0.004f, 0.0f, 0.0f, 0.0f, 0.0f, 0.025f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		drawCircle(glm::vec3(1.0f, 1.0f, 1.0f), line);
	}

	/*
	Drawing a white dotted line between two lanes as ellipses
	*/
	for (int a = 0; a < 360; a += 12) 
	{
		glm::mat4 rotate_dotted = glm::mat4(cos(float(a) * M_PI / 180.0f), sin(float(a) * M_PI / 180.0f), 0.0f, 0.0f, -sin(float(a) * M_PI / 180.0f), cos(float(a) * M_PI / 180.0f), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 translate_dotted = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.815f, 0.0f, 1.0f);
		glm::mat4 dotted_line = rotate_dotted * translate_dotted * glm::mat4(0.024f, 0.0f, 0.0f, 0.0f, 0.0f, 0.006f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		drawCircle(glm::vec3(1.0f, 1.0f, 1.0f), dotted_line);
	}

	/*
	Adding two race cars with outer car twice as fast (in terms of angular rotation) with clockwise rotation
	*/
	glm::mat4 rotate_outside = glm::mat4(cos(fmod((0.0f - runTime * 20), 360) * M_PI / 180.0f), sin(fmod((0.0f - runTime * 20), 360) * M_PI / 180.0f), 0.0f, 0.0f, -sin(fmod((0.0f - runTime * 20), 360) * M_PI / 180.0f), cos(fmod((0.0f - runTime * 20), 360) * M_PI / 180.0f), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 rotate_inside = glm::mat4(cos(fmod((0.0f - runTime * 10), 360) * M_PI / 180.0f), sin(fmod((0.0f - runTime * 10), 360) * M_PI / 180.0f), 0.0f, 0.0f, -sin(fmod((0.0f - runTime * 10), 360) * M_PI / 180.0f), cos(fmod((0.0f - runTime * 10), 360) * M_PI / 180.0f), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 translate_outside = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -0.8475f, 0.0f, 0.0f, 1.0f);
	glm::mat4 translate_inside = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -0.7825f, 0.0f, 0.0f, 1.0f);
	glm::mat4 outside_car = rotate_outside * translate_outside * glm::mat4(0.015f, 0.0f, 0.0f, 0.0f, 0.0f, 0.06f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 inside_car = rotate_inside * translate_inside * glm::mat4(0.015f, 0.0f, 0.0f, 0.0f, 0.0f, 0.06f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	drawCircle(glm::vec3(0.0f, 1.0f, 0.0f), outside_car);
	drawCircle(glm::vec3(1.0f, 1.0f, 0.0f), inside_car);
}
void task::initCustomResources() {}
void task::deleteCustomResources() {}
