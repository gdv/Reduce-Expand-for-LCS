
#System Library
LIBS=  -lm

#-------------------------------------------------------------------
#Compiler (Here we use a compiler for ANSI C, user may change it.)
#User should make sure the directory path is correct according to
#user's UNIX file system. 
#-------------------------------------------------------------------
CC=gcc -O3


#Final output name of the execution file
TARGETS =       generate approx stats                              

all:            $(TARGETS)

OBJS =          GenerateInstances Approximation CollectResults        


#$(TARGETS):     $(OBJS) 
#		$(CC) -o $(TARGETS)  $(OBJS) $(LIBS)

generate:    GenerateInstances.c  
		$(CC)  GenerateInstances.c -o GenerateInstances

approx:    Approximation.c 
		$(CC)  Approximation.c -o Approximation 

stats:    CollectResults.c 
		$(CC)  CollectResults.c -o CollectResults -lm

clean:
		rm -f *.o core $(OBJ)

###################### END Makefile ###########################
