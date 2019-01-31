///////////////////////////////////////////////////////////////////////////////
// utilisation de fonctions
///////////////////////////////////////////////////////////////////////////////

#include "lib_image.hpp"
#include <cstdio>
#include <cstdlib>
#include <cmath>

// couleurs
RGBF noir(0.0,0.0,0.0);
RGBF gris50(0.5,0.5,0.5);
RGBF rouge(1.0,0.0,0.0);
RGBF blanc(1.0,1.0,1.0);
RGBF bleu(0.0,0.0,1.0);
RGBF vert(0.0,1.0,0.0);
RGBF jaune10(1.0,1.0,0.9);

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

// conversion d'une valeur RGBF en niveau de gris
float gris(RGBF p)
{
	return 0.2125*p.r + 0.7154*p.g + 0.0721*p.b;
}

//////////////////////////////////////////////////////////////////////////////
// classe FiltreLineaire :
//   définition de filtres linéaires
//   trois types de filtres prédéfinis :
//     FiltreLineaire::Moyenne 
//     FiltreLineaire::Chapeau 
//     FiltreLineaire::Gaussien
//////////////////////////////////////////////////////////////////////////////
class FiltreLineaire
{
	int n; // taille du filtre --> dimension (2n+1) * (2n+1)
	float *T; // tableau contenant les valeurs du filtre
	
	// creer un filtre moyenne de taille n
	void filtre_moyenne()
	{
		UINT p=0;
		float v = 1.0/(float)((2*n+1)*(2*n+1));
		for (int i=-n; i<=n; i++)
		for (int j=-n; j<=n; j++)
		{
			T[p++] = v;
		}
	}

	// creer un filtre chapeau de taille n
	void filtre_chapeau()
	{
		UINT p=0;
		float v = 1.0/powf((float)(n+1),4.0);
		for (int i=-n; i<=n; i++)
		for (int j=-n; j<=n; j++)
		{
			T[p++] = v*(n+1-fabsf(i))*(n+1-fabsf(j));
		}
	}

	// creer un filtre gaussien de taille n
	void filtre_gaussien()
	{
		UINT p=0;
		float s = 0.0;
		float a = (float)(n*n)/3.0;
		for (int i=-n; i<=n; i++)
		for (int j=-n; j<=n; j++)
		{
			s += T[p++] = exp((float)(-i*i-j*j)/a);
		}
		
		// normalisation
		for (p=0; p<(2*n+1)*(2*n+1); p++)
			T[p] /= s;
	}

public :
	static const int Moyenne  = 1;
	static const int Chapeau  = 2;
	static const int Gaussien = 3;
	
	// crée un filtre de taille 0
	FiltreLineaire()
	{
		n = 0;
		T = new float[1];
		T[0] = 1.0;
	}
	
	// crée un filtre de taille n0 suivant le type t (par défaut Moyenne)
	FiltreLineaire(int n0, int type_filtre = Moyenne)
	{
		if (n0<0) n0=-n0;
		n = n0;
		UINT N = (2*n+1)*(2*n+1);
		T = new float[N];
		switch (type_filtre)
		{
			case Chapeau  : filtre_chapeau();  break;
			case Gaussien : filtre_gaussien(); break;
			default       : filtre_moyenne();  break;
		}
	}
	
	// constructeur par copie
	FiltreLineaire(FiltreLineaire &K)
	{
		n = K.n;
		UINT N = (2*n+1)*(2*n+1);
		T = new float[N];
		for (UINT i=0; i<N; i++)
			T[i] = K.T[i];
	}
	
	// affectation
	FiltreLineaire& operator=(const FiltreLineaire &K)
	{
		if (this != &K)
		{
			delete[] T;
			n = K.n;
			UINT N = (2*n+1)*(2*n+1);
			T = new float[N];
			for (UINT i=0; i<N; i++)
				T[i] = K.T[i];
		}
		return *this;
	}
	
	// destructeur
	~FiltreLineaire()
	{
		delete[] T;
	}
	
	// acces à la valeur d'indice (i,j), -n <= i,j <= n
	float operator()(int i, int j)
	{
		if (-i>n || -j>n || i>n || j>n) return 0.0;
		return T[(i+n)+(2*n+1)*(j+n)];
	}
	
	// renvoie la taille du filtre
	UINT taille() { return n; }

	// renvoie la dimension du filtre
	UINT dimension() { return 2*n+1; }

	// ecrire le filtre à l'ecran
	void print()
	{
		UINT p=0;
		printf("\n");
		for (int i=-n; i<=n; i++)
		{
			for (int j=-n; j<=n; j++)
				printf("%11.8f ", T[p++]);
			printf("\n");
		}
	}
	
	// somme des éleménts du filtre
	float somme()
	{
		UINT N = (2*n+1)*(2*n+1);
		float s = 0.0;
		for (UINT i=0; i<N; i++)
			s += T[i];
		return s;
	}
	
}; // class FiltreLineaire


// calculer l'image résultat de la convolution de 
// l'image I par le filtre linéaire de noyau K
// Entrée : I = image de type ImageCouleurF
//          K = filtre linéaire
// La fonction renvoie une image de meme dimension que I
ImageCouleurF convolution(ImageCouleurF& I, FiltreLineaire K) 
{
	UINT L = I.largeur();
	UINT H = I.hauteur();
	ImageCouleurF R(L,H);
	int n = K.taille();
	
	// PARTIE A COMPLETER
	
	return R;
}

// fonction de comparaison pour le tri d'un tableau de RGBF
int comp_rgb(const void *p1, const void *p2)
{
	RGBF rgb1 = *(float*)p1; float v1 = gris(rgb1);
	RGBF rgb2 = *(float*)p2; float v2 = gris(rgb2);
	if (v1<v2) return -1;
	if (v1>v2) return  1;
	return 0;
}

// fonction triant un tableau de M valeurs de type RGBF suivant leurs valeurs
// en échelle de gris correspondantes
void tri(RGBF *T, UINT M)
{
	qsort((void *)T, M, sizeof(RGBF), comp_rgb);
}

int main(int argc, char *argv[])
{
	
	// charger une image
	ImageCouleurF I("bastille.png");
	I.afficher();
	
	// créer un filtre moyenne de taille n=1 (de dimension 3 x 3)
	FiltreLineaire K(1,  FiltreLineaire::Moyenne);
	
	// appliquer le filtre K à l'image I
	ImageCouleurF R;
	R = convolution(I,K);
	R.afficher();
	
	// en attente de taper une touche
	printf("Taper un caractere au clavier pour continuer");
	getchar();
		
	return 0;
}
