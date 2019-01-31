///////////////////////////////////////////////////////////////////////// 
//   
//    Calcul de surface isovaleur - Visualisation OpenGL  
// 
//    Version avec traqueboule
//   
//    Création de l'exécutable :
//  	 g++ surfaces_isovaleur_corrige.cpp -lglut -lGL -lGLU
//   
//    Exemple d'exécution - cas d'une fonction implicite
//  	 ./surfaces_isovaleur_corrige
// 
//    Exemple d'exécution - cas de données volumiques lues dans un fichier
//  	 ./surfaces_isovaleur_corrige CThead.bin 256 256 113 2 2
//  	 ./surfaces_isovaleur_corrige lobster.raw 301 324 56 1.4 1
// 
//    Nicolas SZAFRAN - UFRIM2AG - 2015
//
//////////////////////////////////////////////////////////////////////// 
 
#include <GL/gl.h> 
#include <GL/glu.h> 
#include <GL/glut.h> 
 
#include <float.h> 
#include <math.h> 
#include <stdio.h>
#include <stdlib.h>

//----------------------------
// définitions des structures  
//----------------------------
 
// point de l'espace 3D  
typedef struct _Point3D 
{ 
	float x; 
	float y; 
	float z;  
} Point3D; 
 
// facette triangulaire 3D  
typedef struct _Triangle3D 
{ 
	Point3D p[3]; 
}
Triangle3D; 

//--------------------
// variables globales  
//--------------------
 
// buffer des données volumiques 
float *buffer_vol;
unsigned long taille_buffer_vol;

// dimensions de la grille de données 
int sx, sy, sz;

// zoom pour l'affichage de l'isosurface 
float zoom;

// echelle en x,y,z 
float eX=1.0;
float eY=1.0;
float eZ=1.0;

// isoValeur  
float isoVal; 

// taille du cube elementaire 
int taille;

// buffer des triangles à afficher 
Triangle3D *lT;
unsigned long nTmax;
unsigned long nT;

// booleen pour le dessin de la grille de cubes
bool dessin_grille = false;
 		 
///////////////////////////////////////////////////////////////////////// 
//
// Module d'eclairage pour OpenGL.
//
//////////////////////////////////////////////////////////////////////// 


//--------------------
// variables globales 
//--------------------

// propriété de la matiere 
GLfloat matSpecular[]={0.0,0.0,0.0};
GLfloat matShininess[]={128.0};

// propriété de la lumière 
GLfloat lightAmbient[]={0.0,0.0,0.5,0.0};
GLfloat lightDiffuse[]={1.0,1.0,1.0,0.0};
GLfloat lightSpecular[]={1.0,1.0,1.0,1.0};

GLfloat lightPosition[]={100.0,-100.0,1000.0,0.0};
GLfloat lightSpotDirection[]={0.0,0.0,0.0};


/////////////////
//
// fonction d'initialisation de la lumière
//
////////////////
void initLumiere(void)
{
	// specifie le rendu des ombres 
	glShadeModel(GL_SMOOTH);

	// parametrage d'une lumière 
	// position de la lumière dans la scene 
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);

	// parametrage de la lumière ambiente, diffuse et speculaire 
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular);
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);

	// activation de la lumière 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}

/////////////////
//
// fonction d'initialisation des materiaux
//
////////////////
void initMatiere(void)
{
	// les faces avants et arrières des polygones 
	//   sont considérées comme des faces avants 
	glFrontFace(GL_FRONT_AND_BACK);

	// definit les propriétés du matériaux 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

	// permet de faire capter la lumière à toutes les facettes (2 cotés) 
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Partie pour la gestion d'une traqueboule                                  
// (rotation de la scene à la souris)                                        
/////////////////////////////////////////////////////////////////////////////

////////////////////////////
// inversion p=inverse de m 
////////////////////////////
void tbInverse( const GLdouble *m, GLdouble *p )
{
	{   // transposition de la rotation 
		int i,j;
		for(i=0; i<3; ++i )
			for(j=0; j<3; ++j )
				p[i+4*j] = m[j+4*i];
	}

	{   // calcul de la translation 
		double q[3]={0,0,0};
		int i,j;
		for(i=0; i<3; ++i ){
			for(j=0; j<3; ++j )
				q[i] -= p[i+4*j]*m[12+j];
		}
		p[12]=q[0]; p[13]=q[1]; p[14]=q[2];
	}

	// derniere ligne 
	p[3] = p[7] = p[11] = 0; p[15] = 1;
}

// matrices de placement de la scene par rapport a la camera 
GLdouble tb_matrix[16] =   { 1,0,0,0,
	                        0,1,0,0,
							0,0,1,0,
							0,0,0,1  };
GLdouble tb_inverse[16] =  { 1,0,0,0,
	                        0,1,0,0,
							0,0,1,0,
							0,0,0,1  };

// variables pour la gestion de la souris 
int tb_ancienX, tb_ancienY, tb_tournerXY=0, tb_translaterXY=0, tb_bougerZ=0;


/////////////////////////////////////////////////////////////////////
// lit dans la matrice courante la position initiale du point de vue 
/////////////////////////////////////////////////////////////////////
void tbInitTransform()
{
	glGetDoublev( GL_MODELVIEW_MATRIX, tb_matrix );
	tbInverse( tb_matrix, tb_inverse );
}

//////////////////////////////////////////////
// applique la transformation de point de vue 
//////////////////////////////////////////////
void tbVisuTransform()
{
  glMultMatrixd( tb_matrix );
};

////////////////////////////////////////////////
// callback de gestion des boutons de la souris 
////////////////////////////////////////////////
void tbMouseFunc( int button, int state, int x, int y )
{
  // enfoncer gauche 
  if( button==GLUT_LEFT_BUTTON && state==GLUT_DOWN ){
    tb_tournerXY = 1;
    tb_ancienX = x;
    tb_ancienY = y;
  }
  // relacher gauche 
  else if( button==GLUT_LEFT_BUTTON && state==GLUT_UP ){
    tb_tournerXY = 0;
  }
  // enfoncer milieu 
  if( button==GLUT_MIDDLE_BUTTON && state==GLUT_DOWN ){
    tb_bougerZ = 1;
    tb_ancienX = x;
    tb_ancienY = y;
  }
  // relacher milieu 
  else if( button==GLUT_MIDDLE_BUTTON && state==GLUT_UP ){
    tb_bougerZ = 0;
  }
}

///////////////////////////////////////////////////
// callback de changement de position de la souris 
///////////////////////////////////////////////////
void tbMotionFunc( int x, int y )
{
	double dx,dy,nrm, tx,ty,tz;

	if( tb_tournerXY || tb_translaterXY || tb_bougerZ )
	{
		// deplacement 
		dx = x - tb_ancienX;
		dy = tb_ancienY - y; // axe vertical dans l'autre sens 
		
		if( tb_tournerXY ){
			tx = tb_matrix[12]; tb_matrix[12]=0;
			ty = tb_matrix[13]; tb_matrix[13]=0;
			tz = tb_matrix[14]; tb_matrix[14]=0;

			nrm = sqrt( dx*dx+dy*dy+dx*dx+dy*dy );
			glLoadIdentity();
			glRotatef( nrm, -dy, dx, 0 );//axe perpendiculaire au deplacement
			glMultMatrixd( tb_matrix );
			glGetDoublev( GL_MODELVIEW_MATRIX, tb_matrix );

			tb_matrix[12] = tx;
			tb_matrix[13] = ty;
			tb_matrix[14] = tz;
		}
		else if( tb_translaterXY ){
			tb_matrix[12] += dx/100.0;
			tb_matrix[13] += dy/100.0;
		}
		else if( fabs(dx)>fabs(dy) ){ // rotation z 
			tx = tb_matrix[12]; tb_matrix[12]=0;
			ty = tb_matrix[13]; tb_matrix[13]=0;
			tz = tb_matrix[14]; tb_matrix[14]=0;

			glLoadIdentity();
			glRotatef( dx, 0,0,-1 );//axe perpendiculaire a l'ecran
			glMultMatrixd( tb_matrix );
			glGetDoublev( GL_MODELVIEW_MATRIX, tb_matrix );

			tb_matrix[12] = tx;
			tb_matrix[13] = ty;
			tb_matrix[14] = tz;
		}
		else if( fabs(dy)>fabs(dx) ){
			tb_matrix[14] -= dy/100.0;
		}
		tb_ancienX = x;
		tb_ancienY = y;
		tbInverse( tb_matrix, tb_inverse );
		glutPostRedisplay();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Fin de la partie pour la gestion d'une traqueboule                        
// (rotation de la scene à la souris)                                        
/////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////// 
//  
// calcul de la normale d'un triangle 3D
// 
/////////////////////////////////////////////////// 
Point3D calcule_normale_triangle(Triangle3D triangle)
{
	Point3D V10,V20;
	Point3D N;
	float norme;

	// triangle = [P0,P1,P1]  
	// --> calcul des vecteurs V10 = P1-P0 et V20 = P2-P0 
	V10.x=triangle.p[1].x-triangle.p[0].x;
	V10.y=triangle.p[1].y-triangle.p[0].y;
	V10.z=triangle.p[1].z-triangle.p[0].z;

	V20.x=triangle.p[2].x-triangle.p[0].x;
	V20.y=triangle.p[2].y-triangle.p[0].y;
	V20.z=triangle.p[2].z-triangle.p[0].z;


	// produit vectoriel N = V10 /\ V20 
	N.x=(V10.y*V20.z-V20.y*V10.z);
	N.y=(V10.z*V20.x-V20.z*V10.x);
	N.z=(V10.x*V20.y-V20.x*V10.y);

	// normaliser le vecteur 
	norme=sqrt(N.x*N.x+N.y*N.y+N.z*N.z);
	if (norme>1e-6)
	{
		N.x/=norme;
		N.y/=norme;
		N.z/=norme;
	}
	return N;
}


////////////////////////////// 
// 
// affichage d'un triangle 
// 
/////////////////////////////// 
static void dessine_triangle(Triangle3D triangle) 
{ 
	Point3D N=calcule_normale_triangle(triangle);
	glBegin(GL_TRIANGLES); 
	{ 
		glNormal3f(N.x,N.y,N.z);
		glVertex3f(triangle.p[0].x,triangle.p[0].y,triangle.p[0].z); 
		glNormal3f(N.x,N.y,N.z);
		glVertex3f(triangle.p[1].x,triangle.p[1].y,triangle.p[1].z); 
		glNormal3f(N.x,N.y,N.z);
		glVertex3f(triangle.p[2].x,triangle.p[2].y,triangle.p[2].z); 
	} 
	glEnd();
} 

//////////////////////////////
//
// affichage des triangles formant la surface isovaleur
//
///////////////////////////////
void dessine_surface_isovaleur()
{
	for (unsigned int i=0; i < nT; i++)
	{
		dessine_triangle(lT[i]);
	}
}



/////////////////////////////////////////////////////////////////////////
///
///	Algorithme du marching cube / tetrahedra 
///
////////////////////////////////////////////////////////////////////////

// tetraedre 3D  
typedef struct _Tetraedre
{ 
  Point3D p0,p1,p2,p3; 
  float v0,v1,v2,v3; 
} 
Tetraedre; 
		   
// découpage du cube en tetraedre 
#define NB_TETRAEDRE_CUBE 6
static int tableDesSommets[NB_TETRAEDRE_CUBE][4]={    
  {0,2,3,7},
  {0,6,1,4},
  {5,6,1,4},
  {0,2,6,7},
  {0,4,6,7},
  {0,6,1,2}
};

/////////////////
//
// fonction qui calcule l'intersection du segment [(p1,v1),(p2,v2)] avec
// l'hyperplan v=isoVal
//
////////////////
static Point3D intersection_arete_hyperplan(
  float isoVal,Point3D p1,Point3D p2,float v1,float v2)
{
  Point3D p;

  //calcul de l'interpolation 3D
  p.x=p1.x+(isoVal-v1)/(v2-v1)*(p2.x-p1.x);
  p.y=p1.y+(isoVal-v1)/(v2-v1)*(p2.y-p1.y);
  p.z=p1.z+(isoVal-v1)/(v2-v1)*(p2.z-p1.z);

  return p;
}


// booleen pour indiquer la memoire trinagles est disponible (non pleine) 
int infoAjout;

void AJOUT_TRIANGLE(Triangle3D t)
{
	if (nT<nTmax) lT[nT++]=t; 
	else if (infoAjout==1)
	{ 
		printf(" !mémoire triangle pleine\n"); 
		infoAjout=0;
	} 
}
 

// Calcul de la marque du sommet P de valeur associee F
int marque(Point3D P, float F)
{
	if(F < isoVal)
		return 0;
	return 1;
}

void traitement_tetraedre(Tetraedre T)
{
	Triangle3D t1;
	Triangle3D t2;

	int m0 = marque(T.p0, T.v0);
	int m1 = marque(T.p1, T.v1);
	int m2 = marque(T.p2, T.v2);
	int m3 = marque(T.p3, T.v3);

	if(m0 != m1 && m0 != m2 && m0 != m3)
	{
		t1.p[0] = intersection_arete_hyperplan(isoVal, T.p0, T.p1, T.v0, T.v1);
		t1.p[1] = intersection_arete_hyperplan(isoVal, T.p0, T.p2, T.v0, T.v2);
		t1.p[2] = intersection_arete_hyperplan(isoVal, T.p0, T.p3, T.v0, T.v3);

		AJOUT_TRIANGLE(t1);
	}


	if(m1 != m2 && m1 != m3 && m1 != m0)
	{
		t1.p[0] = intersection_arete_hyperplan(isoVal, T.p1, T.p2, T.v1, T.v2);
		t1.p[1] = intersection_arete_hyperplan(isoVal, T.p1, T.p3, T.v1, T.v3);
		t1.p[2] = intersection_arete_hyperplan(isoVal, T.p1, T.p0, T.v1, T.v0);
		
		AJOUT_TRIANGLE(t1);
	}

	if(m2 != m3 && m2 != m0 && m2 != m1)
	{
		t1.p[0] = intersection_arete_hyperplan(isoVal, T.p2, T.p3, T.v2, T.v3);
		t1.p[1] = intersection_arete_hyperplan(isoVal, T.p2, T.p0, T.v2, T.v0);
		t1.p[2] = intersection_arete_hyperplan(isoVal, T.p2, T.p1, T.v2, T.v1);
		
		AJOUT_TRIANGLE(t1);
	}

	if(m3 != m0 && m3 != m1 && m3 != m2)
	{
		t1.p[0] = intersection_arete_hyperplan(isoVal, T.p3, T.p0, T.v3, T.v0);
		t1.p[1] = intersection_arete_hyperplan(isoVal, T.p3, T.p1, T.v3, T.v1);
		t1.p[2] = intersection_arete_hyperplan(isoVal, T.p3, T.p2, T.v3, T.v2);
		
		AJOUT_TRIANGLE(t1);
	}

	if(m0 == m1 && m1 != m2 && m2 == m3)
	{
		t1.p[0] = intersection_arete_hyperplan(isoVal, T.p0, T.p2, T.v0, T.v2);
		t1.p[1] = intersection_arete_hyperplan(isoVal, T.p0, T.p3, T.v0, T.v3);
		t1.p[2] = intersection_arete_hyperplan(isoVal, T.p1, T.p2, T.v1, T.v2);

		t2.p[0] = intersection_arete_hyperplan(isoVal, T.p1, T.p3, T.v1, T.v3);
		t2.p[1] = intersection_arete_hyperplan(isoVal, T.p0, T.p3, T.v0, T.v3);
		t2.p[2] = intersection_arete_hyperplan(isoVal, T.p1, T.p2, T.v1, T.v2);

		AJOUT_TRIANGLE(t1);
		AJOUT_TRIANGLE(t2);
	}

	if(m0 == m2 && m2 != m1 && m1 == m3)
	{
		t1.p[0] = intersection_arete_hyperplan(isoVal, T.p0, T.p1, T.v0, T.v1);
		t1.p[1] = intersection_arete_hyperplan(isoVal, T.p0, T.p3, T.v0, T.v3);
		t1.p[2] = intersection_arete_hyperplan(isoVal, T.p2, T.p1, T.v2, T.v1);

		t2.p[0] = intersection_arete_hyperplan(isoVal, T.p2, T.p3, T.v2, T.v3);
		t2.p[1] = intersection_arete_hyperplan(isoVal, T.p0, T.p3, T.v0, T.v3);
		t2.p[2] = intersection_arete_hyperplan(isoVal, T.p2, T.p1, T.v2, T.v1);

		AJOUT_TRIANGLE(t1);
		AJOUT_TRIANGLE(t2);
	}

	if(m0 == m3 && m3 != m1 && m1 == m3)
	{
		t1.p[0] = intersection_arete_hyperplan(isoVal, T.p0, T.p1, T.v0, T.v1);
		t1.p[1] = intersection_arete_hyperplan(isoVal, T.p0, T.p2, T.v0, T.v2);
		t1.p[2] = intersection_arete_hyperplan(isoVal, T.p3, T.p1, T.v3, T.v1);

		t2.p[0] = intersection_arete_hyperplan(isoVal, T.p3, T.p2, T.v3, T.v2);
		t2.p[1] = intersection_arete_hyperplan(isoVal, T.p0, T.p2, T.v0, T.v2);
		t2.p[2] = intersection_arete_hyperplan(isoVal, T.p3, T.p1, T.v3, T.v1);
		
		AJOUT_TRIANGLE(t1);
		AJOUT_TRIANGLE(t2);
	}
	
}

///////////////////////////////////////////////////////////////////////////////
// 
// fonction qui calcule la surface isovaleur
// chaque cube élémentaire est un cube d'une grille grossière incluse dans
//   la grille initiale 
// la grille initiale correspond aux points P(i,j,k) avec
// 0 <= i < sx, 0 <= j < sy, 0 <= k < sz
// la grille grossière correspond aux points 
//   P(i1=i*taille,j1=j*taille,k1=k*taille) avec
// 0 <= i1 < sx-taille, 0 <= j1 < sy-taille, 0 <= k1 < sz-taille
// 
// le point P(i,j,k) a pour coordonnées :
//   x = i
//   y = j
//   z = k
// 
// la macro AJOUT_TRIANGLE utilisée pour l'ajout de triangle ne peut etre
// incluse que dans la routine calcul_surface_isovaleur() 
//
void calcul_surface_isovaleur()
{
	// indices de la base du cube élémentaire 
	int i1, j1, k1;

	fprintf(stdout, "calcul_surface_isovaleur : taille = %2d,", taille);
	fprintf(stdout, " isoVal = %5.2f,", isoVal );
	nT=0; // effacement de la liste de triangles 
	infoAjout=1; // memoire triangles disponible 
			
	// prise en compte des différents cubes élémentaires 
	// inclus dans la grille de points initiale 	
	for(k1=0;k1<sz-taille ;k1+=taille)
	{
		for(j1=0;j1<sy-taille;j1+=taille)
		{
			for(i1=0;i1<sx-taille;i1+=taille)
			{
				int i;
				int im,jm,km;
				Point3D p[8];   // coordonnées des sommets du cube élémentaire 
				float val[8];  // valeurs aux sommets du cube élémentaire 
				Tetraedre tetra; // un tetraedre issu du cube 
				
				// calcul du cube elementaire : 8 sommets (m=0 à 7) 
				//   p[m]   : coordonnées du sommet m               
				//   val[m] : valeur au sommet m                    
				// le sommet m du cube élémentaire correspond au    
				// point P(im,jm,km) de la grille de données        
				// les 6 faces du cube sont :                       
				//      7------------4          z                   
				//     /|           /|          |                   
				//    / |          / |          |                   
				//   6------------5  |          |                   
				//   |  |         |  |          |                   
				//   |  |         |  |          |                   
				//   |  |         |  |          |                   
				//   |  3---------|--0          .------------y      
				//   | /          | /          /                    
				//   |/           |/          /                     
				//   2------------1          x                      
				

				im=i1;
				jm=j1+taille;
				km=k1;
				p[0].x=(float)(im);
				p[0].y=(float)(jm);
				p[0].z=(float)(km); 
				val[0] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1+taille;
				jm=j1+taille;
				km=k1;
				p[1].x=(float)(im);
				p[1].y=(float)(jm);
				p[1].z=(float)(km);
				val[1] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1+taille;
				jm=j1;
				km=k1;
				p[2].x=(float)(im);
				p[2].y=(float)(jm);
				p[2].z=(float)(km);
				val[2] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1;
				jm=j1;
				km=k1;
				p[3].x=(float)(im);
				p[3].y=(float)(jm);
				p[3].z=(float)(km);
				val[3] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1;
				jm=j1+taille;
				km=k1+taille;
				p[4].x=(float)(im);
				p[4].y=(float)(jm);
				p[4].z=(float)(km);
				val[4] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1+taille;
				jm=j1+taille;
				km=k1+taille;
				p[5].x=(float)(im);
				p[5].y=(float)(jm);
				p[5].z=(float)(km);
				val[5] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1+taille;
				jm=j1;
				km=k1+taille;
				p[6].x=(float)(im);
				p[6].y=(float)(jm);
				p[6].z=(float)(km);
				val[6] = buffer_vol[im+jm*sx+km*sx*sy];

				im=i1;
				jm=j1;
				km=k1+taille;
				p[7].x=(float)(im);
				p[7].y=(float)(jm);
				p[7].z=(float)(km);
				val[7] = buffer_vol[im+jm*sx+km*sx*sy];

				// découpage du cube élémentaire en 6 tétraèdres
				// pour chaque tétraèdre, tester s'il intersecte la
				// surface isovaleur et calculer les triangles correspondants
				// pour ajouter un triangle t :
				//   calculer les 3 sommets S0,S1,S2
				//   t.p[0]=S0;
				//   t.p[1]=S1;
				//   t.p[2]=S2;
				//   AJOUT_TRIANGLE(t);	
				
				// Triangle3D t;

				// DEBUT PARTIE A COMPLETER 
				
				// 6 Tetraedres : [3067] [0574] [0657] [1306] [1065] [2136]

				tetra.p0 = p[3];
				tetra.p1 = p[0];
				tetra.p2 = p[6];
				tetra.p3 = p[7];
				tetra.v0 = val[3];
				tetra.v1 = val[0];
				tetra.v2 = val[6];
				tetra.v3 = val[7];
				traitement_tetraedre(tetra);

				tetra.p0 = p[0];
				tetra.p1 = p[5];
				tetra.p2 = p[7];
				tetra.p3 = p[4];
				tetra.v0 = val[0];
				tetra.v1 = val[5];
				tetra.v2 = val[7];
				tetra.v3 = val[4];
				traitement_tetraedre(tetra);

				tetra.p0 = p[0];
				tetra.p1 = p[6];
				tetra.p2 = p[5];
				tetra.p3 = p[7];
				tetra.v0 = val[0];
				tetra.v1 = val[6];
				tetra.v2 = val[5];
				tetra.v3 = val[7];
				traitement_tetraedre(tetra);

				tetra.p0 = p[1];
				tetra.p1 = p[3];
				tetra.p2 = p[0];
				tetra.p3 = p[6];
				tetra.v0 = val[1];
				tetra.v1 = val[3];
				tetra.v2 = val[0];
				tetra.v3 = val[6];
				traitement_tetraedre(tetra);

				tetra.p0 = p[1];
				tetra.p1 = p[0];
				tetra.p2 = p[6];
				tetra.p3 = p[5];
				tetra.v0 = val[1];
				tetra.v1 = val[0];
				tetra.v2 = val[6];
				tetra.v3 = val[5];
				traitement_tetraedre(tetra);

				tetra.p0 = p[2];
				tetra.p1 = p[1];
				tetra.p2 = p[3];
				tetra.p3 = p[6];
				tetra.v0 = val[2];
				tetra.v1 = val[1];
				tetra.v2 = val[3];
				tetra.v3 = val[6];
				traitement_tetraedre(tetra);



				// FIN PARTIE A COMPLETER 
									 
						 
				// si memoire triangle pleine : sortir de la boucle 
				if (infoAjout==0) goto 	FinIsoSurface;
								 
			} // for i1 
		} // for j1 
	} // for k1 
	
FinIsoSurface :	
	fprintf(stdout, " nT : %8ld", nT);
	fflush(stdout);
	fprintf(stdout, "\b\b\b\b\b\b\b\b\b");
	fprintf(stdout, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
	fprintf(stdout, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
}

// dessine la grille de cubes
void dessine_grille_cubes()
{
	int i1,j1,k1;
    glDisable(GL_LIGHTING);
	glColor3d(0.7,0.7,0.7);
	glLineWidth(1.0);
	
	glBegin(GL_LINES);
	for(k1=0;k1<sz-taille ;k1+=taille)
	{
		for(j1=0;j1<sy-taille;j1+=taille)
		{
			for(i1=0;i1<sx-taille;i1+=taille)
			{
				glVertex3d(i1       ,j1       ,k1       ); 
				glVertex3d(i1+taille,j1       ,k1       ); 
				glVertex3d(i1       ,j1+taille,k1       ); 
				glVertex3d(i1+taille,j1+taille,k1       ); 
				glVertex3d(i1       ,j1       ,k1+taille); 
				glVertex3d(i1+taille,j1       ,k1+taille); 
				glVertex3d(i1       ,j1+taille,k1+taille); 
				glVertex3d(i1+taille,j1+taille,k1+taille); 
				
				glVertex3d(i1       ,j1       ,k1       ); 
				glVertex3d(i1       ,j1+taille,k1       ); 
				glVertex3d(i1+taille,j1 	  ,k1		); 
				glVertex3d(i1+taille,j1+taille,k1		); 
				glVertex3d(i1       ,j1       ,k1+taille); 
				glVertex3d(i1       ,j1+taille,k1+taille); 
				glVertex3d(i1+taille,j1 	  ,k1+taille); 
				glVertex3d(i1+taille,j1+taille,k1+taille); 
				
				glVertex3d(i1       ,j1       ,k1       ); 
				glVertex3d(i1       ,j1       ,k1+taille); 
				glVertex3d(i1+taille,j1 	  ,k1		); 
				glVertex3d(i1+taille,j1 	  ,k1+taille); 
				glVertex3d(i1       ,j1+taille,k1		); 
				glVertex3d(i1       ,j1+taille,k1+taille); 
				glVertex3d(i1+taille,j1+taille,k1		); 
				glVertex3d(i1+taille,j1+taille,k1+taille); 
			}
		}
	}
	glEnd();
    glEnable(GL_LIGHTING);
}

///////////////// 
// 
// routine de (ré)initialisation  
// 
//////////////// 
void initialisation()
{
	glLoadIdentity();	
	taille=10;
	isoVal=0.5; 
	zoom = 1.0;
	tbInitTransform();
	
	calcul_surface_isovaleur();
}

///////////////// 
// 
// fonction d'affichage du systeme 
// 
//////////////// 
void dessin()
{
	// glClearColor (0.0, 0.0, 0.0, 0.0);  
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
    // si le type de tracé est ligne on desactive l'éclairage 
    glEnable(GL_LIGHTING);

	glLoadIdentity();  // repere camera  

	tbVisuTransform(); // origine et orientation de la scene 

	// echelle 
	glScalef(zoom*eX,zoom*eY,zoom*eZ);
	
	// centrage de la scene                    
	// prise en compte de la boite englobante  
	glTranslatef( -sx/2.0, -sy/2.0, -sz/2.0);
     
	// dessin des triangles du MarchingCube 
	dessine_surface_isovaleur();
	
	// dessin de la grille de cube
	if (dessin_grille)
		dessine_grille_cubes();
		
	// envoi des commandes GL 
	glFlush();
	glutSwapBuffers();
}

 
///////////////////////////////////////////////////////////////////////////////
// prise en compte d'un redimensionnement de la fenetre 
void redimensionnement(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ratio = (float)w/(float)h;
	if (ratio>1.0)
		glOrtho(-sx/2.0*ratio,sx/2.0*ratio,
		        -sy/2.0, sy/2.0, 
				-1000.0*eZ, +1000.0*eZ);
	else
		glOrtho(-sx/2.0,sx/2.0,
		        -sy/2.0/ratio, sy/2.0/ratio, 
				-1000.0*eZ, +1000.0*eZ);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

 
///////////////////////////////////////////////////////////////////////////////
//  fonction de gestion des touches clavier 
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
	 
	// reset 
	case 'r': 
	case 'R': 
		initialisation();  
	break; 

	// modification de l'iso-valeur 
	case '-' :
		if (isoVal>=0.02)
		{
			isoVal-=0.02;
			calcul_surface_isovaleur();
		}
	break;

	case '+' :
		if (isoVal<=0.98)
		{
			isoVal+=0.02;
			calcul_surface_isovaleur();
		}
	break;

	// réduction de la taille élémentaire du cube
	case 't' :
	case 'T' :
		if (taille>1)
		{
			taille-=1;
			calcul_surface_isovaleur();
		}
	break;
	
	// augmentation de la taille élémentaire du cube
	case 'y' :
	case 'Y' :
		if (taille<40)
		{
			taille+=1;
			calcul_surface_isovaleur();
		}
	break;

	// zoom +/-
	case 'E' :
		if (zoom<10) zoom *= 1.05;
		break;
	case 'e' :
		if (zoom>0.1) zoom /= 1.05;
		break;

	// switch on/off du dessin de la grille de cubes
	case 'G' :
	case 'g' :
		dessin_grille = !dessin_grille;
		glutPostRedisplay();
		break;

    default : 
	break; 
	} 
} 
 

//////////////////////////////////////////////////////////////////
// lecture d'un fichier de données volumiques 
///////////////////////////////////////////////////////////////// 
void lire_fichier_data(int argc, char **argv)
{
  unsigned int i;
  char *datafile;
  FILE *f;
  int format;

  //////////////////////////////////////
  // test et récupération des arguments 
  //////////////////////////////////////

  if (argc <7) {
	fprintf( stderr, "usage:\n");
	fprintf( stderr, "%s filename sx sy sz hz format\n", argv[0]);
	fprintf( stderr, " sx sy sz : dimensions des données (entier)\n");
	fprintf( stderr, " eZ : facteur d'échelle en z (réel)\n");
	fprintf( stderr, " format (entier) :\n");
	fprintf( stderr, "  1 - valeur sur 1 octet \n");
	fprintf( stderr, "  2 - valeur sur 2 octets\n");
	exit(-1);
  }

  // sx , sy = dimensions des slices 
  // sz =      nb de slices 
  datafile = argv[1];
  sx = atoi(argv[2]); 
  sy = atoi(argv[3]); 
  sz = atoi(argv[4]); 
  eZ = atof(argv[5]); 
  format = atoi(argv[6]);

  taille_buffer_vol = 
		(unsigned long)(sx) *
		(unsigned long)(sy) *
		(unsigned long)(sz);
		
  /////////////////////////////////////////
  // lecture et normalisation des données  
  // chaque donnée entre 0 et 65535        
  /////////////////////////////////////////
  
  // tableau des données volumiques 
  buffer_vol = (float*) malloc(sizeof(float) * taille_buffer_vol );

  // ouverture du fichier de donnée volumique 
  if ((f=fopen( datafile, "r")) == 0) {
	fprintf( stderr, "Impossible de lire le fichier %s\n", argv[1]);
	exit(-1);
  }
  
  // lecture des données binaires : 
  // si format = 2 alors                                          
  //   chaque valeur lue v est codée sur 2 octets                 
  //  (unsigned short v entre 0 et 65535)                         
  // si format = 1 alors                                          
  //   chaque valeur lue v est codée sur 1 octet                  
  //  (unsigned char v entre 0 et 255)                            
  // buffer_vol[i + sx*j + sx*sy*k] (valeur réelle)               
  //   est la donnée correspondant à :                            
  //    x = i  avec  0 <= i <= sx-1								  
  //    y = j  avec  0 <= j <= sy-1								  
  //    z = k  avec  0 <= k <= sz-1								  
  
  if (format==2) // valeur du fichier sur 2 octets 
    {
		for (i=0; i<taille_buffer_vol; i++)
		{
			unsigned short v;
  			fread( &v, sizeof(unsigned short), 1, f);
			buffer_vol[i] = (float)v;
		}
	}
  else if (format==1) // valeur du fichier sur 1 octet 
    {
		for (i=0; i<taille_buffer_vol; i++)
		{
			unsigned char v;
  			fread( &v, sizeof(unsigned char), 1, f);
			buffer_vol[i] = (float)v;
		}
	}
  else 
	{
		fprintf(stderr, "Mauvais format\n");
		return;
	}

  fclose(f);

}

////////////////////////////////////////////////////////////////////////////// 
// normalise les données pour quelles soient comprises entre 0.0 et 1.0 
///////////////////////////////////////////////////////////////////////////// 
void normalise_data()
{
	float Max, Min;
	unsigned long i;
	
	// transformation affine des données pour que 
	// min(valeurs) = 0.0 et max(valeurs) = 1.0   
	Min=Max=buffer_vol[0];
	for (i=1;i<taille_buffer_vol;i++) 
	{
		if (Min>buffer_vol[i]) Min=buffer_vol[i];
		if (Max<buffer_vol[i]) Max=buffer_vol[i];
	}
	fprintf( stderr, "Min VolData = %f\n", Min);
	fprintf( stderr, "Max VolData = %f\n", Max);
	for (i=0;i<taille_buffer_vol;i++) 
		buffer_vol[i] = (buffer_vol[i]-Min)/(Max-Min);

}

//////////////////////////////////////////////////////////////////////////////
// allocation de memoire pour le buffer de triangles 
///////////////////////////////////////////////////////////////////////////// 
void init_memoire_triangles()
{
	nTmax = 64000000;
	do
	{
		nTmax /= 2;
		lT = (Triangle3D*)malloc(sizeof(Triangle3D)*nTmax);
	} while (lT == (Triangle3D*)NULL);
	printf("Mémoire allouée pour %ld triangles\n", nTmax);
	
}

////////////////////// 
// infos au démarrage
/////////////////////
void usage()
{
	fprintf( stdout, "\n");
	fprintf( stdout, "Dimensions des données %d %d %d\n", sx, sy, sz);

	fprintf( stdout, "\nMode d'emploi :\n");
	fprintf( stdout, " Rotations avec la souris (bouton gauche ou milieu enfoncé)\n");
	fprintf( stdout, " Touches clavier :\n"); 
	fprintf( stdout, " touches +/- : augmente/diminue l'iso-valeur\n");  
	fprintf( stdout, "        (isovaleur entre 0.0 et 1.0 - pas de 0.02)\n"); 
	fprintf( stdout, " touche t : diminue la taille du cube élémentaire\n"); 
	fprintf( stdout, " touche y : augmente la taille du cube élémentaire\n");
	fprintf( stdout, "        (taille du cube élémentaire entre 1 et 40)\n"); 
	fprintf( stdout, " touches E/e : augmente/diminue le zoom\n");  
	fprintf( stdout, " touches G:g : switch on/off de la grille de cubes\n");  
	fprintf( stdout, " touche r : reset (rotation, zoom, taille cube)\n"); 
	fprintf( stdout, " touches q/Q/ESC : FIN\n");  
	
}

///////////////// 
// 
// FONCTION PRINCIPALE
// 
////////////////
int main(int argc, char **argv)
{ 
	// infos des différentes touches 
	usage();
	
	// création d'une mémoire pour stocker                  
	// les triangles de la surface iso-valeur                
	// -> tableau lT de dimension maximale nTmax            
	// la variable nT donne le nombre de triangles calculés 
	// (cf. routine calcul_surface_isovaleur)                       
	init_memoire_triangles();
	
	// création d'une grille de données 3D de dimensions (sx,sy,sz)  
	// -> tableau buffer_vol                                         
	// buffer_vol[i+sx*j+sx*sy*k] contient la valeur du sommet de    
	// coordonnées (i,j,k), la valeur entre un réel entre 0.0 et 1.0 
	lire_fichier_data(argc, argv); // données à partir d'un fichier   
	normalise_data();

	//creation de la fenetre 
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Marching Tetrahedra");

	// initialisation des parametres de vue 
	initialisation();

	// fonction de redimensionnement
	glutReshapeFunc(redimensionnement);
	
	// fonction affichage  
	glutDisplayFunc (dessin); 
 
	// gestion touche clavier  
	glutKeyboardFunc(clavier); 
		
	// function de gestion des moments où rien ne se passe    	
	glutIdleFunc(dessin);
    
	// fonctions pour la gestion de la traqueboule 
	glutMouseFunc(tbMouseFunc);     
	glutMotionFunc(tbMotionFunc);  
	
	 // initialisation lumière et materiaux 
	initLumiere();
	initMatiere();
	glClearColor( 1., 1., 1., 0.);
		
	// cache les elements invisibles  
    glEnable(GL_DEPTH_TEST); 
	glEnable(GL_NORMALIZE);
	
	// boucle d'evenements GLUT  
	glutMainLoop();
	
	return 0;
}
 
 