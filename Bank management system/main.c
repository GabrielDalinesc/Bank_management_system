#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define name_lenght 50
#define iban_lenght 20
#define coin_lenght 5
#define max_accounts 500
#define accounts_file "accounts.txt"
#define PERSONS_FILE "persons.txt"

typedef struct {
    char name[name_lenght];
    char surname[name_lenght];
} Person;

typedef struct {
    char iban[iban_lenght];
    Person owner;
    char coin[coin_lenght];
    float amount;
} Account;

void saveAccounts(Account accounts[], int numAccounts);
void loadAccounts(Account accounts[], int *numAccounts);
void createAccount(Account accounts[], int *numAccounts);
void editAccount(Account accounts[], int numAccounts);
void deleteAccount(Account accounts[], int *numAccounts);
void viewAccount(Account accounts[], int numAccounts);
void makeTransaction(Account accounts[], int numAccounts);

int main() {
    Account accounts[max_accounts];
    int numAccounts = 0;
    loadAccounts(accounts, &numAccounts);

    int choice;
    do {
        printf("\nBank Management System\n");
        printf("1. Create Account\n");
        printf("2. Edit Account\n");
        printf("3. Delete Account\n");
        printf("4. View Account\n");
        printf("5. Make Transaction\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount(accounts, &numAccounts);
                break;
            case 2:
                editAccount(accounts, numAccounts);
                break;
            case 3:
                deleteAccount(accounts, &numAccounts);
                break;
            case 4:
                viewAccount(accounts, numAccounts);
                break;
            case 5:
                makeTransaction(accounts, numAccounts);
                break;
            case 6:
                saveAccounts(accounts, numAccounts);
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 6.\n");
        }
    } while (choice != 6);

    return 0;
}



void saveAccounts(Account accounts[], int numAccounts) {
    FILE *file = fopen(accounts_file, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%s %s %s %.2f\n", accounts[i].iban, accounts[i].owner.name, accounts[i].owner.surname, accounts[i].amount);
    }

    fclose(file);
}

void loadAccounts(Account accounts[], int *numAccounts) {
    FILE *file = fopen(accounts_file, "r");
    if (file == NULL) {
        printf("No existing accounts found.\n");
        return;
    }

    *numAccounts = 0;
    while ((*numAccounts < max_accounts) && fscanf(file, "%s %s %s %f", accounts[*numAccounts].iban, accounts[*numAccounts].owner.name, accounts[*numAccounts].owner.surname, &accounts[*numAccounts].amount) == 4) {
        (*numAccounts)++;
    }

    fclose(file);
}

void createAccount(Account accounts[], int *numAccounts) {
    if (*numAccounts < max_accounts) {
        Account newAccount;
        printf("\nEnter account details:\n");
        printf("IBAN: ");
        scanf("%s", newAccount.iban);
        printf("Owner's Name: ");
        scanf("%s", newAccount.owner.name);
        printf("Owner's Surname: ");
        scanf("%s", newAccount.owner.surname);
        printf("Currency (RON/EUR/USD): ");
        scanf("%s", newAccount.coin);
        printf("Amount: ");
        scanf("%f", &newAccount.amount);

        accounts[*numAccounts] = newAccount;
        (*numAccounts)++;
        printf("Account created successfully.\n");
    } else {
        printf("Maximum number of accounts reached.\n");
    }
}

void editAccount(Account accounts[], int numAccounts) {
    char iban[iban_lenght];
    printf("\nEnter the IBAN of the account you want to edit: ");
    scanf("%s", iban);

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].iban, iban) == 0) {
            printf("Enter new account details:\n");
            printf("IBAN: ");
            scanf("%s", accounts[i].iban);
            printf("Owner's Name: ");
            scanf("%s", accounts[i].owner.name);
            printf("Owner's Surname: ");
            scanf("%s", accounts[i].owner.surname);
            printf("Currency (RON/EUR/USD): ");
            scanf("%s", accounts[i].coin);
            printf("Amount: ");
            scanf("%f", &accounts[i].amount);
            printf("Account edited successfully.\n");
            return;
        }
    }

    printf("Account not found.\n");
}

void deleteAccount(Account accounts[], int *numAccounts) {
    char iban[iban_lenght];
    printf("\nEnter the IBAN of the account you want to delete: ");
    scanf("%s", iban);

    for (int i = 0; i < *numAccounts; i++) {
        if (strcmp(accounts[i].iban, iban) == 0) {
            for (int j = i; j < *numAccounts - 1; j++) {
                accounts[j] = accounts[j + 1];
            }
            (*numAccounts)--;
            printf("Account deleted successfully.\n");
            return;
        }
    }

    printf("Account not found.\n");
}

void viewAccount(Account accounts[], int numAccounts) {
    char iban[iban_lenght];
    printf("\nEnter the IBAN of the account you want to view: ");
    scanf("%s", iban);

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].iban, iban) == 0) {
            printf("\nAccount Details:\n");
            printf("IBAN: %s\n", accounts[i].iban);
            printf("Owner: %s %s\n", accounts[i].owner.name, accounts[i].owner.surname);
            printf("Currency: %s\n", accounts[i].coin);
            printf("Amount: %.2f\n", accounts[i].amount);
            return;
        }
    }

    printf("Account not found.\n");
}


void makeTransaction(Account accounts[], int numAccounts) {
    char sourceIban[iban_lenght];
    char destIban[iban_lenght];
    float amount;
    printf("\nEnter source IBAN: ");
    scanf("%s", sourceIban);
    printf("Enter destination IBAN: ");
    scanf("%s", destIban);
    printf("Enter amount: ");
    scanf("%f", &amount);

    int sourceIndex = -1;
    int destIndex = -1;

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].iban, sourceIban) == 0) {
            sourceIndex = i;
        }
        if (strcmp(accounts[i].iban, destIban) == 0) {
            destIndex = i;
        }
    }

    if (sourceIndex == -1 || destIndex == -1) {
        printf("One or both accounts not found.\n");
        return;
    }

    if (accounts[sourceIndex].amount < amount) {
        printf("Insufficient funds in the source account.\n");
        return;
    }

    accounts[sourceIndex].amount -= amount;
    accounts[destIndex].amount += amount;
    printf("Transaction successful.\n");
}
