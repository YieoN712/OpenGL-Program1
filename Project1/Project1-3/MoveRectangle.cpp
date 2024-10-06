#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

struct Rectangle {
	float x1, y1, x2, y2;  // 좌표
	float r, g, b;         // 색상
};

std::vector<struct Rectangle> rectangle;

bool isDragging = false;
int selectedRect = -1;
float offsetX, offsetY;
float targetX, targetY;
bool moveToTarget = false;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid mouse(int button, int state, int x, int y);
GLvoid motion(int x, int y);
GLvoid keyboard(unsigned char key, int x, int y);
bool isInsideRect(float x, float y, const struct Rectangle& rect);
bool isOverlapping(const struct Rectangle& rect1, const struct Rectangle& rect2);
void addRectangle();
void mergeRectangles(int draggedRectIndex);
void timer(int value);

void main(int argc, char** argv)
{
	srand(static_cast<unsigned int>(time(NULL)));

	//--- 윈도우 생성하기
	glutInit(&argc, argv);                            // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);    // 디스플레이 모드 설정
	glutInitWindowPosition(620, 250);                // 윈도우의 위치 지정
	glutInitWindowSize(500, 500);                    // 윈도우의 크기 지정
	glutCreateWindow("Move Rectangle");                // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)                        // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(16, timer, 0);

	glutMainLoop();
}

GLvoid drawScene() {
	glClear(GL_COLOR_BUFFER_BIT);

	// 네모를 그리기
	for (const auto& rect : rectangle) {
		glColor3f(rect.r, rect.g, rect.b);
		glRectf(rect.x1, rect.y1, rect.x2, rect.y2);
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void mouse(int button, int state, int x, int y) {
	// 윈도우 좌표를 OpenGL 좌표로 변환
	float nx = (float)(x) / (glutGet(GLUT_WINDOW_WIDTH) / 2) - 1.0f;
	float ny = 1.0f - (float)(y) / (glutGet(GLUT_WINDOW_HEIGHT) / 2);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// 네모 안에 마우스가 있으면 드래그 시작
		for (int i = rectangle.size() - 1; i >= 0; --i) {
			if (isInsideRect(nx, ny, rectangle[i])) {
				isDragging = true;
				selectedRect = i;
				offsetX = nx - rectangle[i].x1;
				offsetY = ny - rectangle[i].y1;
				break;
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		// 드래그 해제
		isDragging = false;

		// 드래그된 네모가 다른 네모와 겹칠 때만 합침
		if (selectedRect != -1) {
			mergeRectangles(selectedRect);
		}

		selectedRect = -1;
	}

	glutPostRedisplay();
}

void motion(int x, int y) {
	if (isDragging && selectedRect != -1) {
		// 윈도우 좌표를 OpenGL 좌표로 변환
		float nx = (float)(x) / (glutGet(GLUT_WINDOW_WIDTH) / 2) - 1.0f;
		float ny = 1.0f - (float)(y) / (glutGet(GLUT_WINDOW_HEIGHT) / 2);

		// float width = rectangle[selectedRect].x2 - rectangle[selectedRect].x1;
		// float height = rectangle[selectedRect].y2 - rectangle[selectedRect].y1;

		// 마우스의 이동 위치에 따라 네모의 위치를 업데이트
		/*rectangle[selectedRect].x1 = nx - offsetX;
		rectangle[selectedRect].x2 = rectangle[selectedRect].x1 + width;
		rectangle[selectedRect].y1 = ny - offsetY;
		rectangle[selectedRect].y2 = rectangle[selectedRect].y1 + height;

		std::cout << "Dragging rectangle " << selectedRect << " to ("
			<< rectangle[selectedRect].x1 << ", " << rectangle[selectedRect].y1 << ")\n";

		glutPostRedisplay();*/

		targetX = nx - offsetX;
		targetY = ny - offsetY;
		moveToTarget = true;
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'a' && rectangle.size() < 10) {
		addRectangle();
	}

	glutPostRedisplay();
}

void addRectangle() {
	// 네모 생성, 고정된 크기 0.2f
	float fixedSize = 0.2f;
	float x1 = ((rand() % 80) / 50.0f) - 0.8f;
	float y1 = ((rand() % 80) / 50.0f) - 0.8f;
	float x2 = x1 + fixedSize;
	float y2 = y1 + fixedSize;

	// 랜덤한 색상 설정
	float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	struct Rectangle newRect = { x1, y1, x2, y2, r, g, b };
	rectangle.push_back(newRect);
}

bool isOverlapping(const struct Rectangle& rect1, const struct Rectangle& rect2) {
	// 두 네모가 겹치는지 여부 확인
	return !(rect1.x1 > rect2.x2 || rect1.x2 < rect2.x1 || rect1.y1 > rect2.y2 || rect1.y2 < rect2.y1);
}

bool isInsideRect(float x, float y, const struct Rectangle& rect) {
	// 마우스가 네모 안에 있는지 여부 확인
	return (x > rect.x1 && x < rect.x2 && y < rect.y1 && y > rect.y2);
}

void mergeRectangles(int draggedRectIndex) {
	// 드래그한 네모만 다른 네모와 겹칠 때 합침
	for (size_t j = 0; j < rectangle.size();) {
		if (j != draggedRectIndex && isOverlapping(rectangle[draggedRectIndex], rectangle[j])) {
			// 드래그한 네모와 다른 네모를 합침
			rectangle[draggedRectIndex].x1 = std::min(rectangle[draggedRectIndex].x1, rectangle[j].x1);
			rectangle[draggedRectIndex].y1 = std::min(rectangle[draggedRectIndex].y1, rectangle[j].y1);
			rectangle[draggedRectIndex].x2 = std::max(rectangle[draggedRectIndex].x2, rectangle[j].x2);
			rectangle[draggedRectIndex].y2 = std::max(rectangle[draggedRectIndex].y2, rectangle[j].y2);

			// 색상을 랜덤하게 변경
			rectangle[draggedRectIndex].r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			rectangle[draggedRectIndex].g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			rectangle[draggedRectIndex].b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

			// 겹친 네모 제거
			rectangle.erase(rectangle.begin() + j);
		}
		else {
			++j;
		}
	}
}

void timer(int value) {
	if (moveToTarget && selectedRect != -1) {
		float speed = 0.05f;  // 이동 속도
		float width = rectangle[selectedRect].x2 - rectangle[selectedRect].x1;
		float height = rectangle[selectedRect].y2 - rectangle[selectedRect].y1;

		// 네모의 현재 좌표
		float currentX = rectangle[selectedRect].x1;
		float currentY = rectangle[selectedRect].y1;

		// 목표 위치로 이동
		rectangle[selectedRect].x1 += (targetX - currentX) * speed;
		rectangle[selectedRect].y1 += (targetY - currentY) * speed;
		rectangle[selectedRect].x2 = rectangle[selectedRect].x1 + width;
		rectangle[selectedRect].y2 = rectangle[selectedRect].y1 + height;

		// 목표 위치에 도달했을 경우
		if (abs(targetX - rectangle[selectedRect].x1) < 0.01f && abs(targetY - rectangle[selectedRect].y1) < 0.01f) {
			moveToTarget = false;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);  // 16ms 간격으로 타이머 호출 (약 60fps)
}