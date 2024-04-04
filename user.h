//
// Created by User on 02/04/2024.
//

#ifndef FINALWORKINC_USER_H
#define FINALWORKINC_USER_H
#include "playList.h"
#include "album.h"
#include "songRepository.h"


typedef struct {
    char* userName;
    PlayList* userPlayLists;
    int numOfPlaylists;
    Album* userAlbums;
    int numOfAlbums;
}User;

void initUser(User* pUser);

int addPlayListToUser(User* pUser, PlayList* pPlay);
int createPlayListToUser(User* pUser, SongRepository* pSongs);
int deleteSongFromUserPlayList(User* pUser,int index);
int addAlbumToUser(User* pUser, Album* pAlbums);
int deleteAlbumFromUser(User* pUser);
int deletePlayListFromUser(User* pUser);

void ShufflePlayList(const User* pUser, PlayList* pPlay); // maby receives only user and asks him which one to play
void playByOrderPlayList(const User* pUser); // same comments to all play functions


void printUser(const User* pUser);
void printPlayListForUser(const User* pUser);
void printAlbumsForUser(const User* pUser);
void freeUser(User* pUser);


int writeUserToTextFile(User* pUser, const char* fileName);
int readUserFromTextFile(User* pUser, const char* fileName);
int writeUserToBFile(User* pUser, const char* fileName);
int readUserFromBFile(User* pUser, const char* fileName);



#endif //FINALWORKINC_USER_H
