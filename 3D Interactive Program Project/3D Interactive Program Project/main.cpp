#include "includer.h"
#include "shader_program.h"
#include "Figure.h"
#include "util.h"
#define WIN_WIDTH 800	// ������ ���� ũ��
#define WIN_HIGHT 800	// ������ ���� ũ��

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> F_urd(0.1, 1.0);	// Figure class�� ���� ���� ������

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
glm::vec3 cameraPos; //--- ī�޶� ��ġ
glm::vec3 cameraDirection; //--- ī�޶� �ٶ󺸴� ����
glm::vec3 cameraUp; //--- ī�޶� ���� ����

int window1; // use destroy

Figure line;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(WIN_WIDTH, WIN_HIGHT); // �������� ũ�� ����
	window1 = glutCreateWindow("Shader Programing"); // ������ ���� (������ �̸�)
		//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	InitShader();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	cameraStartPos = glm::vec3(5.0f, 5.0f, 5.0f);
	cameraStartDir = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraPos = cameraStartPos; //--- ī�޶� ��ġ
	cameraDirection = cameraStartDir; //--- ī�޶� �ٶ󺸴� ����
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����

	line.MakeCube(0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0);


	//glutTimerFunc(10, Timer, 1);

	glutDisplayFunc(drawScene); // ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ��Լ� ����
	glutKeyboardFunc(Keyboard); // Ű���� �Է� �ݹ��Լ� ����
	glutKeyboardUpFunc(Keyboard_up); // Ű���� �Է� �ݹ��Լ� ����
	glutSpecialFunc(Special); // Ư�� Ű �Է� �ݹ��Լ� ����
	glutSpecialUpFunc(Special_up); // Ư�� Ű �Է� �ݹ��Լ� ����
	glutMouseFunc(Mouse); // ���콺 �Է� �ݹ��Լ� ����
	glutMotionFunc(Motion);
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // �������� ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	//--- ��ȯ ��� ���� ���ؽ� ���̴��� ������
	unsigned int transformLocation = glGetUniformLocation(s_program, "model");
	unsigned int viewLocation = glGetUniformLocation(s_program, "view");
	unsigned int projectionLocation = glGetUniformLocation(s_program, "projection");
	unsigned int colorLocation = glGetUniformLocation(s_program, "model_color");
	glUniform3f(colorLocation, -1.0, -1.0, -1.0);

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projMat = glm::mat4(1.0f);
	view =glm::lookAt(cameraPos, cameraDirection, cameraUp);

	projMat = glm::perspective((GLfloat)glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HIGHT, 0.1f, 50.0f);
	projMat = glm::translate(projMat, glm::vec3(0.0, 0.0, -5.0));


	{
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projMat));



		//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(line.GetTransformMat()));
		line.Draw(transformLocation);
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q': case 'Q': glutDestroyWindow(window1); break; // ���α׷� ����
	}
	glutPostRedisplay(); //--- ������ �ٲ𶧸��� ��� �ݹ��Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}
GLvoid Keyboard_up(unsigned char key, int x, int y)
{
	switch (key) {

	}
}
GLvoid Special(int key, int x, int y)
{
	switch (key) {

	}
}
GLvoid Special_up(int key, int x, int y)
{
	switch (key) {

	}
}

GLvoid Mouse(int button, int state, int x, int y)
{
}

GLvoid Motion(int x, int y)
{
}

GLvoid Timer(int value)
{


	glutPostRedisplay();
	glutTimerFunc(10, Timer, 1);
}