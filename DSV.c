#include <GL/glut.h>
#include <math.h>

#define N 48
#define X 8
#define Y 6

int a = 0;
int p[N][2];
int o[5] = {1, 43, 20, 12, 34};
/*the above array should be a randomized sequence of non-repeating,
	non-negative integers from 0 to N inclusive*/

GLfloat red[3] = {1, 0, 0};
GLfloat green[3] = {0, 1, 0};
GLfloat blue[3] = {0, 0, 1};

void printStr(int x, int y, GLfloat* color, char *str)
{
	int i;
	glColor3fv(color);
	glRasterPos2f(x, y);
	for(i = 0 ; str[i] != '\0' ; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
	}
}

void printNum(int x, int y, GLfloat* color, int num)
{
	int i, length, rev;
	glColor3fv(color);
	glRasterPos2f(x, y);

	if(num < 10)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '0'+num);
		return;
	}

	length = (int)(log10(num))+1;
	for(i = 0 ; i < length ; i++)
	{
		rev = (rev * 10) + (num % 10);
		num /= 10;
	}

	for(i = 0 ; i < length ; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (rev%10)+'0');
		rev /= 10;
	}
}
void newSquare(int x, int y)
{
	glColor3fv(red);
	glBegin(GL_POLYGON);
	glVertex2f(x+25, y+25);
	glVertex2f(x+75, y+25);
	glVertex2f(x+75, y+75);
	glVertex2f(x+25, y+75);
	glEnd();

	glColor3fv(blue);
	glBegin(GL_POLYGON);
	glVertex2f(x+75, y+25);
	glVertex2f(x+85, y+25);
	glVertex2f(x+85, y+75);
	glVertex2f(x+75, y+75);
	glEnd();
}

void grid()
{
	int i;
	glBegin(GL_LINES);
	for(i = 0 ; i < 600 ; i += 100)
	{
		glVertex2f(0, i);
		glVertex2f(800, i);
	}
	for(i = 0 ; i < 900 ; i += 100)
	{
		glVertex2f(i, 0);
		glVertex2f(i, 600);
	}
	glEnd();
	for(i = 0 ; i < N ; i++)
	{
		printNum(p[i][0]+5, p[i][1]+5, red, i);
	}

}

void display()
{
	int i;
	int lastx, lasty;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 0);
	grid();
	for(i = 0 ; i < a ; i++)
	{
		newSquare(p[o[i]][0], p[o[i]][1]);
		if(i != 0)
		{
			glBegin(GL_LINES);
			glVertex2f(lastx+85, lasty+50);
			glVertex2f(p[o[i]][0]+50, p[o[i]][1]+50);
			glEnd();
		}
		printNum(p[o[i]][0]+50, p[o[i]][1]+50, green, i);
		glColor3f(1, 0, 0);
		lastx = p[o[i]][0];
		lasty = p[o[i]][1];
	}

	glFlush();
}

void mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		a++;
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if(a > 0)
		{
			a--;
		}
	}
}

void main(int argc, char* argv[])
{
	int i, j;
	for(i = 0 ; i < Y ; i++)
	{
		for(j = 0 ; j < X ; j++)
		{
			p[i*X+j][0] = j*100;
			p[i*X+j][1] = i*100; 
		}
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("CG project");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 600);
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
}