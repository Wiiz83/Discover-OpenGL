///////////////////////////////////////////////////////////////////////////////
// modifications de dimensions
///////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// operations sur les valeurs RGBF
//////////////////////////////////////////////////////////////////////////////

// somme de deux valeurs RGBF
RGBF operator+(RGBF p1, RGBF p2)
{
	return RGBF(p1.r+p2.r,p1.g+p2.g,p1.b+p2.b);
}

// produit d'une valeur RGBF par un scalaire
RGBF operator*(float a, RGBF p)
{
	return RGBF(a*p.r,a*p.g,a*p.b);
}

// produit d'une valeur RGBF par un scalaire
RGBF operator*(RGBF p, float a)
{
	return RGBF(a*p.r,a*p.g,a*p.b);
}

// calculer l'image I2 correspondant 
// à l'image I1 de dimensions L1 x H1 agrandie d'un facteur a
// Entrée : I1 = image couleur à valeurs RGB entre 0.0 et 1.0
//          a = réel supérieur à 1
// La fonction renvoie une image couleur I2 à valeurs RGB entre 0.0 et 1.0 
// utilisation de l'interpolation au plus proche voisin
ImageCouleurF agrandissement1(ImageCouleurF& I1, float a)
{
	UINT L1 = I1.largeur();
	UINT H1 = I1.hauteur();
	UINT L2 = (UINT)round(L1*a);
	UINT H2 = (UINT)round(H1*a);
	ImageCouleurF I2(L2,H2);
	
	for (UINT x2=0; x2<L2; x2++)
	for (UINT y2=0; y2<H2; y2++)
	{
		// indices du pixel correspondant dans I1
		UINT x1 = (UINT)round(x2*(L1-1.0)/(L2-1.0));
		UINT y1 = (UINT)round(y2*(H1-1.0)/(H2-1.0));
		I2(x2,y2) = I1(x1,y1);
	}
	
	return I2;
}


ImageCouleurF agrandissement2(ImageCouleurF& I1, float a)
{

	
	UINT L1 = I1.largeur();
	UINT H1 = I1.hauteur();
	UINT L2 = (UINT)round(L1*a);
	UINT H2 = (UINT)round(H1*a);
	ImageCouleurF I2(L2,H2);
	
	for (UINT x2=0; x2<L2; x2++)
	for (UINT y2=0; y2<H2; y2++)
	{

		float xr = (x2*(L1-1.0)/(L2-1.0));
		float yr = (y2*(H1-1.0)/(H2-1.0));

		UINT x = (UINT)floor(xr);
		UINT y = (UINT)floor(yr);

		float dx = xr - x;
		float dy = yr - y;

		
		I2(x2,y2) = (1-dx)*(1-dy)*I1(x,y) + dx*(1-dy)*I1(x+1,y)
			 + (1-dx)*dy*I1(x,y+1) + dx*dy*I1(x+1,y+1);

	}
	
	return I2;



}

int main(int argc, char *argv[])
{
	string nom_f;
	if (argc<2)
	{
		// par défaut le fichier cercles.png
		nom_f = "cercles.png";
	}
	else
	{
		// sinon argv[1]
		nom_f = argv[1];
	}
	
	
	ImageCouleurF I1(nom_f.c_str());
	I1.afficher();


	// calculer l'image I_1_5, agrandissement de I1 du facteur 1.5
	ImageCouleurF I1_1_5;
	I1_1_5 = agrandissement1(I1,1.5);
	I1_1_5.afficher();
	
	// calculer l'image I_2, agrandissement de I1 du facteur 2
	ImageCouleurF I1_2;
	I1_2 = agrandissement1(I1,2);
	I1_2.afficher();
	
	// calculer l'image I_3, agrandissement de I1 du facteur 3
	ImageCouleurF I1_3;
	I1_3 = agrandissement1(I1,3);
	I1_3.afficher();


	// calculer l'image I_1_5, agrandissement de I1 du facteur 1.5
	ImageCouleurF I2_1_5;
	I2_1_5 = agrandissement2(I1,1.5);
	I2_1_5.afficher();
	
	// calculer l'image I_2, agrandissement de I1 du facteur 2
	ImageCouleurF I2_2;
	I2_2 = agrandissement2(I1,2);
	I2_2.afficher();
	
	// calculer l'image I_3, agrandissement de I1 du facteur 3
	ImageCouleurF I2_3;
	I2_3 = agrandissement2(I1,3);
	I2_3.afficher();




	
	// en attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();
	
	return 0;
}


/*

int main(int argc, char *argv[])
{
	if (argc<2 || argc>3)
 	{
 		fprintf(stderr, "syntaxe : %s fichier_image [a]\n", argv[0]);
 		fprintf(stderr, 
 		 " avec fichier_image : nom du fichier image à agrandir\n");
 		fprintf(stderr, 
 		 "      a : facteur d'agrandissement (réel > 1 - par défaut a=2)\n");
 		return 1;
 	}
 	
 	float a = 2.0;
 	if (argc==3)
 	{
 		a = atof(argv[2]);
 		if (a<=1.0)
 		{
 			fprintf(stderr, 
 			 "Le facteur d'agrandissement a doit etre superieur à 1\n");
 			return 1;
 		}
 	}
 	
 	ImageCouleurF I1(argv[1]);
 	I1.afficher();
 	
 	// calculer l'image I2, agrandissement de I1 du facteur a
 	ImageCouleurF I2;
 	I2 = agrandissement1(I1,a);
 	I2.afficher();
 	
 	// en attente de taper une touche
 	printf("Taper un caractere au clavier pour continuer");
 	getchar();
 	
 	return 0;
}
*/
