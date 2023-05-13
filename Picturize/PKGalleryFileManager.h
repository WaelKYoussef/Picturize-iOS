//
//  PKGalleryFileManager.h
//  PhotoKit
//
//  Created by Wael on 8/21/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef PhotoKit_PKGalleryFileManager_h
#define PhotoKit_PKGalleryFileManager_h
#import <Photos/Photos.h>
#import <CoreLocation/CoreLocation.h>
#include "wViewAPI.h"
#import "PKAlertManager.h"
BOOL CreateMetadataFileForAsset(PHAsset*asset, BOOL override);
typedef unsigned int PKMetaPropertyKey;
#define PKMetaPropertyKey_MetaData 0
#define PKMetaPropertyKey_Identifier 1
#define PKMetaPropertyKey_Comment 2

#define PKMetaPropertyKey_Tumbnail_Small 3
#define PKMetaPropertyKey_Tumbnail_Medium 4
#define PKMetaPropertyKey_Tumbnail_Large 5

typedef struct _PKLocation{
    float latitude,longitude;
}PKLocation;

typedef struct _PKMetaData{
    wDate creationDate;
    wDate lastModificationDate;
    PKLocation location;
    unsigned short filters[10];
    unsigned short otherProperties[10];
    unsigned char colorTag;
}PKMetaData;

typedef struct _PKThumb{
    unsigned int dataSize, width,height;
    wTextureFormat format;
}PKThumb;

typedef struct _PKMetaProperty{
    PKMetaPropertyKey key;
    unsigned int dataSize;
    void*data;
}PKMetaProperty;

typedef struct _PKMeta{
    PKMetaProperty*properties;
    float version;
    unsigned int fileSize;
    unsigned int propertyCount;
    unsigned int propertyCapacity;
    char signature[10];
}PKMeta;


BOOL GM_AccessIsAllowedToUserPhotos();
NSString*GM_PathForImageNameForITunes(NSString*imageName);
BOOL GM_ReloadAlbumAtIndex(unsigned int index);

void savethumbforimage(CGImageRef image, NSString*imageID);
void*PKThumb_TumbnailFromCGImage(CGImageRef imageRef);
#pragma mark - PKMeta.
bool PKMeta_FileIsAuthentic(void*fileData, unsigned int fileSize);
PKMeta*PKMeta_NewMeta();
PKMeta*PKMeta_LoadFromData(void*fileData);
void*PKMeta_SaveToData(PKMeta*meta);
void PKMeta_AddProperty(PKMeta*meta, PKMetaPropertyKey key, unsigned int dataSize, void*data);
void PKMeta_RemoveProperty(PKMeta*meta, PKMetaPropertyKey key);
PKMetaProperty PKMeta_GetPropertyForKey(PKMetaPropertyKey key,PKMeta*meta);
PKMetaProperty PKMeta_GetPropertyForKeyFromFile(PKMetaPropertyKey key,void*fileData);
void PKMeta_Release(PKMeta*meta);


NSMutableArray*Albums;
NSMutableArray*AlbumAssets;

void setupGL();
NSString* pathForThumbnailFileForID(NSString*ID);

#endif
