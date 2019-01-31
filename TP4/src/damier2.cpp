///////////////////////////////////////////////////////////////////////////////
// dessin d'un damier avec des pièces 
//    
//    le déplacement de la caméra suivant son axe de visee (distance par rapport a
//    l'origine) se fait a l'aide des touches c et v 
// 	   
//    le changement de la valeur de l'azimut de la camera se fait à l'aide des
//    touches j et l
//    	  
//    le changement de la valeur de l'elevation de la camera se fait à l'aide des
//    touches i et k
//    	  
//    le changement de l'angle d'ouverture de la camera se fait à l'aide des
//    touches - et +
//    	  
//    la touche q ou ESC met fin a l'execution du programme
//    	   
//    compilation
//      g++ damier2.cpp -lglut -lGLU -lGL -o damier2
// 	 
//    Nicolas SZAFRAN - UFRIM2AG - 2015	 
//  
///////////////////////////////////////////////////////////////////////////////

#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

// angle d'ouverture de la camera (exprime en degres) 
GLdouble angle_camera = 45.0;

// position en coordonnées sphériques de la caméra 
GLdouble distance = 10.0; // distance caméra - axe Oz 
GLdouble azimut   = 45.0; // azimut (angle en degrés) 
GLdouble hauteur  = 10.0; // hauteur / plan {z=0} 
GLdouble beta = 1.0;

GLdouble ratioFenetre; // rapport largeur sur hauteur 


//***************************************************************************
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
#define H_PION 0.3
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
	x_case = 9-((n-1)%5)*2-(y_case%2);
	 
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

void positionnement_camera()
{
	GLdouble Cx,Cy,Cz; // position de la camera 
	GLdouble vx,vy,vz; // vecteur de vue vertical 
	GLdouble vbx, vby, vbz;
	GLdouble ux, uy, uz;
	GLdouble R = sqrt(distance*distance+hauteur*hauteur);
	GLdouble a = azimut/180.0*M_PI; // l'angle azimut exprimé en radians 

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle_camera, ratioFenetre, 0.1, 10000.0); 
    
	// calcul de la position de la camera 
	Cx = distance*cos(a);
	Cy = distance*sin(a);
	Cz = hauteur;
	 
	// calcul du vecteur vertical de vue 
	vx = -hauteur*cos(a)/R;
	vy = -hauteur*sin(a)/R;
	vz = distance/R;

	ux = -sin(azimut);
	uy = cos(azimut);
	uz = 0;

	vbx = vx * cos(beta) + ux * sin(beta);
	vby = vy * cos(beta) + uy * sin(beta);
	vbz = vz * cos(beta) + uz * sin(beta);


	 
	gluLookAt(Cx, Cy, Cz,
	          0.0,0.0,0.0,
			  vbx, vby, vbz);
}

// prise en compte d'un redimensionnement de la fenetre 
void redimensionnement(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	ratioFenetre = (double)(w)/(double)(h);
	positionnement_camera();
}

// gestion des touches clavier 
void clavier(unsigned char key, int x, int y) 
{ 
	switch(key) 
	{ 
	
	// augmentation de la distance camera - axe vertical Oz 
	case 'c' :  
	case 'C' :
		distance++;
		positionnement_camera();
		glutPostRedisplay();
		break; 
	
	// réduction de la distance camera - axe vertical Oz  
	// telle que la distance soit toujours supérieure à 1 
	case 'v' :  
	case 'V' :
		if(distance>1)
		{
			distance--;
			positionnement_camera();
			glutPostRedisplay();
		}
		
		break; 
	
	// augmentation de l'angle d'azimut 
	case 'j' :  
	case 'J' :
		azimut++;
		positionnement_camera();
		glutPostRedisplay();
		break; 
	
	// diminution de l'angle d'azimut 
	case 'l' :  
	case 'L' :
		azimut--;
		positionnement_camera();
		glutPostRedisplay();
		break; 
	
	// augmentation de la hauteur 
	case 'i' :  
	case 'I' :
		hauteur++;
		positionnement_camera();
		glutPostRedisplay();
		break; 
	
	// diminution de la hauteur 
	case 'k' :  
	case 'K' :
		hauteur--;
		positionnement_camera();
		glutPostRedisplay();
		break; 

	case 'b':
	case 'B':
		beta += 0.1;
		positionnement_camera();
		glutPostRedisplay();
		break; 

	case 'n':
	case 'N':
		beta -= 0.1;
		positionnement_camera();
		glutPostRedisplay();
		break; 	
	// augmentation de l'angle d'ouverture de la camera 
	case '+' :
		if (angle_camera<90.0)
			angle_camera *= 1.01;
		else
			angle_camera = 180.0-(180.0-angle_camera)/1.01;
		positionnement_camera();
		glutPostRedisplay();	
		break; 

	// diminution de l'angle d'ouverture de la camera 
	case '-' :
		if (angle_camera<90.0)
			angle_camera /= 1.01;
		else
			angle_camera = 180.0-(180.0-angle_camera)*1.01;
		positionnement_camera();
		glutPostRedisplay();	
		break; 
	
	// fin du programme 
	case 27 :  
	case 'q' :  
	case 'Q' :  
		exit(0); 
		break; 
	 
	}
}

// la routine principale 
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
