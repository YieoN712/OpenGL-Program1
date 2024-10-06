#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

struct Rect {
	float x1, y1, x2, y2;
	float r, g, b;
};

struct EraserRect {
	float x, y;
	float r, g, b;

	EraserRect() : r(1.0f), g(1.0f), b(1.0f), x(0.0f), y(0.0f) {}
};

std::vector<Rect> rectangle;

EraserRect eraser;
bool erasing = false;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid mouseButton(int button, int state, int x, int y);
GLvoid mouseMove(int x, int y);
void RandomRectDraw();
void drawEraser();
void checkCollision();

void main(int argc, char** argv)
{
	srand(static_cast<unsigned int>(time(NULL)));

	//--- 윈도우 생성하기
	glutInit(&argc, argv);							// glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	// 디스플레이 모드 설정
	glutInitWindowPosition(620, 250);				// 윈도우의 위치 지정
	glutInitWindowSize(500, 500);					// 윈도우의 크기 지정
	glutCreateWindow("Clear screen");				// 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)						// glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (const Rect& rect : rectangle) {
		glColor3f(rect.r, rect.g, rect.b);
		glBegin(GL_QUADS);
		glVertex2f(rect.x1, rect.y1);
		glVertex2f(rect.x2, rect.y1);
		glVertex2f(rect.x2, rect.y2);
		glVertex2f(rect.x1, rect.y2);
		glEnd();
	}

	drawEraser();
	checkCollision();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid keyboard(unsigned char key, int x, int y) {
	if (key == 'r') {
		rectangle.clear();
		RandomRectDraw();
		glutPostRedisplay();
	}
}

GLvoid mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		erasing = true;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		erasing = false;
	}
}

GLvoid mouseMove(int x, int y) {
	if (erasing) {
		eraser.x = static_cast<float>(x) / (glutGet(GLUT_WINDOW_WIDTH) / 2) - 1.0f;
		eraser.y = 1.0f - static_cast<float>(y) / (glutGet(GLUT_WINDOW_HEIGHT) / 2);

		glutPostRedisplay();
	}
}

void RandomRectDraw() {
	int numRects = 20 + rand() % 21;
	for (int i = 0; i < numRects; ++i) {
		Rect rect;

		rect.x1 = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
		rect.y1 = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
		rect.x2 = rect.x1 + 0.1f;
		rect.y2 = rect.y1 + 0.1f;

		rect.r = static_cast<float>(rand()) / RAND_MAX;
		rect.g = static_cast<float>(rand()) / RAND_MAX;
		rect.b = static_cast<float>(rand()) / RAND_MAX;

		rectangle.push_back(rect);
	}
}

void drawEraser() {
	if (erasing) {
		glColor3f(eraser.r, eraser.g, eraser.b);
		glBegin(GL_QUADS);
		glVertex2f(eraser.x - 0.2f, eraser.y - 0.2f);
		glVertex2f(eraser.x + 0.2f, eraser.y - 0.2f);
		glVertex2f(eraser.x + 0.2f, eraser.y + 0.2f);
		glVertex2f(eraser.x - 0.2f, eraser.y + 0.2f);
		glEnd();
	}
}

void checkCollision() {
	for (auto it = rectangle.begin(); it != rectangle.end(); ) {
		if (eraser.x - 0.2f < it->x2 && eraser.x + 0.2f > it->x1 &&
			eraser.y - 0.2f < it->y2 && eraser.y + 0.2f > it->y1) {
			eraser.r = it->r;
			eraser.g = it->g;
			eraser.b = it->b;

			it = rectangle.erase(it);
		}
		else {
			++it;
		}
	}
}