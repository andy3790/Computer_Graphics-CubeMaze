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

	bool checker[3];
	for (int i = 0; i < 3; i++) {
		checker[i] = false;
		if (a_min[i] > b_max[i] || a_max[i] < b_min[i]) {
			std::cout << i;
			return false;
		}
	}
	std::cout << '\n';
	if (checker[0] || checker[1] || checker[2]) {
		return false;
	}
	return true;
}