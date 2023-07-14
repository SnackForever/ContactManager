#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    int id;
    char name[100];
    char phoneNumber[100];
} Contact;

void flushInputBuffer();
void addContact();
void searchMenu();
void searchWithName();
void searchWithPhoneNumber();
void searchContact(const char *searchTerm);
void writeContact(Contact contact);
void removeContact();
void menu();
int generateUniqueID();

int main()
{
    menu();
    return 0;
}

void menu()
{
    system("cls");
    printf("Contact Manager\n");
    printf("---------------\n");
    printf("1. Add new contact\n");
    printf("2. Search contact\n");
    printf("3. Remove contact\n");
    printf("4. Exit\n");
    printf("Choose the action: ");

    int choice;
    scanf("%d", &choice);
    flushInputBuffer();

    switch (choice)
    {
    case 1:
        addContact();
        break;

    case 2:
        searchMenu();
        break;

    case 3:
        removeContact();
        break;

    default:
        break;
    }
}

void addContact()
{
    Contact newContact;
    system("cls");
    printf("Adding new contact...\n");
    printf("---------------------\n");
    printf("Enter contact name: ");
    scanf(" %[^\n]", newContact.name);
    flushInputBuffer();

    printf("Enter contact phone number: ");
    fgets(newContact.phoneNumber, sizeof(newContact.phoneNumber), stdin);
    newContact.phoneNumber[strcspn(newContact.phoneNumber, "\n")] = '\0';

    newContact.id = generateUniqueID();

    writeContact(newContact);
}

void searchMenu()
{
    int choice;
    system("cls");
    printf("Searching for a contact...\n");
    printf("--------------------------\n");
    printf("1. Search user with name\n");
    printf("2. Search user with phone number\n");
    printf("3. Go back to the menu\n");
    printf("4. Exit\n");
    printf("Choose the action: ");

    scanf("%d", &choice);
    flushInputBuffer();

    switch (choice)
    {
    case 1:
        searchWithName();
        break;

    case 2:
        searchWithPhoneNumber();
        break;

    case 3:
        menu();
        break;

    default:
        break;
    }
}

void searchWithPhoneNumber()
{
    char phoneNumber[100];
    system("cls");
    printf("Enter the phone number of your contact: ");
    scanf("%s", phoneNumber);
    flushInputBuffer();
    searchContact(phoneNumber);
}

void searchWithName()
{
    char name[100];
    system("cls");
    printf("Enter the name of your contact: ");
    scanf(" %[^\n]", name);
    flushInputBuffer();
    searchContact(name);
}

void searchContact(const char *searchTerm)
{
    FILE *file;
    file = fopen("contact.txt", "r");

    if (file == NULL)
    {
        printf("Impossible to open the file\n");
        return;
    }

    char line[100];
    int found = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        int id;
        char name[100];
        char phoneNumber[100];

        if (sscanf(line, "%d %s %s", &id, name, phoneNumber) == 3)
        {
            if (strcmp(name, searchTerm) == 0 || strcmp(phoneNumber, searchTerm) == 0)
            {
                printf("Contact found: ID=%d, Name=%s, Phone=%s\n", id, name, phoneNumber);
                found = 1;
            }
        }
    }

    if (!found)
    {
        printf("Contact not found: %s\n", searchTerm);
    }

    fclose(file);
    printf("\nPress Enter to continue...");
    getchar();
    menu();
}

void writeContact(Contact contact)
{
    FILE *file;
    file = fopen("contact.txt", "a");

    if (file == NULL)
    {
        printf("Impossible to open the file\n");
        return;
    }

    fprintf(file, "%d %s %s\n", contact.id, contact.name, contact.phoneNumber);
    fclose(file);
    printf("Contact added successfully!\n");
    printf("\nPress Enter to continue...");
    getchar();
    menu();
}

void removeContact()
{
    int contactId;
    printf("Enter the ID of the contact to remove: ");
    scanf("%d", &contactId);
    flushInputBuffer();
    FILE *file;
    file = fopen("contact.txt", "r");

    if (file == NULL)
    {
        printf("Impossible to open the file\n");
        return;
    }

    FILE *tempFile;
    tempFile = fopen("temp.txt", "w");

    if (tempFile == NULL)
    {
        printf("Error creating temporary file\n");
        fclose(file);
        return;
    }

    char line[100];
    int found = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        int id;
        char name[100];
        char phoneNumber[100];

        if (sscanf(line, "%d %s %s", &id, name, phoneNumber) == 3)
        {
            if (id != contactId)
            {
                fprintf(tempFile, "%d %s %s\n", id, name, phoneNumber);
            }
            else
            {
                found = 1;
            }
        }
    }

    fclose(file);
    fclose(tempFile);

    if (!found)
    {
        printf("Contact not found: ID=%d\n", contactId);
    }
    else
    {
        remove("contact.txt");
        rename("temp.txt", "contact.txt");
        printf("Contact removed successfully!\n");
    }

    printf("\nPress Enter to continue...");
    getchar();
    menu();
}

int generateUniqueID()
{
    time_t currentTime = time(NULL);

    char timestamp[20];
    sprintf(timestamp, "%ld", currentTime);

    int randomNum = rand();

    char id[30];
    sprintf(id, "%s%d", timestamp, randomNum);

    int uniqueID = atoi(id);

    return uniqueID;
}

void flushInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
