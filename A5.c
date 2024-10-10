#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 100 // Starting capacity, can grow dynamically

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

    // Find the middle point to ensure balance
    int mid = (start + end) / 2;
    struct Node* root = createNode(points[mid].x, points[mid].y);

    // Recursively build the left and right subtrees
    root->left = buildBalancedBST(points, start, mid - 1);
    root->right = buildBalancedBST(points, mid + 1, end);

    return root;
}

int isInsideCircle(int x, int y, int center_x, int center_y, int radius_squared) {
    int dx = x - center_x;
    int dy = y - center_y;
    return (dx * dx + dy * dy <= radius_squared); 
}


int countPointsInCircle(struct Node* root, int center_x, int center_y, int radius) {
    if (root == NULL) {
        return 0;
    }

    int radius_squared = radius * radius;
    int count = 0;
    if (isInsideCircle(root->x, root->y, center_x, center_y, radius_squared)) {
        count = 1;
    }

    if (root->x >= center_x - radius || root->y >= center_y - radius) {
        count += countPointsInCircle(root->left, center_x, center_y, radius);
    }

    if (root->x <= center_x + radius || root->y <= center_y + radius) {
        count += countPointsInCircle(root->right, center_x, center_y, radius);
    }

    return count;
}


int main(int argc, char* argv[]) {
    

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Dynamically allocate memory for points array
    int capacity = INITIAL_CAPACITY;
    struct Point* points = (struct Point*)malloc(capacity * sizeof(struct Point));
    if (points == NULL) {
        return 1;
    }

    int numPoints = 0;
    int x, y;

    // Read points from the file and resize the array if necessary
    while (fscanf(file, "%d %d", &x, &y) == 2) {
        // Resize the array if it's full
        if (numPoints == capacity) {
            capacity += 1; // add 1 to our capacity if our array is full
            struct Point* temp = (struct Point*)realloc(points, capacity * sizeof(struct Point));
            if (temp == NULL) {
                free(points);
                return 1;
            }
            points = temp;
        }

        // Store the point in the array
        points[numPoints].x = x;
        points[numPoints].y = y;
        numPoints++;
    }
    fclose(file);

    // Sort the points by x and y
    qsort(points, numPoints, sizeof(struct Point), comparePoints);

    // Build a balanced BST from the sorted points
    struct Node* root = buildBalancedBST(points, 0, numPoints - 1);


    // Handle queries (you can reuse the query code from the previous solution)
    int center_x, center_y, radius;
    while (1) {
        if (scanf("%d %d %d", &center_x, &center_y, &radius) != 3) {
            break;
        }


        int count = countPointsInCircle(root, center_x, center_y, radius);
        printf("%d\n", count);
    }

    // Free dynamically allocated memory before exiting
    free(points);

    return 0;
}

