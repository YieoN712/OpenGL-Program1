#include <iostream>
#include <gl/glew.h>
#include <cstdlib>
#include <ctime>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void RandomColor();

float r = 0.0f; float g = 0.0f; float b = 0.0f;

void main(int argc, char** argv)
{	
	srand(static_cast<unsigned int>(time(NULL)));

	//--- ������ �����ϱ�
	glutInit(&argc, argv);							// glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	// ���÷��� ��� ����
	glutInitWindowPosition(620, 250);				// �������� ��ġ ����
	glutInitWindowSize(500, 500);					// �������� ũ�� ����
	glutCreateWindow("Color Change");				// ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)						// glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);						// ��� �Լ��� ����
	glutReshapeFunc(Reshape);						// �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard); //--- Ű���� �Է� �ݹ��Լ� ����
	glutMainLoop();									// �̺�Ʈ ó�� ����
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);			// �������� ��blue���� ����
	glClear(GL_COLOR_BUFFER_BIT);					// ������ ������ ��ü�� ĥ�ϱ�
	glutSwapBuffers();								// ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		break;
	
	case 'm':
		break;
	
	case 'y':
		break;
	
	case 'a':
		break;
	
	case 'w':
		break;
	
	case 'k':
		break;
	
	case 't':
		break;
	
	case 's':
		break;
	
	case 'q':
		exit(0);
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

void RandomColor() {
	r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}