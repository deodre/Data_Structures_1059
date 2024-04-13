#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define LINESIZE 128

typedef struct Song Song;
typedef struct Node Node;


struct Song {
    int trackId;
    char* title;
    char* artist;
    float royaltyPerPlay;
};

Song readSongFromFile(FILE* f) {

    Song song;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    song.trackId = atoi(buffer);

    fgets(buffer, LINESIZE, f);
    song.title = malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(song.title, buffer);

    fgets(buffer, LINESIZE, f);
    song.artist = malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(song.artist, buffer);

    fgets(buffer, LINESIZE, f);
    song.royaltyPerPlay = atof(buffer);

    return song;
}

void readSongsFromFile(const char* filename, Song** songsArray, int* songsNumber) {

    FILE* f;
    
    f = fopen(filename, "r");

    char buffer[LINESIZE];
    fgets(buffer, LINESIZE, f);

    *songsNumber = atoi(buffer);

    *songsArray = malloc((*songsNumber) * sizeof(Song));

    for (int i = 0; i < *songsNumber; i++) {
        (*songsArray)[i] = readSongFromFile(f);
    }

    fclose(f);

}

void printSongToConsole(Song song) {
    printf("--------\nSong ID: %d\nSong title: %sSong artist: %sRoyalty per play: %.3f\n--------\n", song.trackId, song.title, song.artist, song.royaltyPerPlay);
}

struct Node {
    Song song;
    Node* next;
};

void insertAtBeginning(Node** head, Song song) {

    Node* newNode = malloc(sizeof(Node));

    newNode->song = song;
    newNode->next = *head;

    *head = newNode;

}

void parseListAndPrint(Node* head) {
    if (head) {
        while (head) {
            printSongToConsole(head->song);
            head = head->next;
        }
    }
    else {
        printf("List is empty\n\n");
    }
}

void insertAtEnd(Node** head, Song song) {
    if (*head) {

        Node* aux = *head;

        while (aux->next) {
            aux = aux->next;
        }

        Node* newNode = malloc(sizeof(Node));
        newNode->song = song;
        newNode->next = NULL; // aux->next

        aux->next = newNode;

    }
    else {
        Node* newNode = malloc(sizeof(Node));

        newNode->song = song;
        newNode->next = *head; // NULL

        *head = newNode;
    }
}

Song deleteSongAtTheEnd(Node** head) {

    if (*head) {
        if ((*head)->next) {
            Node* aux = *head;
            while (aux->next->next) {
                aux = aux->next;
            }

            Song song = aux->next->song;
            //free(aux->next);
            aux->next = NULL;
            return song;
        }
        else {
            Song song = (*head)->song;
            //free((*head)->song);
            *head = NULL;
            return song;
        }
    }
    
    Song song;
    return song;
}

int main() {

    int songsNumber;
    Song* playlist;

    readSongsFromFile("playlist.txt", &playlist, &songsNumber);

   /* for (int i = 0; i < songsNumber; i++) {
        printSongToConsole(playlist[i]);
    }*/

    Node* head = NULL;

    parseListAndPrint(head);

    printf("List after one insertion:\n");
    insertAtBeginning(&head, playlist[0]);
    parseListAndPrint(head);
    printf("-------------------------\n");

    printf("List after the second insertion:\n");
    insertAtBeginning(&head, playlist[1]);
    parseListAndPrint(head);
    printf("-------------------------\n");

    printf("List after the third insertion:\n");
    insertAtBeginning(&head, playlist[2]);
    parseListAndPrint(head);
    printf("-------------------------\n");


    printf("\n\nList after inserting all songs:\n");
    for (int i = 0; i < songsNumber; i++) {
        insertAtBeginning(&head, playlist[i]);
    }

    parseListAndPrint(head);

    printf("\nINSERT AT THE END\n-----------------------\n\n");

    Node* head2 = NULL;

    parseListAndPrint(head2);

    printf("List after the first insertion:\n");
    insertAtEnd(&head2, playlist[0]);
    parseListAndPrint(head2);
    printf("-------------------------\n");

    printf("List after the second insertion:\n");
    insertAtEnd(&head2, playlist[1]);
    parseListAndPrint(head2);
    printf("-------------------------\n");

    printf("List after the third insertion:\n");
    insertAtEnd(&head2, playlist[2]);
    parseListAndPrint(head2);
    printf("-------------------------\n");

    Song song = deleteSongAtTheEnd(&head2);

    parseListAndPrint(head2);

    return 0;
}
