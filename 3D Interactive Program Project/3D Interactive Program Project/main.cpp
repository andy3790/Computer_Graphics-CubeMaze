#include "includer.h"
#include "shader_program.h"
#include "Figure.h"
#include "util.h"
#include"make_maze.h"
#define WIN_WIDTH 800	// 윈도우 가로 크기
#define WIN_HIGHT 800	// 윈도우 세로 크기

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> F_urd(0.1, 1.0);	// Figure class를 위한 랜덤 생성자

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Keyboard_up(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLvoid Special(int key, int x, int y);
GLvoid Special_up(int key, int x, int y);
GLvoid Timer(int value);

GLuint s_program;
GLuint vertexShader;
GLuint fragmentShader;

glm::vec3 cameraStartPos;
glm::vec3 cameraStartDir;
glm::vec3 cameraPos; //--- 카메라 위치
glm::vec3 cameraDirection; //--- 카메라 바라보는 방향
glm::vec3 cameraUp; //--- 카메라 위쪽 방향

glm::vec4 lightPos;
glm::mat4 lightT;
glm::mat4 lightR;
bool lightOn;

int window1; // use destroy

Figure line;
Figure flashlight;
Cube test2;

glm::mat4 cameraRot;

extern bool*** maze;
extern int maze_size;
int t;
int drawType;
bool cube_rotate_flag;
bool suffle_Flag;

bool is_left_butten_up;
bool is_right_butten_up;
int mouse_x;
int mouse_y;
float max_rotation_magnification; // 화면 끝에서 끝으로 갈 때 돌아가는 각의 크기

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(WIN_WIDTH, WIN_HIGHT); // 윈도우의 크기 지정
	window1 = glutCreateWindow("Shader Programing"); // 윈도우 생성 (윈도우 이름)
		//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnable(GL_BLEND);

	InitShader();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	cameraStartPos = glm::vec3(5.0f, 5.0f, 5.0f);
	cameraStartDir = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraPos = cameraStartPos; //--- 카메라 위치
	cameraDirection = cameraStartDir; //--- 카메라 바라보는 방향
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

	lightT = glm::mat4(1.0f);
	lightR = glm::mat4(1.0f);
	lightT = glm::translate(lightT, glm::vec3(0.0f, 5.0f, -10.0f));
	lightPos = glm::vec4(0.0, 0.0, 0.0, 1.0);
	lightOn = true;

	line.MakeLine(5.0f);
	flashlight.MakeCube(0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0);
	test2.MakeCube(1, 1, 1, 1, 1, 1, 0.0, 0.0, 0.0, 3.0, 3.0, 3.0, CUBE_COLOR_POINT_RAND);

	cameraRot = glm::mat4(1.0f);
	t = -1;
	drawType = 0;
	cube_rotate_flag = true;
	suffle_Flag = false;

	is_left_butten_up = true;
	is_right_butten_up = true;
	mouse_x = 0;	// 상관없음
	mouse_y = 0;	// 상관없음
	max_rotation_magnification = 180.0f; // 화면 끝에서 끝으로 갈 때 돌아가는 각의 크기

	glutTimerFunc(10, Timer, 1);

	glutDisplayFunc(drawScene); // 출력 콜백함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 콜백함수 지정
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백함수 지정
	glutKeyboardUpFunc(Keyboard_up); // 키보드 입력 콜백함수 지정
	glutSpecialFunc(Special); // 특수 키 입력 콜백함수 지정
	glutSpecialUpFunc(Special_up); // 특수 키 입력 콜백함수 지정
	glutMouseFunc(Mouse); // 마우스 입력 콜백함수 지정
	glutMotionFunc(Motion);
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	//--- 변환 행렬 값을 버텍스 세이더로 보내기
	unsigned int transformLocation = glGetUniformLocation(s_program, "model");
	unsigned int viewLocation = glGetUniformLocation(s_program, "view");
	unsigned int projectionLocation = glGetUniformLocation(s_program, "projection");
	unsigned int colorLocation = glGetUniformLocation(s_program, "model_color");
	glUniform3f(colorLocation, -1.0, -1.0, -1.0);

	unsigned int lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	unsigned int lightColorLocation = glGetUniformLocation(s_program, "lightColor");
	unsigned int lightOnLocation = glGetUniformLocation(s_program, "lightOn");
	unsigned int viewPosLocation = glGetUniformLocation(s_program, "viewPos");
	glm::vec4 tlight = lightR * lightT * lightPos;
	glUniform3f(lightPosLocation, tlight.x, tlight.y, tlight.z);
	glUniform1i(lightOnLocation, lightOn);
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projMat = glm::mat4(1.0f);
	view =glm::lookAt(cameraPos, cameraDirection, cameraUp) * cameraRot;

	projMat = glm::perspective((GLfloat)glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HIGHT, 0.1f, 50.0f);
	projMat = glm::translate(projMat, glm::vec3(0.0, 0.0, -5.0));


	{
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projMat));


		line.Draw(transformLocation);
		//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(line.GetTransformMat()));
		//cube.Draw(transformLocation);

		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(lightR * lightT * glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2))));
		//flashlight.Draw();

		test2.Draw_Use_CubeMat(transformLocation, drawType);
	}

	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':case '2':case '3':case '4':case '5':case '6': if (cube_rotate_flag) { t = key - '0'; cube_rotate_flag = !cube_rotate_flag; } break;
	case 'c': case 'C': t = 7; break;
	case 'v': case 'V': t = -1; break;
	case '`': suffle_Flag = true; break;

	case 'x': cameraRot = glm::rotate(cameraRot, (GLfloat)glm::radians(1.0f), glm::vec3(1.0, 0.0, 0.0)); break;
	case 'X': cameraRot = glm::rotate(cameraRot, (GLfloat)glm::radians(-1.0f), glm::vec3(1.0, 0.0, 0.0)); break;
	case 'y': cameraRot = glm::rotate(cameraRot, (GLfloat)glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0)); break;
	case 'Y': cameraRot = glm::rotate(cameraRot, (GLfloat)glm::radians(-1.0f), glm::vec3(0.0, 1.0, 0.0)); break;
	case 'i': case 'I': glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
	case 'o': case 'O': glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
	case 'p': case 'P': drawType = (drawType + 1) % 2; break;
	case 'g': case 'G': get_size_of_maze(30, 1); test2.MakeCube(3, 3, 3, maze_size / 3, maze_size / 3, maze_size / 3, 0.0, 0.0, 0.0, 3.0, 3.0, 3.0, CUBE_COLOR_CUBE_SIDE_DEFAULT); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break; // 미로 크기 재설정
	case 'm': case 'M': make_maze_wilson(); print_maze(); test2.InputMaze(maze); break; // 미로 재생성
	case 'q': case 'Q': glutDestroyWindow(window1); break; // 프로그램 종료
	}
	glutPostRedisplay(); //--- 배경색이 바뀔때마다 출력 콜백함수를 호출하여 화면을 refresh 한다
}
GLvoid Keyboard_up(unsigned char key, int x, int y)
{
	switch (key) {

	}
}
GLvoid Special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1: cameraStartPos = glm::vec3(5.0f, 5.0f, 5.0f); cameraPos = cameraStartPos;  cameraRot = glm::mat4(1.0f); break;
	//case GLUT_KEY_RIGHT: cameraStartPos = glm::vec3(5.0f * sqrt(3), 0.0f, 0.0f); cameraPos = cameraStartPos; cameraRot = glm::mat4(1.0f); break;
	//case GLUT_KEY_LEFT: cameraStartPos = glm::vec3(-5.0f * sqrt(3), 0.0f, 0.0f); cameraPos = cameraStartPos; cameraRot = glm::mat4(1.0f); break;
	//case GLUT_KEY_UP: cameraStartPos = glm::vec3(0.0f, 5.0f * sqrt(3), 0.0f); cameraPos = cameraStartPos; cameraRot = glm::mat4(1.0f); break;
	//case GLUT_KEY_DOWN: cameraStartPos = glm::vec3(0.0f, -5.0f * sqrt(3), 0.0f); cameraPos = cameraStartPos; cameraRot = glm::mat4(1.0f); break;
	}
}
GLvoid Special_up(int key, int x, int y)
{
	switch (key) {

	}
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			is_left_butten_up = false;
			mouse_x = x;
			mouse_y = y;
		}
		else if (state == GLUT_UP)
		{
			is_left_butten_up = true;
		}
	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			is_right_butten_up = false;
			mouse_x = x;
			mouse_y = y;
		}
		else if (state == GLUT_UP)
		{
			is_right_butten_up = true;
		}
	}
}

GLvoid Motion(int x, int y)
{
	if (!is_left_butten_up || !is_right_butten_up)
	{
		// 마우스 조작으로 인해 회전해야할 각
		float x_angle = (float)(x - mouse_x) / WIN_WIDTH * max_rotation_magnification;
		float y_angle = (float)(y - mouse_y) / WIN_HIGHT * max_rotation_magnification;

		glm::vec3 axis_vec3;
		glm::vec4 axis_vec4;
		glm::mat4 TM_of_axis_of_rotation = cameraRot;

		// 세로 회전

		// 회전축 지정
		//if (x < WIN_WIDTH / 3)			axis_vec4 = glm::vec4(cameraStartPos[2], 0.0, cameraStartPos[0], sqrt(pow(cameraStartPos[0], 2) + pow(cameraStartPos[2], 2)));
		/*else if (x < WIN_WIDTH / 3 * 2) */axis_vec4 = glm::vec4(cameraStartPos[2], 0.0, -cameraStartPos[0], sqrt(pow(cameraStartPos[0], 2) + pow(cameraStartPos[2], 2)));
		//else							axis_vec4 = glm::vec4(-cameraStartPos[2], 0.0, -cameraStartPos[0], sqrt(pow(cameraStartPos[0], 2) + pow(cameraStartPos[2], 2)));

		// 카메라와 객체의 회전 상태를 적용하기위해 각자의 회전행렬을 곱한다.
		axis_vec4 = axis_vec4 * cameraRot;
		if(!is_right_butten_up)
			axis_vec4 = axis_vec4 * test2.get_cubeRot();
		axis_vec3 = glm::vec3(axis_vec4);
		// 마우스 조작 크기만큼 회전
		if (!is_left_butten_up)
			cameraRot = glm::rotate(cameraRot, (GLfloat)glm::radians(y_angle), axis_vec3);
		if(!is_right_butten_up)
			test2.Rotate_Cube(axis_vec3, y_angle);


		// 가로 회전

		// 회전축 지정
		float tmp_vec_x = -cameraStartPos[0];
		float tmp_vec_y = (pow(cameraStartPos[0], 2) + pow(cameraStartPos[2], 2)) / cameraStartPos[1];
		float tmp_vec_z = -cameraStartPos[2];
		axis_vec4 = glm::vec4(tmp_vec_x, tmp_vec_y, tmp_vec_z, sqrt(pow(tmp_vec_x, 2) + pow(tmp_vec_y, 2) + pow(tmp_vec_z, 2)));
		// 카메라와 객체의 회전 상태를 적용하기위해 각자의 회전행렬을 곱한다.
		axis_vec4 = axis_vec4 * cameraRot;
		if (!is_right_butten_up)
			axis_vec4 = axis_vec4 * test2.get_cubeRot();
		axis_vec3 = glm::vec3(axis_vec4);
		// 마우스 조작 크기만큼 회전
		if (!is_left_butten_up)
			cameraRot = glm::rotate(cameraRot, (GLfloat)glm::radians(x_angle), axis_vec3);
		if (!is_right_butten_up)
			test2.Rotate_Cube(axis_vec3, x_angle);

		mouse_x = x;
		mouse_y = y;
	}
}

GLvoid Timer(int value)
{
	// 광원 회전 코드
	lightR = glm::rotate(lightR, (GLfloat)glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));

	//test2.Rotate_Cube(CUBE_Y, -1.0f);
	if (t == 7) {
		//test2.Rotate_Specific_Side(0, 1.0f);
		//test2.Rotate_Specific_Side(1, 1.0f);
		//test2.Rotate_Specific_Side(2, 1.0f);
		//test2.Rotate_Specific_Side(3, 1.0f);
		//test2.Rotate_Specific_Side(4, 1.0f);
		//test2.Rotate_Specific_Side(5, 1.0f);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (j % 2 != 0) {
					test2.Rotate_Specific_Side_Check_Rot(i, j, 1.0f);
				}
				else {
					test2.Rotate_Specific_Side_Check_Rot(i, j, -1.0f);
				}
			}
		}
	}
	else if (t == -1) {
	}
	else {
		if (cube_rotate_flag = test2.Rotate_Specific_Side_Check_Rot((t - 1) / 2, (t - 1) % 2 + (t - 1) % 2, 1.0f)) {
			t = -1;
		}
	}

	if (suffle_Flag) {
		suffle_Flag = Shuffle_Cube(&test2, 3);
	}



	glutPostRedisplay();
	glutTimerFunc(10, Timer, 1);
}