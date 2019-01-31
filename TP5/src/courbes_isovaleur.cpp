///////////////////////////////////////////////////////////////////////////////
//  
//   Visualisation de données volumiques par slicing et
//     représentation de courbes isovaleur
//   Fichier de données de type RAW :
//  	format 1 : unsigned char 
//  	format 2 : unsigned short
//   de dimensions (sx,sy,sz)
//   avec échelle hz suivant z
//   
//   Compilation : 
//    g++ courbes_isovaleur.cpp -lglut -lGLU -lGL -o courbes_isovaleur
//   Exécution   : 
//    ./courbes_isovaleur filename sx sy sz hz format
//      par exemple
//    ./courbes_isovaleur CThead.bin 256 256 113 2.0 2
//  
//   Nicolas SZAFRAN - UFRIM2AG - 2015
//
///////////////////////////////////////////////////////////////////////////////
 
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <GL/glut.h>    

// booleen pour l'affichage des codes d'erreur lors de la création des textures
bool reportErrorTextures = false;

// booleen pour l'affichage de la vitesse de rafraichissement 
bool reportSpeed = false;

// booleen pour l'affichage d'info 
bool reportInfo = true;

// facteur d'échelle global 
double zoom=1.0;

// booleen pour le tracé de l'isocourbe 
bool dessiner_courbes_isovaleur = true;

// épaisseur du tracé des courbes isovaleur
int epaisseur_courbes_isovaleur = 1;
#define epaisseur_courbes_isovaleur_max 5

// position actuelle de la souris (utilisée lors d'un déplacement souris) 
int startx, starty;

// tableau des données volumiques 
float *buffer_vol;

// tableau utilisé pour l'image des slices 
unsigned short *TextureBuffer;

// dimensions du volume 
int sx, sy, sz;

// dimensions pour les textures 
int sx2, sy2, sz2;

// échelle en z 
double hz = 1.0;

// indices des positions des 3 slices 
int Ox, Oy, Oz;

// booleens pour l'affichage des plans resp. Ox, Oy et Oz
bool affiche_Ox = true;
bool affiche_Oy = true;
bool affiche_Oz = true;


// isovaleur de la courbe 
float isoVal=0.5;
// pas pour la modification de l'isovaleur  
float pasIsoVal=0.005; 

// type Point 3D 
typedef GLfloat GLPoint3[3];


GLPoint3 XPlane[4]; // coordonnées du bord de la coupe x constant 
GLPoint3 YPlane[4]; // coordonnées du bord de la coupe y constant 
GLPoint3 ZPlane[4]; // coordonnées du bord de la coupe z constant 

// numéros des images des trois slices 
enum { OxTexture, OyTexture, OzTexture};


///////////////////////////////////////////////////////////////////////////////
// Partie pour la gestion d'une traqueboule                                  
// (rotation de la scene à la souris)                                        
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////
// inversion p=inverse de m //
//////////////////////////////
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
GLdouble tb_matrix[16] =  { 1,0,0,0,
		                      0,1,0,0,
							0,0,1,0,
							0,0,0,1  };
GLdouble tb_inverse[16] = { 1,0,0,0,
		                      0,1,0,0,
							0,0,1,0,
							0,0,0,1  };

// variables pour la gestion de la souris 
int tb_ancienX, tb_ancienY, tb_tournerXY=0, tb_translaterXY=0, tb_bougerZ=0;


///////////////////////////////////////////////////////////////////////
// lit dans la matrice courante la position initiale du point de vue //
///////////////////////////////////////////////////////////////////////
void tbInitTransform()
{
	glGetDoublev( GL_MODELVIEW_MATRIX, tb_matrix );
	tbInverse( tb_matrix, tb_inverse );
}

////////////////////////////////////////////////
// applique la transformation de point de vue //
////////////////////////////////////////////////
void tbVisuTransform()
{
	glMultMatrixd( tb_matrix );
};

//////////////////////////////////////////////////
// callback de gestion des boutons de la souris //
//////////////////////////////////////////////////
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

/////////////////////////////////////////////////////
// callback de changement de position de la souris //
/////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
// Fin de la partie pour la gestion d'une traqueboule                        //
// (rotation de la scene à la souris)                                        //
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////
// affichage d'une erreur OpenGL //
///////////////////////////////////
void affiche_code_erreur_GL(int Code)
{
	switch (Code) {
	case GL_NO_ERROR:
		fprintf( stdout, "GL_NO_ERROR\n");
		break;
	case GL_INVALID_ENUM:
		fprintf( stdout, "GL_INVALID_ENUM\n");
		break;
	case GL_INVALID_VALUE:
		fprintf( stdout, "GL_INVALID_VALUE\n");
		break;
	case GL_INVALID_OPERATION:
		fprintf( stdout, "GL_INVALID_OPERATION\n");
		break;
	case GL_STACK_OVERFLOW:
		fprintf( stdout, "GL_STACK_OVERFLOW\n");
		break;
	case GL_STACK_UNDERFLOW:
		fprintf( stdout, "GL_STACK_UNDERFLOW\n");
		break;
	case GL_OUT_OF_MEMORY:
		fprintf( stdout, "GL_OUT_OF_MEMORY\n");
		break;
	default:
		fprintf( stdout, "Code d'erreur GL inconnu\n");
		break;
	}
}


///////////////////////////////////////////////////////////////////////
// copie de la fenetre OpenGL dans le fichier filename au format PPM //
///////////////////////////////////////////////////////////////////////
void copie_ecran(const char *filename) 
{ 
	GLint viewport[4] ; 
	int x, y, l; 
	int width, height; 
	unsigned int lineSize; 
	unsigned char *data; 
	FILE *fd; 
	
	// récupération des position et dimensions de la fenetre 
	glGetIntegerv(GL_VIEWPORT, viewport) ; 
	x=viewport[0];
	y=viewport[1]; 
	width=viewport[2];
	height=viewport[3] ; 
	
	// lecture de l'image dans data 
	lineSize = ((width*3+3)/4)*4 ;
	data = (unsigned char *)malloc(height*lineSize) ;
	glReadBuffer(GL_FRONT) ; 
	glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data) ; 
	
	// création du fichier ppm correspondant 
	fd = fopen(filename, "w");
	fprintf(fd, "P6\n%d %d\n255\n", width, height) ; 
	for (l=height-1; l>=0; --l) 
	{ 
		unsigned char *ptr = data + l*lineSize ; 
		fwrite(ptr, width*3, 1, fd);
	} 
	fclose(fd) ; 
	free(data); 
	fprintf(stdout, "Fichier %s créé\n", filename);
}


////////////////////////////////////////
// coordonnées des bords des 3 slices //
////////////////////////////////////////
void coordonnees_plans_slices(void)
{
	ZPlane[0][0] = 0.0;
	ZPlane[0][1] = 0.0;
	ZPlane[0][2] = Oz*hz;

	ZPlane[1][0] = sx-1;
	ZPlane[1][1] = 0.0;
	ZPlane[1][2] = Oz*hz;

	ZPlane[2][0] = sx-1;
	ZPlane[2][1] = sy-1;
	ZPlane[2][2] = Oz*hz;

	ZPlane[3][0] = 0.0;
	ZPlane[3][1] = sy-1;
	ZPlane[3][2] = Oz*hz;

//

	YPlane[0][0] = 0.0;
	YPlane[0][1] = Oy;
	YPlane[0][2] = 0.0*hz;

	YPlane[1][0] = sx-1;
	YPlane[1][1] = Oy;
	YPlane[1][2] = 0.0*hz;

	YPlane[2][0] = sx-1;
	YPlane[2][1] = Oy;
	YPlane[2][2] = (sz-1)*hz;

	YPlane[3][0] = 0.0;
	YPlane[3][1] = Oy-1;
	YPlane[3][2] = (sz-1)*hz;

//

	XPlane[0][0] = Ox;
	XPlane[0][1] = 0.0;
	XPlane[0][2] = 0.0*hz;

	XPlane[1][0] = Ox;
	XPlane[1][1] = sy-1;
	XPlane[1][2] = 0.0*hz;

	XPlane[2][0] = Ox;
	XPlane[2][1] = sy-1;
	XPlane[2][2] = (sz-1)*hz;

	XPlane[3][0] = Ox;
	XPlane[3][1] = 0.0;
	XPlane[3][2] = (sz-1)*hz;
}

///////////////////////////////////////////////////
// calcul des images des trois slices à afficher //
// -- slices x=Ox constant : numéro OxTexture    //
// -- slices y=Oy constant : numéro OyTexture    //
// -- slices z=Oz constant : numéro OzTexture    //
///////////////////////////////////////////////////
void creer_textures_slices(void)
{
	int i,j,k, Code;
	unsigned short *p;

	coordonnees_plans_slices();

	/////////////////////////////////
	// image de la coupe (slice) k=Oz 
	k = Oz;
	p = TextureBuffer;
	for (j=0; j<sy; j++) 
	{
		for (i=0; i<sx; i++) 
		{
			*p = (unsigned short)(65535.0*buffer_vol[i+j*sx+k*sx*sy]);
			p++;
		}
	}
	
	glBindTexture( GL_TEXTURE_2D, OzTexture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, sx);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE16, sx2, sy2, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, TextureBuffer);

	if (reportErrorTextures)
	{
		Code = glGetError();
		affiche_code_erreur_GL(Code);
	}


	/////////////////////////////////
	// image de la coupe (slice) j=Oy 
	j = Oy;
	p = TextureBuffer;
	for (k=0; k<sz; k++) 
	{
		for (i=0; i<sx; i++) 
		{
			*p = (unsigned short)(65535.0*buffer_vol[i+j*sx+k*sx*sy]);
			p++;
		}
	}
	
	glBindTexture( GL_TEXTURE_2D, OyTexture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // pour s'assurer que la carte graphique va chercher les données
		                                     // au bon endroit dans le tableau

	glPixelStorei(GL_UNPACK_ROW_LENGTH, sx); // inutile si la largeur de l'image correspond aux lignes du tableau

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, sx2, sz2, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, TextureBuffer);
	if (reportErrorTextures)
	{
		Code = glGetError();
		affiche_code_erreur_GL(Code);
	}

	/////////////////////////////////
	// image de la coupe (slice) i=Ox 
	i = Ox;
	p = TextureBuffer;
	for (k=0; k<sz; k++) 
	{
		for (j=0; j<sy; j++) 
		{
			*p = (unsigned short)(65535.0*buffer_vol[i+j*sx+k*sx*sy]);
			p++;
		}
	}
	
	glBindTexture( GL_TEXTURE_2D, OxTexture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, sy); // inutile si la largeur de l'image 
	                                     // correspond aux lignes du tableau
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, sy2, sz2, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, TextureBuffer);
	if (reportErrorTextures)
	{
		Code = glGetError();
		affiche_code_erreur_GL(Code);
	}
}


//////////////////////////////////////////////////////////////
// dessin des slices : affichage des images correspondantes //
//////////////////////////////////////////////////////////////
void dessin_slices(void)
{
	float xmax, ymax, zmax;
	
	xmax = ((float)sx)/((float)sx2);
	ymax = ((float)sy)/((float)sy2);
	zmax = ((float)sz)/((float)sz2);
	
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0,1.0,1.0);
	
	if (affiche_Oz)
	{
		glBindTexture( GL_TEXTURE_2D, OzTexture);
	
		glBegin(GL_QUADS);
			glTexCoord2f(0.0 , 0.0 );
			glVertex3fv(ZPlane[0]);
			glTexCoord2f(xmax, 0.0 );
			glVertex3fv(ZPlane[1]);
			glTexCoord2f(xmax, ymax);
			glVertex3fv(ZPlane[2]);
			glTexCoord2f(0.0 , ymax);
			glVertex3fv(ZPlane[3]);
		glEnd();
	}
	
	if (affiche_Oy)
	{
		glBindTexture( GL_TEXTURE_2D, OyTexture);
	
		glBegin(GL_QUADS);
			glTexCoord2f(0.0 , 0.0 );
			glVertex3fv(YPlane[0]);
			glTexCoord2f(xmax, 0.0 );
			glVertex3fv(YPlane[1]);
			glTexCoord2f(xmax, zmax);
			glVertex3fv(YPlane[2]);
			glTexCoord2f(0.0 , zmax);
			glVertex3fv(YPlane[3]);
		glEnd();
	}

	if (affiche_Ox)
	{
		glBindTexture( GL_TEXTURE_2D, OxTexture);
	
		glBegin(GL_QUADS);
			glTexCoord2f(0.0 , 0.0 );
			glVertex3fv(XPlane[0]);
			glTexCoord2f(ymax, 0.0 );
			glVertex3fv(XPlane[1]);
			glTexCoord2f(ymax, zmax);
			glVertex3fv(XPlane[2]);
			glTexCoord2f(0.0 , zmax);
			glVertex3fv(XPlane[3]);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
}

///////////////////////////////////
// dessin des bords des 3 slices //
///////////////////////////////////
void dessin_bords_slices(void)
{
	glLineWidth(3.0);
	glColor3f(1.0,1.0,0.0);

	if (affiche_Oz)
	{
		glBegin(GL_LINE_STRIP);
			glVertex3fv(ZPlane[0]);
			glVertex3fv(ZPlane[1]);
			glVertex3fv(ZPlane[2]);
			glVertex3fv(ZPlane[3]);
			glVertex3fv(ZPlane[0]);
		glEnd();
	}

	if (affiche_Oy)
	{
		glBegin(GL_LINE_STRIP);
			glVertex3fv(YPlane[0]);
			glVertex3fv(YPlane[1]);
			glVertex3fv(YPlane[2]);
			glVertex3fv(YPlane[3]);
			glVertex3fv(YPlane[0]);
		glEnd();
	}

	if (affiche_Ox)
	{
		glBegin(GL_LINE_STRIP);
			glVertex3fv(XPlane[0]);
			glVertex3fv(XPlane[1]);
			glVertex3fv(XPlane[2]);
			glVertex3fv(XPlane[3]);
			glVertex3fv(XPlane[0]);
		glEnd();
	}

	glBegin(GL_LINES);
	if (affiche_Oy && affiche_Oz)
	{
		glVertex3f( 0.0, Oy , Oz *hz);
		glVertex3f( sx , Oy , Oz *hz);
	}
	if (affiche_Ox && affiche_Oz)
	{
		glVertex3f( Ox , 0.0, Oz *hz);
		glVertex3f( Ox , sy , Oz *hz);
	}
	if (affiche_Ox && affiche_Oy)
	{
		glVertex3f( Ox , Oy , 0.0*hz);
		glVertex3f( Ox , Oy , sz *hz);
	}
	glEnd();
}

void intersection_arete_plan(double * i, double P[], double Q[], double v)
{
	//printf("INTERSECTION");
	double d = (v - P[2]) / (Q[2] - P[2]);
	i[0] = (1 - d) * P[0] + d * Q[0];
	i[1] = (1 - d) * P[1] + d * Q[1];
	i[2] = Oz*hz;
}

///////////////////////////////////////////////////////////////////////////////
// La donnée (i,j,k) correspond à la position (x,y,z)
//   x = (float)(i)
//   y = (float)(j)
//   z = (float)(k)*hz
// et sa valeur est buffer_vol[i+sx*j+sx*sy*k]
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// calcul et tracé de l'isocourbe de valeur isoVal pour la coupe k=Oz 
////////////////////////////////////////////////////////////////////////
void dessin_courbe_iso_planZ()
{
	int i,j;
	glLineWidth(epaisseur_courbes_isovaleur); // épaisseur   
	glColor3f(0.0,1.0,1.0); // couleur cyan 
	glBegin(GL_LINES);
	
	// calcul et tracé de la courbe isovaleur pour la coupe k=Oz           
	// les données de la coupe k=Oz sont données par              
	// buffer_vol[i+sx*j+sx*sy*k] avec 0<=i<=sx-1, 0<=j<=sy-1 et k=Oz 
	for (i=1; i<sx; i++)
	for (j=1; j<sy; j++)
	{
		// les 4 valeurs aux sommets du rectangle
		//
		//   v00 --- v10   j-1
		//    |       |
		//    |       |
		//    |       |
		//   v01 --- v11   j
		//
		//   i-1      i
		
		double v00 = buffer_vol[Oz*sx*sy+(i-1)+(j-1)*sx];
		double v01 = buffer_vol[Oz*sx*sy+(i-1)+(j  )*sx];
		double v10 = buffer_vol[Oz*sx*sy+(i  )+(j-1)*sx];
		double v11 = buffer_vol[Oz*sx*sy+(i  )+(j  )*sx];

		double p00[3] = {i-1, 	j-1, 	v00};
		double p01[3] = {i-1, 	j, 		v01};
		double p10[3] = {i,   	j-1, 	v10};
		double p11[3] = {i,   	j, 		v11};

		double point[3];

		//Triangle v00 v01 v10
		if((v00>isoVal && isoVal>v01) || (v01>isoVal && isoVal>v00))
		{
			intersection_arete_plan(point, p00, p01, isoVal);
			glVertex3f( point[0] , point[1] , point[2] );
		}
		
		if((v01>isoVal && isoVal>v10) || (v10>isoVal && isoVal>v01))
		{
			intersection_arete_plan(point, p01, p10, isoVal);
			glVertex3f( point[0] , point[1] , point[2] );
		}
		
		if((v10>isoVal && isoVal>v00) || (v00>isoVal && isoVal>v10))
		{
			intersection_arete_plan(point, p10, p00, isoVal);
			glVertex3f( point[0] , point[1] , point[2] );
		}
	glEnd();

	glBegin(GL_LINES);
		//Triangle v01 v10 v11
		if((v01>isoVal && isoVal>v10) || (v10>isoVal && isoVal>v01))
		{
			intersection_arete_plan(point, p01, p10, isoVal);
			glVertex3f( point[0] , point[1] , point[2] );
		}
		
		if((v10>isoVal && isoVal>v11) || (v11>isoVal && isoVal>v10))
		{
			intersection_arete_plan(point, p10, p11, isoVal);
			glVertex3f( point[0] , point[1] , point[2] );
		}
		
		if((v11>isoVal && isoVal>v01) || (v01>isoVal && isoVal>v11))
		{
			intersection_arete_plan(point, p11, p01, isoVal);
			glVertex3f( point[0] , point[1] , point[2] );
		}
		
	}

	glEnd();	
}

/////////////////////////////////////////////////////////////////////
// affichage d'informations sur la position des 3 plans des slices
// et l'isovaleur
/////////////////////////////////////////////////////////////////////
void infos_slices_isovaleur()
{
	if (reportInfo)
	{
		fprintf( stdout, 
		  "Ox = %d(/%d), Oy = %d(/%d), Oz = %d(/%d), isovaleur=%f\n", 
		  Ox, sx, Oy, sy, Oz, sz, isoVal);
	}
}

//////////////////////////////////////////
// callback de (re)dessin de la fenêtre //
//////////////////////////////////////////
void dessin()
{
	int start, end;

	if (reportSpeed) 
	{
		start = glutGet(GLUT_ELAPSED_TIME);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();  // repere camera  

	tbVisuTransform(); // origine et orientation de la scene 

	glScalef(zoom,zoom,zoom);
	glTranslatef( -sx/2.0, -sy/2.0, -sz*hz/2.0);
		 
	dessin_slices();
	dessin_bords_slices();
	if (dessiner_courbes_isovaleur) 
	{  	
// 		if (affiche_Ox)
// 			dessin_courbe_iso_planX();
// 		if (affiche_Oy)
// 			dessin_courbe_iso_planY();
		if (affiche_Oz)
			dessin_courbe_iso_planZ();
	}
	
	if (reportSpeed) 
	{
		glFinish();
		end = glutGet(GLUT_ELAPSED_TIME);
		printf("Speed %.3g frames/sec (%d ms)\n", 1000.0/(end-start), end-start);
	}

	glutSwapBuffers();
}


///////////////////////////////////////////////////////////////////////////////
// lecture des données volumiques suivant les parametres de la ligne de commande
///////////////////////////////////////////////////////////////////////////////
void init_lecture_donnees_volumiques(int argc, char **argv)
{
	long i,j;
	int TailleMax;
	float max_data;
	char *datafile;
	FILE *f;
	int format=2;
	
	////////////////////////////////////////
	// test et récupération des arguments //
	////////////////////////////////////////

	if (argc <7) 
	{
		fprintf( stdout, "usage:\n");
		fprintf( stdout, "%s filename sx sy sz hz format\n", argv[0]);
		fprintf( stdout, " sx sy sz : dimensions des données\n");
		fprintf( stdout, " hz : facteur d'échelle en z\n");
		fprintf( stdout, " format :\n");
		fprintf( stdout, "  1 - valeur sur 1 octet \n");
		fprintf( stdout, "  2 - valeur sur 2 octets\n");
		fprintf( stdout, "exemple :\n");  
		fprintf( stdout, "./%s CThead.bin 256 256 113 2.0 2\n", argv[0]);  
		exit(-1);
	}

	// sx , sy = dimensions des slices 
	// sz =      nb de slices 
	datafile = argv[1];
	sx = atoi(argv[2]); 
	sy = atoi(argv[3]); 
	sz = atoi(argv[4]); 
	hz = atof(argv[5]); 
	format = atoi(argv[6]);

	///////////////////////////////////////////
	// lecture et normalisation des données  //
	// chaque donnée entre 0.0 et 1.0        //
	///////////////////////////////////////////
	
	// tableau des données volumiques 
	buffer_vol = (float*) malloc(sizeof(float) * sx*sy*sz );

	// ouverture du fichier de donnée volumique 
	if ((f=fopen( datafile, "r")) == 0) 
	{
		fprintf( stdout, "Impossible de lire le fichier %s\n", argv[1]);
		exit(-1);
	}
	
	// lecture des données binaires : 
	// chaque valeur sur 2 bytes (entier court entre 0 et 65535)    
	// buffer_vol[i + sx*j + sx*sy*k] est la donnée correspondant à 
	//    x = i  avec  0 <= i <= sx-1								  
	//    y = j  avec  0 <= j <= sy-1								  
	//    z = k  avec  0 <= k <= sz-1								  
	
	if (format==2) // valeur du fichier sur 2 octets 
		{
		for (i=0; i<sx*sy*sz; i++)
		{
			unsigned short v;
				fread( &v, sizeof(unsigned short), 1, f);
			buffer_vol[i] = v;
		}
	}
	else if (format==1) // valeur du fichier sur 1 octet 
		{
		for (i=0; i<sx*sy*sz; i++)
		{
			unsigned char v;
				fread( &v, sizeof(unsigned char), 1, f);
			buffer_vol[i] = v;
		}
	}
	else 
	{
		fprintf(stdout, "Mauvais format\n");
		exit( -1);
	}

	fclose(f);

	// multiplication des données par un facteur pour que 
	// max(buffer_vol[i])=1.0   
	max_data=0.0;
	for (i=0;i<sx*sy*sz;i++) 
	{
		if (max_data<buffer_vol[i]) 
		{
			max_data=buffer_vol[i];j=i;
		}
	}
	fprintf( stdout, "Max VolData = %f, indice=%ld\n", max_data,j);
	for (i=0;i<sx*sy*sz;i++) 
	{
		buffer_vol[i] /= max_data;
	}
	max_data=0.0;
	for (i=0;i<sx*sy*sz;i++) 
	{
		if (max_data<buffer_vol[i]) 
		{
			max_data=buffer_vol[i];
		}
	}
	fprintf( stdout, "Nouveau Max VolData = %f\n", max_data);


	////////////////////////////////////////////////
	// initialisation des variables du programme  //
	////////////////////////////////////////////////
	
	// sx2 : plus petite puissance de 2 >= sx 	
	// sy2 : plus petite puissance de 2 >= sy 	
	// sz2 : plus petite puissance de 2 >= sz 	
	// utilisés pour la représentation des slices 
	sx2 = 1; while (sx2 < sx) {sx2 *= 2;}
	sy2 = 1; while (sy2 < sy) {sy2 *= 2;}
	sz2 = 1; while (sz2 < sz) {sz2 *= 2;}

//  fprintf( stdout, "sz2 = %d\n", sz2);

	// position initiale des coupes 	
	Ox=sx/2; Oy=sy/2; Oz=sz/2;

	// calcul d'une taille convenable pour le buffer de texture 
	TailleMax = sx2;
	if (TailleMax < sy2) {TailleMax = sy2;}
	if (TailleMax < sz2) {TailleMax = sz2;}
	TextureBuffer = (unsigned short*) 
		malloc(sizeof(unsigned short) * TailleMax * TailleMax);


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
				-1000.0*hz, +1000.0*hz);
	else
		glOrtho(-sx/2.0,sx/2.0,
		        -sy/2.0/ratio, sy/2.0/ratio, 
				-1000.0*hz, +1000.0*hz);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
// aide en ligne du programme
void afficher_aide()
{
	fprintf( stdout, "\n");
	fprintf( stdout, "Dimensions des données %d %d %d\n", sx, sy, sz);

	fprintf( stdout, "\nMode d'emploi :\n");
	fprintf( stdout, "\n rotation avec la souris (bouton gauche ou milieu enfoncé)\n");
	
	fprintf( stdout, "\n touches x,y,z,X,Y,Z: modifie la position des 3 plans\n");  
	fprintf( stdout, "       de coupe dans le volume de donnees\n");
	fprintf( stdout, " touche d : switch on/off du tracé de la courbe isovaleur'\n");  
	fprintf( stdout, " touche D : modif épaisseur tracé courbe isovaleur'\n");  
	fprintf( stdout, " touche i : switch on/off des infos plans et iso-valeur\n");  
	fprintf( stdout, " touche 0 : copie d'écran dans le fichier 'image0.ppm'\n");  
	fprintf( stdout, " touche 1 : switch on/off affichage plan X constant\n");  
	fprintf( stdout, " touche 2 : switch on/off affichage plan Y constant\n");  
	fprintf( stdout, " touche 3 : switch on/off affichage plan Z constant\n");  
	fprintf( stdout, " touches +/- : augmente/diminue l'iso-valeur\n");  
	fprintf( stdout, " touches E/e : augmente/diminue le zoom\n");  
	fprintf( stdout, " touches h : affiche l'aide\n");  
	fprintf( stdout, " touches q,Q,ESC : FIN\n");  
}

/////////////////////////////////////////////
// callback associé aux évenements clavier //
/////////////////////////////////////////////
void clavier(unsigned char c, int x, int y)
{
	switch (c)
	{
		// fin du programme
		case 27  :
		case 'q' :
		case 'Q' :
			exit(0);

		// modifier la position du slice en X
		case 'X' :
			if (Ox < sx-1)
			{
				Ox++;
				infos_slices_isovaleur();
				creer_textures_slices();
				glutPostRedisplay();
			}
			break;
		case 'x' :
			if (Ox > 0)
			{
				Ox--;
				infos_slices_isovaleur();
				creer_textures_slices();
				glutPostRedisplay();
			}
			break;

		// modifier la position du slice en Y
		case 'Y' :
			if (Oy < sy-1)
			{
				Oy++;
				infos_slices_isovaleur();
				creer_textures_slices();
				glutPostRedisplay();
			}
			break;
		case 'y' :
			if (Oy > 0)
			{
				Oy--;
				infos_slices_isovaleur();
				creer_textures_slices();
				glutPostRedisplay();
			}
			break;

		// modifier la position du slice en Z
		case 'Z' :
			if (Oz < sz-1)
			{
				Oz++;
				infos_slices_isovaleur();
				creer_textures_slices();
				glutPostRedisplay();
			}
			break;
		case 'z' :
			if (Oz > 0)
			{
				Oz--;
				infos_slices_isovaleur();
				creer_textures_slices();
				glutPostRedisplay();
			}
			break;


		// modification de l'isovaleur 
		case '-' :
			if (isoVal >= pasIsoVal)
			{
				isoVal -= pasIsoVal;
				infos_slices_isovaleur();
				glutPostRedisplay();
			}
			break;
		case '+' :
			if (isoVal <= 1.0-pasIsoVal)
			{
				isoVal += pasIsoVal;
				infos_slices_isovaleur();
				glutPostRedisplay();
			}
			break;

		// changement facteur de zoom 
		case 'E' :
			zoom *= 1.05;
			glutPostRedisplay();
			break;
		case 'e' :
			zoom /= 1.05;
			glutPostRedisplay();
			break;

		// copie d'écran 
		case '0' :
			copie_ecran("image0.ppm");
			break;
			
		// switch affichage plan Ox
		case '1' :
			affiche_Ox = !affiche_Ox;
			glutPostRedisplay();
			break;
			
		// switch affichage plan Oy
		case '2' :
			affiche_Oy = !affiche_Oy;
			glutPostRedisplay();
			break;
			
		// switch affichage plan Oz
		case '3' :
			affiche_Oz = !affiche_Oz;
			glutPostRedisplay();
			break;
			
		// switch dessin des courbes isovaleur
		case 'd' :
			dessiner_courbes_isovaleur = !dessiner_courbes_isovaleur;
			glutPostRedisplay();
			break;
			
		// modif épaisseur de tracé des courbes isovaleur
		case 'D' :
			epaisseur_courbes_isovaleur++;
			if (epaisseur_courbes_isovaleur>epaisseur_courbes_isovaleur_max)
				epaisseur_courbes_isovaleur=1;
			glutPostRedisplay();
			break;
			
		// switch affichage information
		case 'i' :
			reportInfo = !reportInfo;
			glutPostRedisplay();
			break;
			
		// affichage aide
		case 'h' :
			afficher_aide();
			break;
	}
}

	

///////////////////////////////////////////////////////////////////////////////
// FONCTION PRINCIPALE                                                       //
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	///////////////////////////////////////////////////
	// initialisation et lecture des données suivant //
	// les arguments de la ligne de commande         //
	///////////////////////////////////////////////////
	init_lecture_donnees_volumiques(argc, argv);

	////////////////////////////////////////////////
	// infos diverses dans la fenetre de commande //
	////////////////////////////////////////////////
	
	afficher_aide();
	infos_slices_isovaleur();

	///////////////////////////////////////////////////
	// création de la fenetre avec la librairie GLUT //	
	///////////////////////////////////////////////////
	
	glutInit(&argc, argv);

	// créer une fenetre carree dont la dimension est la moitie
	// de la hauteur de l'écran
	int dim_fenetre = glutGet(GLUT_SCREEN_HEIGHT)/2;
	glutInitWindowSize(dim_fenetre,dim_fenetre);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Visu. de données volumiques");

	glMatrixMode(GL_MODELVIEW);
	tbInitTransform();     // initialisation du point de vue 

	// Register GLUT callbacks. 
	glutReshapeFunc(redimensionnement);
	glutDisplayFunc(dessin);
	glutMouseFunc(tbMouseFunc);    // gestion traqueboule  
	glutMotionFunc(tbMotionFunc);  // gestion traqueboule 
	glutKeyboardFunc(clavier);

	glClearColor( 1.0, 1.0, 1.0, 0.0);
	glClearDepth( 1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	// calcul des images des 3 coupes (slices) à afficher 
	creer_textures_slices();

	glutMainLoop();
	return 0;             // ANSI C requires main to return int. 
}
