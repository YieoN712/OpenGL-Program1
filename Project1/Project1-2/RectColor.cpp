#include <iostream>
#include <cstdlib>
#include <ctime>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void mouse(int button, int state, int x, int y);
bool isInsideRect(float x, float y, const struct Rectangle& rect);
void scaleRect(struct Rectangle& rect, bool expend);
void RandomColor(float& r, float& g, float& b);
void changeBGColor();

struct Rectangle {
	float x1, x2, y1, y2;
	float r, g, b;
};

struct Rectangle rectangle[4] = {
	{-1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
	{-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f}
};

float BGr = 0.0f, BGg = 0.0f, BGb = 0.0f;

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

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouse);

	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(BGr, BGg, BGb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

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
	if (state == GLUT_DOWN) {
		std::cout << x << ',' << y << '\n';
		float nx = (float)(x) / (glutGet(GLUT_WINDOW_WIDTH) / 2) - 1.0f;
		float ny = 1.0f - (float)(y) / (glutGet(GLUT_WINDOW_HEIGHT) / 2);
		std::cout << nx << ',' << ny << '\n';

		bool insideRect = false;

		for (auto& rect : rectangle) {
			if (isInsideRect(nx, ny, rect)) {
				insideRect = true;

				if (button == GLUT_LEFT_BUTTON) {
					RandomColor(rect.r, rect.g, rect.b);
				}
				else if (button == GLUT_RIGHT_BUTTON) {
					scaleRect(rect, false);
				}
				break;
			}
		}

		if (!insideRect) {
			if (button == GLUT_LEFT_BUTTON) {
				changeBGColor();
			}
			else if (button == GLUT_RIGHT_BUTTON) {
				for (auto& rect : rectangle) {
					scaleRect(rect, true);
				}
			}

			glutPostRedisplay();
		}
	}
}

bool isInsideRect(float x, float y, const struct Rectangle& rect) {
	return (x > rect.x1 && x < rect.x2 && y < rect.y1 && y > rect.y2);
}

void RandomColor(float& r, float& g, float& b) {
	r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

void changeBGColor() {
	RandomColor(BGr, BGg, BGb);
}

void scaleRect(struct Rectangle& rect, bool expend) {
	float scaleFactor = (expend ? 0.04f : -0.04f);

	float width = rect.x2 - rect.x1;
	float height = rect.y1 - rect.y2;
	std::cout << width << ',' << height << '\n';

	if ((width + scaleFactor > 0.05f || expend) && (height + scaleFactor > 0.05f || expend)) {
		rect.x1 -= scaleFactor / 2;
		rect.x2 += scaleFactor / 2;
		rect.y1 += scaleFactor / 2;
		rect.y2 -= scaleFactor / 2;
	}
}