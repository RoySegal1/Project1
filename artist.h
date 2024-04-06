#pragma once


typedef unsigned char BYTE;
typedef struct{
    char* name;
    int amountOfPlay;
    int amountOfSongs;
    char* aboutMe;
}Artist;

void showArtist(const void* pArtist);
void showArtistName(const void* pArtist);
//void getArtistName(Artist* artist);
int	loadArtistFromTextFile(Artist * artist, FILE* fp);
int	saveArtistToTextFile(const Artist * artist, FILE* fp);
int saveArtistToCompressFile(Artist* pArtist, FILE* fp);
//int saveArtistArrToCompressFile();
int creatArtist(Artist* artist);
int writeArtistToBFile(Artist* pArtist,FILE* fp);
int readArtistFromBFile(Artist* pArtist,FILE* fp);
int readArtistFromCompressFile(Artist* pArtist,FILE* fp);
void freeArtist(Artist *artist);
