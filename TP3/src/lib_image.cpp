///////////////////////////////////////////////////////////////////////////
// librairie � partir de Cimg - partie impl�mentation
///////////////////////////////////////////////////////////////////////////

#include "CImg.h"
#include "lib_image.hpp"
#include <cstdio>
#include <list>
	
using namespace cimg_library;
using namespace std;

///////////////////////////////////////////////////////////////////////////
// conversion d'une composante de type float vers une composante de type UINT8
UINT8 float_to_UINT8(float v)
{
	if (v<0.0) return 0;
	if (v>1.0) return 255;
	return (UINT8)round(v*255.0);
}

///////////////////////////////////////////////////////////////////////////
// conversion d'une composante de type UINT8 vers une composante de type float
float UINT8_to_float(UINT8 v)
{
	return (float)v / 255.0;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// partie op�rations de base sur les objets CImg<UINT8>
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// remplir l'image I avec la valeur gris v
void remplir_gris(CImg<UINT8> &I, UINT8 v)
{
	I.fill(v);
} 

// remplir l'image I avec la valeur couleur v
void remplir_couleur(CImg<UINT8> &I, RGB v)
{
	if (I.spectrum()==1)
	{
		// convertir v en niveau de gris
		RGBF vF(v);
		float gF = 0.2125 * vF.r + 0.7154 * vF.g + 0.0721 * vF.b; 
		UINT g = (UINT8)round(gF*255.0);
		I.fill(g);
	}
	else
	{
		UINT L = I.width();
		UINT H = I.height();
		UINT N = L*H;
		for (UINT i=0; i<N; i++)
		{
			I(i) = v.r;
			I(i+N) = v.g;
			I(i+2*N) = v.b;
		}
	}
} 

// remplir le rectangle (x0,y0)-(x1-y1) de l'image I avec la valeur couleur v
void remplir_rectangle_couleur
 (CImg<UINT8> &I, UINT x0, UINT y0, UINT x1, UINT y1, RGB v)
{
	// permuter �ventuellement les bornes du rectangle
	if (x0>x1) { UINT r=x0; x0=x1; x1=r;}
	if (y0>y1) { UINT r=y0; y0=y1; y1=r;}
	
	if (I.spectrum()==1)
	{
		// convertir v en niveau de gris
		RGBF vF(v);
		float gF = 0.2125 * vF.r + 0.7154 * vF.g + 0.0721 * vF.b; 
		UINT g = (UINT8)round(gF*255.0);
		for (UINT x=x0; x<x1; x++)
		for (UINT y=y0; y<y1; y++)
			I(x,y) = gF;
	}
	else
	{
		UINT L = I.width();
		UINT H = I.height();
		UINT N = L*H;
		for (UINT x=x0; x<x1; x++)
		for (UINT y=y0; y<y1; y++)
		{
			I(x,y,0,0) = v.r;
			I(x,y,0,1) = v.g;
			I(x,y,0,2) = v.b;
		}
	}
} 

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// partie affichage � l'�cran des images, histogrammes, fonctions
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// renvoie la largeur de l'�cran
UINT L_ECRAN()
{
	return CImgDisplay::screen_width();
}

// renvoie la hauteur de l'�cran
UINT H_ECRAN()
{
	return CImgDisplay::screen_height();
}

// pour une image de dimensions L x H et un facteur d'�chelle z
// la fonction renvoie le facteur d'�chelle effectif ze
// le zoom effectif doit etre au moins �gal � 1
// et s'il est sp�rieur ou �gal � 2, la taille de l'image zoomee ne doit pas
// d�passer la taille de l'ecran
UINT zoom_effectif(UINT L, UINT H, UINT z)
{
	UINT ze;
	if (z==0 || z==1) 
	{
		ze = 1;
	}
	else if (z*L<=L_ECRAN() && z*H<=H_ECRAN()) 
	{
		ze = z;
	}
	else if (L>L_ECRAN() || H>H_ECRAN())
	{
		ze = 1;
	}
	else
	{
		UINT zL = L_ECRAN()/L; 
		UINT zH = H_ECRAN()/H;
		ze = zL<zH ? zL : zH;
	}
// printf("zoom_effectif : L=%ld, H=%ld, z=%ld --> ze=%ld\n", L, H, z, ze);
	return ze;
}

// type fenetre
struct fenetre
{
	CImgDisplay *F;
	int num;
};

// la liste des fenetres
list<fenetre> liste_f;

// num�ro de fenetre
int num_fen = 0;

// cr�er une CImg<UINT8> � partir de l'ImageGris I avec le facteur de zoom z
CImg<UINT8> conversion_avec_zoom(ImageGris &I, UINT z=1)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	
	// calculer le zoom effectif  
	z = zoom_effectif(L,H,z);
	
	// l'image zoom�e
	CImg<UINT8> I_zoomee(L*z,H*z,1,1);
	
	for (int x=0; x<L; x++)
	for (int y=0; y<H; y++)
	{
		UINT8 v = I(x,y);
		for (int i=0; i<z; i++)
		for (int j=0; j<z; j++)
			I_zoomee(x*z+i,y*z+j) = v;
	}
	
	return I_zoomee;
}

// cr�er une CImg<UINT8> � partir de l'ImageGrisF I avec le facteur de zoom z
CImg<UINT8> conversion_avec_zoom(ImageGrisF &I, UINT z=1)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	
	// calculer le zoom effectif  
	z = zoom_effectif(L,H,z);
	
	// l'image zoom�e
	CImg<UINT8> I_zoomee(L*z,H*z,1,1);
	
	for (int x=0; x<L; x++)
	for (int y=0; y<H; y++)
	{
		UINT8 v = float_to_UINT8(I(x,y));
		for (int i=0; i<z; i++)
		for (int j=0; j<z; j++)
			I_zoomee(x*z+i,y*z+j) = v;
	}
	
	return I_zoomee;
}

// cr�er une CImg<UINT8> � partir de l'ImageCouleur I avec le facteur de zoom z
CImg<UINT8> conversion_avec_zoom(ImageCouleur &I, UINT z=1)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	
	// calculer le zoom effectif  
	z = zoom_effectif(L,H,z);
	
	// l'image zoom�e
	CImg<UINT8> I_zoomee(L*z,H*z,1,3);
	
	for (int x=0; x<L; x++)
	for (int y=0; y<H; y++)
	{
		RGB p = I(x,y);
		for (int i=0; i<z; i++)
		for (int j=0; j<z; j++)
		{
			I_zoomee(x*z+i,y*z+j,0,0) = p.r;
			I_zoomee(x*z+i,y*z+j,0,1) = p.g;
			I_zoomee(x*z+i,y*z+j,0,2) = p.b;
		}
	}
	
	return I_zoomee;
}

// cr�er une CImg<UINT8> � partir de l'ImageCouleurF I avec le facteur de zoom z
CImg<UINT8> conversion_avec_zoom(ImageCouleurF &I, UINT z=1)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	
	// calculer le zoom effectif  
	z = zoom_effectif(L,H,z);
	
	// l'image zoom�e
	CImg<UINT8> I_zoomee(L*z,H*z,1,3);
	
	for (int x=0; x<L; x++)
	for (int y=0; y<H; y++)
	{
		RGB p = RGB(I(x,y));
		for (int i=0; i<z; i++)
		for (int j=0; j<z; j++)
		{
			I_zoomee(x*z+i,y*z+j,0,0) = p.r;
			I_zoomee(x*z+i,y*z+j,0,1) = p.g;
			I_zoomee(x*z+i,y*z+j,0,2) = p.b;
		}
	}
	
	return I_zoomee;
}

// cr�er une CImg<UINT8> � partir de l'ImageIndexee I avec le facteur de zoom z
CImg<UINT8> conversion_avec_zoom(ImageIndexee &I, UINT z=1)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	
	// calculer le zoom effectif  
	z = zoom_effectif(L,H,z);
	
	// l'image zoom�e
	CImg<UINT8> I_zoomee(L*z,H*z,1,3);
	
	for (int x=0; x<L; x++)
	for (int y=0; y<H; y++)
	{
		RGB p = I.couleur(I.pixel(x+L*y));
		for (int i=0; i<z; i++)
		for (int j=0; j<z; j++)
		{
			I_zoomee(x*z+i,y*z+j,0,0) = p.r;
			I_zoomee(x*z+i,y*z+j,0,1) = p.g;
			I_zoomee(x*z+i,y*z+j,0,2) = p.b;
		}
	}
	
	return I_zoomee;
}

// creer une fenetre graphique pour afficher l'image I 
// avec un zoom entier z (si z=0 le zoom est mis � 1)
// renvoie le num�ro (entier positif) de la fenetre cr��e
template <typename Image>
int creer_fenetre_graphique(Image &I, UINT z=1)
{
	// l'image zoom�e
	CImg<UINT8> I_zoomee = conversion_avec_zoom(I,z);
	
	// creer la fenetre pour l'affichage de I_zoomee
	num_fen++;
	char titre[100];
	sprintf(titre, "Fen. %d", num_fen);
	CImgDisplay *CID = new CImgDisplay(I_zoomee, titre, 0);
	
	// ajouter la fenetre � la liste
	fenetre f = {CID, num_fen};
	liste_f.push_back(f);	
	
	return num_fen;
}

// changer le titre de la fenetre de numero nf avec la chaine de caracteres t
void changer_titre_fenetre_graphique(int nf, const char *t)
{
	bool trouve = false;
	list<fenetre>::iterator i;
	
	// parcourir la liste
	i = liste_f.begin();
	while (i != liste_f.end() && !trouve)
	{
		fenetre f = *i;
		if (f.num == nf)
		{
			trouve = true;
		}
		else
		{
			i++;
		}
	}
	
	if (trouve)
	{
		// changer le titre
		fenetre f = *i;
		f.F->set_title("%s", t);
	}
	
}

// supprimer la fenetre graphique de numero nf
void supprimer_fenetre_graphique(int nf)
{
	bool trouve = false;
	list<fenetre>::iterator i;
	
	// parcourir la liste
	i = liste_f.begin();
	while (i != liste_f.end() && !trouve)
	{
		fenetre f = *i;
		if (f.num == nf)
		{
			trouve = true;
		}
		else
		{
			i++;
		}
	}
	
	if (trouve)
	{
		// supprimer la fenetre
		fenetre f = *i;
		liste_f.erase(i);
		delete f.F;
	}
}

// charger une ImageCouleur � partir du fichier nomm� nom_f
ImageCouleur charger(const char *nom_f)
{
	
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe ImageGris : image en 256 niveaux de gris
//
// utilisation d'une Cimg<UINT8> avec size_z = size_c = 1
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// cr�ation d'une image vide
ImageGris::ImageGris()
{
	pixels = NULL;
	L = H = 0;
}
	
// cr�ation d'une image de largeur L0 et de hauteur H0
// et avec les tous les pixels initialis�s � v (par d�faut v=0)
ImageGris::ImageGris(UINT L0, UINT H0, UINT8 v)
{
	pixels = new UINT8[L0*H0];
	L = L0;
	H = H0;
	
	// initialiser les pixels avec la valeur v
	for (UINT k=0; k<L*H; k++)
		pixels[k] = v;
}
	
// cr�ation d'une image de largeur L0 et de hauteur H0
// � partir d'un tableau de pixels p
// le pixel(x,y) de l'image est p[x+y*k]
ImageGris::ImageGris(UINT L0, UINT H0, UINT8* p)
{
	pixels = new UINT8[L0*H0];
	L = L0;
	H = H0;
	
	// copier les pixels de p
	for (UINT k=0; k<L*H; k++)
		pixels[k] = p[k];
}

// cr�ation � partir d'un fichier image nomm� nom_f
ImageGris::ImageGris(const char *nom_f)
{
	CImg<UINT8> img1 = CImg<UINT8>(nom_f);
	L = img1.width();
	H = img1.height();
	UINT8 *pixels1 = img1.data();
	
	// cr�ation de l'image
	pixels = new UINT8[L*H];
	
	if (img1.spectrum()==1)
	{
		// img1 est une image en niveaux de gris
		// --> la copier telle quelle
		for (UINT x=0; x<L; x++)
		for (UINT y=0; y<H; y++)
			pixels[x+L*y] = pixels1[x+L*y];
	}
	else
	{
		// img1 est une image couleurs
		// --> la copier en passant chaque pixel en niveaux de gris
		for (UINT x=0; x<L; x++)
		for (UINT y=0; y<H; y++)
		{
			float r = UINT8_to_float(pixels1[x+L*y]);
			float g = UINT8_to_float(pixels1[x+L*y+L*H]);
			float b = UINT8_to_float(pixels1[x+L*y+2*L*H]);
			pixels[x+L*y] = float_to_UINT8(0.213*r+0.715*g+0.072*b);
		}
	}
	
}
	
// constructeur par copie de I
ImageGris::ImageGris(ImageGris &I)
{
	L = I.L;
	H = I.H;
	pixels = new UINT8[L*H];
	
	// copier les pixels de I
	for (UINT k=0; k<L*H; k++)
	{
		pixels[k] = I.pixels[k];
	}	
}

ImageGris::ImageGris(ImageGrisF &I)
{
	L = I.largeur();
	H = I.hauteur();
	pixels = new UINT8[L*H];
	
	// copier les pixels de I
	for (UINT k=0; k<L*H; k++)
	{
		pixels[k] = float_to_UINT8(I.pixel(k));
	}	
}

///////////////////////////////////////////////////////////////////////////
// surcharges
///////////////////////////////////////////////////////////////////////////

// surcharge de l'op�rateur =
ImageGris& ImageGris::operator=(const ImageGris& I)
{
	if (this != &I)
	{
		if (pixels) 
			delete[] pixels;
		L = I.L;
		H = I.H;
		pixels = new UINT8[L*H];

		// copier les pixels de I
		for (UINT k=0; k<L*H; k++)
		{
			pixels[k] = I.pixels[k];
		}	
	}
	return *this;
}

///////////////////////////////////////////////////////////////////////////
// acc�s � l'image
///////////////////////////////////////////////////////////////////////////

// renvoie la largeur
UINT ImageGris::largeur()
{
	return L;
}

// renvoie la largeur
UINT ImageGris::hauteur()
{
	return H;
}
	
// renvoie le nombre de pixels
UINT ImageGris::nb_pixels()
{
	return L*H;
}
	
// renvoie la r�f�rence du pixel(i) = pixel(x+y*largeur)
UINT8& ImageGris::pixel(UINT i)
{
	return pixels[i];
}
	
UINT8& ImageGris::operator()(UINT i)
{
	return pixels[i];
}
	
// renvoie la r�f�rence du pixel(x,y)
UINT8& ImageGris::pixel(UINT x, UINT y)
{
	return pixels[x+L*y];
}

// renvoie la r�f�rence du pixel (x,y)
UINT8& ImageGris::operator()(UINT x, UINT y)
{
	return pixels[x+L*y];
}

// remplace le pixel (x,y) avec la valeur v
// ne fait rien si (x,y) est hors de l'image
void ImageGris::pixel(UINT x, UINT y, UINT8 v)
{
	if (x<L && y<H)
	{
		pixels[x+L*y] = v;
	}
}
	
///////////////////////////////////////////////////////////////////////////
// afficher l'image dans une fenetre graphique avec le zoom z 
// (par defaut z=1)
// renvoie le num�ro de la fenetre cr��e
int ImageGris::afficher(UINT z)
{
	return creer_fenetre_graphique(*this, z);
}

///////////////////////////////////////////////////////////////////////////
// afficher l'image dans la console 
// avec le format f pour l'affichage des valeurs
// par d�faut f = "%3d"
///////////////////////////////////////////////////////////////////////////
void ImageGris::print()
{
	for (UINT y=0; y<H; y++)
	{
		for (UINT x=0; x<L; x++)
		{
			printf(" ");
			printf("%3d", pixels[x+L*y]);
		}
		printf("\n");
	}
}

void ImageGris::print(const char *f)
{
	for (UINT y=0; y<H; y++)
	{
		for (UINT x=0; x<L; x++)
		{
			printf(" ");
			printf(f, pixels[x+L*y]);
		}
		printf("\n");
	}
}

///////////////////////////////////////////////////////////////////////////
// sauvegarde de l'image dans un fichier image
///////////////////////////////////////////////////////////////////////////

// sauvegarde dans un fichier suivant l'extension du nom de fichier nom_f
void ImageGris::sauvegarder(const char* nom_f)
{
	CImg<UINT8> I = conversion_avec_zoom(*this);
	I.save(nom_f);
}

// sauvegarde dans un fichier nomm� nom_f au format JPG 
// avec la qualit� q (par d�faut q=100)
// si q>100 alors q=100 est utilis�
void ImageGris::sauvegarder_jpeg(const char* nom_f, UINT8 q) 
{
	CImg<UINT8> I = conversion_avec_zoom(*this);
	if (q==0) q=1;
	if (q>100) q=100;
	I.save_jpeg(nom_f, q);
} 
	
///////////////////////////////////////////////////////////////////////////
// destructeur
///////////////////////////////////////////////////////////////////////////
ImageGris::~ImageGris()
{
	if (pixels)
		delete[] pixels;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe ImageGrisF : image en 256 niveaux de gris
//
// utilisation d'une Cimg<float> avec size_z = size_c = 1
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// cr�ation d'une image vide
ImageGrisF::ImageGrisF()
{
	pixels = NULL;
	L = H = 0;
}
	
// cr�ation d'une image de largeur L0 et de hauteur H0
// et avec les tous les pixels initialis�s � v (par d�faut v=0)
ImageGrisF::ImageGrisF(UINT L0, UINT H0, float v)
{
	L = L0;
	H = H0;
	pixels = new float[L*H];

	// initialiser les pixels avec la valeur v
	for (UINT k=0; k<L*H; k++)
		pixels[k] = v;
}
	
// cr�ation d'une image de largeur L0 et de hauteur H0
// � partir d'un tableau de pixels p
// le pixel(x,y) de l'image est p[x+y*k]
ImageGrisF::ImageGrisF(UINT L0, UINT H0, float* p)
{
	L = L0;
	H = H0;
	pixels = new float[L*H];
	
	// copier les pixels de p
	for (UINT k=0; k<L*H; k++)
		pixels[k] = p[k];
}

// cr�ation � partir d'un fichier image nomm� nom_f
ImageGrisF::ImageGrisF(const char *nom_f)
{
	CImg<UINT8> img1 = CImg<UINT8>(nom_f);
	L = img1.width();
	H = img1.height();
	UINT8 *pixels1 = img1.data();
	
	// cr�ation de l'image
	pixels = new float[L*H];
	
	if (img1.spectrum()==1)
	{
		// img1 est une image en niveaux de gris
		// --> la copier telle quelle
		for (UINT x=0; x<L; x++)
		for (UINT y=0; y<H; y++)
			pixels[x+L*y] = UINT8_to_float(pixels1[x+L*y]);
	}
	else
	{
		// img1 est une image couleurs
		// --> la copier en passant chaque pixel en niveaux de gris
		for (UINT x=0; x<L; x++)
		for (UINT y=0; y<H; y++)
		{
			float r = UINT8_to_float(pixels1[x+L*y]);
			float g = UINT8_to_float(pixels1[x+L*y+L*H]);
			float b = UINT8_to_float(pixels1[x+L*y+2*L*H]);
			pixels[x+L*y] = 0.213*r+0.715*g+0.072*b;
		}
	}
}
	
// constructeur par copie de I
ImageGrisF::ImageGrisF(ImageGrisF &I)
{
	L = I.L;
	H = I.H;
	pixels = new float[L*H];
	
	// copier les pixels de I
	for (UINT k=0; k<L*H; k++)
	{
		pixels[k] = I.pixel(k);
	}	
}

ImageGrisF::ImageGrisF(ImageGris &I)
{
	L = I.largeur();
	H = I.hauteur();
	pixels = new float[L*H];
	
	// copier les pixels de I
	for (UINT k=0; k<L*H; k++)
	{
		pixels[k] = UINT8_to_float(I.pixel(k));
	}	
}

///////////////////////////////////////////////////////////////////////////
// surcharges
///////////////////////////////////////////////////////////////////////////

// surcharge de l'op�rateur =
ImageGrisF& ImageGrisF::operator=(const ImageGrisF& I)
{
	if (this != &I)
	{
		if (pixels) 
			delete[] pixels;
		L = I.L;
		H = I.H;
		pixels = new float[L*H];

		// copier les pixels de I
		for (UINT k=0; k<L*H; k++)
		{
			pixels[k] = I.pixels[k];
		}	
	}
	return *this;
}

///////////////////////////////////////////////////////////////////////////
// acc�s � l'image
///////////////////////////////////////////////////////////////////////////

// renvoie la largeur
UINT ImageGrisF::largeur()
{
	return L;
}

// renvoie la largeur
UINT ImageGrisF::hauteur()
{
	return H;
}
	
// renvoie le nombre de pixels
UINT ImageGrisF::nb_pixels()
{
	return L*H;
}
	
// renvoie la r�f�rence du pixel(i) = pixel(x+y*largeur)
float& ImageGrisF::pixel(UINT i)
{
	return pixels[i];
}
	
float& ImageGrisF::operator()(UINT i)
{
	return pixels[i];
}
	
// renvoie la r�f�rence du pixel (x,y)
float& ImageGrisF::pixel(UINT x, UINT y)
{
	return pixels[x+L*y];
}

// renvoie la r�f�rence du pixel (x,y)
float& ImageGrisF::operator()(UINT x, UINT y)
{
	return pixels[x+L*y];
}

// remplace le pixel (x,y) avec la valeur v
// ne fait rien si (x,y) est hors de l'image
void ImageGrisF::pixel(UINT x, UINT y, float v)
{
	if (x<L && y<H)
	{
		pixels[x+L*y] = v;
	}
}
	
///////////////////////////////////////////////////////////////////////////
// afficher l'image dans une fenetre graphique avec le zoom z 
// (par defaut z=1)
// renvoie le num�ro de la fenetre cr��e
int ImageGrisF::afficher(UINT z)
{
	return creer_fenetre_graphique(*this, z);
}

///////////////////////////////////////////////////////////////////////////
// afficher l'image dans la console 
// avec le format f pour l'affichage des valeurs
// par d�faut f = "%6.3f"
///////////////////////////////////////////////////////////////////////////
void ImageGrisF::print()
{
	for (UINT y=0; y<H; y++)
	{
		for (UINT x=0; x<L; x++)
		{
			printf(" ");
			printf("%6.3f", pixels[x+L*y]);
		}
		printf("\n");
	}
}

void ImageGrisF::print(const char *f)
{
	for (UINT y=0; y<H; y++)
	{
		for (UINT x=0; x<L; x++)
		{
			printf(" ");
			printf(f, pixels[x+L*y]);
		}
		printf("\n");
	}
}

///////////////////////////////////////////////////////////////////////////
// sauvegarde de l'image dans un fichier image
///////////////////////////////////////////////////////////////////////////

// sauvegarde dans un fichier suivant l'extension du nom de fichier nom_f
void ImageGrisF::sauvegarder(const char* nom_f)
{
	CImg<UINT8> I = conversion_avec_zoom(*this);
	I.save(nom_f);
}

// sauvegarde dans un fichier nomm� nom_f au format JPG 
// avec la qualit� q (par d�faut q=100)
// si q>100 alors q=100 est utilis�
void ImageGrisF::sauvegarder_jpeg(const char* nom_f, UINT8 q) 
{
	CImg<UINT8> I = conversion_avec_zoom(*this);
	if (q==0) q=1;
	if (q>100) q=100;
	I.save_jpeg(nom_f, q);
} 
	
///////////////////////////////////////////////////////////////////////////
// destructeur
///////////////////////////////////////////////////////////////////////////
ImageGrisF::~ImageGrisF()
{
	if (pixels)
		delete[] pixels;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe RGB : 3 entiers de type UINT8
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
	
// constructeur � partir de 3 valeurs (r0,g0,b0)
RGB::RGB(UINT8 r0, UINT8 g0, UINT8 b0)
{
	r = r0; g = g0; b = b0;
}

// constructeur � partir d'une valeur RGBF v
RGB::RGB(RGBF v)
{
	r = float_to_UINT8(v.r);
	g = float_to_UINT8(v.g);
	b = float_to_UINT8(v.b);
}

// conversion en niveau de gris
UINT8 RGB::gris()
{
	return float_to_UINT8(
	  0.213*UINT8_to_float(r)+
	  0.715*UINT8_to_float(g)+
	  0.072*UINT8_to_float(b)
	);
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe ImageCouleur : image couleur en mode RGB 24 bits
//   valeurs des pixels : 3 entiers UINT8 entre 0 et 255
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// cr�ation d'une image vide
ImageCouleur::ImageCouleur()
{
	pixels = NULL;
	L = H = 0;
}
	
// cr�ation d'une image de largeur L0 et de hauteur H0
// et avec les tous les pixels initialis�s � v (par d�faut v=RGB(0,0,0))
ImageCouleur::ImageCouleur(UINT L0, UINT H0, RGB v)
{
	L = L0;
	H = H0;
	pixels = new RGB[L*H];

	// initialiser les pixels avec la valeur v
	for (UINT i=0; i<L*H; i++)
		pixels[i] = v;
}
	
// cr�ation d'une image de largeur L0 et de hauteur H0
// � partir d'un tableau p de valeurs RGB
ImageCouleur::ImageCouleur(UINT L0, UINT H0, RGB* p)
{
	L = L0;
	H = H0;
	pixels = new RGB[L*H];
	
	// copier les pixels de p
	for (UINT i=0; i<L*H; i++)
		pixels[i] = p[i];
}

// cr�ation � partir d'un fichier image nomm� nom_f
ImageCouleur::ImageCouleur(const char *nom_f)
{
	CImg<UINT8> img1 = CImg<UINT8>(nom_f);
	L = img1.width();
	H = img1.height();
	UINT8 *pixels1 = img1.data();
	
	// cr�ation de l'image
	pixels = new RGB[L*H];
	
	if (img1.spectrum()==3)
	{
		// img1 est une image couleur
		// --> la copier telle quelle
		for (UINT i=0; i<L*H; i++)
		{
			pixels[i].r = pixels1[i];
			pixels[i].g = pixels1[i+L*H];
			pixels[i].b = pixels1[i+2*L*H];
		}
	}
	else
	{
		// img1 est une image noir et blanc
		for (UINT i=0; i<L*H; i++)
		{
			UINT8 v = pixels1[i];
			pixels[i].r = v;
			pixels[i].g = v;
			pixels[i].b = v;
		}
	}
	
}
	
// constructeur par copie de I
ImageCouleur::ImageCouleur(ImageCouleur &I)
{
	L = I.largeur();
	H = I.hauteur();
	pixels = new RGB[L*H];
	
	// copie des pixels de I
	for (UINT i=0; i<L*H; i++)
		pixels[i] = I.pixels[i];
}

ImageCouleur::ImageCouleur(ImageCouleurF &I)
{
	L = I.largeur();
	H = I.hauteur();
	pixels = new RGB[L*H];
	
	// copier les pixels de I
	for (UINT i=0; i<L*H; i++)
		pixels[i] = RGB(I.pixel(i));
}

///////////////////////////////////////////////////////////////////////////
// surcharges
///////////////////////////////////////////////////////////////////////////

// surcharge de l'op�rateur =
ImageCouleur& ImageCouleur::operator=(const ImageCouleur& I)
{
	if (this != &I)
	{
		if (pixels) 
			delete[] pixels;
			
		L = I.L;
		H = I.H;
		pixels = new RGB[L*H];
	
		// copier les pixels de I
		for (UINT i=0; i<L*H; i++)
			pixels[i] = I.pixels[i];
	}
	return *this;
}

///////////////////////////////////////////////////////////////////////////
// acc�s � l'image
///////////////////////////////////////////////////////////////////////////

// renvoie la largeur
UINT ImageCouleur::largeur()
{
	return L;
}

// renvoie la largeur
UINT ImageCouleur::hauteur()
{
	return H;
}
	
// renvoie le nombre de pixels
UINT ImageCouleur::nb_pixels()
{
	return L*H;
}
	
// renvoie la r�f�rence du pixel(i) = pixel(x+y*largeur)
RGB& ImageCouleur::pixel(UINT i)
{
	return pixels[i];
}

RGB& ImageCouleur::operator()(UINT i)
{
	return pixels[i];
}

// renvoie la r�f�rence RGB du pixel (x,y)
RGB& ImageCouleur::pixel(UINT x, UINT y)
{
	return pixels[x+L*y];
}

RGB& ImageCouleur::operator()(UINT x, UINT y)
{
	return pixels[x+L*y];
}
	
// renvoie la r�f�rence de la k-eme composante du pixel (x,y)
UINT8& ImageCouleur::comp(UINT x, UINT y, UINT k)
{
	switch (k%3)
	{
		case 0 : return pixels[x+L*y].r;
		case 1 : return pixels[x+L*y].g;
		case 2 : return pixels[x+L*y].b;
	}
}
	
// renvoie la r�f�rence de la k-eme composante du pixel i
UINT8& ImageCouleur::comp(UINT i, UINT k)
{
	switch (k%3)
	{
		case 0 : return pixels[i].r;
		case 1 : return pixels[i].g;
		case 2 : return pixels[i].b;
	}
}
	
// remplace le pixel (x,y) avec la valeur RGB v
// ne fait rien si (x,y) est hors de l'image
void ImageCouleur::pixel(UINT x, UINT y, RGB v)
{
	if (x<L && y<H)
	{
		pixels[x+L*y] = v;
	}
}
	
///////////////////////////////////////////////////////////////////////////
// afficher l'image dans une fenetre graphique avec le zoom z 
// (par defaut z=1)
// renvoie le num�ro de la fenetre cr��e
int ImageCouleur::afficher(UINT z)
{
	return creer_fenetre_graphique(*this, z);
}

///////////////////////////////////////////////////////////////////////////
// afficher l'image dans la console 
// avec le format f pour l'affichage des valeurs
// par d�faut f = "%3d"
///////////////////////////////////////////////////////////////////////////
void ImageCouleur::print()
{
	for (UINT y=0; y<H; y++)
	{
		for (UINT x=0; x<L; x++)
		{
			RGB v = pixel(x,y);
			printf(" (");
			printf("%3d", v.r);
			printf(",");
			printf("%3d", v.g);
			printf(",");
			printf("%3d", v.b);
			printf(")");
		}
		printf("\n");
	}
}

void ImageCouleur::print(const char *f)
{
	for (UINT y=0; y<H; y++)
	{
		for (UINT x=0; x<L; x++)
		{
			RGB v = pixel(x,y);
			printf(" (");
			printf(f, v.r);
			printf(",");
			printf(f, v.g);
			printf(",");
			printf(f, v.b);
			printf(")");
		}
		printf("\n");
	}
}

///////////////////////////////////////////////////////////////////////////
// sauvegarde de l'image dans un fichier image
///////////////////////////////////////////////////////////////////////////

// sauvegarde dans un fichier suivant l'extension du nom de fichier nom_f
void ImageCouleur::sauvegarder(const char* nom_f)
{
	CImg<UINT8> I = conversion_avec_zoom(*this);
	I.save(nom_f);
}

// sauvegarde dans un fichier nomm� nom_f au format JPG 
// avec la qualit� q (par d�faut q=100)
// si q>100 alors q=100 est utilis�
void ImageCouleur::sauvegarder_jpeg(const char* nom_f, UINT8 q) 
{
	CImg<UINT8> I = conversion_avec_zoom(*this);
	if (q==0) q=1;
	if (q>100) q=100;
	I.save_jpeg(nom_f, q);
} 
	
///////////////////////////////////////////////////////////////////////////
// destructeur
///////////////////////////////////////////////////////////////////////////
ImageCouleur::~ImageCouleur()
{
	if (pixels)
		delete[] pixels;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe RGBF : 3 entiers de type float
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// constructeur � partir de 3 valeurs (r0,g0,b0)
RGBF::RGBF(float r0, float g0, float b0)
{
	r = r0; g = g0; b = b0;
}

// constructeur � partir d'une valeur RGB v
RGBF::RGBF(RGB v)
{
	r = UINT8_to_float(v.r);
	g = UINT8_to_float(v.g);
	b = UINT8_to_float(v.b);
}

// conversion en niveau de gris
float RGBF::gris()
{
	return 0.213*r+0.715*g+0.072*b;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe ImageCouleurF : image couleur en mode RGBF 24 bits
//   valeurs des pixels : 3 entiers float entre 0 et 255
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// cr�ation d'une image vide
ImageCouleurF::ImageCouleurF()
{
	pixels = NULL;
	L = H = 0;
}
	
// cr�ation d'une image de largeur L0 et de hauteur H0
// et avec les tous les pixels initialis�s � v (par d�faut v=RGBF(0,0,0))
ImageCouleurF::ImageCouleurF(UINT L0, UINT H0, RGBF v)
{
	L = L0;
	H = H0;
	pixels = new RGBF[L*H];
	
	// initialiser les valeurs des pixels avec la valeur v
	for (UINT i=0; i<L*H; i++)
		pixels[i] = v;
}
	
// cr�ation d'une image de largeur L0 et de hauteur H0
// � partir d'un tableau p de valeurs RGBF
ImageCouleurF::ImageCouleurF(UINT L0, UINT H0, RGBF* p)
{
	L = L0;
	H = H0;

	// copier les pixels de p
	pixels = new RGBF[L*H];
	for (UINT i=0; i<L*H; i++)
		pixels[i] = p[i];
}

// cr�ation � partir d'un fichier image nomm� nom_f
ImageCouleurF::ImageCouleurF(const char *nom_f)
{
	CImg<UINT8> img1 = CImg<UINT8>(nom_f);
	L = img1.width();
	H = img1.height();
	UINT8 *pixels1 = img1.data();
	
	// cr�ation de l'image
	pixels = new RGBF[L*H];
	
	if (img1.spectrum()==3)
	{
		// img1 est une image couleur
		// --> la copier telle quelle
		for (UINT i=0; i<L*H; i++)
		{
			pixels[i].r = UINT8_to_float(pixels1[i]);
			pixels[i].g = UINT8_to_float(pixels1[i+L*H]);
			pixels[i].b = UINT8_to_float(pixels1[i+2*L*H]);
		}
	}
	else
	{
		// img1 est une image noir et blanc
		for (UINT i=0; i<L*H; i++)
		{
			float vf = UINT8_to_float(pixels1[i]);
			pixels[i].r = vf;
			pixels[i].g = vf;
			pixels[i].b = vf;
		}
	}
	
}
	
// constructeur par copie de I
ImageCouleurF::ImageCouleurF(ImageCouleurF &I)
{
	L = I.largeur();
	H = I.hauteur();
	pixels = new RGBF[L*H];
	
	// copie des pixels de I
	for (UINT i=0; i<L*H; i++)
		pixels[i] = I.pixels[i];
}

ImageCouleurF::ImageCouleurF(ImageCouleur &I)
{
	L = I.largeur();
	H = I.hauteur();
	pixels = new RGBF[L*H];
	
	// copier les pixels de I
	for (UINT i=0; i<L*H; i++)
		pixels[i] = RGBF(I.pixel(i));
}

///////////////////////////////////////////////////////////////////////////
// surcharges
///////////////////////////////////////////////////////////////////////////

// surcharge de l'op�rateur =
ImageCouleurF& ImageCouleurF::operator=(const ImageCouleurF& I)
{
	if (this != &I)
	{
		if (pixels) 
			delete[] pixels;
			
		L = I.L;
		H = I.H;
		pixels = new RGBF[L*H];
	
		// copier les pixels de I
		for (UINT i=0; i<L*H; i++)
			pixels[i] = I.pixels[i];
	}
	return *this;
}

///////////////////////////////////////////////////////////////////////////
// acc�s � l'image
///////////////////////////////////////////////////////////////////////////

// renvoie la largeur
UINT ImageCouleurF::largeur()
{
	return L;
}

// renvoie la largeur
UINT ImageCouleurF::hauteur()
{
	return H;
}
	
// renvoie le nombre de pixels
UINT ImageCouleurF::nb_pixels()
{
	return L*H;
}
	
// renvoie la r�f�rence du pixel(i) = pixel(x+y*largeur)
RGBF& ImageCouleurF::pixel(UINT i)
{
	return pixels[i];
}

RGBF& ImageCouleurF::operator()(UINT i)
{
	return pixels[i];
}

// renvoie la r�f�rence RGBF du pixel (x,y)
RGBF& ImageCouleurF::pixel(UINT x, UINT y)
{
	return pixels[x+L*y];
}

RGBF& ImageCouleurF::operator()(UINT x, UINT y)
{
	return pixels[x+L*y];
}
	
// renvoie la r�f�rence de la k-eme composante du pixel (x,y)
float& ImageCouleurF::comp(UINT x, UINT y, UINT k)
{
	switch (k%3)
	{
		case 0 : return pixels[x+L*y].r;
		case 1 : return pixels[x+L*y].g;
		case 2 : return pixels[x+L*y].b;
	}
}
	
// renvoie la r�f�rence de la k-eme composante du pixel i
float& ImageCouleurF::comp(UINT i, UINT k)
{
	switch (k%3)
	{
		case 0 : return pixels[i].r;
		case 1 : return pixels[i].g;
		case 2 : return pixels[i].b;
	}
}
	
// remplace le pixel (x,y) avec la valeur RGB v
// ne fait rien si (x,y) est hors de l'image
void ImageCouleurF::pixel(UINT x, UINT y, RGBF v)
{
	if (x<L && y<H)
	{
		pixels[x+L*y] = v;
	}
}

///////////////////////////////////////////////////////////////////////////
// afficher l'image dans une fenetre graphique avec le zoom z 
// (par defaut z=1)
// renvoie le num�ro de la fenetre cr��e
int ImageCouleurF::afficher(UINT z)
{
	return creer_fenetre_graphique(*this, z);
}

///////////////////////////////////////////////////////////////////////////
// afficher l'image dans la console 
// avec le format f pour l'affichage des valeurs
// par d�faut f = "%6.2f"
///////////////////////////////////////////////////////////////////////////
void ImageCouleurF::print()
{
	for (UINT y=0; y<H; y++)
	{
		for (UINT x=0; x<L; x++)
		{
			RGBF v = pixel(x,y);
			printf(" (");
			printf("%6.2f", v.r);
			printf(",");
			printf("%6.2f", v.g);
			printf(",");
			printf("%6.2f", v.b);
			printf(")");
		}
		printf("\n");
	}
}

void ImageCouleurF::print(const char *f)
{
	for (UINT y=0; y<H; y++)
	{
		for (UINT x=0; x<L; x++)
		{
			RGBF v = pixel(x,y);
			printf(" (");
			printf(f, v.r);
			printf(",");
			printf(f, v.g);
			printf(",");
			printf(f, v.b);
			printf(")");
		}
		printf("\n");
	}
}

///////////////////////////////////////////////////////////////////////////
// sauvegarde de l'image dans un fichier image
///////////////////////////////////////////////////////////////////////////

// sauvegarde dans un fichier suivant l'extension du nom de fichier nom_f
void ImageCouleurF::sauvegarder(const char* nom_f)
{
	CImg<UINT8> I = conversion_avec_zoom(*this);
	I.save(nom_f);
}

// sauvegarde dans un fichier nomm� nom_f au format JPG 
// avec la qualit� q (par d�faut q=100)
// si q>100 alors q=100 est utilis�
void ImageCouleurF::sauvegarder_jpeg(const char* nom_f, UINT8 q) 
{
	CImg<UINT8> I = conversion_avec_zoom(*this);
	if (q==0) q=1;
	if (q>100) q=100;
	I.save_jpeg(nom_f, q);
} 
	
///////////////////////////////////////////////////////////////////////////
// destructeur
///////////////////////////////////////////////////////////////////////////
ImageCouleurF::~ImageCouleurF()
{
	if (pixels)
		delete[] pixels;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe ImageIndexee : image couleur en mode indexe 
//   table de N couleurs RGB (1 <= N <= 256)
//   valeurs des pixels entre 0 et N-1
//   si N=0 l'image est noire
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// constructeurs
///////////////////////////////////////////////////////////////////////////

// cr�ation d'une image vide
ImageIndexee::ImageIndexee()
{
	pixels = NULL;
	table = NULL;
	L = H = N = 0;
}

// cr�ation d'une image de dimension L0 par H0
// avec une table 2 couleurs (Blanc=0,Noir=1)
// et un tableau de pixels p et
// si p = NULL alors les tous les pixels de l'image sont initialis�s � 0
ImageIndexee::ImageIndexee(UINT L0, UINT H0, UINT *p)
{
	L = L0;
	H = H0;
	N = 2;
	pixels = new UINT[L*H];
	table = new RGB[2];
	
	if (p)
	{
		// copie des valeurs de p
		for (UINT i=0; i<L*H; i++)
			pixels[i] = p[i];
	}
	else
	{
		// tous les pixels � 0
		for (UINT i=0; i<L*H; i++)
			pixels[i] = 0;
	}
	
	table[0] = RGB(255,255,255);
	table[1] = RGB(0,0,0);
}

	
// cr�ation d'une image de dimension L0 par H0
// avec N0 couleurs avec un tableau de pixels p et
// une table de couleurs T
// si p = NULL alors les tous les pixels de l'image sont initialis�s � 0
// si T = NULL alors une table avec toutes les couleurs noires
ImageIndexee::ImageIndexee(UINT L0, UINT H0, UINT N0, RGB *T, UINT *p)
{
	L = L0;
	H = H0;
	N = N0;
	pixels = new UINT[L*H];
	table = new RGB[N];
	
	if (p)
	{
		// copie des valeurs de p
		for (UINT i=0; i<L*H; i++)
			pixels[i] = p[i];
	}
	else
	{
		// tous les pixels � 0
		for (UINT i=0; i<L*H; i++)
			pixels[i] = 0;
	}
	
	if (T)
	{
		// copie des valeurs de T
		for (UINT i=0; i<N; i++)
			table[i] = T[i];
	}
	else
	{
		// copie des valeurs de T
		for (UINT i=0; i<N; i++)
			table[i] = RGB(0,0,0);
	}
}

// constructeur par copie de I
ImageIndexee::ImageIndexee(ImageIndexee &I)
{
	L = I.L;
	H = I.H;
	N = I.N;
	pixels = new UINT[L*H];
	table = new RGB[N];

	// copie des valeurs de I.pixels
	for (UINT i=0; i<L*H; i++)
		pixels[i] = I.pixels[i];

	// copie des valeurs de I.table
	for (UINT i=0; i<N; i++)
		table[i] = I.table[i];

}

///////////////////////////////////////////////////////////////////////////
// surcharges
///////////////////////////////////////////////////////////////////////////

// surcharge de l'op�rateur =
ImageIndexee& ImageIndexee::operator=(const ImageIndexee& I)
{	
	if (this != &I)
	{
		if (pixels) 
			delete []pixels;
		if (table) 
			delete []table;
		L = I.L;
		H = I.H;
		N = I.N;
		pixels = new UINT[L*H];
		table = new RGB[N];
	
		// copie des valeurs de I.pixels
		for (UINT i=0; i<L*H; i++)
			pixels[i] = I.pixels[i];
	
		// copie des valeurs de I.table
		for (UINT i=0; i<N; i++)
			table[i] = I.table[i];
	}
	return *this;
}

///////////////////////////////////////////////////////////////////////////
// acc�s � l'image
///////////////////////////////////////////////////////////////////////////

// renvoie la largeur
UINT ImageIndexee::largeur()
{
	return L;
}

// renvoie la largeur
UINT ImageIndexee::hauteur()
{
	return H;
}
	
// renvoie le nombre de pixels
UINT ImageIndexee::nb_pixels()
{
	return L*H;
}
	
// renvoie le nombre de couleurs de la table
UINT ImageIndexee::nb_couleurs()
{
	return N;
}
	
// renvoie la r�f�rence du pixel(i) = pixel(x+y*largeur)
UINT& ImageIndexee::pixel(UINT i)
{
	return pixels[i];
}
UINT& ImageIndexee::operator()(UINT i)
{
	return pixels[i];
}
	
// renvoie la r�ference du pixel (x,y)
UINT& ImageIndexee::pixel(UINT x, UINT y)
{
	return pixels[x+L*y];
}
UINT& ImageIndexee::operator()(UINT x, UINT y)
{
	return pixels[x+L*y];
}


// renvoie la couleur d'indice i
// renvoie RGB(0,0,0) c'est � dire noir si i>=N
RGB ImageIndexee::couleur(UINT i)
{
	if (i>=N) return RGB(0,0,0);
	return table[i];
}

// modifie le pixel (x,y) avec la valeur i
// ne fait rien si (x,y) est hors de l'image ou si i>=N
void ImageIndexee::pixel(UINT x, UINT y, UINT i)
{
	if (x>=L || y>=H || i>=N) return;
	pixels[x+L*y] = i;
}

// modifie la couleur d'indice i avec la couleur RGB c
// ne fait rien si i>=N
void ImageIndexee::couleur(UINT i, RGB c)
{
	if (i>=N) return;
	table[i] = c;
}

// modifie la couleur d'indice i avec la couleur RGB (r,g,b)
// ne fait rien si i>=N
void ImageIndexee::couleur(UINT i, UINT8 r, UINT8 g, UINT8 b)
{
	if (i>=N) return;
	table[i] = RGB(r,g,b);
}


///////////////////////////////////////////////////////////////////////////
// afficher l'image dans une fenetre graphique avec le zoom z 
// (par defaut z=1)
// renvoie le num�ro de la fenetre cr��e
int ImageIndexee::afficher(UINT z)
{
	return creer_fenetre_graphique(*this, z);
}
 
///////////////////////////////////////////////////////////////////////////
// afficher l'image dans la console 
// avec le format f pour l'affichage des valeurs
///////////////////////////////////////////////////////////////////////////
void ImageIndexee::print()
{
	printf("Image de dimensions %ld x %ld : \n", L, H);
	for (UINT y=0; y<H; y++)
	{
		for (UINT x=0; x<L; x++)
		{
			printf(" %3ld", pixels[x+L*y]);
		}
		printf("\n");
	}
	printf("Table avec %ld couleurs : \n", N);
	for (UINT i=0; i<N; i++)
	{
		printf(" couleur %3ld : [", i);
		printf("%3d", table[i].r);
		printf(",");
		printf("%3d", table[i].g);
		printf(",");
		printf("%3d", table[i].b);
		printf("]\n");
	}
}

void ImageIndexee::print(const char *f)
{
	printf("Image de dimensions %ld x %ld : \n", L, H);
	for (UINT y=0; y<H; y++)
	{
		for (UINT x=0; x<L; x++)
		{
			printf(f, pixels[x+L*y]);
			printf(" ");
		}
		printf("\n");
	}
	printf("Table avec %ld couleurs : \n", N);
	for (UINT i=0; i<N; i++)
	{
		printf(" couleur ");
		printf(f, i);
		printf(" : [");
		printf("%3d", table[i].r);
		printf(",");
		printf("%3d", table[i].g);
		printf(",");
		printf("%3d", table[i].b);
		printf("]\n");
	}
}

///////////////////////////////////////////////////////////////////////////
// sauvegarde de l'image dans un fichier image
///////////////////////////////////////////////////////////////////////////

// sauvegarde dans un fichier suivant l'extension du nom de fichier nom_f
void ImageIndexee::sauvegarder(const char* nom_f)
{
	CImg<UINT8> I = conversion_avec_zoom(*this);
	I.save(nom_f);
}

// sauvegarde dans un fichier nomm� nom_f au format JPG 
// avec la qualit� q (par d�faut q=100)
// si q>100 alors q=100 est utilis�
void ImageIndexee::sauvegarder_jpeg(const char* nom_f, UINT8 q) 
{
	CImg<UINT8> I = conversion_avec_zoom(*this);
	if (q==0) q=1;
	if (q>100) q=100;
	I.save_jpeg(nom_f, q);
} 
	
///////////////////////////////////////////////////////////////////////////
// destructeur
///////////////////////////////////////////////////////////////////////////
ImageIndexee::~ImageIndexee()
{
	if (pixels)
		delete[] pixels;
	if (table)
		delete[] table;
}



///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe Histogramme : histogramme d'une Image
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
	
///////////////////////////////////////////////////////////////////////////
// constructeurs
///////////////////////////////////////////////////////////////////////////

// Histogramme d'une image en niveaux de gris I avec n0 classes
// par d�faut n0=256
// si n0=0 alors une seule classe est choisie
// si n0>256 alors 256 classes sont choisies
Histogramme::Histogramme(ImageGris& I, UINT n0)
{
	if (n0<1) n0 = 1;
	if (n0>256) n0 = 256;
	n = n0;
	CImg<UINT> *H = new CImg<UINT>(n+1,1,1,1,0);
	data = (void *)H;
	classes = H->data();
	
	// calcul de l'histogramme
	for (UINT k=0; k<I.largeur()*I.hauteur(); k++)
	{ 
		UINT v = ((UINT)I.pixel(k)*n)/256;
		classes[v]++;
	}
}

Histogramme::Histogramme(ImageGrisF& I, UINT n0)
{
	if (n0<1) n0 = 1;
	if (n0>256) n0 = 256;
	n = n0;
	CImg<UINT> *H = new CImg<UINT>(n+1,1,1,1,0);
	data = (void *)H;
	classes = H->data();
	
	// calcul de l'histogramme
	for (UINT k=0; k<I.largeur()*I.hauteur(); k++)
	{
		float vf = I.pixel(k)*255.0;
		if (vf<  0.0) vf =   0.0;
		if (vf>255.0) vf = 255.0; 
		UINT v = ((UINT)vf*n)/256;
		classes[v]++;
	}
}

// Histogramme d'une image couleur I avec n0 classes
// par d�faut n0=256
// si n0=0 alors une seule classe est choisie
// si n0>256 alors 256 classes sont choisies
// pour le calcul de l'histogramme, 
// on prend en compte les composantes R, G et/ou B
// suivant les valeurs respectives des boole�ns cR, cG et cB
// si cR=cG=cB=false alors l'histogramme est calcul� sur l'image
// convertie en niveaux de gris 
Histogramme::Histogramme(ImageCouleur& I, UINT n0, bool cR, bool cG, bool cB)
{
	if (n0<1) n0 = 1;
	if (n0>256) n0 = 256;
	n = n0;
	CImg<UINT> *H = new CImg<UINT>(n+1,1,1,1,0);
	data = (void *)H;
	classes = H->data();

	
	// calcul de l'histogramme
	for (UINT k=0; k<I.largeur()*I.hauteur(); k++)
	{
		RGB p = I.pixel(k);
		if (cR || cG || cB)
		{
			if (cR) classes[(n*(UINT)p.r)/256]++;
			if (cG) classes[(n*(UINT)p.g)/256]++;
			if (cB) classes[(n*(UINT)p.b)/256]++;
		}
		else
		{
			classes[(n*(UINT)p.gris())/256]++;
		}
	}
}

Histogramme::Histogramme(ImageCouleurF& I, UINT n0, bool cR, bool cG, bool cB)
{
	if (n0<1) n0 = 1;
	if (n0>256) n0 = 256;
	n = n0;
	CImg<UINT> *H = new CImg<UINT>(n+1,1,1,1,0);
	data = (void *)H;
	classes = H->data();

	// calcul de l'histogramme
	for (UINT k=0; k<I.largeur()*I.hauteur(); k++)
	{
		RGBF p = I.pixel(k);
		if (cR || cG || cB)
		{
			if (cR) classes[((UINT)float_to_UINT8(p.r)*n)/256]++;
			if (cG) classes[((UINT)float_to_UINT8(p.g)*n)/256]++;
			if (cB) classes[((UINT)float_to_UINT8(p.b)*n)/256]++;
		}
		else
		{
			classes[((UINT)float_to_UINT8(p.gris())*n)/256]++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// acc�s � l'histogramme
///////////////////////////////////////////////////////////////////////////

// renvoie le nombre de classes de l'histogramme
UINT Histogramme::nb_classes()
{
	return n;
}

// renvoie l'effectif de la classe i (i entre 0 et nb_classes()-1)
// si i>=nb_classes alors la fonction renvoie 0
UINT Histogramme::classe(UINT i)
{
	return (i<n ? classes[i] : 0);
}

// d�terminer la valeur max de H, i.e. l'effectif maximal parmi les classes
UINT Histogramme::max()
{
	UINT v = 0;
	for (UINT i=0; i<n; i++)
		if (v<classes[i]) v = classes[i];
	return v;
}

///////////////////////////////////////////////////////////////////////////
// afficher l'histogramme dans une fenetre graphique de dimensions L x H
// par d�faut L = 511 et H = 250
// renvoie le num�ro de la fenetre cr��e
int Histogramme::afficher(UINT L, UINT H)
{
	// couleur des barres de l'histogramme
	const unsigned char vert[]  = { 50, 255, 50 };

	// image RGB de l'histogramme de dimensions L x H
	// avec le fond noir
	CImg <UINT8> HImage(L,H,1,3,0);
	
	// ajouter le graphe de l'histogramme en vert
	HImage.draw_graph(*(CImg<UINT> *)data,vert,1,3,0,max(),0);

	// afficher l'image HImage
	// creer la fenetre pour l'affichage de HImage
	num_fen++;
	char titre[100];
	sprintf(titre, "Fen. %d", num_fen);
	CImgDisplay *CID = new CImgDisplay(HImage, titre, 0);
	
	// ajouter la fenetre � la liste
	fenetre f = {CID, num_fen};
	liste_f.push_back(f);	
	
	return num_fen;
}
	 
///////////////////////////////////////////////////////////////////////////
// afficher l'histogramme dans une fenetre graphique de dimensions L x H
// avec cF la couleur de fond et cH la couleur de l'histogramme 
// par d�faut L = 511 et H = 250
// renvoie le num�ro de la fenetre cr��e
int Histogramme::afficher(RGB cF, RGB cH, UINT L, UINT H)
{
	// couleur des barres de l'histogramme
	const unsigned char coul_histogramme[]  = { cH.r, cH.g, cH.b };

	// image RGB de l'histogramme de dimensions L x H
	// avec le fond noir
	CImg <UINT8> HImage(L,H,1,3,0);
	
	// remplir l'image avec la couleur cF
	remplir_couleur(HImage, cF);
	
	// ajouter le graphe de l'histogramme avec la couleur cH
	HImage.draw_graph(*(CImg<UINT> *)data,coul_histogramme,1,3,0,max(),0);

	// afficher l'image HImage
	// creer la fenetre pour l'affichage de HImage
	num_fen++;
	char titre[100];
	sprintf(titre, "Fen. %d", num_fen);
	CImgDisplay *CID = new CImgDisplay(HImage, titre, 0);
	
	// ajouter la fenetre � la liste
	fenetre f = {CID, num_fen};
	liste_f.push_back(f);	
	
	return num_fen;
}
	 
///////////////////////////////////////////////////////////////////////////
// afficher l'histogramme dans une fenetre graphique de dimensions L x H
// avec cF la couleur de fond et cH la couleur de l'histogramme 
// par d�faut L = 511, H = 250, cF=Noir, cH=Rouge
// affiche l'axe x avec valeurs num�riques entre 0.0 et 1.0 
// dans une bande de 15 pixels au dessous de l'histogramme
// la partie pour l'affichage de l'histogramme est au minimum de 30 pixels
// renvoie le num�ro de la fenetre cr��e
int Histogramme::afficher_0_1(RGB cF, RGB cH, UINT L, UINT H)
{
	// couleur des barres de l'histogramme
	const UINT8 coul_histogramme[]  = { cH.r, cH.g, cH.b };
	RGB cAxe(255,255,255);
	const UINT8 couleur_texte[] ={0,80,160};

	// faire en sorte que la hauteur soit au moins �gale � 45
	if (H<45) H=45;
	 
	// image RGB de l'histogramme de dimensions L x H
	// avec le fond noir
	CImg <UINT8> HImage(L,H,1,3,0);
	
	// remplir l'image avec la couleur cF 
	remplir_couleur(HImage, cF);
	
	// remplir le bas de l'image avec la couleur cAxe 
	remplir_rectangle_couleur(HImage, 0,H-14,L,H, cAxe);
	
	// ajouter les indications de valeurs sur l'axe x
	CImg<float> vX(11,1,1,1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0);
	HImage.draw_axis(vX,H,couleur_texte);
	
	// ajouter le graphe de l'histogramme avec la couleur cH
	float m = (float)max();
	if (m==0.0) m=1.0;
	HImage.draw_graph
	 (*(CImg<UINT> *)data,coul_histogramme,1,3,0,m,15.0*m/(15.0-H));

	// afficher l'image HImage
	// creer la fenetre pour l'affichage de HImage
	num_fen++;
	char titre[100];
	sprintf(titre, "Fen. %d", num_fen);
	CImgDisplay *CID = new CImgDisplay(HImage, titre, 0);
	
	// ajouter la fenetre � la liste
	fenetre f = {CID, num_fen};
	liste_f.push_back(f);	
	
	return num_fen;
}

int Histogramme::afficher_0_1(UINT L, UINT H, RGB cF, RGB cH)
{
	return afficher_0_1(cF, cH, L, H);
}

///////////////////////////////////////////////////////////////////////////
// afficher l'histogramme dans une fenetre graphique de dimensions L x H
// avec cF la couleur de fond et cH la couleur de l'histogramme 
// par d�faut L = 511, H = 250, cF=Noir, cH=Rouge
// affiche l'axe x avec valeurs num�riques entre 0 et 255
// dans une bande de 15 pixels au dessous de l'histogramme
// la partie pour l'affichage de l'histogramme est au minimum de 30 pixels
// renvoie le num�ro de la fenetre cr��e
int Histogramme::afficher_0_255(RGB cF, RGB cH, UINT L, UINT H)
{
	// couleur des barres de l'histogramme
	const UINT8 coul_histogramme[]  = { cH.r, cH.g, cH.b };
	RGB cAxe(255,255,255);
	const UINT8 couleur_texte[] ={0,80,160};

	// faire en sorte que la hauteur soit au moins �gale � 45
	if (H<45) H=45;
	 
	// image RGB de l'histogramme de dimensions L x H
	// avec le fond noir
	CImg <UINT8> HImage(L,H,1,3,0);
	
	// remplir l'image avec la couleur cF 
	remplir_couleur(HImage, cF);
	
	// remplir le bas de l'image avec la couleur cAxe 
	remplir_rectangle_couleur(HImage, 0,H-14,L,H, cAxe);
	
	// ajouter les indications de valeurs sur l'axe x
	CImg<UINT> vX(6,1,1,1,0,51,102,153,204,255);
	HImage.draw_axis(vX,H,couleur_texte);
	
	// ajouter le graphe de l'histogramme avec la couleur cH
	float m = (float)max();
	if (m==0.0) m=1.0;
	HImage.draw_graph
	 (*(CImg<UINT> *)data,coul_histogramme,1,3,0,m,15.0*m/(15.0-H));

	// afficher l'image HImage
	// creer la fenetre pour l'affichage de HImage
	num_fen++;
	char titre[100];
	sprintf(titre, "Fen. %d", num_fen);
	CImgDisplay *CID = new CImgDisplay(HImage, titre, 0);
	
	// ajouter la fenetre � la liste
	fenetre f = {CID, num_fen};
	liste_f.push_back(f);	
	
	return num_fen;
}
	 
int Histogramme::afficher_0_255(UINT L, UINT H, RGB cF, RGB cH)
{
	return afficher_0_255(cF, cH, L, H);
}

///////////////////////////////////////////////////////////////////////////
// afficher l'histogramme dans la console 
// avec le format f pour l'affichage des valeurs
///////////////////////////////////////////////////////////////////////////
void Histogramme::print()
{
	for (UINT k=0; k<n; k++)
	{
		printf("Classe %3ld : ", k);
		printf("%ld", classes[k]);
		printf("\n");
	}
}

void Histogramme::print(const char *f)
{
	for (UINT k=0; k<n; k++)
	{
		printf("Classe %3ld : ", k);
		printf(f, classes[k]);
		printf("\n");
	}
}

///////////////////////////////////////////////////////////////////////////
// destructeur
///////////////////////////////////////////////////////////////////////////
Histogramme::~Histogramme()
{
	delete (CImg<UINT>*)data;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//
// Classe Fonction : fonction d�finie sur [0,1] et � valeurs r�elles
//                   x --> f(x)
//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// constructeurs
///////////////////////////////////////////////////////////////////////////

// cr�ation � partir d'une fonction f0 d�finie avec le profil
// float f(float x)
Fonction::Fonction(float (*f0)(float))
{
	f = f0;
}

///////////////////////////////////////////////////////////////////////////
// utilisation de la fonction
///////////////////////////////////////////////////////////////////////////

// renvoie la valeur f(x) avec x de type float
float Fonction::operator()(float x)
{
	return f(x);
}

// renvoie la valeur f(x) avec x de type ImageGrisF
ImageGrisF Fonction::operator()(ImageGrisF &I)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	ImageGrisF fI(L,H);
	for (UINT x=0; x<L; x++)
	for (UINT y=0; y<H; y++)
		fI(x,y) = f(I.pixel(x,y));
	return fI;
}

// renvoie la valeur f(x) avec x de type ImageCouleurF
// la fonction f est appliqu�e aux trois composantes R, G et B
ImageCouleurF Fonction::operator()(ImageCouleurF &I)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	ImageCouleurF fI(L,H);
	for (UINT x=0; x<L; x++)
	for (UINT y=0; y<H; y++)
	{
		fI(x,y).r = f(I(x,y).r);
		fI(x,y).g = f(I(x,y).g);
		fI(x,y).b = f(I(x,y).b);
	}	
	return fI;
}

///////////////////////////////////////////////////////////////////////////
// afficher la fonction dans une fenetre graphique de dimensions L x H
// par d�faut L = 400 et H = 400
// le rep�re pour le trac� est x entre 0 et 1 et y entre 0 et 1
// renvoie le num�ro de la fenetre cr��e 
int Fonction::afficher(UINT L, UINT H)
{
	// image RGB de dimensions L x H avec le fond blanc
	CImg <UINT8> FImage(L,H,1,3,255);
	
	float x0 = 0;
	float x1 = L;
	float y0 = H;
	float y1 = 0;
	
	UINT8 c_grille[]   = {128,128,128};
	UINT8 c_fonction[] = {255,  0,  0};
	UINT8 c_axes[]     = {  0,128,128};
	
	////////////// dessin de la grille //////////////
	// les valeurs de la grille suivant l'axe x
	CImg<float> gx(6,1,1,1);
	for (int i=0; i<6; i++) 
		gx(i) = x0+i*(x1-x0)/5.0;
	
	// les valeurs de la grille suivant l'axe y
	CImg<float> gy(6,1,1,1);
	for (int i=0; i<6; i++) 
		gy(i) = y0+i*(y1-y0)/5.0;
	
	// dessiner la grille
	FImage.draw_grid(gx,gy,c_grille,1,0x44444444,0x44444444);
	
	////////////// dessin des axes //////////////
	// les valeurs du repere suivant l'axe x
	CImg<float> vx(6,1,1,1,0.0,0.2,0.4,0.6,0.8,1.0);
	
	// les valeurs du repere suivant l'axe y
	CImg<float> vy(6,1,1,1,1.0,0.8,0.6,0.4,0.2,0.0);
	
	// dessiner les axes
	FImage.draw_axes(vx,vy,c_axes);
	
	
	////////////// dessin de la fonction //////////////
	float xs1 = x0;
	float ys1 = y0+f((xs1-x0)/(x1-x0))*(y1-y0);
	float xs2;
	float ys2;
	
	for (float x=x0+1; x<=x1; x+=1)
	{
		float xs2 = x;
		float ys2 = y0+f((xs2-x0)/(x1-x0))*(y1-y0);	
		FImage.draw_line(xs1,ys1,xs2,ys2,c_fonction);
		xs1 = xs2; 
		ys1 = ys2; 
	}

	////////////// affichage de l'image FImage //////////////
	// creer la fenetre pour l'affichage de FImage
	num_fen++;
	char titre[100];
	sprintf(titre, "Fen. %d", num_fen);
	CImgDisplay *CID = new CImgDisplay(FImage, titre, 0);
	
	// ajouter la fenetre � la liste
	fenetre f = {CID, num_fen};
	liste_f.push_back(f);	
	
	return num_fen;
}
// int Fonction::afficher(UINT L, UINT H)
// {
// 	// couleur du graphe de la fonction
// 	const unsigned char bleu[]  = { 0, 0, 255 };
// 
// 	// calculer la fonction sur L valeurs entre 0 et 1
// 	CImg<float> fv(L,1,1,1);
// 	
// 	for (UINT i=0; i<L; i++)
// 	{
// 		float v = float(i)/float(L-1);
// 		fv(i) = f(v);
// 	}
// 	
// 	// image RGB de l'histogramme de dimensions L x H
// 	// avec le fond blanc
// 	CImg <UINT8> FImage(L,H,1,3,255);
// 	
// 	// ajouter le graphe de l'histogramme en vert
// 	FImage.draw_graph(fv,bleu,1,1,0,1.0,0.0);
// 
// 	// afficher l'image HImage
// 	// creer la fenetre pour l'affichage de HImage
// 	num_fen++;
// 	char titre[100];
// 	sprintf(titre, "Fen. %d", num_fen);
// 	CImgDisplay *CID = new CImgDisplay(FImage, titre, 0);
// 	
// 	// ajouter la fenetre � la liste
// 	fenetre f = {CID, num_fen};
// 	liste_f.push_back(f);	
// 	
// 	return num_fen;
// }



