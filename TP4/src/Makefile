#############################################################################
# Fichier Makefile - Master 1 MAI -  Image
# N. SZAFRAN - UFR-IM2AG - 2016
#############################################################################


#==================================#
# definition des variables locales #
#----------------------------------#

# compilateur C++
CPP = g++

# directives de compilation
CFLAGS = -Wall -g

# librairies OpenGL pour compiler en C++
LIBS_CPP = -lglut -lGLU -lGL 

# liste des executables
EXECUTABLES = simple_rectangle simple_cercle anneau_circulaire \
 matrices_opengl pantins cubes3d damier1 damier2 petit_train ombrage_face \
 texture1 texture2 texture_couleur

#======================================================#
# règle de compilation d'un programme utilisant OpenGL #
#------------------------------------------------------#

# prog : prog.cpp
# 	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@


simple_rectangle: simple_rectangle.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

simple_cercle: simple_cercle.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

anneau_circulaire: anneau_circulaire.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

matrices_opengl: matrices_opengl.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

pantins: pantins.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

cubes3d: cubes3d.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

damier1: damier1.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

damier2: damier2.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

petit_train: petit_train.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

ombrage_face : ombrage_face.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

texture1: texture1.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

texture2: texture2.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

texture_couleur: texture_couleur.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@


#=====================================#
# regle pour "nettoyer" le répertoire #
#-------------------------------------#

clean:
	rm -f $(EXECUTABLES) *.o

all: $(EXECUTABLES)

