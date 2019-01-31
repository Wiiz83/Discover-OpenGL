###############################################################################
#                                                                             #
#             LICENCE 3 - Informatique - Option Image - 2014/2015             #
#                                                                             #
#                               Makefile - TP2                                #
#                                                                             #
###############################################################################

CPP = g++

COMPILOPTS = -g

LIBLINK = -L. -llib_image -L/usr/lib/i386-linux-gnu -lpthread -lX11

EXECUTABLES = histogramme1 fonction1 agrandissement1 filtre_lineaire1 ex1

#====================================#
# création de la librairie lib_image #
#------------------------------------#
lib_image : lib_image.cpp lib_image.hpp CImg.h
	$(CPP) -c lib_image.cpp
	ar -q liblib_image.a lib_image.o

#=========================================================#
# règle de compilation d'un programme utilisant lib_image #
#---------------------------------------------------------#
histogramme1 : histogramme1.cpp 
	$(CPP) $(COMPILOPTS) $<  $(LIBLINK) -o $@

fonction1 : fonction1.cpp 
	$(CPP) $(COMPILOPTS) $<  $(LIBLINK) -o $@

agrandissement1 : agrandissement1.cpp 
	$(CPP) $(COMPILOPTS) $<  $(LIBLINK) -o $@

filtre_lineaire1 : filtre_lineaire1.cpp 
	$(CPP) $(COMPILOPTS) $<  $(LIBLINK) -o $@


ex1 : ex1.cpp 
	$(CPP) $(COMPILOPTS) $<  $(LIBLINK) -o $@


#--------------------------------------------------------------
# autres commandes

clean :
	rm -rf $(EXECUTABLES) *.o

