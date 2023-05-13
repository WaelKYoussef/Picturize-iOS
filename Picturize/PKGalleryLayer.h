//
//  PKGalleryFileManager.h
//  PhotoKit
//
//  Created by Wael Youssef on 6/28/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Photos/Photos.h>
#import "PKSettings.h"
#import "iOSwTextureExt.h"



#pragma mark - GalleryLayer.
struct _GLMeasurements{
    float MinThumbnailSize;
    int AlbumsPerRow_Portrait;
    int AlbumsPerRow_Landscape;
    int PhotosPerRow_Portrait;
    int PhotosPerRow_Landscape;
    float OutlineSize;
    float BarSize;
    float BarBorderSize;
    float BarButtonSize;
    float TitleFontSize;
    float AlbumFontSize;
    float AlbumEmptyIconSize;
    float AlbumCounterFontSize;
    float AlbumIconSize;
    float PhotoSelectorSize;
    
    float CanvasButtonSize;
};

struct _GLMeasurements GLM;
wView*GalleryLayer;

wView*GL_AlbumGrid;
wView*GL_AlbumGrid_EmptyLogo;
wView*GL_PhotoGrid;

wTexture*GL_AlbumMask;
wFont GL_Album_Font;
wFont GL_AlbumCounter_Font;

wTexture*GL_SelectedMask;
wTexture*GL_NotSelectedMask;

wTexture*GL_Album_EmptyTexture;

wTexture*GL_Album_Icon_Synced;
wTexture*GL_Album_Icon_Cloud;
wTexture*GL_Album_Icon_Favourite;
wTexture*GL_Album_Icon_New;
wTexture*GL_Album_Icon_Panorama;
wTexture*GL_Album_Icon_Imported;

unsigned int SelectedPhotosCount;
unsigned int SelectedDeletablePhotosCount;
bool*selectedFlags;
bool isInSelectMode;
unsigned int SelectedAlbumIndex;

unsigned char SelectedAlbumDeleteMode;
#define AlbumDeleteModeNone 0
#define AlbumDeleteModeRemove 1
#define AlbumDeleteModeDelete 2

NSMutableArray*SelectedAlbum;

void GL_Setup();
void GL_Display();
void GL_SetDisplayedView(void*view);
void GL_SetColorTheme(PKColorTheme colorTheme);
void GL_AdjustOrientation(wDeviceOrientation orientation);

void GL_LoadPhotos(wDeviceOrientation orientation);

void GL_PhotoGrid_EnterSelectMode();
void GL_PhotoGrid_EndSelectMode();
