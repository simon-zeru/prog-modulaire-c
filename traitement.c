#include <pgmio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "traitement.h"
#include <time.h>
#define SEUIL1_PIXEL(valeur, seuil) (valeur < seuil ? 0 : 255)
#define SEUIL2_PIXEL(valeur, seuil) (valeur = (valeur < seuil) ? 0 : 255)


void pgm_show(char *filename) {
    AFFICHER_FONCTION();
    size_t size = strlen(filename) + strlen("eog") + 2;
    char *ptr = (char*)malloc(size);
    snprintf(ptr, size, "eog %s", filename);
    if (ptr == NULL) {
        return;
    }
    system(ptr);
    free(ptr);
}

pgm_t_image *pgm_malloc(size_t width, size_t height) {
    AFFICHER_FONCTION();
    size_t size = width * height;
    pgm_t_pixel *vPixel = (pgm_t_pixel *)malloc(size);
    if (vPixel == NULL) {
        return NULL;
    } 

    pgm_t_image *p1 = (pgm_t_image *)malloc(sizeof(pgm_t_image));
    
    if (p1 == NULL) {
        return NULL;
    }
    p1->pixels = vPixel;
    p1->width = width;
    p1->height = height;
    return p1;
}

void pgm_free(pgm_t_image *image) {
    AFFICHER_FONCTION();
    free(image->pixels);
    free(image);
}

void pgm_solid(pgm_t_image *image, pgm_t_pixel color) {
    AFFICHER_FONCTION();
    for (int i = 0; i < (image->width * image->height); i++) {
        image->pixels[i] = color;
    }
}


void pgm_negative(pgm_t_image *image) {
    AFFICHER_FONCTION();
    for (int i = 0; i < (image->width * image->height); i++) {
        image->pixels[i] = 255 - image->pixels[i];
    }
}



void pgm_threshold(pgm_t_image *image, pgm_t_pixel threshold) {
    AFFICHER_FONCTION();
    if (threshold < 0 || threshold > 255) {
        perror("threshold out of range");
    } else {
        for (int i = 0; i < (image->width * image->height); i++) {
            SEUIL2_PIXEL(image->pixels[i], threshold);
        }
    }

}


void pgm_gradient(pgm_t_image *image, pgm_t_pixel from, pgm_t_pixel to) {
    AFFICHER_FONCTION();
    if (from < 0 || from > 255 || to < 0 || to > 255) {
        perror("from or to out of range");
    } else {
        for (size_t i = 0; i < image->height; i++) {
            for (size_t j = 0; j < image->width; j++) {
                image->pixels[i * image->width + j] = from + (to - from) * j / image->width;
            }
        }
    }
}



int main(void) {
    float timeBefore = clock();
    for(int i = 0; i < 10; i++) {
        pgm_t_image *p1 = pgm_read("guadalest.pgm");
        pgm_negative(p1);
        pgm_write("guadalest-negative.pgm", p1);
        pgm_threshold(p1, 200);
        pgm_write("guadalest-negative-threshold.pgm", p1);
        pgm_gradient(p1, 128, 175);
        pgm_write("guadalest-gradient.pgm", p1);
        pgm_free(p1);
        pgm_show("guadalest-gradient.pgm");
        pgm_show("guadalest-negative.pgm");
        pgm_show("guadalest-negative-threshold.pgm");
        pgm_show("guadalest.pgm");
    }
    float timeAfter = clock();
    float duration = (timeAfter - timeBefore) / CLOCKS_PER_SEC;

    printf("Durée totale : %f\n", duration); 
    // vérification de fuite mémoire : valgrind --leak-check=full ./traitement-image

    return EXIT_SUCCESS;
}