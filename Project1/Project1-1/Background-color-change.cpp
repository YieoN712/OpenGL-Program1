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
bool isTimerRunning = false;

void main(int argc, char** argv)
{	
	srand(static_cast<unsigned int>(time(NULL)));

	//--- 윈도우 생성하기
	glutInit(&argc, argv);							// glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	// 디스플레이 모드 설정
	glutInitWindowPosition(620, 250);				// 윈도우의 위치 지정
	glutInitWindowSize(500, 500);					// 윈도우의 크기 지정
	glutCreateWindow("Color Change");				// 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)						// glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);						// 출력 함수의 지정
	glutReshapeFunc(Reshape);						// 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard); //--- 키보드 입력 콜백함수 지정
	glutMainLoop();									// 이벤트 처리 시작
}

GLvoid drawScene()
{
	glClearColor(r, g, b, 1.0f);			// 바탕색을 ‘blue’로 지정
	glClear(GL_COLOR_BUFFER_BIT);					// 설정된 색으로 전체를 칠하기
	glutSwapBuffers();								// 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid TimerFunction(int value)
{
	if (isTimerRunning) {
		RandomColor();
		glutPostRedisplay();
		glutTimerFunc(1000, TimerFunction, 0);
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		r = 0.0f; g = 1.0f; b = 1.0f;
		break;
	
	case 'm':
		r = 1.0f; g = 0.0f; b = 1.0f;
		break;
	
	case 'y':
		r = 1.0f; g = 1.0f; b = 0.0f;
		break;
	
	case 'a':
		RandomColor();
		break;
	
	case 'w':
		r = 1.0f; g = 1.0f; b = 1.0f;
		break;
	
	case 'k':
		r = 0.0f; g = 0.0f; b = 0.0f;
		break;
	
	case 't':
		if (!isTimerRunning) {
			isTimerRunning = true;
			glutTimerFunc(1000, TimerFunction, 0);
		}
		break;
	
	case 's':
		isTimerRunning = false;
		break;
	
	case 'q':
		exit(0);
	}

	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

void RandomColor() {
	r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}