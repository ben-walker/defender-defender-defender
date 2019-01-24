#include "pgmTranslate.h"
#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

static const int MAX_DEPTH = 255;
static const int MIN_AIRSPACE = 30;
static const int HEIGHTMAP_CUBE = 1;
static const char COMMENT = '#';

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
   return (abs(initialHeight) / (float) MAX_DEPTH) * (WORLDY - MIN_AIRSPACE);
}

void fillYAxisAtCoord(const int x, int y, const int z) {
   for (; y >= 0; y--)
      world[x][y][z] = HEIGHTMAP_CUBE;
}

void addToHeightmap(const char *elevations, int *x, int *z) {
   char *elevationsCp = strdup(elevations);
   char *token = strtok(elevationsCp, " ");

   while (token != NULL) {
      if (*z >= WORLDZ) {
         *x = *x + 1;
         *z = 0;
      }
      if (*x >= WORLDX)
         return;
      fillYAxisAtCoord(*x, lerpHeight(convertToNum(token)), *z);
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
   bool headerProcessed = false;
   FILE *imageFp = open(imageFile);

   while (getline(&elevations, &len, imageFp) != -1) {
      if (isComment(elevations)) continue;
      format(elevations);
      if (headerProcessed)
         addToHeightmap(elevations, &x, &z);
      else if (convertToNum(elevations) == MAX_DEPTH)
         headerProcessed = true;
   }
   fclose(imageFp);
   free(elevations);
}
