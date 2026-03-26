#ifndef CROP_H
#define CROP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Linked list for daily prices
typedef struct PriceNode {
    float price;
    struct PriceNode *next;
} PriceNode;

// Crop tree node
typedef struct CropNode {
    char name[50];
    PriceNode *prices;
    struct CropNode *left, *right;
} CropNode;

// Category tree node
typedef struct CategoryNode {
    char name[50];
    CropNode *cropRoot;
    struct CategoryNode *left, *right;
} CategoryNode;

// Category functions
CategoryNode* insertCategory(CategoryNode *root, const char *name);
CategoryNode* findCategory(CategoryNode *root, const char *name);

// Crop functions
CropNode* insertCrop(CropNode *root, const char *name);
CropNode* findCrop(CropNode *root, const char *name);

// Price functions
PriceNode* insertPrice(PriceNode *head, float price);

// Core features
void addCropToCategory(CategoryNode *catRoot, const char *category, const char *crop);
void addPrice(CategoryNode *catRoot, const char *category, const char *crop, float price);
void displayCropAverage(CategoryNode *catRoot, const char *category, const char *crop);
void displayCropMaxMin(CategoryNode *catRoot, const char *category, const char *crop);
void displayAll(CategoryNode *catRoot);

// Cleanup
void freeAll(CategoryNode *catRoot);

#endif
