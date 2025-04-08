#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME_LENGTH 50
#define PHONE_LENGTH 15
#define FILE_NAME "contacts.txt"

// Structure to store contact details
struct Contact {
    char name[NAME_LENGTH];
    char phone[PHONE_LENGTH];
};

// Binary Search Tree Node structure
struct BSTNode {
    struct Contact contact;
    struct BSTNode *left;
    struct BSTNode *right;
};

// Stack Node structure (for undo functionality)
struct StackNode {
    struct Contact contact;
    struct StackNode *next;
};

// Function prototypes
void addContact(struct StackNode **stack, struct BSTNode **root);
void displayContacts(struct BSTNode *root);
void searchContact(struct BSTNode *root, const char *name);
void deleteContact(struct StackNode **stack, struct BSTNode **root, const char *name);
void undoAddContact(struct StackNode **stack, struct BSTNode **root);
int menu();
void loadContacts(struct BSTNode **root);
void saveContacts(struct BSTNode *root);

// Function to create a new contact
struct Contact createContact(const char *name, const char *phone) {
    struct Contact newContact;
    strncpy(newContact.name, name, NAME_LENGTH);
    strncpy(newContact.phone, phone, PHONE_LENGTH);
    return newContact;
}

// Stack Operations (for Undo)
void pushStack(struct StackNode **stack, struct Contact contact) {
    struct StackNode *newNode = (struct StackNode *)malloc(sizeof(struct StackNode));
    if (newNode == NULL) {
        printf("Memory allocation error.\n");
        return;
    }
    newNode->contact = contact;
    newNode->next = *stack;
    *stack = newNode;
}

struct Contact popStack(struct StackNode **stack) {
    if (*stack == NULL) {
        printf("No actions to undo.\n");
        struct Contact emptyContact = {"", ""};
        return emptyContact;
    }
    struct StackNode *temp = *stack;
    struct Contact contact = temp->contact;
    *stack = (*stack)->next;
    free(temp);
    return contact;
}

// BST Operations (for storing contacts)
struct BSTNode* createNode(struct Contact contact) {
    struct BSTNode *newNode = (struct BSTNode *)malloc(sizeof(struct BSTNode));
    newNode->contact = contact;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void insertBST(struct BSTNode **root, struct Contact contact) {
    if (*root == NULL) {
        *root = createNode(contact);
        return;
    }
    if (strcmp(contact.name, (*root)->contact.name) < 0) {
        insertBST(&(*root)->left, contact);
    } else {
        insertBST(&(*root)->right, contact);
    }
}

struct BSTNode* searchBST(struct BSTNode *root, const char *name) {
    if (root == NULL || strcmp(root->contact.name, name) == 0) {
        return root;
    }
    if (strcmp(name, root->contact.name) < 0) {
        return searchBST(root->left, name);
    }
    return searchBST(root->right, name);
}

struct BSTNode* minNode(struct BSTNode *node) {
    struct BSTNode *current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

struct BSTNode* deleteBST(struct BSTNode *root, const char *name) {
    if (root == NULL) return root;

    if (strcmp(name, root->contact.name) < 0) {
        root->left = deleteBST(root->left, name);
    } else if (strcmp(name, root->contact.name) > 0) {
        root->right = deleteBST(root->right, name);
    } else {
        if (root->left == NULL) {
            struct BSTNode *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct BSTNode *temp = root->left;
            free(root);
            return temp;
        }

        struct BSTNode *temp = minNode(root->right);
        root->contact = temp->contact;
        root->right = deleteBST(root->right, temp->contact.name);
    }
    return root;
}

void inOrderTraversal(struct BSTNode *root) {
    if (root == NULL) return;
    inOrderTraversal(root->left);
    printf("Name: %s, Phone: %s\n", root->contact.name, root->contact.phone);
    inOrderTraversal(root->right);
}

// Function to add a new contact (uses stack for undo and BST for storage)
void addContact(struct StackNode **stack, struct BSTNode **root) {
    char name[NAME_LENGTH], phone[PHONE_LENGTH];

    printf("Enter name: ");
    getchar();  // to clear the buffer
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // remove newline

    printf("Enter phone number: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = 0;  // remove newline

    struct Contact newContact = createContact(name, phone);

    // Push to stack for undo functionality
    pushStack(stack, newContact);

    // Insert into BST
    insertBST(root, newContact);

    printf("Contact added successfully.\n");
}

// Function to display all contacts (in order of BST)
void displayContacts(struct BSTNode *root) {
    if (root == NULL) {
        printf("No contacts to display.\n");
        return;
    }

    printf("\nContacts List:\n");
    inOrderTraversal(root);
}

// Function to search for a contact by name
void searchContact(struct BSTNode *root, const char *name) {
    struct BSTNode *foundNode = searchBST(root, name);
    if (foundNode != NULL) {
        printf("Contact found: Name: %s, Phone: %s\n", foundNode->contact.name, foundNode->contact.phone);
    } else {
        printf("Contact not found.\n");
    }
}

// Function to delete a contact by name (uses stack and BST)
void deleteContact(struct StackNode **stack, struct BSTNode **root, const char *name) {
    struct BSTNode *foundNode = searchBST(*root, name);
    if (foundNode == NULL) {
        printf("Contact not found.\n");
        return;
    }

    // Push to stack before deletion for undo functionality
    pushStack(stack, foundNode->contact);

    // Delete from BST
    *root = deleteBST(*root, name);

    printf("Contact deleted successfully.\n");
}

// Function to undo the last added contact
void undoAddContact(struct StackNode **stack, struct BSTNode **root) {
    struct Contact lastAdded = popStack(stack);
    if (lastAdded.name[0] == '\0') {
        return;
    }

    // Delete the last added contact from BST
    *root = deleteBST(*root, lastAdded.name);
    printf("Undo successful: Contact '%s' removed.\n", lastAdded.name);
}

// Function to load contacts from the file into the BST
void loadContacts(struct BSTNode **root) {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("No saved contacts found.\n");
        return;
    }

    char name[NAME_LENGTH], phone[PHONE_LENGTH];
    while (fscanf(file, "%49[^,],%14[^\n]\n", name, phone) == 2) {
        struct Contact contact = createContact(name, phone);
        insertBST(root, contact);
    }

    fclose(file);
}

// Function to save contacts from the BST to the file
void saveContacts(struct BSTNode *root) {
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct BSTNode *current = root;
    // Saving is done through in-order traversal
    inOrderTraversal(current);

    fclose(file);
}

// Function to display the menu
int menu() {
    int choice;
    printf("\nPhone Book Management System\n");
    printf("1. Add Contact\n");
    printf("2. Display Contacts\n");
    printf("3. Search Contact\n");
    printf("4. Delete Contact\n");
    printf("5. Undo Add Contact\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

// Main program
int main() {
    struct StackNode *undoStack = NULL;
    struct BSTNode *contactTree = NULL;
    int choice;

    // Load contacts from the file into the BST
    loadContacts(&contactTree);

    while (1) {
        choice = menu();

        switch (choice) {
            case 1:
                addContact(&undoStack, &contactTree);
                break;
            case 2:
                displayContacts(contactTree);
                break;
            case 3:
                {
                    char name[NAME_LENGTH];
                    printf("Enter the name to search: ");
                    getchar();  // to clear the buffer
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = 0;  // remove trailing newline
                    searchContact(contactTree, name);
                }
                break;
            case 4:
                {
                    char name[NAME_LENGTH];
                    printf("Enter the name to delete: ");
                    getchar();  // to clear the buffer
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = 0;  // remove trailing newline
                    deleteContact(&undoStack, &contactTree, name);
                }
                break;
            case 5:
                undoAddContact(&undoStack, &contactTree);
                break;
            case 6:
                // Save contacts to file before exiting
                saveContacts(contactTree);
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
