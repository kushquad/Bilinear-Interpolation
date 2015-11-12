#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#include<string.h>
#include<unistd.h>

float xa, ya;
float xb, yb;
float faa, fab, fba, fbb;


//OpenGL initialization for 2D drawing
void myInit()
{ 
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 900.0, 0.0, 600.0);
	glMatrixMode(GL_MODELVIEW);
}

//Bilinear interpolation is composed of two linear interpolations
// i) Linear interpolation on x between xa and xb, using endpoint values
// ii) Linear interpolation on y between ya and yb, using generic x values from i)
float computeBilinearInterpolation(float x, float y)
{
	float term1 = faa*(xb-x)*(yb-y);
	float term2 = fba*(x-xa)*(yb-y);
	float term3 = fab*(xb-x)*(y-ya);
	float term4 = fbb*(x-xa)*(y-ya);
	return (term1+term2+term3+term4)/((xb-xa)*(yb-ya));
}

//We attempt to achieve a interpolation effect similar to
//https://en.wikipedia.org/wiki/File:Bilininterp.png

void initBilinearConstants()
{
	xa = 0.0;
	ya = 0.0;
	xb = 1.0;
	yb = 1.0;
	
	faa = 0.0;
	fab = 1.0;
	fba = 1.0;
	fbb = 0.5;
}

//Custom distribution of color
void setCustomColor(float val)
{
	float red = (val>=0.65)?(val/0.65):(0);
	float green = val;
	float blue = (val<=0.35)?(val/0.35):(0);
	
	//Cel-shading
	glColor3f((int)(red*10)/10.0, (int)(green*10)/10.0, (int)(blue*10)/10.0);
}


//Simple linear distribution of color
void setLinearColor(float val)
{
	
	float red = val;
	float green = (val>=0.5)?(2-2*val):2*val;
	float blue = 1-val;
	//Normal shading
	//glColor3f(red,green,blue);
	//Cel-shading
	glColor3f((int)(red*10)/10.0, (int)(green*10)/10.0, (int)(blue*10)/10.0);
}

//Simple squared distribution of color
void setSquaredColor(float val)
{	
	float red = 1.0-((val-1)*(val-1)/(11.0/16.0));
	float green = 1.0-((val-0.5)*(val-0.5)/(11.0/32.0));
	float blue = 1.0-((val-0)*(val-0)/(11.0/16.0));
	glColor3f(red, green, blue);
}

//Main display function used by OpenGL for rendering scene
void display(void) 
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POINTS);
	int i,j;
	for(i=0;i<400;i++)
		for(j=0;j<400;j++)
		{
			float z = computeBilinearInterpolation(i/400.0, j/400.0);
			setCustomColor(z);
			glVertex2i(i,j);
			setLinearColor(z);
			glVertex2i(500+i,j);
		}
	glEnd();

	glutSwapBuffers();
}


int main(int argc, char* argv[])
{
	//OpenGL functionality
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(900,600);
	glutCreateWindow("Bilinear Interpolation");
	myInit();

	initBilinearConstants();
	//Main rendering here
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMainLoop();	
	return 0;
}
