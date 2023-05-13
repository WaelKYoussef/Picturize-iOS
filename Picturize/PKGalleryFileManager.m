//
//  PKGalleryFileManager.m
//  PhotoKit
//
//  Created by Wael on 8/21/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import "PKGalleryFileManager.h"
#import <AssetsLibrary/AssetsLibrary.h>
NSString*PathForMainDirectory;
NSString*PathForMetaDataFolder;

BOOL GM_AccessIsAllowedToUserPhotos(){
    return [PHPhotoLibrary authorizationStatus]==PHAuthorizationStatusAuthorized;
}

NSString* pathForThumbnailFileForID(NSString*ID){
    NSString*Path=[PathForMetaDataFolder stringByAppendingString:ID];
    Path=[[Path stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];
    Path=[Path stringByAppendingString:@"%.PKMetaData"];
    
    return Path;
}

NSString*GM_PathForImageNameForITunes(NSString*imageName){
    return [PathForMainDirectory stringByAppendingPathComponent:imageName];
}


PHImageManager*ImageManager;
PHImageRequestOptions*ImageManagerOptions;

BOOL GM_ReloadAlbumAtIndex(unsigned int index){
    if (index>=Albums.count) {return false;}
    PHAssetCollection*Album=[Albums objectAtIndex:index];
    
    PHFetchResult*Assets=[PHAsset fetchAssetsInAssetCollection:Album options:nil];
    NSMutableArray*Filtered=[NSMutableArray arrayWithCapacity:Assets.count];
    for (long i=Assets.count-1; i>=0; i--) {
        PHAsset*Asset=[Assets objectAtIndex:i];
        if ([Asset mediaType]==PHAssetMediaTypeImage) {
            [Filtered addObject:Asset];
            CreateMetadataFileForAsset(Asset, false);
        }
    }
    
    [AlbumAssets replaceObjectAtIndex:index withObject:Filtered];
    return true;
}

void setupGL(){
    
    NSArray*PathArray=NSSearchPathForDirectoriesInDomains
    (NSDocumentDirectory, NSUserDomainMask, TRUE);
    PathForMainDirectory=[PathArray objectAtIndex:0];
    
    NSString*PathForDataFolder=[PathForMainDirectory stringByAppendingPathComponent:@"/.DataFolder/"];
    PathForMetaDataFolder=[PathForMainDirectory stringByAppendingPathComponent:@"/.DataFolder/.PhotosMetaData/"];
    
    NSFileManager*FileMan=[NSFileManager defaultManager];
    if (![FileMan fileExistsAtPath:PathForDataFolder])
        [FileMan createDirectoryAtPath:PathForDataFolder withIntermediateDirectories:NO attributes:nil error:nil];
    
    if (![FileMan fileExistsAtPath:PathForMetaDataFolder])
        [FileMan createDirectoryAtPath:PathForMetaDataFolder withIntermediateDirectories:NO attributes:nil error:nil];
    
    
    [PHPhotoLibrary requestAuthorization:^(PHAuthorizationStatus status) {
        if (status==PHAuthorizationStatusAuthorized) {
            dispatch_async(dispatch_get_main_queue(), ^{
                PHFetchResult*recentAlbum=[PHAssetCollection fetchAssetCollectionsWithType:PHAssetCollectionTypeSmartAlbum subtype:PHAssetCollectionSubtypeSmartAlbumRecentlyAdded options:nil];
                
                PHFetchResult*smartAlbums=[PHAssetCollection fetchAssetCollectionsWithType:PHAssetCollectionTypeSmartAlbum subtype:PHAssetCollectionSubtypeAny options:nil];
                
                PHFetchResult*albums=[PHAssetCollection fetchAssetCollectionsWithType:PHAssetCollectionTypeAlbum subtype:PHAssetCollectionSubtypeAny options:nil];
                
                unsigned long albumCount=recentAlbum.count+smartAlbums.count+albums.count;
                Albums=[NSMutableArray arrayWithCapacity:albumCount];
                AlbumAssets=[NSMutableArray arrayWithCapacity:albumCount];
                
                for (unsigned long i=0; i<recentAlbum.count; i++) {
                    [Albums addObject:[recentAlbum objectAtIndex:i]];
                }
                
                for (unsigned long i=0; i<smartAlbums.count; i++) {
                    PHAssetCollection*Album=[smartAlbums objectAtIndex:i];
                    PHAssetCollectionSubtype AlbumSubType=[Album assetCollectionSubtype];
                    if (AlbumSubType==PHAssetCollectionSubtypeAlbumRegular ||
                        AlbumSubType==PHAssetCollectionSubtypeAlbumSyncedAlbum ||
                        AlbumSubType==PHAssetCollectionSubtypeAlbumImported ||
                        AlbumSubType==PHAssetCollectionSubtypeAlbumCloudShared ||
                        AlbumSubType==PHAssetCollectionSubtypeSmartAlbumPanoramas ||
                        AlbumSubType==PHAssetCollectionSubtypeSmartAlbumFavorites) {
                        [Albums addObject:[smartAlbums objectAtIndex:i]];
                    }
                }
                
                for (unsigned long i=0; i<albums.count; i++) {
                    [Albums addObject:[albums objectAtIndex:i]];
                }
                wTimeStamp t1=wTimeStampGet();
                
                ImageManager=[PHImageManager defaultManager];
                ImageManagerOptions=[[PHImageRequestOptions alloc] init];
                ImageManagerOptions.synchronous=true;
                ImageManagerOptions.deliveryMode=PHImageRequestOptionsDeliveryModeFastFormat;
                ImageManagerOptions.resizeMode=PHImageRequestOptionsResizeModeFast;
                
                for (unsigned long i=0; i<Albums.count; i++) {
                    PHAssetCollection*Album=[Albums objectAtIndex:i];
                    
                    PHFetchResult*Assets=[PHAsset fetchAssetsInAssetCollection:Album options:nil];
                    NSMutableArray*Filtered=[NSMutableArray arrayWithCapacity:Assets.count];
                    for (long y=Assets.count-1; y>=0; y--) {
                        PHAsset*Asset=[Assets objectAtIndex:y];
                        if ([Asset mediaType]==PHAssetMediaTypeImage) {
                            [Filtered addObject:Asset];
                            CreateMetadataFileForAsset(Asset, false);
                        }
                    }
                    
                    [AlbumAssets addObject:Filtered];
                }
                wTimeStamp t2=wTimeStampGet();printf("%f\n", wTimerStampDifference(t1, t2));
            });
        }
        else{
            dispatch_async(dispatch_get_main_queue(), ^{
                [AM displayAlert:PKAlertType_Gallery_SavePhotosToITunes];
            });
        }
    }];
}

#import "PKGalleryLayer.h"
BOOL CreateMetadataFileForAsset(PHAsset*asset, BOOL override){

    NSString*identifier=[asset localIdentifier];
    NSString*path=pathForThumbnailFileForID(identifier);
    if ([[NSFileManager defaultManager] fileExistsAtPath:path] && !override)return false;
    if ([asset mediaType]!=PHAssetMediaTypeImage) {return false;}
    
    PKMeta*meta=PKMeta_NewMeta();
    
    /*
    unsigned int width=[asset pixelWidth];
    unsigned int height=[asset pixelHeight];
    CGSize mediumThumbnailSize;
    if (width>height) {mediumThumbnailSize=CGSizeMake(GLM.MinThumbnailSize*(width/height), GLM.MinThumbnailSize);}
    else{mediumThumbnailSize=CGSizeMake(GLM.MinThumbnailSize, GLM.MinThumbnailSize*(height/width));}
    */
    CGSize mediumThumbnailSize=CGSizeMake(GLM.MinThumbnailSize, GLM.MinThumbnailSize);
    [ImageManager requestImageForAsset:asset targetSize:mediumThumbnailSize contentMode:PHImageContentModeAspectFill options:ImageManagerOptions resultHandler:^(UIImage *result, NSDictionary *info) {
    
        CGImageRef image=result.CGImage;
        if (CGImageGetWidth(image)>0 && CGImageGetHeight(image)>0) {
            PKThumb*thumbnail=PKThumb_TumbnailFromCGImage(image);
            PKMeta_AddProperty(meta, PKMetaPropertyKey_Tumbnail_Medium, thumbnail[0].dataSize, thumbnail);
            free(thumbnail);
        }
    }];
    
    
    NSDateComponents*components;
    NSCalendar*calendar=[[NSCalendar alloc]
                             initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSCalendarUnit units=(NSCalendarUnitHour|NSCalendarUnitMinute|NSCalendarUnitSecond|
                          NSCalendarUnitDay|NSCalendarUnitMonth|NSCalendarUnitYear);
    
    wDate creationDate;
    components=[calendar components:units fromDate:[asset creationDate]];
    creationDate.Day=[components day];
    creationDate.Month=[components month];
    creationDate.Year=[components year];
    creationDate.Hour=[components hour];
    creationDate.Minute=[components minute];
    creationDate.Second=[components second];
    
    wDate lastModificationDate;
    components=[calendar components:units fromDate:[asset modificationDate]];
    lastModificationDate.Day=[components day];
    lastModificationDate.Month=[components month];
    lastModificationDate.Year=[components year];
    lastModificationDate.Hour=[components hour];
    lastModificationDate.Minute=[components minute];
    lastModificationDate.Second=[components second];
    
    PKLocation location;
    CLLocationCoordinate2D coordinates=[[asset location] coordinate];
    location.latitude=coordinates.latitude;
    location.longitude=coordinates.longitude;
    
    PKMetaData metaData={0};
    metaData.creationDate=creationDate;
    metaData.lastModificationDate=lastModificationDate;
    metaData.location=location;

    PKMeta_AddProperty(meta, PKMetaPropertyKey_MetaData, sizeof(PKMetaData), &metaData);
    PKMeta_AddProperty(meta, PKMetaPropertyKey_Identifier, identifier.length+1, (void*)identifier.UTF8String);

    void*FileData=PKMeta_SaveToData(meta);
    NSData*Thumbdata=[NSData dataWithBytes:FileData length:meta[0].fileSize];
    [Thumbdata writeToFile:path atomically:true];
 
    PKMeta_Release(meta);
    free(FileData);
 
    return true;
}

#pragma mark -
#pragma mark - PKMeta.

void*PKThumb_TumbnailFromCGImage(CGImageRef imageRef){

    unsigned int width=CGImageGetWidth(imageRef);
    unsigned int height=CGImageGetHeight(imageRef);
    unsigned int dataSize=(width*height*4)+sizeof(PKThumb);
    unsigned int headSize=sizeof(PKThumb);
    void*textureData=malloc(dataSize);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    NSUInteger bytesPerPixel = 4;
    NSUInteger bytesPerRow = bytesPerPixel * width;
    NSUInteger bitsPerComponent = 8;
    CGContextRef context = CGBitmapContextCreate(&textureData[headSize], width, height,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);

    CGColorSpaceRelease(colorSpace);
    CGContextClearRect(context, CGRectMake(0, 0, width, height));

    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    CGContextRelease(context);


    PKThumb*head=textureData;
    head[0].format=wTextureFormatRGBA;
    head[0].width=width;
    head[0].height=height;
    head[0].dataSize=dataSize;
    return textureData;
}



const char PKMeta_Signature[10]="PKMetaData";

bool PKMeta_FileIsAuthentic(void*fileData, unsigned int fileSize){
    bool result=false;
    if (fileSize>=sizeof(PKMeta)) {
        PKMeta*file=fileData;
        if (memcmp(file[0].signature, PKMeta_Signature,
                   sizeof(PKMeta_Signature))==0) {
            result=true;
        }
    }
    return result;
}

PKMeta*PKMeta_NewMeta(){
    PKMeta*meta=malloc(sizeof(PKMeta));
    meta[0].version=1.0;
    meta[0].propertyCount=0;
    meta[0].propertyCapacity=0;
    meta[0].properties=malloc(0);
    meta[0].fileSize=sizeof(PKMeta);
    memcpy(meta[0].signature, PKMeta_Signature, sizeof(PKMeta_Signature));
    return meta;
}

PKMeta*PKMeta_LoadFromData(void*fileData){
    unsigned int location=0;

    PKMeta*meta=malloc(sizeof(PKMeta));
    memcpy(meta, &fileData[location], sizeof(PKMeta));
    location+=sizeof(PKMeta);

    meta[0].properties=malloc(sizeof(PKMetaProperty)*meta[0].propertyCount);
    for (unsigned int i=0; i<meta[0].propertyCount; i++) {
        memcpy(&meta[0].properties[i], &fileData[location], sizeof(PKMetaProperty));
        location+=sizeof(PKMetaProperty);

        meta[0].properties[i].data=malloc(meta[0].properties[i].dataSize);
        memcpy(meta[0].properties[i].data, &fileData[location], meta[0].properties[i].dataSize);
        location+=meta[0].properties[i].dataSize;
    }

    return meta;
}


void*PKMeta_SaveToData(PKMeta*meta){
    void*data=malloc(meta[0].fileSize);
    unsigned int location=0;
    memcpy(&data[location], meta, sizeof(PKMeta));
    location+=sizeof(PKMeta);

    for (unsigned int i=0; i<meta[0].propertyCount; i++) {
        memcpy(&data[location],&meta[0].properties[i],sizeof(PKMetaProperty));
        location+=sizeof(PKMetaProperty);

        memcpy(&data[location], meta[0].properties[i].data, meta[0].properties[i].dataSize);
        location+=meta[0].properties[i].dataSize;
    }

    return data;
}

void PKMeta_AddProperty(PKMeta*meta, PKMetaPropertyKey key, unsigned int dataSize, void*data){

    bool keyExists=false;
    for (unsigned int i=0; i<meta[0].propertyCount; i++) {
        if (meta[0].properties[i].key==key) {

            meta[0].fileSize-=meta[0].properties[i].dataSize;
            free(meta[0].properties[i].data);
            meta[0].properties[i].data=NULL;

            meta[0].fileSize+=dataSize;
            meta[0].properties[i].dataSize=dataSize;
            meta[0].properties[i].data=malloc(dataSize);
            memcpy(meta[0].properties[i].data, data, dataSize);

            keyExists=true;
        }
    }

    if (!keyExists) {
        if (meta[0].propertyCount==meta[0].propertyCapacity) {
            meta[0].propertyCapacity=meta[0].propertyCount+5;
            meta[0].properties=realloc(meta[0].properties, sizeof(PKMetaProperty)*meta[0].propertyCapacity);
        }
        unsigned int index=meta[0].propertyCount;
        meta[0].propertyCount++;
        meta[0].properties[index].key=key;
        meta[0].properties[index].dataSize=dataSize;
        meta[0].properties[index].data=malloc(dataSize);
        memcpy(meta[0].properties[index].data, data, dataSize);

        meta[0].fileSize+=dataSize;
        meta[0].fileSize+=sizeof(PKMetaProperty);
    }
}

void PKMeta_RemoveProperty(PKMeta*meta, PKMetaPropertyKey key){
    bool found=false;
    unsigned int index=0;

    for (unsigned int i=0; i<meta[0].propertyCount; i++) {
        if (meta[0].properties[i].key==key) {
            index=i, found=true; break;
        }
    }

    if (found) {
        meta[0].fileSize-=meta[0].properties[index].dataSize;
        meta[0].fileSize-=sizeof(PKMetaProperty);
        free(meta[0].properties[index].data);
        meta[0].properties[index].data=NULL;

        memmove(&meta[0].properties[index], &meta[0].properties[index+1], sizeof(PKMetaProperty)*(meta[0].propertyCount-index-1));
        meta[0].propertyCount--;

        if (meta[0].propertyCapacity>(meta[0].propertyCount+10)) {
            meta[0].propertyCapacity=meta[0].propertyCount+5;
            meta[0].properties=realloc(meta[0].properties, sizeof(PKMetaProperty)*meta[0].propertyCapacity);
        }
    }
}

PKMetaProperty PKMeta_GetPropertyForKey(PKMetaPropertyKey key, PKMeta*thumb){
    PKMetaProperty property={0,0,NULL};
    PKMetaProperty*metaPtr=NULL;
    for (unsigned int i=0; i<thumb[0].propertyCount; i++) {
        metaPtr=&thumb[0].properties[i];
        if (metaPtr[0].key==key) {
            property=metaPtr[0];
            break;
        }
    }
    return property;
}

PKMetaProperty PKMeta_GetPropertyForKeyFromFile(PKMetaPropertyKey key, void*fileData){
    PKMetaProperty thumb={0,0,NULL};
    PKMeta*file=fileData;
    unsigned int location=sizeof(PKMeta);

    PKMetaProperty*thumbPtr=NULL;
    for (unsigned int i=0; i<file[0].propertyCount; i++) {
        thumbPtr=&fileData[location];
        location+=sizeof(PKMetaProperty);
        if (thumbPtr[0].key==key) {
            thumb=thumbPtr[0];
            thumb.data=&fileData[location];
            break;
        }
        else{
            location+=thumbPtr[0].dataSize;
        }
    }

    return thumb;
}

void PKMeta_Release(PKMeta*meta){
    for (unsigned int i=0; i<meta[0].propertyCount; i++) {
        if (meta[0].properties[i].data) {
            free(meta[0].properties[i].data);
            meta[0].properties[i].data=NULL;
        }
    }

    if (meta[0].properties) {
        free(meta[0].properties);
        meta[0].properties=NULL;
    }

    if (meta) {free(meta),meta=NULL;}
}

