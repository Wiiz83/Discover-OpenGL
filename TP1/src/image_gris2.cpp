//////////////////////////////////////////////////////////////////////////////
// création et affichage d'une image en mode 256 niveaux de gris
// composante réelle
// 
// N. SZAFRAN - UFR-IM2AG - 2015
//////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>

int main()
{
	// ----------------------------- Image I1 ----------------------------- //
	// le tableau de pixels
	float p[] = 
	{  0.0 , 0.5 , 1.0 ,
	   0.25, 0.5 , 0.75};
	
	// création de l'image I1 de dimensions 3 x 2 
	// à partir du tableau de pixels p
	ImageGrisF I1(3,2,p);
	
	// affichage de l'image I1 avec un facteur d'échelle 100
	UINT num_I1 = I1.afficher(100);
	changer_titre_fenetre_graphique(num_I1, "Image I1");

	// ----------------------------- Image I1 ----------------------------- //
	// création de l'image I2 de dimensions 16 x 8
	ImageGrisF I2(16,8);
	
	// définir les valeurs pixels
	for (int x=0; x<=15; x++)
	for (int y=0; y<=7; y++)
		I2.pixel(x,y) = (float)x / 15.0;
	
	// affichage de l'image I2 avec un facteur d'échelle 20
	UINT num_I2 = I2.afficher(20);
	changer_titre_fenetre_graphique(num_I2, "Image I2");
	
	// ----------------------------- Image I3 ----------------------------- //
	// création de l'image I3 de dimensions 401 x 401
	// avec les valeurs du chaque pixel (x,y) égal à (x+y)/800
	
	// -- PARTIE A COMPLETER -- //
	
	// ----------------------------- Image I4 ----------------------------- //
	// création de l'image I4 de dimensions 401 x 401
	// avec les valeurs du chaque pixel (x,y) égal à (x+y)/400-0.5
	
	// -- PARTIE A COMPLETER -- //
	
	// attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();

	// suppression des fenetres graphiques	
	supprimer_fenetre_graphique(num_I1);
	supprimer_fenetre_graphique(num_I2);

	return 0;
}
