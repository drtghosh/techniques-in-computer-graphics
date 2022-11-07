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
	//creating blue_bigger_matrix
	drawCircle(glm::vec3(0.0f, 0.0f, 1.0f), glm::mat4(0.9f, 0.0f, 0.0f, 0.0f, 0.0f, 0.9f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
	drawCircle(glm::vec3(0.25f, 0.25f, 0.25f), glm::mat4(0.88f, 0.0f, 0.0f, 0.0f, 0.0f, 0.88f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
	drawCircle(glm::vec3(0.0f, 0.0f, 1.0f), glm::mat4(0.75f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
	drawCircle(glm::vec3(0.0f, 0.0f, 0.0f), glm::mat4(0.73f, 0.0f, 0.0f, 0.0f, 0.0f, 0.73f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));

	glm::mat4 translate_stand = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -0.94f, 0.0f, 0.0f, 1.0f);
	glm::mat4 stand = translate_stand * glm::mat4(0.03f, 0.0f, 0.0f, 0.0f, 0.0f, 0.66f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	drawCircle(glm::vec3(0.5f, 0.5f, 0.5f), stand);

	for (int l = 0; l < 9; l++) 
	{
		glm::mat4 translate_line = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -0.865f + (float(l) / 75), 0.0f, 0.0f, 1.0f);
		glm::mat4 line = translate_line * glm::mat4(0.004f, 0.0f, 0.0f, 0.0f, 0.0f, 0.025f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		drawCircle(glm::vec3(1.0f, 1.0f, 1.0f), line);
	}
}
void task::initCustomResources() {}
void task::deleteCustomResources() {}
