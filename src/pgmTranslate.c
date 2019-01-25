#include "pgmTranslate.h"
#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

static const int MAX_HEIGHT = 255;
static const int MIN_AIRSPACE = 30;
static const int HEIGHTMAP_CUBE = 1;
static const char COMMENT = '#';
static bool headerProcessed = false;

extern GLubyte world[WORLDX][WORLDY][WORLDZ];

int convertToNum(const char *line) {
   char *end = NULL;
   long converted = strtol(line, &end, 10);

   if (converted == LONG_MAX || converted == LONG_MIN) {
      perror("Could not read height map.");
      exit(EXIT_FAILURE);
   }
   return end == line ? -1 : converted;
}

bool isComment(const char *line) {
   return line[0] == COMMENT;
}

void format(char *line) {
   line[strcspn(line, "\r\n")] = 0;
}

int lerpHeight(const int initialHeight) {
   return (abs(initialHeight) / (float) MAX_HEIGHT) * (WORLDY - MIN_AIRSPACE);
}

void fillYAxisAtCoord(const int x, int y, const int z) {
   for (; y >= 0; y--)
      world[x][y][z] = HEIGHTMAP_CUBE;
}

void processElevations(const char *elevations, int *x, int *z) {
   char *elevationsCp = strdup(elevations);
   char *token = strtok(elevationsCp, " ");
   int height = 0;

   while (token != NULL) {
      height = convertToNum(token);
      if (!headerProcessed && height == MAX_HEIGHT) {
         headerProcessed = true;
         continue;
      }
      if (*z >= WORLDZ) {
         *x = *x + 1;
         *z = 0;
      }
      if (*x >= WORLDX)
         return;
      fillYAxisAtCoord(*x, lerpHeight(height), *z);
      *z = *z + 1;
      token = strtok(NULL, " ");
   }
}

FILE *open(const char *filename) {
   FILE *fp = fopen(filename, "r");
   if (!fp) {
      perror("Could not open file in read mode.");
      exit(EXIT_FAILURE);
   }
   return fp;
}

void buildHeightmapFrom(const char *imageFile) {
   char *elevations = NULL;
   size_t len = 0;
   int x = 0, z = 0;
   FILE *imageFp = open(imageFile);

   while (getline(&elevations, &len, imageFp) != -1) {
      if (isComment(elevations)) continue;
      format(elevations);
      processElevations(elevations, &x, &z);
   }
   fclose(imageFp);
   free(elevations);
}
