#pragma once
#include "includer.h"
// Cube Rotate Type List
#define CUBE_X 'x'
#define CUBE_Y 'y'
#define CUBE_Z 'z'
#define CUBE_ROT_T_ORIGIN 'a'
#define CUBE_ROT_T_INDPEN_SELF 'b'
#define CUBE_ROT_T_DPEN_SELF 'c'
#define CUBE_SCALE_T_ORIGIN 'o'
#define CUBE_SCALE_T_SELF 'm'

// Cube MakeCube Type List
#define CUBE_COLOR_POINT_RAND 0
#define CUBE_COLOR_FIGURE_SIDE_RAND 1
#define CUBE_COLOR_FIGURE_RAND 2
#define CUBE_COLOR_FIGURE_GRAY 3
#define CUBE_COLOR_FIGURE_SMOOTH 6
#define CUBE_COLOR_BLOCK_RAND 4
#define CUBE_COLOR_BLOCK_SMOOTH 5
#define CUBE_COLOR_CUBE_SIDE_DEFAULT 7
#define CUBE_COLOR_FIGURE_GRAY_SMOOTH 8

#define CUBE_COLORTYPE_BLOCK 4

// Cube Print Type List
#define CUBE_PRINT_WALL 0
#define CUBE_PRINT_ROAD 1

// Rotate Return Val
#define CUBE_SEQUENCE_END false
#define CUBE_SEQUENCE_ING true

// Cube Print Animation Type List
#define CUBE_ANIMATION_DEFALUT 1
#define CUBE_ANIMATION_MAZE 2

extern std::random_device rd;
extern std::default_random_engine dre;
extern std::uniform_real_distribution<float> F_urd;

class Data {
private:
public:
	Data() {
		sel = 0;
		line = 0;
		degree = 0;
	}
	Data(int s, int l, int d) {
		sel = s;
		line = l;
		degree = d;
	}
	int sel; // x,y,z
	int line;
	int degree; // -1,1
};

class MyNode {
private:
	Data* data;
	MyNode* next;
	MyNode* prev;
public:
	MyNode() {
		data = NULL;
		next = NULL;
		prev = NULL;
	}
	void SetData(Data* d) { data = d; }
	void SetNext(MyNode* n) { next = n; }
	void SetPrev(MyNode* p) { prev = p; }
	Data* GetData() { return data; }
	MyNode* GetNext() { return next; }
	MyNode* GetPrev() { return prev; }

	~MyNode() {
	}
};

class CubeQueue { // 회전 전용 큐
private:
	MyNode* node;

public:
	CubeQueue() {
		node = NULL;
	}

	void push(Data* NewData) {
		MyNode* NewNode = new MyNode;
		NewNode->SetData(NewData);
		if (node == NULL) {
			node = NewNode;
			NewNode->SetNext(NULL);
			NewNode->SetPrev(NULL);
		}
		else {
			Data* temp = node->GetData();
			if (temp->sel == NewData->sel && temp->line == NewData->line && (temp->degree * -1) == NewData->degree) { // 두 회전이 서로 역방향이다
				// 서로 상쇄되니 넣지 말고 현재 데이터를 빼자
				delete pop();
				delete NewData;
				delete NewNode;
			}
			else {
				NewNode->SetNext(NULL);
				NewNode->SetPrev(node);
				node->SetNext(NewNode);
				node = NewNode;
			}
		}
	}
	Data* pop() {
		if (node != NULL) {
			MyNode* temp = node;
			node = node->GetPrev();
			if (node != NULL) { node->SetNext(NULL); }
			Data* temp2 = temp->GetData();
			delete temp;
			return temp2;
		}
		else {
			return NULL;
		}
	}
};

class Figure {
private:
	GLfloat point[8][6];
	GLfloat vertex[36][12];
	GLfloat midPoint[3];
	GLfloat size[3];
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
			for (int j = 0; j < 12; j++) {
				vertex[i][j] = 0.0f;
			}
		}
		for (int i = 0; i < 3; i++) {
			midPoint[i] = 0.0f;
			size[i] = 0.0f;
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
		for (int i = 0; i < countOfData; i++) {
			vertex[i][6] = 1.0f;
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
		for (int i = 0; i < countOfData; i++) {
			vertex[i][6] = 1.0f;
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
		for (int i = 0; i < countOfData; i++) {
			vertex[i][6] = 1.0f;
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
		for (int i = 0; i < countOfData; i++) {
			vertex[i][6] = 1.0f;
		}
		MakeTransformMat();
		InitBuffer();
	}
	void MakeLine_N(float sx, float sy, float sz, float ex, float ey, float ez) {
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
			for (int j = 0; j < 3; j++) {
				vertex[i][j + 6] = vertex[1][j] - vertex[0][j];
			}
		}
		for (int i = 0; i < countOfData; i++) {
			vertex[i][6] = 1.0f;
		}
		MakeTransformMat();
		InitBuffer();
	}
	void MakeCube() {
		float tempsize = 0.3f;
		InitCubeData(0.0f, 0.0f, 0.0f, tempsize, tempsize, tempsize);

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				vertex[j + 6 * i][3] = F_urd(dre);
				vertex[j + 6 * i][4] = F_urd(dre);
				vertex[j + 6 * i][5] = F_urd(dre);
			}
		}

		InitCubeNormal();
		//for (int i = 0; i < 3; i++) {
		//	for (int j = 0; j < 6; j++) {
		//		float tempcolor = F_urd(dre);
		//		for (int k = 0; k < 6; k++) {
		//			vertex[k + 6 * j][i + 3] = tempcolor;
		//		}
		//	}
		//}
		MakeTransformMat();
		InitBuffer();
	}
	void MakeCube(float midx, float midy, float midz, float sizex, float sizey, float sizez) {
		InitCubeData(midx, midy, midz, sizex, sizey, sizez);

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				vertex[j + 6 * i][3] = F_urd(dre);
				vertex[j + 6 * i][4] = F_urd(dre);
				vertex[j + 6 * i][5] = F_urd(dre);
			}
		}

		InitCubeNormal();
		MakeTransformMat();
		InitBuffer();
	}
	void MakeCube(float midx, float midy, float midz, float sizex, float sizey, float sizez, float colorR, float colorG, float colorB) {
		InitCubeData(midx, midy, midz, sizex, sizey, sizez);

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				vertex[j + 6 * i][3] = colorR;
				vertex[j + 6 * i][4] = colorG;
				vertex[j + 6 * i][5] = colorB;
			}
		}

		InitCubeNormal();
		MakeTransformMat();
		InitBuffer();
	}
	void MakeCube(float midx, float midy, float midz, float sizex, float sizey, float sizez, float colorR, float colorG, float colorB, float alpha) {
		InitCubeData(midx, midy, midz, sizex, sizey, sizez);
		for (int i = 0; i < countOfData; i++) { // alpha
			vertex[i][6] = alpha;
		}
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				vertex[j + 6 * i][3] = colorR;
				vertex[j + 6 * i][4] = colorG;
				vertex[j + 6 * i][5] = colorB;
			}
		}

		InitCubeNormal();
		MakeTransformMat();
		InitBuffer();
	}
	void MakeCube(float midx, float midy, float midz, float sizex, float sizey, float sizez, float colorR[6], float colorG[6], float colorB[6]) {
		InitCubeData(midx, midy, midz, sizex, sizey, sizez);

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				vertex[j + 6 * i][3] = colorR[i];
				vertex[j + 6 * i][4] = colorG[i];
				vertex[j + 6 * i][5] = colorB[i];
			}
		}
		InitCubeNormal();
		MakeTransformMat();
		InitBuffer();
	}
	void MakeCube(float midx, float midy, float midz, float sizex, float sizey, float sizez, int type) {
		InitCubeData(midx, midy, midz, sizex, sizey, sizez);

		if (type == 0) { // 정점별 같은 색
			int front = 0;
			int back = 4;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 3; j++) {
					point[i][j + 3] = F_urd(dre);
				}
			}

			for (int i = 3; i < 6; i++) {
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
		}
		else if (type == 1) { // 면 별 같은 색
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
		else if (type == 2) { // 도형별 같은 색
			float tempColor[3];
			for (int k = 0; k < 3; k++) {
				tempColor[k] = F_urd(dre);
			}
			for (int i = 0; i < 6; i++) {
				for (int k = 0; k < 3; k++) {
					for (int j = 0; j < 6; j++) {
						vertex[j + 6 * i][k + 3] = tempColor[k];
					}
				}
			}
		}
		else if (type == 3) { // 그레이 스케일
			float tempColor;
			tempColor = F_urd(dre);
			for (int i = 0; i < 6; i++) {
				for (int k = 0; k < 3; k++) {
					for (int j = 0; j < 6; j++) {
						vertex[j + 6 * i][k + 3] = tempColor;
					}
				}
			}
		}
		else { // 잘못된 타입의 경우 vertex 별 랜덤색
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 6; j++) {
					vertex[j + 6 * i][3] = F_urd(dre);
					vertex[j + 6 * i][4] = F_urd(dre);
					vertex[j + 6 * i][5] = F_urd(dre);
				}
			}
		}
		InitCubeNormal();
		MakeTransformMat();
		InitBuffer();
	}
	void MakeCube(float midx, float midy, float midz, float sizex, float sizey, float sizez, float alpha, int type) {
		InitCubeData(midx, midy, midz, sizex, sizey, sizez);
		for (int i = 0; i < countOfData; i++) { // alpha
			vertex[i][6] = alpha;
		}
		if (type == 0) { // 정점별 같은 색
			int front = 0;
			int back = 4;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 3; j++) {
					point[i][j + 3] = F_urd(dre);
				}
			}

			for (int i = 3; i < 6; i++) {
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
		}
		else if (type == 1) { // 면 별 같은 색
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
		else if (type == 2) { // 도형별 같은 색
			float tempColor[3];
			for (int k = 0; k < 3; k++) {
				tempColor[k] = F_urd(dre);
			}
			for (int i = 0; i < 6; i++) {
				for (int k = 0; k < 3; k++) {
					for (int j = 0; j < 6; j++) {
						vertex[j + 6 * i][k + 3] = tempColor[k];
					}
				}
			}
		}
		else if (type == 3) { // 그레이 스케일
			float tempColor;
			tempColor = F_urd(dre);
			for (int i = 0; i < 6; i++) {
				for (int k = 0; k < 3; k++) {
					for (int j = 0; j < 6; j++) {
						vertex[j + 6 * i][k + 3] = tempColor;
					}
				}
			}
		}
		else { // 잘못된 타입의 경우 vertex 별 랜덤색
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 6; j++) {
					vertex[j + 6 * i][3] = F_urd(dre);
					vertex[j + 6 * i][4] = F_urd(dre);
					vertex[j + 6 * i][5] = F_urd(dre);
				}
			}
		}
		InitCubeNormal();
		MakeTransformMat();
		InitBuffer();
	}
	void MakePyramid() {
		float tempsize = 0.3f;
		InitPyramidData(0.0f, 0.0f, 0.0f, tempsize, tempsize, tempsize);

		for (int j = 0; j < 3; j++) { point[0][j + 3] = F_urd(dre); }
		for (int j = 0; j < 3; j++) { point[1][j + 3] = F_urd(dre); }
		for (int j = 0; j < 3; j++) { point[2][j + 3] = F_urd(dre); }
		for (int j = 0; j < 3; j++) { point[3][j + 3] = F_urd(dre); }
		for (int j = 0; j < 3; j++) { point[4][j + 3] = F_urd(dre); }

		for (int i = 0; i < 3; i++) {
			//밑면
			vertex[0 + 3 * 0][i + 3] = point[1][i + 3];
			vertex[1 + 3 * 0][i + 3] = point[2][i + 3];
			vertex[2 + 3 * 0][i + 3] = point[3][i + 3];
			vertex[0 + 3 * 1][i + 3] = point[1][i + 3];
			vertex[1 + 3 * 1][i + 3] = point[3][i + 3];
			vertex[2 + 3 * 1][i + 3] = point[0][i + 3];
			//앞면
			vertex[0 + 3 * 2][i + 3] = point[4][i + 3];
			vertex[1 + 3 * 2][i + 3] = point[3][i + 3];
			vertex[2 + 3 * 2][i + 3] = point[2][i + 3];
			//뒷면
			vertex[0 + 3 * 3][i + 3] = point[4][i + 3];
			vertex[1 + 3 * 3][i + 3] = point[1][i + 3];
			vertex[2 + 3 * 3][i + 3] = point[0][i + 3];
			//왼쪽 면
			vertex[0 + 3 * 4][i + 3] = point[4][i + 3];
			vertex[1 + 3 * 4][i + 3] = point[0][i + 3];
			vertex[2 + 3 * 4][i + 3] = point[3][i + 3];
			//오른쪽 면
			vertex[0 + 3 * 5][i + 3] = point[4][i + 3];
			vertex[1 + 3 * 5][i + 3] = point[2][i + 3];
			vertex[2 + 3 * 5][i + 3] = point[1][i + 3];
		}
		InitPyramidNormal();
		MakeTransformMat();
		InitBuffer();
	}
	void MakePyramid(float midx, float midy, float midz, float sizex, float sizey, float sizez) {
		InitPyramidData(midx, midy, midz, sizex, sizey, sizez);
		for (int j = 0; j < 3; j++) { point[0][j + 3] = F_urd(dre); }
		for (int j = 0; j < 3; j++) { point[1][j + 3] = F_urd(dre); }
		for (int j = 0; j < 3; j++) { point[2][j + 3] = F_urd(dre); }
		for (int j = 0; j < 3; j++) { point[3][j + 3] = F_urd(dre); }
		for (int j = 0; j < 3; j++) { point[4][j + 3] = F_urd(dre); }

		for (int i = 0; i < 3; i++) {
			//밑면
			vertex[0 + 3 * 0][i + 3] = point[1][i + 3];
			vertex[1 + 3 * 0][i + 3] = point[2][i + 3];
			vertex[2 + 3 * 0][i + 3] = point[3][i + 3];
			vertex[0 + 3 * 1][i + 3] = point[1][i + 3];
			vertex[1 + 3 * 1][i + 3] = point[3][i + 3];
			vertex[2 + 3 * 1][i + 3] = point[0][i + 3];
			//앞면
			vertex[0 + 3 * 2][i + 3] = point[4][i + 3];
			vertex[1 + 3 * 2][i + 3] = point[3][i + 3];
			vertex[2 + 3 * 2][i + 3] = point[2][i + 3];
			//뒷면
			vertex[0 + 3 * 3][i + 3] = point[4][i + 3];
			vertex[1 + 3 * 3][i + 3] = point[1][i + 3];
			vertex[2 + 3 * 3][i + 3] = point[0][i + 3];
			//왼쪽 면
			vertex[0 + 3 * 4][i + 3] = point[4][i + 3];
			vertex[1 + 3 * 4][i + 3] = point[0][i + 3];
			vertex[2 + 3 * 4][i + 3] = point[3][i + 3];
			//오른쪽 면
			vertex[0 + 3 * 5][i + 3] = point[4][i + 3];
			vertex[1 + 3 * 5][i + 3] = point[2][i + 3];
			vertex[2 + 3 * 5][i + 3] = point[1][i + 3];
		}
		InitPyramidNormal();
		MakeTransformMat();
		InitBuffer();
	}

	void InitCubeData(float midx, float midy, float midz, float sizex, float sizey, float sizez) {
		midPoint[0] = midx;
		midPoint[1] = midy;
		midPoint[2] = midz;
		size[0] = sizex;
		size[1] = sizey;
		size[2] = sizez;

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

		for (int i = 0; i < 3; i++) {
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
		for (int i = 0; i < countOfData; i++) { // alpha
			vertex[i][6] = 1.0f;
		}
		InitCubeUVPos();
	}
	void InitPyramidData(float midx, float midy, float midz, float sizex, float sizey, float sizez) {
		midPoint[0] = midx;
		midPoint[1] = midy;
		midPoint[2] = midz;
		size[0] = sizex;
		size[1] = sizey;
		size[2] = sizez;

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

		point[1][0] = midx + sizex;
		point[1][1] = midy - sizey;
		point[1][2] = midz - sizez;

		point[2][0] = midx + sizex;
		point[2][1] = midy - sizey;
		point[2][2] = midz + sizez;

		point[3][0] = midx - sizex;
		point[3][1] = midy - sizey;
		point[3][2] = midz + sizez;

		point[4][0] = midx;
		point[4][1] = midy + sizey;
		point[4][2] = midz;


		for (int i = 0; i < 3; i++) {
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
		for (int i = 0; i < countOfData; i++) {
			vertex[i][6] = 1.0f;
		}
		InitPyramidUVPos();
	}

	glm::vec3 calNormal(int sv, int sel, int vcount)
	{
		float v1[3], v2[3], cube_sideSelecter_i[3];
		static const int x = 0;
		static const int y = 1;
		static const int z = 2;

		v1[x] = vertex[sv + 0 + vcount * sel][x] - vertex[sv + 1 + vcount * sel][x];
		v1[y] = vertex[sv + 0 + vcount * sel][y] - vertex[sv + 1 + vcount * sel][y];
		v1[z] = vertex[sv + 0 + vcount * sel][z] - vertex[sv + 1 + vcount * sel][z];

		v2[x] = vertex[sv + 1 + vcount * sel][x] - vertex[sv + 2 + vcount * sel][x];
		v2[y] = vertex[sv + 1 + vcount * sel][y] - vertex[sv + 2 + vcount * sel][y];
		v2[z] = vertex[sv + 1 + vcount * sel][z] - vertex[sv + 2 + vcount * sel][z];

		cube_sideSelecter_i[x] = v1[y] * v2[z] - v1[z] * v2[y];
		cube_sideSelecter_i[y] = v1[z] * v2[x] - v1[x] * v2[z];
		cube_sideSelecter_i[z] = v1[x] * v2[y] - v1[y] * v2[x];

		float tlen = (float)sqrt(cube_sideSelecter_i[x] * cube_sideSelecter_i[x] + cube_sideSelecter_i[y] * cube_sideSelecter_i[y] + cube_sideSelecter_i[z] * cube_sideSelecter_i[z]);

		return glm::vec3(cube_sideSelecter_i[x] / tlen, cube_sideSelecter_i[y] / tlen, cube_sideSelecter_i[z] / tlen);
	}
	void InitCubeNormal() {
		glm::vec3 t[6];
		for (int i = 0; i < 6; i++) {
			t[i] = calNormal(0, i, 6);
		}

		for (int i = 0; i < 6; i++) {	// 노멀 저장
			for (int j = 0; j < 3; j++) {
				//앞면
				vertex[i + 6 * 0][j + 7] = t[0][j];
				//뒷면
				vertex[i + 6 * 1][j + 7] = t[1][j];
				//오른쪽 면
				vertex[i + 6 * 2][j + 7] = t[2][j];
				//왼쪽 면
				vertex[i + 6 * 3][j + 7] = t[3][j];
				//위쪽 면
				vertex[i + 6 * 4][j + 7] = t[4][j];
				//아래쪽 면
				vertex[i + 6 * 5][j + 7] = t[5][j];
			}
		}
	}
	void InitPyramidNormal() {
		glm::vec3 t[5];
		for (int i = 0; i < 5; i++) {
			t[i] = calNormal(0, i + 1, 3);
		}

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				//밑면
				vertex[j + 3 * 0][7 + i] = t[0][i];
				vertex[j + 3 * 1][7 + i] = t[0][i];
				//앞면
				vertex[j + 3 * 2][7 + i] = t[1][i];
				//뒷면
				vertex[j + 3 * 3][7 + i] = t[2][i];
				//왼쪽 면
				vertex[j + 3 * 4][7 + i] = t[3][i];
				//오른쪽 면
				vertex[j + 3 * 5][7 + i] = t[4][i];
			}
		}
	}

	void InitCubeUVPos() {
		const int U = 10;
		const int V = 11;
		// 앞면
		vertex[0 + 6 * 0][U] = 0.0f;			vertex[0 + 6 * 0][V] = 1.0f;
		vertex[1 + 6 * 0][U] = 0.0f;			vertex[1 + 6 * 0][V] = 0.0f;
		vertex[2 + 6 * 0][U] = 1.0f;			vertex[2 + 6 * 0][V] = 0.0f;
		vertex[3 + 6 * 0][U] = 0.0f;			vertex[3 + 6 * 0][V] = 1.0f;
		vertex[4 + 6 * 0][U] = 1.0f;			vertex[4 + 6 * 0][V] = 0.0f;
		vertex[5 + 6 * 0][U] = 1.0f;			vertex[5 + 6 * 0][V] = 1.0f;
		//뒷면
		vertex[0 + 6 * 1][U] = 0.0f;			vertex[0 + 6 * 1][V] = 1.0f;
		vertex[1 + 6 * 1][U] = 0.0f;			vertex[1 + 6 * 1][V] = 0.0f;
		vertex[2 + 6 * 1][U] = 1.0f;			vertex[2 + 6 * 1][V] = 0.0f;
		vertex[3 + 6 * 1][U] = 0.0f;			vertex[3 + 6 * 1][V] = 1.0f;
		vertex[4 + 6 * 1][U] = 1.0f;			vertex[4 + 6 * 1][V] = 0.0f;
		vertex[5 + 6 * 1][U] = 1.0f;			vertex[5 + 6 * 1][V] = 1.0f;
		//오른쪽 면
		vertex[0 + 6 * 2][U] = 0.0f;			vertex[0 + 6 * 2][V] = 1.0f;
		vertex[1 + 6 * 2][U] = 0.0f;			vertex[1 + 6 * 2][V] = 0.0f;
		vertex[2 + 6 * 2][U] = 1.0f;			vertex[2 + 6 * 2][V] = 0.0f;
		vertex[3 + 6 * 2][U] = 0.0f;			vertex[3 + 6 * 2][V] = 1.0f;
		vertex[4 + 6 * 2][U] = 1.0f;			vertex[4 + 6 * 2][V] = 0.0f;
		vertex[5 + 6 * 2][U] = 1.0f;			vertex[5 + 6 * 2][V] = 1.0f;
		//왼쪽 면
		vertex[0 + 6 * 3][U] = 0.0f;			vertex[0 + 6 * 3][V] = 1.0f;
		vertex[1 + 6 * 3][U] = 0.0f;			vertex[1 + 6 * 3][V] = 0.0f;
		vertex[2 + 6 * 3][U] = 1.0f;			vertex[2 + 6 * 3][V] = 0.0f;
		vertex[3 + 6 * 3][U] = 0.0f;			vertex[3 + 6 * 3][V] = 1.0f;
		vertex[4 + 6 * 3][U] = 1.0f;			vertex[4 + 6 * 3][V] = 0.0f;
		vertex[5 + 6 * 3][U] = 1.0f;			vertex[5 + 6 * 3][V] = 1.0f;
		//위쪽 면
		vertex[0 + 6 * 4][U] = 0.0f;			vertex[0 + 6 * 4][V] = 1.0f;
		vertex[1 + 6 * 4][U] = 0.0f;			vertex[1 + 6 * 4][V] = 0.0f;
		vertex[2 + 6 * 4][U] = 1.0f;			vertex[2 + 6 * 4][V] = 0.0f;
		vertex[3 + 6 * 4][U] = 0.0f;			vertex[3 + 6 * 4][V] = 1.0f;
		vertex[4 + 6 * 4][U] = 1.0f;			vertex[4 + 6 * 4][V] = 0.0f;
		vertex[5 + 6 * 4][U] = 1.0f;			vertex[5 + 6 * 4][V] = 1.0f;
		//아래쪽 면
		vertex[0 + 6 * 5][U] = 0.0f;			vertex[0 + 6 * 5][V] = 1.0f;
		vertex[1 + 6 * 5][U] = 0.0f;			vertex[1 + 6 * 5][V] = 0.0f;
		vertex[2 + 6 * 5][U] = 1.0f;			vertex[2 + 6 * 5][V] = 0.0f;
		vertex[3 + 6 * 5][U] = 0.0f;			vertex[3 + 6 * 5][V] = 1.0f;
		vertex[4 + 6 * 5][U] = 1.0f;			vertex[4 + 6 * 5][V] = 0.0f;
		vertex[5 + 6 * 5][U] = 1.0f;			vertex[5 + 6 * 5][V] = 1.0f;
	}
	void InitPyramidUVPos() {
		const int U = 10;
		const int V = 11;

		for (int i = 0; i < 3; i++) {
			//밑면
			vertex[0 + 3 * 0][U] = 0.0f;			vertex[0 + 3 * 0][V] = 0.0f;
			vertex[1 + 3 * 0][U] = 0.0f;			vertex[1 + 3 * 0][V] = 1.0f;
			vertex[2 + 3 * 0][U] = 1.0f;			vertex[2 + 3 * 0][V] = 1.0f;
			vertex[0 + 3 * 1][U] = 0.0f;			vertex[0 + 3 * 1][V] = 0.0f;
			vertex[1 + 3 * 1][U] = 1.0f;			vertex[1 + 3 * 1][V] = 1.0f;
			vertex[2 + 3 * 1][U] = 1.0f;			vertex[2 + 3 * 1][V] = 0.0f;
			//앞면
			vertex[0 + 3 * 2][U] = 0.5f;			vertex[0 + 3 * 2][V] = 1.0f;
			vertex[1 + 3 * 2][U] = 0.0f;			vertex[1 + 3 * 2][V] = 0.0f;
			vertex[2 + 3 * 2][U] = 1.0f;			vertex[2 + 3 * 2][V] = 0.0f;
			//뒷면
			vertex[0 + 3 * 3][U] = 0.5f;			vertex[0 + 3 * 3][V] = 1.0f;
			vertex[1 + 3 * 3][U] = 0.0f;			vertex[1 + 3 * 3][V] = 0.0f;
			vertex[2 + 3 * 3][U] = 1.0f;			vertex[2 + 3 * 3][V] = 0.0f;
			//왼쪽 면
			vertex[0 + 3 * 4][U] = 0.5f;			vertex[0 + 3 * 4][V] = 1.0f;
			vertex[1 + 3 * 4][U] = 0.0f;			vertex[1 + 3 * 4][V] = 0.0f;
			vertex[2 + 3 * 4][U] = 1.0f;			vertex[2 + 3 * 4][V] = 0.0f;
			//오른쪽 면
			vertex[0 + 3 * 5][U] = 0.5f;			vertex[0 + 3 * 5][V] = 1.0f;
			vertex[1 + 3 * 5][U] = 0.0f;			vertex[1 + 3 * 5][V] = 0.0f;
			vertex[2 + 3 * 5][U] = 1.0f;			vertex[2 + 3 * 5][V] = 0.0f;
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
	void Rotate(glm::vec3 shaft, float degree) {
		SelfRxy = glm::rotate(SelfRxy, (GLfloat)glm::radians(degree), shaft); //--- 임의의 축에 대하여 회전
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
			TR = So * Ry * Rx * Tz * Ty * Tx * SelfRxy * Sm;
		}
		else if (Shape == 1) {
			TR =  Ry * Rx * Tz * Tx * Ty;
		}
		else if (Shape == 2 || Shape == 3) {
			TR = So * Ry * Rx * Tz * Ty * Tx * SelfRxy * Sm;
		}
		else if (Shape == 4) {
			TR = Tz * Rx * Tx * Ry * Ty;
		}
		TR = TR;
	}

	bool CrashCheck(Figure* b) {
		float a_min[3];
		float a_max[3];
		float b_min[3];
		float b_max[3];
		float* btempMid = b->GetMidPos();
		float* bSize = b->GetSize();
		glm::vec4 bMid = b->GetTransformMat() * glm::vec4(btempMid[0], btempMid[1], btempMid[2], 1.0f);

		for (int i = 0; i < 3; i++) {
			a_min[i] = midPoint[i] - size[i];
			a_max[i] = midPoint[i] + size[i];
			b_min[i] = bMid[i] - bSize[i];
			b_max[i] = bMid[i] + bSize[i];
			if (a_min[i] > b_max[i] || a_max[i] < b_min[i]) {
				return false;
			}
		}
		return true;
	}
	bool CrashCheck(Figure* b, glm::mat4 mymat) {
		float a_min[3];
		float a_max[3];
		float b_min[3];
		float b_max[3];
		float* btempMid = b->GetMidPos();
		float* bSize = b->GetSize();
		glm::vec4 bMid = b->GetTransformMat() * glm::vec4(btempMid[0], btempMid[1], btempMid[2], 1.0f);
		glm::vec4 aMid = mymat * glm::vec4(midPoint[0], midPoint[1], midPoint[2], 1.0f);

		for (int i = 0; i < 3; i++) {
			a_min[i] = aMid[i] - size[i];
			a_max[i] = aMid[i] + size[i];
			b_min[i] = bMid[i] - bSize[i];
			b_max[i] = bMid[i] + bSize[i];
			if (a_min[i] > b_max[i] || a_max[i] < b_min[i]) {
				return false;
			}
		}
		return true;
	}

	glm::mat4 GetTransformMat() {
		return TR;
	}
	float* GetMidPos() { return midPoint; }
	glm::vec3 GetNowMidPosVec() { return glm::vec3(TR * glm::vec4(midPoint[0], midPoint[1], midPoint[2], 1.0f)); }
	float* GetSize() { return size; }

	GLuint InitBuffer()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (countOfData * 12) * sizeof(GLfloat), vertex, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)(10 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		return VAO;
	}

	~Figure() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
};


class Block {
private:
	Figure*** blocks;
	Figure myFigure;
	int*** mazeWall;
	int blockCount[3];
	float blockSize[3];
	const int x = 0;
	const int y = 1;
	const int z = 2;
	int figureType;
	int draw_count;
	glm::mat4 blockRot;
public:
	Block() {
		blocks = NULL;
		mazeWall = NULL;
		blockCount[x] = 0;
		blockCount[y] = 0;
		blockCount[z] = 0;
		blockSize[x] = 0.0;
		blockSize[y] = 0.0;
		blockSize[z] = 0.0;
		figureType = -1;
		draw_count = 0;
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
		figureType = CUBE_COLORTYPE_BLOCK;
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
		myFigure.MakeCube(midx, midy, midz, blockSize[x] * blockCount[x], blockSize[y] * blockCount[y], blockSize[z] * blockCount[z], colorR, colorG, colorB);
	}
	GLvoid MakeBlock(int count_x, int count_y, int count_z, float midx, float midy, float midz, float size_x, float size_y, float size_z, int type) {
		SettingBlocks(count_x, count_y, count_z, midx, midy, midz, size_x, size_y, size_z);
		figureType = type;

		if (type == CUBE_COLOR_POINT_RAND) { // 정점별 랜덤 색
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
		else if (type == CUBE_COLOR_FIGURE_SIDE_RAND) { // 면별 랜덤 색
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
		else if (type == CUBE_COLOR_FIGURE_RAND) { // 도형별 랜덤 색
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
		else if (type == CUBE_COLOR_FIGURE_GRAY) { // 그래이 스케일
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
		else if (type == CUBE_COLOR_FIGURE_SMOOTH) { // 그라데이션
			for (int i = 0; i < blockCount[z]; i++) {
				for (int j = 0; j < blockCount[y]; j++) {
					for (int k = 0; k < blockCount[x]; k++) {
						blocks[i][j][k].MakeCube(-size_x + blockSize[x] * (float)k * 2.0f + blockSize[x] + midx,
							-size_y + blockSize[y] * (float)j * 2.0f + blockSize[y] + midy,
							-size_z + blockSize[z] * (float)i * 2.0f + blockSize[z] + midz,
							blockSize[x], blockSize[y], blockSize[z], 
							(-size_x + blockSize[x] * (float)k * 2.0f + blockSize[x] + midx + size_x) / (size_x * 2.0f),
							(-size_y + blockSize[y] * (float)j * 2.0f + blockSize[y] + midy + size_y) / (size_y * 2.0f),
							(-size_z + blockSize[z] * (float)i * 2.0f + blockSize[z] + midz + size_z) / (size_z * 2.0f));
					}
				}
			}
		}
		else if (type == CUBE_COLOR_FIGURE_GRAY_SMOOTH) { // 흑백 그라데이션
			for (int i = 0; i < blockCount[z]; i++) {
				for (int j = 0; j < blockCount[y]; j++) {
					for (int k = 0; k < blockCount[x]; k++) {
						float temp = ((-size_x + blockSize[x] * (float)k * 2.0f + blockSize[x] + midx + size_x) + (-size_y + blockSize[y] * (float)j * 2.0f + blockSize[y] + midy + size_y) + (-size_z + blockSize[z] * (float)i * 2.0f + blockSize[z] + midz + size_z)) / ((size_x * 2.0f) + (size_y * 2.0f) + (size_z * 2.0f));
						blocks[i][j][k].MakeCube(-size_x + blockSize[x] * (float)k * 2.0f + blockSize[x] + midx,
							-size_y + blockSize[y] * (float)j * 2.0f + blockSize[y] + midy,
							-size_z + blockSize[z] * (float)i * 2.0f + blockSize[z] + midz,
							blockSize[x], blockSize[y], blockSize[z],
							temp,
							temp,
							temp);
					}
				}
			}
		}
	}
	GLvoid MakeBlock(int count_x, int count_y, int count_z, float midx, float midy, float midz, float size_x, float size_y, float size_z, float colorR[6], float colorG[6], float colorB[6]) {
		SettingBlocks(count_x, count_y, count_z, midx, midy, midz, size_x, size_y, size_z);
		figureType = CUBE_COLORTYPE_BLOCK;

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
		myFigure.MakeCube(midx, midy, midz, blockSize[x] * blockCount[x], blockSize[y] * blockCount[y], blockSize[z] * blockCount[z], colorR, colorG, colorB);
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
			blockRot = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(1.0, 0.0, 0.0)) * blockRot; //--- x축에대하여 회전
		}
		else if (type == 'y') {
			blockRot = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(0.0, 1.0, 0.0)) * blockRot; //--- y축에대하여 회전
		}
		else if (type == 'z') {
			blockRot = glm::rotate(glm::mat4(1.0f), (GLfloat)glm::radians(degree), glm::vec3(0.0, 0.0, 1.0)) * blockRot; //--- z축에대하여 회전
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
					if (mazeWall[i][j][k] != -1) {
						blocks[i][j][k].Draw(transformLocation, afterMat * blockRot);
					}
				}
			}
		}
	}
	GLvoid Draw(unsigned int transformLocation, glm::mat4 afterMat, int printType) {
		if (printType == CUBE_PRINT_ROAD) {
			for (int i = 0; i < blockCount[z]; i++) {
				for (int j = 0; j < blockCount[y]; j++) {
					for (int k = 0; k < blockCount[x]; k++) {
						if (mazeWall[i][j][k] != -1) {
							blocks[i][j][k].Draw(transformLocation, afterMat * blockRot);
						}
					}
				}
			}
		}
		else if (printType == CUBE_PRINT_WALL) {
			if (figureType == CUBE_COLORTYPE_BLOCK) {
				myFigure.Draw(transformLocation, afterMat * blockRot);
			}
			else {
				for (int i = 0; i < blockCount[z]; i++) {
					for (int j = 0; j < blockCount[y]; j++) {
						for (int k = 0; k < blockCount[x]; k++) {
							if (mazeWall[i][j][k]) {
								blocks[i][j][k].Draw(transformLocation, afterMat * blockRot);
							}
						}
					}
				}
			}
		}
	}
	bool Draw(unsigned int transformLocation, glm::mat4 afterMat, int printType, int val) {
		if (draw_count < blockCount[z] * blockCount[y] * blockCount[x]) {
			int tx, ty, tz;
			if (printType == CUBE_PRINT_ROAD) {
				for (int i = 0; i < draw_count; i++) {
					tx = i % blockCount[x];
					ty = i / blockCount[y] % blockCount[x];
					tz = i / blockCount[z] / blockCount[y] % blockCount[x];
					if (!mazeWall[tz][ty][tx]) {
						blocks[tz][ty][tx].Draw(transformLocation, afterMat * blockRot);
					}
				}
			}
			else if (printType == CUBE_PRINT_WALL) {
				for (int i = 0; i < draw_count; i++) {
					tx = i % blockCount[x];
					ty = i / blockCount[y] % blockCount[x];
					tz = i / blockCount[z] / blockCount[y] % blockCount[x];
					if (mazeWall[tz][ty][tx]) {
						blocks[tz][ty][tx].Draw(transformLocation, afterMat * blockRot);
					}
				}
			}
			draw_count += val;
			if (draw_count >= blockCount[z] * blockCount[y] * blockCount[x]) { return true; }
		}
		else {
			Draw(transformLocation, afterMat, printType);
		}
		return false;
	}
	bool Draw(unsigned int transformLocation, glm::mat4 afterMat, int printType, int animationType, int val) {
		if (animationType == CUBE_ANIMATION_DEFALUT) {
			if (draw_count < blockCount[z] * blockCount[y] * blockCount[x]) {
				int tx, ty, tz;
				if (printType == CUBE_PRINT_ROAD) {
					for (int i = 0; i < draw_count; i++) {
						tx = i % blockCount[x];
						ty = i / blockCount[y] % blockCount[x];
						tz = i / blockCount[z] / blockCount[y] % blockCount[x];
						if (!mazeWall[tz][ty][tx]) {
							blocks[tz][ty][tx].Draw(transformLocation, afterMat * blockRot);
						}
					}
				}
				else if (printType == CUBE_PRINT_WALL) {
					for (int i = 0; i < draw_count; i++) {
						tx = i % blockCount[x];
						ty = i / blockCount[y] % blockCount[x];
						tz = i / blockCount[z] / blockCount[y] % blockCount[x];
						if (mazeWall[tz][ty][tx]) {
							blocks[tz][ty][tx].Draw(transformLocation, afterMat * blockRot);
						}
					}
				}
				draw_count += val;
				if (draw_count >= blockCount[z] * blockCount[y] * blockCount[x]) { return true; }
			}
			else {
				Draw(transformLocation, afterMat, printType);
			}
		}
		else if (animationType == CUBE_ANIMATION_MAZE) {
			if (printType == CUBE_PRINT_ROAD) {
				for (int i = 0; i < blockCount[x]; i++) {
					for (int j = 0; j < blockCount[y]; j++) {
						for (int k = 0; k < blockCount[z]; k++) {
							if (draw_count > mazeWall[k][j][i] && mazeWall[k][j][i] != -1) {
								blocks[k][j][i].Draw(transformLocation, afterMat * blockRot);
							}
						}
					}
				}
			}
			else if (printType == CUBE_PRINT_WALL) {
				for (int i = 0; i < blockCount[x]; i++) {
					for (int j = 0; j < blockCount[y]; j++) {
						for (int k = 0; k < blockCount[z]; k++) {
							if (mazeWall[k][j][i] == -1) {
								blocks[k][j][i].Draw(transformLocation, afterMat * blockRot);
							}
						}
					}
				}
			}
			draw_count += val;
		}
		return false;
	}
	GLvoid DrawTest(unsigned int transformLocation, glm::mat4 afterMat, int posx, int posy, int posz) {
		if (mazeWall[posz][posy][posx] == -1) {
			blocks[posz][posy][posx].Draw(transformLocation, afterMat);
		}
	}

	void InputMaze(int*** maze, int startX, int startY, int startZ) {
		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					mazeWall[i][j][k] = maze[startZ * blockCount[z] + i][startY * blockCount[y] + j][startX * blockCount[x] + k];
				}
			}
		}
	}

	void SettingBlocks(int count_x, int count_y, int count_z, float midx, float midy, float midz, float size_x, float size_y, float size_z) {
		if (blockCount[x] != 0 || blockCount[y] != 0 || blockCount[z] != 0) { ClearBlocks(); }
		blockCount[x] = count_x;
		blockCount[y] = count_y;
		blockCount[z] = count_z;

		blocks = new Figure**[blockCount[z]];
		mazeWall = new int**[blockCount[z]];
		for (int i = 0; i < blockCount[z]; i++) {
			blocks[i] = new Figure*[blockCount[y]];
			mazeWall[i] = new int*[blockCount[y]];
			for (int j = 0; j < blockCount[y]; j++) {
				blocks[i][j] = new Figure[blockCount[x]];
				mazeWall[i][j] = new int[blockCount[x]];
			}
		}

		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					mazeWall[i][j][k] = true;
				}
			}
		}

		blockSize[x] = size_x / (float)blockCount[x];
		blockSize[y] = size_y / (float)blockCount[y];
		blockSize[z] = size_z / (float)blockCount[z];

		myFigure.MakeCube(midx, midy, midz, size_x, size_y, size_z);
	}
	void ClearBlocks() {
		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				delete[] blocks[i][j];
				delete[] mazeWall[i][j];
			}
			delete[] blocks[i];
			delete[] mazeWall[i];
		}
		delete[] blocks;
		delete[] mazeWall;
		blockCount[x] = 0;
		blockCount[y] = 0;
		blockCount[z] = 0;
		figureType = -1;
		draw_count = 0;
		blockRot = glm::mat4(1.0f);
	}
	void Reset() {
		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					blocks[i][j][k].Reset();
				}
			}
		}
		blockRot = glm::mat4(1.0f);
		figureType = -1;
		draw_count = 0;
	}

	bool CrashCheck(Figure* b) {
		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				for (int k = 0; k < blockCount[x]; k++) {
					if (mazeWall[i][j][k] == -1) {
						if (blocks[i][j][k].CrashCheck(b, blockRot)) {
							return true;
						}
					}
				}
			}
		}
		return false;
	}
	bool CrashCheck(Figure* b, int posx, int posy, int posz) {
		if (mazeWall[posz][posy][posx] == -1) {
			return blocks[posz][posy][posx].CrashCheck(b);
		}
		return false;
	}

	Figure* GetSpecificFigure(int posx, int posy, int posz, int printType) {
		if (printType == CUBE_PRINT_ROAD) {
			if (mazeWall[posz][posy][posx] != -1) {
				return &blocks[posz][posy][posx];
			}
			else {
				return NULL;
			}
		}
		else if (printType == CUBE_PRINT_WALL) {
			if (mazeWall[posz][posy][posx] == -1) {
				return &blocks[posz][posy][posx];
			}
			else {
				return NULL;
			}
		}
	}

	~Block() {
		for (int i = 0; i < blockCount[z]; i++) {
			for (int j = 0; j < blockCount[y]; j++) {
				delete[] blocks[i][j];
				delete[] mazeWall[i][j];
			}
			delete[] blocks[i];
			delete[] mazeWall[i];
		}
		delete[] blocks;
		delete[] mazeWall;
	}
};

class Cube {
private:
	Block*** cube_blocks;
	int**** cube_block_pos;
	int cube_blockCount[3];
	float cube_blockSize[3];
	int blockCount[3];
	float blockSize[3];
	const int x = 0;
	const int y = 1;
	const int z = 2;
	int draw_count;
	float nowRotDegree;
	glm::mat4 cubeRot;
	Figure gravity;
	Figure myObject;
	CubeQueue rotReverseDirQueue;
public:
	Cube() {
		cube_blocks = NULL;
		cube_block_pos = NULL;
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
		draw_count = 0;
		nowRotDegree = 0.0;
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

		if (type < 4) {
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[y]; j++) {
					for (int k = 0; k < cube_blockCount[x]; k++) {
						cube_blocks[i][j][k].MakeBlock(bCountx, bCounty, bCountz, -size_x + cube_blockSize[x] * (float)k * 2.0f + cube_blockSize[x] + midx,
							-size_y + cube_blockSize[y] * (float)j * 2.0f + cube_blockSize[y] + midy,
							-size_z + cube_blockSize[z] * (float)i * 2.0f + cube_blockSize[z] + midz,
							cube_blockSize[x], cube_blockSize[y], cube_blockSize[z], type);
					}
				}
			}
		}
		else if (type == CUBE_COLOR_BLOCK_RAND) { // 블럭별 랜덤 색
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[y]; j++) {
					for (int k = 0; k < cube_blockCount[x]; k++) {
						cube_blocks[i][j][k].MakeBlock(bCountx, bCounty, bCountz, -size_x + cube_blockSize[x] * (float)k * 2.0f + cube_blockSize[x] + midx,
							-size_y + cube_blockSize[y] * (float)j * 2.0f + cube_blockSize[y] + midy,
							-size_z + cube_blockSize[z] * (float)i * 2.0f + cube_blockSize[z] + midz,
							cube_blockSize[x], cube_blockSize[y], cube_blockSize[z], F_urd(dre), F_urd(dre), F_urd(dre));
					}
				}
			}
		}
		else if (type == CUBE_COLOR_BLOCK_SMOOTH) { // 블럭 단위 그라데이션
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[y]; j++) {
					for (int k = 0; k < cube_blockCount[x]; k++) {
						cube_blocks[i][j][k].MakeBlock(bCountx, bCounty, bCountz, -size_x + cube_blockSize[x] * (float)k * 2.0f + cube_blockSize[x] + midx,
							-size_y + cube_blockSize[y] * (float)j * 2.0f + cube_blockSize[y] + midy,
							-size_z + cube_blockSize[z] * (float)i * 2.0f + cube_blockSize[z] + midz,
							cube_blockSize[x], cube_blockSize[y], cube_blockSize[z],
							(-size_x + cube_blockSize[x] * (float)k * 2.0f + cube_blockSize[x] + midx + size_x) / (size_x * 2.0f),
							(-size_y + cube_blockSize[y] * (float)j * 2.0f + cube_blockSize[y] + midy + size_y) / (size_y * 2.0f),
							(-size_z + cube_blockSize[z] * (float)i * 2.0f + cube_blockSize[z] + midz + size_z) / (size_z * 2.0f));
					}
				}
			}
		}
		else if (type == CUBE_COLOR_FIGURE_SMOOTH) { // Figure 단위 그라데이션
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[y]; j++) {
					for (int k = 0; k < cube_blockCount[x]; k++) {
						cube_blocks[i][j][k].MakeBlock(bCountx, bCounty, bCountz, -size_x + cube_blockSize[x] * (float)k * 2.0f + cube_blockSize[x] + midx,
							-size_y + cube_blockSize[y] * (float)j * 2.0f + cube_blockSize[y] + midy,
							-size_z + cube_blockSize[z] * (float)i * 2.0f + cube_blockSize[z] + midz,
							cube_blockSize[x], cube_blockSize[y], cube_blockSize[z], type);
					}
				}
			}
		}
		else if (type == CUBE_COLOR_CUBE_SIDE_DEFAULT) { // 큐브 면 단위 같은 색
			float colorR[6] = { 0 };
			float colorG[6] = { 0 };
			float colorB[6] = { 0 };
			colorR[0] = 1.0f;
			colorR[1] = 1.0f; colorG[1] = 0.5f;
			colorG[2] = 1.0f;
			colorB[3] = 1.0f;
			colorR[4] = 1.0f; colorG[4] = 1.0f;
			colorR[5] = 1.0f; colorG[5] = 1.0f; colorB[5] = 1.0f;

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
		else if (type == CUBE_COLOR_FIGURE_GRAY_SMOOTH) { // Figure 단위 흑백 그라데이션
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[y]; j++) {
					for (int k = 0; k < cube_blockCount[x]; k++) {
						cube_blocks[i][j][k].MakeBlock(bCountx, bCounty, bCountz, -size_x + cube_blockSize[x] * (float)k * 2.0f + cube_blockSize[x] + midx,
							-size_y + cube_blockSize[y] * (float)j * 2.0f + cube_blockSize[y] + midy,
							-size_z + cube_blockSize[z] * (float)i * 2.0f + cube_blockSize[z] + midz,
							cube_blockSize[x], cube_blockSize[y], cube_blockSize[z], type);
					}
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
	void Rotate_Cube(glm::vec3 shaft, float degree) {
		// 원점 기준 회전
		cubeRot = glm::rotate(cubeRot, (GLfloat)glm::radians(degree), shaft); //--- 임의의 축에 대하여 회전
	}
	void Rotate_Specific_Block(int sel_x, int sel_y, int sel_z, const char type, float degree) {
		cube_blocks[sel_z][sel_y][sel_x].Rotate_Block(type, degree);
	}
	void Rotate_Specific_Side(int sel, float degree) {
		// 0  1  2  3  4  5
		// 앞 뒤 왼 오 위 밑
		if (sel == 0) {
			for (int i = 0; i < cube_blockCount[y]; i++) {
				for (int j = 0; j < cube_blockCount[x]; j++) {
					cube_blocks[cube_blockCount[z] - 1][i][j].Rotate_Block('z', degree);
				}
			}
		}
		else if (sel == 1) {
			for (int i = 0; i < cube_blockCount[y]; i++) {
				for (int j = 0; j < cube_blockCount[x]; j++) {
					cube_blocks[0][i][j].Rotate_Block('z', degree);
				}
			}
		}
		else if (sel == 2) {
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[y]; j++) {
					cube_blocks[i][j][0].Rotate_Block('x', degree);
				}
			}
		}
		else if (sel == 3) {
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[y]; j++) {
					cube_blocks[i][j][cube_blockCount[x] - 1].Rotate_Block('x', degree);
				}
			}
		}
		else if (sel == 4) {
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[x]; j++) {
					cube_blocks[i][cube_blockCount[y] - 1][j].Rotate_Block('y', degree);
				}
			}
		}
		else if (sel == 5) {
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[x]; j++) {
					cube_blocks[i][0][j].Rotate_Block('y', degree);
				}
			}
		}
	}
	void Rotate_Specific_Side(int sel, int line, float degree) {
		if (sel == 0) { // z
			for (int i = 0; i < cube_blockCount[y]; i++) {
				for (int j = 0; j < cube_blockCount[x]; j++) {
					cube_blocks[line][i][j].Rotate_Block('z', degree);
				}
			}
		}
		else if (sel == 1) { // y
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[x]; j++) {
					cube_blocks[i][line][j].Rotate_Block('y', degree);
				}
			}
		}
		else if (sel == 2) { // x
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[y]; j++) {
					cube_blocks[i][j][line].Rotate_Block('x', degree);
				}
			}
		}
	}
	bool Rotate_Specific_Side_Check_Rot(int sel, int line, float degree) {
		float tempSize = (float)(cube_blockCount[0] - 1) / 2.0f;
		bool reVal = CUBE_SEQUENCE_ING;
		nowRotDegree += degree;
		if ((int)nowRotDegree == 0) { reVal = CUBE_SEQUENCE_END; }
		if (nowRotDegree >= 90.0f || nowRotDegree <= -90.0f) {
			int* temp;
			if (nowRotDegree >= 90.0f) {
				degree += 90.0f - nowRotDegree;
				Arr_Rotate(sel, line, 0);
			}
			else {
				degree += nowRotDegree + 90.0f;
				Arr_Rotate(sel, line, 1);
			}
			nowRotDegree = 0.0f;
			if (degree > 0) { rotReverseDirQueue.push(new Data(sel, line, -1)); } // 회전 정보 저장
			else { rotReverseDirQueue.push(new Data(sel, line, 1)); }
			reVal = CUBE_SEQUENCE_END;
		}
		if (sel == 0) { // z
			for (int i = 0; i < cube_blockCount[y]; i++) {
				for (int j = 0; j < cube_blockCount[x]; j++) {
					cube_blocks[cube_block_pos[line][i][j][z]][cube_block_pos[line][i][j][y]][cube_block_pos[line][i][j][x]].Rotate_Block('z', degree);
				}
			}
		}
		else if (sel == 1) { // y
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[x]; j++) {
					cube_blocks[cube_block_pos[i][line][j][z]][cube_block_pos[i][line][j][y]][cube_block_pos[i][line][j][x]].Rotate_Block('y', degree);
				}
			}
		}
		else if (sel == 2) { // x
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[y]; j++) {
					cube_blocks[cube_block_pos[i][j][line][z]][cube_block_pos[i][j][line][y]][cube_block_pos[i][j][line][x]].Rotate_Block('x', degree);
				}
			}
		}
		return reVal;
	}
	bool Rotate_Specific_Side_Check_Rot(int sel, int line, bool front, float degree) {
		float tempSize = (float)(cube_blockCount[0] - 1) / 2.0f;
		bool reVal = CUBE_SEQUENCE_ING;
		if (line <= cube_blockCount[0] / 2 && front == false) { degree *= -1; }
		else if (line > cube_blockCount[0] / 2 && front == true) { degree *= -1; }
		nowRotDegree += degree;
		if ((int)nowRotDegree == 0) { reVal = CUBE_SEQUENCE_END; }
		if (nowRotDegree >= 90.0f || nowRotDegree <= -90.0f) {
			int* temp;
			if (nowRotDegree >= 90.0f) {
				degree += 90.0f - nowRotDegree;
				Arr_Rotate(sel, line, 0);
			}
			else {
				degree += nowRotDegree + 90.0f;
				Arr_Rotate(sel, line, 1);
			}
			nowRotDegree = 0.0f;
			if (degree > 0) { rotReverseDirQueue.push(new Data(sel, line, -1)); } // 회전 정보 저장
			else { rotReverseDirQueue.push(new Data(sel, line, 1)); }
			reVal = CUBE_SEQUENCE_END;
		}
		if (sel == 0) { // z
			for (int i = 0; i < cube_blockCount[y]; i++) {
				for (int j = 0; j < cube_blockCount[x]; j++) {
					cube_blocks[cube_block_pos[line][i][j][z]][cube_block_pos[line][i][j][y]][cube_block_pos[line][i][j][x]].Rotate_Block('z', degree);
				}
			}
		}
		else if (sel == 1) { // y
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[x]; j++) {
					cube_blocks[cube_block_pos[i][line][j][z]][cube_block_pos[i][line][j][y]][cube_block_pos[i][line][j][x]].Rotate_Block('y', degree);
				}
			}
		}
		else if (sel == 2) { // x
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[y]; j++) {
					cube_blocks[cube_block_pos[i][j][line][z]][cube_block_pos[i][j][line][y]][cube_block_pos[i][j][line][x]].Rotate_Block('x', degree);
				}
			}
		}
		return reVal;
	}

	void Arr_Rotate(int sel, int line, int rotateDir) {
		if (rotateDir == 0) {
			if (sel == 0) { // z
				Arr_Rotate_Z(line);
			}
			else if (sel == 1) { // y
				for (int i = 0; i < 3; i++) {
					Arr_Rotate_Y_Reverse(line);
				}
			}
			else if (sel == 2) { // x
				Arr_Rotate_X(line);
			}
		}
		else {
			if (sel == 0) { // z
				for (int i = 0; i < 3; i++) {
					Arr_Rotate_Z(line);
				}
			}
			else if (sel == 1) { // y
				Arr_Rotate_Y_Reverse(line);

			}
			else if (sel == 2) { // x
				for (int i = 0; i < 3; i++) {
					Arr_Rotate_X(line);
				}
			}
		}
	}
	void Arr_Rotate_X(int line) {
		int* temp;
		for (int i = 0; i < cube_blockCount[z] / 2; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				temp = cube_block_pos[i][j][line];
				cube_block_pos[i][j][line] = cube_block_pos[cube_blockCount[z] - i - 1][j][line];
				cube_block_pos[cube_blockCount[z] - i - 1][j][line] = temp;
			}
		}
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = i; j < cube_blockCount[y]; j++) {
				temp = cube_block_pos[i][j][line];
				cube_block_pos[i][j][line] = cube_block_pos[j][i][line];
				cube_block_pos[j][i][line] = temp;
			}
		}
	}
	void Arr_Rotate_Y_Reverse(int line) {
		int* temp;
		for (int i = 0; i < cube_blockCount[z] / 2; i++) {
			for (int j = 0; j < cube_blockCount[x]; j++) {
				temp = cube_block_pos[i][line][j];
				cube_block_pos[i][line][j] = cube_block_pos[cube_blockCount[z] - i - 1][line][j];
				cube_block_pos[cube_blockCount[z] - i - 1][line][j] = temp;
			}
		}
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = i; j < cube_blockCount[x]; j++) {
				temp = cube_block_pos[i][line][j];
				cube_block_pos[i][line][j] = cube_block_pos[j][line][i];
				cube_block_pos[j][line][i] = temp;
			}
		}
	}
	void Arr_Rotate_Z(int line) {
		int* temp;
		for (int i = 0; i < cube_blockCount[y] / 2; i++) {
			for (int j = 0; j < cube_blockCount[x]; j++) {
				temp = cube_block_pos[line][i][j];
				cube_block_pos[line][i][j] = cube_block_pos[line][cube_blockCount[y] - i - 1][j];
				cube_block_pos[line][cube_blockCount[y] - i - 1][j] = temp;
			}
		}
		for (int i = 0; i < cube_blockCount[y]; i++) {
			for (int j = i; j < cube_blockCount[x]; j++) {
				temp = cube_block_pos[line][i][j];
				cube_block_pos[line][i][j] = cube_block_pos[line][j][i];
				cube_block_pos[line][j][i] = temp;
			}
		}
	}

	bool Undo_Rotate_Specific_Side(float degree) {
		static bool rotFlag = CUBE_SEQUENCE_END;
		static int select;
		static int line;
		static float rotdegree;

		if (rotFlag == CUBE_SEQUENCE_END) {
			Data* tdata = rotReverseDirQueue.pop();
			if (tdata == NULL) { return CUBE_SEQUENCE_END; }
			select = tdata->sel;
			line = tdata->line;
			rotdegree = (float)tdata->degree * degree;
		}
		rotFlag = SolveRotFuck(select, line, rotdegree);

		return rotFlag;
	}
	bool AutoSolveCube(float degree) {
		static bool rotFlag = CUBE_SEQUENCE_END;
		static int select;
		static int line;
		static float rotdegree;

		if (rotFlag == CUBE_SEQUENCE_END) {
			Data* tdata = rotReverseDirQueue.pop();
			if (tdata == NULL) { return CUBE_SEQUENCE_END; }
			select = tdata->sel;
			line = tdata->line;
			rotdegree = (float)tdata->degree * degree;
		}
		rotFlag = SolveRotFuck(select, line, rotdegree);

		return CUBE_SEQUENCE_ING;
	}
	bool SolveRotFuck(int sel, int line, float degree) {
		float tempSize = (float)(cube_blockCount[0] - 1) / 2.0f;
		bool reVal = CUBE_SEQUENCE_ING;
		nowRotDegree += degree;
		if (nowRotDegree >= 90.0f || nowRotDegree <= -90.0f) {
			int* temp;
			if (nowRotDegree >= 90.0f) {
				degree += 90.0f - nowRotDegree;
				Arr_Rotate(sel, line, 0);
			}
			else {
				degree += nowRotDegree + 90.0f;
				Arr_Rotate(sel, line, 1);
			}
			nowRotDegree = 0.0f;
			reVal = CUBE_SEQUENCE_END;
		}
		if (sel == 0) { // z
			for (int i = 0; i < cube_blockCount[y]; i++) {
				for (int j = 0; j < cube_blockCount[x]; j++) {
					cube_blocks[cube_block_pos[line][i][j][z]][cube_block_pos[line][i][j][y]][cube_block_pos[line][i][j][x]].Rotate_Block('z', degree);
				}
			}
		}
		else if (sel == 1) { // y
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[x]; j++) {
					cube_blocks[cube_block_pos[i][line][j][z]][cube_block_pos[i][line][j][y]][cube_block_pos[i][line][j][x]].Rotate_Block('y', degree);
				}
			}
		}
		else if (sel == 2) { // x
			for (int i = 0; i < cube_blockCount[z]; i++) {
				for (int j = 0; j < cube_blockCount[y]; j++) {
					cube_blocks[cube_block_pos[i][j][line][z]][cube_block_pos[i][j][line][y]][cube_block_pos[i][j][line][x]].Rotate_Block('x', degree);
				}
			}
		}
		return reVal;
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
	GLvoid Draw_Use_CubeMat(unsigned int transformLocation, int printType) {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].Draw(transformLocation, cubeRot, printType);
				}
			}
		}
		gravity.Draw(transformLocation, cubeRot * glm::transpose(cubeRot));
	}
	GLvoid Draw_Use_CubeMat(unsigned int transformLocation, int printType, int val) {
		if (draw_count <= cube_blockCount[x] * cube_blockCount[y] * cube_blockCount[z]) {
			int tx;
			int ty;
			int tz;
			bool flag = false;
			for (int i = 0; i <= draw_count; i++) {
				tx = i % cube_blockCount[x];
				tz = i / cube_blockCount[z] % cube_blockCount[x];
				ty = i / cube_blockCount[z] / cube_blockCount[y] % cube_blockCount[x];
				if (cube_blocks[tz][ty][tx].Draw(transformLocation, cubeRot, printType, val)) { flag = true; }
			}
			if (flag) { draw_count++; }
		}
		else {
			Draw_Use_CubeMat(transformLocation, printType);
		}
	}
	GLvoid Draw_Use_CubeMat(unsigned int transformLocation, int printType, int animationType, int val) {
		if (animationType == CUBE_ANIMATION_DEFALUT) {
			if (draw_count <= cube_blockCount[x] * cube_blockCount[y] * cube_blockCount[z]) {
				int tx;
				int ty;
				int tz;
				bool flag = false;
				for (int i = 0; i <= draw_count; i++) {
					tx = i % cube_blockCount[x];
					tz = i / cube_blockCount[z] % cube_blockCount[x];
					ty = i / cube_blockCount[z] / cube_blockCount[y] % cube_blockCount[x];
					if (cube_blocks[tz][ty][tx].Draw(transformLocation, cubeRot, printType, val)) { flag = true; }
				}
				if (flag) { draw_count++; }
			}
			else {
				Draw_Use_CubeMat(transformLocation, printType);
			}
		}
		else if (animationType == CUBE_ANIMATION_MAZE) {
			if (printType == CUBE_PRINT_ROAD) {
				if (draw_count <= cube_blockCount[x] * cube_blockCount[y] * cube_blockCount[z] * blockCount[x] * blockCount[y] * blockCount[z]) {
					for (int i = 0; i < cube_blockCount[x]; i++) {
						for (int j = 0; j < cube_blockCount[y]; j++) {
							for (int k = 0; k < cube_blockCount[z]; k++) {
								cube_blocks[k][j][i].Draw(transformLocation, cubeRot, printType, animationType, val);
							}
						}
					}
					draw_count += val;
				}
				else {
					Draw_Use_CubeMat(transformLocation, printType);
				}
			}
			else if (printType == CUBE_PRINT_WALL) {
				Draw_Use_CubeMat(transformLocation, printType);
			}
		}
	}
	GLvoid DrawGravityVec(unsigned int transformLocation) {
		gravity.Draw(transformLocation, cubeRot * glm::transpose(cubeRot));
	}
	GLvoid DrawTest(unsigned int transformLocation, Figure* b) {
		float* temp = b->GetMidPos();
		float cubeSize = cube_blockSize[x] * cube_blockCount[x] * 2.0f;
		float figureSize = blockSize[x] * 2.0f;
		glm::vec4 mypos = b->GetTransformMat() * glm::translate(glm::mat4(1.0f), glm::vec3(cubeSize / 2, cubeSize / 2, cubeSize / 2)) * glm::vec4(temp[x], temp[y], temp[z], 1.0f);

		int blockPos[3];
		blockPos[x] = mypos.x / figureSize;
		blockPos[y] = mypos.y / figureSize;
		blockPos[z] = mypos.z / figureSize;

		//std::cout << mypos[x] << ' ' << mypos[y] << ' ' << mypos[z] << '\t';
		//std::cout << blockPos[x] << ' ' << blockPos[y] << ' ' << blockPos[z] << '\n';

		int maxCount = cube_blockCount[x] * blockCount[x];
		int tpos[3];
		tpos[x] = blockPos[x];
		tpos[y] = blockPos[y];
		tpos[z] = blockPos[z];
		if (tpos[x] < 0 || tpos[x] >= maxCount ||
			tpos[y] < 0 || tpos[y] >= maxCount ||
			tpos[z] < 0 || tpos[z] >= maxCount) {
			return;
		}

		int printsize = 1;

		for (int i = -printsize; i <= printsize; i++) {
			for (int j = -printsize; j <= printsize; j++) {
				for (int k = -printsize; k <= printsize; k++) {
					tpos[x] = blockPos[x] + i;
					tpos[y] = blockPos[y] + j;
					tpos[z] = blockPos[z] + k;
					if (tpos[x] < 0 || tpos[x] >= maxCount ||
						tpos[y] < 0 || tpos[y] >= maxCount ||
						tpos[z] < 0 || tpos[z] >= maxCount) {
					}
					else {
						cube_blocks[tpos[z] / blockCount[z]][tpos[y] / blockCount[y]][tpos[x] / blockCount[x]].
							DrawTest(transformLocation, cubeRot, tpos[x] % blockCount[x], tpos[y] % blockCount[y], tpos[z] % blockCount[z]);
					}
				}
			}
		}

		//if (blockPos[x] - 1 >= 0) {
		//	tpos[x] = blockPos[x] - 1;
		//	cube_blocks[blockPos[z] / blockCount[z]][blockPos[y] / blockCount[y]][tpos[x] / blockCount[x]].
		//		DrawTest(transformLocation, cubeRot, tpos[x] % blockCount[x], blockPos[y] % blockCount[y], blockPos[z] % blockCount[z]);
		//}
		//if (blockPos[x] + 1 < maxCount) {
		//	tpos[x] = blockPos[x] + 1;
		//	cube_blocks[blockPos[z] / blockCount[z]][blockPos[y] / blockCount[y]][tpos[x] / blockCount[x]].
		//		DrawTest(transformLocation, cubeRot, tpos[x] % blockCount[x], blockPos[y] % blockCount[y], blockPos[z] % blockCount[z]);
		//}

		//if (blockPos[y] - 1 >= 0) {
		//	tpos[y] = blockPos[y] - 1;
		//	cube_blocks[blockPos[z] / blockCount[z]][tpos[y] / blockCount[y]][blockPos[x] / blockCount[x]].
		//		DrawTest(transformLocation, cubeRot, blockPos[x] % blockCount[x], tpos[y] % blockCount[y], blockPos[z] % blockCount[z]);
		//}
		//if (blockPos[y] + 1 < maxCount) {
		//	tpos[y] = blockPos[y] + 1;
		//	cube_blocks[blockPos[z] / blockCount[z]][tpos[y] / blockCount[y]][blockPos[x] / blockCount[x]].
		//		DrawTest(transformLocation, cubeRot, blockPos[x] % blockCount[x], tpos[y] % blockCount[y], blockPos[z] % blockCount[z]);
		//}

		//if (blockPos[z] - 1 >= 0) {
		//	tpos[z] = blockPos[z] - 1;
		//	cube_blocks[tpos[z] / blockCount[z]][blockPos[y] / blockCount[y]][blockPos[x] / blockCount[x]].
		//		DrawTest(transformLocation, cubeRot, blockPos[x] % blockCount[x], blockPos[y] % blockCount[y], tpos[z] % blockCount[z]);
		//}
		//if (blockPos[z] + 1 < maxCount) {
		//	tpos[z] = blockPos[z] + 1;
		//	cube_blocks[tpos[z] / blockCount[z]][blockPos[y] / blockCount[y]][blockPos[x] / blockCount[x]].
		//		DrawTest(transformLocation, cubeRot, blockPos[x] % blockCount[x], blockPos[y] % blockCount[y], tpos[z] % blockCount[z]);
		//}


	}
	GLvoid DrawAroundWithAlpha(unsigned int transformLocation, unsigned int alphaValueLocation, Figure* b, int range, glm::vec3 cameraPosition, int printType, bool is_print_line) {
		float* temp = b->GetMidPos();
		float cubeSize = cube_blockSize[x] * cube_blockCount[x] * 2.0f;
		float figureSize = blockSize[x] * 2.0f;
		glm::vec4 mypos = b->GetTransformMat() * glm::translate(glm::mat4(1.0f), glm::vec3(cubeSize / 2, cubeSize / 2, cubeSize / 2)) * glm::vec4(temp[x], temp[y], temp[z], 1.0f);

		int blockPos[3];
		blockPos[x] = mypos.x / figureSize;
		blockPos[y] = mypos.y / figureSize;
		blockPos[z] = mypos.z / figureSize;

		int maxCount = cube_blockCount[x] * blockCount[x];
		int tpos[3];
		tpos[x] = blockPos[x];
		tpos[y] = blockPos[y];
		tpos[z] = blockPos[z];
		if (tpos[x] < 0 || tpos[x] >= maxCount ||
			tpos[y] < 0 || tpos[y] >= maxCount ||
			tpos[z] < 0 || tpos[z] >= maxCount) {
			return;
		}

		//Figure** figurelist = new Figure * [(range * 2 + 1) * 3];

		std::map<float, Figure*> sorted;
		std::map<float, int> sot2;

		int counter = 0;
		for (int i = -range; i <= range; i++) {
			for (int j = -range; j <= range; j++) {
				for (int k = -range; k <= range; k++) {
					tpos[x] = blockPos[x] + i;
					tpos[y] = blockPos[y] + j;
					tpos[z] = blockPos[z] + k;
					if (tpos[x] < 0 || tpos[x] >= maxCount ||
						tpos[y] < 0 || tpos[y] >= maxCount ||
						tpos[z] < 0 || tpos[z] >= maxCount) {
					}
					else {
						Figure* t = NULL;
						t = cube_blocks[tpos[z] / blockCount[z]][tpos[y] / blockCount[y]][tpos[x] / blockCount[x]].
							GetSpecificFigure(tpos[x] % blockCount[x], tpos[y] % blockCount[y], tpos[z] % blockCount[z], printType);
						if (t != NULL) {
							float dis = glm::length(cameraPosition - glm::vec3(cubeRot * glm::vec4(t->GetNowMidPosVec(), 1.0f)));
							sorted[dis] = t;
							int tmp[3];
							int val = 0;
							tmp[0] = i;
							tmp[1] = j;
							tmp[2] = k;
							for (int s = 0; s < 3; s++) {
								if (tmp[s] < 0) { tmp[s] *= -1; }
								if (tmp[s] > val) { val = tmp[s]; }
							}
							sot2[dis] = val;
						}
					}
				}
			}
		}
		{
			float dis = glm::length(cameraPosition - glm::vec3(cubeRot * glm::vec4(b->GetNowMidPosVec(), 1.0f)));
			sorted[dis] = b;
			sot2[dis] = -1;
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		myObject.Draw(transformLocation, cubeRot);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		std::map<float, int>::reverse_iterator it2 = sot2.rbegin();
		for (std::map<float, Figure*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it, ++it2) {
			if (it->second == b) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else if (is_print_line) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glUniform1f(alphaValueLocation, 1.0f / (it2->second + 2));
			(it->second)->Draw(transformLocation, cubeRot);
		}

		//delete[] figurelist;
	}

	void InputMaze(int*** maze) {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].InputMaze(maze, k, j, i);
				}
			}
		}
	}

	void SettingCube(int count_x, int count_y, int count_z, int bCountx, int bCounty, int bCountz, float midx, float midy, float midz, float size_x, float size_y, float size_z) {
		if (cube_blockCount[x] != 0 || cube_blockCount[y] != 0 || cube_blockCount[z] != 0) { ClearBlocks(); }
		while (true) { Data* temp = rotReverseDirQueue.pop(); if (temp == NULL) break; delete temp; }
		cube_blockCount[x] = count_x;
		cube_blockCount[y] = count_y;
		cube_blockCount[z] = count_z;
		blockCount[x] = bCountx;
		blockCount[y] = bCounty;
		blockCount[z] = bCountz;

		cube_blocks = new Block * *[cube_blockCount[z]];
		for (int i = 0; i < cube_blockCount[z]; i++) {
			cube_blocks[i] = new Block * [cube_blockCount[y]];
			for (int j = 0; j < cube_blockCount[y]; j++) {
				cube_blocks[i][j] = new Block[cube_blockCount[x]];
			}
		}
		cube_block_pos = new int*** [cube_blockCount[z]];
		for (int i = 0; i < cube_blockCount[z]; i++) {
			cube_block_pos[i] = new int** [cube_blockCount[y]];
			for (int j = 0; j < cube_blockCount[y]; j++) {
				cube_block_pos[i][j] = new int* [cube_blockCount[x]];
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_block_pos[i][j][k] = new int[3];
				}
			}
		}
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_block_pos[i][j][k][x] = k;
					cube_block_pos[i][j][k][y] = j;
					cube_block_pos[i][j][k][z] = i;
				}
			}
		}

		myObject.MakeCube(midx, midy, midz, size_x, size_y, size_z);

		cube_blockSize[x] = size_x / (float)cube_blockCount[x];
		cube_blockSize[y] = size_y / (float)cube_blockCount[y];
		cube_blockSize[z] = size_z / (float)cube_blockCount[z];
		blockSize[x] = cube_blockSize[x] / (float)blockCount[x];
		blockSize[y] = cube_blockSize[y] / (float)blockCount[y];
		blockSize[z] = cube_blockSize[z] / (float)blockCount[z];
		gravity.MakeLine_N(0.0, 0.0, 0.0, 0.0, -1.0, 0.0);
	}

	void PrintBlockPos() {
		for (int i = cube_blockCount[z] - 1; i >= 0; i--) {
			for (int j = cube_blockCount[y] - 1; j >= 0; j--) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					//for (int s = 0; s < 3; s++) {
					//	std::cout << cube_block_pos[i][j][k][s] << ' ';
					//}
					std::cout << cube_block_pos[i][j][k][0] + cube_block_pos[i][j][k][1] * 3 + cube_block_pos[i][j][k][2] * 9;
					std::cout << '\t';
				}
				std::cout << '\n';
			}
			std::cout << '\n';
		}
	}

	void ClearBlocks() {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				delete[] cube_blocks[i][j];
			}
			delete[] cube_blocks[i];
		}
		delete[] cube_blocks;

		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					delete[] cube_block_pos[i][j][k];
				}
				delete[] cube_block_pos[i][j];
			}
			delete[] cube_block_pos[i];
		}
		delete[] cube_block_pos;

		cube_blockCount[x] = 0;
		cube_blockCount[y] = 0;
		cube_blockCount[z] = 0;
		cubeRot = glm::mat4(1.0f);
		nowRotDegree = 0.0f;
		draw_count = 0;
	}
	void Reset() {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					cube_blocks[i][j][k].Reset();
				}
			}
		}
		cubeRot = glm::mat4(1.0f);
		nowRotDegree = 0.0f;
		draw_count = 0;
	}

	bool CrashCheck(Figure* b) {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					if (cube_blocks[i][j][k].CrashCheck(b)) {
						return true;
					}
				}
			}
		}
		return false;
	}
	bool CrashCheck(char type, Figure* b) {
		float* temp = b->GetMidPos();
		float cubeSize = cube_blockSize[x] * cube_blockCount[x] * 2.0f;
		float figureSize = blockSize[x] * 2.0f;
		glm::vec4 mypos = b->GetTransformMat() * glm::translate(glm::mat4(1.0f), glm::vec3(cubeSize / 2, cubeSize / 2, cubeSize / 2)) * glm::vec4(temp[x], temp[y], temp[z], 1.0f);

		int blockPos[3];
		blockPos[x] = mypos.x / figureSize;
		blockPos[y] = mypos.y / figureSize;
		blockPos[z] = mypos.z / figureSize;

		//std::cout << mypos[x] << ' ' << mypos[y] << ' ' << mypos[z] << '\t';
		//std::cout << blockPos[x] << ' ' << blockPos[y] << ' ' << blockPos[z] << '\n';

		int maxCount = cube_blockCount[x] * blockCount[x];
		int tpos[3];
		tpos[x] = blockPos[x];
		tpos[y] = blockPos[y];
		tpos[z] = blockPos[z];
		if (tpos[x] < 0 || tpos[x] >= maxCount ||
			tpos[y] < 0 || tpos[y] >= maxCount ||
			tpos[z] < 0 || tpos[z] >= maxCount) {
			return true;
		}

		bool checkVal = false;

		if (type == CUBE_X) {
			if (blockPos[x] - 1 >= 0 && !checkVal) {
				tpos[x] = blockPos[x] - 1;
				checkVal = cube_blocks[blockPos[z] / blockCount[z]][blockPos[y] / blockCount[y]][tpos[x] / blockCount[x]].
					CrashCheck(b, tpos[x] % blockCount[x], blockPos[y] % blockCount[y], blockPos[z] % blockCount[z]);
			}
			if (blockPos[x] + 1 < maxCount && !checkVal) {
				tpos[x] = blockPos[x] + 1;
				checkVal = cube_blocks[blockPos[z] / blockCount[z]][blockPos[y] / blockCount[y]][tpos[x] / blockCount[x]].
					CrashCheck(b, tpos[x] % blockCount[x], blockPos[y] % blockCount[y], blockPos[z] % blockCount[z]);
			}
		}
		else if (type == CUBE_Y) {
			if (blockPos[y] - 1 >= 0 && !checkVal) {
				tpos[y] = blockPos[y] - 1;
				checkVal = cube_blocks[blockPos[z] / blockCount[z]][tpos[y] / blockCount[y]][blockPos[x] / blockCount[x]].
					CrashCheck(b, blockPos[x] % blockCount[x], tpos[y] % blockCount[y], blockPos[z] % blockCount[z]);
			}
			if (blockPos[y] + 1 < maxCount && !checkVal) {
				tpos[y] = blockPos[y] + 1;
				checkVal = cube_blocks[blockPos[z] / blockCount[z]][tpos[y] / blockCount[y]][blockPos[x] / blockCount[x]].
					CrashCheck(b, blockPos[x] % blockCount[x], tpos[y] % blockCount[y], blockPos[z] % blockCount[z]);
			}
		}
		else if (type == CUBE_Z) {
			if (blockPos[z] - 1 >= 0 && !checkVal) {
				tpos[z] = blockPos[z] - 1;
				checkVal = cube_blocks[tpos[z] / blockCount[z]][blockPos[y] / blockCount[y]][blockPos[x] / blockCount[x]].
					CrashCheck(b, blockPos[x] % blockCount[x], blockPos[y] % blockCount[y], tpos[z] % blockCount[z]);
			}
			if (blockPos[z] + 1 < maxCount && !checkVal) {
				tpos[z] = blockPos[z] + 1;
				checkVal = cube_blocks[tpos[z] / blockCount[z]][blockPos[y] / blockCount[y]][blockPos[x] / blockCount[x]].
					CrashCheck(b, blockPos[x] % blockCount[x], blockPos[y] % blockCount[y], tpos[z] % blockCount[z]);
			}

		}
		return checkVal;
	}

	bool CheckCubeBlocksLocation() {
		for (int i = 0; i < cube_blockCount[z]; i++) {
			for (int j = 0; j < cube_blockCount[y]; j++) {
				for (int k = 0; k < cube_blockCount[x]; k++) {
					if (
						cube_block_pos[i][j][k][x] != k ||
						cube_block_pos[i][j][k][y] != j ||
						cube_block_pos[i][j][k][z] != i
						) {
						return false;
					}
				}
			}
		}
		return true;
	}

	glm::mat4 get_cubeRot() {
		return cubeRot;
	}
	glm::vec4 get_gravityVec() {
		return glm::transpose(cubeRot) * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
	}
	glm::vec4* get_plainNormal() {
		glm::vec4* tnormal = new glm::vec4[6];
		tnormal[0] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		tnormal[1] = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
		tnormal[2] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		tnormal[3] = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
		tnormal[4] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		tnormal[5] = glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f);
		for (int i = 0; i < 6; i++) {
			tnormal[i] = cubeRot * tnormal[i];
		}
		return tnormal;
	}
	bool get_drawCountEnd(int cube_mazeSelecter) {
		if (cube_mazeSelecter == 0) {
			return draw_count > (cube_blockCount[x] * cube_blockCount[y] * cube_blockCount[z] * blockCount[x] * blockCount[y] * blockCount[z]) / 4;
		}
		else if (cube_mazeSelecter == 1) {
			return draw_count > (cube_blockCount[x] * cube_blockCount[y] * cube_blockCount[z] * blockCount[x] * blockCount[y] * blockCount[z]) / 2.5f;
		}
		else {
			return draw_count > (cube_blockCount[x] * cube_blockCount[y] * cube_blockCount[z] * blockCount[x] * blockCount[y] * blockCount[z]) / 4;
		}
	}

	~Cube() {
		ClearBlocks();
	}
};



class Maze_BY_ANDY {
	int*** MazeWall; // 미로 벽 활성화 (-1 벽 else 길)

	int Count; // x,y,z 개수
	const int x = 0, y = 1, z = 2;
	std::uniform_int_distribution<> uid;

public:
	Maze_BY_ANDY() {
		MazeWall = NULL;
		Count = 0;
	}
	void MakeFild(int size) {

		//MazeWall[0][0] = false;
		//MazeWall[CountH - 1][CountW - 1] = false;
	}

	bool CheckRoad(int x, int y, int z) { // x,y,z 위치 상하좌우에 뚫려있는 위치가 1개일때 true
		int tempCount = 0;
		if (x - 1 > 0 && MazeWall[z][y][x - 1] != -1) { tempCount += 1; }
		if (x + 1 < (Count - 1) && MazeWall[z][y][x + 1] != -1) { tempCount += 1; }
		if (y - 1 > 0 && MazeWall[z][y - 1][x] != -1) { tempCount += 1; }
		if (y + 1 < (Count - 1) && MazeWall[z][y + 1][x] != -1) { tempCount += 1; }
		if (z - 1 > 0 && MazeWall[z - 1][y][x] != -1) { tempCount += 1; }
		if (z + 1 < (Count - 1) && MazeWall[z + 1][y][x] != -1) { tempCount += 1; }

		if (tempCount == 1) { return true; }
		else { return false; }
	}
	int MakeRoad(int x, int y, int z, int counter) {
		int RoadArr[6];
		int RoadCount = 0;
		for (int i = 0; i < Count / 3; i++) {
			RoadCount = 0;
			if (x - 1 > 0 && MazeWall[z][y][x - 1] == -1 && CheckRoad(x - 1, y, z)) { RoadArr[RoadCount++] = 1; } // 좌
			if (x + 1 < (Count - 1) && MazeWall[z][y][x + 1] == -1 && CheckRoad(x + 1, y, z)) { RoadArr[RoadCount++] = 2; } // 우
			if (y - 1 > 0 && MazeWall[z][y - 1][x] == -1 && CheckRoad(x, y - 1, z)) { RoadArr[RoadCount++] = 3; } // 하
			if (y + 1 < (Count - 1) && MazeWall[z][y + 1][x] == -1 && CheckRoad(x, y + 1, z)) { RoadArr[RoadCount++] = 4; } // 상
			if (z - 1 > 0 && MazeWall[z - 1][y][x] == -1 && CheckRoad(x, y, z - 1)) { RoadArr[RoadCount++] = 5; } // 전
			if (z + 1 < (Count - 1) && MazeWall[z + 1][y][x] == -1 && CheckRoad(x, y, z + 1)) { RoadArr[RoadCount++] = 6; } // 후

			if (RoadCount == 0) { return counter; }

			int temp = RoadArr[(uid(dre) % RoadCount)];
			if (temp == 1) { MazeWall[z][y][--x] = counter++; }
			else if (temp == 2) { MazeWall[z][y][++x] = counter++; }
			else if (temp == 3) { MazeWall[z][--y][x] = counter++; }
			else if (temp == 4) { MazeWall[z][++y][x] = counter++; }
			else if (temp == 5) { MazeWall[--z][y][x] = counter++; }
			else if (temp == 6) { MazeWall[++z][y][x] = counter++; }
		}
		return counter;
	}
	int*** MakeMaze(int size) {
		if (size == 0) { return NULL; }
		ResetMaze(size);

		MazeWall[1][1][1] = 0;

		int orderCounter = 1;
		orderCounter = MakeRoad(1, 1, 1, orderCounter);

		for (int i = 0; i < Count * Count * Count; i++) {
			int tempX, tempY, tempZ;
			do {
				tempX = uid(dre) % Count;
				tempY = uid(dre) % Count;
				tempZ = uid(dre) % Count;
			} while (MazeWall[tempZ][tempY][tempX] == -1);
			orderCounter = MakeRoad(tempX, tempY, tempZ, orderCounter);
		}

		//MazeWall[Count - 1][Count - 1] = false;

		if (MazeWall[Count - 2][Count - 3][Count - 2] == -1 && MazeWall[Count - 2][Count - 2][Count - 3] == -1 && MazeWall[Count - 3][Count - 2][Count - 2] == -1) { MazeWall[Count - 2][Count - 3][Count - 2] = 0; }

		return MazeWall;
	}
	void ResetMaze(int size) {
		if (Count != 0) {
			for (int i = 0; i < Count; i++) {
				for (int j = 0; j < Count; j++) {
					delete[] MazeWall[i][j];
				}
			}
			delete[] MazeWall;
		}

		Count = size;

		MazeWall = new int** [Count];
		for (int i = 0; i < Count; i++) {
			MazeWall[i] = new int* [Count];
			for (int j = 0; j < Count; j++) {
				MazeWall[i][j] = new int[Count];
			}
		}
		for (int i = 0; i < Count; i++) {
			for (int j = 0; j < Count; j++) {
				for (int k = 0; k < Count; k++) {
					MazeWall[i][j][k] = -1;
				}
			}
		}
	}

	~Maze_BY_ANDY() {
		if (Count != 0) {
			for (int i = 0; i < Count; i++) {
				for (int j = 0; j < Count; j++) {
					delete[] MazeWall[i][j];
				}
			}
			delete[] MazeWall;
		}
	}
};
