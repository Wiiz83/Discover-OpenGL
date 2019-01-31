///////////////////////////////////////////////////////////////////////////
// librairie � partir de Cimg - partie interface
//
// A faire :
// - afficher : traiter le cas d'image vide
///////////////////////////////////////////////////////////////////////////

#ifndef _LIB_IMAGE_H_
#define _LIB_IMAGE_H_

typedef unsigned char UINT8;
typedef unsigned long UINT;
#ifndef NULL
#define NULL ((void *)0L)
#endif

///////////////////////////////////////////////////////////////////////////
// conversion d'une composante de type float vers une composante de type UINT8
// le r�sultat est arrondi(v*255.0)
// si v<0.0 alors le r�sultat est 0
// si v>1.0 alors le r�sultat est 255
UINT8 float_to_UINT8(float v);

///////////////////////////////////////////////////////////////////////////
// conversion d'une composante de type UINT8 vers une composante de type float
// le r�sultat est v/255.0
float UINT8_to_float(UINT8 v);

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe ImageGris : image en 256 niveaux de gris
//   valeurs des pixels de type UINT8 : entier entre 0 et 255
//    0 : noir
//    entre 0 et 255 : gris
//    1 : blanc
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
class ImageGrisF;

class ImageGris
{
public :
	
	///////////////////////////////////////////////////////////////////////////
	// constructeurs
	///////////////////////////////////////////////////////////////////////////
	
	// cr�ation d'une image vide
	ImageGris();
	
	// cr�ation d'une image de largeur L0 et de hauteur H0
	// et avec les tous les pixels initialis�s � v (par d�faut v=0)
	ImageGris(UINT L0, UINT H0, UINT8 v=0);
	
	// cr�ation d'une image de largeur L0 et de hauteur H0
	// � partir d'un tableau de pixels p
	// le pixel(x,y) de l'image est p[x+y*k]
	ImageGris(UINT L0, UINT H0, UINT8* p);
	
	// cr�ation � partir d'un fichier image nomm� nom_f
	ImageGris(const char *nom_f);
	
	// constructeur par copie de I
	ImageGris(ImageGris &I);
	ImageGris(ImageGrisF &I);
	
	///////////////////////////////////////////////////////////////////////////
	// surcharges
	///////////////////////////////////////////////////////////////////////////

	// surcharge de l'op�rateur =
	ImageGris& operator=(const ImageGris& I);
	
	///////////////////////////////////////////////////////////////////////////
	// acc�s � l'image
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie la largeur
	UINT largeur();
	
	// renvoie la largeur
	UINT hauteur();
	
	// renvoie le nombre de pixels
	UINT nb_pixels();
	
	// renvoie la r�f�rence du pixel(i) = pixel(x+y*largeur)
	UINT8& pixel(UINT i);
	UINT8& operator()(UINT i);
	
	// renvoie la r�f�rence du pixel(x,y)
	UINT8& pixel(UINT x, UINT y);
	UINT8& operator()(UINT x, UINT y);
	
	// remplace le pixel (x,y) avec la valeur v
	// ne fait rien si (x,y) est hors de l'image
	void pixel(UINT x, UINT y, UINT8 v);

	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans une fenetre graphique avec le zoom z 
	// (par defaut z=1)
	// renvoie le num�ro de la fenetre cr��e
	int afficher(UINT z=1);

	///////////////////////////////////////////////////////////////////////////
	// sauvegarde de l'image dans un fichier image
	///////////////////////////////////////////////////////////////////////////
	
	// sauvegarde dans un fichier suivant l'extension du nom de fichier nom_f
	void sauvegarder(const char* nom_f);  
	
	// sauvegarde dans un fichier nomm� nom_f au format JPG 
	// avec la qualit� q (par d�faut q=100)
	// si q>100 alors q=100 est utilis�
	void sauvegarder_jpeg(const char* nom_f, UINT8 q=100);  
	
	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans la console 
	// avec le format f pour l'affichage des valeurs
	// par d�faut f = "%3d "
	///////////////////////////////////////////////////////////////////////////
	void print();
	void print(const char *f);
	
	///////////////////////////////////////////////////////////////////////////
	// destructeur
	///////////////////////////////////////////////////////////////////////////
	~ImageGris();

private:
	UINT L,H; // dimensions de l'image
	UINT8 *pixels; // le tableau de pixels correspondant
	               // pixels[i+L*j] : pixel (i,j)
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe ImageGrisF : image en �chelle de gris
//   valeurs des pixels de type r�el (float) :
//    <= 0.0 : noir
//    entre 0.0 et 1.0 : gris
//    >= 1.0 : blanc
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
class ImageGrisF
{
public :
	
	///////////////////////////////////////////////////////////////////////////
	// constructeurs
	///////////////////////////////////////////////////////////////////////////
	
	// cr�ation d'une image vide
	ImageGrisF();
	
	// cr�ation d'une image de largeur L0 et de hauteur H0
	// et avec les tous les pixels initialis�s � v (par d�faut v=0.0)
	ImageGrisF(UINT L0, UINT H0, float v=0.0);
	
	// cr�ation d'une image de largeur L0 et de hauteur H0
	// � partir d'un tableau de pixels p
	// le pixel(x,y) de l'image est p[x+y*k]
	ImageGrisF(UINT L0, UINT H0, float* p);

	// cr�ation � partir d'un fichier image nomm� nom_f
	ImageGrisF(const char *nom_f);
	
	// constructeur par copie de I
	ImageGrisF(ImageGrisF &I);
	ImageGrisF(ImageGris &I);
	
	///////////////////////////////////////////////////////////////////////////
	// surcharges
	///////////////////////////////////////////////////////////////////////////

	// surcharge de l'op�rateur =
	ImageGrisF& operator=(const ImageGrisF& I);
	
	///////////////////////////////////////////////////////////////////////////
	// acc�s � l'image
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie la largeur
	UINT largeur();
	
	// renvoie la largeur
	UINT hauteur();
	
	// renvoie le nombre de pixels
	UINT nb_pixels();
	
	// renvoie la r�f�rence du pixel(i) = pixel(x+y*largeur)
	float& pixel(UINT i);
	float& operator()(UINT i);
	
	// renvoie la r�f�rence du pixel (x,y)
	float& pixel(UINT x, UINT y);
	float& operator()(UINT x, UINT y);
	
	// remplace le pixel (x,y) avec la valeur v
	// ne fait rien si (x,y) est hors de l'image
	void pixel(UINT x, UINT y, float v);

	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans une fenetre graphique avec le zoom z 
	// (par defaut z=1)
	// renvoie le num�ro de la fenetre cr��e
	int afficher(UINT z=1);
	 
	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans la console 
	// avec le format f pour l'affichage des valeurs
	// par d�faut f = "%6.3f "
	///////////////////////////////////////////////////////////////////////////
	void print();
	void print(const char *f);
	
	///////////////////////////////////////////////////////////////////////////
	// sauvegarde de l'image dans un fichier image
	///////////////////////////////////////////////////////////////////////////
	
	// sauvegarde dans un fichier suivant l'extension du nom de fichier nom_f
	void sauvegarder(const char* nom_f);  
	
	// sauvegarde dans un fichier nomm� nom_f au format JPG 
	// avec la qualit� q (par d�faut q=100)
	// si q>100 alors q=100 est utilis�
	void sauvegarder_jpeg(const char* nom_f, UINT8 q=100);  
	
	///////////////////////////////////////////////////////////////////////////
	// destructeur
	///////////////////////////////////////////////////////////////////////////
	~ImageGrisF();

private :
	UINT L,H; // dimensions de l'image
	float *pixels; // le tableau de pixels correspondant
	               // pixels[i+L*j] : pixel (i,j)
};


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe ImageCouleur : image couleur en mode RGB 24 bits
//   valeurs des pixels : 3 entiers UINT8 entre 0 et 255
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

class RGBF;
class ImageCouleurF;
class ImageIndexee;

// classe RGB : 3 entiers de type UINT8
class RGB
{
public :
	UINT8 r,g,b;
	
	///////////////////////////////////////////////////////////////////////////
	// constructeurs
	///////////////////////////////////////////////////////////////////////////
	
	// constructeur � partir de 3 valeurs (r0,g0,b0)
	RGB(UINT8 r0=0, UINT8 g0=0, UINT8 b0=0);
	
	// constructeur � partir d'une valeur RGBF v
	RGB(RGBF v);
	
	///////////////////////////////////////////////////////////////////////////
	// conversion en niveau de gris
	UINT8 gris();
	
};

class ImageCouleur
{
public :
	
	///////////////////////////////////////////////////////////////////////////
	// constructeurs
	///////////////////////////////////////////////////////////////////////////
	
	// cr�ation d'une image vide
	ImageCouleur();
	
	// cr�ation d'une image de largeur L0 et de hauteur H0
	// et avec les tous les pixels initialis�s � v (par d�faut v=RGB(0,0,0))
	ImageCouleur(UINT L0, UINT H0, RGB v=RGB(0,0,0));
	
	// cr�ation d'une image de largeur L0 et de hauteur H0
	// � partir d'un tableau p de valeurs RGB
	// le pixel(x,y) de l'image est p[x+y*k]
	ImageCouleur(UINT L0, UINT H0, RGB* p);
	
	// cr�ation � partir d'un fichier image nomm� nom_f
	ImageCouleur(const char *nom_f);
	
	// constructeur par copie de I
	ImageCouleur(ImageCouleur &I);
	ImageCouleur(ImageCouleurF &I);
	
	///////////////////////////////////////////////////////////////////////////
	// surcharges
	///////////////////////////////////////////////////////////////////////////

	// surcharge de l'op�rateur =
	ImageCouleur& operator=(const ImageCouleur& I);
	
	///////////////////////////////////////////////////////////////////////////
	// acc�s � l'image
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie la largeur
	UINT largeur();
	
	// renvoie la largeur
	UINT hauteur();
	
	// renvoie le nombre de pixels
	UINT nb_pixels();
	
	// renvoie la r�f�rence du pixel(i) = pixel(x+y*largeur)
	RGB& pixel(UINT i);
	RGB& operator()(UINT i);
	
	// renvoie la r�f�rence RGB du pixel (x,y)
	RGB& pixel(UINT x, UINT y);
	RGB& operator()(UINT x, UINT y);
	
	// renvoie la r�f�rence de la k-eme composante du pixel (x,y)
	UINT8& comp(UINT x, UINT y, UINT k);
	// renvoie la r�f�rence de la k-eme composante du pixel i
	UINT8& comp(UINT i, UINT k);
	
	// remplace le pixel (x,y) avec la valeur RGB v
	// ne fait rien si (x,y) est hors de l'image
	void pixel(UINT x, UINT y, RGB v);


	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans une fenetre graphique avec le zoom z 
	// (par defaut z=1)
	// renvoie le num�ro de la fenetre cr��e
	int afficher(UINT z=1);
	 
	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans la console 
	// avec le format f pour l'affichage des valeurs
	///////////////////////////////////////////////////////////////////////////
	void print();
	void print(const char *f);
	
	///////////////////////////////////////////////////////////////////////////
	// sauvegarde de l'image dans un fichier image
	///////////////////////////////////////////////////////////////////////////
	
	// sauvegarde dans un fichier suivant l'extension du nom de fichier nom_f
	void sauvegarder(const char* nom_f);  
	
	// sauvegarde dans un fichier nomm� nom_f au format JPG 
	// avec la qualit� q (par d�faut q=100)
	// si q>100 alors q=100 est utilis�
	void sauvegarder_jpeg(const char* nom_f, UINT8 q=100);  
	
	///////////////////////////////////////////////////////////////////////////
	// destructeur
	///////////////////////////////////////////////////////////////////////////
	~ImageCouleur();

private :
	UINT L,H; // dimensions de l'image
	RGB *pixels; // le tableau de pixels correspondant
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe ImageCouleurF : image couleur en mode RGB 
//   valeurs des pixels : 3 r�els de type float 
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// classe RGBF : 3 r�els de type float
class RGBF
{
public :
	float r,g,b;
	
	///////////////////////////////////////////////////////////////////////////
	// constructeurs
	///////////////////////////////////////////////////////////////////////////
	
	// constructeur � partir de 3 valeurs (r0,g0,b0)
	RGBF(float r0=0.0, float g0=0.0, float b0=0.0);

	// constructeur � partir d'une valeur RGB v
	RGBF(RGB v);

	///////////////////////////////////////////////////////////////////////////
	// conversion en niveau de gris
	float gris();
	
};

class ImageCouleurF
{
public :
	
	///////////////////////////////////////////////////////////////////////////
	// constructeurs
	///////////////////////////////////////////////////////////////////////////
	
	// cr�ation d'une image vide
	ImageCouleurF();
	
	// cr�ation d'une image de largeur L0 et de hauteur H0
	// et avec les tous les pixels initialis�s � v (par d�faut v=RGB(0,0,0))
	ImageCouleurF(UINT L0, UINT H0, RGBF v=RGBF(0,0,0));
	
	// cr�ation d'une image de largeur L0 et de hauteur H0
	// � partir d'un tableau de pixels p
	// le pixel(x,y) de l'image est p[x+y*k]
	ImageCouleurF(UINT L0, UINT H0, RGBF* p);
	
	// cr�ation � partir d'un fichier image nomm� nom_f
	ImageCouleurF(const char *nom_f);
	
	// constructeur par copie de I
	ImageCouleurF(ImageCouleurF &I);
	ImageCouleurF(ImageCouleur &I);
	
	///////////////////////////////////////////////////////////////////////////
	// surcharges
	///////////////////////////////////////////////////////////////////////////

	// surcharge de l'op�rateur =
	ImageCouleurF& operator=(const ImageCouleurF& I);
	
	///////////////////////////////////////////////////////////////////////////
	// acc�s � l'image
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie la largeur
	UINT largeur();
	
	// renvoie la largeur
	UINT hauteur();
	
	// renvoie le nombre de pixels
	UINT nb_pixels();
	
	// renvoie la r�f�rence du pixel(i) = pixel(x+y*largeur)
	RGBF& pixel(UINT i);
	RGBF& operator()(UINT i);
	
	// renvoie la r�f�rence RGB du pixel (x,y)
	RGBF& pixel(UINT x, UINT y);
	RGBF& operator()(UINT x, UINT y);
	
	// renvoie la r�f�rence de la k-eme composante du pixel (x,y)
	float& comp(UINT x, UINT y, UINT k);
	// renvoie la r�f�rence de la k-eme composante du pixel i
	float& comp(UINT i, UINT k);
	
	// remplace le pixel (x,y) avec la valeur RGB v
	// ne fait rien si (x,y) est hors de l'image
	void pixel(UINT x, UINT y, RGBF v);

	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans une fenetre graphique avec le zoom z 
	// (par defaut z=1)
	// renvoie le num�ro de la fenetre cr��e
	int afficher(UINT z=1);
	 
	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans la console 
	// avec le format f pour l'affichage des valeurs
	///////////////////////////////////////////////////////////////////////////
	void print();
	void print(const char *f);
	
	///////////////////////////////////////////////////////////////////////////
	// sauvegarde de l'image dans un fichier image
	///////////////////////////////////////////////////////////////////////////
	
	// sauvegarde dans un fichier suivant l'extension du nom de fichier nom_f
	void sauvegarder(const char* nom_f);  
	
	// sauvegarde dans un fichier nomm� nom_f au format JPG 
	// avec la qualit� q (par d�faut q=100)
	// si q>100 alors q=100 est utilis�
	void sauvegarder_jpeg(const char* nom_f, UINT8 q=100);  
	
	///////////////////////////////////////////////////////////////////////////
	// destructeur
	///////////////////////////////////////////////////////////////////////////
	~ImageCouleurF();

private :
	UINT L,H; // dimensions de l'image
	RGBF *pixels; // le tableau de pixels correspondant
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe ImageIndexee : image couleur en mode indexe 
//   table de N couleurs RGB (1 <= N <= 256)
//   valeurs des pixels entre 0 et N-1
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
class ImageIndexee
{
public :

	///////////////////////////////////////////////////////////////////////////
	// constructeurs
	///////////////////////////////////////////////////////////////////////////
	
	// cr�ation d'une image vide
	ImageIndexee();
	
	// cr�ation d'une image de dimension L0 par H0
	// avec une table 2 couleurs ( 0:Blanc , 1:Noir )
	// et un tableau de pixels p et
	// si p = NULL alors les tous les pixels de l'image sont initialis�s � 0
	ImageIndexee(UINT L0, UINT H0, UINT *p = (UINT*)NULL);
	
	// cr�ation d'une image de dimension L0 par H0
	// avec N0 couleurs avec un tableau de pixels p et
	// une table de couleurs T
	// si p = NULL alors les tous les pixels de l'image sont initialis�s � 0
	// si T = NULL alors une table avec toutes les couleurs noires
	ImageIndexee(UINT L0, UINT H0, UINT N, RGB *T, UINT *p = (UINT*)NULL);
	
	// constructeur par copie de I
	ImageIndexee(ImageIndexee &I);
	
	///////////////////////////////////////////////////////////////////////////
	// surcharges
	///////////////////////////////////////////////////////////////////////////

	// surcharge de l'op�rateur =
	ImageIndexee& operator=(const ImageIndexee& I);
	
	///////////////////////////////////////////////////////////////////////////
	// acc�s � l'image
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie la largeur
	UINT largeur();
	
	// renvoie la largeur
	UINT hauteur();
	
	// renvoie le nombre de pixels
	UINT nb_pixels();
	
	// renvoie le nombre de couleurs de la table
	UINT nb_couleurs();
	
	// renvoie la r�f�rence du pixel(i) = pixel(x+y*largeur)
	UINT& pixel(UINT i);
	UINT& operator()(UINT i);
	
	// renvoie la r�ference du pixel (x,y)
	UINT& pixel(UINT x, UINT y);
	UINT& operator()(UINT x, UINT y);
	
	// renvoie la couleur d'indice i
	// renvoie RGB(0,0,0) c'est � dire noir si i>=N
	RGB couleur(UINT i);
	
	// modifie le pixel (x,y) avec la valeur i
	// ne fait rien si (x,y) est hors de l'image ou si i>=N
	void pixel(UINT x, UINT y, UINT i);
	
	// modifie la couleur d'indice i avec la couleur RGB c
	// ne fait rien si i>=N
	void couleur(UINT i, RGB c);
	
	// modifie la couleur d'indice i avec la couleur RGB (r,g,b)
	// ne fait rien si i>=N
	void couleur(UINT i, UINT8 r, UINT8 g, UINT8 b);
	
	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans une fenetre graphique avec le zoom z 
	// (par defaut z=1)
	// renvoie le num�ro de la fenetre cr��e
	int afficher(UINT z=1);
	 
	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans la console 
	// avec le format f pour l'affichage des valeurs
	///////////////////////////////////////////////////////////////////////////
	void print();
	void print(const char *f);
	
	///////////////////////////////////////////////////////////////////////////
	// sauvegarde de l'image dans un fichier image
	///////////////////////////////////////////////////////////////////////////
	
	// sauvegarde dans un fichier suivant l'extension du nom de fichier nom_f
	void sauvegarder(const char* nom_f);  
	
	// sauvegarde dans un fichier nomm� nom_f au format JPG 
	// avec la qualit� q (par d�faut q=100)
	// si q>100 alors q=100 est utilis�
	void sauvegarder_jpeg(const char* nom_f, UINT8 q=100);  
	
	///////////////////////////////////////////////////////////////////////////
	// destructeur
	///////////////////////////////////////////////////////////////////////////
	~ImageIndexee();

private :
	UINT L,H; // dimensions de l'image
	UINT *pixels; // le tableau de pixels correspondant
	UINT N; // nombre de couleurs de la table
	RGB *table; // la table de couleurs
};


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe Histogramme : histogramme d'une Image
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
	
class Histogramme
{
public :
	UINT n; // nb de classes
	UINT *classes; // tableau des classes
	
	///////////////////////////////////////////////////////////////////////////
	// constructeurs
	///////////////////////////////////////////////////////////////////////////
	
	// Histogramme d'une image en niveaux de gris I avec n0 classes
	// par d�faut n0=256
	// si n0=0 alors une seule classe est choisie
	// si n0>256 alors 256 classes sont choisies
	Histogramme(ImageGris& I, UINT n0=256);
	Histogramme(ImageGrisF& I, UINT n0=256);
	
	// Histogramme d'une image couleur I avec n0 classes
	// par d�faut n0=256
	// si n0=0 alors une seule classe est choisie
	// si n0>256 alors 256 classes sont choisies
	// pour le calcul de l'histogramme, 
	// on prend en compte les composantes R, G et/ou B
	// suivant les valeurs respectives des boole�ns cR, cG et cB
	// si cR=cG=cB=false alors l'histogramme est calcul� sur l'image
	// convertie en niveaux de gris 
	Histogramme(ImageCouleur& I, UINT n0=256, 
	  bool cR=true, bool cG=true, bool cB=true);
	Histogramme(ImageCouleurF& I, UINT n0=256, 
	  bool cR=true, bool cG=true, bool cB=true);
	
	///////////////////////////////////////////////////////////////////////////
	// acc�s � l'histogramme
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie le nombre de classes de l'histogramme
	UINT nb_classes();
	
	// renvoie l'effectif de la classe i (i entre 0 et nb_classes()-1)
	// si i>=nb_classes alors la fonction renvoie 0
	UINT classe(UINT i);
	
	// d�terminer la valeur max de H, i.e. l'effectif maximal parmi les classes
	UINT max();
	
	///////////////////////////////////////////////////////////////////////////
	// afficher l'histogramme dans une fenetre graphique de dimensions L x H
	// par d�faut L = 511 et H = 250
	// renvoie le num�ro de la fenetre cr��e
	int afficher(UINT L=511, UINT H=250);
	 
	///////////////////////////////////////////////////////////////////////////
	// afficher l'histogramme dans une fenetre graphique de dimensions L x H
	// avec cF la couleur de fond et cH la couleur de l'histogramme 
	// par d�faut L = 511 et H = 250
	// renvoie le num�ro de la fenetre cr��e
	int afficher(RGB cF, RGB cH, UINT L=511, UINT H=250);
	 
	///////////////////////////////////////////////////////////////////////////
	// afficher l'histogramme dans une fenetre graphique de dimensions L x H
	// avec cF la couleur de fond et cH la couleur de l'histogramme 
	// par d�faut L = 511, H = 250, cF=Noir, cH=Rouge
	// affiche l'axe x avec valeurs num�riques entre 0.0 et 1.0 
	// dans une bande de 15 pixels au dessous de l'histogramme
	// la partie pour l'affichage de l'histogramme est au minimum de 30 pixels
	// renvoie le num�ro de la fenetre cr��e
	int afficher_0_1(RGB cF, RGB cH, UINT L=511, UINT H=250);
	int afficher_0_1(UINT L=511, UINT H=250, 
	  RGB cF = RGB(0,0,0), RGB cH = RGB(255,0,0));
	
	///////////////////////////////////////////////////////////////////////////
	// afficher l'histogramme dans une fenetre graphique de dimensions L x H
	// avec cF la couleur de fond et cH la couleur de l'histogramme 
	// par d�faut L = 511, H = 250, cF=Noir, cH=Rouge
	// affiche l'axe x avec valeurs num�riques entre 0 et 255
	// dans une bande de 15 pixels au dessous de l'histogramme
	// la partie pour l'affichage de l'histogramme est au minimum de 30 pixels
	// renvoie le num�ro de la fenetre cr��e
	int afficher_0_255(RGB cF, RGB cH, UINT L=511, UINT H=250);
	int afficher_0_255(UINT L=511, UINT H=250, 
	  RGB cF = RGB(0,0,0), RGB cH = RGB(255,0,0));
	
	///////////////////////////////////////////////////////////////////////////
	// afficher l'histogramme dans la console 
	// avec le format f pour l'affichage des valeurs
	///////////////////////////////////////////////////////////////////////////
	void print();
	void print(const char *f);
	
	///////////////////////////////////////////////////////////////////////////
	// destructeur
	///////////////////////////////////////////////////////////////////////////
	~Histogramme();

	void *data; // donn�es internes
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe Fonction : fonction d�finie sur [0,1] et � valeurs r�elles
//                   x --> f(x)
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

class Fonction
{
public :
	float (*f)(float); // la d�finition de la fonction
	
	///////////////////////////////////////////////////////////////////////////
	// constructeurs
	///////////////////////////////////////////////////////////////////////////
	
	// cr�ation � partir d'une fonction f0 d�finie avec le profil
	// float f0(float x)
	Fonction(float (*f0)(float));
	
	///////////////////////////////////////////////////////////////////////////
	// utilisation de la fonction
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie la valeur f(x) avec x de type float
	float operator()(float x);
	
	// renvoie la valeur f(I) avec I de type ImageGrisF
	ImageGrisF operator()(ImageGrisF &I);
	
	// renvoie la valeur f(x) avec x de type ImageCouleurF
	// la fonction f est appliqu�e aux trois composantes R, G et B
	ImageCouleurF operator()(ImageCouleurF &I);
	
	///////////////////////////////////////////////////////////////////////////
	// afficher la fonction dans une fenetre graphique de dimensions L x H
	// par d�faut L = 400 et H = 400
	// le rep�re pour le trac� est x entre 0 et 1 et y entre 0 et 1
	// renvoie le num�ro de la fenetre cr��e
	int afficher(UINT L=400, UINT H=400);
	 
};


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// partie affichage � l'�cran des images, histogrammes, fonctions
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// renvoie la largeur de l'�cran
UINT L_ECRAN();

// renvoie la hauteur de l'�cran
UINT H_ECRAN();

// changer le titre de la fenetre de numero nf avec la chaine de caracteres t
void changer_titre_fenetre_graphique(int nf, const char *t);

// supprimer la fenetre graphique de numero nf
void supprimer_fenetre_graphique(int nf);

#endif
