###############################################################################
#                                                                             #
#             LICENCE 3 - Informatique - Option Image - 2014/2015             #
#                                                                             #
#                               Makefile - TP1                                #
#                                                                             #
###############################################################################

CPP = g++

COMPILOPTS = -g

LIBLINK = -L. -llib_image -L/usr/lib/i386-linux-gnu -lpthread -lX11

EXECUTABLES = image_gris1 image_couleur1 image_composante_rgb image_indexee1 \
 image_fichier image_gris2 image_couleur2 image_rgb_hsv \
 image_trace_segment

#====================================#
# création de la librairie lib_image #
#------------------------------------#
lib_image :
	$(CPP) -c lib_image.cpp
	ar -q liblib_image.a lib_image.o

#=========================================================#
# règle de compilation d'un programme utilisant lib_image #
#---------------------------------------------------------#
image_gris1 : image_gris1.cpp
	$(CPP) $(COMPILOPTS) $< $(LIBLINK) -o $@

image_couleur1 : image_couleur1.cpp
	$(CPP) $(COMPILOPTS) $< $(LIBLINK) -o $@

image_composante_rgb : image_composante_rgb.cpp
	$(CPP) $(COMPILOPTS) $< $(LIBLINK) -o $@

image_indexee1 : image_indexee1.cpp
	$(CPP) $(COMPILOPTS) $< $(LIBLINK) -o $@

image_fichier : image_fichier.cpp
	$(CPP) $(COMPILOPTS) $< $(LIBLINK) -o $@

image_gris2 : image_gris2.cpp
	$(CPP) $(COMPILOPTS) $< $(LIBLINK) -o $@

image_couleur2 : image_couleur2.cpp
	$(CPP) $(COMPILOPTS) $< $(LIBLINK) -o $@

image_rgb_hsv : image_rgb_hsv.cpp
	$(CPP) $(COMPILOPTS) $< $(LIBLINK) -o $@

image_trace_segment : image_trace_segment.cpp
	$(CPP) $(COMPILOPTS) $< $(LIBLINK) -o $@

#--------------------------------------------------------------
# autres commandes

all_exec : $(EXECUTABLES)

clean :
	rm -rf $(EXECUTABLES) *.o

