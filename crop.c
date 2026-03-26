#include "crop.h"

//  Category Tree 

static CategoryNode* createCategoryNode(const char *name) {
    CategoryNode *node = malloc(sizeof(CategoryNode));
    strcpy(node->name, name);
    node->cropRoot = NULL;
    node->left = node->right = NULL;
    return node;
}

CategoryNode* insertCategory(CategoryNode *root, const char *name) {
    if (!root) return createCategoryNode(name);
    int cmp = strcmp(name, root->name);
    if (cmp < 0) root->left = insertCategory(root->left, name);
    else if (cmp > 0) root->right = insertCategory(root->right, name);
    return root;
}

CategoryNode* findCategory(CategoryNode *root, const char *name) {
    if (!root) return NULL;
    int cmp = strcmp(name, root->name);
    if (cmp == 0) return root;
    return cmp < 0 ? findCategory(root->left, name) : findCategory(root->right, name);
}

//  Crop Tree 

static CropNode* createCropNode(const char *name) {
    CropNode *node = malloc(sizeof(CropNode));
    strcpy(node->name, name);
    node->prices = NULL;
    node->left = node->right = NULL;
    return node;
}

CropNode* insertCrop(CropNode *root, const char *name) {
    if (!root) return createCropNode(name);
    int cmp = strcmp(name, root->name);
    if (cmp < 0) root->left = insertCrop(root->left, name);
    else if (cmp > 0) root->right = insertCrop(root->right, name);
    return root;
}

CropNode* findCrop(CropNode *root, const char *name) {
    if (!root) return NULL;
    int cmp = strcmp(name, root->name);
    if (cmp == 0) return root;
    return cmp < 0 ? findCrop(root->left, name) : findCrop(root->right, name);
}

//  Prices 

PriceNode* insertPrice(PriceNode *head, float price) {
    PriceNode *newNode = malloc(sizeof(PriceNode));
    newNode->price = price;
    newNode->next = NULL;

    if (!head) return newNode;

    PriceNode *temp = head;
    while (temp->next) temp = temp->next;
    temp->next = newNode;
    return head;
}

//  Core Functions 

void addCropToCategory(CategoryNode *catRoot, const char *category, const char *crop) {
    CategoryNode *cat = findCategory(catRoot, category);
    if (!cat) {
        printf("Category '%s' not found.\n", category);
        return;
    }
    cat->cropRoot = insertCrop(cat->cropRoot, crop);
    printf("Added crop '%s' under category '%s'.\n", crop, category);
}

void addPrice(CategoryNode *catRoot, const char *category, const char *crop, float price) {
    CategoryNode *cat = findCategory(catRoot, category);
    if (!cat) {
        printf("Category '%s' not found.\n", category);
        return;
    }
    CropNode *cr = findCrop(cat->cropRoot, crop);
    if (!cr) {
        printf("Crop '%s' not found in '%s'.\n", crop, category);
        return;
    }
    cr->prices = insertPrice(cr->prices, price);
}

void displayCropAverage(CategoryNode *catRoot, const char *category, const char *crop) {
    CategoryNode *cat = findCategory(catRoot, category);
    if (!cat) return (void)printf("Category not found.\n");

    CropNode *cr = findCrop(cat->cropRoot, crop);
    if (!cr) return (void)printf("Crop not found.\n");

    float sum = 0, count = 0;
    for (PriceNode *p = cr->prices; p; p = p->next)
        sum += p->price, count++;

    if (!count) printf("No prices for %s yet.\n", crop);
    else printf("Average price of %s (%s): %.2f\n", crop, category, sum / count);
}

void displayCropMaxMin(CategoryNode *catRoot, const char *category, const char *crop) {
    CategoryNode *cat = findCategory(catRoot, category);
    if (!cat) return (void)printf("Category not found.\n");

    CropNode *cr = findCrop(cat->cropRoot, crop);
    if (!cr) return (void)printf("Crop not found.\n");
    if (!cr->prices) return (void)printf("No prices recorded for %s.\n", crop);

    float max = cr->prices->price, min = cr->prices->price;
    for (PriceNode *p = cr->prices; p; p = p->next) {
        if (p->price > max) max = p->price;
        if (p->price < min) min = p->price;
    }
    printf("%s (%s): Max = %.2f | Min = %.2f\n", crop, category, max, min);
}

// Helper to display all crops in a category
static void showCrops(CropNode *root) {
    if (!root) return;
    showCrops(root->left);
    printf("Crop: %s\n", root->name);
    showCrops(root->right);
}

// Display all categories and crops
void displayAll(CategoryNode *catRoot) {
    if (!catRoot) return;

    displayAll(catRoot->left);

    printf("\nCategory: %s\n", catRoot->name);
    printf("-\n");
    showCrops(catRoot->cropRoot);

    displayAll(catRoot->right);
}


//  Memory Cleanup 

// Helper to free crop tree
static void freeCrops(CropNode *root) {
    if (!root) return;
    freeCrops(root->left);
    freeCrops(root->right);

    for (PriceNode *p = root->prices; p;) {
        PriceNode *tmp = p;
        p = p->next;
        free(tmp);
    }
    free(root);
}

// Free entire category tree
void freeAll(CategoryNode *catRoot) {
    if (!catRoot) return;

    freeAll(catRoot->left);
    freeAll(catRoot->right);

    freeCrops(catRoot->cropRoot);
    free(catRoot);
}