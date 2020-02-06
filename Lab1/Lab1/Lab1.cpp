#include "pch.h"
#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <ctime>
#include <cstdlib>

GLfloat randFloat() {

	return ((float)rand() / (float)(RAND_MAX));
}

void Rectangle(GLfloat x, GLfloat y, GLfloat a, GLfloat b) {

	glBegin(GL_TRIANGLES);
	glVertex2f(x, y);
	glVertex2f(x + a, y);
	glVertex2f(x + a, y + b);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(x, y);
	glVertex2f(x, y + b);
	glVertex2f(x + a, y + b);
	glEnd();
}

void RemoveCenter(GLfloat x, GLfloat y, GLfloat a, GLfloat b) {
	glColor3f(0.5f, 0.5f, 0.5f); // background color
	Rectangle(x + a * (1.0 / 3.0), y + b * (1.0 / 3.0), a / 3.0, b / 3.0);
}

void CutRectangle(GLfloat x, GLfloat y, GLfloat a, GLfloat b) {
	
	if (a < 1.0) return;

	RemoveCenter(x, y, a, b);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			CutRectangle(x + (i / 3.0)*a, y + (j / 3.0)*b, a / 3.0, b / 3.0);
		}
	}

}


void Fractal(GLfloat x, GLfloat y, GLfloat a, GLfloat b) {

	glColor3f(randFloat(), randFloat(), randFloat());
	Rectangle(x, y, a, b);
	CutRectangle(x, y, a, b);

}

void RenderFractal(void) {

	glClear(GL_COLOR_BUFFER_BIT);
	Fractal(-75.0f, -50.0f, 150.0f, 100.0f);
	glFlush();

}

void MyInit(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
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
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
	else
		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

int main(void)
{
	srand(time(NULL));
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Lab 1 - Fractal");
	glutDisplayFunc(RenderFractal);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glutMainLoop();

	return 0;
}
