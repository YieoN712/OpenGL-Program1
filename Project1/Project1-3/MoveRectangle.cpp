#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

struct Rectangle {
	float x1, y1, x2, y2;  // ��ǥ
	float r, g, b;         // ����
};

std::vector<struct Rectangle> rectangle;

bool isDragging = false;
int selectedRect = -1;
float offsetX, offsetY;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid mouse(int button, int state, int x, int y);
GLvoid motion(int x, int y);
GLvoid keyboard(unsigned char key, int x, int y);
bool isInsideRect(float x, float y, const struct Rectangle& rect);
bool isOverlapping(const struct Rectangle& rect1, const struct Rectangle& rect2);
void addRectangle();
void mergeRectangles(int draggedRectIndex);

void main(int argc, char** argv)
{
	srand(static_cast<unsigned int>(time(NULL)));

	//--- ������ �����ϱ�
	glutInit(&argc, argv);                            // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);    // ���÷��� ��� ����
	glutInitWindowPosition(620, 250);                // �������� ��ġ ����
	glutInitWindowSize(500, 500);                    // �������� ũ�� ����
	glutCreateWindow("Move Rectangle");                // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)                        // glew �ʱ�ȭ
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

	glutMainLoop();
}

GLvoid drawScene() {
	glClear(GL_COLOR_BUFFER_BIT);

	// �׸� �׸���
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
	// ������ ��ǥ�� OpenGL ��ǥ�� ��ȯ
	float nx = (float)(x) / (glutGet(GLUT_WINDOW_WIDTH) / 2) - 1.0f;
	float ny = 1.0f - (float)(y) / (glutGet(GLUT_WINDOW_HEIGHT) / 2);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// �׸� �ȿ� ���콺�� ������ �巡�� ����
		for (int i = rectangle.size() - 1; i >= 0; --i) {
			if (isInsideRect(nx, ny, rectangle[i])) {
				isDragging = true;
				selectedRect = i;
				offsetX = nx - rectangle[i].x1;
				offsetY = ny - rectangle[i].y1;

				std::cout << "Selected rectangle " << i << " at (" << nx << ", " << ny << ")\n";
				break;
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		// �巡�� ����
		isDragging = false;

		// �巡�׵� �׸� �ٸ� �׸�� ��ĥ ���� ��ħ
		if (selectedRect != -1) {
			std::cout << "merge\n";
			mergeRectangles(selectedRect);
		}

		selectedRect = -1;

		std::cout << "Drag ended.\n";
	}

	glutPostRedisplay();
}

void motion(int x, int y) {
	if (isDragging && selectedRect != -1) {
		// ������ ��ǥ�� OpenGL ��ǥ�� ��ȯ
		float nx = (float)(x) / (glutGet(GLUT_WINDOW_WIDTH) / 2) - 1.0f;
		float ny = 1.0f - (float)(y) / (glutGet(GLUT_WINDOW_HEIGHT) / 2);

		float width = rectangle[selectedRect].x2 - rectangle[selectedRect].x1;
		float height = rectangle[selectedRect].y2 - rectangle[selectedRect].y1;

		// ���콺�� �̵� ��ġ�� ���� �׸��� ��ġ�� ������Ʈ
		rectangle[selectedRect].x1 = nx - offsetX;
		rectangle[selectedRect].x2 = rectangle[selectedRect].x1 + width;
		rectangle[selectedRect].y1 = ny - offsetY;
		rectangle[selectedRect].y2 = rectangle[selectedRect].y1 + height;

		std::cout << "Dragging rectangle " << selectedRect << " to ("
			<< rectangle[selectedRect].x1 << ", " << rectangle[selectedRect].y1 << ")\n";

		glutPostRedisplay();
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'a' && rectangle.size() < 10) {
		addRectangle();
	}

	glutPostRedisplay();
}

void addRectangle() {
	// �׸� ����, ������ ũ�� 0.2f
	float fixedSize = 0.2f;
	float x1 = ((rand() % 80) / 50.0f) - 0.8f;
	float y1 = ((rand() % 80) / 50.0f) - 0.8f;
	float x2 = x1 + fixedSize;
	float y2 = y1 + fixedSize;

	// ������ ���� ����
	float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	struct Rectangle newRect = { x1, y1, x2, y2, r, g, b };
	rectangle.push_back(newRect);
}

bool isOverlapping(const struct Rectangle& rect1, const struct Rectangle& rect2) {
	// �� �׸� ��ġ���� ���� Ȯ��
	return !(rect1.x1 > rect2.x2 || rect1.x2 < rect2.x1 || rect1.y1 > rect2.y2 || rect1.y2 < rect2.y1);
}

bool isInsideRect(float x, float y, const struct Rectangle& rect) {
	// ���콺�� �׸� �ȿ� �ִ��� ���� Ȯ��
	return (x > rect.x1 && x < rect.x2 && y < rect.y1 && y > rect.y2);
}

void mergeRectangles(int draggedRectIndex) {
	// �巡���� �׸� �ٸ� �׸�� ��ĥ �� ��ħ
	for (size_t j = 0; j < rectangle.size();) {
		if (j != draggedRectIndex && isOverlapping(rectangle[draggedRectIndex], rectangle[j])) {
			// �巡���� �׸�� �ٸ� �׸� ��ħ
			rectangle[draggedRectIndex].x1 = std::min(rectangle[draggedRectIndex].x1, rectangle[j].x1);
			rectangle[draggedRectIndex].y1 = std::min(rectangle[draggedRectIndex].y1, rectangle[j].y1);
			rectangle[draggedRectIndex].x2 = std::max(rectangle[draggedRectIndex].x2, rectangle[j].x2);
			rectangle[draggedRectIndex].y2 = std::max(rectangle[draggedRectIndex].y2, rectangle[j].y2);

			// ������ �����ϰ� ����
			rectangle[draggedRectIndex].r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			rectangle[draggedRectIndex].g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			rectangle[draggedRectIndex].b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

			// ��ģ �׸� ����
			rectangle.erase(rectangle.begin() + j);
		}
		else {
			++j;
		}
	}
}
