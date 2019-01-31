//////////////////////////////////////////////////////////////////////////////
// affichage séparé des composantes R G B 
// 
// N. SZAFRAN - UFR-IM2AG - 2015
//////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>
#include <cmath>

int main(int argc, char **argv)
{
	// créer l'image I1 à partir du fichier argv[1]
	// ou bien le fichier papillon.png
	ImageCouleur I(argc==2 ? argv[1] : "papillon.png");
	UINT N = I.nb_pixels();
	UINT num_I = I.afficher();
	changer_titre_fenetre_graphique(num_I, "Image");

	// conserver uniquement la composante R
	ImageCouleur IR(I);	
	for (UINT i=0; i<N; i++)
	{
		IR(i).g = 0;
		IR(i).b = 0;
	}
	UINT num_IR = IR.afficher();
	changer_titre_fenetre_graphique(num_IR, "Composante R");

	// attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();

	return 0;
}
