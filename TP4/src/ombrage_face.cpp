///////////////////////////////////////////////////////////////////////////////
// ombrages de face
//    affichage de 2 spheres facettisées s'intersectant
//    
//    les touches 1,2,3,4 permettent de choisir un niveau de facettisation
//      1 : grossier
//      2 : moyen
//      3 : fin
//      4 : tres fin
//    
//    La caméra se déplace sur un cylindre centré à l'origine, d'axe vertical, et
//    de rayon RCamera, et pointe en direction de l'origine.
//    Le déplacement de la caméra est fait en modifiant 
//    soit sa hauteur hCamera, soit sa position angulaire azimut :
//    	   
//    - le changement de la valeur de l'azimut de la camera se fait
//      à l'aide des touches j et l
//    	  
//    - le changement de la valeur de la hauteur de la camera se fait 
//      à l'aide des touches i et k
//    	  
//    - le changement de l'angle d'ouverture de la camera se fait
//      à l'aide des touches - et +
//    
//    Une direction d'illumination est définie en coordonnées sphériques 
//    (deux angles, La - azimu - et Le elevation) :  
//    - le changement de la direction de la lumiere se fait
//      à l'aide des touches g,h (azimut) et b,y (elevation)
//       	  
//    La touche q ou ESC met fin a l'execution du programme
// 
//    compilation
//      g++ ombrage_face.cpp -lglut -lGLU -lGL -o ombrage_face
// 	 
//    Nicolas SZAFRAN - UFRIM2AG - 2015
//  
///////////////////////////////////////////////////////////////////////////////

#include <GL/glut.h>
#include <cmath>
#include <cstdlib>


///////////////////////////////////////////////////////////////////////////////
// classe Point en dimension 3
class Point {
public :
	GLdouble c[3]; // les coordonnées
	
	////////////////
	// constructeurs
	
	// créer un point avec les coordonnées (x,y,z)
	Point(GLdouble x=0.0, GLdouble y=0.0, GLdouble z=0.0)
	{
		c[0] = x; c[1] = y; c[2] = z; 
	}
	
	// affectation
	Point& operator=(const Point& p)
	{
		c[0] = p.c[0];
		c[1] = p.c[1];
		c[2] = p.c[2];
		return *this; 
	}
	
	// opérations sur les points
	GLdouble norme()
	{
		return sqrt(c[0]*c[0]+c[1]*c[1]+c[2]*c[2]);
	} 
	
	Point& operator+=(const Point &p)
	{
		c[0] += p.c[0];
		c[1] += p.c[1];
		c[2] += p.c[2];
		return *this;
	}
	
	Point& operator-=(const Point &p)
	{
		c[0] -= p.c[0];
		c[1] -= p.c[1];
		c[2] -= p.c[2];
		return *this;
	}
	
	Point& operator*=(GLdouble r)
	{
		c[0] *= r;
		c[1] *= r;
		c[2] *= r;
		return *this;
	}
	
	Point& operator/=(GLdouble r)
	{
		c[0] /= r;
		c[1] /= r;
		c[2] /= r;
		return *this;
	}
	
	void normalise()
	{
		GLdouble np = norme();
		if (np>0.0)
		{
			c[0] /= np;
			c[1] /= np;
			c[2] /= np;
		}	
	}
	
	friend Point operator+(Point p1, const Point& p2)
	{
		return p1 += p2;
	}
	
	friend Point operator-(Point p1, const Point& p2)
	{
		return p1 -= p2;
	}
	
	friend Point operator*(GLdouble r, Point p)
	{
		return Point(r*p.c[0],r*p.c[1],r*p.c[2]);
	}
	
	friend GLdouble produit_scalaire(Point p1, Point p2)
	{
		return p1.c[0]*p2.c[0]+p1.c[1]*p2.c[1]+p1.c[2]*p2.c[2];
	}
	
	friend Point produit_vectoriel(Point p1, Point p2)
	{
		return Point(
		 p1.c[1]*p2.c[2]-p1.c[2]*p2.c[1],
		 p1.c[2]*p2.c[0]-p1.c[0]*p2.c[2],
		 p1.c[0]*p2.c[1]-p1.c[1]*p2.c[0]);
	}
	
};

#define Vecteur Point
#define CouleurRGB Point
#define GL_COULEUR_RGB(_c) glColor3dv(_c.c)
#define GL_VERTEX(_v)      glVertex3dv(_v.c)
#define GL_TRANSLATE(_v)   glTranslated(_v.c[0],_v.c[1],_v.c[2])

// ratio radian/degre 
GLdouble r_radian_degre = M_PI/180.0;

// la caméra  
// angle d'ouverture de la camera 
GLdouble angle_camera = 45.0;

// position angulaire de l'axe de visee de la camera 
//   par rapport a l'axe Ox 
GLdouble azimut = -45.0;       // azimut 
GLdouble delta_angle = 0.5;  
GLdouble hCamera = 0.0;    // hauteur 
GLdouble RCamera = 50.0;    // rayon de cylindre 
GLdouble delta_h = 0.5;   

GLdouble ratio_fenetre; // rapport largeur sur hauteur 

// couleur de fond 
CouleurRGB couleur_fond(0.3,0.3,0.3);  

// les différentes facettisations possibles  
int dim_facettisation[] = {10,20,40,80};
int facettisation=0;

// direction de lumiere 
GLdouble La=45.0; // azimut de la direction de la lumière 
GLdouble Le=30.0; // elevation de la direction de la lumière 
// direction de la lumière L = ( cos(La)*cos(Le) , sin(La)*cos(Le) , sin(Le) )
Vecteur  Lumiere; 

// discretisation de la surface suivant le parametre u 
int DIM1;
// discretisation de la surface suivant le parametre v 
int DIM2;

// calcul de la couleur résultante en utilisant le modèle de Lambert 
// en fonction de la normale N, la couleur de l'objet c           
// ainsi que la couleur de fond couleur_fond                               
// et la direction de lumière Lumiere                                  
// Entrée : N = normale orientée    
//          couleur_fond = couleur de fond   
//          c = couleur de l'objet   
CouleurRGB calcul_couleur_modele_lambert(Vecteur N, CouleurRGB couleur_fond, CouleurRGB c)
{
	GLdouble norme_N = N.norme();
	GLdouble l = produit_scalaire(N,Lumiere)/norme_N;
	if (l<0.0) l=0.0;
	return (1-l)*couleur_fond + l*c;
}

void sphere_unite2(CouleurRGB cS)
{
	int i,j;
	GLdouble umin = 0;
	GLdouble umax = 2.0*M_PI;
	GLdouble vmin = -M_PI/2.0;
	GLdouble vmax = +M_PI/2.0;
	
	// les faces 
	for (i=0; i<DIM1; i++)
	for (j=0; j<DIM2; j++)
	{
		Point   P11, P12, P21, P22; // les quatre sommets de la face
		Vecteur N11, N12, N21, N22; // les normales correspondantes aux sommets
		Vecteur NF;                 // la normale à la face
		CouleurRGB C11, C12, C21, C22;
		
		// face = quadrilatere correspondant au parametrage (u,v) 
		// avec u1 <= u <= u2 et v1 <= v <= v2                    
		GLdouble u1  = umin + (i+0.0)*(umax-umin)/(double)DIM1; 
		GLdouble u2  = umin + (i+1.0)*(umax-umin)/(double)DIM1; 
		GLdouble v1  = vmin + (j+0.0)*(vmax-vmin)/(double)DIM2; 
		GLdouble v2  = vmin + (j+1.0)*(vmax-vmin)/(double)DIM2; 
		
		// les quatre sommets du quadrangle 
		// sommet P11 correspondant aux parametres (u1,v1) 
		P11 = Point( cos(u1)*cos(v1) , sin(u1)*cos(v1) , sin(v1) );
		N11 = P11;
		// sommet P12 correspondant aux parametres (u1,v2) 
		P12 = Point( cos(u1)*cos(v2) , sin(u1)*cos(v2) , sin(v2) );
		N12 = P12;
		// sommet P21 correspondant aux parametres (u1,v1) 
		P21 = Point( cos(u2)*cos(v1) , sin(u2)*cos(v1) , sin(v1) );
		N21 = P21;
		// sommet P22 correspondant aux parametres (u1,v2) 
		P22 = Point( cos(u2)*cos(v2) , sin(u2)*cos(v2) , sin(v2) );
		N22 = P22;
		
		// calcul de la couleur de la face
		C11 = calcul_couleur_modele_lambert(N11, cS, couleur_fond);
		C12 = calcul_couleur_modele_lambert(N12, cS, couleur_fond);
		C21 = calcul_couleur_modele_lambert(N21, cS, couleur_fond);
		C22 = calcul_couleur_modele_lambert(N22, cS, couleur_fond);
				
		// dessin de la face 
		glBegin(GL_QUADS);
			GL_COULEUR_RGB(C11);
			GL_VERTEX(P11);
			GL_COULEUR_RGB(C12);
			GL_VERTEX(P12);
			GL_COULEUR_RGB(C22);
			GL_VERTEX(P22);
			GL_COULEUR_RGB(C21);
			GL_VERTEX(P21);
		glEnd();
	}
}

///////////////////////////////////////////////////////////////////////////////
// les ordres graphiques 
///////////////////////////////////////////////////////////////////////////////

// dessin d'une sphere unité (centre (0,0,0) et rayon 1) et de couleur cS 
void sphere_unite(CouleurRGB cS)
{
	int i,j;
	GLdouble umin = 0;
	GLdouble umax = 2.0*M_PI;
	GLdouble vmin = -M_PI/2.0;
	GLdouble vmax = +M_PI/2.0;
	
	// les faces 
	for (i=0; i<DIM1; i++)
	for (j=0; j<DIM2; j++)
	{
		Point   P11, P12, P21, P22; // les quatre sommets de la face
		Vecteur N11, N12, N21, N22; // les normales correspondantes aux sommets
		Vecteur NF;                 // la normale à la face
		CouleurRGB c;
		
		// face = quadrilatere correspondant au parametrage (u,v) 
		// avec u1 <= u <= u2 et v1 <= v <= v2                    
		GLdouble u1  = umin + (i+0.0)*(umax-umin)/(double)DIM1; 
		GLdouble u2  = umin + (i+1.0)*(umax-umin)/(double)DIM1; 
		GLdouble v1  = vmin + (j+0.0)*(vmax-vmin)/(double)DIM2; 
		GLdouble v2  = vmin + (j+1.0)*(vmax-vmin)/(double)DIM2; 
		
		// les quatre sommets du quadrangle 
		// sommet P11 correspondant aux parametres (u1,v1) 
		P11 = Point( cos(u1)*cos(v1) , sin(u1)*cos(v1) , sin(v1) );
		N11 = P11;
		// sommet P12 correspondant aux parametres (u1,v2) 
		P12 = Point( cos(u1)*cos(v2) , sin(u1)*cos(v2) , sin(v2) );
		N12 = P12;
		// sommet P21 correspondant aux parametres (u1,v1) 
		P21 = Point( cos(u2)*cos(v1) , sin(u2)*cos(v1) , sin(v1) );
		N21 = P21;
		// sommet P22 correspondant aux parametres (u1,v2) 
		P22 = Point( cos(u2)*cos(v2) , sin(u2)*cos(v2) , sin(v2) );
		N22 = P22;
		
		// calcul de la normale à la face comme 
		// moyenne des normales aux sommets
		NF = 0.25*(N11+N12+N21+N22); 
		
		// calcul de la couleur de la face
		c = calcul_couleur_modele_lambert(NF, cS, couleur_fond);
		GL_COULEUR_RGB(c);
				
		// dessin de la face 
		glBegin(GL_QUADS);
			GL_VERTEX(P11);
			GL_VERTEX(P12);
			GL_VERTEX(P22);
			GL_VERTEX(P21);
		glEnd();
	}
}

// dessin d'une sphere de centre C, de rayon r et de couleur cS 
void sphere(Point C, GLdouble r, CouleurRGB cS)
{
	// utiliser la sphère unité 
	glPushMatrix();
	GL_TRANSLATE(C);
	glScaled(r,r,r);
	sphere_unite2(cS);
	glPopMatrix();
}

void dessin() {
	GLdouble La_radian = La*r_radian_degre;
	GLdouble Le_radian = Le*r_radian_degre;
	
	// effacer la fenetre avec la couleur blanche 
	glClearColor(1.0,1.0,1.0,0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// calcul de la direction de lumiere 
	Lumiere = Point(cos(La_radian)*cos(Le_radian),
	                sin(La_radian)*cos(Le_radian),
					sin(Le_radian));

	// la scene 
	DIM1 = 2*dim_facettisation[facettisation];
	DIM2 = dim_facettisation[facettisation];
	glPushMatrix();
	{
		CouleurRGB couleur_1(1.0,0.4,0.0); // couleur de la sphere 1
		CouleurRGB couleur_2(0.4,1.0,0.0); // couleur de la sphere 2
		
		// sphere de centre (-3,-3,-3) et rayon 10 
		sphere(Point(-3.0,-3.0,-3.0), 10.0, couleur_1);			
		// sphere de centre (4,4,1) et rayon 9 
		sphere(Point( 4.0, 4.0, 1.0),  9.0, couleur_2);			
	}
	glPopMatrix();
	
	glutSwapBuffers();
}

void positionnement_camera()
{
	GLdouble Cx,Cy,Cz; // position de la camera 
	GLdouble nx,ny,nz; // direction de visee 
	GLdouble vx,vy,vz; // vecteur de vue vertical 
	GLdouble azimut_radian    = azimut*r_radian_degre;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle_camera, ratio_fenetre, 0.1, 10000.0); 
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// calcul de la position de la camera 
	Cx = RCamera*cos(azimut_radian);
	Cy = RCamera*sin(azimut_radian);
	Cz = hCamera;
	
	// calcul de la direction de visee 
	nx = -Cx;
	ny = -Cy;
	nz = -Cz;
	
	// calcul du vecteur vertical de vue 
	vx = -Cx*Cz;
	vy = -Cy*Cz;
	vz =  RCamera*RCamera;
	
	gluLookAt(Cx, Cy, Cz,
	          0.0,0.0,0.0,
			  vx, vy, vz);
}

// prise en compte d'un redimensionnement de la fenetre 
void redimensionnement(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	ratio_fenetre = (double)(w)/(double)(h);
	positionnement_camera();
}

// gestion des touches clavier 
void clavier(unsigned char key, int x, int y) 
{ 
	switch(key) 
	{ 
	
	// modification de la direction de la lumiere 
	case 'g' :  
	case 'G' :
		La -= delta_angle;
		glutPostRedisplay();
		break; 
	
	case 'h' :  
	case 'H' :
		La += delta_angle;
		glutPostRedisplay();
		break; 
	
	case 'b' :  
	case 'B' :
		Le -= delta_angle;
		glutPostRedisplay();
		break; 
	
	case 'y' :  
	case 'Y' :
		Le += delta_angle;
		glutPostRedisplay();
		break; 
	
	
	// modification de l'azimut 
	case 'j' :  
	case 'J' :
		azimut -= delta_angle;
		positionnement_camera();
		glutPostRedisplay();
		break; 
	
	case 'l' :  
	case 'L' :
		azimut += delta_angle;
		positionnement_camera();
		glutPostRedisplay();
		break; 
	
	// modification de la hauteur de la camera 
	case 'i' :  
	case 'I' :
		hCamera += delta_h;
		positionnement_camera();
		glutPostRedisplay();
		break; 
	
	case 'k' :  
	case 'K' :
		hCamera -= delta_h;
		positionnement_camera();
		glutPostRedisplay();
		break; 
	
	// modification de l'angle d'ouverture de la camera 
	case '-' :
		if (angle_camera<90.0)
			angle_camera /= 1.01;
		else
			angle_camera = 180.0-(180.0-angle_camera)*1.01;
		positionnement_camera();
		glutPostRedisplay();	
		break; 
	case '+' :
		if (angle_camera<90.0)
			angle_camera *= 1.01;
		else
			angle_camera = 180.0-(180.0-angle_camera)/1.01;
		positionnement_camera();
		glutPostRedisplay();	
		break; 
	
	// choix de la facettisation  
	case '1' :
	case '2' :
	case '3' :
	case '4' :
	{
		int cas = key-49;
		if (cas!=facettisation)
		{				
			facettisation = cas;	
			glutPostRedisplay();
		}
		break;
	}
		
	// fin du programme 
	case 27 :  
	case 'q' :  
	case 'Q' :  
		exit(0); 
		break; 
	 
	}
}

// la fonction principale 
int main(int argc, char **argv) {
	// initialisation de la librairie GLUT
	//   doit etre appelee avant toute autre instruction 
	//   de la librairie GLUT 
	glutInit(&argc,argv);
	
	// creation d'une fenetre graphique 
	//   de dimension 500 x 500
	//   en double buffer graphique, mode RGB et z-buffer 
	//   dont le nom est celui du programme 
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutCreateWindow(argv[0]);
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
		
	// boucle principale 
	glutMainLoop();
	
	return(0);
}
