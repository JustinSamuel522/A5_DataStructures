#include <stdio.h>
#include <stdlib.h>
#include <math.h>  // For sqrt

#define INITIAL_CAPACITY 100

// Define a structure for points
struct Point {
    int x, y;
};

// Define a node for the binary search tree
struct Node {
    int x, y;           // x and y coordinates
    struct Node* left;
    struct Node* right;
};

// Function to create a new node
struct Node* createNode(int x, int y) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->x = x;
    newNode->y = y;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Compare function for sorting points by x-coordinate, then y-coordinate
int comparePoints(const void* a, const void* b) {
    struct Point* pointA = (struct Point*)a;
    struct Point* pointB = (struct Point*)b;

    if (pointA->x == pointB->x) {
        return pointA->y - pointB->y;
    }
    return pointA->x - pointB->x;
}

// Function to build a balanced BST from a sorted array of points
struct Node* buildBalancedBST(struct Point* points, int start, int end) {
    if (start > end) {
        return NULL;
    }

    int mid = (start + end) / 2;
    struct Node* root = createNode(points[mid].x, points[mid].y);

    root->left = buildBalancedBST(points, start, mid - 1);
    root->right = buildBalancedBST(points, mid + 1, end);

    return root;
}

// Function to check if a point is within the circle
int isInsideCircle(int x, int y, int center_x, int center_y, int radius_squared) {
    int dx = x - center_x;
    int dy = y - center_y;
    return (dx * dx + dy * dy <= radius_squared); 
}

// Binary search to find the lower bound of y-coordinates within a given range
int findLowerBoundY(struct Point* points, int numPoints, int minY) {
    int low = 0, high = numPoints - 1;
    while (low < high) {
        int mid = (low + high) / 2;
        if (points[mid].y >= minY)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

// Binary search to find the upper bound of y-coordinates within a given range
int findUpperBoundY(struct Point* points, int numPoints, int maxY) {
    int low = 0, high = numPoints - 1;
    while (low < high) {
        int mid = (low + high) / 2;
        if (points[mid].y > maxY)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

// Optimized function for counting points with the same x-coordinate
int countPointsWithSameX(struct Point* points, int numPoints, int center_x, int center_y, int radius) {
    int radius_squared = radius * radius;
    int minY = center_y - radius;
    int maxY = center_y + radius;
    
    int lowerBound = findLowerBoundY(points, numPoints, minY);
    int upperBound = findUpperBoundY(points, numPoints, maxY);
    
    int count = 0;
    for (int i = lowerBound; i <= upperBound && i < numPoints; i++) {
        if (isInsideCircle(points[i].x, points[i].y, center_x, center_y, radius_squared)) {
            count++;
        }
    }

    return count;
}

// Function to count points inside the circle using the BST
int countPointsInCircle(struct Node* root, int center_x, int center_y, int radius) {
    if (root == NULL) {
        return 0;
    }

    int radius_squared = radius * radius;
    int count = 0;

    if (isInsideCircle(root->x, root->y, center_x, center_y, radius_squared)) {
        count = 1;
    }

    if (root->x >= center_x - radius) {
        count += countPointsInCircle(root->left, center_x, center_y, radius);
    }

    if (root->x <= center_x + radius) {
        count += countPointsInCircle(root->right, center_x, center_y, radius);
    }

    return count;
}

// Free the memory for the BST
void freeTree(struct Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(int argc, char* argv[]) {

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    int capacity = INITIAL_CAPACITY;
    struct Point* points = (struct Point*)malloc(capacity * sizeof(struct Point));
    if (points == NULL) {
        return 1;
    }

    int numPoints = 0;
    int x, y;

    while (fscanf(file, "%d %d", &x, &y) == 2) {
        if (numPoints == capacity) {
            capacity *= 2;
            struct Point* temp = (struct Point*)realloc(points, capacity * sizeof(struct Point));
            if (temp == NULL) {
                free(points);
                return 1;
            }
            points = temp;
        }

        points[numPoints].x = x;
        points[numPoints].y = y;
        numPoints++;
    }
    fclose(file);

    qsort(points, numPoints, sizeof(struct Point), comparePoints);

    // Check if all points have the same x-coordinate
    if (points[0].x == points[numPoints - 1].x) {
        int center_x, center_y, radius;
        while (scanf("%d %d %d", &center_x, &center_y, &radius) == 3) {
            int count = countPointsWithSameX(points, numPoints, center_x, center_y, radius);
            printf("%d\n", count);
        }
    } else {
        struct Node* root = buildBalancedBST(points, 0, numPoints - 1);
        int center_x, center_y, radius;
        while (scanf("%d %d %d", &center_x, &center_y, &radius) == 3) {
            int count = countPointsInCircle(root, center_x, center_y, radius);
            printf("%d\n", count);
        }
        freeTree(root);
    }

    free(points);

    return 0;
}
