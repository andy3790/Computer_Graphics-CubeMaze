#pragma once
#include "includer.h"
#include "Figure.h"

bool CrashCheck_AABB(float a_midx, float a_midy, float a_midz, float a_sizex, float a_sizey, float a_sizez,
	float b_midx, float b_midy, float b_midz, float b_sizex, float b_sizey, float b_sizez); // 기본 충돌 체크 함수

bool Shuffle_Cube(Cube* cube, int cubeSize, float degree);

float dot(glm::vec3 v_1, glm::vec3 v_2);
float dot(glm::vec4 v_1, glm::vec4 v_2);

glm::vec3 cross(glm::vec3 a, glm::vec3 b);
glm::vec4 cross(glm::vec4 a, glm::vec4 b);