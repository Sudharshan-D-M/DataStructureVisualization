#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#define N 48
#define X 8
#define Y 6
#define WIDTH 800
#define HEIGHT 600
#define SIDE 100

struct node
{
	int value;
	int xx;
	int yy;
	struct node* next;
};
typedef struct node node;

node* head = NULL;
int a = 1;
int p[X*Y][2];
int o[X][Y];

GLfloat red[3] = {1, 0, 0};
GLfloat green[3] = {0, 1, 0};
GLfloat blue[3] = {0, 0, 1};
GLfloat yellow[3] = {1, 1, 0};

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
	rev = 0;
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

void newSquare(int x, int y, int last)
{
	glColor3fv(red);
	glBegin(GL_POLYGON);
	glVertex2f(x+SIDE/4, y+SIDE/4);
	glVertex2f(x+3*SIDE/4, y+SIDE/4);
	glVertex2f(x+3*SIDE/4, y+3*SIDE/4);
	glVertex2f(x+SIDE/4, y+3*SIDE/4);
	glEnd();

	if(last == 0)
	{
		glColor3fv(green);
	}
	else
	{
		glColor3fv(blue);
	}
	glBegin(GL_POLYGON);
	glVertex2f(x+3*SIDE/4, y+SIDE/4);
	glVertex2f(x+3*SIDE/4+SIDE/8, y+SIDE/4);
	glVertex2f(x+3*SIDE/4+SIDE/8, y+3*SIDE/4);
	glVertex2f(x+3*SIDE/4, y+3*SIDE/4);
	glEnd();
}

void grid()
{
	int i;
	glBegin(GL_LINES);
	for(i = 0 ; i < HEIGHT ; i += SIDE)
	{
		glVertex2f(0, i);
		glVertex2f(WIDTH, i);
	}
	for(i = 0 ; i < WIDTH+SIDE ; i += SIDE)
	{
		glVertex2f(i, 0);
		glVertex2f(i, HEIGHT);
	}
	glEnd();
	for(i = 0 ; i < X*Y ; i++)
	{
		printNum(p[i][0]+SIDE/20, p[i][1]+SIDE/20, red, i);
	}
}

void addNode(int new_value, int x, int y)
{
	node* traveller;
	node* new_node = (node*)malloc(sizeof(struct node));
	new_node->value = new_value;
	new_node->xx = x;
	new_node->yy = y;
	new_node->next = NULL;

	if(head == NULL)
	{
		head = new_node;
		return;
	}

	for(traveller = head;
		traveller->next != NULL;
		traveller = traveller->next);

	traveller->next = new_node;
}

void deleteNode(int node_value)
{
	node* traveller;
	node* previous = NULL;

	if(head == NULL)
	{
		return;
	}

	if(head->next == head)
	{
		free(head);
		head = NULL;
		return;
	}

	if(head->value == node_value)
	{
		previous = head;
		head = head->next;
		free(previous);
		return;
	}

	previous = head;
	for(traveller = head->next;
		traveller != NULL;
		traveller = traveller->next)
	{
		if(traveller->value == node_value)
		{
			previous->next = traveller->next;
			free(traveller);
			return;
		}
		previous = traveller;
	}
}

void display()
{
	node* traveller;
	node* previous = NULL;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3fv(red);
	grid();

	for(traveller = head;
		traveller != NULL;
		traveller = traveller->next)
	{
		if(traveller->next == NULL)
		{
			newSquare(traveller->xx, traveller->yy, 0);
		}
		else
		{
			newSquare(traveller->xx, traveller->yy, 1);
		}

		if(previous != NULL)
		{
			glColor3fv(blue);
			glBegin(GL_LINES);
			glVertex2f(previous->xx + 3*SIDE/4+SIDE/8, previous->yy + SIDE/2);
			glVertex2f(traveller->xx + SIDE/2, traveller->yy + SIDE/2);
			glEnd();
		}
		printNum(traveller->xx + SIDE/2, traveller->yy + SIDE/2, yellow, traveller->value);
		previous = traveller;
	}
	glFlush();
}

void mouse(int button, int state, int x, int y)
{
	int v;
	y = HEIGHT-y;
	v = o[x/SIDE][y/SIDE];
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(v == 0)
		{
			addNode(a, x - (x%SIDE), y - (y%SIDE));
			v = a;
			a++;
		}
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if(v != 0)
		{
			deleteNode(v);
			v = 0;
		}
	}

	o[x/SIDE][y/SIDE] = v;
}

void main(int argc, char* argv[])
{
	int i, j;
	for(i = 0 ; i < Y ; i++)
	{
		for(j = 0 ; j < X ; j++)
		{
			p[i*X+j][0] = j*SIDE;
			p[i*X+j][1] = i*SIDE;
			o[X][Y] = 0; 
		}
	}
	head = NULL;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WIDTH+2*SIDE, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("CG project");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WIDTH+2*SIDE, 0, HEIGHT);
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
}