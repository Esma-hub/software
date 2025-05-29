#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    int accountNo;
    char name[100];
    float balance;
} Account;

Account accounts[MAX];
int accountCount = 0;

// Load accounts from file
void loadAccounts() {
    FILE *fp = fopen("accounts.txt", "r");
    if (!fp) {
        printf("No existing accounts. Starting new.\n");
        return;
    }

    while (fscanf(fp, "%d %s %f",
           &accounts[accountCount].accountNo,
           accounts[accountCount].name,
           &accounts[accountCount].balance) == 3) {
        accountCount++;
    }

    fclose(fp);
}

// Save accounts to file
void saveAccounts() {
    FILE *fp = fopen("accounts.txt", "w");
    int i;
    for (i = 0; i < accountCount; i++) {
        fprintf(fp, "%d %s %.2f\n", accounts[i].accountNo,
                                    accounts[i].name,
                                    accounts[i].balance);
    }
    fclose(fp);
}

void createAccount() {
    if (accountCount >= MAX) {
        printf("Maximum account limit reached!\n");
        return;
    }

    Account newAcc;
    printf("Enter name: ");
    getchar(); // clear newline buffer
    fgets(newAcc.name, sizeof(newAcc.name), stdin);
    newAcc.name[strcspn(newAcc.name, "\n")] = '\0';  // remove newline

    printf("Enter initial balance: ");
    scanf("%f", &newAcc.balance);

    newAcc.accountNo = 1000 + accountCount;
    accounts[accountCount++] = newAcc;

    printf("Account created successfully! Account No: %d\n", newAcc.accountNo);
    saveAccounts();
}

int findAccount(int accountNo) {
    int i;
    for (i = 0; i < accountCount; i++) {
        if (accounts[i].accountNo == accountNo) {
            return i;
        }
    }
    return -1;
}

void viewBalance() {
    int no, index;
    printf("Enter account number: ");
    scanf("%d", &no);

    index = findAccount(no);
    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    printf("Name: %s\nBalance: %.2f\n", accounts[index].name, accounts[index].balance);
}

void depositMoney() {
    int no, index;
    float amount;
    printf("Enter account number: ");
    scanf("%d", &no);

    index = findAccount(no);
    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    printf("Enter deposit amount: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid amount.\n");
        return;
    }

    accounts[index].balance += amount;
    printf("New balance: %.2f\n", accounts[index].balance);
    saveAccounts();
}

void withdrawMoney() {
    int no, index;
    float amount;
    printf("Enter account number: ");
    scanf("%d", &no);

    index = findAccount(no);
    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    printf("Enter withdrawal amount: ");
    scanf("%f", &amount);

    if (amount <= 0 || amount > accounts[index].balance) {
        printf("Invalid transaction.\n");
        return;
    }

    accounts[index].balance -= amount;
    printf("New balance: %.2f\n", accounts[index].balance);
    saveAccounts();
}

void transferMoney() {
    int senderNo, receiverNo, senderIndex, receiverIndex;
    float amount;

    printf("Sender account number: ");
    scanf("%d", &senderNo);
    printf("Receiver account number: ");
    scanf("%d", &receiverNo);
    printf("Transfer amount: ");
    scanf("%f", &amount);

    senderIndex = findAccount(senderNo);
    receiverIndex = findAccount(receiverNo);

    if (senderIndex == -1 || receiverIndex == -1) {
        printf("Account(s) not found.\n");
        return;
    }

    if (amount <= 0 || accounts[senderIndex].balance < amount) {
        printf("Invalid amount.\n");
        return;
    }

    accounts[senderIndex].balance -= amount;
    accounts[receiverIndex].balance += amount;
    printf("Transfer successful!\n");
    saveAccounts();
}

int main() {
    int choice;

    loadAccounts();

    while (1) {
        printf("\n--- Bank Account Management ---\n");
        printf("1. Create Account\n");
        printf("2. View Balance\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Transfer Money\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewBalance(); break;
            case 3: depositMoney(); break;
            case 4: withdrawMoney(); break;
            case 5: transferMoney(); break;
            case 6: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice.\n");
        }
    }

    return 0;
}
