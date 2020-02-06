#include "pch.h"
#include <windows.h>
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <cmath>
#include <gl/gl.h>
#include <gl/glut.h>


struct Point {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat xu;
	GLfloat xv;
	GLfloat yu;
	GLfloat yv;
	GLfloat zu;
	GLfloat zv;
};

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

void Egg() {

	const int N = 25;

	Point matrix[N][N];

	for (int i = 0; i < N; i++) {

		float u = ((float)i) / (N - 1);

		for (int j = 0; j < N; j++) {

			float v = ((float)j) / (N - 1);

			matrix[i][j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(M_PI*v);
			matrix[i][j].y = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
			matrix[i][j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(M_PI*v);

			matrix[i][j].xu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*cos(M_PI*v);
			matrix[i][j].xv = M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(M_PI*v);
			matrix[i][j].yu = (640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u);
			matrix[i][j].yv = 0;
			matrix[i][j].zu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*sin(M_PI*v);
			matrix[i][j].zv = -M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(M_PI*v);

		}
	}

	for (int i = 0; i < N - 1; i++) {
		for (int j = 0; j < N - 1; j++) {

			GLfloat lengthV1 = sqrt(pow(matrix[i][j].yu * matrix[i][j].zv - matrix[i][j].zu * matrix[i][j].yv,2) +
									pow(matrix[i][j].zu * matrix[i][j].xv - matrix[i][j].xu * matrix[i][j].zv,2) +
									pow(matrix[i][j].xu * matrix[i][j].yv - matrix[i][j].yu * matrix[i][j].xv,2));

			GLfloat lengthV2 = sqrt(pow(matrix[i + 1][j].yu * matrix[i + 1][j].zv - matrix[i + 1][j].zu * matrix[i + 1][j].yv, 2) +
									pow(matrix[i + 1][j].zu * matrix[i + 1][j].xv - matrix[i + 1][j].xu * matrix[i + 1][j].zv, 2) +
									pow(matrix[i + 1][j].xu * matrix[i + 1][j].yv - matrix[i + 1][j].yu * matrix[i + 1][j].xv, 2));

			GLfloat lengthV3 = sqrt(pow(matrix[i][j + 1].yu * matrix[i][j + 1].zv - matrix[i][j + 1].zu * matrix[i][j + 1].yv, 2) +
									pow(matrix[i][j + 1].zu * matrix[i][j + 1].xv - matrix[i][j + 1].xu * matrix[i][j + 1].zv, 2) +
									pow(matrix[i][j + 1].xu * matrix[i][j + 1].yv - matrix[i][j + 1].yu * matrix[i][j + 1].xv, 2));


			GLfloat lengthV4 = sqrt(pow(matrix[i + 1][j + 1].yu * matrix[i + 1][j + 1].zv - matrix[i + 1][j + 1].zu * matrix[i + 1][j + 1].yv, 2) +
									pow(matrix[i + 1][j + 1].zu * matrix[i + 1][j + 1].xv - matrix[i + 1][j + 1].xu * matrix[i + 1][j + 1].zv, 2) +
									pow(matrix[i + 1][j + 1].xu * matrix[i + 1][j + 1].yv - matrix[i + 1][j + 1].yu * matrix[i + 1][j + 1].xv, 2));

			GLfloat v1[3] = { (matrix[i][j].yu * matrix[i][j].zv - matrix[i][j].zu * matrix[i][j].yv) / lengthV1,
							(matrix[i][j].zu * matrix[i][j].xv - matrix[i][j].xu * matrix[i][j].zv) / lengthV1,
							(matrix[i][j].xu * matrix[i][j].yv - matrix[i][j].yu * matrix[i][j].xv) / lengthV1 };

			GLfloat v2[3] = { (matrix[i + 1][j].yu * matrix[i + 1][j].zv - matrix[i + 1][j].zu * matrix[i + 1][j].yv) / lengthV2,
								(matrix[i + 1][j].zu * matrix[i + 1][j].xv - matrix[i + 1][j].xu * matrix[i + 1][j].zv) / lengthV2,
								(matrix[i + 1][j].xu * matrix[i + 1][j].yv - matrix[i + 1][j].yu * matrix[i + 1][j].xv) / lengthV2 };

			GLfloat v3[3] = { (matrix[i][j + 1].yu * matrix[i][j + 1].zv - matrix[i][j + 1].zu * matrix[i][j + 1].yv) / lengthV3,
								(matrix[i][j + 1].zu * matrix[i][j + 1].xv - matrix[i][j + 1].xu * matrix[i][j + 1].zv) / lengthV3,
								(matrix[i][j + 1].xu * matrix[i][j + 1].yv - matrix[i][j + 1].yu * matrix[i][j + 1].xv) / lengthV3 };

			GLfloat v4[3] = { (matrix[i + 1][j + 1].yu * matrix[i + 1][j + 1].zv - matrix[i + 1][j + 1].zu * matrix[i + 1][j + 1].yv) / lengthV4,
								(matrix[i + 1][j + 1].zu * matrix[i + 1][j + 1].xv - matrix[i + 1][j + 1].xu * matrix[i + 1][j + 1].zv) / lengthV4,
								(matrix[i + 1][j + 1].xu * matrix[i + 1][j + 1].yv - matrix[i + 1][j + 1].yu * matrix[i + 1][j + 1].xv) / lengthV4 };

			if (i < N / 2) {
				v1[0] *= -1; v1[1] *= -1; v1[2] *= -1;
				v2[0] *= -1; v2[1] *= -1; v2[2] *= -1;
				v3[0] *= -1; v3[1] *= -1; v3[2] *= -1;
				v4[0] *= -1; v4[1] *= -1; v4[2] *= -1;
			}

			glBegin(GL_TRIANGLES);

			glNormal3fv(v1);
			glVertex3f(matrix[i][j].x, 5 - matrix[i][j].y, matrix[i][j].z);
			
			glNormal3fv(v2);
			glVertex3f(matrix[i + 1][j].x, 5 - matrix[i + 1][j].y, matrix[i + 1][j].z);
			
			glNormal3fv(v4);
			glVertex3f(matrix[i + 1][j + 1].x, 5 - matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);
			
			glEnd();

			glBegin(GL_TRIANGLES);
			
			glNormal3fv(v1);
			glVertex3f(matrix[i][j].x, 5 - matrix[i][j].y, matrix[i][j].z);
			
			glNormal3fv(v3);
			glVertex3f(matrix[i][j + 1].x, 5 - matrix[i][j + 1].y, matrix[i][j + 1].z);
			
			glNormal3fv(v4);
			glVertex3f(matrix[i + 1][j + 1].x, 5 - matrix[i + 1][j + 1].y, matrix[i + 1][j + 1].z);
			
			glEnd();
		}
	}
}

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

		if (theta_z > 5.0) {
			theta_z = 5.0;
		}
		else if (theta_z < -20.0) {
			theta_z = -20.0;
		}

		glTranslatef(0.0, 0.0, theta_z);

	}
	else if (status == 2 && model == 3) {

		theta_z += delta_z * pix2angle;

		if (theta_z > 25.0) {
			theta_z = 25.0;
		}
		else if (theta_z < 1.0) {
			theta_z = 1.0;
		}

		gluLookAt(viewer[0], viewer[1], theta_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

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

	Egg();

	glFlush();
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
	if (key == '1') model = 1;
	if (key == '2') model = 2;
	if (key == '3') model = 3;

	RenderScene(); 
}

void MyInit(void)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcz¹cy (wype³nienia okna) ustawiono na czarny

	GLfloat mat_ambient[] = { 0.25, 0.25, 0.25, 0.1 };
	// wspó³czynniki ka =[kar,kag,kab] dla œwiat³a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki kd =[kdr,kdg,kdb] œwiat³a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki ks =[ksr,ksg,ksb] dla œwiat³a odbitego             

	GLfloat mat_shininess = { 100.0 };
	// wspó³czynnik n opisuj¹cy po³ysk powierzchni

	GLfloat light_position[] = { 0.0, 10.0, 0.0, 1.0 };
	// po³o¿enie Ÿród³a

	GLfloat light_position1[] = { 10.0, 0.0, 0.0, 1.0 };
	// po³o¿enie Ÿród³a

	GLfloat light_ambient[] = { 1.0, 0.0, 0.0, 0.5 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 0.0, 0.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat light_ambient1[] = { 0.0, 0.0, 1.0, 0.5 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse1[] = { 0.0, 0.0, 1.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular1[] = { 0.0, 0.0, 1.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0 };
	// sk³adowa sta³a ds dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	GLfloat att_linear = { 0.05 };
	// sk³adowa liniowa dl dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	GLfloat att_quadratic = { 0.001 };
	// sk³adowa kwadratowa dq dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	// Ustawienie parametrów materia³u i Ÿród³a œwiat³a

	// Ustawienie patrametrów materia³u

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	// Ustawienie parametrów Ÿród³a

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

	// Ustawienie opcji systemu oœwietlania sceny

	glShadeModel(GL_SMOOTH); // w³aczenie ³agodnego cieniowania
	glEnable(GL_LIGHTING);   // w³aczenie systemu oœwietlenia sceny
	glEnable(GL_LIGHT0);     // w³¹czenie Ÿród³a o numerze 0
	glEnable(GL_LIGHT1);     // w³¹czenie Ÿród³a o numerze 1
	glEnable(GL_DEPTH_TEST); // w³¹czenie mechanizmu z-bufora

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
	glutCreateWindow("Lab 4 - Light");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(keys);
	glutMainLoop();
}
