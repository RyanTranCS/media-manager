/**
 * This program stores the string values concerning media contained within a
 * text file into Media structures whose pointers are stored in an array,
 * sorts the array by media name, and prints the contents of the sorted array
 * in a separate text file.
 * 
 * CSC 3304 Programming Project No. 2
 * 
 * @author Ryan Tran
 * @since 2/25/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Media.h"

#define INPUT_FILE_PATH "./TestInput.txt"
#define OUTPUT_FILE_PATH "./out.txt"

#define MAX_STR_LEN 100
#define MAX_LINES 500

void ProcessMusicInfo(Media **media, FILE *inFilePtr, char *fileLine);
void ProcessBookInfo(Media **media, FILE *inFilePtr, char *fileLine);
void ProcessMovieInfo(Media **media, FILE *inFilePtr, char *fileLine);

void ProcessName(Media **media, FILE *inFilePtr, char *fileLine);
void ProcessYear(Media **media, FILE *inFilePtr, char *fileLine);

void BubbleSort(Media *array[], int arrayLength);
void Swap(Media **p1, Media **p2);

void OutputMediaInfo(Media *media[], int elementPosition, FILE *outFilePtr, int arrayLength);
void OutputMusicInfo(Media *media, FILE *outFilePtr);
void OutputBookInfo(Media *media, FILE *outFilePtr);
void OutputMovieInfo(Media *media, FILE *outFilePtr);

void FreeMediaPointerArray(Media *array[], int arrayLength);

int main() {
    // opens input file
    FILE *inFilePtr = fopen(INPUT_FILE_PATH, "r");
    if (!inFilePtr) {
        printf("Error Opening File: Exiting Program");
        return 1;
    }

    // declares array of Media struct pointers
    Media *media[MAX_LINES];

    // variable to store string values from fgets()
    char *fileLine = malloc(sizeof(char) * MAX_STR_LEN);
    
    // variable to access array elements
    // represents number of initialized array elements after loop finishes
    int counter = 0;

    // stores Media data from input file in array
    while (fgets(fileLine, MAX_STR_LEN, inFilePtr) != NULL) {
        // strips newline character from fgets()
        fileLine[strcspn(fileLine, "\n")] = 0;

        media[counter] = malloc(sizeof(Media));

        if (strcmp(fileLine, "music") == 0) {
            ProcessMusicInfo(&media[counter], inFilePtr, fileLine);
        }
        else if (strcmp(fileLine, "book") == 0) {
            ProcessBookInfo(&media[counter], inFilePtr, fileLine);
        }
        else {
            ProcessMovieInfo(&media[counter], inFilePtr, fileLine);
        }
        counter = counter + 1;
    }

    fclose(inFilePtr);
    
    free(fileLine);

    BubbleSort(media, counter);

    // opens output file
    FILE *outFilePtr = fopen(OUTPUT_FILE_PATH, "w");
    if (!outFilePtr) {
        printf("Error Opening File: Freeing Memory and Exiting Program");
        FreeMediaPointerArray(media, counter);
        return 1;
    }

    // writes Media structure data to output file
    for (int i = 0; i < counter; i++) {
        OutputMediaInfo(media, i, outFilePtr, counter);
    }

    fclose(outFilePtr);
    
    FreeMediaPointerArray(media, counter);

    return 0;
}

/**
 * The following 5 methods process media data contained within a file.
 * The data contained within the file is copied into the appropriate structure
 * for that media type.
 * 
 * routines:ProcessMusicInfo, ProcessBookInfo, ProcessMediaInfo, ProcessName,
 *  ProcessYear
 * 
 * return type: void
 * 
 * parameters:
 *  media           MEDIA**      pointer to a struct pointer
 *  inFilePtr       FILE*        pointer to stream
 *  fileLine        char*        pointer to string
 * 
 * @author Ryan Tran
 * @since 2/25/2021
 * */

void ProcessMusicInfo(Media **media, FILE *inFilePtr, char *fileLine) {
    (*media)->typeFlag = "music";

    ProcessName(media, inFilePtr, fileLine);

    fgets(fileLine, MAX_STR_LEN, inFilePtr);
    fileLine[strcspn(fileLine, "\n")] = 0;
    (*media)->type.music.artist = malloc(strlen(fileLine) + 1);
    strcpy((*media)->type.music.artist, fileLine);

    fgets(fileLine, MAX_STR_LEN, inFilePtr);
    fileLine[strcspn(fileLine, "\n")] = 0;
    (*media)->type.music.genre = malloc(strlen(fileLine) + 1);
    strcpy((*media)->type.music.genre, fileLine);

    ProcessYear(media, inFilePtr, fileLine);
}


void ProcessBookInfo(Media **media, FILE *inFilePtr, char *fileLine) {
    (*media)->typeFlag = "book";

    ProcessName(media, inFilePtr, fileLine);

    fgets(fileLine, MAX_STR_LEN, inFilePtr);
    fileLine[strcspn(fileLine, "\n")] = 0;
    (*media)->type.book.author = malloc(strlen(fileLine) + 1);
    strcpy((*media)->type.book.author, fileLine);

    fgets(fileLine, MAX_STR_LEN, inFilePtr);
    fileLine[strcspn(fileLine, "\n")] = 0;
    (*media)->type.book.genre = malloc(strlen(fileLine) + 1);
    strcpy((*media)->type.book.genre, fileLine);

    ProcessYear(media, inFilePtr, fileLine);
}

void ProcessMovieInfo(Media **media, FILE *inFilePtr, char *fileLine) {
    (*media)->typeFlag = "movie";

    ProcessName(media, inFilePtr, fileLine);

    fgets(fileLine, MAX_STR_LEN, inFilePtr);
    fileLine[strcspn(fileLine, "\n")] = 0;
    (*media)->type.movie.contentRating = malloc(strlen(fileLine) + 1);
    strcpy((*media)->type.movie.contentRating, fileLine);
    
    ProcessYear(media, inFilePtr, fileLine);
}

void ProcessName(Media **media, FILE *inFilePtr, char *fileLine) {
    fgets(fileLine, MAX_STR_LEN, inFilePtr);
    fileLine[strcspn(fileLine, "\n")] = 0;
    (*media)->name = malloc(strlen(fileLine) + 1);
    strcpy((*media)->name, fileLine);
}

void ProcessYear(Media **media, FILE *inFilePtr, char *fileLine) {
    fgets(fileLine, MAX_STR_LEN, inFilePtr);
    fileLine[strcspn(fileLine, "\n")] = 0;
    (*media)->year = malloc(strlen(fileLine) + 1);
    strcpy((*media)->year, fileLine);
}

/**
 * This method sorts an array containing Media structures in ascending order
 * using the bubble sort sorting algorithm.
 * 
 * routine: BubbleSort
 * 
 * return type: void
 * 
 * parameters:
 *  array[]         Media*      array to be sorted
 *  arrayLength     int         length of array
 * 
 * @author Ryan Tran
 * @since 2/25/2021
 * */

void BubbleSort(Media *array[], int arrayLength) {
    for (int i = 0; i < arrayLength - 1; i++) {
        for (int j = 0; j < arrayLength - i - 1; j++) {
            if (strcmp(array[j]->name, array[j + 1]->name) > 0) {
                Swap(&array[j], &array[j + 1]);
            }
        }
    }
}

/**
 * This method swaps pointers to Media structures.
 * 
 * routine: swap
 * 
 * return type: void
 * 
 * parameters:
 *  p1    [Media**]    address of first pointer to be swapped
 *  p2    [Media**]    address of second pointer to be swapped
 * 
 * @author Ryan Tran
 * @since 2/25/2021
 * */

void Swap(Media **p1, Media **p2) {
    Media *temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

/**
 * This method writes a Media structure's data to a specified stream.
 * 
 * routine: OutputMediaInfo
 * 
 * return type: void
 * 
 * parameters:
 *  media[]             Media*      array of Media structure pointers
 *  elementPosition     int         position of element to be accessed
 *  outFilePtr          FILE*       pointer to stream
 *  arrayLength         int         length of array
 * 
 * @author Ryan Tran
 * @since 2/25/2021
 * */

void OutputMediaInfo(Media *media[], int elementPosition, FILE *outFilePtr, int arrayLength) {
    fputs(media[elementPosition]->typeFlag, outFilePtr);
    fputs("\n", outFilePtr);

    fputs(media[elementPosition]->name, outFilePtr);
    fputs("\n", outFilePtr);

    if (media[elementPosition]->typeFlag == "music") {
        OutputMusicInfo(media[elementPosition], outFilePtr);
    }
    else if (media[elementPosition]->typeFlag == "book") {
        OutputBookInfo(media[elementPosition], outFilePtr);
    }
    else {
        OutputMovieInfo(media[elementPosition], outFilePtr);
    }

    fputs(media[elementPosition]->year, outFilePtr);

    if (elementPosition != arrayLength - 1) {
        fputs("\n\n", outFilePtr);
    }
}

/**
 * The following 3 methods writes a Media structure's data, which correspond
 * to its media format, to a specified stream.
 * 
 * routines: OutputMusicInfo, OutputBookInfo, OutputMovieInfo
 * 
 * return type: void
 * 
 * parameters:
 *  media       Media*    pointer to Media struct
 *  outFilePtr  FILE*     pointer to stream
 * 
 * @author Ryan Tran
 * @since 2/25/2021
 * */

void OutputMusicInfo(Media *media, FILE *outFilePtr) {
    fputs(media->type.music.genre, outFilePtr);
    fputs("\n", outFilePtr);
}

void OutputBookInfo(Media *media, FILE *outFilePtr) {
    fputs(media->type.book.author, outFilePtr);
    fputs("\n", outFilePtr);
    fputs(media->type.book.genre, outFilePtr);
    fputs("\n", outFilePtr);
}

void OutputMovieInfo(Media *media, FILE *outFilePtr) {
    fputs(media->type.movie.contentRating, outFilePtr);
    fputs("\n", outFilePtr);
}

/**
 * This method frees the memory allocated to the string variables contained
 * within a Media structure pointed to in an array. It then frees the
 * memory allocated to each pointer. It repeats this for each array element.
 * 
 * routine: FreeMediaPointerArray
 * 
 * return type: void
 * 
 * parameters:
 *  array[]         Media*  array to be freed
 *  arrayLength     int     length of array
 * 
 * @author Ryan Tran
 * @since 2/25/2021
 * */

void FreeMediaPointerArray(Media *array[], int arrayLength) {
    for (int i = 0; i < arrayLength; i++) {
        free(array[i]->name);

        if (strcmp(array[i]->typeFlag, "music") == 0) {
            free(array[i]->type.music.artist);
            free(array[i]->type.music.genre);
        }
        else if (strcmp(array[i]->typeFlag, "book") == 0) {
            free(array[i]->type.book.author);
            free(array[i]->type.book.genre);
        }
        else {
            free(array[i]->type.movie.contentRating);
        }

        free(array[i]->year);

        free(array[i]);
    }
}
