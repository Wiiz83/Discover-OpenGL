///////////////////////////////////////////////////////////////////////////////
// scene réalisée à l'aide de formes géométriques de base
//  
//   compilation
//    g++ pantins.c -lglut -lGLU -lGL -o pantins
//	 
//   Nicolas SZAFRAN - UFRIM2AG - 2015	 

#include <GL/glut.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
// les ordres graphiques 
///////////////////////////////////////////////////////////////////////////////

/////////////////
// le carre unite 
void carre()
{
	glBegin(GL_QUADS);
		glVertex3d(-1.0, -1.0, 0.0);
		glVertex3d( 1.0, -1.0, 0.0);
		glVertex3d( 1.0,  1.0, 0.0);
		glVertex3d(-1.0,  1.0, 0.0);
	glEnd();
}

//////////////////
// le cercle unite
void cercle()
{
	int i, N=100;
	glBegin(GL_POLYGON);
		for (i=0; i<N; i++)
		{
			GLdouble x,y,a;
			a = 2.0*i*M_PI/N;
			x = cos(a); 
			y = sin(a);
			glVertex3d(x, y, 0.0);
		}
	glEnd();
}

////////////////////////////////////////////
// dessine le pantin avec la couleur (r,g,b) 
// avec r,g,b 3 réels de type GLdouble
void pantin(GLdouble r, GLdouble g, GLdouble b)
{
	// utiliser la couleur (r,g,b) 
	glColor3d(r,g,b);
	
	// la tete 
	glPushMatrix();
	glTranslated(0.0, 7.5, 0.0);
	cercle();
	glPopMatrix();

	// le chapeau
	glPushMatrix();
	glColor3d(1,1,0);
	glTranslated(0.0, 8.7, 0.0);
	glScaled(sqrt(2.0), sqrt(0.5), 1.0);
	carre();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 9.2, 0.0);
	carre();
	glPopMatrix();
	
	glColor3d(r,g,b);
	// le tronc 
	glPushMatrix();
	glTranslated(0.0, 4.0, 0.0);
	glScaled(1.5, 2.5, 0.0);
	cercle();
	glPopMatrix();
	
	// la jambe gauche 
	glPushMatrix();
	glTranslated(-2.0, 1.5, 0.0);
	glRotated(45.0, 0.0, 0.0, 1.0);
	glScaled(sqrt(2.0), sqrt(0.5), 1.0);
	carre();
	glPopMatrix();
	
	// la jambe gauche 
	glPushMatrix();
	glTranslated(2.0, 1.5, 0.0);
	glRotated(-45.0, 0.0, 0.0, 1.0);
	glScaled(sqrt(2.0), sqrt(0.5), 1.0);
	carre();
	glPopMatrix();
	
	// le bras gauche 
	glPushMatrix();
	glTranslated(-2.0, 6.5, 0.0);
	glRotated(-45.0, 0.0, 0.0, 1.0);
	glScaled(sqrt(2.0), sqrt(0.5), 1.0);
	carre();
	glPopMatrix();
	
	// le bras droit 
	glPushMatrix();
	glTranslated(2.0, 5.5, 0.0);
	glRotated(0.0, 0.0, 0.0, 1.0);
	glScaled(sqrt(2.0), sqrt(0.5), 1.0);
	carre();
	glPopMatrix();


} 

////////////////////////////////////////////
// dessine la scene
void draw() 
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// effacer la fenetre avec la couleur blanc
	glClearColor(1.0,1.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);


	//////////////
	// la scene //
	
	// le sol 
	glPushMatrix();
	glColor3d(0.6,0.3,0.1);
	glTranslated(0.0, -0.5, 0.0);
	glScaled(100, 0.5, 1.0);
	carre();
	glPopMatrix();
	
	// les quatre pantins sur le sol 
	glPushMatrix();
	glTranslated(-10.5,0.0,0.0);	
	pantin(1.0,0.0,0.0);         // pantin rouge
	glTranslated(  7.0,0.0,0.0);	
	pantin(0.0,0.0,0.0);         // pantin noir
	glTranslated( -14.0,0.0,0.0);	
	pantin(0.0,0.0,1.0);         // pantin bleu
	glTranslated(  28.0,0.0,0.0);	
	pantin(1.0,0.0,1.0);         // pantin magenta
	glPopMatrix();
	
	// les deux petits pantins en l'air 
	glPushMatrix();
	glTranslated(-7.0,15.0,0.0);
	glRotated(30.0,0.0,0.0,1.0);	
	glScaled(0.7,0.7,0.7);	
	pantin(0.0,0.5,0.0);         // pantin vert foncé
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(10.0,12.0,0.0);
	glRotated(-60.0,0.0,0.0,1.0);	
	glScaled(0.5,0.5,0.5);	
	pantin(1.0,0.6,0.2);         // pantin orange
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(15.0,15.0,0.0);
	glRotated(35.0,0.0,0.0,1.0);	
	glScaled(0.5,0.5,0.5);	
	pantin(1.0,0.7,0.2);         // pantin volant
	glPopMatrix();
	
	glFlush();
}

///////////////////////////////////////////////////////////////////////////////
// prise en compte d'un redimensionnement de la fenetre 
void reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0, 25.0, -5.0, 25.0, -1.0, 1.0);
}

///////////////////////////////////////////////////////////////////////////////
// la fonction principale 
int main(int argc,char **argv) 
{
	glutInit(&argc,argv);
	glutInitWindowSize(500,300);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return(0);
}