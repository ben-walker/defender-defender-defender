#include "pgmTranslate.h"
#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

extern GLubyte world[WORLDX][WORLDY][WORLDZ];

int convertToNum(const char *line) {
   char *end = NULL;
   long converted = strtol(line, &end, 10);

   if (converted == LONG_MAX || converted == LONG_MIN) {
      fprintf(stderr, "Could not read pgm height map.");
      exit(EXIT_FAILURE);
   }
   if (end == line) return -1;
   return converted;
}

bool isComment(const char *line) {
   return line[0] == '#';
}

void formatLine(char *line) {
   line[strcspn(line, "\r\n")] = 0;
}

int lerpHeight(const int initialHeight) {
   int height = (initialHeight / (float) UCHAR_MAX) * (WORLDY - 30);
   if (height < 0) height = 0;
   else if (height > WORLDY - 1) height = WORLDY - 1;
   return height;
}

void fillYAxis(const int x, int y, const int z) {
   for (; y >= 0; y--) world[x][y][z] = 1;
}

void addHeightsToWorld(const char *line, int *x, int *z) {
   char *copy = strdup(line);
   int height;

   if (*x == WORLDX - 1 && *z == WORLDZ - 1) return;

   char *token = strtok(copy, " ");
   while (token != NULL) {
      height = lerpHeight(convertToNum(token));
      if (*z == WORLDZ - 1) {
         *x = *x + 1;
         *z = 0;
      }
      fillYAxis(*x, height, *z);
      *z = *z + 1;
      token = strtok(NULL, " ");
   }
}

void buildTerrainFromPgm(const char *pgmFile) {
   char *line = NULL;
   size_t len = 0;
   int x = 0, z = 0;
   bool foundStart = false;
   FILE *pgm = fopen(pgmFile, "r");

   if (!pgm) {
      fprintf(stderr, "Could not open .pgm in read mode.");
      exit(EXIT_FAILURE);
   }

   while (getline(&line, &len, pgm) != -1) {
      if (isComment(line)) continue;
      formatLine(line);
      if (foundStart)
         addHeightsToWorld(line, &x, &z);
      else if (strcmp(line, "255") == 0)
         foundStart = true;
   }
   fclose(pgm);
   free(line);
}
