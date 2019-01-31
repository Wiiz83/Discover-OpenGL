//////////////////////////////////////////////////////////////////////////////
// Transformée de Hough - détection de droites
// 
// N. SZAFRAN - UFR-IM2AG - 2016
//////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//                    Opérations diverses sur des images                     //
///////////////////////////////////////////////////////////////////////////////

// conversion d'une ImageGrisF en ImageCouleurF
ImageCouleurF gris2rgb(ImageGrisF &I)
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	ImageCouleurF IC(L,H);
	for (UINT i=0; i<L*H; i++)
	{
		float v = I(i);
		IC(i) = RGBF(v,v,v);
	}
	return IC;
}

// tracer le pixel (x,y) dans l'image I avec la couleur c et l'épaisseur e
template <typename Image, typename Couleur>
void dessiner_pixel(Image &I, int x, int y, Couleur c, int e)
{
	for (int xe = x-e+1; xe<=x+e-1; xe++) 
	for (int ye = y-e+1; ye<=y+e-1; ye++) 
		I.pixel(xe,ye,c); 
}


// tracer le segment du pixel (x1,y1) au pixel (x2,y2)
// dans l'image I avec la couleur c et l'épaisseur e
template <typename Image, typename Couleur>
void dessiner_segment(Image &I, int x1, int y1, int x2, int y2, Couleur c,
int e)
{
// printf("dessiner_segment (%d,%d)-(%d,%d)\n", x1,y1,x2,y2);
	int sx, sy, dx, dy;
	if (x1<=x2)
	{
		sx = 1; dx = x2-x1;
	}
	else
	{
		sx = -1; dx = x1-x2;
	}
	if (y1<=y2)
	{
		sy = 1; dy = y2-y1;
	}
	else
	{
		sy = -1; dy = y1-y2;
	}
	
	if (dx>=dy)
	{	// boucle sur x
	
		// initialisation
		int x = x1;
		int y = y1;
		int fP = 0;
		int dfM  =   dx-2*dy;
		int dfPcas1 = 2*dx-2*dy;
		int dfPcas2 =     -2*dy;

		while (x != x2)
		{
			// dessiner le pixel (x,y) avec la couleur c et l'épaisseur e
			dessiner_pixel(I, x, y, c, e);
			
			// passer au pixel suivant
			if (fP+dfM < 0)
			{
				y += sy;
				fP += dfPcas1;
			}
			else
			{
				fP += dfPcas2;
			}
			x += sx;
		}
		// dessiner le pixel (x,y) avec la couleur c et l'épaisseur e
		dessiner_pixel(I, x, y, c, e);
	}
	else
	{	// boucle sur y
	
		// initialisation
		int x = x1;
		int y = y1;
		int fP = 0;
		int dfM  =   dy-2*dx;
		int dfPcas1 = 2*dy-2*dx;
		int dfPcas2 =     -2*dx;
		
		while (y != y2)
		{
			// dessiner le pixel (x,y) avec la couleur c et l'épaisseur e
			dessiner_pixel(I, x, y, c, e);
			
			// passer au pixel suivant
			if (fP+dfM < 0)
			{
				x += sx;
				fP += dfPcas1;
			}
			else
			{
				fP += dfPcas2;
			}
			y += sy;
		}
		// dessiner le pixel (x,y) avec la couleur c et l'épaisseur e
		dessiner_pixel(I, x, y, c, e);
	}
	
}

// tracer la droite avec la couleur coul et l'épaisseur e
//   définie par (theta, alpha) dans l'image IC
// la droite est -sin(theta)*(x-L/2)+cos(theta)*(y-H/2)=alpha
void tracer_droite
  (ImageCouleurF &IC, float theta, float alpha, RGBF coul, int e=1)
{
	float L = IC.largeur();
	float H = IC.hauteur();
	float D = sqrt(L*L+H*H);
	float c = cos(theta);
	float s = sin(theta);
	alpha += 0.5*(H*c-L*s);
	float x0 = -alpha*s;
	float y0 =  alpha*c;
	float x1 = x0-D*c;
	float y1 = y0-D*s;
	float x2 = x0+D*c;
	float y2 = y0+D*s;
	dessiner_segment(IC, (int)x1, (int)y1, (int)x2, (int)y2, coul, e);
}

// normaliser les valeurs de l'image I pour les ramener entre 0.0 et 1.0
void normaliser(ImageGrisF &I)
{
	float min, max, d;
	
	//  calcul des valeurs min et max de I
	min = max = I(0);
	for (UINT i=1; i<I.nb_pixels(); i++)
	{
		if (min > I(i)) min = I(i);
		if (max < I(i)) max = I(i);
	}
	
	// normalisation
	d = max-min;
	for (UINT i=0; i<I.nb_pixels(); i++)
		I(i) = d>0.0 ? (I(i)-min)/d : 0.0;
	
}


///////////////////////////////////////////////////////////////////////////////
// calcul de la transformee de Hough pour la détection de droite
// de l'image I
// Entrée : I = image en niveau de gris (valeur réelle)
//          M, N = dimensions de l'image Transformée de Hough
// La fonction renvoie une image de dimensions M x N
// le pixel d'indice (X,Y) de cette image correspond à :
// -- la valeur theta = theta_min + X*(theta_max-theta_min)/M
// -- la valeur alpha = alpha_min + Y*(alpha_max-alpha_min)/N
// avec : 
//  theta_min = 0        theta_max = PI
//  alpha_min = -D/2     alpha_max = D/2
ImageGrisF transformee_hough(ImageGrisF &I, UINT M, UINT N)
{
    // dimensions de l'image I
    UINT L = I.largeur();
    UINT H = I.hauteur();
    
    // D : longueur de la diagonale de l'image I
    float D = sqrt((float)L*(float)L+(float)H*(float)H);
    
    // création de l'image de la transformée de Hough
    // de dimensions N x N et tous les pixels avec la valeur 0.0
    ImageGrisF TH(M,N,0.0);
    
    // parcours de l'image I et traitement des pixels blancs (égaux à 1)
    for (int X1 = 0; X1 < L; X1++)
    for (int Y1 = 0; Y1 < H; Y1++)
        if (I(X1,Y1)==1.0)
        {
            float x = X1-0.5*L;
            float y = Y1-0.5*H;

            for (int X2 = 0; X2 < M; ++X2)
            {
                float theta = (X2*M_PI)/M;
                float alpha = -x*sin(theta)+y*cos(theta);
                int Y2 = floor(N*(alpha+D/2)/D);
                TH(X2,Y2) = TH(X2,Y2)+1;
            }
        }

    normaliser(TH);
    return TH;
}



///////////////////////////////////////////////////////////////////////////////
//           outils pour la recherche de maxima locaux                       //
//           dans l'image de la transformée de Hough                         //
///////////////////////////////////////////////////////////////////////////////

// structure pour un pixel
struct pixel_valeur {
	UINT x,y; // coordonnées du pixel
	float v;  // valeur du pixel
};

// fonction de comparaison de deux structures pixel_valeur
// afin de pouvoir trier une séquence de pixel_valeur
bool comp_pixel_valeur(pixel_valeur p1, pixel_valeur p2)
{
	return p1.v>p2.v;
}

// recherche les maxima locaux de l'image TH
// Entree : TH = image "transformée de Hough" à valeurs réelles
//          s = valeur seuil
//          d = voisinage pour le calcul du max (par défaut d=1)
// La fonction renvoie la séquence des maxima sous forme d'un vector
//   de type pixel_valeur, triée par ordre décroissant
// Ne sont pris en compte que les pixels de TH qui sont des maxima locaux
//   et dont la valeur v est supérieure au seuil s
// La distance d est utilisée pour le voisinage du maximum local
// (x,y) est un maximum local de l'image TH si les deux conditions suivantes 
//   sont vérifiées :
//   (1) TH(x,y) >= s
//   (2) pour tout k et l entre -d et d, TH(x,y) >= TH(x+k,y+l)
vector<pixel_valeur> sequence_maxima_locaux(ImageGrisF &TH, float s, int d)
{
	vector<pixel_valeur> V;
	
	// retourner une liste vide si d<1
	if (d<1) return V;
	
	int LT = TH.largeur();
	int HT = TH.hauteur();
	for (int x=0; x<LT; x++)
	for (int y=0; y<HT; y++)
	{
		float v = TH(x,y);
		if (v>=s)
		{ 
			// le pixel (x,y) a une valeur supérieure au seuil
			// tester si ce pixel est un maximum local
			// c'est à dire si
			//  pour tout k et l entre -d et d, TH(x,y) >= TH(x+k,y+l)
			bool est_maxi = true;
			for (int dx=-d; dx<=d; dx++)
			for (int dy=-d; dy<=d; dy++)
			{
				int x1 = x+dx;
				int y1 = y+dy;
				
				// gestion du bord
				if (x1<0)
				{
					x1 = x1+LT; y1 = HT-y1;
				}
				if (x1>=LT)
				{
					x1 = x1-LT; y1 = HT-y1;
				}
				
				if (x1>=0 && x1<LT && y1>=0 && y1<HT)
				{
					if (v<TH(x1,y1)) est_maxi = false;
				}
			}
		
			if (est_maxi)
			{
				// ajouter le pixel (x,y) à la liste v
				pixel_valeur pv = {x,y,v};
				V.push_back(pv); // ajouter en fin de séquence 
			}
		}
	}
	
	// tri de V par valeurs v décroissantes
	sort(V.begin(), V.end(), comp_pixel_valeur);
	return V;
}

// affiche la séquence de pixel_valeur V
void print(vector<pixel_valeur> &V)
{
	for (int i=0; i<V.size(); i++)
	{
		pixel_valeur pv = V[i];
		printf(" pixel(%4ld,%4ld) : valeur = %f\n", pv.x, pv.y, pv.v);
	}
}

///////////////////////////////////////////////////////////////////////////////
//                          création d'images tests                          //
///////////////////////////////////////////////////////////////////////////////

// image contenant 1 seul pixel blanc
ImageGrisF image_test_1()
{
	ImageGrisF I(100,100,0.0);
	
	// dessiner dans l'image I le pixel (10,30)
	// avec la couleur 1.0 (blanc) et l'épaisseur 1
	// dessiner_pixel(I, 10,30 , 1.0 , 1); 
	dessiner_pixel(I, 10, 10 , 1.0 , 1);
	dessiner_pixel(I, 20, 20 , 1.0 , 1);
	dessiner_pixel(I, 30, 30 , 1.0 , 1);
	dessiner_pixel(I, 40, 40 , 1.0 , 1);
	dessiner_pixel(I, 50, 50 , 1.0 , 1);

	
	return I;
}

// image contenant un segment 
ImageGrisF image_test_2()
{
	ImageGrisF I(100,100,0.0);
	
	// dessiner dans l'image I le segment (5,20)-(95,60)
	// avec la couleur 1.0 (blanc) et l'épaisseur 1
	dessiner_segment(I, 20,40 , 85,10 , 1.0 , 1);
	dessiner_segment(I, 85,10 , 10,65 , 1.0 , 1);
	dessiner_segment(I, 10,65 , 20,40 , 1.0 , 1);
	
	return I;
}


///////////////////////////////////////////////////////////////////////////////
//                          le programme principal                           //
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	// booleen pour faire ou non la partie 2
	bool partieB=true;
	
	// créer une image test
	ImageGrisF I;
	if (argc<2)
		I = image_test_2();
	else
		I = ImageGrisF(argv[1]);


	//I.afficher();
	
	UINT L = I.largeur();    // largeur de l'image
	UINT H = I.hauteur();    // hauteur de l'image
	float D = sqrt(pow(L,2)+pow(H,2)); // diagonale de l'image

	// calculer l'image TH de la transformée de Hough de l'image I
	ImageGrisF TH;
	UINT M=360; // discrétisation de la transformée de Hough pour theta
	UINT N=400; // discrétisation de la transformée de Hough pour alpha
	TH = transformee_hough(I,M,N);
	
	// normaliser l'image TH et l'afficher
	normaliser(TH);
	//TH.afficher();
	
	
	if (partieB)
	{
	// -----------------------------------------------------------------------
	// . détermination des maxima locaux de TH
	// . affichage des maxima locaux de TH
	// . affichage des droites correspondantes sur l'image I
	
	// calculer la sequence de maxima locaux
	//float seuil = 1.0/3.0;

	float seuil = 1.0/1.125;

	// d = distance de voisinage
	int d = 1;

	vector<pixel_valeur> V = sequence_maxima_locaux(TH, seuil, d);
	printf("Séquence des maxima local (taille = %ld) :\n", V.size());
	print(V);
	
	// representer les maxima locaux sur l'image TH comme des points rouges
	// faire une copie de TH --> image couleur THC
	ImageCouleurF THC;
	THC = gris2rgb(TH);
	RGBF Rouge(1.0,0.0,0.0);
	UINT nb_max_admis = 10;
	for (UINT i=0; i<V.size() && i<nb_max_admis; i++)
	{
		pixel_valeur pv = V[i];
		dessiner_pixel(THC, pv.x, pv.y, Rouge, 3);
	}
	THC.afficher();
	
	// representer les droites correspondant aux maxima locaux sur l'image I 
	// faire une copie de I --> image couleur IC
	ImageCouleurF IC;
	IC = gris2rgb(I);
	for (UINT i=0; i<V.size() && i<nb_max_admis; i++)
	{
		pixel_valeur pv = V[i];
		
		// trouver les valeurs (theta,alpha) correspondant à (pv.x,pv.y)
		float theta = M_PI*pv.x/M;
		float alpha = -0.5*D+pv.y*D/N;
		tracer_droite(IC, theta, alpha, Rouge, 2);
	}
	IC.afficher();
	
	} // fin cas partieB = VRAI
		
	// en attente de taper une touche
	fprintf(stderr, "Taper un caractere au clavier pour continuer");
	getchar();

	return 0;
}
