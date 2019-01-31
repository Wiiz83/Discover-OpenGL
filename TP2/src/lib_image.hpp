///////////////////////////////////////////////////////////////////////////
// librairie à partir de Cimg - partie interface
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
// le résultat est arrondi(v*255.0)
// si v<0.0 alors le résultat est 0
// si v>1.0 alors le résultat est 255
UINT8 float_to_UINT8(float v);

///////////////////////////////////////////////////////////////////////////
// conversion d'une composante de type UINT8 vers une composante de type float
// le résultat est v/255.0
float UINT8_to_float(UINT8 v);

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe ImageGris : image en 256 niveaux de gris
//   valeurs des pixels de type UINT8 : entier entre 0 et 255
//    0 : noir
//    entre 0 et 255 : ensemble des gris 
//    255 : blanc
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
	
	// création d'une image vide
	ImageGris();
	
	// création d'une image de largeur L0 et de hauteur H0
	// et avec les tous les pixels initialisés à v (par défaut v=0)
	ImageGris(UINT L0, UINT H0, UINT8 v=0);
	
	// création d'une image de largeur L0 et de hauteur H0
	// à partir d'un tableau de pixels p
	// le pixel(x,y) de l'image est p[x+y*k]
	ImageGris(UINT L0, UINT H0, UINT8* p);
	
	// création à partir d'un fichier image nommé nom_f
	ImageGris(const char *nom_f);
	
	// constructeur par copie de I
	ImageGris(ImageGris &I);
	ImageGris(ImageGrisF &I);
	
	///////////////////////////////////////////////////////////////////////////
	// surcharges
	///////////////////////////////////////////////////////////////////////////

	// surcharge de l'opérateur =
	ImageGris& operator=(const ImageGris& I);
	
	///////////////////////////////////////////////////////////////////////////
	// accès à l'image
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie la largeur
	UINT largeur();
	
	// renvoie la largeur
	UINT hauteur();
	
	// renvoie le nombre de pixels
	UINT nb_pixels();
	
	// renvoie la référence du pixel(i) = pixel(x+y*largeur)
	UINT8& pixel(UINT i);
	UINT8& operator()(UINT i);
	
	// renvoie la référence du pixel(x,y)
	UINT8& pixel(UINT x, UINT y);
	UINT8& operator()(UINT x, UINT y);
	
	// remplace le pixel (x,y) avec la valeur v
	// ne fait rien si (x,y) est hors de l'image
	void pixel(UINT x, UINT y, UINT8 v);

	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans une fenetre graphique avec le zoom z 
	// (par defaut z=1)
	// renvoie le numéro de la fenetre créée
	int afficher(UINT z=1);

	///////////////////////////////////////////////////////////////////////////
	// sauvegarde de l'image dans un fichier image
	///////////////////////////////////////////////////////////////////////////
	
	// sauvegarde dans un fichier suivant l'extension du nom de fichier nom_f
	void sauvegarder(const char* nom_f);  
	
	// sauvegarde dans un fichier nommé nom_f au format JPG 
	// avec la qualité q (par défaut q=100)
	// si q>100 alors q=100 est utilisé
	void sauvegarder_jpeg(const char* nom_f, UINT8 q=100);  
	
	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans la console 
	// avec le format f pour l'affichage des valeurs
	// par défaut f = "%3d "
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
// Classe ImageGrisF : image en échelle de gris
//   valeurs des pixels de type réel (float) :
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
	
	// création d'une image vide
	ImageGrisF();
	
	// création d'une image de largeur L0 et de hauteur H0
	// et avec les tous les pixels initialisés à v (par défaut v=0.0)
	ImageGrisF(UINT L0, UINT H0, float v=0.0);
	
	// création d'une image de largeur L0 et de hauteur H0
	// à partir d'un tableau de pixels p
	// le pixel(x,y) de l'image est p[x+y*k]
	ImageGrisF(UINT L0, UINT H0, float* p);

	// création à partir d'un fichier image nommé nom_f
	ImageGrisF(const char *nom_f);
	
	// constructeur par copie de I
	ImageGrisF(ImageGrisF &I);
	ImageGrisF(ImageGris &I);
	
	///////////////////////////////////////////////////////////////////////////
	// surcharges
	///////////////////////////////////////////////////////////////////////////

	// surcharge de l'opérateur =
	ImageGrisF& operator=(const ImageGrisF& I);
	
	///////////////////////////////////////////////////////////////////////////
	// accès à l'image
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie la largeur
	UINT largeur();
	
	// renvoie la largeur
	UINT hauteur();
	
	// renvoie le nombre de pixels
	UINT nb_pixels();
	
	// renvoie la référence du pixel(i) = pixel(x+y*largeur)
	float& pixel(UINT i);
	float& operator()(UINT i);
	
	// renvoie la référence du pixel (x,y)
	float& pixel(UINT x, UINT y);
	float& operator()(UINT x, UINT y);
	
	// remplace le pixel (x,y) avec la valeur v
	// ne fait rien si (x,y) est hors de l'image
	void pixel(UINT x, UINT y, float v);

	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans une fenetre graphique avec le zoom z 
	// (par defaut z=1)
	// renvoie le numéro de la fenetre créée
	int afficher(UINT z=1);
	 
	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans la console 
	// avec le format f pour l'affichage des valeurs
	// par défaut f = "%6.3f "
	///////////////////////////////////////////////////////////////////////////
	void print();
	void print(const char *f);
	
	///////////////////////////////////////////////////////////////////////////
	// sauvegarde de l'image dans un fichier image
	///////////////////////////////////////////////////////////////////////////
	
	// sauvegarde dans un fichier suivant l'extension du nom de fichier nom_f
	void sauvegarder(const char* nom_f);  
	
	// sauvegarde dans un fichier nommé nom_f au format JPG 
	// avec la qualité q (par défaut q=100)
	// si q>100 alors q=100 est utilisé
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
	
	// constructeur à partir de 3 valeurs (r0,g0,b0)
	RGB(UINT8 r0=0, UINT8 g0=0, UINT8 b0=0);
	
	// constructeur à partir d'une valeur RGBF v
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
	
	// création d'une image vide
	ImageCouleur();
	
	// création d'une image de largeur L0 et de hauteur H0
	// et avec les tous les pixels initialisés à v (par défaut v=RGB(0,0,0))
	ImageCouleur(UINT L0, UINT H0, RGB v=RGB(0,0,0));
	
	// création d'une image de largeur L0 et de hauteur H0
	// à partir d'un tableau p de valeurs RGB
	// le pixel(x,y) de l'image est p[x+y*k]
	ImageCouleur(UINT L0, UINT H0, RGB* p);
	
	// création à partir d'un fichier image nommé nom_f
	ImageCouleur(const char *nom_f);
	
	// constructeur par copie de I
	ImageCouleur(ImageCouleur &I);
	ImageCouleur(ImageCouleurF &I);
	
	///////////////////////////////////////////////////////////////////////////
	// surcharges
	///////////////////////////////////////////////////////////////////////////

	// surcharge de l'opérateur =
	ImageCouleur& operator=(const ImageCouleur& I);
	
	///////////////////////////////////////////////////////////////////////////
	// accès à l'image
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie la largeur
	UINT largeur();
	
	// renvoie la largeur
	UINT hauteur();
	
	// renvoie le nombre de pixels
	UINT nb_pixels();
	
	// renvoie la référence du pixel(i) = pixel(x+y*largeur)
	RGB& pixel(UINT i);
	RGB& operator()(UINT i);
	
	// renvoie la référence RGB du pixel (x,y)
	RGB& pixel(UINT x, UINT y);
	RGB& operator()(UINT x, UINT y);
	
	// renvoie la référence de la k-eme composante du pixel (x,y)
	UINT8& comp(UINT x, UINT y, UINT k);
	// renvoie la référence de la k-eme composante du pixel i
	UINT8& comp(UINT i, UINT k);

	// renvoie la référence de chaque composante du pixel (x,y)
	UINT8& R(UINT x, UINT y);
	UINT8& G(UINT x, UINT y);
	UINT8& B(UINT x, UINT y);
		
	// renvoie la référence de chaque composante du pixel i
	UINT8& R(UINT i);
	UINT8& G(UINT i);
	UINT8& B(UINT i);
		
	// remplace le pixel (x,y) avec la valeur RGB v
	// ne fait rien si (x,y) est hors de l'image
	void pixel(UINT x, UINT y, RGB v);


	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans une fenetre graphique avec le zoom z 
	// (par defaut z=1)
	// renvoie le numéro de la fenetre créée
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
	
	// sauvegarde dans un fichier nommé nom_f au format JPG 
	// avec la qualité q (par défaut q=100)
	// si q>100 alors q=100 est utilisé
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
//   valeurs des pixels : 3 réels de type float 
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// classe RGBF : 3 réels de type float
class RGBF
{
public :
	float r,g,b;
	
	///////////////////////////////////////////////////////////////////////////
	// constructeurs
	///////////////////////////////////////////////////////////////////////////
	
	// constructeur à partir de 3 valeurs (r0,g0,b0)
	RGBF(float r0=0.0, float g0=0.0, float b0=0.0);

	// constructeur à partir d'une valeur RGB v
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
	
	// création d'une image vide
	ImageCouleurF();
	
	// création d'une image de largeur L0 et de hauteur H0
	// et avec les tous les pixels initialisés à v (par défaut v=RGB(0,0,0))
	ImageCouleurF(UINT L0, UINT H0, RGBF v=RGBF(0,0,0));
	
	// création d'une image de largeur L0 et de hauteur H0
	// à partir d'un tableau de pixels p
	// le pixel(x,y) de l'image est p[x+y*k]
	ImageCouleurF(UINT L0, UINT H0, RGBF* p);
	
	// création à partir d'un fichier image nommé nom_f
	ImageCouleurF(const char *nom_f);
	
	// constructeur par copie de I
	ImageCouleurF(ImageCouleurF &I);
	ImageCouleurF(ImageCouleur &I);
	
	///////////////////////////////////////////////////////////////////////////
	// surcharges
	///////////////////////////////////////////////////////////////////////////

	// surcharge de l'opérateur =
	ImageCouleurF& operator=(const ImageCouleurF& I);
	
	///////////////////////////////////////////////////////////////////////////
	// accès à l'image
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie la largeur
	UINT largeur();
	
	// renvoie la largeur
	UINT hauteur();
	
	// renvoie le nombre de pixels
	UINT nb_pixels();
	
	// renvoie la référence du pixel(i) = pixel(x+y*largeur)
	RGBF& pixel(UINT i);
	RGBF& operator()(UINT i);
	
	// renvoie la référence RGB du pixel (x,y)
	RGBF& pixel(UINT x, UINT y);
	RGBF& operator()(UINT x, UINT y);
	
	// renvoie la référence de la k-eme composante du pixel (x,y)
	float& comp(UINT x, UINT y, UINT k);
	// renvoie la référence de la k-eme composante du pixel i
	float& comp(UINT i, UINT k);
	
	// renvoie la référence de chaque composante du pixel (x,y)
	float& R(UINT x, UINT y);
	float& G(UINT x, UINT y);
	float& B(UINT x, UINT y);
		
	// renvoie la référence de chaque composante du pixel i
	float& R(UINT i);
	float& G(UINT i);
	float& B(UINT i);
		
	// remplace le pixel (x,y) avec la valeur RGB v
	// ne fait rien si (x,y) est hors de l'image
	void pixel(UINT x, UINT y, RGBF v);

	///////////////////////////////////////////////////////////////////////////
	// afficher l'image dans une fenetre graphique avec le zoom z 
	// (par defaut z=1)
	// renvoie le numéro de la fenetre créée
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
	
	// sauvegarde dans un fichier nommé nom_f au format JPG 
	// avec la qualité q (par défaut q=100)
	// si q>100 alors q=100 est utilisé
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
	
	// création d'une image vide
	ImageIndexee();
	
	// création d'une image de dimension L0 par H0
	// avec une table 2 couleurs ( 0:Blanc , 1:Noir )
	// et un tableau de pixels p et
	// si p = NULL alors les tous les pixels de l'image sont initialisés à 0
	ImageIndexee(UINT L0, UINT H0, UINT *p = (UINT*)NULL);
	
	// création d'une image de dimension L0 par H0
	// avec N0 couleurs avec un tableau de pixels p et
	// une table de couleurs T
	// si p = NULL alors les tous les pixels de l'image sont initialisés à 0
	// si T = NULL alors une table avec toutes les couleurs noires
	ImageIndexee(UINT L0, UINT H0, UINT N, RGB *T, UINT *p = (UINT*)NULL);
	
	// constructeur par copie de I
	ImageIndexee(ImageIndexee &I);
	
	///////////////////////////////////////////////////////////////////////////
	// surcharges
	///////////////////////////////////////////////////////////////////////////

	// surcharge de l'opérateur =
	ImageIndexee& operator=(const ImageIndexee& I);
	
	///////////////////////////////////////////////////////////////////////////
	// accès à l'image
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie la largeur
	UINT largeur();
	
	// renvoie la largeur
	UINT hauteur();
	
	// renvoie le nombre de pixels
	UINT nb_pixels();
	
	// renvoie le nombre de couleurs de la table
	UINT nb_couleurs();
	
	// renvoie la référence du pixel(i) = pixel(x+y*largeur)
	UINT& pixel(UINT i);
	UINT& operator()(UINT i);
	
	// renvoie la réference du pixel (x,y)
	UINT& pixel(UINT x, UINT y);
	UINT& operator()(UINT x, UINT y);
	
	// renvoie la couleur d'indice i
	// renvoie RGB(0,0,0) c'est à dire noir si i>=N
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
	// renvoie le numéro de la fenetre créée
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
	
	// sauvegarde dans un fichier nommé nom_f au format JPG 
	// avec la qualité q (par défaut q=100)
	// si q>100 alors q=100 est utilisé
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
	// par défaut n0=256
	// si n0=0 alors une seule classe est choisie
	// si n0>256 alors 256 classes sont choisies
	Histogramme(ImageGris& I, UINT n0=256);
	Histogramme(ImageGrisF& I, UINT n0=256);
	
	// Histogramme d'une image couleur I avec n0 classes
	// par défaut n0=256
	// si n0=0 alors une seule classe est choisie
	// si n0>256 alors 256 classes sont choisies
	// pour le calcul de l'histogramme, 
	// on prend en compte les composantes R, G et/ou B
	// suivant les valeurs respectives des booleéns cR, cG et cB
	// si cR=cG=cB=false alors l'histogramme est calculé sur l'image
	// convertie en niveaux de gris 
	Histogramme(ImageCouleur& I, UINT n0=256, 
	  bool cR=true, bool cG=true, bool cB=true);
	Histogramme(ImageCouleurF& I, UINT n0=256, 
	  bool cR=true, bool cG=true, bool cB=true);
	
	///////////////////////////////////////////////////////////////////////////
	// accès à l'histogramme
	///////////////////////////////////////////////////////////////////////////
	
	// renvoie le nombre de classes de l'histogramme
	UINT nb_classes();
	
	// renvoie l'effectif de la classe i (i entre 0 et nb_classes()-1)
	// si i>=nb_classes alors la fonction renvoie 0
	UINT classe(UINT i);
	
	// déterminer la valeur max de H, i.e. l'effectif maximal parmi les classes
	UINT max();
	
	///////////////////////////////////////////////////////////////////////////
	// afficher l'histogramme dans une fenetre graphique de dimensions L x H
	// par défaut L = 511 et H = 250
	// renvoie le numéro de la fenetre créée
	int afficher(UINT L=511, UINT H=250);
	 
	///////////////////////////////////////////////////////////////////////////
	// afficher l'histogramme dans une fenetre graphique de dimensions L x H
	// avec cF la couleur de fond et cH la couleur de l'histogramme 
	// par défaut L = 511 et H = 250
	// renvoie le numéro de la fenetre créée
	int afficher(RGB cF, RGB cH, UINT L=511, UINT H=250);
	 
	///////////////////////////////////////////////////////////////////////////
	// afficher l'histogramme dans une fenetre graphique de dimensions L x H
	// avec cF la couleur de fond et cH la couleur de l'histogramme 
	// par défaut L = 511, H = 250, cF=Noir, cH=Rouge
	// affiche l'axe x avec valeurs numériques entre 0.0 et 1.0 
	// dans une bande de 15 pixels au dessous de l'histogramme
	// la partie pour l'affichage de l'histogramme est au minimum de 30 pixels
	// renvoie le numéro de la fenetre créée
	int afficher_0_1(RGB cF, RGB cH, UINT L=511, UINT H=250);
	int afficher_0_1(UINT L=511, UINT H=250, 
	  RGB cF = RGB(0,0,0), RGB cH = RGB(255,0,0));
	
	///////////////////////////////////////////////////////////////////////////
	// afficher l'histogramme dans une fenetre graphique de dimensions L x H
	// avec cF la couleur de fond et cH la couleur de l'histogramme 
	// par défaut L = 511, H = 250, cF=Noir, cH=Rouge
	// affiche l'axe x avec valeurs numériques entre 0 et 255
	// dans une bande de 15 pixels au dessous de l'histogramme
	// la partie pour l'affichage de l'histogramme est au minimum de 30 pixels
	// renvoie le numéro de la fenetre créée
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

	void *data; // données internes
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe Fonction : fonction définie sur [0,1] et à valeurs réelles
//                   x --> f(x)
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

class Fonction
{
public :
	float (*f)(float); // la définition de la fonction
	
	///////////////////////////////////////////////////////////////////////////
	// constructeurs
	///////////////////////////////////////////////////////////////////////////
	
	// création à partir d'une fonction f0 définie avec le profil
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
	// la fonction f est appliquée aux trois composantes R, G et B
	ImageCouleurF operator()(ImageCouleurF &I);
	
	///////////////////////////////////////////////////////////////////////////
	// afficher la fonction dans une fenetre graphique de dimensions L x H
	// par défaut L = 400 et H = 400
	// le repère pour le tracé est x entre 0 et 1 et y entre 0 et 1
	// renvoie le numéro de la fenetre créée
	int afficher(UINT L=400, UINT H=400);
	 
};


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// partie affichage à l'écran des images, histogrammes, fonctions
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// renvoie la largeur de l'écran
UINT L_ECRAN();

// renvoie la hauteur de l'écran
UINT H_ECRAN();

// changer le titre de la fenetre de numero nf avec la chaine de caracteres t
void changer_titre_fenetre_graphique(int nf, const char *t);

// supprimer la fenetre graphique de numero nf
void supprimer_fenetre_graphique(int nf);


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//                                                                         
//  opérations mathématiques sur des images de type ImageGrisF             
//                                                                         
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// récuperer l'imagette de dimensions 8x8 à partir de la 
//   position (x0,y0) dans l'image I
// Entrée : I = image 
//          x0, y0 = coordonnées du pixel de base
// La fonction renvoie une image de dimension 8x8
ImageGrisF imagette8x8(ImageGrisF &I, UINT x0, UINT y0);

// place l'imagette I8 de dimensions 8x8 à partir de la 
//   position (x0,y0) dans l'image I
// Entrée : I = image 
//          x0, y0 = coordonnées du pixel de base
//          I8 = image de dimension 8x8
// Sortie : I = image modifiée
void imagette8x8(ImageGrisF &I, UINT x0, UINT y0, ImageGrisF &I8);

// calcule le produit des images I1 et I2 considérées comme des matrices
// Entrée : I1 = image de dimensions l1 x h1 (matrice de dim h1 x l1)
//          I2 = image de dimensions l2 x h2 (matrice de dim h2 x l2)
// La fonction renvoie une image de dimension l2 x h1 (matrice de dim h1 x l2)
// Si l1 et h2 ne sont pas égaux, le calcul est fait avec le min de l1 et h2
ImageGrisF operator*(ImageGrisF I1, ImageGrisF I2);

// calcule la valeur min de l'image I
// Entrée : I = image de dimension l x h
float min(ImageGrisF& I);

// calcule la valeur max de l'image I
// Entrée : I = image de dimension l x h
float max(ImageGrisF& I);

// calcule min(I,v)
// Entrée : I = image de dimension l x h
//          v = réel de type float
// La fonction renvoie I2, image de dimension l x h
//   I2(x,y) = min(I(x,y),v) avec 0<=x<l et 0<=y<h
ImageGrisF min(ImageGrisF& I, float v);

// calcule max(I,v)
// Entrée : I = image de dimension l x h
//          v = réel de type float
// La fonction renvoie I2, image de dimension l x h
//   I2(x,y) = max(I(x,y),v) avec 0<=x<l et 0<=y<h
ImageGrisF max(ImageGrisF& I, float v);

// calcule l'arrondi de I par la valeur v
// Entrée : I = image de dimension l x h
//          v = réel de type float strictement positif
// La fonction renvoie I2, image de dimension l x h
//   I2(x,y) = round(I(x,y)/v)*v avec 0<=x<l et 0<=y<h
// Si v <= 0 aucun arrondi n'est fait, et I2 = I
ImageGrisF arrondi(ImageGrisF& I, float v);

// calcule l'image TI transposee de l'image I
// Entrée : I = image de dimension l x h
// La fonction renvoie TI, image de dimension h x l
//   TI(x,y) = I(y,x) avec 0<=x<h et 0<=y<l
ImageGrisF transpose(ImageGrisF& I);

// calcule l'image FI = f(I)
// Entrée : I = image de dimension l x h
//          f = fonction définie sur la forme float f(float)
// La fonction renvoie FI, image de dimension l x h
//   FI(x,y) = f(I(x,y)) avec 0<=x<l et 0<=y<h
ImageGrisF f(ImageGrisF& I, float (*f)(float));

// calcule l'image Ia = I*a
// Entrée : I = image de dimension l x h
//          a = réel de type float
// La fonction renvoie Ia, image de dimension l x h
//   FI(x,y) = I(x,y)*a avec 0<=x<l et 0<=y<h
ImageGrisF mul(ImageGrisF& I, float a);

// calcule l'image Ia = I/a
// Entrée : I = image de dimension l x h
//          a = réel de type float
// La fonction renvoie Ia, image de dimension l x h
//   FI(x,y) = I(x,y)/a avec 0<=x<l et 0<=y<h
ImageGrisF div(ImageGrisF& I, float a);

// calculer la matrice correspondant à la DCT de taille n
ImageGrisF matrice_dct(UINT n);

// calculer la matrice correspondant à la IDCT de taille n
ImageGrisF matrice_idct(UINT n);

// calcule la DCT de l'image I
ImageGrisF DCT(ImageGrisF &I);

// calcule la DCT inverse de l'image I
ImageGrisF IDCT(ImageGrisF &I);

// convertir une image couleur du mode RGB vers le mode YCbCr
// Entree : I_RGB = image en mode RGBF
// Sortie : I_Y, I_Cb, I_Cr = composantes Y, Cb et CR en mode GrisF
// Les valeurs des différentes images sont entre 0.0 et 1.0 
void conversion_RGB_YCbCr(ImageCouleurF &I_RGB, 
 ImageGrisF& I_Y, ImageGrisF& I_Cb, ImageGrisF& I_Cr);

// convertir une image couleur du mode YCbCr vers le mode RGB
// Entree : I_Y, I_Cb, I_Cr = composantes Y, Cb et CR en mode GrisF
// Sortie : I_RGB = image en mode RGBF
// Les valeurs des différentes images sont entre 0.0 et 1.0 
void conversion_YCbCr_RGB
 (ImageGrisF& I_Y, ImageGrisF& I_Cb, ImageGrisF& I_Cr, ImageCouleurF &I_RGB);

#endif
