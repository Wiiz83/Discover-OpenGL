#############################################################################
# Fichier Makefile - Licence 3 Informatique - Option Image
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
EXECUTABLES = courbes_isovaleur surfaces_isovaleur

#======================================================#
# règle de compilation d'un programme utilisant OpenGL #
#------------------------------------------------------#

# prog : prog.cpp
# 	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@


courbes_isovaleur: courbes_isovaleur.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@

surfaces_isovaleur: surfaces_isovaleur.cpp
	$(CPP) $(CFLAGS) $< $(LIBS_CPP) -o $@


#=====================================#
# regle pour "nettoyer" le répertoire #
#-------------------------------------#

clean:
	rm -f $(EXECUTABLES) *.o

