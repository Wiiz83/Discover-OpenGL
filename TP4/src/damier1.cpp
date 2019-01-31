///////////////////////////////////////////////////////////////////////////////
// dessin d'un damier avec des pièces 
// 
//    la touche q ou ESC met fin a l'execution du programme
//    	   
//    compilation
//      g++ damier1.cpp -lglut -lGLU -lGL -o damier1
// 	 
//    Nicolas SZAFRAN - UFRIM2AG - 2015	 
// 
///////////////////////////////////////////////////////////////////////////////

#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

GLdouble ratio_fenetre; // rapport largeur sur hauteur 

// les ordres graphiques 

// le damier 
void damier()
{
	int i,j;
	
	// le dessus du damier 
	for (i=0; i<10; i++)
	for (j=0; j<10; j++)
	{
		if ((i+j)%2==0)
			glColor3d(0.0,0.0,0.2);
		else
			glColor3d(0.95,0.95,1.0);
		glBegin(GL_QUADS);
			glVertex3d(i*1.0    -5.0, j*1.0    -5.0, 0.0);
			glVertex3d(i*1.0+1.0-5.0, j*1.0    -5.0, 0.0);
			glVertex3d(i*1.0+1.0-5.0, j*1.0+1.0-5.0, 0.0);
			glVertex3d(i*1.0    -5.0, j*1.0+1.0-5.0, 0.0);
		glEnd();
	}
	
	// le bord du damier 
	glColor3d(0.4,0.4,0.8);
	glBegin(GL_QUADS);
		glVertex3d(-5.0,-5.0,-0.0);
		glVertex3d(-5.0, 5.0,-0.0);
		glVertex3d(-5.2, 5.2,-0.2);
		glVertex3d(-5.2,-5.2,-0.2);
	glEnd();
	glColor3d(0.45,0.45,0.9);
	glBegin(GL_QUADS);
		glVertex3d( 5.0,-5.0,-0.0);
		glVertex3d( 5.0, 5.0,-0.0);
		glVertex3d( 5.2, 5.2,-0.2);
		glVertex3d( 5.2,-5.2,-0.2);
	glEnd();
	glColor3d(0.38,0.38,0.75);
	glBegin(GL_QUADS);
		glVertex3d(-5.0,-5.0,-0.0);
		glVertex3d( 5.0,-5.0,-0.0);
		glVertex3d( 5.2,-5.2,-0.2);
		glVertex3d(-5.2,-5.2,-0.2);
	glEnd();
	glColor3d(0.43,0.43,0.85);
	glBegin(GL_QUADS);
		glVertex3d(-5.0, 5.0,-0.0);
		glVertex3d( 5.0, 5.0,-0.0);
		glVertex3d( 5.2, 5.2,-0.2);
		glVertex3d(-5.2, 5.2,-0.2);
	glEnd();
	glColor3d(0.3,0.3,0.6);
	glBegin(GL_QUADS);
		glVertex3d(-5.2,-5.2,-0.2);
		glVertex3d(-5.2, 5.2,-0.2);
		glVertex3d(-5.2, 5.2,-0.5);
		glVertex3d(-5.2,-5.2,-0.5);
	glEnd();
	glColor3d(0.4,0.4,0.8);
	glBegin(GL_QUADS);
		glVertex3d( 5.2,-5.2,-0.2);
		glVertex3d( 5.2, 5.2,-0.2);
		glVertex3d( 5.2, 5.2,-0.5);
		glVertex3d( 5.2,-5.2,-0.5);
	glEnd();
	glColor3d(0.25,0.25,0.5);
	glBegin(GL_QUADS);
		glVertex3d(-5.2,-5.2,-0.2);
		glVertex3d( 5.2,-5.2,-0.2);
		glVertex3d( 5.2,-5.2,-0.5);
		glVertex3d(-5.2,-5.2,-0.5);
	glEnd();
	glColor3d(0.35,0.35,0.7);
	glBegin(GL_QUADS);
		glVertex3d(-5.2, 5.2,-0.2);
		glVertex3d( 5.2, 5.2,-0.2);
		glVertex3d( 5.2, 5.2,-0.5);
		glVertex3d(-5.2, 5.2,-0.5);
	glEnd();
	
	// le dessous 
	glColor3d(0.15,0.15,0.3);
	glBegin(GL_QUADS);
		glVertex3d(-5.2, 5.2,-0.5);
		glVertex3d(-5.2,-5.2,-0.5);
		glVertex3d( 5.2,-5.2,-0.5);
		glVertex3d( 5.2, 5.2,-0.5);
	glEnd();
}


// pion du jeu de dames corespondant au joueur j (j=0 ou 1) 
#define NB_FACES 100
#define H_PION 0.2
void pion(int j)
{
	GLdouble c=j ? 1.0 : 0.5;
	
	// coordonnées du cercle unité discrétisé 
	GLdouble Cx[NB_FACES];
	GLdouble Cy[NB_FACES];
	
	int i;
	
	// calcul du cercle unité 
	for (i=0; i<NB_FACES; i++)
	{
		GLdouble a;
		a = 2.0*i*M_PI/NB_FACES;
		Cx[i] = cos(a);
		Cy[i] = sin(a);
	}
	
	// le dessous 
	glColor3d(0.2,0.2*c,0.2*c);
	glBegin(GL_POLYGON);
		for (i=0; i<NB_FACES; i++)	
		{	
			glVertex3d(0.35*Cx[i], 0.35*Cy[i], 0.0);
		}
	glEnd();
	
	// le dessus 
	glColor3d(0.8,0.8*c,0.8*c);
	glBegin(GL_POLYGON);
		for (i=0; i<NB_FACES; i++)	
		{	
			glVertex3d(0.35*Cx[i], 0.35*Cy[i], H_PION);
		}
	glEnd();
	
	// le bord 
	for (i=0; i<NB_FACES; i++)
	{
		GLdouble intensite = 0.3*Cx[i]+0.7;
		int j = (i+1)%NB_FACES;
		glColor3d(intensite, intensite*c, intensite*c);
		glBegin(GL_QUADS);
			glVertex3d(0.36*Cx[i], 0.36*Cy[i], 0.02);
			glVertex3d(0.36*Cx[i], 0.36*Cy[i], H_PION-0.02);
			glVertex3d(0.36*Cx[j], 0.36*Cy[j], H_PION-0.02);
			glVertex3d(0.36*Cx[j], 0.36*Cy[j], 0.02);
		glEnd();
	}	
	
	// le chanfrein du bas 
	for (i=0; i<NB_FACES; i++)
	{
		GLdouble intensite = 0.3*Cx[i]+0.5;
		int j = (i+1)%NB_FACES;
		glColor3d(intensite, intensite*c, intensite*c);
		glBegin(GL_QUADS);
			glVertex3d(0.36*Cx[i], 0.36*Cy[i], 0.02);
			glVertex3d(0.35*Cx[i], 0.35*Cy[i], 0.00);
			glVertex3d(0.35*Cx[j], 0.35*Cy[j], 0.00);
			glVertex3d(0.36*Cx[j], 0.36*Cy[j], 0.02);
		glEnd();
	}	
	
	// le chanfrein du haut 
	for (i=0; i<NB_FACES; i++)
	{
		GLdouble intensite = 0.3*Cx[i]+0.6;
		int j = (i+1)%NB_FACES;
		glColor3d(intensite, intensite*c, intensite*c);
		glBegin(GL_QUADS);
			glVertex3d(0.36*Cx[i], 0.36*Cy[i], H_PION-0.02);
			glVertex3d(0.35*Cx[i], 0.35*Cy[i], H_PION);
			glVertex3d(0.35*Cx[j], 0.35*Cy[j], H_PION);
			glVertex3d(0.36*Cx[j], 0.36*Cy[j], H_PION-0.02);
		glEnd();
	}	
	
}

// dessin d'une dame : deux pions l'un sur l'autre 
void dame(int j)
{
	pion(j);
	glPushMatrix();
	glTranslated(0.0,0.0,H_PION);
	pion(j);
	glPopMatrix();
}

// dessiner un pion du joueur j sur la case n avec n entre 1 et 50 
void dessiner_pion(int j, int n)
{
	GLint x_case, y_case;
	
	// ne rien dessiner si le numero de case est invalide 
	if (n<1 || n>50) return;
	
	y_case = (n-1)/5;
	x_case = 8-((n-1)%5)*2+(y_case%2);
	 
	glPushMatrix();
	glTranslated(x_case-4.5,y_case-4.5,0.0);
	pion(j);
	glPopMatrix();
}

// dessiner une dame du joueur j sur la case n avec n entre 1 et 50 
void dessiner_dame(int j, int n)
{
	GLint x_case, y_case;
	
	// ne rien dessiner si le numero de case est invalide 
	if (n<1 || n>50) return;
	
	y_case = (n-1)/5;
	x_case = 8-((n-1)%5)*2+(y_case%2);
	 
	glPushMatrix();
	glTranslated(x_case-4.5,y_case-4.5,0.0);
	dame(j);
	glPopMatrix();
}

void dessin() {
	glMatrixMode(GL_MODELVIEW);

	// effacer la fenetre avec la couleur blanche 
	glClearColor(1.0,1.0,1.0,0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// la scene 
	
	// le damier 
	damier();
	
	// les pieces 
	dessiner_pion(0,3);		
	dessiner_pion(0,4);		
	dessiner_pion(0,10);		
	dessiner_pion(0,11);		
	dessiner_pion(0,18);		
	dessiner_pion(0,19);		
	dessiner_pion(0,25);		
	dessiner_dame(0,46);
	dessiner_pion(1,28);		
	dessiner_pion(1,29);		
	dessiner_pion(1,34);		
	dessiner_pion(1,40);		
	dessiner_pion(1,43);		
	dessiner_pion(1,45);		
			
	glutSwapBuffers();
}

// prise en compte d'un redimensionnement de la fenetre 
void redimensionnement(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	ratio_fenetre = (double)(w)/(double)(h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, ratio_fenetre, 0.1, 10000.0);  
	//glOrtho(-10.0*ratio_fenetre, 10.0*ratio_fenetre, 
	 //      -10.0, 10.0, 
	//		0.1, 10000.0); 
    gluLookAt(-30.0,-20.0,15.0,
	          0.0,  0.0, 0.0,
			  0.0,  0.0, 1.0);
}

// gestion des touches clavier 
void clavier(unsigned char key, int x, int y) 
{ 
	switch(key) 
	{ 
	// fin du programme 
	case 27 :  
	case 'q' :  
	case 'Q' :  
		exit(0); 
		break; 
	 
	}
}

// la fonction principale 
int main(int argc,char **argv) {
	// initialisation de la librairie GLUT
	//   doit etre appelee avant toute autre instruction 
	//   de la librairie GLUT 
	glutInit(&argc,argv);
	
	// creation d'une fenetre graphique 
	//   de dimension 500 x 500
	//   en simple buffer et mode RGB
	//   dont le nom est celui du programme 
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutCreateWindow(argv[0]);
	
	// activer le z-buffer 
    glEnable(GL_DEPTH_TEST); 
	
	// definition de la routine de dessin 
	glutDisplayFunc(dessin);
	// definition de la routine de redimensionnement de la fenetre 
	glutReshapeFunc(redimensionnement);
	// definition de la routine de gestion des touches clavier
	//   (touches avec code ASCII correspondant) 
	glutKeyboardFunc(clavier);
	
	// initialisation de la matrice pour le modele 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// boucle principale 
	glutMainLoop();
	
	return(0);
}
