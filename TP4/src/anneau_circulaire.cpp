///////////////////////////////////////////////////////////////////////////////
// affichage d'un demi-anneau circulaire
//   
//    Nicolas SZAFRAN - UFR-IM2AG - 2015	 

#include <GL/glut.h>
#include <cmath>


///////////////////////////////////////////////////////////////////////////////
// les ordres graphiques 
void dessin() 
{
	int M=20;
	int i;

	// initialiser les transformations géométriques dans le repère scène
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// effacer le buffer graphique 
	glClear(GL_COLOR_BUFFER_BIT);
	
	// discrétisation du demi-anneau de centre (0,0) et rayons r1 et r2
	// sous forme d'un polygone formé de M quadrangles 
	glColor3d(0.0, 0.5, 1.0);
	glBegin(GL_TRIANGLES);
	for (i=0; i<M; i++)
	{
		GLdouble a,x,y,z,r1,r2;
		r1 = 10.0;
		r2 = 25.0;
		a = 1.0*i*M_PI/(M-1.0);
		x = r1*cos(a);
		y = r1*sin(a);
		z = 0.0;
		glVertex3d(x,y,z);
		x = r2*cos(a);
		y = r2*sin(a);
		z = 0.0;
		glVertex3d(x,y,z);
	}
	glEnd();

	// tracé du contour du demi-anneau en blanc 
	glColor3d(1.0, 1.0, 1.0);
	glLineWidth(3.0);
	
	glBegin(GL_LINE_STRIP);
	
	// partie interne
	for (i=0; i<M; i++)
	{
		GLdouble a,x,y,z,r1;
		r1 = 10.0;
		a = 1.0*i*M_PI/(M-1.0);
		x = r1*cos(a);
		y = r1*sin(a);
		z = 0.0;
		glVertex3d(x,y,z);
	}
	
	// partie externe
	for (i=M-1; i>=0; i--)
	{
		GLdouble a,x,y,z,r2;
		r2 = 25.0;
		a = 1.0*i*M_PI/(M-1.0);
		x = r2*cos(a);
		y = r2*sin(a);
		z = 0.0;
		glVertex3d(x,y,z);
	}
	glEnd();

	// mettre à jour l'affichage
	glFlush();
}

///////////////////////////////////////////////////////////////////////////////
// prise en compte d'un redimensionnement de la fenetre 
void redimensionnement(int w, int h)
{
	// met à jour le rectangle image suivant les dimensions de la fenetre
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	
	// initialiser les transformations géométriques dans le repère caméra	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// projection orthographique 
	glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
}

///////////////////////////////////////////////////////////////////////////////
// la fonction principale 
int main(int argc, char **argv) 
{
	// initialisation de la librairie GLUT
	// doit etre appelee avant toute autre instruction 
	// de la librairie GLUT 
	glutInit(&argc,argv);
	
	// creation d'une fenetre graphique 
	// de dimension 500 x 500
	// en simple buffer et mode RGB
	// dont le nom est celui du programme 
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow(argv[0]);
	
	// definition de la fonction callback de dessin 
	glutDisplayFunc(dessin);
	// definition de la fonction callback de redimensionnement de la fenetre 
	glutReshapeFunc(redimensionnement);
	
	// boucle principale 
	glutMainLoop();
	
	return(0);
}
