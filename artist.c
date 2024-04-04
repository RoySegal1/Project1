#include <stdio.h>
#include <stdlib.h>
#include "artist.h"
#include "string.h"
#include "General.h"
#include "fileHelper.h"
#include "macros.h"


void initArtist(Artist* artist)
{
    artist->name = NULL;
    artist->aboutMe = NULL;
    artist->amountOfPlay = 0;
    artist->amountOfSongs = 0;
}


void showArtist(const Artist* artist)
{
    printf("Artist Name: %s\n ",artist->name);
    printf("About him: %s\n ",artist->aboutMe);
}


int creatArtist(Artist* artist)
{
    artist->name = getStrExactName("Enter artist name\n");
    if(!artist->name)
        return 0;
    artist->aboutMe = getStrExactName("Write about the artist\n");
    if(!artist->aboutMe)
    {
        free(artist->name);
        return 0;
    }
    return 1;

}

int	saveArtistToTextFile(const Artist * artist, FILE* fp)
{
    if (!artist)
        return 0;
    fprintf(fp, "%s\n", artist->name);
    fprintf(fp, "%s\n", artist->aboutMe);
    fprintf(fp, "%d\n", artist->amountOfPlay);
    fprintf(fp, "%d\n", artist->amountOfSongs);
    return 1;
}

int	loadArtistFromTextFile(Artist * artist, FILE* fp)
{
    char temp[MAX_STR_LEN];
    CHECK_RETURN_0(artist)

    myGets(temp, MAX_STR_LEN, fp);
    artist->name = getDynStr(temp);
    CHECK_RETURN_0_PRINT(artist->name,"Error reading artist name\n")

    myGets(temp, MAX_STR_LEN, fp);
    artist->aboutMe = getDynStr(temp);
    CHECK_RETURN_0_PRINT(artist->aboutMe,"Error reading artist CV\n")
    if(fscanf(fp, "%d", &artist->amountOfPlay) != 1)
    {
        printf("Error reading artist's amount of played\n");
        free(artist->name);
        free(artist->aboutMe);
        return 0;
    }
    if(fscanf(fp, "%d", &artist->amountOfSongs) != 1)
    {
        printf("Error reading artist's amount of songs\n");
        free(artist->name);
        free(artist->aboutMe);
        return 0;
    }

    return 1;
}

int writeArtistToBFile(Artist * pArtist,FILE* fp)
{
    if(!writeStringToFile(pArtist->name,fp,"Error Writing Artist Name"))
        return 0;
    if(!writeStringToFile(pArtist->aboutMe,fp,"Error Writing \"About me\""))
        return 0;
    if(!writeIntToFile(pArtist->amountOfPlay,fp,"Error Writing amount of played songs"))
        return 0;
    if(!writeIntToFile(pArtist->amountOfSongs,fp,"Error Writing amount of songs"))
        return 0;
    return 1;
}
int saveArtistToCompressFile(Artist* pArtist, FILE* fp)
{
    BYTE dataArtist[4];
    int len1  = strlen(pArtist->name);
    int len2 = strlen(pArtist->aboutMe);
    dataArtist[0] = len1 << 3 | len2 >> 5;
    dataArtist[1] = (len2 & 0x1F) << 3 | pArtist->amountOfSongs >> 4 ;
    dataArtist[2] = (pArtist->amountOfSongs & 0xF) << 4 | pArtist->amountOfPlay >> 8;
    dataArtist[3] = pArtist->amountOfPlay & 0xFF;
    if(fwrite(dataArtist, sizeof(BYTE), 4, fp) != 4)
    {
        printf("Error write artist data to compress file\n");
        return 0;
    }

    if(!writeStringToComprassFile(pArtist->name,fp,"Error write artist name to compress file\n"))
        return 0;
    if (!writeStringToComprassFile(pArtist->aboutMe,fp,"Error write artist VC to compress file\n"))
        return 0;

    return 1;
}

int readArtistFromBFile(Artist * pArtist,FILE* fp)
{
    pArtist->name = readStringFromFile(fp,"Error Reading Artist Name");
    if(pArtist->name == NULL)
        return 0;

    pArtist->aboutMe = readStringFromFile(fp,"Error Reading \"About me\"");
    if(pArtist->aboutMe == NULL)
        return 0;

    if(!readIntFromFile(&pArtist->amountOfPlay,fp,"Error Reading of played songs"))
        return 0;
    if(!readIntFromFile(&pArtist->amountOfSongs,fp,"Error Reading amount of songs"))
        return 0;

    return 1;
}

int readArtistFromCompressFile(Artist * pArtist,FILE* fp)
{
    BYTE dataArtist[4];
    if (fread(&dataArtist, sizeof(BYTE), 4, fp) != 4)
    {
        printf("Error reading artist's data from compress file\n");
        return 0;
    }
    int len1 = dataArtist[0] >> 3;
    int len2 = (dataArtist[0] & 0x7) << 5 | (dataArtist[1] >>3);
    pArtist->amountOfSongs = (dataArtist[1] & 0x7) << 4 | dataArtist[2] >> 4;
    pArtist->amountOfPlay = (dataArtist[2] & 0xF) << 8 | dataArtist[3];
    pArtist->name = calloc(len1 + 1, sizeof(char ));
    if (!pArtist->name || fread(pArtist->name,sizeof(char),len1,fp) != len1)
        return 0;

    pArtist->aboutMe = calloc(len2 + 1, sizeof(char));
    if (!pArtist->aboutMe || fread(pArtist->aboutMe,sizeof(char),len2,fp) != len2)
        return 0;

    return 1;

}

Artist* initArtistArrFromFile(Artist* artist,const char* fileName,int* length, int typeFile)
{
    FILE* fp;
    if (typeFile == 0)// init from text file
    {
        fp = fopen(fileName, "r");
        CHECK_RETURN_NULL(fp)


        if(fscanf(fp,"%d",length) != 1)
        {
            fclose(fp);
            return NULL;
        }

        artist = malloc(sizeof(Artist)*(*length));
        for (int i = 0; i < *length; i++) {
            if (!loadArtistFromTextFile(&artist[i], fp))
                return NULL;
        }
        return artist;
    }

    if (typeFile == 1)// init from binary file
    {
        fp = fopen(fileName, "rb");
        CHECK_RETURN_NULL(fp)

        if (!readIntFromFile(length, fp, "Error reading song count\n"))
        {
            fclose(fp);
            return NULL;
        }
        artist = malloc(sizeof(Artist)*(*length));
        for (int i = 0; i < *length; ++i)
        {
            if(!readArtistFromBFile(&artist[i],fp))
                return NULL;
        }
        return artist;
    }

    if(typeFile == 2)// init from compress binary file
    {
        fp = fopen(fileName, "rb");
        CHECK_RETURN_NULL(fp)
        BYTE dataCounter;
        if (fread(&dataCounter, sizeof(BYTE),1,fp) != 1)
        {
            fclose(fp);
            return NULL;
        }
        *length = dataCounter;
        for (int i = 0; i < *length; ++i)
        {
            if(!readArtistFromCompressFile(&artist[i],fp))
            {
                fclose(fp);
                return NULL;
            }

        }
        return artist;
    }
    return NULL;
}

void freeArtist(Artist *artist)
{
    free(artist->name);
    free(artist->aboutMe);

}
