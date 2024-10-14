#include <iostream>
#include <cstdlib>
#include <ctime>
#include <gl/glew.h>
#include <gl/freeglut.h>

#define MAX_RECTS 100
const float MIN_SIZE = 0.02f;

struct Rect {
	float x1, y1, x2, y2;
	float r, g, b;
	bool active = false;  // 활성화 여부
	bool shrinking = false;  // 축소 중인지 여부
	float dx = 0.0f, dy = 0.0f;  // 이동 속도
};

struct Check {
	bool mode1 = false;
	bool mode2 = false;
	bool mode3 = false;
	bool mode4 = false;
};

Rect rectangles[MAX_RECTS];
int rectCount = 0;
Check c;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid mouseButton(int button, int state, int x, int y);
GLvoid Timer(int value);
void initRect(Rect& rect, float x1, float y1, float x2, float y2, float r, float g, float b);
void divideRect(const Rect& rect);
void shrinkRect(Rect& rect);
bool isInsideRect(float x, float y, const Rect& rect);
void RandomRectDraw();

void main(int argc, char** argv)
{
	srand(static_cast<unsigned int>(time(NULL)));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(520, 150);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Rectangle Divide and Animate");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "GLEW Initialized\n";
	}

	RandomRectDraw();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(mouseButton);
	glutTimerFunc(16, Timer, 0);
	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < rectCount; ++i) {
		if (rectangles[i].active) {
			glColor3f(rectangles[i].r, rectangles[i].g, rectangles[i].b);
			glBegin(GL_QUADS);
			glVertex2f(rectangles[i].x1, rectangles[i].y1);
			glVertex2f(rectangles[i].x2, rectangles[i].y1);
			glVertex2f(rectangles[i].x2, rectangles[i].y2);
			glVertex2f(rectangles[i].x1, rectangles[i].y2);
			glEnd();
		}
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	if (key == '1') {
		c.mode1 = true; c.mode2 = false; c.mode3 = false; c.mode4 = false;
	}
	else if (key == '2') {
		c.mode1 = false; c.mode2 = true; c.mode3 = false; c.mode4 = false;
	}
	else if (key == '3') {
		c.mode1 = false; c.mode2 = false; c.mode3 = true; c.mode4 = false;
	}
	else if (key == '4') {
		c.mode1 = false; c.mode2 = false; c.mode3 = false; c.mode4 = true;
	}
	else {
		std::cout << "wrong number\n";
	}
}

GLvoid mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float normX = (2.0f * x / 800) - 1.0f;
		float normY = 1.0f - (2.0f * y / 800);

		// 클릭된 사각형을 찾아 4등분 및 애니메이션 시작
		for (int i = 0; i < rectCount; ++i) {
			if (rectangles[i].active && isInsideRect(normX, normY, rectangles[i])) {
				divideRect(rectangles[i]);  // 클릭된 사각형을 4등분
				rectangles[i].active = false;  // 클릭된 사각형 비활성화
				break;
			}
		}
		glutPostRedisplay();
	}
}

void initRect(Rect& rect, float x1, float y1, float x2, float y2, float r, float g, float b) {
	rect.x1 = x1; rect.y1 = y1; rect.x2 = x2; rect.y2 = y2;
	rect.r = r; rect.g = g; rect.b = b;
	rect.active = true; rect.shrinking = false;
	rect.dx = 0.0f; rect.dy = 0.0f;
}

void divideRect(const Rect& rect) {
	float midX = (rect.x1 + rect.x2) / 2;
	float midY = (rect.y1 + rect.y2) / 2;
	float speed = 0.01f;

	// 1 left down
	initRect(rectangles[rectCount++], rect.x1, rect.y1, midX, midY, rect.r, rect.g, rect.b);
	// 2 right down
	initRect(rectangles[rectCount++], midX, rect.y1, rect.x2, midY, rect.r, rect.g, rect.b);
	// 3 left up
	initRect(rectangles[rectCount++], rect.x1, midY, midX, rect.y2, rect.r, rect.g, rect.b);
	// 4 right up
	initRect(rectangles[rectCount++], midX, midY, rect.x2, rect.y2, rect.r, rect.g, rect.b);

	if (c.mode4) {
		// 1 left down
		initRect(rectangles[rectCount++], rect.x1, rect.y1, midX, midY, rect.r, rect.g, rect.b);
		// 2 right down
		initRect(rectangles[rectCount++], midX, rect.y1, rect.x2, midY, rect.r, rect.g, rect.b);
		// 3 left up
		initRect(rectangles[rectCount++], rect.x1, midY, midX, rect.y2, rect.r, rect.g, rect.b);
		// 4 right up
		initRect(rectangles[rectCount++], midX, midY, rect.x2, rect.y2, rect.r, rect.g, rect.b);

		for (int i = rectCount - 8; i < rectCount; ++i) {
			if (i >= rectCount - 4) {
				rectangles[i].dx = (i % 2 == 0 ? speed : -speed);
				rectangles[i].dy = (i < rectCount - 2 ? speed : -speed);
			}
			else {
				if (i == rectCount - 8) rectangles[i].dx = -speed;	// 왼쪽
				if (i == rectCount - 7) rectangles[i].dx = speed;	// 오른쪽
				if (i == rectCount - 6) rectangles[i].dy = speed;	// 위
				if (i == rectCount - 5) rectangles[i].dy = -speed;	// 아래
			}

			rectangles[i].shrinking = true;
		}
	}

	for (int i = rectCount - 4; i < rectCount; ++i) {
		if (c.mode1) {
			if (i == rectCount - 4) rectangles[i].dx = -speed;	// 왼쪽
			if (i == rectCount - 3) rectangles[i].dx = speed;	// 오른쪽
			if (i == rectCount - 2) rectangles[i].dy = speed;	// 위
			if (i == rectCount - 1) rectangles[i].dy = -speed;	// 아래
		}
		else if (c.mode2) {
			rectangles[i].dx = (i % 2 == 0 ? speed : -speed);
			rectangles[i].dy = (i < rectCount - 2 ? speed : -speed);
		}
		else if (c.mode3) {
			rectangles[i].dx = speed;
			rectangles[i].dy = speed;
		}

		rectangles[i].shrinking = true;
	}
}

void shrinkRect(Rect& rect) {
	rect.x1 += rect.dx;
	rect.x2 += rect.dx;
	rect.y1 += rect.dy;
	rect.y2 += rect.dy;

	float shrinkFactor = 0.98f;

	float centerX = (rect.x1 + rect.x2) / 2;
	float centerY = (rect.y1 + rect.y2) / 2;

	float halfWidth = (rect.x2 - rect.x1) * shrinkFactor / 2;
	float halfHeight = (rect.y2 - rect.y1) * shrinkFactor / 2;

	rect.x1 = centerX - halfWidth;
	rect.x2 = centerX + halfWidth;
	rect.y1 = centerY - halfHeight;
	rect.y2 = centerY + halfHeight;

	// 최소 크기 이하로 작아지면 비활성화
	if ((rect.x2 - rect.x1) < MIN_SIZE || (rect.y2 - rect.y1) < MIN_SIZE) {
		rect.active = false;
		rect.shrinking = false;
	}
}

GLvoid Timer(int value) {
	for (int i = 0; i < rectCount; ++i) {
		if (rectangles[i].shrinking) {
			shrinkRect(rectangles[i]);
		}
	}

	glutPostRedisplay();
	glutTimerFunc(16, Timer, 0);
}

bool isInsideRect(float x, float y, const Rect& rect) {
	return (x > rect.x1 && x < rect.x2 && y > rect.y1 && y < rect.y2);
}

void RandomRectDraw() {
	rectCount = 5 + rand() % 6;  // 5~10개의 사각형 생성
	for (int i = 0; i < rectCount; ++i) {
		float x1 = static_cast<float>(rand()) / RAND_MAX * 1.8f - 0.9f;
		float y1 = static_cast<float>(rand()) / RAND_MAX * 1.8f - 0.9f;
		float x2 = x1 + 0.2f;
		float y2 = y1 + 0.2f;
		float r = static_cast<float>(rand()) / RAND_MAX;
		float g = static_cast<float>(rand()) / RAND_MAX;
		float b = static_cast<float>(rand()) / RAND_MAX;

		initRect(rectangles[i], x1, y1, x2, y2, r, g, b);
	}
}
