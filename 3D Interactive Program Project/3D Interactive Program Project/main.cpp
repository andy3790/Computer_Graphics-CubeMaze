#include "includer.h"
#include "shader_program.h"
#include "Figure.h"
#include "util.h"
#include"make_maze.h"
#define WIN_WIDTH 800	// 윈도우 가로 크기
#define WIN_HIGHT 800	// 윈도우 세로 크기
#define MAX_ROTATION_MAGNIFICATION 180.0f // 화면 끝에서 끝으로 갈 때 돌아가는 각의 크기
#define MAX_TRANSLATE_DISTANCE 10.0f // 화면 끝에서 끝으로 갈 때 움직이는 카메라의 거리

// 미로 생성 알고리즘 시각화하기
// 미로 생성 알고리즘 추가 (박기정)
// 캐릭터 이동 추가
// x,y,z 좌표를 받으면 어떤 Cube의 Figure 안에 있는지 리턴해주는 함수 필요
// 키 정리


// 상위객체_자신객체명_기능_자료형
// 변수 - _로 구분	기본 소문자 & 두 단어가 합쳐진 단어는 대문자로 구분
// 함수 대소문자로 구분
// u = unsinged
// i = int
// f = float
// is_ = bool
// v() = vec()
// m() = mat()

extern int*** maze_route_i;
extern int maze_size;

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> F_urd(0.1, 1.0);	// Figure class를 위한 랜덤 생성자

GLvoid DrawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLvoid MouseWheel(int button, int dir, int x, int y);
GLvoid Special(int key, int x, int y);
GLvoid SpecialUp(int key, int x, int y);
GLvoid Timer(int value);

GLuint s_program;
GLuint vertexShader;
GLuint fragmentShader;

glm::vec3 camera_startPos_v3; // 카메라 초기 위치
glm::vec3 camera_startDir_v3; // 카메라 초기 바라보는 방향
glm::vec3 camera_pos_v3; //--- 카메라 현재 위치
glm::vec3 camera_direction_v3; //--- 카메라 현재 바라보는 방향
glm::vec3 camera_up_v3; //--- 카메라 위쪽 방향
glm::mat4 camera_rot_m4; // 카메라 회전 행렬
glm::mat4 camera_trans_m4; // 카메라 이동 행렬

glm::vec4 light_pos_v4; // 빛의 위치
glm::mat4 light_trans_m4; // 빛의 이동행렬
glm::mat4 light_rot_m4; // 빛의 회전행렬
bool is_light_on; // 빛 활성화/비활성화

bool is_texture_on; // 텍스쳐 활성화/비활성화

void SetObject(int size);

Cube Cube_mainObject; // 메인오브젝트
Figure Figure_player;
Maze_BY_ANDY Maze_BY_ANDY_maze;

int cube_sideSelecter_i; // 큐브 면 선택 변수
int cube_drawType_i; // 큐브 길 출력 or 벽 출력
int cube_colorType_i; // 큐브 생성 색 타입
int cube_mazeSelecter_i; // 큐브 미로 생성 알고리즘 선택 변수 0=윌슨 1=크루스컬
bool is_cube_canRotate; // 큐브 면 회전용 flag
bool is_cube_suffle; // 큐브 섞기 flag
bool is_print_line; // 선출력 / 면출력
bool is_cube_exist; // 시작 플래그
bool is_cube_autoSolve; // 큐브 자동 풀기 플래그
bool is_cube_undoRot; // 큐브 언두 플래그
bool is_cube_correctOrder; // 큐브가 옳은 모양인가
float cube_rotVal_f; // 큐브 회전 각도

bool is_leftButten_up;
bool is_right_butten_up;
bool is_middleButten_up;
int mouse_x_i;
int mouse_y_i;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(WIN_WIDTH, WIN_HIGHT); // 윈도우의 크기 지정
	glutCreateWindow("Shader Programing"); // 윈도우 생성 (윈도우 이름)
		//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	InitShader();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	camera_startPos_v3 = glm::vec3(5.0f, 5.0f, 5.0f);
	camera_startDir_v3 = glm::vec3(0.0f, 0.0f, 0.0f);
	camera_pos_v3 = camera_startPos_v3; //--- 카메라 위치
	camera_direction_v3 = camera_startDir_v3; //--- 카메라 바라보는 방향
	camera_up_v3 = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	camera_rot_m4 = glm::mat4(1.0f);
	camera_trans_m4 = glm::mat4(1.0f);

	light_trans_m4 = glm::mat4(1.0f);
	light_rot_m4 = glm::mat4(1.0f);
	light_trans_m4 = glm::translate(light_trans_m4, glm::vec3(0.0f, 5.0f, -10.0f));
	light_pos_v4 = glm::vec4(0.0, 0.0, 0.0, 1.0);
	is_light_on = true;

	is_texture_on = false;

	Cube_mainObject.MakeCube(1, 1, 1, 1, 1, 1, 0.0, 0.0, 0.0, 3.0, 3.0, 3.0, CUBE_COLOR_POINT_RAND);

	cube_sideSelecter_i = -1;
	cube_drawType_i = 0;
	cube_colorType_i = 7;
	cube_mazeSelecter_i = 0;
	is_cube_canRotate = true;
	is_cube_suffle = CUBE_SEQUENCE_END;
	is_print_line = true;
	is_cube_exist = false;
	is_cube_autoSolve = CUBE_SEQUENCE_END;
	is_cube_undoRot = CUBE_SEQUENCE_END;
	is_cube_correctOrder = true;
	cube_rotVal_f = 1.0f;

	is_leftButten_up = true;
	is_right_butten_up = true;
	is_middleButten_up = true;
	mouse_x_i = 0;	// 상관없음
	mouse_y_i = 0;	// 상관없음

	glutTimerFunc(10, Timer, 1);

	glutDisplayFunc(DrawScene); // 출력 콜백함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 콜백함수 지정
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백함수 지정
	glutKeyboardUpFunc(KeyboardUp); // 키보드 입력 콜백함수 지정
	glutSpecialFunc(Special); // 특수 키 입력 콜백함수 지정
	glutSpecialUpFunc(SpecialUp); // 특수 키 입력 콜백함수 지정
	glutMouseFunc(Mouse); // 마우스 입력 콜백함수 지정
	glutMouseWheelFunc(MouseWheel); // 마우스 입력 콜백함수 지정
	glutMotionFunc(Motion);
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid DrawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	//--- 변환 행렬 값을 버텍스 세이더로 보내기
	unsigned int transformLocation_ui = glGetUniformLocation(s_program, "model");
	unsigned int viewLocation_ui = glGetUniformLocation(s_program, "view");
	unsigned int projectionLocation_ui = glGetUniformLocation(s_program, "projection");
	unsigned int colorLocation_ui = glGetUniformLocation(s_program, "model_color");
	glUniform3f(colorLocation_ui, -1.0, -1.0, -1.0);

	unsigned int lightPosLocation_ui = glGetUniformLocation(s_program, "lightPos");
	unsigned int lightColorLocation_ui = glGetUniformLocation(s_program, "lightColor");
	unsigned int lightOnLocation_ui = glGetUniformLocation(s_program, "lightOn");
	unsigned int viewPosLocation_ui = glGetUniformLocation(s_program, "viewPos");

	glm::vec4 current_light_pos_v4 = light_rot_m4 * light_trans_m4 * light_pos_v4;
	glUniform3f(lightPosLocation_ui, current_light_pos_v4.x, current_light_pos_v4.y, current_light_pos_v4.z);
	glUniform1i(lightOnLocation_ui, is_light_on);
	glUniform3f(lightColorLocation_ui, 1.0, 1.0, 1.0);
	glUniform3f(viewPosLocation_ui, camera_pos_v3.x, camera_pos_v3.y, camera_pos_v3.z);

	unsigned int textureOnLocation = glGetUniformLocation(s_program, "textureOn");
	glUniform1i(textureOnLocation, is_texture_on);

	glm::mat4 viewMat_m4 = glm::mat4(1.0f);
	glm::mat4 projMat_m4 = glm::mat4(1.0f);
	viewMat_m4 =glm::lookAt(camera_pos_v3, camera_direction_v3, camera_up_v3) * camera_trans_m4 * camera_rot_m4;

	projMat_m4 = glm::perspective((GLfloat)glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HIGHT, 0.1f, 50.0f);
	projMat_m4 = glm::translate(projMat_m4, glm::vec3(0.0, 0.0, -5.0));

	if (is_print_line) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

	{
		glUniformMatrix4fv(viewLocation_ui, 1, GL_FALSE, &viewMat_m4[0][0]);
		glUniformMatrix4fv(projectionLocation_ui, 1, GL_FALSE, glm::value_ptr(projMat_m4));


		Cube_mainObject.Draw_Use_CubeMat(transformLocation_ui, cube_drawType_i, CUBE_ANIMATION_MAZE, 10);
		//Cube_mainObject.DrawTest(transformLocation_ui, &Figure_player);

		if (is_cube_exist && is_cube_correctOrder) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUniformMatrix4fv(transformLocation_ui, 1, GL_FALSE, glm::value_ptr(Cube_mainObject.get_cubeRot() * Figure_player.GetTransformMat()));
			Figure_player.Draw();
		}
	}

	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	glm::vec3 axis_v3;
	glm::vec4 axis_v4;
	static float tmp_vec_x_f;
	static float tmp_vec_y_f;
	static float tmp_vec_z_f;
	switch (key) { // 언제든 입력 받을 수 있는 키 모음
	case '1':case '2':case '3':case '4':case '5':case '6': case '7': case '8': case '0': cube_colorType_i = key - '0'; break;

	case ']': cube_mazeSelecter_i = !cube_mazeSelecter_i; break;
	case 'x': camera_rot_m4 = glm::rotate(camera_rot_m4, (GLfloat)glm::radians(1.0f), glm::vec3(1.0, 0.0, 0.0)); break;
	case 'X': camera_rot_m4 = glm::rotate(camera_rot_m4, (GLfloat)glm::radians(-1.0f), glm::vec3(1.0, 0.0, 0.0)); break;
	case 'y':
		tmp_vec_x_f = -camera_startPos_v3[0];
		tmp_vec_y_f = (pow(camera_startPos_v3[0], 2) + pow(camera_startPos_v3[2], 2)) / camera_startPos_v3[1];
		tmp_vec_z_f = -camera_startPos_v3[2];
		axis_v4 = glm::vec4(tmp_vec_x_f, tmp_vec_y_f, tmp_vec_z_f, sqrt(pow(tmp_vec_x_f, 2) + pow(tmp_vec_y_f, 2) + pow(tmp_vec_z_f, 2)));
		axis_v4 = axis_v4 * camera_rot_m4;
		axis_v3 = glm::vec3(axis_v4);
		camera_rot_m4 = glm::rotate(camera_rot_m4, (GLfloat)glm::radians(1.0f), axis_v3);
		break;
	case 'Y':
		tmp_vec_x_f = -camera_startPos_v3[0];
		tmp_vec_y_f = (pow(camera_startPos_v3[0], 2) + pow(camera_startPos_v3[2], 2)) / camera_startPos_v3[1];
		tmp_vec_z_f = -camera_startPos_v3[2];
		axis_v4 = glm::vec4(tmp_vec_x_f, tmp_vec_y_f, tmp_vec_z_f, sqrt(pow(tmp_vec_x_f, 2) + pow(tmp_vec_y_f, 2) + pow(tmp_vec_z_f, 2)));
		axis_v4 = axis_v4 * camera_rot_m4;
		axis_v3 = glm::vec3(axis_v4);
		camera_rot_m4 = glm::rotate(camera_rot_m4, (GLfloat)glm::radians(-1.0f), axis_v3);
		break;
	case 'i': case 'I': is_print_line = true; break;
	case 'o': case 'O': is_print_line = false; break;
	case 'p': case 'P': cube_drawType_i = (cube_drawType_i + 1) % 2; break;
	case 'g': case 'G': get_size_of_maze(30, 1); Cube_mainObject.MakeCube(3, 3, 3, maze_size / 3, maze_size / 3, maze_size / 3, 0.0, 0.0, 0.0, 3.0, 3.0, 3.0, cube_colorType_i); is_print_line = false; is_cube_exist = true; break; // 미로 크기 재설정
	case 'm': case 'M': if (make_maze_wilson()) {/* print_maze();*/ Cube_mainObject.InputMaze(maze_route_i); } break; // 미로 재생성
	case VK_BACK: is_cube_undoRot = CUBE_SEQUENCE_ING; break;
	case 27: 
		if (is_cube_exist) { // 초기화면으로
			Cube_mainObject.MakeCube(1, 1, 1, 1, 1, 1, 0.0, 0.0, 0.0, 3.0, 3.0, 3.0, CUBE_COLOR_POINT_RAND);
			is_print_line = true; is_cube_exist = false; is_cube_suffle = false;
		}
		else { exit(0); }
		break;
	}
	if (is_cube_exist) { // 시작 후에만 입력 받을 수 있는 키 모음
		switch (key) {
		case 'q': if (is_cube_canRotate) { cube_sideSelecter_i = 2; is_cube_canRotate = !is_cube_canRotate; cube_rotVal_f = +1.0f; } break;
		case 'Q': if (is_cube_canRotate) { cube_sideSelecter_i = 2; is_cube_canRotate = !is_cube_canRotate; cube_rotVal_f = -1.0f; } break;
		case 'a': if (is_cube_canRotate) { cube_sideSelecter_i = 1; is_cube_canRotate = !is_cube_canRotate; cube_rotVal_f = +1.0f; } break;
		case 'A': if (is_cube_canRotate) { cube_sideSelecter_i = 1; is_cube_canRotate = !is_cube_canRotate; cube_rotVal_f = -1.0f; } break;
		case 'w': if (is_cube_canRotate) { cube_sideSelecter_i = 4; is_cube_canRotate = !is_cube_canRotate; cube_rotVal_f = +1.0f; } break;
		case 'W': if (is_cube_canRotate) { cube_sideSelecter_i = 4; is_cube_canRotate = !is_cube_canRotate; cube_rotVal_f = -1.0f; } break;
		case 's': if (is_cube_canRotate) { cube_sideSelecter_i = 3; is_cube_canRotate = !is_cube_canRotate; cube_rotVal_f = +1.0f; } break;
		case 'S': if (is_cube_canRotate) { cube_sideSelecter_i = 3; is_cube_canRotate = !is_cube_canRotate; cube_rotVal_f = -1.0f; } break;
		case 'e': if (is_cube_canRotate) { cube_sideSelecter_i = 6; is_cube_canRotate = !is_cube_canRotate; cube_rotVal_f = +1.0f; } break;
		case 'E': if (is_cube_canRotate) { cube_sideSelecter_i = 6; is_cube_canRotate = !is_cube_canRotate; cube_rotVal_f = -1.0f; } break;
		case 'd': if (is_cube_canRotate) { cube_sideSelecter_i = 5; is_cube_canRotate = !is_cube_canRotate; cube_rotVal_f = +1.0f; } break;
		case 'D': if (is_cube_canRotate) { cube_sideSelecter_i = 5; is_cube_canRotate = !is_cube_canRotate; cube_rotVal_f = -1.0f; } break;
		case 'c': case 'C': cube_sideSelecter_i = 7; break;
		case 'v': case 'V': cube_sideSelecter_i = -1; break;
		case '`': is_cube_suffle = CUBE_SEQUENCE_ING; break;
		}
	}
	glutPostRedisplay(); //--- 배경색이 바뀔때마다 출력 콜백함수를 호출하여 화면을 refresh 한다
}
GLvoid KeyboardUp(unsigned char key, int x, int y)
{
	switch (key) {

	}
}
GLvoid Special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1: camera_startPos_v3 = glm::vec3(5.0f, 5.0f, 5.0f); camera_pos_v3 = camera_startPos_v3;  camera_rot_m4 = glm::mat4(1.0f); break;
	case GLUT_KEY_F2: SetObject(7); break;
	case GLUT_KEY_F3: SetObject(13); break;
	case GLUT_KEY_F4: SetObject(19); break;
	case GLUT_KEY_F5: is_cube_autoSolve = CUBE_SEQUENCE_ING; break;
	}
}
GLvoid SpecialUp(int key, int x, int y)
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
			is_leftButten_up = false;
			mouse_x_i = x;
			mouse_y_i = y;
		}
		else if (state == GLUT_UP)
		{
			is_leftButten_up = true;
		}
	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			is_right_butten_up = false;
			mouse_x_i = x;
			mouse_y_i = y;
		}
		else if (state == GLUT_UP)
		{
			is_right_butten_up = true;
		}
	}
	if (button == GLUT_MIDDLE_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			is_middleButten_up = false;
			mouse_x_i = x;
			mouse_y_i = y;
		}
		else if (state == GLUT_UP)
		{
			is_middleButten_up = true;
		}
	}
}

GLvoid Motion(int x, int y)
{
	
	if (cube_sideSelecter_i == -1 && (!is_leftButten_up || !is_right_butten_up))
	//if (Cube_mainObject.Rotate_Specific_Side_Check_Rot(0.0, 0.0, 0.0) && (!is_leftButten_up || !is_right_butten_up))
	{
		// 마우스 조작으로 인해 회전해야할 각
		int modifier_i = glutGetModifiers();
		float x_angle_f = 0;
		float y_angle_f = 0;
		if (modifier_i == GLUT_ACTIVE_CTRL)
		{
			x_angle_f = (float)(x - mouse_x_i) / WIN_WIDTH * (MAX_ROTATION_MAGNIFICATION / 10);
			y_angle_f = (float)(y - mouse_y_i) / WIN_HIGHT * (MAX_ROTATION_MAGNIFICATION / 10);
		}
		else
		{
			x_angle_f = (float)(x - mouse_x_i) / WIN_WIDTH * MAX_ROTATION_MAGNIFICATION;
			y_angle_f = (float)(y - mouse_y_i) / WIN_HIGHT * MAX_ROTATION_MAGNIFICATION;
		}
		glm::vec3 axis_v3;
		glm::vec4 axis_v4;

		// 세로 회전

		// 회전축 지정
		if (x < WIN_WIDTH / 3)			axis_v4 = glm::vec4(camera_startPos_v3[2], 0.0, camera_startPos_v3[0], sqrt(pow(camera_startPos_v3[0], 2) + pow(camera_startPos_v3[2], 2)));
		else if (x < WIN_WIDTH / 3 * 2) axis_v4 = glm::vec4(camera_startPos_v3[2], 0.0, -camera_startPos_v3[0], sqrt(pow(camera_startPos_v3[0], 2) + pow(camera_startPos_v3[2], 2)));
		else							axis_v4 = glm::vec4(-camera_startPos_v3[2], 0.0, -camera_startPos_v3[0], sqrt(pow(camera_startPos_v3[0], 2) + pow(camera_startPos_v3[2], 2)));

		// 카메라와 객체의 회전 상태를 적용하기위해 각자의 회전행렬을 곱한다.
		axis_v4 = axis_v4 * camera_rot_m4;
		if(!is_right_butten_up)
			axis_v4 = axis_v4 * Cube_mainObject.get_cubeRot();
		axis_v3 = glm::vec3(axis_v4);
		// 마우스 조작 크기만큼 회전
		if (!is_leftButten_up)
			camera_rot_m4 = glm::rotate(camera_rot_m4, (GLfloat)glm::radians(y_angle_f), axis_v3);
		if(!is_right_butten_up)
			Cube_mainObject.Rotate_Cube(axis_v3, y_angle_f);


		// 가로 회전

		// 회전축 지정
		float tmp_vec_x_f = -camera_startPos_v3[0];
		float tmp_vec_y_f = (pow(camera_startPos_v3[0], 2) + pow(camera_startPos_v3[2], 2)) / camera_startPos_v3[1];
		float tmp_vec_z_f = -camera_startPos_v3[2];
		axis_v4 = glm::vec4(tmp_vec_x_f, tmp_vec_y_f, tmp_vec_z_f, sqrt(pow(tmp_vec_x_f, 2) + pow(tmp_vec_y_f, 2) + pow(tmp_vec_z_f, 2)));
		// 카메라와 객체의 회전 상태를 적용하기위해 각자의 회전행렬을 곱한다.
		axis_v4 = axis_v4 * camera_rot_m4;
		if (!is_right_butten_up)
			axis_v4 = axis_v4 * Cube_mainObject.get_cubeRot();
		axis_v3 = glm::vec3(axis_v4);
		// 마우스 조작 크기만큼 회전
		if (!is_leftButten_up)
			camera_rot_m4 = glm::rotate(camera_rot_m4, (GLfloat)glm::radians(x_angle_f), axis_v3);
		if (!is_right_butten_up)
			Cube_mainObject.Rotate_Cube(axis_v3, x_angle_f);
	}
	else if (!is_middleButten_up)
	{
		int modifier_i = glutGetModifiers();
		float x_point_f = 0;
		float y_point_f = 0;
		float size_of_camera_startPos_f = sqrt(pow(camera_startPos_v3.x, 2) + pow(camera_startPos_v3.y, 2) + pow(camera_startPos_v3.z, 2));
		if (modifier_i == GLUT_ACTIVE_CTRL)
		{
			x_point_f = (float)(x - mouse_x_i) / WIN_WIDTH * (MAX_TRANSLATE_DISTANCE / 10);
			y_point_f = (float)(y - mouse_y_i) / WIN_HIGHT * (MAX_TRANSLATE_DISTANCE / 10);
		}
		else
		{
			x_point_f = (float)(x - mouse_x_i) / WIN_WIDTH * MAX_TRANSLATE_DISTANCE;
			y_point_f = (float)(y - mouse_y_i) / WIN_HIGHT * MAX_TRANSLATE_DISTANCE;
		}
		camera_trans_m4 = glm::translate(camera_trans_m4, glm::vec3(camera_startPos_v3.z * x_point_f, -camera_startPos_v3.y * y_point_f, -camera_startPos_v3.x * x_point_f) / size_of_camera_startPos_f);

	}
	mouse_x_i = x;
	mouse_y_i = y;
}

GLvoid MouseWheel(int button, int dir, int x, int y)
{
	int modifier_i = glutGetModifiers();
	float size_of_camera_startPos_f = sqrt(pow(camera_startPos_v3.x, 2) + pow(camera_startPos_v3.y, 2) + pow(camera_startPos_v3.z, 2));
	float magnification;

	if (modifier_i == GLUT_ACTIVE_CTRL) { magnification = 10; }
	else { magnification = 1; }
	size_of_camera_startPos_f *= magnification;

	if (dir > 0) // Zoom Out
	{
		camera_trans_m4 = glm::translate(camera_trans_m4, glm::vec3(-camera_startPos_v3.x, -camera_startPos_v3.y, -camera_startPos_v3.z) / size_of_camera_startPos_f);
	}
	else // Zoom In
	{
		camera_trans_m4 = glm::translate(camera_trans_m4, glm::vec3(camera_startPos_v3.x, camera_startPos_v3.y, camera_startPos_v3.z) / size_of_camera_startPos_f);
	}
}

GLvoid Timer(int value)
{
	// 광원 회전 코드
	light_rot_m4 = glm::rotate(light_rot_m4, (GLfloat)glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));

	if (cube_sideSelecter_i == 7) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (j % 2 != 0) {
					Cube_mainObject.Rotate_Specific_Side_Check_Rot(i, j, 1.0f);
				}
				else {
					Cube_mainObject.Rotate_Specific_Side_Check_Rot(i, j, -1.0f);
				}
			}
		}
	}
	else if (cube_sideSelecter_i == -1) {

	}
	else {
		// 카메라 회전행렬과 큐브 회전행렬의 역행렬을 곱한 값
		glm::vec4 use_compare_v4 = glm::vec4(1.0, 0.0, 0.0, 1.0f);
		glm::vec4* cube_normal_v4 = Cube_mainObject.get_plainNormal();
		//for (int i = 0; i < 6; i++) // 노멀 출력 확인용 코드
		//	std::cout << cube_normal_v4[i].x << ' ' << cube_normal_v4[i].y << ' ' << cube_normal_v4[i].z << std::endl;

		// 걍 6개 나눠서 부여
		switch (cube_sideSelecter_i)
		{
		case 1: use_compare_v4 = glm::vec4(camera_startPos_v3.x, camera_startPos_v3.y, camera_startPos_v3.z, 1.0); break;
		case 2: use_compare_v4 = glm::vec4(-camera_startPos_v3.x, -camera_startPos_v3.y, -camera_startPos_v3.z, 1.0); break;
		case 3: use_compare_v4 = glm::vec4(-camera_startPos_v3.x, (pow(camera_startPos_v3[0], 2) + pow(camera_startPos_v3[2], 2)) / camera_startPos_v3[1], -camera_startPos_v3.z, 1.0); break;// or 1,5 를 외적
		case 4: use_compare_v4 = glm::vec4(camera_startPos_v3.x, -(pow(camera_startPos_v3[0], 2) + pow(camera_startPos_v3[2], 2)) / camera_startPos_v3[1], camera_startPos_v3.z, 1.0); break;
		case 5: use_compare_v4 = glm::vec4(camera_startPos_v3.z, 0.0, -camera_startPos_v3.x, 1.0); break;// or x,-y,z 와 외적
		case 6: use_compare_v4 = glm::vec4(-camera_startPos_v3.z, 0.0, camera_startPos_v3.x, 1.0); break;
		}
		//switch (cube_sideSelecter_i)
		//{
		//case 1: use_compare_v4 = glm::vec4(0.0, 0.0, 1.0, 1.0); break;
		//case 2: use_compare_v4 = glm::vec4(0.0, 0.0, -1.0, 1.0); break;
		//case 3: use_compare_v4 = glm::vec4(0.0, 1.0, 0.0, 1.0); break;
		//case 4: use_compare_v4 = glm::vec4(0.0, -1.0, 0.0, 1.0); break;
		//case 5: use_compare_v4 = glm::vec4(1.0, 0.0, 0.0, 1.0); break;
		//case 6: use_compare_v4 = glm::vec4(-1.0, 0.0, 0.0, 1.0); break;
		//}

		float max_dotVal_f = -1;
		float tmp_dotVal_f;
		int selected_side_i;
		for (int i = 0; i < 6; i++)
		{
			tmp_dotVal_f = dot(camera_rot_m4 * cube_normal_v4[i], use_compare_v4);
			//std::cout << i << ' : ' << tmp_dotVal_f << std::endl;
			if (tmp_dotVal_f > max_dotVal_f)
			{
				max_dotVal_f = tmp_dotVal_f;
				selected_side_i = i + 1;
			}
		}
		//std::cout << std::endl;

		if (dot(use_compare_v4, camera_rot_m4 * Cube_mainObject.get_cubeRot() * glm::vec4(camera_pos_v3, 1.0f)) > 0)
		{
			if (is_cube_canRotate = Cube_mainObject.Rotate_Specific_Side_Check_Rot((selected_side_i - 1) / 2, (selected_side_i - 1) % 2 + (selected_side_i - 1) % 2, -cube_rotVal_f * 5.0f) == CUBE_SEQUENCE_END) { cube_sideSelecter_i = -1; }
		}
		else
		{
			if (is_cube_canRotate = Cube_mainObject.Rotate_Specific_Side_Check_Rot((selected_side_i - 1) / 2, (selected_side_i - 1) % 2 + (selected_side_i - 1) % 2, cube_rotVal_f * 5.0f) == CUBE_SEQUENCE_END) { cube_sideSelecter_i = -1; }
		}

		delete[] cube_normal_v4;
	}


	if (is_cube_suffle == CUBE_SEQUENCE_ING) {
		float cube_suffle_degree_f;
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000) { cube_suffle_degree_f = 90.0f; }
		else { cube_suffle_degree_f = 5.0f; }

		is_cube_suffle = Shuffle_Cube(&Cube_mainObject, 3, cube_suffle_degree_f);
		is_cube_canRotate = !is_cube_suffle;
	}
	else if (is_cube_autoSolve == CUBE_SEQUENCE_ING) {
		float cube_solve_degree_f;
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000) { cube_solve_degree_f = 90.0f; }
		else { cube_solve_degree_f = 10.0f; }

		is_cube_autoSolve = Cube_mainObject.AutoSolveCube(cube_solve_degree_f);
	}
	else if (is_cube_undoRot == CUBE_SEQUENCE_ING) {
		//float cube_undo_degree_f;
		//if (GetAsyncKeyState(VK_CONTROL) & 0x8000) { cube_undo_degree_f = 90.0f; }
		//else { cube_undo_degree_f = 10.0f; }

		is_cube_undoRot = Cube_mainObject.Undo_Rotate_Specific_Side(10.0f);
	}

	is_cube_correctOrder = Cube_mainObject.CheckCubeBlocksLocation(); // 큐브 모양 체크

	if (is_cube_exist) {
		glm::vec4 temp = Cube_mainObject.get_gravityVec();
		Figure_player.Translate(temp.x / 100, temp.y / 100, temp.z / 100);
		//camera_trans_m4 = glm::translate(camera_trans_m4, glm::vec3(temp.x / 100, temp.y / 100, temp.z / 100));
		if (Cube_mainObject.CrashCheck(CUBE_X, &Figure_player)) {
			Figure_player.Translate(CUBE_X, -temp.x / 100);
			//camera_trans_m4 = glm::translate(camera_trans_m4, glm::vec3(-temp.x / 100, 0.0, 0.0));
		}
		if (Cube_mainObject.CrashCheck(CUBE_Y, &Figure_player)) {
			Figure_player.Translate(CUBE_Y, -temp.y / 100);
			//camera_trans_m4 = glm::translate(camera_trans_m4, glm::vec3(0.0, -temp.y / 100, 0.0));
		}
		if (Cube_mainObject.CrashCheck(CUBE_Z, &Figure_player)) {
			Figure_player.Translate(CUBE_Z, -temp.z / 100);
			//camera_trans_m4 = glm::translate(camera_trans_m4, glm::vec3(0.0, 0.0, -temp.z / 100));
		}
	}

	glutPostRedisplay();
	glutTimerFunc(10, Timer, 1);
}

void SetObject(int size) {
	get_size_of_maze(size); 
	Cube_mainObject.MakeCube(3, 3, 3, maze_size / 3, maze_size / 3, maze_size / 3, 0.0, 0.0, 0.0, 3.0, 3.0, 3.0, cube_colorType_i);
	//Cube_mainObject.MakeCube(3, 3, 3, maze_size / 3, maze_size / 3, maze_size / 3, 0.0, 0.0, 0.0, 3.0, 3.0, 3.0, 0.7, 0.7, 0.7);
	Figure_player.Reset();
	Figure_player.MakeCube(0.0, 0.0, 0.0, 3.0 / maze_size / 2, 3.0 / maze_size / 2, 3.0 / maze_size / 2);
	is_print_line = false;
	make_maze_wilson(); 
	if (cube_mazeSelecter_i == 0) {
		Cube_mainObject.InputMaze(maze_route_i);
	}
	else if (cube_mazeSelecter_i == 1) {
		Cube_mainObject.InputMaze(Maze_BY_ANDY_maze.MakeMaze(maze_size));
	}
	else {
		Cube_mainObject.InputMaze(maze_route_i);
	}
	is_cube_exist = true;
}