#include "pch.h"
#include <iostream>
#include <windows.h>
#include <cmath>
#include <gl/gl.h>
#include <gl/glut.h>

typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };

int model = 1;

static GLfloat theta_x = 0.0;   // k¹t obrotu obiektu
static GLfloat theta_y = 0.0;
static GLfloat theta_z = 0.0;

static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy
							   // 0 - nie naciœniêto ¿adnego klawisza
							   // 1 - naciœniêty zostaæ lewy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int z_pos_old = 0;

static int delta_x = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
									  // i poprzedni¹ kursora myszy
static int delta_y = 0;
static int delta_z = 0;

void Axes(void)
{

	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz?tek i koniec obrazu osi y
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

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         
		y_pos_old = y;
							 
		status = 1;          
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		x_pos_old = x;         
		y_pos_old = y;
		z_pos_old = x;
		status = 2;          
	}
	else

		status = 0;          
}

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;    
	delta_y = y - y_pos_old;
	delta_z = x - x_pos_old;

	x_pos_old = x;            
	y_pos_old = y;
	z_pos_old = x;

	glutPostRedisplay();     
}

void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	Axes();
	glColor3f(1.0f, 1.0f, 1.0f);


	if (status == 1)
	{
		theta_x += delta_x * pix2angle;    
		theta_y += delta_y * pix2angle;
	}                                  

	if (status == 2 && model == 1) {
		
		theta_z += delta_z * pix2angle;
		
		if (theta_z >  5.0) {
			theta_z = 5.0;
		}
		else if (theta_z < -20.0) {
			theta_z = -20.0;
		}

		glTranslatef(0.0, 0.0, theta_z);

	}
	else if (status == 2 && model == 2) {

		theta_x += delta_x * pix2angle;
		theta_y += delta_y * pix2angle;

		int R = 10;

		int x = R * cos(theta_x / 60)*cos(theta_y / 60);
		int y = R * sin(theta_y / 60);
		int z = R * sin(theta_x / 60)*cos(theta_y / 60);

		gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	}

	glRotatef(theta_x, 0.0, 1.0, 0.0); 
	glRotatef(theta_y, 1.0, 0.0, 0.0);

	glutWireTeapot(3.0);
	glFlush();
	glutSwapBuffers();

}

void keys(unsigned char key, int x, int y)
{
	if (key == '1') model = 1;
	if (key == '2') model = 2;

	RenderScene(); 
}


void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal; 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1.0, 1.0, 100.0);

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(300, 300);
	glutCreateWindow("Lab 3 - Mouse");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);         
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(keys);
	glutMainLoop();
}