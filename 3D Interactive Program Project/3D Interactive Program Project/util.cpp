#include "util.h"

bool CrashCheck_AABB(float a_midx, float a_midy, float a_midz, float a_sizex, float a_sizey, float a_sizez,
	float b_midx, float b_midy, float b_midz, float b_sizex, float b_sizey, float b_sizez) // 기본 충돌 체크 함수
{
	float a_min[3];
	float a_max[3];
	float b_min[3];
	float b_max[3];
	a_min[0] = a_midx - a_sizex;
	a_min[1] = a_midy - a_sizey;
	a_min[2] = a_midz - a_sizez;
	a_max[0] = a_midx + a_sizex;
	a_max[1] = a_midy + a_sizey;
	a_max[2] = a_midz + a_sizez;
	b_min[0] = b_midx - b_sizex;
	b_min[1] = b_midy - b_sizey;
	b_min[2] = b_midz - b_sizez;
	b_max[0] = b_midx + b_sizex;
	b_max[1] = b_midy + b_sizey;
	b_max[2] = b_midz + b_sizez;

	for (int i = 0; i < 3; i++) {
		if (a_min[i] > b_max[i] || a_max[i] < b_min[i]) {
			return false;
		}
	}
	return true;
}

bool Shuffle_Cube(Cube* cube, int cubeSize, float degree) {
	static std::random_device rd;
	static std::default_random_engine dre(rd());
	static std::uniform_int_distribution<> uid;

	static int suffleCount = uid(dre) % 10 + 25;
	static bool rotFlag = CUBE_SEQUENCE_END;
	static int select = uid(dre) % 3;
	static int Figure_axis = uid(dre) % cubeSize;
	static float rotdegree = degree;

	rotFlag = cube->Rotate_Specific_Side_Check_Rot(select, Figure_axis, rotdegree);
	if (rotFlag == CUBE_SEQUENCE_END) {
		select = uid(dre) % 3;
		Figure_axis = uid(dre) % cubeSize;
		suffleCount -= 1;
		if (uid(dre) % 2 == 0) {
			rotdegree = degree;
		}
		else {
			rotdegree = -degree;
		}
	}

	if (suffleCount <= 0) {
		suffleCount = uid(dre) % 10 + 25;
		return CUBE_SEQUENCE_END;
	}

	return CUBE_SEQUENCE_ING;
}

float dot(glm::vec3 v_1, glm::vec3 v_2) {
	return v_1.x * v_2.x + v_1.y * v_2.y + v_1.z * v_2.z;
}

float dot(glm::vec4 v_1, glm::vec4 v_2) {
	return v_1.x * v_2.x + v_1.y * v_2.y + v_1.z * v_2.z;
}