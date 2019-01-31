///////////////////////////////////////////////////////////////////////////////
// conversion RGB <--> HSV
// conversion RGB <--> HSL
///////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>
#include <cmath>
#include <iostream>

using namespace std;

// conversion RGB vers HSV
// Entrée : r,g,b = composantes réelles (entre 0.0 et 1.0)
// Sortie : h,s,v = composantes réelles entre 0.0 et 1.0
void rgb2hsv(float r, float g, float b, float &h, float &s, float &v)
{
	// mettre les composantes (r,g,b) entre 0 et 1
	if (r<0.0) r=0.0; if (r>1.0) r=1.0;
	if (g<0.0) g=0.0; if (g>1.0) g=1.0;
	if (b<0.0) b=0.0; if (b>1.0) b=1.0;
	
	// valeur min et max des valeurs r,g,b
	float min, max;
	min = max = r;
	if (min>g) min = g; if (min>b) min = b;
	if (max<g) max = g; if (max<b) max = b;
	
	// calcul de h	
	if (min==max) 
	{
		h = 0.0;
	}
	else if (max==r) 
	{
		h = (g-b)/(max-min)/6.0;
		if (h<0.0) h+=1.0;
	}
	else if (max==g) 
	{
		h = (b-r)/(max-min)/6.0+1.0/3.0;
	}
	else // if (max==b) 
	{
		h = (r-g)/(max-min)/6.0+2.0/3.0;
	}
	
	// calcul de s
	s = max==0.0 ? 0 : 1.0-min/max;
	
	// calcul de v
	v = max;
}


// conversion HSV --> RGB
// Entrée : h,s,v = composantes réelles (entre 0.0 et 1.0)
// Sortie : r,g,b = composantes réelles entre 0.0 et 1.0
void hsv2rgb(float h, float s, float v, float &r, float &g, float &b)
{
	// mettre les composantes (h,s,v) entre 0 et 1
	if (h<0.0) h=0.0; if (h>1.0) h=1.0;
	if (s<0.0) s=0.0; if (s>1.0) s=1.0;
	if (v<0.0) v=0.0; if (v>1.0) v=1.0;
	
	int hi = (int)floor(h*6.0);
	float f = h*6.0-(float)hi;
	float l = v*(1.0-s);
	float m = v*(1.0-f*s);
	float n = v*(1.0-(1.0-f)*s);

	switch (hi)
	{
		case 0 : r=v; g=n; b=l; break;
		case 1 : r=m; g=v; b=l; break;
		case 2 : r=l; g=v; b=n; break;
		case 3 : r=l; g=m; b=v; break;
		case 4 : r=n; g=l; b=v; break;
		case 5 : r=v; g=l; b=m; break;
		case 6 : r=v; g=n; b=l; break; // idem cas 0
	}
}

// conversion d'une ImageCouleurF I du mode RGB au mode HSV
ImageCouleurF rgb2hsv(ImageCouleurF &I)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	ImageCouleurF I2(L,H);
	
	for (UINT i=0; i<L*H; i++)
	{
		float r,g,b,h,s,v;
		r = I(i).r; g = I(i).g; b = I(i).b;
		rgb2hsv(r,g,b,h,s,v);
		I2.comp(i,0) = h; I2.comp(i,1) = s; I2.comp(i,2) = v; 
	}
	return I2;
}

// conversion d'une ImageCouleurF I du mode HSV au mode RGB
ImageCouleurF hsv2rgb(ImageCouleurF &I)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	ImageCouleurF I2(L,H);
	
	for (UINT i=0; i<L*H; i++)
	{
		float r,g,b,h,s,v;
		h = I.comp(i,0); s = I.comp(i,1); v = I.comp(i,2);
		hsv2rgb(h,s,v,r,g,b);
		I2(i).r = r; I2(i).g = g; I2(i).b = b; 
	}
	
	return I2;
}

void partie1()
{
	// création d'une image I_HSV avec pour chaque pixel :
	// -- la premiere composante  (H) constante et correspond au vert
	// -- la deuxième composante  (S) variant suivant x 
	// -- la troisieme composante (V) variant suivant y 
	ImageCouleurF I_HSV(301,301);
	
	for (UINT x=0; x<=300; x++)
	for (UINT y=0; y<=300; y++)
	{
		I_HSV.comp(x,y,0) = 1.0/3.0;        // composante H
		I_HSV.comp(x,y,1) = (float)x/300.0; // composante S
		I_HSV.comp(x,y,2) = (float)y/300.0; // composante V
	}
	
	// convertir l'image I_HSV du format HSV au format RGB 
	// --> image I_RGB
	ImageCouleurF I_RGB;
	I_RGB = hsv2rgb(I_HSV);
	
	// affichage de I_RGB
	I_RGB.afficher();
	
	// en attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();
}

void partie2()
{
	// lecture d'une image couleur
	ImageCouleurF I_RGB("bastille.ppm");
	I_RGB.afficher();
	
	// passage au mode HSV
	ImageCouleurF I_HSV;
	I_HSV = rgb2hsv(I_RGB);
	
	//-------------------------------------------------
	// mise à 1.0 des 2ème et 3ème composantes (S et I) 
	for (UINT x=0; x<I_HSV.largeur(); x++)
	for (UINT y=0; y<I_HSV.hauteur(); y++)
	{
		I_HSV.comp(x,y,1) = 1.0;
		I_HSV.comp(x,y,2) = 1.0;
	}
	
	// passage au mode RGB
	ImageCouleurF I2_RGB;
	I2_RGB = hsv2rgb(I_HSV);
	I2_RGB.afficher();
	
	
	// en attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();
}

int main()
{
	partie1();
	return 0;
}
	
