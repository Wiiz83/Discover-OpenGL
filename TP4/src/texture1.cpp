///////////////////////////////////////////////////////////////////////////////
// 	Utilisation de plusieurs textures
// 	compilation :
// 		
// 	touche a/A : switch tracé aretes
// 	
//     touches x,y,z,X,Y,Z : rotation de la scene autour des axes x,y,z
// 
// 	touches +/- : zoom + et -
// 	
// 	compilation :
// 		
//     g++ textures1.cpp -lglut -lGLU -lGL -o textures1
// 
//    Nicolas SZAFRAN - UFRIM2AG - 2015
///////////////////////////////////////////////////////////////////////////////

#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

GLdouble delta_rot=2.0;  // modification d'angle 
GLboolean trace_aretes = GL_FALSE;

// position de la camera 
GLdouble cameraX=-5.0, cameraY=0.0, cameraZ=0.0;

GLdouble ratio_fenetre; // rapport largeur sur hauteur 

////////////////////////////////////////////////////////////////////////////
// gestion des textures 
////////////////////////////////////////////////////////////////////////////

/////////////////
// classe Texture 
class Texture
{
	// dimension de l'image 
	GLint L,H;	
	
	// nb de bytes du tableau image
	GLint N;
	
	// image de la texture 
	// doit etre un tableau de dimension Lligne * h
	// avec Lligne : plus petit entier multiple de 4 supérieur ou égal à 3*L
	GLubyte *image;
	
	// numéro identificateur de la texture 
	GLuint id;

	// creation de la texture correspondant à l'image PPM contenu
	//   dans le tableau t_image_ppm
	//   Entrée : t_image_ppm = tableau de 3*L*H GLubyte (unsigned char)
	//            le pixel en ligne i et colonne j est donné par :
	//			  t_image_ppm[3*(L*j+i)  ] (composante R entre 0 et 255)
	//			  t_image_ppm[3*(L*j+i)+1] (composante G entre 0 et 255)
	//			  t_image_ppm[3*(L*j+i)+2] (composante B entre 0 et 255)
	//			  avec 0<=i<H et 0<=j<L
	//			L,H : dimensions de l'image
	//   Sortie : structure correspondante
	void init_texture()
	{ 	
		
		// creation de la texture 
		// genere un numero de texture 
		glGenTextures(1,&id); 	
		// selectionne ce numero 
		glBindTexture(GL_TEXTURE_2D,id); 	
		
		glTexImage2D (
			GL_TEXTURE_2D, 	// Type : texture 2D 
			0,              // Mipmap : aucun 
			3, 				// ModeRGB (3 composantes) 
			L, 	 			// Largeur 
			H, 				// Hauteur 
			0, 				// Largeur du bord : 0 
			GL_RGB,  		// Format : RGBA 
			GL_UNSIGNED_BYTE, 	// composante de type UNSIGNED_BYTE 
			image   		// buffer de l'image de texture 
		); 	
		
		// definit le type de texture (2D) et comment elle est appliquee 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	} 

	// lecture de l'image PPM au format brut RGB - unsigned char 
	//       (valeurs entre 0 et 255)
	//  la fonction renvoie 1 si l'image a pu etre lue et 
	//  les dimensions sont correctes, 0 sinon
	int ppm2rgb(const char *nom_f)
	{
		GLint nb_bytes_ligne, i,j,k;
		char *ligne;
		size_t nligne;
		FILE *f;
		
		// ouverture du fichier 
		f = fopen(nom_f, "r");
		if (f == (FILE*)NULL)
		{
			fprintf(stderr, "ppm2rgb\n");
			fprintf(stderr, " le fichier %s ne peut etre ouvert\n", nom_f);
			return 0;	
		}
		
		// lecture ligne 1 et test 
		ligne=NULL;
		getline(&ligne, &nligne, f);
		if (ligne[0]!='P' || ligne[1]!='6')
		{
			fprintf(stderr, "ppm2rgb\n");
			fprintf(stderr, " format fichier incorrect\n");
			fclose(f);
			free(ligne);
			return 0;	
		}
		free(ligne);
		
		// lecture ligne 2 
		ligne=NULL;
		getline(&ligne, &nligne, f);
		if (ligne[0]=='#')
		{
			// ligne de commentaire -> lire la ligne suivante 
			free(ligne);
			ligne=NULL;
			getline(&ligne, &nligne, f);
		}
		sscanf(ligne,"%d %d", &L, &H);
		if (L<1 || H<1)
		{
			fprintf(stderr, "ppm2rgb\n");
			fprintf(stderr, " dimensions incorrectes (%d,%d)\n", L, H);
			fclose(f);
			free(ligne);
			return 0;	
		}
		free(ligne);
		
		// lecture ligne 3 - cas PGM ou PPM 
		ligne=NULL;
		getline(&ligne, &nligne, f);
		sscanf(ligne, "%d", &i);
		if (i!=255)
		{
			fprintf(stderr, "ppm2rgb\n");
			fprintf(stderr, " N (%d) incorrect, 255 attendu\n",i);
			fclose(f);
			free(ligne);
			return 0;	
		}
		free(ligne);
	
		// allocation de la mémoire nécessaire 
		nb_bytes_ligne = 4*((3*L+3)/4);
		N = nb_bytes_ligne*H;
		image = new GLubyte[N];
		if (!image)
		{
			fprintf(stderr, "ppm2rgb\n");
			fprintf(stderr, " allocation memoire impossible\n");
			fclose(f);
			return 0;
		}
		
		for (j=0; j<H; j++)
		{
			k=j*nb_bytes_ligne;
			// lire les L pixels de la ligne 
			for (i=0; i<L; i++)
			{
				unsigned char cRGB[3];
				fread(cRGB, 3, 1, f);
				image[k++] = cRGB[0]; 
				image[k++] = cRGB[1]; 
				image[k++] = cRGB[2]; 
			}
		}
		
		fclose(f);
		return 1;
	} 

	// créer une texture à partir d'une image de dimensions
	// L0 x H0 contenue dans le tableau Timage
	void creer_texture(GLint L0, GLint H0, GLubyte *Timage)
	{
		L = L0;
		H = H0;
		N = 4*((3*L+3)/4)*H;
		image = new GLubyte[N];
		for (int i=0; i<N; i++)
			image[i] = Timage[i];
	}

	// créer une texture basique (noir-blanc/blac-noir)
	void creer_texture_basique()
	{
		GLubyte image_basique[] = {
		    0,  0,  0   , 255,255,255 , 0,0 , // ligne 1 
		    255,255,255 ,   0,  0,  0 , 0,0}; // ligne 2
		L = 2;
		H = 2;
		N = 4*((3*L+3)/4)*H;
		image = new GLubyte[N];
		for (int i=0; i<N; i++)
			image[i] = image_basique[i];
	}

public:
	////////////////
	// constructeurs
	
	// creer une texture basique
	Texture()
	{
		creer_texture_basique();
		init_texture();
	}
	
	// créer une texture OpenGL à partir d'un fichier image au format PPM brut
	Texture(const char* nom_f)
	{
		image = NULL;
		if (ppm2rgb(nom_f)==0)
		{
			if (image) delete[] image; // supprimer le tableau image 
			
			// créer une texture basique
			creer_texture_basique();
		}
		init_texture();
	}
	
	// créer une texture OpenGL à partir d'une image de dimensions
	// L0 x H0 contenue dans le tableau Timage
	Texture(GLint L0, GLint H0, GLubyte *Timage)
	{
		creer_texture(L0, H0, Timage);
		init_texture();
	}
	
	// création par copie
	Texture(const Texture &t)
	{
		creer_texture(t.L, t.H, t.image);
		init_texture();
	}
	
	// surcharge affectation
	Texture &operator=(const Texture &t)
	{
		if (this != &t)
		{
			if (image) delete[] image; // supprimer le tableau image 
			
			creer_texture(t.L, t.H, t.image);
			init_texture();
		}
		return *this;
	}
	
	// renvoie le numéro de la texture
	int numero()
	{
		return id;
	}
	
	// destructeur
	~Texture()
	{
		if (image)
		delete[] image;
	}
};	

// texture du cube 
Texture texture_cube, tex1, tex2, tex3, tex4, tex5, tex6;

// tableau-image de texture
GLubyte image_texture1[] = {
//  colonne 1 |   colonne 2 |   +2 octets pour atteindre 8 
  0,255,0, 
  0,0,0, 
  255,255,0,
  0,0,0, 
  255,0,255, 
  255,255,255, 
  255,0,0,
  0,0,0, 
  0,255,255,
  127,127,127, 
  0,0,255, 
  0,0,0 // ligne 2 
};


////////////////////////////////////////////////////////////////////////////
// objets géométriques 3D et dessin 
////////////////////////////////////////////////////////////////////////////

// le cube3d unite 
#define NB_FACES_CUBE 6
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
	
void cube()
{
	int i;
	// les coordonnées-textures des 4 sommets 
	/*
	//                1        2        3        4        5        6                  1    2    3    4    5    6
	GLdouble tx1[] = {0.0,     1.0/3.0, 2.0/3.0, 0.0,     1.0/3.0, 2.0/3.0}, ty1[] = {0.0, 0.0, 0.0, 0.5, 0.5, 0.5};
	GLdouble tx2[] = {1.0/3.0, 2.0/3.0, 1.0,     1.0/3.0, 2.0/3.0, 1.0},     ty2[] = {0.0, 0.0, 0.0, 0.5, 0.5, 0.5}; 
	GLdouble tx3[] = {1.0/3.0, 2.0/3.0, 1.0,     1.0/3.0, 2.0/3.0, 1.0},     ty3[] = {0.5, 0.5, 0.5, 1.0, 1.0, 1.0};
	GLdouble tx4[] = {0.0,     1.0/3.0, 2.0/3.0, 0.0,     1.0/3.0, 2.0/3.0}, ty4[] = {0.5, 0.5, 0.5, 1.0, 1.0, 1.0};
	*/
	
	GLdouble tx1 = 0.0, ty1 = 0.0;
	GLdouble tx2 = 1.0, ty2 = 0.0;
	GLdouble tx3 = 1.0, ty3 = 1.0;
	GLdouble tx4 = 0.0, ty4 = 1.0;
	
	// active le texturing 
	glEnable(GL_TEXTURE_2D); 	
	
	// utiliser la texture 
	//glBindTexture(GL_TEXTURE_2D, texture_cube.numero());
	
	// tracé des faces 
	glColor3d(1.0,1.0,1.0);
	for (i=0; i<NB_FACES_CUBE; i++)
	{
		/*switch(i){
			case 0:
				glBindTexture(GL_TEXTURE_2D, tex1.numero());
				break;
			case 1:
				glBindTexture(GL_TEXTURE_2D, tex2.numero());
				break;
			case 2:
				glBindTexture(GL_TEXTURE_2D, tex3.numero());
				break;
			case 3:
				glBindTexture(GL_TEXTURE_2D, tex4.numero());
				break;
			case 4:
				glBindTexture(GL_TEXTURE_2D, tex5.numero());
				break;
			case 5:
				glBindTexture(GL_TEXTURE_2D, tex6.numero());
				break;
			default:
				glBindTexture(GL_TEXTURE_2D, texture_cube.numero());
				break;
		}*/
		int s0 = faces[i][0];
		int s1 = faces[i][1];
		int s2 = faces[i][2];
		int s3 = faces[i][3];
		
		/*
		glBegin(GL_QUADS);
			glTexCoord2d(tx1[i],ty1[i]);
			glVertex3d(sommets[s0][0], sommets[s0][1], sommets[s0][2]);
			glTexCoord2d(tx2[i],ty2[i]);
			glVertex3d(sommets[s1][0], sommets[s1][1], sommets[s1][2]);
			glTexCoord2d(tx3[i],ty3[i]);
			glVertex3d(sommets[s2][0], sommets[s2][1], sommets[s2][2]);
			glTexCoord2d(tx4[i],ty4[i]);
			glVertex3d(sommets[s3][0], sommets[s3][1], sommets[s3][2]);
		glEnd();
		
		*/
		glBegin(GL_QUADS);
			glTexCoord2d(tx1,ty1);
			glVertex3d(sommets[s0][0], sommets[s0][1], sommets[s0][2]);
			glTexCoord2d(tx2,ty2);
			glVertex3d(sommets[s1][0], sommets[s1][1], sommets[s1][2]);
			glTexCoord2d(tx3,ty3);
			glVertex3d(sommets[s2][0], sommets[s2][1], sommets[s2][2]);
			glTexCoord2d(tx4,ty4);
			glVertex3d(sommets[s3][0], sommets[s3][1], sommets[s3][2]);
		glEnd();
		
	}
	if (trace_aretes)
	{
		// tracé des aretes 
		glColor3d(0.0,0.0,0.0);
		glLineWidth(2.0);
		for (i=0; i<NB_FACES_CUBE; i++)
		{
			int s0 = faces[i][0];
			int s1 = faces[i][1];
			int s2 = faces[i][2];
			int s3 = faces[i][3];
			glBegin(GL_LINE_LOOP);
				glVertex3d(sommets[s0][0], sommets[s0][1], sommets[s0][2]);
				glVertex3d(sommets[s1][0], sommets[s1][1], sommets[s1][2]);
				glVertex3d(sommets[s2][0], sommets[s2][1], sommets[s2][2]);
				glVertex3d(sommets[s3][0], sommets[s3][1], sommets[s3][2]);
			glEnd();
		}
	}
	
	// désactive le texturing 
	glDisable(GL_TEXTURE_2D); 	
}

void dessin() 
{
	glMatrixMode(GL_MODELVIEW);

	// effacer la fenetre avec la couleur blanche 
	glClearColor(1.0,1.0,1.0,0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// les cubes 
    cube(); 
	
	glutSwapBuffers();
}

void positionnement_camera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, ratio_fenetre, 0.1, 1000.0);
	gluLookAt(cameraX,cameraY,cameraZ,
	          cameraX+1.0,cameraY,cameraZ,
			  0.0,0.0,1.0);
}

// prise en compte d'un redimensionnement de la fenetre 
void redimensionnement(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	ratio_fenetre = (double)(w)/(double)(h);
	positionnement_camera();
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
	
	// switch trace aretes 
	case 'a':
	case 'A':
		trace_aretes = !trace_aretes;	 
		glutPostRedisplay();
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

///////////////////////////////////////////////////////////////////////////////
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
	
	// definition de la fonction de dessin 
	glutDisplayFunc(dessin);
	// definition de la fonction de redimensionnement de la fenetre 
	glutReshapeFunc(redimensionnement);
	// definition de la fonction de gestion des touches clavier
	//   (touches avec code ASCII correspondant) 
	glutKeyboardFunc(clavier);
	
	// initialisation de la matrice pour le modele 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// creation des textures 
	//texture_cube = Texture(3,3,image_texture1);
 	//texture_cube = Texture("texture_tigres.ppm");
 	//texture_cube = Texture("texture_de.ppm") ;
 	
 	tex1 = Texture("texture_de_1.ppm");
 	tex2 = Texture("texture_de_2.ppm");
 	tex3 = Texture("texture_de_3.ppm");
 	tex4 = Texture("texture_de_4.ppm");
 	tex5 = Texture("texture_de_5.ppm");
 	tex6 = Texture("texture_de_6.ppm");
	
	// boucle principale 
	glutMainLoop();
	
	return(0);
}
