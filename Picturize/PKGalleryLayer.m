//
//  PKGalleryFileManager.m
//  PhotoKit
//
//  Created by Wael Youssef on 6/28/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import "PKCamera.h"
#import "PKSettings.h"
#import "PKGalleryLayer.h"
#import "PKGalleryFileManager.h"
#import "PKGallery_TopBar.h"
#import "PKGallery_Canvas.h"

#pragma mark -
#pragma mark - GalleryLayer.
void GL_LoadAlbums(wDeviceOrientation orientation){
    wTableViewSetOffsetFromBottom(GL_AlbumGrid, 0);
    wRect frame=wViewGetFrame(GL_AlbumGrid);
    if (wDeviceOrientationIsPortrait(orientation)) {
        frame.size=wScreenGetPortraitFrame().size;
        wTableViewSetItemsPerRow(GL_AlbumGrid, GLM.AlbumsPerRow_Portrait);
    }
    else{
        frame.size=wScreenGetLandscapeFrame().size;
        wTableViewSetItemsPerRow(GL_AlbumGrid, GLM.AlbumsPerRow_Landscape);
    }
    wViewSetFrame(GL_AlbumGrid, frame);
    wTableViewLoad(GL_AlbumGrid);
}

void GL_LoadPhotos(wDeviceOrientation orientation){
    wTableViewSetOffsetFromBottom(GL_PhotoGrid, 0);
    wRect frame=wViewGetFrame(GL_PhotoGrid);
    if (wDeviceOrientationIsPortrait(orientation)) {
        frame.size=wScreenGetPortraitFrame().size;
        wTableViewSetItemsPerRow(GL_PhotoGrid, GLM.PhotosPerRow_Portrait);
    }
    else{
        frame.size=wScreenGetLandscapeFrame().size;
        wTableViewSetItemsPerRow(GL_PhotoGrid, GLM.PhotosPerRow_Landscape);
    }
    wViewSetFrame(GL_PhotoGrid, frame);
    wTableViewLoad(GL_PhotoGrid);
}

void GL_AdjustOrientation(wDeviceOrientation orientation){
    GL_LoadAlbums(orientation);
    if (!wViewGetIsHidden(GL_PhotoGrid)){
        GL_LoadPhotos(orientation);
    }else if (!wViewGetIsHidden(GL_Canvas)){
        GL_LoadPhotos(orientation);
    }
}

void GL_SetColorTheme(PKColorTheme colorTheme){
    wViewSetBackgroundColor(GL_TopBar_LV1_BackButton, PKForeColor);
    wViewSetBackgroundColor(GL_TopBar_LV1_SelectModeButton, PKForeColor);
    wViewSetBackgroundColor(GL_TopBar_LV2_DoneButton, PKForeColor);
    
    wColor borderColor=PKBarColor;
    borderColor.a=1.0;
    wViewSetBackgroundColor(GL_TopBar_Border, borderColor);

    wViewSetBackgroundColor(GL_TopBar, PKBarColor);
    wViewSetBackgroundColor(GL_Canvas_LeftButton, PKBarColor);
    wViewSetBackgroundColor(GL_Canvas_RightButton, PKBarColor);
    wViewSetBackgroundColor(GL_AlbumGrid, wColorNewGreyScaleFloat(0.9, 1.0));
    wViewSetBackgroundColor(GL_PhotoGrid, wColorNewGreyScaleFloat(0.9, 1.0));
    wViewSetBackgroundColor(GL_TopBar_LV1_Title, PKForeColor);
    
    wViewSetBackgroundColor(GL_Canvas_LeftButton, PKBarColor);
    wViewSetBackgroundColor(wArrayGetItemAtIndex(wViewGetSubviewArray(GL_Canvas_LeftButton), 0), PKForeColor);
    
    wViewSetBackgroundColor(GL_Canvas_RightButton, PKBarColor);
    wViewSetBackgroundColor(wArrayGetItemAtIndex(wViewGetSubviewArray(GL_Canvas_RightButton), 0), PKForeColor);
}


void GL_SetDisplayedView(void*view){
    wViewHide(GL_AlbumGrid);
    wViewHide(GL_PhotoGrid);
    wViewHide(GL_Canvas);
    wViewShow(view);
}

void GL_Display_End(){
    wViewHide(CameraLayer);
    CM_StopCamera();
}

void GL_Display(){
    wAutoRotationSetAnimateRotation(true);
    [iOSwOpenGLView overrideUserInputOrientation:false];
    
    wRect screenFrame=wScreenGetFrame();
    screenFrame.origin.x+=screenFrame.size.width;
    wViewSetFrame(GalleryLayer, screenFrame);
    screenFrame.origin.x-=screenFrame.size.width;
    
    GL_TopBar_SetTitle([NSString stringWithUTF8String:wStringGetCString(SM_GetTitleAtIndex(0))]);
    GL_TopBar_LV1_SelectModeButton_SetIsDisplayed(false);
    
    wViewShow(GalleryLayer);
    GL_SetDisplayedView(GL_AlbumGrid);
    wScrollViewSetContentOffset(GL_AlbumGrid, wPointZero);
    wTableViewSetItemCount(GL_AlbumGrid, Albums.count);
    
    GL_LoadAlbums(wDeviceGetOrientation());
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateFrame(GalleryLayer, screenFrame);
    wAnimationApplyContext();
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.5);
    wAnimationSetEndFunction(GL_Display_End, NULL);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateFrame(GL_AlbumGrid, screenFrame);
    wAnimationApplyContext();
}

#pragma mark AlbumGrid.
void GL_AlbumGrid_SelectAlbumAction(int index){
    SelectedAlbumIndex=index;
    wRect screenFrame=wScreenGetFrame();
    screenFrame.origin.x+=screenFrame.size.width;
    wViewSetFrame(GL_PhotoGrid, screenFrame);
    screenFrame.origin.x-=screenFrame.size.width;
    
    PHAssetCollection*Album=[Albums objectAtIndex:index];
    if ([Album canPerformEditOperation:PHCollectionEditOperationRemoveContent]) {
        SelectedAlbumDeleteMode=AlbumDeleteModeRemove;}
    else if ([Album canPerformEditOperation:PHCollectionEditOperationDeleteContent]){
        SelectedAlbumDeleteMode=AlbumDeleteModeDelete;}
    else{SelectedAlbumDeleteMode=AlbumDeleteModeNone;}
    
    SelectedAlbum=[AlbumAssets objectAtIndex:index];
    if (SelectedAlbum.count>0) {wViewHide(GL_AlbumGrid_EmptyLogo);}
    else{wViewShow(GL_AlbumGrid_EmptyLogo);}
    
    GL_TopBar_SetTitle([Album localizedTitle]);
    GL_TopBar_LV1_SelectModeButton_SetIsDisplayed(true);
    
    wScrollViewSetContentOffset(GL_PhotoGrid, wPointZero);
    wTableViewSetItemCount(GL_PhotoGrid, SelectedAlbum.count);
    GL_LoadPhotos(wDeviceGetOrientation());
    wViewShow(GL_PhotoGrid);
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetEndFunction(GL_SetDisplayedView, GL_PhotoGrid);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateFrame(GL_PhotoGrid, screenFrame);
    
    wAnimationApplyContext();
}

void GL_AlbumGrid_CleanAlbum(wView*album, int index){
    wArray*subviews=wViewGetSubviewArray(album);
    wView*imageView=wArrayGetItemAtIndex(subviews, 1);
    if ([[AlbumAssets objectAtIndex:wViewGetTag(album)] count]>0){
        wTextureRelease(wViewGetBackgroundImage(imageView));
    }
    wViewSetBackgroundImage(imageView, NULL);

    wView*titleView=wArrayGetItemAtIndex(subviews, 2);
    wTextureRelease(wViewGetBackgroundMask(titleView));
    wViewSetBackgroundMask(titleView, NULL);

    wView*counterView=wArrayGetItemAtIndex(subviews, 3);
    wTextureRelease(wViewGetBackgroundMask(counterView));
    wViewSetBackgroundMask(counterView, NULL);
    
    wView*iconView=wArrayGetItemAtIndex(subviews, 4);
    wViewHide(iconView);
}

wView*GL_AlbumGrid_ViewForAlbum(){
    
    wView*AlbumSlot=wViewNew();
    wView*imageFrame=wViewNew();
    wView*imageView=wViewNew();
    wView*titleView=wViewNew();
    wView*counterView=wViewNew();
    wView*iconView=wViewNew();
    wView*emptyIconView=wViewNew();
    
    wViewAddSubview(AlbumSlot, imageFrame);
    wViewAddSubview(AlbumSlot, imageView);
    wViewAddSubview(AlbumSlot, titleView);
    wViewAddSubview(AlbumSlot, counterView);
    wViewAddSubview(AlbumSlot, iconView);
    wViewAddSubview(AlbumSlot, emptyIconView);
    
    wViewSetBackgroundImage(imageFrame, GL_AlbumMask);
    wViewSetBackgroundMask(emptyIconView, GL_Album_EmptyTexture);
    wViewSetIsOpaque(titleView, false);
    wViewSetIsOpaque(counterView, false);
    wViewSetIsOpaque(iconView, false);
    wViewSetIsOpaque(emptyIconView, false);
    wViewHide(iconView);
    wViewHide(emptyIconView);
    
    wViewSetBackgroundColor(titleView,wColorBlack());
    wViewSetBackgroundColor(counterView, wColorBlack());
    wViewSetBackgroundColor(iconView, wColorBlack());
    wViewSetBackgroundColor(imageFrame, wColorBlack());

    wViewSetBackgroundImageDisplayMode(imageView, wViewBackgroundImageDisplayModeCrop);
    return AlbumSlot;
}

void GL_AlbumGrid_PresentAlbum(wView*album, int index){    
    wArray*subviews=wViewGetSubviewArray(album);
    wView*imageView=wArrayGetItemAtIndex(subviews, 1);
    wView*titleView=wArrayGetItemAtIndex(subviews, 2);
    wView*counterView=wArrayGetItemAtIndex(subviews, 3);
    wView*iconView=wArrayGetItemAtIndex(subviews, 4);
    wView*imageFrame=wArrayGetItemAtIndex(subviews, 0);
    wView*emptyIconView=wArrayGetItemAtIndex(subviews, 5);
    
    unsigned int assetsCount=[[AlbumAssets objectAtIndex:index] count];
    PHAssetCollection*Album=[Albums objectAtIndex:index];
    
    
    wTexture*iconTexture=NULL;
    switch ([Album assetCollectionSubtype]) {
        case PHAssetCollectionSubtypeAlbumSyncedEvent:
        case PHAssetCollectionSubtypeAlbumSyncedFaces:
        case PHAssetCollectionSubtypeAlbumSyncedAlbum:
            iconTexture=GL_Album_Icon_Synced;break;
        case PHAssetCollectionSubtypeSmartAlbumPanoramas:
            iconTexture=GL_Album_Icon_Panorama;break;
        case PHAssetCollectionSubtypeAlbumImported:
            iconTexture=GL_Album_Icon_Imported;break;
        case PHAssetCollectionSubtypeAlbumCloudShared:
            iconTexture=GL_Album_Icon_Cloud;break;
        case PHAssetCollectionSubtypeSmartAlbumFavorites:
            iconTexture=GL_Album_Icon_Favourite;break;
        case PHAssetCollectionSubtypeSmartAlbumRecentlyAdded:
            iconTexture=GL_Album_Icon_New;break;
        default:break;
    }
    

    
    if (assetsCount>0) {
        PHAsset*Asset=[[AlbumAssets objectAtIndex:index] objectAtIndex:0];
        
        CreateMetadataFileForAsset(Asset, false);
        
        NSString*path=pathForThumbnailFileForID([Asset localIdentifier]);
        NSData*metaData=[NSData dataWithContentsOfFile:path];
        PKMetaProperty thumb=PKMeta_GetPropertyForKeyFromFile(PKMetaPropertyKey_Tumbnail_Medium, (void*)metaData.bytes);
        
        PKThumb*header=thumb.data;
        wTexture*image=wTextureNew(&thumb.data[sizeof(PKThumb)], header[0].width, header[0].height, header[0].format);
        wViewSetBackgroundColor(imageView, wColorClear());
        wViewSetIsOpaque(imageView, true);
        wViewSetBackgroundImage(imageView, image);
        
        wViewHide(emptyIconView);
    }
    else{
        wViewSetBackgroundColor(imageView, PKBarColor);
        wViewSetIsOpaque(imageView, false);
        wViewSetBackgroundColor(emptyIconView, PKForeColor);
        wViewShow(emptyIconView);
    }

    wRect cellFrame=wViewGetFrame(album);
    wTexture*count=[iOSwTextureExt wTextureFromNSString:[NSString stringWithFormat:@"%d Photos",assetsCount] Font:GL_AlbumCounter_Font MaxWidth:cellFrame.size.width];
    wViewSetBackgroundMask(counterView, count);
    float widthOffset=0.0;
    if (iconTexture) {widthOffset=GLM.AlbumIconSize;}
    wRect counterFrame=wRectNew((cellFrame.size.width-(count[0].width+widthOffset))/2.0,
                                cellFrame.size.height-count[0].height,
                                count[0].width, count[0].height);
    wViewSetFrame(counterView, counterFrame);
    
    if (iconTexture) {
        wViewSetBackgroundMask(iconView, iconTexture);
        wRect iconFrame=wRectNew(counterFrame.origin.x+
                                 counterFrame.size.width+GLM.OutlineSize,
                                 counterFrame.origin.y+(counterFrame.size.height-GLM.AlbumIconSize),
                                 GLM.AlbumIconSize, GLM.AlbumIconSize);
        wViewSetFrame(iconView, iconFrame);
        wViewShow(iconView);
    }

    wTexture*title=[iOSwTextureExt wTextureFromNSString:[Album localizedTitle] Font:GL_Album_Font MaxWidth:cellFrame.size.width];
    wViewSetBackgroundMask(titleView, title);
    wRect titleFrame=wRectNew((cellFrame.size.width-title[0].width)/2.0,
                                cellFrame.size.height-title[0].height-count[0].height,
                                title[0].width, title[0].height);
    wViewSetFrame(titleView, titleFrame);
    
    
    float imageSize=cellFrame.size.height-(title[0].height+count[0].height);
    wRect imageRect=wRectNew((cellFrame.size.width-imageSize)/2.0, 0,
                              imageSize, imageSize);
    wViewSetFrame(imageFrame, imageRect);
    
    float borderSize=imageSize*0.05;
    imageRect=wRectNew(imageRect.origin.x+borderSize, borderSize, imageSize-(borderSize*2), imageSize-(borderSize*2));

    wViewSetFrame(imageView, imageRect);
    
    if (!wViewGetIsHidden(emptyIconView)) {
        wViewSetFrame(emptyIconView, wRectNew(floorf((cellFrame.size.width-GLM.AlbumEmptyIconSize)/2.0), floorf(((imageRect.size.height-GLM.AlbumEmptyIconSize)/2.0)+imageRect.origin.y), GLM.AlbumEmptyIconSize, GLM.AlbumEmptyIconSize));
    }else{
        wViewSetFrame(emptyIconView, wRectZero);
    }
}

#pragma mark PhotoGrid.
wView*GL_PhotoGrid_ViewForPhoto(){
    wView*photo=wViewNew();
    
    wView*SelectionBox=wViewNew();
    wViewAddSubview(photo, SelectionBox);
    wViewSetIsOpaque(SelectionBox, true);
    
    wViewSetBackgroundImageDisplayMode(photo, wViewBackgroundImageDisplayModeCrop);

    wViewSetFrame(SelectionBox, wRectNew(wViewGetFrame(photo).size.width-GLM.PhotoSelectorSize, 0, GLM.PhotoSelectorSize, GLM.PhotoSelectorSize));
    
    wAutoLayoutOptions opt=wAutoLayoutNone;
    opt.anchor.x=1.0;
    wViewSetAutoLayoutOptions(SelectionBox, opt);
    

    return photo;
}
bool GL_PhotoGrid_SelectPhotoAction(int index){
    
    if (isInSelectMode) {
        if (selectedFlags[index]) {SelectedPhotosCount--;
            PHAsset*Asset=[SelectedAlbum objectAtIndex:index];
            if ([Asset canPerformEditOperation:PHAssetEditOperationDelete]) {
                SelectedDeletablePhotosCount--;}}
        else{SelectedPhotosCount++;
            PHAsset*Asset=[SelectedAlbum objectAtIndex:index];
            if ([Asset canPerformEditOperation:PHAssetEditOperationDelete]) {
                SelectedDeletablePhotosCount++;}}
        
        if (SelectedAlbumDeleteMode==AlbumDeleteModeRemove) {
            if (SelectedPhotosCount>0) {
                GL_TopBar_LV2_DeleteButton_SetIsEnabled(true);}
            else{GL_TopBar_LV2_DeleteButton_SetIsEnabled(false);}}
        else if (SelectedAlbumDeleteMode==AlbumDeleteModeDelete) {
            if (SelectedDeletablePhotosCount>0) {
                GL_TopBar_LV2_DeleteButton_SetIsEnabled(true);}
            else{GL_TopBar_LV2_DeleteButton_SetIsEnabled(false);}}
        
        if (SelectedPhotosCount>0) {
            GL_TopBar_LV2_ShareButton_SetIsEnabled(true);}
        else{GL_TopBar_LV2_ShareButton_SetIsEnabled(false);}
        
        selectedFlags[index]=!selectedFlags[index];
        wTableViewUpdateItemAtIndex(GL_PhotoGrid, index);
        return true;
    }
    
    wRect screenFrame=wScreenGetFrame();
    screenFrame.origin.x+=screenFrame.size.width;
    wViewSetFrame(GL_Canvas, screenFrame);
    screenFrame.origin.x-=screenFrame.size.width;

    GL_Canvas_LoadAssetAtIndex(index);
    
    wViewShow(GL_Canvas);
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetEndFunction(GL_SetDisplayedView, GL_Canvas);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateFrame(GL_Canvas, screenFrame);
    wAnimationApplyContext();
    
    wAutoRotationSetAnimateResizing(true);
    
    return true;
}

void GL_PhotoGrid_CleanPhoto(wView*photo, int index){
    wTextureRelease(wViewGetBackgroundImage(photo));
    wViewSetBackgroundImage(photo, NULL);
}

void GL_PhotoGrid_PresentPhoto(wView*photo, int index){
    PHAsset*Asset=[SelectedAlbum objectAtIndex:index];
    
    CreateMetadataFileForAsset(Asset, false);

    NSString*path=pathForThumbnailFileForID([Asset localIdentifier]);
    NSData*metaData=[NSData dataWithContentsOfFile:path];
    PKMetaProperty thumb=PKMeta_GetPropertyForKeyFromFile(PKMetaPropertyKey_Tumbnail_Medium, (void*)metaData.bytes);
    
    PKThumb*header=thumb.data;
    wTexture*image=wTextureNew(&thumb.data[sizeof(PKThumb)], header[0].width, header[0].height, header[0].format);
    
    wViewSetBackgroundImage(photo, image);
    
    wView*selectionBox=wArrayGetItemAtIndex(wViewGetSubviewArray(photo), 0);
    if (isInSelectMode) {
        wViewShow(selectionBox);
        if (selectedFlags[index]) {
            wViewSetBackgroundImage(selectionBox, GL_SelectedMask);}
        else{wViewSetBackgroundImage(selectionBox, GL_NotSelectedMask);}}
    else{wViewHide(selectionBox);}
}

void GL_PhotoGrid_EnterSelectMode(){
    if (!isInSelectMode) {
        SelectedPhotosCount=0;
        SelectedDeletablePhotosCount=0;
        GL_TopBar_LV2_DeleteButton_SetIsEnabled(false);
        GL_TopBar_LV2_ShareButton_SetIsEnabled(false);
        isInSelectMode=true;
        unsigned int size=sizeof(bool)*wTableViewGetItemCount(GL_PhotoGrid);
        selectedFlags=malloc(size);
        memset(selectedFlags, false, size);
    }
}

void GL_PhotoGrid_EndSelectMode(){
    if (isInSelectMode) {
        isInSelectMode=false;
        if (selectedFlags) {free(selectedFlags),selectedFlags=NULL;}
    }
}

void GL_Setup(){
    GL_Album_Font=[iOSwTextureExt wFontNewFromUIFont:[UIFont fontWithName:@"HelveticaNeue-Light" size:GLM.AlbumFontSize]];
    GL_AlbumCounter_Font=[iOSwTextureExt wFontNewFromUIFont:[UIFont fontWithName:@"HelveticaNeue-Light" size:GLM.AlbumCounterFontSize]];

    wRect frame=wScreenGetFrame();
    frame.origin.x+=frame.size.width;
    
    GalleryLayer=wViewNewWithFrame(frame);
    wViewSetShouldDraw(GalleryLayer, false);
    wViewAddSubview(wRootView, GalleryLayer);
    wViewHide(GalleryLayer);
    
    GL_AlbumGrid=wTableViewNewWithFrame(frame);
    wViewAddSubview(GalleryLayer,GL_AlbumGrid);
    wTableViewSetItemsOutlineSize(GL_AlbumGrid, GLM.OutlineSize);
    wTableViewSetViewForItemAtIndexFunction(GL_AlbumGrid,
                                            GL_AlbumGrid_ViewForAlbum);
    wTableViewSetPresentItemAtIndexFunction(GL_AlbumGrid,
                                            GL_AlbumGrid_PresentAlbum);
    wTableViewSetCleanItemAtIndexFunction(GL_AlbumGrid,
                                          GL_AlbumGrid_CleanAlbum);
    wTableViewSetSelectedItemAtIndexFunction(GL_AlbumGrid,
                                             GL_AlbumGrid_SelectAlbumAction);
    wTableViewSetOffsetFromTop(GL_AlbumGrid, GLM.BarSize);
    wTableViewSetSquaredItems(GL_AlbumGrid, true);

    
    GL_PhotoGrid=wTableViewNewWithFrame(frame);
    wViewAddSubview(GalleryLayer,GL_PhotoGrid);
    wTableViewSetItemsOutlineSize(GL_PhotoGrid, GLM.OutlineSize);
    wTableViewSetViewForItemAtIndexFunction(GL_PhotoGrid, GL_PhotoGrid_ViewForPhoto);
    wTableViewSetPresentItemAtIndexFunction(GL_PhotoGrid, GL_PhotoGrid_PresentPhoto);
    wTableViewSetCleanItemAtIndexFunction(GL_PhotoGrid, GL_PhotoGrid_CleanPhoto);
    wTableViewSetSelectedItemAtIndexFunction(GL_PhotoGrid, GL_PhotoGrid_SelectPhotoAction);
    wTableViewSetOffsetFromTop(GL_PhotoGrid, GLM.BarSize);
    wViewHide(GL_PhotoGrid);
    wTableViewSetSquaredItems(GL_PhotoGrid, true);
    
    NSString*Suffix=nil;
    NSString*maskName=nil;
    NSString*albumEmptyName=nil;
    NSString*photoEmptyName=nil;
    switch (deviceType) {
        case deviceType_iPhone:{
            Suffix=@"25.png";
            maskName=@"GL_AlbumMask100.png";
            albumEmptyName=@"GL_Album_Empty50.png";
            photoEmptyName=@"GL_Photo_Empty100.png";break;}
        case deviceType_iPhoneRetina:{
            Suffix=@"50.png";
            maskName=@"GL_AlbumMask200.png";
            albumEmptyName=@"GL_Album_Empty100.png";
            photoEmptyName=@"GL_Photo_Empty200.png";break;}
        case deviceType_iPad:{
            Suffix=@"30.png";
            maskName=@"GL_AlbumMask130.png";
            albumEmptyName=@"GL_Album_Empty80.png";
            photoEmptyName=@"GL_Photo_Empty160.png";break;}
        case deviceType_iPadRetina:{
            Suffix=@"60.png";
            maskName=@"GL_AlbumMask260.png";
            albumEmptyName=@"GL_Album_Empty160.png";
            photoEmptyName=@"GL_Photo_Empty320.png";break;}
        default:{
            Suffix=@"30.png";
            maskName=@"GL_AlbumMask200.png";
            albumEmptyName=@"GL_Album_Empty100.png";
            photoEmptyName=@"GL_Photo_Empty200.png";break;}
    }
    GL_AlbumMask=[iOSwTextureExt wTextureFromImageWithName:maskName];
    GL_SelectedMask=[iOSwTextureExt wTextureFromImageWithName:[@"GL_Selected" stringByAppendingString:Suffix]];
    GL_NotSelectedMask=[iOSwTextureExt wTextureFromImageWithName:[@"GL_UnSelected" stringByAppendingString:Suffix]];
    GL_Album_EmptyTexture=[iOSwTextureExt wTextureFromImageWithName:albumEmptyName];

    
    wAutoLayoutOptions layoutOpt=wAutoLayoutNone;
    layoutOpt.widthOption=wAutoLayoutStretchToSuperView;
    layoutOpt.heightOption=wAutoLayoutStretchToSuperView;
    wViewSetAutoLayoutOptions(GalleryLayer, layoutOpt);
    
    GL_AlbumGrid_EmptyLogo=wViewNewWithFrame(wRectNew((wScreenGetWidth()-(GLM.AlbumEmptyIconSize*2))/2.0, (wScreenGetHeight()-(GLM.AlbumEmptyIconSize*2))/2.0, GLM.AlbumEmptyIconSize*2, GLM.AlbumEmptyIconSize*2));
    wViewSetIsOpaque(GL_AlbumGrid_EmptyLogo, false);
    wViewSetBackgroundMask(GL_AlbumGrid_EmptyLogo, [iOSwTextureExt wTextureFromImageWithName:photoEmptyName]);
    wViewSetBackgroundColor(GL_AlbumGrid_EmptyLogo, wColorBlack());
    wViewAddSubview(GL_PhotoGrid, GL_AlbumGrid_EmptyLogo);
    wAutoLayoutOptions GL_AlbumGrid_EmptyLogoLOOpt=wAutoLayoutNone;
    GL_AlbumGrid_EmptyLogoLOOpt.anchor.x=0.5;
    GL_AlbumGrid_EmptyLogoLOOpt.anchor.y=0.5;
    wViewSetAutoLayoutOptions(GL_AlbumGrid_EmptyLogo, GL_AlbumGrid_EmptyLogoLOOpt);
    
    GL_Canvas_Create();
    GL_TopBar_Create();
   // GL_BottomBar_Create();
    
    
    Suffix=nil;
    switch (deviceType) {
        case deviceType_iPhone:case deviceType_iPad:{
            Suffix=@"20.png";break;}
        case deviceType_iPhoneRetina:case deviceType_iPadRetina:{
            Suffix=@"40.png";break;}
        default:{Suffix=@"40.png";break;}
    }
    GL_Album_Icon_Synced=[iOSwTextureExt wTextureFromImageWithName:[@"GL_Album_Icon_Sync" stringByAppendingString:Suffix]];
    GL_Album_Icon_Cloud=[iOSwTextureExt wTextureFromImageWithName:[@"GL_Album_Icon_Cloud" stringByAppendingString:Suffix]];
    GL_Album_Icon_Favourite=[iOSwTextureExt wTextureFromImageWithName:[@"GL_Album_Icon_Favourite" stringByAppendingString:Suffix]];
    GL_Album_Icon_New=[iOSwTextureExt wTextureFromImageWithName:[@"GL_Album_Icon_New" stringByAppendingString:Suffix]];
    GL_Album_Icon_Panorama=[iOSwTextureExt wTextureFromImageWithName:[@"GL_Album_Icon_Panorama" stringByAppendingString:Suffix]];
    GL_Album_Icon_Imported=[iOSwTextureExt wTextureFromImageWithName:[@"GL_Album_Icon_Imported" stringByAppendingString:Suffix]];
}
