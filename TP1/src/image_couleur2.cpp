//////////////////////////////////////////////////////////////////////////////
// création et affichage d'une image en mode RGBF réel
// 
// N. SZAFRAN - UFR-IM2AG - 2015
//////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>

// les couleurs de base
RGBF Blanc  (255,255,255);
RGBF Noir   (  0,  0,  0);
RGBF Rouge  (255,  0,  0);
RGBF Vert   (  0,255,  0);
RGBF Bleu   (  0,  0,255);
RGBF Cyan   (  0,255,255);
RGBF Magenta(255,  0,255);
RGBF Jaune  (255,255,  0);

int main()
{
	// le tableau de pixels
	RGBF p[] = 
	{  Noir , Bleu,    Vert,  Cyan,
	   Rouge, Magenta, Jaune, Blanc};
	
	// création de l'image I1 de dimensions 4 x 2 
	// à partir du tableau de pixels p
	ImageCouleurF I1(4,2,p);
	
	// affichage de l'image I1 avec un facteur d'échelle 100
	UINT num_I1 = I1.afficher(100);
	changer_titre_fenetre_graphique(num_I1, "Image I1");
	
	// création de l'image I2 de dimensions 256 x 256
	ImageCouleurF I2(256,256);
	
	// définir les valeurs pixels
	for (int x=0; x<=255; x++)
	for (int y=0; y<=255; y++)
		I2.pixel(x,y) = RGBF((float)x/255.0,(float)y/255.0,0.0);
	
	// affichage de l'image I2 
	UINT num_I2 = I2.afficher();
	changer_titre_fenetre_graphique(num_I2, "Image I2");
	
	// attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();
	
	// suppression des fenetres graphiques	
	supprimer_fenetre_graphique(num_I1);
	supprimer_fenetre_graphique(num_I2);

	return 0;
}
