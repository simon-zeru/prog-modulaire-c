#ifndef TRAITEMENT_H_
#define TRAITEMENT_H_

#define DEBUG 0

#if DEBUG
    #include <stdio.h>
    #define AFFICHER_FONCTION() printf("Function: %s\n", __func__)
#else
    #define AFFICHER_FONCTION() 
#endif

#endif 