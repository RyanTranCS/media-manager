typedef struct Music {
    char *artist;
    char *genre;
} Music;

typedef struct Book {
    char *author;
    char *genre;
} Book;

typedef struct Movie {
    char *contentRating;
} Movie;

typedef struct Media {
    char *typeFlag;
    char *name;
    char *year;
    union Type {
        Music music;
        Book book;
        Movie movie;
    } type;
} Media;
