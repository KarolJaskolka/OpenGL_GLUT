#include "pch.h"
#include <windows.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <gl/gl.h>
#include <gl/glut.h>
#include <ctime>
#include <cstdlib>

typedef float point3[3];

int model = 1;

static GLfloat theta[] = { 0.0, 0.0, 0.0 };

GLfloat randFloat() {

	return ((float)rand() / (float)(RAND_MAX));
}

struct Point {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

void Axes(void){

	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz¹tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz¹tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz¹tek i koniec obrazu osi y

	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();

}

void Egg() {
	
	const int N = 25;

	Point matrix[N][N];

	for (int i = 0; i < N; i++) {

		float u = ((float)i) / (N - 1);

		for (int j = 0; j < N; j++) {
			
			float v = ((float)j) / (N - 1);

			matrix[i][j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(M_PI*v);
			matrix[i][j].y = 5 - (160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2));
			matrix[i][j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(M_PI*v);

		}
	}
	
	switch (model) {
	case 1:
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				
				glColor3f(1.0, 1.0, 1.0);
				
				glBegin(GL_POINTS);
				glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
				glEnd();

			}
		}
		break;
	case 2:
		for (int i = 0; i < N-1; i++) {
			for (int j = 0; j < N-1; j++) {
				
				glColor3f(0.0, 0.0, 1.0);
				
				glBegin(GL_LINES);
				glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
				glVertex3f(matrix[i+1][j].x, matrix[i+1][j].y, matrix[i+1][j].z);
				glEnd();
				
				glBegin(GL_LINES);
				glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
				glVertex3f(matrix[i][j+1].x, matrix[i][j+1].y, matrix[i][j+1].z);
				glEnd();

			}
		}
		break;
	case 3:
		for (int i = 0; i < N-1; i++) {
			for (int j = 0; j < N-1; j++) {
				
				switch (N % 2) {
				case 1:
					if (i % 2 == 0 && i < N / 2) {
						glColor3f(1.0, 0.0, 0.0);
					}
					else if (i % 2 == 1 && i < N / 2) {
						glColor3f(1.0, 1.0, 1.0);
					}
					else if (i % 2 == 0 && i >= N / 2) {

						glColor3f(1.0, 1.0, 1.0);
					}
					else if (i % 2 == 1 && i >= N / 2) {
						glColor3f(1.0, 0.0, 0.0);
					}
					break;
				default:
					if (i % 2 == 0) {
						glColor3f(1.0, 0.0, 0.0);
					}
					else {
						glColor3f(1.0, 1.0, 1.0);
					}
					break;
				}


				glBegin(GL_TRIANGLES);
				glVertex3f(matrix[i][j].x, matrix[i][j].y,matrix[i][j].z);
				glVertex3f(matrix[i + 1][j].x, matrix[i + 1][j].y, matrix[i + 1][j].z);
				glVertex3f(matrix[i + 1 ][j + 1].x, matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);
				glEnd();

				glBegin(GL_TRIANGLES);
				glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
				glVertex3f(matrix[i][j + 1].x, matrix[i][j + 1].y, matrix[i][j + 1].z);
				glVertex3f(matrix[i + 1][j + 1].x, matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);
				glEnd();
			}
		}
		break;
	default:
		for (int i = 0; i < N - 2; i+=2) {
			for (int j = 0; j < N - 2; j+=2) {

				glColor3f(0.0, 0.75, 0.0);

				glBegin(GL_TRIANGLE_STRIP);
				
				glVertex3f(matrix[i][j + 2].x, matrix[i][j + 2].y, matrix[i][j + 2].z);
				glVertex3f(matrix[i + 1][j + 2].x, matrix[i + 1][j + 2].y, matrix[i + 1][j + 2].z);
				glVertex3f(matrix[i][j + 1].x, matrix[i][j + 1].y, matrix[i][j + 1].z);
				glVertex3f(matrix[i + 1][j + 1].x, matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);
				glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
				glVertex3f(matrix[i + 1][j].x, matrix[i + 1][j].y, matrix[i + 1][j].z);
				
				glEnd();

				glColor3f(0.0, 0.0, 0.75);

				glBegin(GL_TRIANGLE_STRIP);

				glVertex3f(matrix[i + 1][j + 2].x, matrix[i + 1][j + 2].y, matrix[i + 1][j + 2].z);
				glVertex3f(matrix[i + 2][j + 2].x, matrix[i + 2][j + 2].y, matrix[i + 2][j + 2].z);
				glVertex3f(matrix[i][j + 1].x, matrix[i][j + 1].y, matrix[i][j + 1].z);
				glVertex3f(matrix[i + 2][j + 1].x, matrix[i + 2][j + 1].y, matrix[i + 2][j + 1].z);
				glVertex3f(matrix[i][j].x,matrix[i][j].y, matrix[i][j].z);
				glVertex3f(matrix[i + 2][j].x, matrix[i + 2][j].y, matrix[i + 2][j].z);

				glEnd();

			}
		}
		break;
	}

}

void RenderScene(void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	Axes();

	glRotatef(theta[0], 1.0, 0.0, 0.0);

	glRotatef(theta[1], 0.0, 1.0, 0.0);

	glRotatef(theta[2], 0.0, 0.0, 1.0);

	Egg();
	
	glFlush();

	glutSwapBuffers();
}

void spinEgg()
{

	theta[0] -= 0.05;
	if (theta[0] > 360.0) theta[0] -= 360.0;

	theta[1] -= 0.05;
	if (theta[1] > 360.0) theta[1] -= 360.0;

	theta[2] -= 0.05;
	if (theta[2] > 360.0) theta[2] -= 360.0;

	glutPostRedisplay();
}

void keys(unsigned char key, int x, int y)
{
	if (key == '1') model = 1;
	if (key == '2') model = 2;
	if (key == '3') model = 3;
	if (key == '4') model = 4;

	RenderScene(); // przerysowanie obrazu sceny
}

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	GLfloat AspectRatio;

	if (vertical == 0) vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();        
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;  

	if (horizontal <= vertical)
		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else
		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);

	glMatrixMode(GL_MODELVIEW);                                 
	glLoadIdentity();

}

void main(void){
	srand(time(NULL));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(300, 300);
	glutCreateWindow("Lab 2 - Egg");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutKeyboardFunc(keys);
	glutIdleFunc(spinEgg);
	glutMainLoop();
}