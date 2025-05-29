#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    char title[100];
    char author[100];
    int year;
    char category[50];
} Book;

Book library[MAX];
int bookCount = 0;

void saveBooksToBinary() {
    FILE *file = fopen("library.dat", "wb");
    if (file != NULL) {
        fwrite(library, sizeof(Book), bookCount, file);
        fclose(file);
    }
}

void loadBooksFromBinary() {
    FILE *file = fopen("library.dat", "rb");
    if (file != NULL) {
        bookCount = fread(library, sizeof(Book), MAX, file);
        fclose(file);
    }
}

void addBook() {
    if (bookCount >= MAX) {
        printf("Library is full!\n");
        return;
    }

    Book newBook;
    printf("Enter book title: ");
    getchar();  // consume leftover newline
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title, "\n")] = '\0';

    printf("Enter author: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = '\0';

    printf("Enter publication year: ");
    scanf("%d", &newBook.year);
    getchar(); // consume leftover newline

    printf("Enter category: ");
    fgets(newBook.category, sizeof(newBook.category), stdin);
    newBook.category[strcspn(newBook.category, "\n")] = '\0';

    library[bookCount++] = newBook;
    saveBooksToBinary();
    printf("Book added successfully!\n");
}

void listBooks() {
    int i;
    if (bookCount == 0) {
        printf("No books in the library.\n");
        return;
    }

    printf("\n--- Book List ---\n");
    for (i = 0; i < bookCount; i++) {
        printf("%d. %s by %s (%d) [%s]\n", i + 1,
               library[i].title,
               library[i].author,
               library[i].year,
               library[i].category);
    }
}

void deleteBook() {
    int index;
    printf("Enter book number to delete (1 to %d): ", bookCount);
    scanf("%d", &index);
    if (index < 1 || index > bookCount) {
        printf("Invalid book number.\n");
        return;
    }

    for (; index < bookCount; index++) {
        library[index - 1] = library[index];
    }
    bookCount--;
    saveBooksToBinary();
    printf("Book deleted successfully.\n");
}

void updateBook() {
    int index;
    printf("Enter book number to update (1 to %d): ", bookCount);
    scanf("%d", &index);
    getchar(); // consume newline

    if (index < 1 || index > bookCount) {
        printf("Invalid book number.\n");
        return;
    }

    index--;

    printf("Enter new title: ");
    fgets(library[index].title, sizeof(library[index].title), stdin);
    library[index].title[strcspn(library[index].title, "\n")] = '\0';

    printf("Enter new author: ");
    fgets(library[index].author, sizeof(library[index].author), stdin);
    library[index].author[strcspn(library[index].author, "\n")] = '\0';

    printf("Enter new year: ");
    scanf("%d", &library[index].year);
    getchar();

    printf("Enter new category: ");
    fgets(library[index].category, sizeof(library[index].category), stdin);
    library[index].category[strcspn(library[index].category, "\n")] = '\0';

    saveBooksToBinary();
    printf("Book updated successfully.\n");
}

void bubbleSortByTitle() {
    int i, j;
    for (i = 0; i < bookCount - 1; i++) {
        for (j = 0; j < bookCount - i - 1; j++) {
            if (strcmp(library[j].title, library[j + 1].title) > 0) {
                Book temp = library[j];
                library[j] = library[j + 1];
                library[j + 1] = temp;
            }
        }
    }
    printf("Books sorted by title.\n");
}

int main() {
    int choice;
    loadBooksFromBinary();

    while (1) {
        printf("\nLibrary Menu:\n");
        printf("1. Add Book\n");
        printf("2. List Books\n");
        printf("3. Delete Book\n");
        printf("4. Update Book\n");
        printf("5. Sort Books by Title\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: listBooks(); break;
            case 3: deleteBook(); break;
            case 4: updateBook(); break;
            case 5: bubbleSortByTitle(); break;
            case 6: exit(0);
            default: printf("Invalid choice.\n");
        }
    }

    return 0;
}
