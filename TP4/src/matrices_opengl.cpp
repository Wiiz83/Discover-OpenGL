///////////////////////////////////////////////////////////////////////////////
//
//   visualisation des piles de matrices
//   
//    compilation
//      g++ matrices_OpenGL.c -lglut -lGLU -lGL -o matrices_OpenGL
// 	 
//    Nicolas SZAFRAN - 2016	 
//
///////////////////////////////////////////////////////////////////////////////

#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

bool visu_pile_projection = false;
bool visu_pile_modelview = true;

///////////////////////////////////////////////////////////////////////////////
// partie gestion des piles de matrices
///////////////////////////////////////////////////////////////////////////////

// definition des modes pour l'affichage des matrices 
#define MODE_XYZW false // toutes les composantes 
#define MODE_XYW  true  // pas la composante Z 

// ecrit à l'ecran une matrice 4x4 définie par un tableau M de 16 double *
// M(i,j) = M[i+4*j] 
void ecrire_matrice(GLdouble *M)
{
	int i,j;
	for (i=0; i<4; i++)
	{
		printf("|");
		for (j=0; j<4; j++)
			printf(" %10.5f", M[i+4*j]);
		printf(" |\n");
	}
	printf("\n");
}

// ecrit à l'ecran une matrice 3x3 définie par un tableau M de 16 double *
// M(i,j) = M[i+4*j] 
// la composante en z n'est pas affichée
void ecrire_matrice_xy(GLdouble *M)
{
	int i,j;
	for (i=0; i<4; i++)
	if (i!=2) 
	{
		printf("|");
		for (j=0; j<4; j++)
		if (j!=2)
			printf(" %10.5f", M[i+4*j]);
		printf(" |\n");
	}
	printf("\n");
}

// ecrit à l'ecran la matrice de transformation GL_PROJECTION 
void ecrire_matrice_projection()
{
	GLdouble M[16];
	
	printf("\n");
	printf("------------------------\n");
	printf("-- MATRICE PROJECTION --\n");
	printf("------------------------\n");
	printf("\n");
	
	glGetDoublev(GL_PROJECTION_MATRIX, M);
	printf("Matrice	GL_PROJECTION :\n");
	ecrire_matrice(M);
}

// ecrit à l'ecran la matrice de transformation GL_MODELVIEW 
void ecrire_matrice_modelview()
{
	GLdouble M[16];
	
	glGetDoublev(GL_MODELVIEW_MATRIX, M);
	printf("Matrice	GL_MODELVIEW :\n");
	ecrire_matrice(M);
}

// ecrit à l'écran la pile des matrices de transformation GL_MODELVIEW 
// si ind2d est VRAI, la composante en z n'est pas affichée pour       
// les différentes matrices                                            
void ecrire_pile_matrices_modelview(bool ind2d = true)
{
	GLint mode_courant;
	GLint nb_max, nb;
	GLdouble *TM;
	GLint i;
	
	printf("\n");
	printf("--------------------------------\n");
	printf("-- PILE DE MATRICES MODELVIEW --\n");
	printf("--------------------------------\n");
	
	// récupérer le mode courant 
	glGetIntegerv(GL_MATRIX_MODE, &mode_courant);
	
	// passer en mode MODELVIEW 
	glMatrixMode(GL_MODELVIEW);
	
	// récupérer la taille maximale de la pile 
	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &nb_max);
	printf("  La taille max. de la pile GL_MODELVIEW est %d\n", nb_max);
	
	// récupérer la taille de la pile 
	glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &nb);
	printf("  La taille de la pile GL_MODELVIEW est %d\n", nb);
	
	// créer un tableau annexe pour stocker les matrices 
	TM = (GLdouble*)malloc(16*nb*sizeof(GLdouble));
	
	// récupérer les matrices en les dépilant et les stockant dans TM 
	// récupérer la matrice en sommet de pile 
	glGetDoublev(GL_MODELVIEW_MATRIX, &TM[0]); 
	for (i=1; i<nb; i++)
	{
		glPopMatrix(); // dépiler 
		// récupérer la matrice en sommet de pile 
		glGetDoublev(GL_MODELVIEW_MATRIX, &TM[i*16]); 
	}
	
	// afficher la pile de matrices 
	printf("\nPile des matrices : \n");
	for (i=0; i<nb; i++)
	if (ind2d)
		ecrire_matrice_xy(&TM[i*16]); 
	else
		ecrire_matrice(&TM[i*16]); 
	
	// remettre les matrices dans la pile 
	for (i=nb-2; i>=0; i--)
	{
		glPushMatrix(); // dupliquer le sommet de la pile 
		// remplacer la matrice en sommet de pile 
		// par celle stocker dans TM 
		glLoadMatrixd(&TM[i*16]); 
	}
	
	// libérer la mémoire allouée pour TM 
	free((void*)TM);	
	
	// repasser dans le mode courant 
	glMatrixMode(mode_courant);
}

// initialiser la pile des matrices de transformation GL_MODELVIEW 
void initialiser_pile_matrices_modelview()
{
	GLint mode_courant;
	GLint nb;
	GLint i;
	
	// récupérer le mode courant 
	glGetIntegerv(GL_MATRIX_MODE, &mode_courant);
	
	// passer en mode MODELVIEW 
	glMatrixMode(GL_MODELVIEW);
	
	// récupérer la taille de la pile 
	glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &nb);
	
	// dépiler les matrices sauf la dernière 
	for (i=1; i<nb; i++)
	{
		glPopMatrix(); // dépiler 
	}
	
	// initialiser avec la matrice identité 
	glLoadIdentity();
	
	// repasser dans le mode courant 
	glMatrixMode(mode_courant);
}

// ecrit à l'écran la pile des matrices de transformation GL_PROJECTION 
// si ind2d est VRAI, la composante en z n'est pas affichée pour       
// les différentes matrices                                            
void ecrire_pile_matrices_projection(bool ind2d = true)
{
	GLint mode_courant;
	GLint nb_max, nb;
	GLdouble *TM;
	GLint i;
	
	// récupérer le mode courant 
	glGetIntegerv(GL_MATRIX_MODE, &mode_courant);
	
	// passer en mode PROJECTION 
	glMatrixMode(GL_PROJECTION);
	
	// récupérer la taille maximale de la pile 
	glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &nb_max);
	printf("La taille max. de la pile GL_PROJECTION est %d\n", nb_max);
	
	// récupérer la taille de la pile 
	glGetIntegerv(GL_PROJECTION_STACK_DEPTH, &nb);
	printf("La taille de la pile GL_PROJECTION est %d\n", nb);
	
	// créer un tableau annexe pour stocker les matrices 
	TM = (GLdouble*)malloc(16*nb*sizeof(GLdouble));
	
	// récupérer les matrices en les dépilant et les stockant dans TM 
	// récupérer la matrice en sommet de pile 
	glGetDoublev(GL_PROJECTION_MATRIX, &TM[0]); 
	for (i=1; i<nb; i++)
	{
		glPopMatrix(); // dépiler 
		// récupérer la matrice en sommet de pile 
		glGetDoublev(GL_PROJECTION_MATRIX, &TM[i*16]); 
	}
	
	// afficher la pile de matrices 
	printf("\nPile des matrices : \n");
	for (i=0; i<nb; i++)
	if (ind2d)
		ecrire_matrice_xy(&TM[i*16]); 
	else
		ecrire_matrice(&TM[i*16]); 
	
	// remettre les matrices dans la pile 
	for (i=nb-2; i>=0; i--)
	{
		glPushMatrix(); // dupliquer le sommet de la pile 
		// remplacer la matrice en sommet de pile 
		// par celle stocker dans TM 
		glLoadMatrixd(&TM[i*16]); 
	}
	
	// libérer la mémoire allouée pour TM 
	free((void*)TM);	
	
	// repasser dans le mode courant 
	glMatrixMode(mode_courant);
}

// initialiser la pile des matrices de MODELVIEW 
void init_modelview()
{
	if (visu_pile_modelview)
	{
		printf("\ninitialisation de la pile de matrices MODELVIEW\n");
		printf("-----------------------------------------------\n");
		printf("glMatrixMode(GL_MODELVIEW);\n");
	}	
	glMatrixMode(GL_MODELVIEW);
	if (visu_pile_modelview)
	{
		printf("glLoadIdentity();\n");
	}
	glLoadIdentity();
	if (visu_pile_modelview)
	{
		ecrire_pile_matrices_modelview(MODE_XYZW);
	}
}

// initialiser la pile des matrices de PROJECTION 
void init_projection()
{
printf("glMatrixMode(GL_MODELVIEW);\n");
	if (visu_pile_projection)
	{
		printf("\ninitialisation de la pile de matrices PROJECTION\n");
		printf("------------------------------------------------\n");
		printf("glMatrixMode(GL_PROJECTION);\n");
	}	
	glMatrixMode(GL_PROJECTION);
	if (visu_pile_projection)
	{
		printf("glLoadIdentity();\n");
	}
	glLoadIdentity();
	if (visu_pile_projection)
	{
		ecrire_matrice_projection();
	}
}

///////////////////////////////////////////////////////////////////////////////
// les ordres graphiques 
///////////////////////////////////////////////////////////////////////////////

// dessiner la grille du repère Oxy de la scène
void dessin_grille()
{
	glColor3d(0.6,0.6,0.6); // gris 0,6

	// la grille unitaire
	glLineWidth(1.0);       // épaisseur 1
	glBegin(GL_LINES);
		for (int x=-5; x<=5; x++)
		{
			glVertex2d(x,-5);
			glVertex2d(x, 5);
		}
		for (int y=-5; y<=5; y++)
		{	
			glVertex2d(-5, y);
			glVertex2d( 5, y);
		}
	glEnd();
	
	// les deux axes
	glLineWidth(2.0);       // épaisseur 2
	glBegin(GL_LINES);
		glVertex2d(0,-5);
		glVertex2d(0, 5);
		glVertex2d(-5, 0);
		glVertex2d( 5, 0);
	glEnd();
	
}

// dessiner l'objet
void dessin_objet()
{
	glColor3d(1.0,0.0,0.0); // rouge
	glLineWidth(3.0);       // épaisseur 3
	glBegin(GL_LINE_LOOP);
		glVertex2d( 4.0/8.0+2.0, 3.0/8.0+1.0);
		glVertex2d( 2.0/8.0+2.0,-2.0/8.0+1.0);
		glVertex2d( 1.0/8.0+2.0,-2.0/8.0+1.0);
		glVertex2d( 1.0/8.0+2.0,-3.0/8.0+1.0);
		glVertex2d( 3.0/8.0+2.0,-3.0/8.0+1.0);
		glVertex2d( 3.0/8.0+2.0,-4.0/8.0+1.0);

		glVertex2d(-3.0/8.0+2.0,-4.0/8.0+1.0);
		glVertex2d(-3.0/8.0+2.0,-3.0/8.0+1.0);
		glVertex2d(-1.0/8.0+2.0,-3.0/8.0+1.0);
		glVertex2d(-1.0/8.0+2.0,-2.0/8.0+1.0);
		glVertex2d(-2.0/8.0+2.0,-2.0/8.0+1.0);
		glVertex2d(-4.0/8.0+2.0, 3.0/8.0+1.0);
	glEnd();
}

void dessin() {
	
	// initialisation de la pile de matrices MODELVIEW 
	init_modelview();
	
	// effacer le buffer graphique avec la couleur blanche 
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	if (visu_pile_modelview)
	{	
		printf("\n\n");
		printf("########################\n");
		printf("## Dessin de la scene ##\n");
		printf("########################\n");
	}
		
	// effectuer les ordres graphiques 
	dessin_grille();
	
	glPushMatrix();
		glTranslated( 2.0, 1.0,0.0);
		glRotated(90.0,0.0,0.0,1.0);
		glTranslated(-2.0,-1.0,0.0);
		glPushMatrix();
			dessin_objet();
		glPopMatrix();
		if (visu_pile_modelview)
		{	
			ecrire_pile_matrices_modelview(MODE_XYZW);
		}	
	glPopMatrix();

	glFlush();
}

// prise en compte d'un redimensionnement de la fenetre 
void redimensionnement(int w, int h)
{
	// initialiser la pile des matrices de PROJECTION 
	init_projection();
	
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);

	if (visu_pile_projection)
	{
		printf("glOrtho(-5.0, 5.0, -5.0, 5.0, -1.0, 1.0);\n");
	} 
	glOrtho(-5.0, 5.0, -5.0, 5.0, -1.0, 1.0);
	if (visu_pile_projection)
	{
		ecrire_matrice_projection();
	}
}

///////////////////////////////////////////////////////////////////////////////
// la fonction principale 
///////////////////////////////////////////////////////////////////////////////

int main(int argc,char **argv) 
{
	// initialisation de la librairie GLUT
	//   doit etre appelee avant toute autre instruction 
	//   de la librairie GLUT 
	glutInit(&argc,argv);
	
	// creation d'une fenetre graphique 
	//   de dimension 500 x 500
	//   en simple buffer et mode RGB
	//   dont le nom est celui du programme 
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow(argv[0]);
	
	// definition du callback de dessin 
	glutDisplayFunc(dessin);
	// definition du callback de redimensionnement 
  	glutReshapeFunc(redimensionnement);
	
	// boucle principale 
	glutMainLoop();
	
	return(0);
}
