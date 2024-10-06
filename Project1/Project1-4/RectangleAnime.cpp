#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

struct Rect {
	float x, y;
	float initX, initY;
	float size;
	float r, g, b;
	float dx, dy;
};

std::vector<Rect> rectangles;
int max_rectangles = 5;
bool isAnimating = true;
bool isDiagonal = false;
bool isZigzag = false;
bool isSizeChange = false;
bool isColorChange = false;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
void update(int value);
void randColor(int value);
void randSize(int value);
void createRectangle(int x, int y);
void resetRectangles();
void toggleDiagonal();
void toggleZigzag();
void toggleSizeChange();
void toggleColorChange();

void main(int argc, char** argv)
{
	srand(static_cast<unsigned int>(time(NULL)));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(620, 250);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Color Change");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutTimerFunc(16, update, 0);

	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (const Rect& rect : rectangles) {
		glColor3f(rect.r, rect.g, rect.b);
		glBegin(GL_QUADS);
		glVertex2f(rect.x - rect.size / 2, rect.y - rect.size / 2);
		glVertex2f(rect.x + rect.size / 2, rect.y - rect.size / 2);
		glVertex2f(rect.x + rect.size / 2, rect.y + rect.size / 2);
		glVertex2f(rect.x - rect.size / 2, rect.y + rect.size / 2);
		glEnd();
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (tolower(key)) {
	case '1':
		toggleDiagonal();
		break;

	case '2':
		toggleZigzag();
		break;

	case '3':
		toggleSizeChange();
		break;

	case '4':
		toggleColorChange();
		break;

	case 's':
		isAnimating = false;
		isColorChange = false;
		isSizeChange = false;
		break;

	case 'm':
		// 원래 위치로 이동
		resetRectangles();
		break;

	case 'r':
		rectangles.clear();  // 사각형 삭제
		glutPostRedisplay();
		break;

	case 'q':
		exit(0);  // 프로그램 종료
		break;
	}
}

GLvoid Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && rectangles.size() < max_rectangles) {
		createRectangle(x, y);
	}
}

void createRectangle(int x, int y) {
	Rect newRect;

	newRect.x = static_cast<float>(x);
	newRect.y = static_cast<float>(y);
	newRect.initX = static_cast<float>(x);
	newRect.initY = static_cast<float>(y);
	newRect.size = 30.0f;
	newRect.r = static_cast<float>(rand()) / RAND_MAX;
	newRect.g = static_cast<float>(rand()) / RAND_MAX;
	newRect.b = static_cast<float>(rand()) / RAND_MAX;

	newRect.dx = newRect.dy = 0.0f;

	isDiagonal = false;
	isZigzag = false;

	rectangles.push_back(newRect);
}

void toggleDiagonal() {
	isDiagonal = !isDiagonal;
	if (isDiagonal) {
		for (Rect& rect : rectangles) {
			rect.dx = 2.0f * (rand() % 2 == 0 ? 1 : -1);
			rect.dy = 2.0f * (rand() % 2 == 0 ? 1 : -1);
		}
	}
	else {
		for (Rect& rect : rectangles) {
			rect.dx = rect.dy = 0.0f;
		}
	}
}

void toggleZigzag() {
	isZigzag = !isZigzag;
	if (isZigzag) {
		for (Rect& rect : rectangles) {
			rect.dx = (rand() % 2 == 0 ? 2.0f : -2.0f);
			rect.dy = 0.0f;
		}
	}
	else {
		for (Rect& rect : rectangles) {
			rect.dx = rect.dy = 0.0f;
		}
	}
}

void toggleSizeChange() {
	isSizeChange = !isSizeChange;
	if (isSizeChange) {
		glutTimerFunc(64, randSize, 1);
	}
}

void toggleColorChange() {
	isColorChange = !isColorChange;
	if (isColorChange) {
		glutTimerFunc(64, randColor, 1);
	}
}

void resetRectangles() {
	for (Rect& rect : rectangles) {
		rect.x = rect.initX;
		rect.y = rect.initY;
		rect.dx = rect.dy = 0.0f;
	}
}

void update(int value) {
	if (isAnimating) {
		for (Rect& rect : rectangles) {
			rect.x += rect.dx;
			rect.y += rect.dy;

			// 벽에 닿으면 튕기기
			if (rect.x - rect.size / 2 < 0 || rect.x + rect.size / 2 > glutGet(GLUT_WINDOW_WIDTH)) {
				rect.dx *= -1;
			}
			if (rect.y - rect.size / 2 < 0 || rect.y + rect.size / 2 > glutGet(GLUT_WINDOW_HEIGHT)) {
				rect.dy *= -1;
			}
		}

		glutPostRedisplay();
	}

	glutTimerFunc(16, update, 0);
}

void randColor(int value) {
	if (isColorChange) {
		for (Rect& rect : rectangles) {
			rect.r = static_cast<float>(rand()) / RAND_MAX;
			rect.g = static_cast<float>(rand()) / RAND_MAX;
			rect.b = static_cast<float>(rand()) / RAND_MAX;
		}

		glutPostRedisplay();

		glutTimerFunc(160, randColor, 1);
	}
}

void randSize(int value) {
	if (isSizeChange) {
		for (Rect& rect : rectangles) {
			rect.size = 20.0f + rand() % 30;
		}

		glutPostRedisplay();

		glutTimerFunc(160, randSize, 2);
	}
}