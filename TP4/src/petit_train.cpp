///////////////////////////////////////////////////////////////////////////////
// cube 3D
//    la rotation du cube 3D se fait en utilisant les touches du clavier
//      touche x : rotation élémentaire dans le sens - autour de Ox  
//      touche X : rotation élémentaire dans le sens + autour de Ox  
//      touche y : rotation élémentaire dans le sens - autour de Oy  
//      touche Y : rotation élémentaire dans le sens + autour de Oy  
//      touche z : rotation élémentaire dans le sens - autour de Oz  
//      touche Z : rotation élémentaire dans le sens + autour de Oz
// 	   
//    le zoom se fait a l'aide des touches + (zoom avant) et - (zoom arriere)
//    
//    la touche q ou ESC met fin a l'execution du programme
//    	   
//    compilation
//      g++ cubes3d.cpp -lglut -lGLU -lGL -o cubes3d
// 	 
//    Nicolas SZAFRAN - 2011	 
//  
///////////////////////////////////////////////////////////////////////////////

#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

GLdouble delta_rot=2.0;  // modification elementaire d'angle 

///////////////////////////
// les ordres graphiques //
///////////////////////////

/* cylindre de base circulaire de rayon 1 et de hauteur 2 
   l'axe du cylindre entre les points (0,0,-1) et (0,0,1) */
#define NB_FACES_CYLINDRE 100
void cylindre()
{
	/* coordonnées du cercle unité discrétisé */
	GLdouble Cx[NB_FACES_CYLINDRE];
	GLdouble Cy[NB_FACES_CYLINDRE];
	
	int i;
	
	/* calcul du cercle unité */
	for (i=0; i<NB_FACES_CYLINDRE; i++)
	{
		GLdouble a;
		a = 2.0*i*M_PI/NB_FACES_CYLINDRE;
		Cx[i] = cos(a);
		Cy[i] = sin(a);
	}
	
	/* la base de centre (0,0,-1) */
	glColor3d(0.8,0.8,0.8);
	glBegin(GL_POLYGON);
		for (i=0; i<NB_FACES_CYLINDRE; i++)	
		{	
			glVertex3d(Cx[i], Cy[i], -1.0);
		}
	glEnd();
	/* la base de centre (0,0,1) */
	glColor3d(0.8,0.8,0.8);
	glBegin(GL_POLYGON);
		for (i=0; i<NB_FACES_CYLINDRE; i++)	
		{	
			glVertex3d(Cx[i], Cy[i],  1.0);
		}
	glEnd();
	/* le bord du cylindre */
	for (i=0; i<NB_FACES_CYLINDRE; i++)
	{
		GLdouble gris = 0.3*Cx[i]+0.7;
		int j = (i+1)%NB_FACES_CYLINDRE;
		glColor3d(gris, gris, gris);
		glBegin(GL_QUADS);
			glVertex3d(Cx[i], Cy[i], -1.0);
			glVertex3d(Cx[i], Cy[i],  1.0);
			glVertex3d(Cx[j], Cy[j],  1.0);
			glVertex3d(Cx[j], Cy[j], -1.0);
		glEnd();
	}	
	
}
   
// le cube3d unite 
#define NB_FACES_CUBE 6
void cube()
{
    // les sommets du cube 
	GLdouble sommets[][3] = {
	  {-1.0,-1.0,-1.0},
	  { 1.0,-1.0,-1.0},
	  {-1.0, 1.0,-1.0},
	  { 1.0, 1.0,-1.0},
	  {-1.0,-1.0, 1.0},
	  { 1.0,-1.0, 1.0},
	  {-1.0, 1.0, 1.0},
	  { 1.0, 1.0, 1.0},
	};
	
	// les faces du cube, chaque face décrite par ses quatre sommets 
	int faces[][4] = {
	  {0,1,3,2},
	  {5,4,6,7},
	  {0,4,5,1},
	  {6,2,3,7},
	  {0,2,6,4},
	  {3,1,5,7}
	};
	
	// les couleurs des faces 
	GLdouble couleurs[][3] = {
	  {1.0,0.5,0.5},
	  {0.0,0.5,0.5},
	  {0.5,1.0,0.5},
	  {0.5,0.0,0.5},
	  {0.5,0.5,1.0},
	  {0.6,0.3,0.1}
	};
	
	int i;
	
	// tracé des faces 
	for (i=0; i<NB_FACES_CUBE; i++)
	{
		int s0 = faces[i][0];
		int s1 = faces[i][1];
		int s2 = faces[i][2];
		int s3 = faces[i][3];
		glColor3dv(couleurs[i]);
		glBegin(GL_QUADS);
			glVertex3dv(sommets[s0]);
			glVertex3dv(sommets[s1]);
			glVertex3dv(sommets[s2]);
			glVertex3dv(sommets[s3]);
		glEnd();
	}
}

// afficher un cube unité en position (x,y,z)
void dessin_cube(GLdouble x, GLdouble y, GLdouble z)
{
	glPushMatrix();
		glTranslated(x,y,z);
		cube();
	glPopMatrix();
}

void dessin_container(GLdouble x, GLdouble y, GLdouble z){
	glPushMatrix();
		glTranslated(x,y,z);
		glScaled(4, 2.5, 1.5);
		cube();
	glPopMatrix();	
}

void dessin_roue(GLdouble x, GLdouble y, GLdouble z){
	glPushMatrix();
		glTranslated(x,y,z);
		glScaled(1, 1, 0.2);
		cylindre();
	glPopMatrix();	
}



void dessin_crochet(GLdouble x, GLdouble y, GLdouble z){
	glPushMatrix();
		glTranslated(x,y,z);
		glScaled(0.5, 0.2, 0.2);
		cube();
	glPopMatrix();	
}

void dessiner_wagon(){
	dessin_container(0.0,0.0,0.0);
	dessin_roue(-2.0,-2.5,1.7);
	dessin_roue(3.0,-2.5,1.7);
	dessin_roue(-2.0,-2.5,-1.7);
	dessin_roue(3.0,-2.5,-1.7);
	dessin_crochet(-4.0,-2.3,0.0);

}

void dessiner_base_locomotive(GLdouble x, GLdouble y, GLdouble z){
	glPushMatrix();
		glTranslated(x,y,z);
		glScaled(4.5, 0.8, 1.5);
		cube();
	glPopMatrix();	
}

void dessiner_cockpit(GLdouble x, GLdouble y, GLdouble z){
	glPushMatrix();
		glTranslated(x,y,z);
		glScaled(1.0, 1.5, 1.5);
		cube();
		glTranslated(1.0,1.0,0.0);
		glScaled(2.0, 0.2, 1.0);
		cube();
	glPopMatrix();	
}


void dessiner_cuve(GLdouble x, GLdouble y, GLdouble z){
	glPushMatrix();
		glTranslated(x,y,z);
		glRotated(90.0, 0.0, 1.0, 0.0);
		glScaled(1.5,1.5,2.5);
		cylindre();
	glPopMatrix();
}

void dessiner_chemine(GLdouble x, GLdouble y, GLdouble z){
	glPushMatrix();
		glTranslated(x,y,z);
		glRotated(90.0, 1.0, 0.0, 0.0);
		glScaled(0.15,0.15,0.3);
		cylindre();
	glPopMatrix();	
}

void dessiner_sommet_chemine(GLdouble x, GLdouble y, GLdouble z){
	glPushMatrix();
		glTranslated(x,y,z);
		glRotated(90.0, 0.0, 0.0, 1.0);
		glScaled(0.1, 0.3, 0.3);
		cube();
	glPopMatrix();	

}

void dessiner_locomotive(){
	dessiner_base_locomotive(0.0,0.0,0.0);
	dessin_roue(-3.0,-0.5,1.7);
	dessin_roue(-3.0,-0.5,-1.7);
	dessin_roue(-0.5,-0.5,1.7);
	dessin_roue(-0.5,-0.5,-1.7);
	dessin_roue(3.0,-0.5,1.7);
	dessin_roue(3.0,-0.5,-1.7);
	dessiner_cockpit(1.5, 1.5, 0.0);

	dessiner_cuve(-2.0,1.8,0.0);
	dessiner_chemine(-3.0,3.5,0.0);
	dessiner_sommet_chemine(-3.0,3.8,0.0);

}

void dessiner_train(){
	glPushMatrix();
		glTranslated(-15,0.0,0.0);
		dessiner_locomotive();
		glTranslated(9.0,1.7,0.0);
		dessiner_wagon();
		glTranslated(8.5,0.0,0.0);
		dessiner_wagon();
		glTranslated(8.5,0.0,0.0);
		dessiner_wagon();
		glTranslated(8.5,0.0,0.0);
		dessiner_wagon();
	glPopMatrix();
}
void dessin() 
{

	// effacer la fenetre avec la couleur blanche 
	glClearColor(1.0,1.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	dessiner_train();
	 
	glFlush(); 
}

// prise en compte d'un redimensionnement de la fenetre 
void redimensionnement(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20.0, 20.0, -20.0, 20.0, -20.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

// effectuer une rotation elementaire d'angle delta 
//   autour d'un des axes de l'ecran :
//   1 : axe 0x (horizontal)
//   2 : axe 0y (vertical)
//   3 : axe 0z (perpendiculaire)
void rotation(GLdouble delta, int axe)
{
	GLdouble M[16];
	
	// recuperer la matrice courante du modele -> M 
	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, M);
	
	// initialiser avec la matrice identite 
	glLoadIdentity();
	
	// effectuer la rotation 
	if (axe==1)
		glRotated(delta, 1.0, 0.0, 0.0);
	else if (axe==2)
		glRotated(delta, 0.0, 1.0, 0.0);
	else
		glRotated(delta, 0.0, 0.0, 1.0);
		
	// multiplier par la matrice M 	
	glMultMatrixd(M);
	
	// envoyer un evenement pour rafraichir l'affichage  
	glutPostRedisplay();
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
	 
	// rotations 
	case 'x':
		 rotation(-delta_rot, 1);
		 break; 
	case 'X':
		 rotation( delta_rot, 1);
		 break; 
	case 'y':
		 rotation(-delta_rot, 2);
		 break; 
	case 'Y':
		 rotation( delta_rot, 2);
		 break; 
	case 'z':
		 rotation(-delta_rot, 3);
		 break; 
	case 'Z':
		 rotation( delta_rot, 3);
		 break; 
	
	// zooms 	 
	case '-':
		glScaled(10.0/11.0,10.0/11.0,10.0/11.0);	 
		glutPostRedisplay();
		break; 
	
	case '+':
		glScaled(11.0/10.0,11.0/10.0,11.0/10.0);	 
		glutPostRedisplay();
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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
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
