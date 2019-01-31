///////////////////////////////////////////////////////////////////////////////
// affichage d'histogrammes
///////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "CImg.h"

// définition de quelques couleurs
RGB noir(0,0,0);
RGB blanc(255,255,255);
RGB rouge(255,0,0);
RGB vert(0,255,0);
RGB bleu(0,0,255);
RGB cyan(255,255,255);
RGB magenta(255,0,255);
RGB jaune(255,255,0);
RGB gris(105,105,105);

int main(int argc, char *argv[])
{
	ImageCouleur I;
	
	// chargement de l'image
	if (argc<2)
		I = ImageCouleur("bastille.png"); // POUR COULEUR
	else
		I = ImageCouleur(argv[1]); // POUR COULEUR
	I.afficher();
		

	// nombre de classes de l'histogramme
	UINT nb_classes;
	if (argc<3)
		nb_classes = 256;
	else
		nb_classes = atoi(argv[2]);
	
	// ramener le nombre de classes entre 2 et 256
	if (nb_classes<2  ) nb_classes = 2  ;
	if (nb_classes>256) nb_classes = 256;
		
	

	Histogramme H_Rouge(I, nb_classes, true, false, false);
	H_Rouge.afficher(blanc, rouge);
	
	Histogramme H_Vert(I, nb_classes, false, true, false);
	H_Vert.afficher(blanc, vert);
	
	Histogramme H_Bleu(I, nb_classes, false, false, true);
	H_Bleu.afficher(blanc, bleu);
	
	Histogramme H_All(I, nb_classes, true, true, true);
	H_All.afficher(blanc, noir);
	
	Histogramme H_Gris(I, nb_classes, false, false, false);
	H_Gris.afficher(blanc, gris);
	

	
	// en attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();

	return 0;
}
