#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Define a node for the binary search tree (for x-values)
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

// Function to insert a new node into the binary search tree
struct Node* insert(struct Node* root, int x, int y) {
    if (root == NULL) {
        return createNode(x, y);
    }

    if (x < root->x) {
        root->left = insert(root->left, x, y);
    } else if (x > root->x) {
        root->right = insert(root->right, x, y);
    } else {
        // if x values are the same, insert based on y (optional)
        if (y < root->y) {
            root->left = insert(root->left, x, y);
        } else {
            root->right = insert(root->right, x, y);
        }
    }

    return root;
}

// Function to check if a point is within the circle
int isInsideCircle(int x, int y, int center_x, int center_y, int radius_squared) {
    // Calculate distance squared from the center
    int dx = x - center_x;
    int dy = y - center_y;
    return (dx * dx + dy * dy <= radius_squared);
}

// Function to count points in the BST that lie within the circular region
int countPointsInCircle(struct Node* root, int center_x, int center_y, int radius) {
    if (root == NULL) {
        return 0;
    }

    // Calculate the squared radius to avoid floating-point calculations
    int radius_squared = radius * radius;

    // Check if the current point is inside the circle
    int count = 0;
    if (isInsideCircle(root->x, root->y, center_x, center_y, radius_squared)) {
        count = 1;
    }

    // If the current node's x value is too far to the left, skip the left subtree
    if (root->x > center_x - radius) {
        count += countPointsInCircle(root->left, center_x, center_y, radius);
    }

    // If the current node's x value is too far to the right, skip the right subtree
    if (root->x < center_x + radius) {
        count += countPointsInCircle(root->right, center_x, center_y, radius);
    }

    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <points_file>\n", argv[0]);
        return 1;
    }

    // Open the file for reading
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    struct Node* root = NULL;  // Initialize the root of the BST
    int x, y;

    // Read the points from the file and insert them into the BST
    while (fscanf(file, "%d %d", &x, &y) == 2) {
        root = insert(root, x, y);
    }

    // Close the file
    fclose(file);

    // Query the user for center and radius inputs
    int center_x, center_y, radius;
    while (1) {
        if (scanf("%d %d %d", &center_x, &center_y, &radius) != 3) {
            break;  // Exit on invalid input
        }

        // Count points within the circle defined by (center_x, center_y) and radius
        int count = countPointsInCircle(root, center_x, center_y, radius);
        printf("%d\n", count);
    }

    return 0;
}
