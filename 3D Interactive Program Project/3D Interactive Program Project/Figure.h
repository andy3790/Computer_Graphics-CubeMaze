#pragma once
#include "includer.h"

extern std::random_device rd;
extern std::default_random_engine dre;
extern std::uniform_real_distribution<float> F_urd;

class Figure {
private:
	GLfloat point[8][6];
	GLfloat vertex[36][9];
	GLuint VAO;
	GLuint VBO;
	int countOfData;
	int Shape; // 1 line 2 Cube 3 Pyramid
	float T_val[3]; // x , y , z
	float C_R_val_x; // camera
	float C_R_val_y;
	float rotate_val_x; // rotate
	float rotate_val_y;
	float Self_R_val_x; // self rotate
	float Self_R_val_y;
	float ScaleVal;
	glm::mat4 Tx; //--- translation matrix
	glm::mat4 Ty; //--- translation matrix
	glm::mat4 Tz; //--- translation matrix
	glm::mat4 Rx; //--- rotation matrix
	glm::mat4 Ry; //--- rotation matrix
	glm::mat4 SelfRx; //--- rotation matrix
	glm::mat4 SelfRy; //--- rotation matrix
	glm::mat4 SelfRxy; //--- rotation matrix
	glm::mat4 So; //--- scale matrix
	glm::mat4 Sm; //--- scale matrix
	glm::mat4 TR; //--- transformation matrix
public:
	Figure() {
		Shape = 0;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 6; j++) {
				point[i][j] = 0.0f;
			}
		}
		for (int i = 0; i < 36; i++) {
			for (int j = 0; j < 9; j++) {
				vertex[i][j] = 0.0f;
			}
		}
		VAO = 0;
		VBO = 0;
		countOfData = 0;
		C_R_val_x = 30.0f;
		C_R_val_y = -30.0f;
		rotate_val_x = 0.0f;
		rotate_val_y = 0.0f;
		Self_R_val_x = 0.0f;
		Self_R_val_y = 0.0f;
		T_val[0] = 0.0f;
		T_val[1] = 0.0f;
		T_val[2] = 0.0f;
		ScaleVal = 1.0f;

		Tx = glm::mat4(1.0f);
		Ty = glm::mat4(1.0f);
		Tz = glm::mat4(1.0f);
		Rx = glm::mat4(1.0f);
		Ry = glm::mat4(1.0f);
		SelfRx = glm::mat4(1.0f);
		SelfRy = glm::mat4(1.0f);
		SelfRxy = glm::mat4(1.0f);
		So = glm::mat4(1.0f);
		Sm = glm::mat4(1.0f);
		TR = glm::mat4(1.0f);
	}

	void MakeLine() {
		Shape = 1;
		countOfData = 6;
		rotate_val_x = 0.0f;
		rotate_val_y = 0.0f;
		T_val[0] = 0.0f;
		T_val[1] = 0.0f;
		T_val[2] = 0.0f;

		for (int i = 0; i < countOfData; i++) {
			for (int j = 0; j < 3; j++) {
				vertex[i][j] = 0.0f;
				if (i / 2 == j) {
					float temp = 1;
					for (int k = 0; k < i % 2; k++) { temp *= -1; }
					vertex[i][j] = 1.0f * temp;
				}
			}
		}

		for (int i = 0; i < 3; i++) {
			float tempcolor = F_urd(dre);
			tempcolor = 0.0;
			for (int j = 0; j < countOfData; j++) {
				vertex[j][i + 3] = tempcolor;
				if (j / 2 == i) {
					vertex[j][i + 3] = 1.0f;
				}
			}
		}
		MakeTransformMat();
		InitBuffer();
	}
	void MakeLine(float len) {
		Shape = 1;
		countOfData = 6;
		rotate_val_x = 0.0f;
		rotate_val_y = 0.0f;
		T_val[0] = 0.0f;
		T_val[1] = 0.0f;
		T_val[2] = 0.0f;

		for (int i = 0; i < countOfData; i++) {
			for (int j = 0; j < 3; j++) {
				vertex[i][j] = 0.0f;
				if (i / 2 == j) {
					float temp = 1;
					for (int k = 0; k < i % 2; k++) { temp *= -1; }
					vertex[i][j] = len * temp;
				}
			}
		}

		for (int i = 0; i < 3; i++) {
			float tempcolor = F_urd(dre);
			tempcolor = 0.0;
			for (int j = 0; j < countOfData; j++) {
				vertex[j][i + 3] = tempcolor;
				if (j / 2 == i) {
					vertex[j][i + 3] = 1.0f;
				}
			}
		}
		MakeTransformMat();
		InitBuffer();
	}
	void MakeLine(const char type) {
		Shape = 1;
		countOfData = 2;
		rotate_val_x = 0.0f;
		rotate_val_y = 0.0f;
		T_val[0] = 0.0f;
		T_val[1] = 0.0f;
		T_val[2] = 0.0f;
		int tempVal = 0;
		if (type == 'x') { tempVal = 0; }
		else if (type == 'y') { tempVal = 2; }
		else if (type == 'z') { tempVal = 4; }

		for (int i = 0; i < countOfData; i++) {
			for (int j = 0; j < 3; j++) {
				vertex[i][j] = 0.0f;
				if ((i + tempVal) / 2 == j) {
					float temp = 1;
					for (int k = 0; k < i % 2; k++) { temp *= -1; }
					vertex[i][j] = 1.0f * temp;
				}
			}
		}

		for (int i = 0; i < 3; i++) {
			float tempcolor = F_urd(dre);
			tempcolor = 0.0;
			for (int j = 0; j < countOfData; j++) {
				vertex[j][i + 3] = tempcolor;
				if ((j + tempVal) / 2 == i) {
					vertex[j][i + 3] = 1.0f;
				}
			}
		}
		MakeTransformMat();
		InitBuffer();
	}
	void MakeLine(float sx, float sy, float sz, float ex, float ey, float ez) {
		Shape = 1;
		countOfData = 2;
		rotate_val_x = 0.0f;
		rotate_val_y = 0.0f;
		T_val[0] = 0.0f;
		T_val[1] = 0.0f;
		T_val[2] = 0.0f;

		vertex[0][0] = sx;
		vertex[0][1] = sy;
		vertex[0][2] = sz;
		vertex[1][0] = ex;
		vertex[1][1] = ey;
		vertex[1][2] = ez;

		for (int i = 0; i < countOfData; i++) {
			for (int j = 0; j < 3; j++) {
				vertex[i][j + 3] = F_urd(dre);
			}
		}

		MakeTransformMat();
		InitBuffer();
	}
	void MakeCube() {
		int front = 0;
		int back = 4;
		Shape = 2;
		float tempsize = 0.3f;
		countOfData = 36;
		rotate_val_x = 0.0f;
		rotate_val_y = 0.0f;
		T_val[0] = 0.0f;
		T_val[1] = 0.0f;
		T_val[2] = 0.0f;

		point[0][0] = -tempsize;
		point[0][1] = +tempsize;
		point[0][2] = +tempsize;
		for (int j = 0; j < 3; j++) { point[0][j + 3] = F_urd(dre); }

		point[1][0] = +tempsize;
		point[1][1] = +tempsize;
		point[1][2] = +tempsize;
		for (int j = 0; j < 3; j++) { point[1][j + 3] = F_urd(dre); }

		point[2][0] = +tempsize;
		point[2][1] = -tempsize;
		point[2][2] = +tempsize;
		for (int j = 0; j < 3; j++) { point[2][j + 3] = F_urd(dre); }

		point[3][0] = -tempsize;
		point[3][1] = -tempsize;
		point[3][2] = +tempsize;
		for (int j = 0; j < 3; j++) { point[3][j + 3] = F_urd(dre); }

		for (int i = 0; i < 4; i++) {
			point[i + 4][0] = point[i][0];
			point[i + 4][1] = point[i][1];
			point[i + 4][2] = point[i][2] * -1;
			for (int j = 0; j < 3; j++) { point[i + 4][j + 3] = F_urd(dre); }
		}


		for (int i = 0; i < 6; i++) {
			// 앞면
			vertex[0 + 6 * 0][i] = point[0 + front][i];
			vertex[1 + 6 * 0][i] = point[3 + front][i];
			vertex[2 + 6 * 0][i] = point[2 + front][i];
			vertex[3 + 6 * 0][i] = point[0 + front][i];
			vertex[4 + 6 * 0][i] = point[2 + front][i];
			vertex[5 + 6 * 0][i] = point[1 + front][i];
			//뒷면
			vertex[0 + 6 * 1][i] = point[1 + back][i];
			vertex[1 + 6 * 1][i] = point[2 + back][i];
			vertex[2 + 6 * 1][i] = point[3 + back][i];
			vertex[3 + 6 * 1][i] = point[1 + back][i];
			vertex[4 + 6 * 1][i] = point[3 + back][i];
			vertex[5 + 6 * 1][i] = point[0 + back][i];
			//오른쪽 면
			vertex[0 + 6 * 2][i] = point[1 + front][i];
			vertex[1 + 6 * 2][i] = point[2 + front][i];
			vertex[2 + 6 * 2][i] = point[2 + back][i];
			vertex[3 + 6 * 2][i] = point[1 + front][i];
			vertex[4 + 6 * 2][i] = point[2 + back][i];
			vertex[5 + 6 * 2][i] = point[1 + back][i];
			//왼쪽 면
			vertex[0 + 6 * 3][i] = point[0 + back][i];
			vertex[1 + 6 * 3][i] = point[3 + back][i];
			vertex[2 + 6 * 3][i] = point[3 + front][i];
			vertex[3 + 6 * 3][i] = point[0 + back][i];
			vertex[4 + 6 * 3][i] = point[3 + front][i];
			vertex[5 + 6 * 3][i] = point[0 + front][i];
			//위쪽 면
			vertex[0 + 6 * 4][i] = point[0 + back][i];
			vertex[1 + 6 * 4][i] = point[0 + front][i];
			vertex[2 + 6 * 4][i] = point[1 + front][i];
			vertex[3 + 6 * 4][i] = point[0 + back][i];
			vertex[4 + 6 * 4][i] = point[1 + front][i];
			vertex[5 + 6 * 4][i] = point[1 + back][i];
			//아래쪽 면
			vertex[0 + 6 * 5][i] = point[3 + front][i];
			vertex[1 + 6 * 5][i] = point[3 + back][i];
			vertex[2 + 6 * 5][i] = point[2 + back][i];
			vertex[3 + 6 * 5][i] = point[3 + front][i];
			vertex[4 + 6 * 5][i] = point[2 + back][i];
			vertex[5 + 6 * 5][i] = point[2 + front][i];
		}
		InitCubeNormal();
		//for (int i = 0; i < 3; i++) {
		//	for (int j = 0; j < 6; j++) {
		//		float tempcolor = urd(dre);
		//		for (int k = 0; k < 6; k++) {
		//			vertex[k + 6 * j][i + 3] = tempcolor;
		//		}
		//	}
		//}
		MakeTransformMat();
		InitBuffer();
	}
	void MakeCube(float midx, float midy, float midz, float sizex, float sizey, float sizez) {
		int front = 0;
		int back = 4;
		Shape = 2;
		countOfData = 36;
		rotate_val_x = 0.0f;
		rotate_val_y = 0.0f;
		T_val[0] = 0.0f;
		T_val[1] = 0.0f;
		T_val[2] = 0.0f;

		point[0][0] = midx - sizex;
		point[0][1] = midy + sizey;
		point[0][2] = midz + sizez;
		for (int j = 0; j < 3; j++) { point[0][j + 3] = F_urd(dre); }

		point[1][0] = midx + sizex;
		point[1][1] = midy + sizey;
		point[1][2] = midz + sizez;
		for (int j = 0; j < 3; j++) { point[1][j + 3] = F_urd(dre); }

		point[2][0] = midx + sizex;
		point[2][1] = midy - sizey;
		point[2][2] = midz + sizez;
		for (int j = 0; j < 3; j++) { point[2][j + 3] = F_urd(dre); }

		point[3][0] = midx - sizex;
		point[3][1] = midy - sizey;
		point[3][2] = midz + sizez;
		for (int j = 0; j < 3; j++) { point[3][j + 3] = F_urd(dre); }

		for (int i = 0; i < 4; i++) {
			point[i + 4][0] = point[i][0];
			point[i + 4][1] = point[i][1];
			point[i + 4][2] = point[i][2] - (sizez * 2);
			for (int j = 0; j < 3; j++) { point[i + 4][j + 3] = F_urd(dre); }
		}


		for (int i = 0; i < 6; i++) {
			// 앞면
			vertex[0 + 6 * 0][i] = point[0 + front][i];
			vertex[1 + 6 * 0][i] = point[3 + front][i];
			vertex[2 + 6 * 0][i] = point[2 + front][i];
			vertex[3 + 6 * 0][i] = point[0 + front][i];
			vertex[4 + 6 * 0][i] = point[2 + front][i];
			vertex[5 + 6 * 0][i] = point[1 + front][i];
			//뒷면
			vertex[0 + 6 * 1][i] = point[1 + back][i];
			vertex[1 + 6 * 1][i] = point[2 + back][i];
			vertex[2 + 6 * 1][i] = point[3 + back][i];
			vertex[3 + 6 * 1][i] = point[1 + back][i];
			vertex[4 + 6 * 1][i] = point[3 + back][i];
			vertex[5 + 6 * 1][i] = point[0 + back][i];
			//오른쪽 면
			vertex[0 + 6 * 2][i] = point[1 + front][i];
			vertex[1 + 6 * 2][i] = point[2 + front][i];
			vertex[2 + 6 * 2][i] = point[2 + back][i];
			vertex[3 + 6 * 2][i] = point[1 + front][i];
			vertex[4 + 6 * 2][i] = point[2 + back][i];
			vertex[5 + 6 * 2][i] = point[1 + back][i];
			//왼쪽 면
			vertex[0 + 6 * 3][i] = point[0 + back][i];
			vertex[1 + 6 * 3][i] = point[3 + back][i];
			vertex[2 + 6 * 3][i] = point[3 + front][i];
			vertex[3 + 6 * 3][i] = point[0 + back][i];
			vertex[4 + 6 * 3][i] = point[3 + front][i];
			vertex[5 + 6 * 3][i] = point[0 + front][i];
			//위쪽 면
			vertex[0 + 6 * 4][i] = point[0 + back][i];
			vertex[1 + 6 * 4][i] = point[0 + front][i];
			vertex[2 + 6 * 4][i] = point[1 + front][i];
			vertex[3 + 6 * 4][i] = point[0 + back][i];
			vertex[4 + 6 * 4][i] = point[1 + front][i];
			vertex[5 + 6 * 4][i] = point[1 + back][i];
			//아래쪽 면
			vertex[0 + 6 * 5][i] = point[3 + front][i];
			vertex[1 + 6 * 5][i] = point[3 + back][i];
			vertex[2 + 6 * 5][i] = point[2 + back][i];
			vertex[3 + 6 * 5][i] = point[3 + front][i];
			vertex[4 + 6 * 5][i] = point[2 + back][i];
			vertex[5 + 6 * 5][i] = point[2 + front][i];
		}
		InitCubeNormal();
		MakeTransformMat();
		InitBuffer();
	}
	void MakeCube(float midx, float midy, float midz, float sizex, float sizey, float sizez, float colorR, float colorG, float colorB) {
		int front = 0;
		int back = 4;
		Shape = 2;
		countOfData = 36;
		rotate_val_x = 0.0f;
		rotate_val_y = 0.0f;
		T_val[0] = 0.0f;
		T_val[1] = 0.0f;
		T_val[2] = 0.0f;

		point[0][0] = midx - sizex;
		point[0][1] = midy + sizey;
		point[0][2] = midz + sizez;

		point[1][0] = midx + sizex;
		point[1][1] = midy + sizey;
		point[1][2] = midz + sizez;

		point[2][0] = midx + sizex;
		point[2][1] = midy - sizey;
		point[2][2] = midz + sizez;

		point[3][0] = midx - sizex;
		point[3][1] = midy - sizey;
		point[3][2] = midz + sizez;

		for (int i = 0; i < 4; i++) {
			point[i + 4][0] = point[i][0];
			point[i + 4][1] = point[i][1];
			point[i + 4][2] = point[i][2] - (sizez * 2);
		}

		for (int i = 0; i < 8; i++) {
			point[i][3] = colorR;
			point[i][4] = colorG;
			point[i][5] = colorB;
		}

		//std::cout << midx << ' ' << midy << ' ' << midz << ' ' << sizex << ' ' << sizey << ' ' << sizez << '\n';
		//std::cout << point[2][0] << '\t' << point[3][0] << '\t' << point[6][0] << '\t' << point[7][0] << '\t' << '\t';
		//std::cout << point[2][2] << '\t' << point[3][2] << '\t' << point[6][2] << '\t' << point[7][2] << '\n';

		for (int i = 0; i < 6; i++) {
			// 앞면
			vertex[0 + 6 * 0][i] = point[0 + front][i];
			vertex[1 + 6 * 0][i] = point[3 + front][i];
			vertex[2 + 6 * 0][i] = point[2 + front][i];
			vertex[3 + 6 * 0][i] = point[0 + front][i];
			vertex[4 + 6 * 0][i] = point[2 + front][i];
			vertex[5 + 6 * 0][i] = point[1 + front][i];
			//뒷면
			vertex[0 + 6 * 1][i] = point[1 + back][i];
			vertex[1 + 6 * 1][i] = point[2 + back][i];
			vertex[2 + 6 * 1][i] = point[3 + back][i];
			vertex[3 + 6 * 1][i] = point[1 + back][i];
			vertex[4 + 6 * 1][i] = point[3 + back][i];
			vertex[5 + 6 * 1][i] = point[0 + back][i];
			//오른쪽 면
			vertex[0 + 6 * 2][i] = point[1 + front][i];
			vertex[1 + 6 * 2][i] = point[2 + front][i];
			vertex[2 + 6 * 2][i] = point[2 + back][i];
			vertex[3 + 6 * 2][i] = point[1 + front][i];
			vertex[4 + 6 * 2][i] = point[2 + back][i];
			vertex[5 + 6 * 2][i] = point[1 + back][i];
			//왼쪽 면
			vertex[0 + 6 * 3][i] = point[0 + back][i];
			vertex[1 + 6 * 3][i] = point[3 + back][i];
			vertex[2 + 6 * 3][i] = point[3 + front][i];
			vertex[3 + 6 * 3][i] = point[0 + back][i];
			vertex[4 + 6 * 3][i] = point[3 + front][i];
			vertex[5 + 6 * 3][i] = point[0 + front][i];
			//위쪽 면
			vertex[0 + 6 * 4][i] = point[0 + back][i];
			vertex[1 + 6 * 4][i] = point[0 + front][i];
			vertex[2 + 6 * 4][i] = point[1 + front][i];
			vertex[3 + 6 * 4][i] = point[0 + back][i];
			vertex[4 + 6 * 4][i] = point[1 + front][i];
			vertex[5 + 6 * 4][i] = point[1 + back][i];
			//아래쪽 면
			vertex[0 + 6 * 5][i] = point[3 + front][i];
			vertex[1 + 6 * 5][i] = point[3 + back][i];
			vertex[2 + 6 * 5][i] = point[2 + back][i];
			vertex[3 + 6 * 5][i] = point[3 + front][i];
			vertex[4 + 6 * 5][i] = point[2 + back][i];
			vertex[5 + 6 * 5][i] = point[2 + front][i];
		}
		InitCubeNormal();
		MakeTransformMat();
		InitBuffer();
	}
	void MakeCube(float midx, float midy, float midz, float sizex, float sizey, float sizez, int type) {
		int front = 0;
		int back = 4;
		Shape = 2;
		countOfData = 36;
		rotate_val_x = 0.0f;
		rotate_val_y = 0.0f;
		T_val[0] = 0.0f;
		T_val[1] = 0.0f;
		T_val[2] = 0.0f;

		point[0][0] = midx - sizex;
		point[0][1] = midy + sizey;
		point[0][2] = midz + sizez;

		point[1][0] = midx + sizex;
		point[1][1] = midy + sizey;
		point[1][2] = midz + sizez;

		point[2][0] = midx + sizex;
		point[2][1] = midy - sizey;
		point[2][2] = midz + sizez;

		point[3][0] = midx - sizex;
		point[3][1] = midy - sizey;
		point[3][2] = midz + sizez;

		for (int i = 0; i < 4; i++) {
			point[i + 4][0] = point[i][0];
			point[i + 4][1] = point[i][1];
			point[i + 4][2] = point[i][2] - (sizez * 2);
			for (int j = 0; j < 3; j++) { point[i + 4][j + 3] = F_urd(dre); }
		}

		for (int i = 0; i < 6; i++) {
			// 앞면
			vertex[0 + 6 * 0][i] = point[0 + front][i];
			vertex[1 + 6 * 0][i] = point[3 + front][i];
			vertex[2 + 6 * 0][i] = point[2 + front][i];
			vertex[3 + 6 * 0][i] = point[0 + front][i];
			vertex[4 + 6 * 0][i] = point[2 + front][i];
			vertex[5 + 6 * 0][i] = point[1 + front][i];
			//뒷면
			vertex[0 + 6 * 1][i] = point[1 + back][i];
			vertex[1 + 6 * 1][i] = point[2 + back][i];
			vertex[2 + 6 * 1][i] = point[3 + back][i];
			vertex[3 + 6 * 1][i] = point[1 + back][i];
			vertex[4 + 6 * 1][i] = point[3 + back][i];
			vertex[5 + 6 * 1][i] = point[0 + back][i];
			//오른쪽 면
			vertex[0 + 6 * 2][i] = point[1 + front][i];
			vertex[1 + 6 * 2][i] = point[2 + front][i];
			vertex[2 + 6 * 2][i] = point[2 + back][i];
			vertex[3 + 6 * 2][i] = point[1 + front][i];
			vertex[4 + 6 * 2][i] = point[2 + back][i];
			vertex[5 + 6 * 2][i] = point[1 + back][i];
			//왼쪽 면
			vertex[0 + 6 * 3][i] = point[0 + back][i];
			vertex[1 + 6 * 3][i] = point[3 + back][i];
			vertex[2 + 6 * 3][i] = point[3 + front][i];
			vertex[3 + 6 * 3][i] = point[0 + back][i];
			vertex[4 + 6 * 3][i] = point[3 + front][i];
			vertex[5 + 6 * 3][i] = point[0 + front][i];
			//위쪽 면
			vertex[0 + 6 * 4][i] = point[0 + back][i];
			vertex[1 + 6 * 4][i] = point[0 + front][i];
			vertex[2 + 6 * 4][i] = point[1 + front][i];
			vertex[3 + 6 * 4][i] = point[0 + back][i];
			vertex[4 + 6 * 4][i] = point[1 + front][i];
			vertex[5 + 6 * 4][i] = point[1 + back][i];
			//아래쪽 면
			vertex[0 + 6 * 5][i] = point[3 + front][i];
			vertex[1 + 6 * 5][i] = point[3 + back][i];
			vertex[2 + 6 * 5][i] = point[2 + back][i];
			vertex[3 + 6 * 5][i] = point[3 + front][i];
			vertex[4 + 6 * 5][i] = point[2 + back][i];
			vertex[5 + 6 * 5][i] = point[2 + front][i];
		}

		if (type == 1) {
			float tempColor[3];
			for (int i = 0; i < 6; i++) {
				for (int k = 0; k < 3; k++) {
					tempColor[k] = F_urd(dre);
				}
				for (int k = 0; k < 3; k++) {
					for (int j = 0; j < 6; j++) {
						vertex[j + 6 * i][k + 3] = tempColor[k];
					}
				}
			}
		}
		InitCubeNormal();
		MakeTransformMat();
		InitBuffer();
	}
	void MakePyramid() {
		Shape = 3;
		float tempsize = 0.3f;
		countOfData = 18;
		rotate_val_x = 0.0f;
		rotate_val_y = 0.0f;
		T_val[0] = 0.0f;
		T_val[1] = 0.0f;
		T_val[2] = 0.0f;

		point[0][0] = -tempsize;
		point[0][1] = -tempsize;
		point[0][2] = -tempsize;
		for (int j = 0; j < 3; j++) { point[0][j + 3] = F_urd(dre); }

		point[1][0] = +tempsize;
		point[1][1] = -tempsize;
		point[1][2] = -tempsize;
		for (int j = 0; j < 3; j++) { point[1][j + 3] = F_urd(dre); }

		point[2][0] = +tempsize;
		point[2][1] = -tempsize;
		point[2][2] = +tempsize;
		for (int j = 0; j < 3; j++) { point[2][j + 3] = F_urd(dre); }

		point[3][0] = -tempsize;
		point[3][1] = -tempsize;
		point[3][2] = +tempsize;
		for (int j = 0; j < 3; j++) { point[3][j + 3] = F_urd(dre); }

		point[4][0] = 0;
		point[4][1] = +tempsize;
		point[4][2] = 0;
		for (int j = 0; j < 3; j++) { point[4][j + 3] = F_urd(dre); }


		for (int i = 0; i < 6; i++) {
			//밑면
			vertex[0 + 3 * 0][i] = point[1][i];
			vertex[1 + 3 * 0][i] = point[2][i];
			vertex[2 + 3 * 0][i] = point[3][i];
			vertex[0 + 3 * 1][i] = point[1][i];
			vertex[1 + 3 * 1][i] = point[3][i];
			vertex[2 + 3 * 1][i] = point[0][i];
			//앞면
			vertex[0 + 3 * 2][i] = point[4][i];
			vertex[1 + 3 * 2][i] = point[3][i];
			vertex[2 + 3 * 2][i] = point[2][i];
			//뒷면
			vertex[0 + 3 * 3][i] = point[4][i];
			vertex[1 + 3 * 3][i] = point[1][i];
			vertex[2 + 3 * 3][i] = point[0][i];
			//왼쪽 면
			vertex[0 + 3 * 4][i] = point[4][i];
			vertex[1 + 3 * 4][i] = point[0][i];
			vertex[2 + 3 * 4][i] = point[3][i];
			//오른쪽 면
			vertex[0 + 3 * 5][i] = point[4][i];
			vertex[1 + 3 * 5][i] = point[2][i];
			vertex[2 + 3 * 5][i] = point[1][i];
		}
		InitPyramidNormal();
		MakeTransformMat();
		InitBuffer();
	}
	void MakePyramid(float midx, float midy, float midz, float sizex, float sizey, float sizez) {
		Shape = 3;
		countOfData = 18;
		rotate_val_x = 0.0f;
		rotate_val_y = 0.0f;
		T_val[0] = 0.0f;
		T_val[1] = 0.0f;
		T_val[2] = 0.0f;

		point[0][0] = midx - sizex;
		point[0][1] = midy - sizey;
		point[0][2] = midz - sizez;
		for (int j = 0; j < 3; j++) { point[0][j + 3] = F_urd(dre); }

		point[1][0] = midx + sizex;
		point[1][1] = midy - sizey;
		point[1][2] = midz - sizez;
		for (int j = 0; j < 3; j++) { point[1][j + 3] = F_urd(dre); }

		point[2][0] = midx + sizex;
		point[2][1] = midy - sizey;
		point[2][2] = midz + sizez;
		for (int j = 0; j < 3; j++) { point[2][j + 3] = F_urd(dre); }

		point[3][0] = midx - sizex;
		point[3][1] = midy - sizey;
		point[3][2] = midz + sizez;
		for (int j = 0; j < 3; j++) { point[3][j + 3] = F_urd(dre); }

		point[4][0] = midx;
		point[4][1] = midy + sizey;
		point[4][2] = midz;
		for (int j = 0; j < 3; j++) { point[4][j + 3] = F_urd(dre); }


		for (int i = 0; i < 6; i++) {
			//밑면
			vertex[0 + 3 * 0][i] = point[1][i];
			vertex[1 + 3 * 0][i] = point[2][i];
			vertex[2 + 3 * 0][i] = point[3][i];
			vertex[0 + 3 * 1][i] = point[1][i];
			vertex[1 + 3 * 1][i] = point[3][i];
			vertex[2 + 3 * 1][i] = point[0][i];
			//앞면
			vertex[0 + 3 * 2][i] = point[4][i];
			vertex[1 + 3 * 2][i] = point[3][i];
			vertex[2 + 3 * 2][i] = point[2][i];
			//뒷면
			vertex[0 + 3 * 3][i] = point[4][i];
			vertex[1 + 3 * 3][i] = point[1][i];
			vertex[2 + 3 * 3][i] = point[0][i];
			//왼쪽 면
			vertex[0 + 3 * 4][i] = point[4][i];
			vertex[1 + 3 * 4][i] = point[0][i];
			vertex[2 + 3 * 4][i] = point[3][i];
			//오른쪽 면
			vertex[0 + 3 * 5][i] = point[4][i];
			vertex[1 + 3 * 5][i] = point[2][i];
			vertex[2 + 3 * 5][i] = point[1][i];
		}
		InitPyramidNormal();
		MakeTransformMat();
		InitBuffer();
	}

	glm::vec3 calNormal(int sv, int sel, int vcount)
	{
		float v1[3], v2[3], t[3];
		static const int x = 0;
		static const int y = 1;
		static const int z = 2;

		v1[x] = vertex[sv + 0 + vcount * sel][x] - vertex[sv + 1 + vcount * sel][x];
		v1[y] = vertex[sv + 0 + vcount * sel][y] - vertex[sv + 1 + vcount * sel][y];
		v1[z] = vertex[sv + 0 + vcount * sel][z] - vertex[sv + 1 + vcount * sel][z];

		v2[x] = vertex[sv + 1 + vcount * sel][x] - vertex[sv + 2 + vcount * sel][x];
		v2[y] = vertex[sv + 1 + vcount * sel][y] - vertex[sv + 2 + vcount * sel][y];
		v2[z] = vertex[sv + 1 + vcount * sel][z] - vertex[sv + 2 + vcount * sel][z];

		t[x] = v1[y] * v2[z] - v1[z] * v2[y];
		t[y] = v1[z] * v2[x] - v1[x] * v2[z];
		t[z] = v1[x] * v2[y] - v1[y] * v2[x];

		float tlen = (float)sqrt(t[x] * t[x] + t[y] * t[y] + t[z] * t[z]);

		return glm::vec3(t[x] / tlen, t[y] / tlen, t[z] / tlen);
	}
	void InitCubeNormal() {
		glm::vec3 t[6];
		for (int i = 0; i < 6; i++) {
			t[i] = calNormal(0, i, 6);
		}

		for (int i = 0; i < 6; i++) {	// 노멀 저장
			for (int j = 0; j < 3; j++) {
				//앞면
				vertex[i + 6 * 0][j + 6] = t[0][j];
				//뒷면
				vertex[i + 6 * 1][j + 6] = t[1][j];
				//오른쪽 면
				vertex[i + 6 * 2][j + 6] = t[2][j];
				//왼쪽 면
				vertex[i + 6 * 3][j + 6] = t[3][j];
				//위쪽 면
				vertex[i + 6 * 4][j + 6] = t[4][j];
				//아래쪽 면
				vertex[i + 6 * 5][j + 6] = t[5][j];
			}
		}
	}
	void InitPyramidNormal() {
		glm::vec3 t[5];
		for (int i = 0; i < 5; i++) {
			t[i] = calNormal(0, i + 1, 3);
		}

		for (int i = 0; i < 3; i++) {
			//밑면
			vertex[0 + 3 * 0][6 + i] = t[0][i];
			vertex[1 + 3 * 0][6 + i] = t[0][i];
			vertex[2 + 3 * 0][6 + i] = t[0][i];
			vertex[0 + 3 * 1][6 + i] = t[0][i];
			vertex[1 + 3 * 1][6 + i] = t[0][i];
			vertex[2 + 3 * 1][6 + i] = t[0][i];
			//앞면
			vertex[0 + 3 * 2][6 + i] = t[1][i];
			vertex[1 + 3 * 2][6 + i] = t[1][i];
			vertex[2 + 3 * 2][6 + i] = t[1][i];
			//뒷면
			vertex[0 + 3 * 3][6 + i] = t[2][i];
			vertex[1 + 3 * 3][6 + i] = t[2][i];
			vertex[2 + 3 * 3][6 + i] = t[2][i];
			//왼쪽 면
			vertex[0 + 3 * 4][6 + i] = t[3][i];
			vertex[1 + 3 * 4][6 + i] = t[3][i];
			vertex[2 + 3 * 4][6 + i] = t[3][i];
			//오른쪽 면
			vertex[0 + 3 * 5][6 + i] = t[4][i];
			vertex[1 + 3 * 5][6 + i] = t[4][i];
			vertex[2 + 3 * 5][6 + i] = t[4][i];
		}
	}

	void Reset() {
		Tx = glm::mat4(1.0f);
		Ty = glm::mat4(1.0f);
		Tz = glm::mat4(1.0f);
		Rx = glm::mat4(1.0f);
		Ry = glm::mat4(1.0f);
		SelfRx = glm::mat4(1.0f);
		SelfRy = glm::mat4(1.0f);
		SelfRxy = glm::mat4(1.0f);
		So = glm::mat4(1.0f);
		Sm = glm::mat4(1.0f);
		TR = glm::mat4(1.0f);
		rotate_val_x = 0.0f;
		rotate_val_y = 0.0f;
		Self_R_val_x = 0.0f;
		Self_R_val_y = 0.0f;
		ScaleVal = 1.0f;

		MakeTransformMat();
	}

	void Translate(float x, float y, float z) {
		Tx = glm::translate(Tx, glm::vec3(x, 0.0, 0.0)); //--- x축으로 translation
		Ty = glm::translate(Ty, glm::vec3(0.0, y, 0.0)); //--- y축으로 translation
		Tz = glm::translate(Tz, glm::vec3(0.0, 0.0, z)); //--- z축으로 translation
		ChangeTransformMat();
	}
	void Translate(const char type, float val) {
		if (type == 'x') {
			Tx = glm::translate(Tx, glm::vec3(val, 0.0, 0.0)); //--- x축으로 translation
		}
		else if (type == 'y') {
			Ty = glm::translate(Ty, glm::vec3(0.0, val, 0.0)); //--- y축으로 translation
		}
		else if (type == 'z') {
			Tz = glm::translate(Tz, glm::vec3(0.0, 0.0, val)); //--- z축으로 translation
		}
		ChangeTransformMat();
	}
	void Translate_Reset(float x, float y, float z) {
		Tx = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0, 0.0)); //--- x축으로 translation
		Ty = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, y, 0.0)); //--- y축으로 translation
		Tz = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, z)); //--- z축으로 translation
		ChangeTransformMat();
	}
	void Translate_Reset(const char type, float val) {
		if (type == 'x') {
			Tx = glm::translate(glm::mat4(1.0f), glm::vec3(val, 0.0, 0.0)); //--- x축으로 translation
		}
		else if (type == 'y') {
			Ty = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, val, 0.0)); //--- y축으로 translation
		}
		else if (type == 'z') {
			Tz = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, val)); //--- z축으로 translation
		}
		ChangeTransformMat();
	}

	void Rotate(const char type, const char type2, float degree) {
		if (type == 'a') { // 원점 기준 회전
			if (type2 == 'x') {
				Rx = glm::rotate(Rx, (GLfloat)glm::radians(degree), glm::vec3(1.0, 0.0, 0.0)); //--- x축에대하여 회전
			}
			else if (type2 == 'y') {
				Ry = glm::rotate(Ry, (GLfloat)glm::radians(degree), glm::vec3(0.0, 1.0, 0.0)); //--- y축에대하여 회전
			}
			else if (type2 == 'z') {
				Ry = glm::rotate(Ry, (GLfloat)glm::radians(degree), glm::vec3(0.0, 0.0, 1.0)); //--- z축에대하여 회전
			}
		}
		else if (type == 'b') { // 회전 영향을 안받는 제자리 회전
			if (type2 == 'x') {
				SelfRxy = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(1.0, 0.0, 0.0)) * SelfRxy; //--- x축에대하여 회전
			}
			else if (type2 == 'y') {
				SelfRxy = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(0.0, 1.0, 0.0)) * SelfRxy; //--- y축에대하여 회전
			}
			else if (type2 == 'z') {
				SelfRxy = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(0.0, 0.0, 1.0)) * SelfRxy; //--- z축에대하여 회전
			}
		}
		else if (type == 'c') { // 회전 영향을 받는 제자리 회전
			if (type2 == 'x') {
				SelfRxy = glm::rotate(SelfRxy, (GLfloat)glm::radians(degree), glm::vec3(1.0, 0.0, 0.0)); //--- x축에대하여 회전
			}
			else if (type2 == 'y') {
				SelfRxy = glm::rotate(SelfRxy, (GLfloat)glm::radians(degree), glm::vec3(0.0, 1.0, 0.0)); //--- y축에대하여 회전
			}
			else if (type2 == 'z') {
				SelfRxy = glm::rotate(SelfRxy, (GLfloat)glm::radians(degree), glm::vec3(0.0, 0.0, 1.0)); //--- z축에대하여 회전
			}
		}
		ChangeTransformMat();
	}

	void Rotate_Reset(const char type, const char type2, float degree) {
		if (type == 'a') { // 원점 기준 회전
			if (type2 == 'x') {
				Rx = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(1.0, 0.0, 0.0)); //--- x축에대하여 회전
			}
			else if (type2 == 'y') {
				Ry = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(0.0, 1.0, 0.0)); //--- y축에대하여 회전
			}
			else if (type2 == 'z') {
				Ry = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(0.0, 0.0, 1.0)); //--- z축에대하여 회전
			}
		}
		else if (type == 'b') { // 회전 영향을 안받는 제자리 회전
			if (type2 == 'x') {
				SelfRxy = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(1.0, 0.0, 0.0)) * SelfRxy; //--- x축에대하여 회전
			}
			else if (type2 == 'y') {
				SelfRxy = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(0.0, 1.0, 0.0)) * SelfRxy; //--- y축에대하여 회전
			}
			else if (type2 == 'z') {
				SelfRxy = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(0.0, 0.0, 1.0)) * SelfRxy; //--- z축에대하여 회전
			}
		}
		else if (type == 'c') { // 회전 영향을 받는 제자리 회전
			if (type2 == 'x') {
				SelfRxy = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(1.0, 0.0, 0.0)); //--- x축에대하여 회전
			}
			else if (type2 == 'y') {
				SelfRxy = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(0.0, 1.0, 0.0)); //--- y축에대하여 회전
			}
			else if (type2 == 'z') {
				SelfRxy = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(0.0, 0.0, 1.0)); //--- z축에대하여 회전
			}
		}
		ChangeTransformMat();
	}

	void Scale(const char type, float sizex, float sizey, float sizez) {
		if (type == 'o') { // 원점을 기준으로 스케일링
			So = glm::scale(So, glm::vec3(sizex, sizey, sizez));
		}
		else if (type == 'm') { // 자신을 기준으로 스케일링
			Sm = glm::scale(Sm, glm::vec3(sizex, sizey, sizez));
		}
		ChangeTransformMat();
	}

	void PrintPoints() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 3; j++) {
				std::cout << point[i][j] << '\t';
			}
			std::cout << '\n';
		}
		std::cout << '\n';
	}
	void Erasing() {
		ScaleVal -= 0.05f;
		Sm = glm::scale(glm::mat4(1.0f), glm::vec3(ScaleVal, ScaleVal, ScaleVal));
		ChangeTransformMat();
	}
	float GetScaleVal() { return ScaleVal; }

	GLvoid Draw() {
		if (ScaleVal < 0.1) { return; }
		glBindVertexArray(VAO);
		if (Shape == 1) {
			glDrawArrays(GL_LINES, 0, countOfData);
		}
		else if (Shape == 2) {
			glDrawArrays(GL_TRIANGLES, 0, countOfData);
		}
		else if (Shape == 3) {
			glDrawArrays(GL_TRIANGLES, 0, countOfData);
		}
	}
	GLvoid Draw(int sel) {
		glBindVertexArray(VAO);
		if (Shape == 1) {
			glDrawArrays(GL_LINES, sel * 3, 2);
		}
		else if (Shape == 2) {
			glDrawArrays(GL_TRIANGLES, sel * 6, 6);
		}
		else if (Shape == 3) {
			glDrawArrays(GL_TRIANGLES, sel * 3, 3);
		}
	}
	GLvoid Draw(unsigned int transformLocation) {
		glBindVertexArray(VAO);
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(TR));
		if (Shape == 1) {
			glDrawArrays(GL_LINES, 0, countOfData);
		}
		else if (Shape == 2) {
			glDrawArrays(GL_TRIANGLES, 0, countOfData);
		}
		else if (Shape == 3) {
			glDrawArrays(GL_TRIANGLES, 0, countOfData);
		}
	}
	GLvoid Draw(unsigned int transformLocation, int sel) {
		glBindVertexArray(VAO);
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(TR));
		if (Shape == 1) {
			glDrawArrays(GL_LINES, sel * 3, 2);
		}
		else if (Shape == 2) {
			glDrawArrays(GL_TRIANGLES, sel * 6, 6);
		}
		else if (Shape == 3) {
			glDrawArrays(GL_TRIANGLES, sel * 3, 3);
		}
	}
	GLvoid Draw(unsigned int transformLocation, glm::mat4 afterMat) {
		glBindVertexArray(VAO);
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(afterMat * TR));
		if (Shape == 1) {
			glDrawArrays(GL_LINES, 0, countOfData);
		}
		else if (Shape == 2) {
			glDrawArrays(GL_TRIANGLES, 0, countOfData);
		}
		else if (Shape == 3) {
			glDrawArrays(GL_TRIANGLES, 0, countOfData);
		}
	}

	GLvoid MakeTransformMat() {
		//--- 변환 행렬 만들기

		Tx = glm::translate(Tx, glm::vec3(T_val[0], 0.0, 0.0)); //--- x축으로 translation
		Ty = glm::translate(Ty, glm::vec3(0.0, T_val[1], 0.0)); //--- y축으로 translation
		Tz = glm::translate(Tz, glm::vec3(0.0, 0.0, T_val[2])); //--- z축으로 translation
		Rx = glm::rotate(Rx, (GLfloat)glm::radians(rotate_val_x), glm::vec3(1.0, 0.0, 0.0)); //--- x축에대하여 회전
		Ry = glm::rotate(Ry, (GLfloat)glm::radians(rotate_val_y), glm::vec3(0.0, 1.0, 0.0)); //--- y축에대하여 회전

		//TR = Rx * Txyz * Ry; //--- 합성 변환 행렬: rotate -> translat
		ChangeTransformMat();
	}

	GLvoid ChangeTransformMat() {
		if (Shape == 0) {
			TR = So * Tz * Ry * Rx * Ty * Tx * SelfRxy * Sm;
		}
		else if (Shape == 1) {
			TR = Tz * Rx * Tx * Ry * Ty;
		}
		else if (Shape == 2 || Shape == 3) {
			TR = So * Tz * Ry * Rx * Ty * Tx * SelfRxy * Sm;
		}
		else if (Shape == 4) {
			TR = Tz * Rx * Tx * Ry * Ty;
		}
		TR = TR;
	}

	glm::mat4 GetTransformMat() {
		return TR;
	}

	GLuint InitBuffer()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (countOfData * 9) * sizeof(GLfloat), vertex, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		return VAO;
	}
};


class Block {
private:
	Figure*** blocks;
	int blockCount[3];
	float blockSize[3];
	const int x = 0;
	const int y = 1;
	const int z = 2;
	glm::mat4 blockRot;
public:
	Block() {
		blocks = NULL;
		blockCount[x] = 0;
		blockCount[y] = 0;
		blockCount[z] = 0;
		blockSize[x] = 0.0;
		blockSize[y] = 0.0;
		blockSize[z] = 0.0;
		blockRot = glm::mat4(1.0f);
	}
	GLvoid MakeBlock() {
		SettingBlocks(1, 1, 1, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f);

		blocks[0][0][0].MakeCube(0.0, 0.0, 0.0, blockSize[x], blockSize[y], blockSize[z]);
	}
	GLvoid MakeBlock(int count_x, int count_y, int count_z) {
		SettingBlocks(count_x, count_y, count_z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					blocks[i][j][k].MakeCube(-1.0f + blockSize[x] * (float)k * 2.0f + blockSize[x],
											 -1.0f + blockSize[y] * (float)j * 2.0f + blockSize[y],
											 -1.0f + blockSize[z] * (float)i * 2.0f + blockSize[z],
											 blockSize[x], blockSize[y], blockSize[z]);
				}
			}
		}
	}
	GLvoid MakeBlock(int count_x, int count_y, int count_z, float midx, float midy, float midz) {
		SettingBlocks(count_x, count_y, count_z, midx, midy, midz, 1.0f, 1.0f, 1.0f);

		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					blocks[i][j][k].MakeCube(-1.0f + blockSize[x] * (float)k * 2.0f + blockSize[x] + midx,
						-1.0f + blockSize[y] * (float)j * 2.0f + blockSize[y] + midy,
						-1.0f + blockSize[z] * (float)i * 2.0f + blockSize[z] + midz,
						blockSize[x], blockSize[y], blockSize[z]);
				}
			}
		}
	}
	GLvoid MakeBlock(int count_x, int count_y, int count_z, float midx, float midy, float midz, float size_x, float size_y, float size_z) {
		SettingBlocks(count_x, count_y, count_z, midx, midy, midz, size_x, size_y, size_z);

		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					blocks[i][j][k].MakeCube(-size_x + blockSize[x] * (float)k * 2.0f + blockSize[x] + midx,
						-size_y + blockSize[y] * (float)j * 2.0f + blockSize[y] + midy,
						-size_z + blockSize[z] * (float)i * 2.0f + blockSize[z] + midz,
						blockSize[x], blockSize[y], blockSize[z]);
				}
			}
		}
	}
	GLvoid MakeBlock(int count_x, int count_y, int count_z, float midx, float midy, float midz, float size_x, float size_y, float size_z, float colorR, float colorG, float colorB) {
		SettingBlocks(count_x, count_y, count_z, midx, midy, midz, size_x, size_y, size_z);

		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					blocks[i][j][k].MakeCube(-size_x + blockSize[x] * (float)k * 2.0f + blockSize[x] + midx,
						-size_y + blockSize[y] * (float)j * 2.0f + blockSize[y] + midy,
						-size_z + blockSize[z] * (float)i * 2.0f + blockSize[z] + midz,
						blockSize[x], blockSize[y], blockSize[z], colorR, colorG, colorB);
				}
			}
		}
	}
	GLvoid MakeBlock(int count_x, int count_y, int count_z, float midx, float midy, float midz, float size_x, float size_y, float size_z, int type) {
		SettingBlocks(count_x, count_y, count_z, midx, midy, midz, size_x, size_y, size_z);

		if (type == 0) { // 정점별 랜덤 색
			for (int i = 0; i < blockCount[z]; i++) {
				for (int j = 0; j < blockCount[y]; j++) {
					for (int k = 0; k < blockCount[x]; k++) {
						blocks[i][j][k].MakeCube(-size_x + blockSize[x] * (float)k * 2.0f + blockSize[x] + midx,
							-size_y + blockSize[y] * (float)j * 2.0f + blockSize[y] + midy,
							-size_z + blockSize[z] * (float)i * 2.0f + blockSize[z] + midz,
							blockSize[x], blockSize[y], blockSize[z]);
					}
				}
			}
		}
		else if (type == 1) { // 면별 랜덤 색
			for (int i = 0; i < blockCount[z]; i++) {
				for (int j = 0; j < blockCount[y]; j++) {
					for (int k = 0; k < blockCount[x]; k++) {
						blocks[i][j][k].MakeCube(-size_x + blockSize[x] * (float)k * 2.0f + blockSize[x] + midx,
							-size_y + blockSize[y] * (float)j * 2.0f + blockSize[y] + midy,
							-size_z + blockSize[z] * (float)i * 2.0f + blockSize[z] + midz,
							blockSize[x], blockSize[y], blockSize[z], 1);
					}
				}
			}
		}
		else if (type == 2) { // 도형별 랜덤 색
			for (int i = 0; i < blockCount[z]; i++) {
				for (int j = 0; j < blockCount[y]; j++) {
					for (int k = 0; k < blockCount[x]; k++) {
						blocks[i][j][k].MakeCube(-size_x + blockSize[x] * (float)k * 2.0f + blockSize[x] + midx,
							-size_y + blockSize[y] * (float)j * 2.0f + blockSize[y] + midy,
							-size_z + blockSize[z] * (float)i * 2.0f + blockSize[z] + midz,
							blockSize[x], blockSize[y], blockSize[z], F_urd(dre), F_urd(dre), F_urd(dre));
					}
				}
			}
		}
		else if (type == 3) { // 그래이 스케일
			float tempColor;
			for (int i = 0; i < blockCount[z]; i++) {
				for (int j = 0; j < blockCount[y]; j++) {
					for (int k = 0; k < blockCount[x]; k++) {
						tempColor = F_urd(dre);
						blocks[i][j][k].MakeCube(-size_x + blockSize[x] * (float)k * 2.0f + blockSize[x] + midx,
							-size_y + blockSize[y] * (float)j * 2.0f + blockSize[y] + midy,
							-size_z + blockSize[z] * (float)i * 2.0f + blockSize[z] + midz,
							blockSize[x], blockSize[y], blockSize[z], tempColor, tempColor, tempColor);
					}
				}
			}
		}
	}

	void Translate(float xVal, float yVal, float zVal) {
		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					blocks[i][j][k].Translate(xVal, yVal, zVal);
				}
			}
		}
	}

	void Rotate(const char type, const char type2, float degree) {
		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					blocks[i][j][k].Rotate(type, type2, degree);
				}
			}
		}
	}
	void Rotate_Block(const char type, float degree) {
		// 원점 기준 회전
		if (type == 'x') {
			blockRot = glm::rotate(blockRot, (GLfloat)glm::radians(degree), glm::vec3(1.0, 0.0, 0.0)); //--- x축에대하여 회전
		}
		else if (type == 'y') {
			blockRot = glm::rotate(blockRot, (GLfloat)glm::radians(degree), glm::vec3(0.0, 1.0, 0.0)); //--- y축에대하여 회전
		}
		else if (type == 'z') {
			blockRot = glm::rotate(blockRot, (GLfloat)glm::radians(degree), glm::vec3(0.0, 0.0, 1.0)); //--- z축에대하여 회전
		}
	}

	GLvoid Draw(unsigned int transformLocation) {
		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					blocks[i][j][k].Draw(transformLocation);
				}
			}
		}
	}
	GLvoid Draw(unsigned int transformLocation, glm::mat4 afterMat) {
		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					blocks[i][j][k].Draw(transformLocation, afterMat * blockRot);
				}
			}
		}
	}

	void SettingBlocks(int count_x, int count_y, int count_z, float midx, float midy, float midz, float size_x, float size_y, float size_z) {
		if (blockCount[x] != 0 || blockCount[y] != 0 || blockCount[z] != 0) { ClearBlocks(); }
		blockCount[x] = count_x;
		blockCount[y] = count_y;
		blockCount[z] = count_z;

		blocks = new Figure * *[blockCount[z]];
		for (int i = 0; i < blockCount[z]; i++) {
			blocks[i] = new Figure * [blockCount[y]];
			for (int j = 0; j < blockCount[y]; j++) {
				blocks[i][j] = new Figure[blockCount[x]];
			}
		}

		blockSize[x] = size_x / (float)blockCount[x];
		blockSize[y] = size_y / (float)blockCount[y];
		blockSize[z] = size_z / (float)blockCount[z];
	}
	void ClearBlocks() {
		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				delete[] blocks[i][j];
			}
			delete[] blocks[i];
		}
		delete[] blocks;
		blockCount[x] = 0;
		blockCount[y] = 0;
		blockCount[z] = 0;
	}
	void Reset() {
		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					blocks[i][j][k].Reset();
				}
			}
		}
	}

	~Block() {
		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				delete[] blocks[i][j];
			}
			delete[] blocks[i];
		}
		delete[] blocks;
	}
};

class Cube {
private:
	Block*** cube_blocks;
	int cube_blockCount[3];
	float cube_blockSize[3];
	int blockCount[3];
	float blockSize[3];
	const int x = 0;
	const int y = 1;
	const int z = 2;
	glm::mat4 cubeRot;
public:
	Cube() {
		cube_blocks = NULL;
		cube_blockCount[x] = 0;
		cube_blockCount[y] = 0;
		cube_blockCount[z] = 0;
		cube_blockSize[x] = 0.0;
		cube_blockSize[y] = 0.0;
		cube_blockSize[z] = 0.0;
		blockCount[x] = 0;
		blockCount[y] = 0;
		blockCount[z] = 0;
		blockSize[x] = 0.0;
		blockSize[y] = 0.0;
		blockSize[z] = 0.0;
		cubeRot = glm::mat4(1.0f);
	}
	GLvoid MakeCube(int count_x, int count_y, int count_z, int bCountx, int bCounty, int bCountz) {
		SettingCube(count_x, count_y, count_z, bCountx, bCounty, bCountz, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].MakeBlock(bCountx, bCounty, bCountz, -1.0f + cube_blockSize[x] * (float)k * 2.0f + cube_blockSize[x],
						-1.0f + cube_blockSize[y] * (float)j * 2.0f + cube_blockSize[y],
						-1.0f + cube_blockSize[z] * (float)i * 2.0f + cube_blockSize[z],
						cube_blockSize[x], cube_blockSize[y], cube_blockSize[z]);
				}
			}
		}
	}
	GLvoid MakeCube(int count_x, int count_y, int count_z, int bCountx, int bCounty, int bCountz, float midx, float midy, float midz) {
		SettingCube(count_x, count_y, count_z, bCountx, bCounty, bCountz, midx, midy, midz, 1.0f, 1.0f, 1.0f);

		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].MakeBlock(bCountx, bCounty, bCountz, -1.0f + cube_blockSize[x] * (float)k * 2.0f + cube_blockSize[x] + midx,
						-1.0f + cube_blockSize[y] * (float)j * 2.0f + cube_blockSize[y] + midy,
						-1.0f + cube_blockSize[z] * (float)i * 2.0f + cube_blockSize[z] + midz,
						cube_blockSize[x], cube_blockSize[y], cube_blockSize[z]);
				}
			}
		}
	}
	GLvoid MakeCube(int count_x, int count_y, int count_z, int bCountx, int bCounty, int bCountz, float midx, float midy, float midz, float size_x, float size_y, float size_z) {
		SettingCube(count_x, count_y, count_z, bCountx, bCounty, bCountz, midx, midy, midz, size_x, size_y, size_z);

		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].MakeBlock(bCountx, bCounty, bCountz, -size_x + cube_blockSize[x] * (float)k * 2.0f + cube_blockSize[x] + midx,
						-size_y + cube_blockSize[y] * (float)j * 2.0f + cube_blockSize[y] + midy,
						-size_z + cube_blockSize[z] * (float)i * 2.0f + cube_blockSize[z] + midz,
						cube_blockSize[x], cube_blockSize[y], cube_blockSize[z]);
				}
			}
		}
	}
	GLvoid MakeCube(int count_x, int count_y, int count_z, int bCountx, int bCounty, int bCountz, float midx, float midy, float midz, float size_x, float size_y, float size_z, float colorR, float colorG, float colorB) {
		SettingCube(count_x, count_y, count_z, bCountx, bCounty, bCountz, midx, midy, midz, size_x, size_y, size_z);


		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].MakeBlock(bCountx, bCounty, bCountz, -size_x + cube_blockSize[x] * (float)k * 2.0f + cube_blockSize[x] + midx,
						-size_y + cube_blockSize[y] * (float)j * 2.0f + cube_blockSize[y] + midy,
						-size_z + cube_blockSize[z] * (float)i * 2.0f + cube_blockSize[z] + midz,
						cube_blockSize[x], cube_blockSize[y], cube_blockSize[z], colorR, colorG, colorB);
				}
			}
		}
	}
	GLvoid MakeCube(int count_x, int count_y, int count_z, int bCountx, int bCounty, int bCountz, float midx, float midy, float midz, float size_x, float size_y, float size_z, int type) {
		SettingCube(count_x, count_y, count_z, bCountx, bCounty, bCountz, midx, midy, midz, size_x, size_y, size_z);

		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].MakeBlock(bCountx, bCounty, bCountz, -size_x + cube_blockSize[x] * (float)k * 2.0f + cube_blockSize[x] + midx,
						-size_y + cube_blockSize[y] * (float)j * 2.0f + cube_blockSize[y] + midy,
						-size_z + cube_blockSize[z] * (float)i * 2.0f + cube_blockSize[z] + midz,
						cube_blockSize[x], cube_blockSize[y], cube_blockSize[z], 1);
				}
			}
		}
	}

	void Translate(float xVal, float yVal, float zVal) {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].Translate(xVal, yVal, zVal);
				}
			}
		}
	}

	void Rotate(const char type, const char type2, float degree) {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].Rotate(type, type2, degree);
				}
			}
		}
	}
	void Rotate_Cube(const char type, float degree) {
		// 원점 기준 회전
		if (type == 'x') {
			cubeRot = glm::rotate(cubeRot, (GLfloat)glm::radians(degree), glm::vec3(1.0, 0.0, 0.0)); //--- x축에대하여 회전
		}
		else if (type == 'y') {
			cubeRot = glm::rotate(cubeRot, (GLfloat)glm::radians(degree), glm::vec3(0.0, 1.0, 0.0)); //--- y축에대하여 회전
		}
		else if (type == 'z') {
			cubeRot = glm::rotate(cubeRot, (GLfloat)glm::radians(degree), glm::vec3(0.0, 0.0, 1.0)); //--- z축에대하여 회전
		}
	}

	GLvoid Draw(unsigned int transformLocation) {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].Draw(transformLocation);
				}
			}
		}
	}
	GLvoid Draw_Use_CubeMat(unsigned int transformLocation) {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].Draw(transformLocation, cubeRot);
				}
			}
		}
	}

	void SettingCube(int count_x, int count_y, int count_z, int bCountx, int bCounty, int bCountz, float midx, float midy, float midz, float size_x, float size_y, float size_z) {
		if (cube_blockCount[x] != 0 || cube_blockCount[y] != 0 || cube_blockCount[z] != 0) { ClearBlocks(); }
		cube_blockCount[x] = count_x;
		cube_blockCount[y] = count_y;
		cube_blockCount[z] = count_z;
		blockCount[x] = bCountx;
		blockCount[y] = bCounty;
		blockCount[z] = bCountz;

		cube_blocks = new Block**[cube_blockCount[z]];
		for (int i = 0; i < cube_blockCount[z]; i++) {
			cube_blocks[i] = new Block*[cube_blockCount[y]];
			for (int j = 0; j < cube_blockCount[y]; j++) {
				cube_blocks[i][j] = new Block[cube_blockCount[x]];
			}
		}

		cube_blockSize[x] = size_x / (float)cube_blockCount[x];
		cube_blockSize[y] = size_y / (float)cube_blockCount[y];
		cube_blockSize[z] = size_z / (float)cube_blockCount[z];
	}
	void ClearBlocks() {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				delete[] cube_blocks[i][j];
			}
			delete[] cube_blocks[i];
		}
		delete[] cube_blocks;
		cube_blockCount[x] = 0;
		cube_blockCount[y] = 0;
		cube_blockCount[z] = 0;
	}
	void Reset() {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].Reset();
				}
			}
		}
	}

	~Cube() {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				delete[] cube_blocks[i][j];
			}
			delete[] cube_blocks[i];
		}
		delete[] cube_blocks;
	}
};