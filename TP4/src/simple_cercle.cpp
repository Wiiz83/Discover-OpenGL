///////////////////////////////////////////////////////////////////////////////
// affichage d'un simple cercle vert 
//   
//    compilation
//      g++ simple_cercle.cpp -lglut -lGLU -lGL -o simple_cercle
// 	 
//    Nicolas SZAFRAN - UFR IM2AG - 2015	 
///////////////////////////////////////////////////////////////////////////////

#include <GL/glut.h>
#include <cmath>

///////////////////////////////////////////////////////////////////////////////
// les ordres graphiques 
void dessin() 
{
	int N=100;

	// initialiser les transformations géométriques dans le repère scène
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// effacer le buffer graphique 
	glClear(GL_COLOR_BUFFER_BIT);
	
	// discrétisation du cercle de centre (0,0) et rayon r
	// sous forme d'un polygone régulier à N sommets 
	glColor3d(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	for (int i=0; i<N; i++)
	{
		GLdouble a,x,y,z,r;
		r = 25.0;
		a = 2.0*i*M_PI/N;
		x = r*cos(a);
		y = r*sin(a);
		z = 0.0;
		glVertex3d(x,y,z);
	}
	glEnd();

	// tracé du contour du cercle en blanc 
	glColor3d(1.0, 1.0, 1.0);
	glLineWidth(3.0);
	glBegin(GL_LINE_LOOP);
	for (int i=0; i<N; i++)
	{
		GLdouble a,x,y,z,r;
		r = 25.0;
		a = 2.0*i*M_PI/N;
		x = r*cos(a);
		y = r*sin(a);
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
	if(w > h){
		glOrtho(-50.0*((float)w/(float)h), 50.0*((float)w/(float)h), -50.0, 50.0, -1.0, 1.0);
	}
	else if(w == h){
		glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
	}
	else{
		glOrtho(-50.0, 50.0, -50.0*((float)h/(float)w), 50.0*((float)h/(float)w), -1.0, 1.0);
	}
	
}

///////////////////////////////////////////////////////////////////////////////
// la fonction principale 
int main(int argc,char **argv) 
{
	// initialisation de la librairie GLUT
	//   doit etre appelee avant toute autre instruction de la librairie GLUT *
	glutInit(&argc,argv);
	
	// creation d'une fenetre graphique 
	//   de dimension 500 x 500
	//   en simple buffer et mode RGB
	//   dont le nom est celui du programme 
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow(argv[0]);
	
	// definition de la fonction de dessin 
	glutDisplayFunc(dessin);
	// definition de la fonction de redimensionnement de la fenetre 
	glutReshapeFunc(redimensionnement);
	
	// boucle principale 
	glutMainLoop();
	
	return(0);
}
