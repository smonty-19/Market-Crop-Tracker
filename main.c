#include "crop.h"

int main() {
    CategoryNode *catRoot = NULL;
    int choice;
    char category[50], crop[50];
    float price;

    while (1) {
        printf("\n--- Farmer's Market Tracker ---\n");
        printf("1. Add Category\n");
        printf("2. Add Crop to Category\n");
        printf("3. Add Daily Price\n");
        printf("4. Show Average Price\n");
        printf("5. Show Max/Min Price\n");
        printf("6. View All Categories & Crops\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter category name: ");
                scanf("%s", category);
                catRoot = insertCategory(catRoot, category);
                break;

            case 2:
                printf("Enter category name: ");
                scanf("%s", category);
                printf("Enter crop name: ");
                scanf("%s", crop);
                addCropToCategory(catRoot, category, crop);
                break;

            case 3:
                printf("Enter category: ");
                scanf("%s", category);
                printf("Enter crop: ");
                scanf("%s", crop);
                printf("Enter price: ");
                scanf("%f", &price);
                addPrice(catRoot, category, crop, price);
                break;

            case 4:
                printf("Enter category: ");
                scanf("%s", category);
                printf("Enter crop: ");
                scanf("%s", crop);
                displayCropAverage(catRoot, category, crop);
                break;

            case 5:
                printf("Enter category: ");
                scanf("%s", category);
                printf("Enter crop: ");
                scanf("%s", crop);
                displayCropMaxMin(catRoot, category, crop);
                break;

            case 6:
                displayAll(catRoot);
                break;

            case 7:
                freeAll(catRoot);
                printf("Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }
}
