#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct
{
    char name[50];
    char phone[15];
} Contact;
typedef struct Node
{
    Contact contact;
    struct Node *left;
    struct Node *right;
    int height;
} Node;
int height(Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
int max(int a, int b)
{
    return (a > b) ? a : b;
}
Node* createNode(Contact contact)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->contact = contact;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}
Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;
    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}
Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;
    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}
int getBalance(Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
Node* insert(Node* node, Contact contact)
{
    if (node == NULL)
        return createNode(contact);

    if (strcmp(contact.name, node->contact.name) < 0)
        node->left = insert(node->left, contact);
    else if (strcmp(contact.name, node->contact.name) > 0)
        node->right = insert(node->right, contact);
    else
        return node; // Duplicate names not allowed

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && strcmp(contact.name, node->left->contact.name) < 0)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && strcmp(contact.name, node->right->contact.name) > 0)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && strcmp(contact.name, node->left->contact.name) > 0)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && strcmp(contact.name, node->right->contact.name) < 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void inOrder(Node* root)
{
    if (root != NULL)
    {
        inOrder(root->left);  // Visit left subtree
        printf("Name: %s\n", root->contact.name);  // Print contact name
        printf("    Phone: %s\n", root->contact.phone);  // Print contact phone number
        inOrder(root->right);  // Visit right subtree
    }
}
void caseface1()
{
    printf("\t\t----------------------------\n\t\t");
    char c[]="| Please Give Your Details |";
    int l=sizeof(c);
    for(int i=0; i<l; i++)
    {
        printf("%c",c[i]);
        usleep(100000);
    }
    printf("\n\t\t----------------------------\n");
}
Node* search(Node* root, const char* name)
{
    if (root == NULL || strcmp(root->contact.name, name) == 0)
    {
        return root;  // Contact found or reached end of tree
    }

    if (strcmp(name, root->contact.name) < 0)
    {
        return search(root->left, name);  // Search in the left subtree
    }

    return search(root->right, name);  // Search in the right subtree
}
// Function to find the minimum node in a tree
Node* findMin(Node* root)
{
    while (root->left != NULL)
    {
        root = root->left;
    }
    return root;
}

// Function to delete a contact
Node* deleteContact(Node* root, const char* name)
{
    if (root == NULL)
        return root;

    if (strcmp(name, root->contact.name) < 0)
        root->left = deleteContact(root->left, name);
    else if (strcmp(name, root->contact.name) > 0)
        root->right = deleteContact(root->right, name);
    else
    {
        if ((root->left == NULL) || (root->right == NULL))
        {
            Node* temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;

            free(temp);
        }
        else
        {
            Node* temp = findMin(root->right);
            root->contact = temp->contact;
            root->right = deleteContact(root->right, temp->contact.name);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    // Left Left
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

int main()
{
    Node* root = NULL;
    char name[50], phone_number[15];
    int choice;
X:
    printf("\n\t\t\t--------------------------------\n\t\t\t");
    char c[]="| Phone Book Management System |";
    int l=sizeof(c);
    for(int i=0; i<l; i++)
    {
        printf("%c",c[i]);
      usleep(100000);
    }
    printf("\n\t\t\t--------------------------------");
    printf("\n\t1.Add Contact\n");
    printf("\t2.View Contacts\n"); //Password Protection
    printf("\t3.Search Contact\n");
    printf("\t4.Delete Contact\n");
    printf("\t5.Update Contact\n");
    printf("\t6.Exit\n");
    printf("\tEnter your choice: ");
    scanf("%d", &choice);
    system("cls");
    switch(choice)
    {
    case 1:
        caseface1();
        // Add Contact - Case 1
        printf("\tEnter Name: ");
        getchar(); // To consume the newline character left by previous input
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0'; // Remove the newline at the end of the name

        printf("\tEnter Phone Number: ");
        fgets(phone_number, sizeof(phone_number), stdin);
        phone_number[strcspn(phone_number, "\n")] = '\0'; // Remove the newline at the end of the phone number
        Contact newContact;
        strcpy(newContact.name, name);
        strcpy(newContact.phone, phone_number);

        // Insert the new contact into the BST
        root = insert(root, newContact);
        printf("\t\t----------------------------\n\t\t");
        char c[]="| Contact Added Successfully |";
        int l=sizeof(c);
        for(int i=0; i<l; i++)
        {
            printf("%c",c[i]);
            usleep(100000);
        }
        printf("\n\t\t----------------------------\n");
        system("cls");
        goto X;
    case 2:    // View Contacts - Case 2
        if (root == NULL)
        {
            printf("\n\t\t\tNo contacts available.\n");
        }
        else
        {
            printf("\t\t-----------------\n\t\t");

            char e[]="| contacts List |";
            int l3=sizeof(e);
            for(int i=0; i<l3; i++)
            {
                printf("%c",e[i]);
                usleep(10000);
            }
            printf("\n\t\t-----------------\n");
            inOrder(root);  // Print all contacts using in-order traversal
        }
        printf("\n\n\t\t\t-----------------------------------\n\t\t\t");
        printf("| Press Any Key To Go to Main Menu |");
        printf("\n\t\t\t-----------------------------------\n\n");
        getch();
        system("cls");
        goto X ;
    case 3:
        printf("\n\tEnter the name of the contact to search: ");
        getchar();  // Consume newline character left by previous input
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0'; // Remove the newline at the end of the name
        for(int i=1; i<101; i++)
        {
            printf("\n\n\t\tSearching....");
            printf("%d",i);
            system("cls");
            usleep(100);
        }
        Node* found = search(root, name); // Search for the contact

        if (found != NULL)
        {
            printf("\t\t----------------");
            printf("\n\t\t| Contact Found |\n");
            printf("\t\t----------------");

            printf("\n\tName: %s\n", found->contact.name);
            printf("\tPhone: %s\n", found->contact.phone);
        }
        else
        {
            printf("\n\t\t--------------------");
            printf("\n\t\t| Contact not found |\n");
            printf("\t\t--------------------");

        }
        printf("\n\n\t\t\t-----------------------------------\n\t\t\t");
        printf("| Press Any Key To Go to Main Menu |");
        printf("\n\t\t\t-----------------------------------\n\n");
        getch();
        system("cls");
        goto X;
    case 4:
        printf("\n\tEnter the name of the contact to delete: ");
        getchar();  // Consume newline character left by previous input
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0'; // Remove the newline at the end of the name
        for(int i=1; i<101; i++)
        {
            printf("\n\n\t\tSearching....");
            printf("%d",i);
            system("cls");
            usleep(100);
        }
        Node* fond = search(root, name);

        if (fond != NULL)
        {
            printf("\n\t\t----------------");
            printf("\n\t\t| Contact Found |\n");
            printf("\t\t----------------");
            root = deleteContact(root, name);
            printf("\n\t\t-------------------------------\n\t\t");
            char c2[] = "| Contact Deleted Successfully |";
            int l2 = sizeof(c2);
            for (int i = 0; i < l2; i++)
            {
                printf("%c", c2[i]);
                usleep(100000);
            }
            printf("\n\t\t-------------------------------\n");
            printf("\n\n\t\t\t-----------------------------------\n\t\t\t");
            printf("| Press Any Key To Go to Main Menu |");
            printf("\n\t\t\t-----------------------------------\n\n");
            getch();
        }
        else
        {
            printf("\n\t\t\t---------------------");
            printf("\n\t\t\t| Contact not found |");
            printf("\n\t\t\t---------------------");
            printf("\n\n\t\t\t-----------------------------------\n\t\t\t");
            printf("| Press Any Key To Go to Main Menu |");
            printf("\n\t\t\t-----------------------------------\n\n");
            getch();
        }
        system("cls");
        goto X;
    case 5:
        printf("\n\tEnter the name of the contact to update: ");
        getchar();  // Consume newline character left by previous input
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0'; // Remove newline

        Node* contactToUpdate = search(root, name);

        if (contactToUpdate != NULL)
        {
            printf("\n\t\t------------------");
            printf("\n\t\t| Contact Found  |");
            printf("\n\t\t------------------");

            printf("\n\tCurrent Name: %s", contactToUpdate->contact.name);
            printf("\n\tCurrent Phone: %s", contactToUpdate->contact.phone);

            // First delete the existing contact
            root = deleteContact(root, name);

            printf("\n\n\tEnter new name: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0'; // Remove newline

            printf("\tEnter new phone number: ");
            fgets(phone_number, sizeof(phone_number), stdin);
            phone_number[strcspn(phone_number, "\n")] = '\0'; // Remove newline

            Contact updatedContact;
            strcpy(updatedContact.name, name);
            strcpy(updatedContact.phone, phone_number);

            // Insert updated contact into tree
            root = insert(root, updatedContact);

            printf("\n\t\t------------------------------");
            printf("\n\t\t| Contact Updated Successfully |");
            printf("\n\t\t------------------------------");

        }
        else
        {
            printf("\n\t\t----------------------");
            printf("\n\t\t| Contact not found  |");
            printf("\n\t\t----------------------");
        }

        printf("\n\n\t\t\t-----------------------------------\n\t\t\t");
        printf("| Press Any Key To Go to Main Menu |");
        printf("\n\t\t\t-----------------------------------\n\n");
        getch();
        system("cls");
        goto X;
    case 6:
        break;
    default :
        printf("\n\t\t Wrong choice.Please Try again\n\n\n");
    }
    return 0;
}
