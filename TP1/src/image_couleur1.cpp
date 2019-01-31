//////////////////////////////////////////////////////////////////////////////
// création et affichage d'une image en mode RGB 24 bits
// 
// N. SZAFRAN - UFR-IM2AG - 2015
//////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>

// les couleurs de base
RGB Blanc  (255,255,255);
RGB Noir   (  0,  0,  0);
RGB Rouge  (255,  0,  0);
RGB Vert   (  0,255,  0);
RGB Bleu   (  0,  0,255);
RGB Cyan   (  0,255,255);
RGB Magenta(255,  0,255);
RGB Jaune  (255,255,  0);

int main()
{
	// le tableau de pixels
	RGB p[] = 
	{  Noir , Bleu,    Vert,  Cyan,
	   Rouge, Magenta, Jaune, Blanc};
	
	// création de l'image I1 de dimensions 4 x 2 
	// à partir du tableau de pixels p
	ImageCouleur I1(4,2,p);
	
	// affichage de l'image I1 avec un facteur d'échelle 100
	UINT num_I1 = I1.afficher(100);
	changer_titre_fenetre_graphique(num_I1, "Image I1");
	
	// création de l'image I2 de dimensions 256 x 256
	ImageCouleur I2(256,256);
	
	// définir les valeurs pixels
	for (int x=0; x<=255; x++)
	for (int y=0; y<=255; y++)
		I2.pixel(x,y) = RGB(x,y,0);
	
	// affichage de l'image I2 
	UINT num_I2 = I2.afficher();
	changer_titre_fenetre_graphique(num_I2, "Image I2");
	
	//-------------------------------------------------------------------
	// création et affichage d'une image représentant le drapeau français
	
	// ---- PARTIE A COMPLETER ---- //
	
	
	//----------------------------------------------------------
	// création et affichage d'image représentant l'ensemble des couleurs RGB
	// en fixant une des 3 composantes :
	// - image avec la composante B égale à 255
	// - image avec la composante G égale à 0
	// - image avec la composante G égale à 255
	// - image avec la composante R égale à 0
	// - image avec la composante R égale à 255
	
	// ---- PARTIE A COMPLETER ---- //
	
	
	//-------------------------------
	// en attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();
	
	
	//------------------------------------
	// suppression des fenetres graphiques	
	supprimer_fenetre_graphique(num_I1);
	supprimer_fenetre_graphique(num_I2);

	return 0;
}
