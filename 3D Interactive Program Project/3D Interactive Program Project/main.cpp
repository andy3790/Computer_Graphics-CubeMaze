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

glm::vec4 lightPos;
glm::mat4 lightT;
glm::mat4 lightR;
bool lightOn;

int window1; // use destroy

Figure line;
Figure cube;
Block test;
Cube test2;

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

	lightT = glm::mat4(1.0f);
	lightR = glm::mat4(1.0f);
	lightT = glm::translate(lightT, glm::vec3(0.0f, 2.0f, 4.0f));
	lightPos = glm::vec4(0.0, 0.0, 0.0, 1.0);
	lightOn = true;

	line.MakeLine(5.0f);
	cube.MakeCube(0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0);
	test.MakeBlock(3, 3, 3, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0);
	test2.MakeCube(3, 3, 3, 3, 3, 3);

	glutTimerFunc(10, Timer, 1);

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
	view =glm::lookAt(cameraPos, cameraDirection, cameraUp);

	projMat = glm::perspective((GLfloat)glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HIGHT, 0.1f, 50.0f);
	projMat = glm::translate(projMat, glm::vec3(0.0, 0.0, -5.0));


	{
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projMat));


		line.Draw(transformLocation);
		//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(line.GetTransformMat()));
		//cube.Draw(transformLocation);

		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(lightR * lightT * glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2))));
		cube.Draw();

		//test.Draw(transformLocation);
		test2.Draw(transformLocation);
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
	// ��ü �̵� �׽�Ʈ �ڵ�
	case 'a': cube.Translate('x', -0.1f); break;
	case 'd': cube.Translate('x', 0.1f); break;
	case 'w': cube.Translate('z', -0.1f); break;
	case 's': cube.Translate('z', 0.1f); break;
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
	// ���� ȸ�� �ڵ�
	lightR = glm::rotate(lightR, (GLfloat)glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));
	//cube.Rotate('b', 'y', -1.0f);
	//test.Rotate('b', 'y', -1.0f);
	//test.Rotate('b', 'x', -1.0f);
	//test2.Rotate('c', 'x', -1.0f);

	glutPostRedisplay();
	glutTimerFunc(10, Timer, 1);
}