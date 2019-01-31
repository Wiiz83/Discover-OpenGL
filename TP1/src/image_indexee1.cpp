//////////////////////////////////////////////////////////////////////////////
// création et affichage d'une image en mode indexé
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
	//--------------------------
	// image du drapeau francais
	
	// le tableau de pixels
	UINT p[] = 
	{ 0,1,2,
	  0,1,2 };
	
	// la table de couleurs
	RGB T[] = {Bleu,Blanc,Rouge};
	
	// création de l'image I1 de dimensions 3 x 2 
	// avec une table T de 3 couleurs et le tableau de pixels p
	ImageIndexee I1(3,2,3,T,p);
	
	// affichage de l'image I1 avec un facteur d'échelle 100
	UINT num_I1 = I1.afficher(100);
	changer_titre_fenetre_graphique(num_I1, "Image I1");
	

	//-----------------------
	// image en noir et blanc
	
	// création de l'image I2 de dimensions 4 x 3 en noir en blanc
	// avec tous les pixels blancs (égaux à 0) 
	ImageIndexee I2(4,3);
	
	// mettre certains pixels en noir (1)
	I2.pixel(2,0) = 1; // ou bien I2.pixel(2,0,1)
	I2.pixel(3,2) = 1; // ou bien I2.pixel(3,2,1)
	I2.pixel(1,1,1);   // ou bien  I2.pixel(1,1) = 1
	I2.pixel(1,2,1);   // ou bien  I2.pixel(1,2) = 1
	
	// affichage de l'image I2 avec un facteur d'échelle 50
	UINT num_I2 = I2.afficher(50);
	changer_titre_fenetre_graphique(num_I2, "Image I2");
	

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
