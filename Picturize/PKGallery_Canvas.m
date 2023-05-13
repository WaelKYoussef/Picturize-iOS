//
//  PKGallery_Canvas.m
//  PhotoKit
//
//  Created by Wael on 8/26/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import "PKGallery_Canvas.h"
#import "PKGalleryLayer.h"
#import "PKGallery_TopBar.h"
#import "PKGalleryFileManager.h"


wSize ImageOriginalSize;
wSize ImageMinSize;
wSize ImageMidSize;
wSize ImageMaxSize;
wRect ImageMinPortraitFrame;
wRect ImageMinLandscapeFrame;
wAutoLayoutOptions ImageAutoLayoutOptions;

wRect fitImageOfSizeToFrame(wSize imageSize, wRect frame){
    
    wRect result;
    if (frame.size.width>frame.size.height) {
        float CanvasRatio=frame.size.width/frame.size.height;
        float ImageRatio=imageSize.width/imageSize.height;
        if (ImageRatio>CanvasRatio) {
            result.size.width=frame.size.width;
            result.size.height=(imageSize.height*frame.size.width)/imageSize.width;
        }
        else{
            result.size.height=frame.size.height;
            result.size.width=(imageSize.width*frame.size.height)/imageSize.height;
        }
    }
    else{
        float CanvasRatio=frame.size.height/frame.size.width;
        float ImageRatio=imageSize.height/imageSize.width;
        if (CanvasRatio>ImageRatio) {
            result.size.width=frame.size.width;
            result.size.height=(imageSize.height*frame.size.width)/imageSize.width;
        }
        else{
            result.size.height=frame.size.height;
            result.size.width=(imageSize.width*frame.size.height)/imageSize.height;
        }
    }
    result.origin.x=(frame.size.width-result.size.width)/2.0;
    result.origin.y=(frame.size.height-result.size.height)/2.0;
    return result;
}

wRect snapImageToCanvas(wRect imageFrame,wRect canvasFrame){
    wRect result=imageFrame;
    
    if (imageFrame.size.height<canvasFrame.size.height) {
        result.origin.y=(canvasFrame.size.height-imageFrame.size.height)/2.0;
    }
    else{
        if (imageFrame.origin.y>0.0) {result.origin.y=0.0;}
        else if ((imageFrame.origin.y+imageFrame.size.height)<canvasFrame.size.height) {
            result.origin.y=(0.0-imageFrame.size.height+canvasFrame.size.height);}
    }
    
    
    if (imageFrame.size.width<canvasFrame.size.width) {
        result.origin.x=(canvasFrame.size.width-imageFrame.size.width)/2.0;
    }
    else{
        if (imageFrame.origin.x>0.0) {result.origin.x=0.0;}
        else if ((imageFrame.origin.x+imageFrame.size.width)<canvasFrame.size.width) {
            result.origin.x=(0.0-imageFrame.size.width+canvasFrame.size.width);}
    }
    
    return result;
}

wAutoLayoutOptions layoutOptionsForFrame(wSize imageSize, wSize canvasSize){
    wAutoLayoutOptions options;
    options.anchor.x=0.5,options.anchor.y=0.5;
    
    if (imageSize.width==canvasSize.width) {
        options.widthOption=wAutoLayoutStretchToSuperView;
        options.heightOption=wAutoLayoutProportionateToWidth;
    }
    else if (imageSize.height==canvasSize.height) {
        options.widthOption=wAutoLayoutProportionateToHeight;
        options.heightOption=wAutoLayoutStretchToSuperView;
    }
    
    return options;
}

void GL_Canvas_LoadPhotoFromAsset(PHAsset*Asset){
    GL_TopBar_LV2_ShareButton_SetIsEnabled(true);
    switch (SelectedAlbumDeleteMode) {
        case AlbumDeleteModeRemove:{GL_TopBar_LV2_DeleteButton_SetIsEnabled(true);break;}
        case AlbumDeleteModeDelete:{
            if ([Asset canPerformEditOperation:PHAssetEditOperationDelete]) {
                GL_TopBar_LV2_DeleteButton_SetIsEnabled(true);}
            else{GL_TopBar_LV2_DeleteButton_SetIsEnabled(false);}break;}
        case AlbumDeleteModeNone:{GL_TopBar_LV2_DeleteButton_SetIsEnabled(false);break;}
        default:break;
    }
    
    PHImageRequestOptions*opt=[[PHImageRequestOptions alloc] init];
    opt.synchronous=true;
    opt.deliveryMode=PHImageRequestOptionsDeliveryModeFastFormat;
    opt.resizeMode=PHImageRequestOptionsResizeModeFast;
    
    GLint textureLimit;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &textureLimit);
    unsigned long width=[Asset pixelWidth];
    unsigned long height=[Asset pixelHeight];
    CGSize targetSize=CGSizeMake((CGFloat)width, (CGFloat)height);
    if (width>height) {if (width>textureLimit) {targetSize.height=(CGFloat)textureLimit*(targetSize.height/targetSize.width);targetSize.width=(CGFloat)textureLimit;}}
    else{if (height>textureLimit) {targetSize.width=(CGFloat)textureLimit*(targetSize.width/targetSize.height);targetSize.height=(CGFloat)textureLimit;}}
    
    
    [[PHImageManager defaultManager] requestImageForAsset:Asset targetSize:targetSize contentMode:PHImageContentModeAspectFill options:opt resultHandler:^(UIImage *result, NSDictionary *info) {
//        wTexture*image=[iOSwTextureExt wTextureFromUIImage:result];
//        wViewSetBackgroundImage(GL_Canvas_Image, image);
        
            wTexture*image=[iOSwTextureExt wTextureFromUIImage:result];
            wViewSetBackgroundImage(GL_Canvas_Image, image);
       
    }];

    wSize imageSize=wSizeNew([Asset pixelWidth], [Asset pixelHeight]);
    
    ImageOriginalSize=imageSize;
    ImageMinPortraitFrame=fitImageOfSizeToFrame(imageSize, wScreenGetPortraitFrame());
    ImageMinLandscapeFrame=fitImageOfSizeToFrame(imageSize, wScreenGetLandscapeFrame());
    
    if (imageSize.width>imageSize.height) {
        float ImageRatio=imageSize.width/imageSize.height;
        
        if (ImageRatio>2.0) {
            float maxHeight=wScreenGetPortraitFrame().size.height;
            ImageMaxSize=wSizeNew(maxHeight*ImageRatio, maxHeight);
        }
        else{
            float maxHeight=wScreenGetPortraitFrame().size.height*1.5;
            ImageMaxSize=wSizeNew(maxHeight*ImageRatio, maxHeight);
        }
        ImageMinSize=wSizeNew(ImageMinPortraitFrame.size.width*0.5, ImageMinPortraitFrame.size.height*0.5);
    }
    else{
        float  ImageRatio=imageSize.height/imageSize.width;
        
        if (ImageRatio>2.0) {
            float maxWidth=wScreenGetLandscapeFrame().size.width;
            ImageMaxSize=wSizeNew(maxWidth, maxWidth*ImageRatio);
        }
        else{
            float maxWidth=wScreenGetPortraitFrame().size.height*1.5;
            ImageMaxSize=wSizeNew(maxWidth, maxWidth*ImageRatio);
        }
        ImageMinSize=wSizeNew(ImageMinLandscapeFrame.size.width*0.5, ImageMinLandscapeFrame.size.height*0.5);
    }
    ImageMidSize=wSizeNew(ImageMaxSize.width*0.5, ImageMaxSize.height*0.5);
    
    
    
    if (wDeviceOrientationIsPortrait(wDeviceGetOrientation())) {
        ImageAutoLayoutOptions=layoutOptionsForFrame(ImageMinPortraitFrame.size, wScreenGetPortraitFrame().size);
        wViewSetFrame(GL_Canvas_Image, ImageMinPortraitFrame);
    }
    else{
        ImageAutoLayoutOptions=layoutOptionsForFrame(ImageMinLandscapeFrame.size, wScreenGetLandscapeFrame().size);
        
        wViewSetFrame(GL_Canvas_Image, ImageMinLandscapeFrame);
    }
    wViewSetAutoLayoutOptions(GL_Canvas_Image, ImageAutoLayoutOptions);
}



void GL_Canvas_LoadAssetAtIndex(unsigned int index){
    IndexOfLoadedAsset=index;
    PHAsset*Asset=[SelectedAlbum objectAtIndex:index];
    
    wTexture*image=wViewGetBackgroundImage(GL_Canvas_Image);
    if (image) {wTextureRelease(image);wViewSetBackgroundImage(GL_Canvas_Image, nil);}
    
    GL_Canvas_LoadPhotoFromAsset(Asset);
}


typedef char HideButtonOption;
#define HideButtonOptionLeft 0
#define HideButtonOptionRight 1
#define HideButtonOptionBoth 2

void GL_Canvas_Navigation_Show(HideButtonOption option){
    
    BOOL Left,Right;
    switch (option) {
    case HideButtonOptionLeft:{Left=true,Right=false;break;}
    case HideButtonOptionRight:{Left=false,Right=true;break;}
    case HideButtonOptionBoth:{Left=true,Right=true;break;}
    default:{Left=true,Right=true;break;}}
    
    wRect screenFrame=wScreenGetFrame();
    float buttonSize=GLM.BarButtonSize+(GLM.OutlineSize*2);
    float buttonY=(screenFrame.size.height-buttonSize)/2.0;
    
    wRect leftFrame;
    if (Left) {
    leftFrame=wRectNew(-buttonSize, buttonY,buttonSize,buttonSize);
    wViewSetFrame(GL_Canvas_LeftButton, leftFrame);
    leftFrame.origin.x+=leftFrame.size.width;}
    
    wRect rightFrame;
    if (Right) {
    rightFrame=wRectNew(screenFrame.size.width, buttonY, buttonSize, buttonSize);
    wViewSetFrame(GL_Canvas_RightButton, rightFrame);
    rightFrame.origin.x-=rightFrame.size.width;}
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    if (Left){wViewAnimateFrame(GL_Canvas_LeftButton, leftFrame);}
    if (Right){wViewAnimateFrame(GL_Canvas_RightButton, rightFrame);}
    wAnimationApplyContext();
}

void GL_Canvas_Navigation_Hide(HideButtonOption option){
    BOOL Left,Right;
    switch (option) {
    case HideButtonOptionLeft:{Left=true,Right=false;break;}
    case HideButtonOptionRight:{Left=false,Right=true;break;}
    case HideButtonOptionBoth:{Left=true,Right=true;break;}
    default:{Left=true,Right=true;break;}}
    
    wRect screenFrame=wScreenGetFrame();
    float buttonSize=GLM.BarButtonSize+(GLM.OutlineSize*2);
    float buttonY=(screenFrame.size.height-buttonSize)/2.0;
    
    wRect leftFrame;
    if (Left){
    leftFrame=wRectNew(0, buttonY,buttonSize,buttonSize);
    wViewSetFrame(GL_Canvas_LeftButton, leftFrame);
    leftFrame.origin.x-=leftFrame.size.width;}
    
    wRect rightFrame;
    if (Right){
    rightFrame=wRectNew(screenFrame.size.width-GLM.BarButtonSize, buttonY, buttonSize, buttonSize);
    wViewSetFrame(GL_Canvas_RightButton, rightFrame);
    rightFrame.origin.x+=rightFrame.size.width;}
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    if (Left){wViewAnimateFrame(GL_Canvas_LeftButton, leftFrame);}
    if (Right){wViewAnimateFrame(GL_Canvas_RightButton, rightFrame);}
    wAnimationApplyContext();
}

void GL_Canvas_EndPhotoTransition(){
    wTexture*image=wViewGetBackgroundImage(GL_Canvas_Image_Temp);
    if (image) {
        wTextureRelease(image);
        wViewSetBackgroundImage(GL_Canvas_Image_Temp, nil);
    }
    wViewHide(GL_Canvas_Image_Temp);
}
void GL_Canvas_LoadNextPhoto(){
    unsigned int newIndex=IndexOfLoadedAsset+1;
    if (newIndex>SelectedAlbum.count-1) {newIndex=0;}
    IndexOfLoadedAsset=newIndex;
    
    wRect currentFrame=wViewGetFrame(GL_Canvas_Image);
    wTexture*currentPhoto=wViewGetBackgroundImage(GL_Canvas_Image);
    
    PHAsset*Asset=[SelectedAlbum objectAtIndex:newIndex];
    GL_Canvas_LoadPhotoFromAsset(Asset);
    
    wRect photoRect=wViewGetFrame(GL_Canvas_Image);
    photoRect.origin.x=currentFrame.origin.x+currentFrame.size.width+GLM.BarSize;
    wViewSetFrame(GL_Canvas_Image, photoRect);
    photoRect.origin.x=(wScreenGetFrame().size.width-photoRect.size.width)/2.0;
    
    wViewSetFrame(GL_Canvas_Image_Temp, currentFrame);
    wViewSetBackgroundImage(GL_Canvas_Image_Temp, currentPhoto);
    wViewShow(GL_Canvas_Image_Temp);
    currentFrame.origin.x=(-currentFrame.size.width-GLM.BarSize);
    
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wAnimationSetEndFunction(GL_Canvas_EndPhotoTransition, NULL);
    wViewAnimateFrame(GL_Canvas_Image, photoRect);
    wViewAnimateFrame(GL_Canvas_Image_Temp, currentFrame);
    wAnimationApplyContext();
}

void GL_Canvas_LoadPreviousPhoto(){
    unsigned int newIndex=IndexOfLoadedAsset-1;
    if (newIndex==InvalidIndex) {newIndex=SelectedAlbum.count-1;}
    IndexOfLoadedAsset=newIndex;
    
    wRect currentFrame=wViewGetFrame(GL_Canvas_Image);
    wTexture*currentPhoto=wViewGetBackgroundImage(GL_Canvas_Image);
    
    PHAsset*Asset=[SelectedAlbum objectAtIndex:newIndex];
    GL_Canvas_LoadPhotoFromAsset(Asset);
    
    wRect photoRect=wViewGetFrame(GL_Canvas_Image);
    photoRect.origin.x=currentFrame.origin.x-photoRect.size.width-GLM.BarSize;
    wViewSetFrame(GL_Canvas_Image, photoRect);
    photoRect.origin.x=(wScreenGetFrame().size.width-photoRect.size.width)/2.0;
    
    wViewSetFrame(GL_Canvas_Image_Temp, currentFrame);
    wViewSetBackgroundImage(GL_Canvas_Image_Temp, currentPhoto);
    wViewShow(GL_Canvas_Image_Temp);
    currentFrame.origin.x=wViewGetFrame(GL_Canvas).size.width+GLM.BarSize;
    
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wAnimationSetEndFunction(GL_Canvas_EndPhotoTransition, NULL);
    wViewAnimateFrame(GL_Canvas_Image, photoRect);
    wViewAnimateFrame(GL_Canvas_Image_Temp, currentFrame);
    wAnimationApplyContext();
}


BOOL barsAreHidden=false;
void GL_HideAndShowBars(wGesture*gesture){
    if (barsAreHidden) {
        wSize canvasSize=wViewGetFrame(GL_Canvas).size;
        wRect LeftFrame=wRectNew(0, 0, GLM.BarButtonSize, canvasSize.height);
        wRect RightFrame=LeftFrame;
        RightFrame.origin.x=canvasSize.width-GLM.BarButtonSize;
        wPoint location=wGestureGetLocationAtIndex(gesture, 0);
        if (wRectContainsPoint(LeftFrame, location)) {
            GL_Canvas_LoadPreviousPhoto();
        }
        else if (wRectContainsPoint(RightFrame, location)){
            GL_Canvas_LoadNextPhoto();
        }else{
            barsAreHidden=false;
            GL_TopBar_Show();
            GL_Canvas_Navigation_Show(HideButtonOptionBoth);
        }
    }
    else{
        barsAreHidden=true;
        GL_TopBar_Hide();
        GL_Canvas_Navigation_Hide(HideButtonOptionBoth);
    }
}

float previous=1.0;
wPoint pCenter;
int touchcount=0;

wPoint pA;
wPoint pB;

BOOL shouldDrag;
BOOL shouldZoom;

void GL_StartZoom(wGesture*gesture){
    touchcount++;
    if (touchcount==1) {
        shouldDrag=true;
        shouldZoom=false;
        pA=wGestureGetLocationAtIndex(gesture, 0);
    }else if (touchcount==2) {
        shouldDrag=false;
        shouldZoom=true;
        pB=wGestureGetLocationAtIndex(gesture, 0);
        previous=wPointDistanceFromPoint(pA, pB);
        pCenter=wPointNew((pA.x+pB.x)/2.0, (pA.y+pB.y)/2.0);
    }else if (touchcount>2) {
        touchcount=2;
        shouldDrag=false;
        shouldZoom=true;
    }
}

void GL_Zoom(wGesture*gesture){
    if (shouldZoom) {
        
        float current=wPointDistanceFromPoint(wGestureGetLocationAtIndex(gesture, 0), wGestureGetLocationAtIndex(gesture, 1));
        
        float r=current/previous;
        
        pA=wGestureGetLocationAtIndex(gesture, 0);
        pB=wGestureGetLocationAtIndex(gesture, 1);
        wPoint cCenter=wPointNew((pA.x+pB.x)/2.0, (pA.y+pB.y)/2.0);
        
        wRect frame=wViewGetFrame(GL_Canvas_Image);
        float newWidth=frame.size.width*r;
        if (newWidth>ImageMaxSize.width) {newWidth=ImageMaxSize.width;}
        else if (newWidth<ImageMinSize.width) {newWidth=ImageMinSize.width;}
        float ratio=newWidth/frame.size.width;
        
        float minX=frame.origin.x;
        float minY=frame.origin.y;
        float maxX=frame.origin.x+frame.size.width;
        float maxY=frame.origin.y+frame.size.height;
        
        minX=((minX-pCenter.x)*ratio)+pCenter.x;
        minY=((minY-pCenter.y)*ratio)+pCenter.y;
        maxX=((maxX-pCenter.x)*ratio)+pCenter.x;
        maxY=((maxY-pCenter.y)*ratio)+pCenter.y;
        
        frame=wRectNew(minX, minY, maxX-minX, maxY-minY);
        frame.origin=wPointAddPoints(frame.origin, wPointNew(cCenter.x-pCenter.x, cCenter.y-pCenter.y));
        
        wViewSetFrame(GL_Canvas_Image, frame);
        previous=current;
        pCenter=cCenter;
    }
}

void GL_EndZoom(wGesture*gesture){
    touchcount--;
    if (touchcount==0) {

        wRect OriginalFrame;
        if (wDeviceOrientationIsPortrait(wDeviceGetOrientation())) {
            OriginalFrame=ImageMinPortraitFrame;}
        else{OriginalFrame=ImageMinLandscapeFrame;}
        
        wRect currentFrame=wViewGetFrame(GL_Canvas_Image);
        wRect targetFrame=currentFrame;
        if (currentFrame.size.width<OriginalFrame.size.width) {
            targetFrame=OriginalFrame;
        }
        else{
            wRect canvasFrame=wScreenGetFrame();
            targetFrame=snapImageToCanvas(currentFrame,canvasFrame);
        }
        
        if (!wRectIsEqualToRect(targetFrame, currentFrame)) {
            wAnimationBeginContext();
            wAnimationSetDuration(0.3);
            wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
            wViewAnimateFrame(GL_Canvas_Image, targetFrame);
            wAnimationApplyContext();
        }

    }else if (touchcount==1) {
        wPoint point=wGestureGetLocationAtIndex(gesture, 0);
        if (wPointDistanceFromPoint(pB, point)>wPointDistanceFromPoint(pA, point)) {
            pA=pB;
        }
        shouldDrag=true;
        shouldZoom=false;
    }else if (touchcount==2){
        shouldDrag=false;
        shouldZoom=true;
    }else if (touchcount>2){
        touchcount=2;
        shouldDrag=false;
        shouldZoom=true;
    }
}


void GL_Drag(wGesture*gesture){
    if (shouldDrag) {
        wPoint cdragpoint=wGestureGetLocationAtIndex(gesture, 0);
        
        wRect frame=wViewGetFrame(GL_Canvas_Image);
        
        frame.origin=wPointAddPoints(frame.origin, wPointNew(cdragpoint.x-pA.x, cdragpoint.y-pA.y));
        
        wViewSetFrame(GL_Canvas_Image, frame);
        pA=cdragpoint;
    }
}

void GL_TapToZoom(wGesture*gesture){
    
    wRect OriginalFrame;
    if (wDeviceOrientationIsPortrait(wDeviceGetOrientation())) {
        OriginalFrame=ImageMinPortraitFrame;}
    else{OriginalFrame=ImageMinLandscapeFrame;}
    
    wRect targetFrame=OriginalFrame;
    wRect currentFrame=wViewGetFrame(GL_Canvas_Image);
    float targetRatio=0.0;
    float currentRatio=(ImageMidSize.width/currentFrame.size.width);
    if (currentRatio>1.0) {targetRatio=currentRatio;}
    else{currentRatio=(ImageMaxSize.width/currentFrame.size.width);
        if (currentRatio>1.0) {targetRatio=currentRatio;}}
    
    if (targetRatio!=0.0) {
        wPoint center=wGestureGetLocationAtIndex(gesture, 0);
        float minX=currentFrame.origin.x;
        float minY=currentFrame.origin.y;
        float maxX=currentFrame.origin.x+currentFrame.size.width;
        float maxY=currentFrame.origin.y+currentFrame.size.height;
        
        minX=((minX-center.x)*targetRatio)+center.x;
        minY=((minY-center.y)*targetRatio)+center.y;
        maxX=((maxX-center.x)*targetRatio)+center.x;
        maxY=((maxY-center.y)*targetRatio)+center.y;
        
        targetFrame=wRectNew(minX, minY, maxX-minX, maxY-minY);
        targetFrame=snapImageToCanvas(targetFrame, wScreenGetFrame());
    }
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateFrame(GL_Canvas_Image, targetFrame);
    wAnimationApplyContext();
}





void GL_Canvas_Create(){
    wRect screenFrame=wScreenGetFrame();
    GL_Canvas=wViewNewWithFrame(screenFrame);
    wViewAddSubview(GalleryLayer, GL_Canvas);
    wViewSetBackgroundColor(GL_Canvas, wColorBlack());
    wViewSetClipsSubviews(GL_Canvas, true);
    wViewSetUserInteractionEnabled(GL_Canvas, true);
    wViewSetMultiTouchEnabled(GL_Canvas, true);
    wAutoLayoutOptions layoutOpt=wAutoLayoutNone;
    layoutOpt.widthOption=wAutoLayoutStretchToSuperView;
    layoutOpt.heightOption=wAutoLayoutStretchToSuperView;
    wViewSetAutoLayoutOptions(GL_Canvas, layoutOpt);
    wViewHide(GL_Canvas);
    
    wGesture*tap=wGestureNew(wGestureTap);
    wGestureSetFunction(tap, GL_HideAndShowBars);
    wViewAddGesture(GL_Canvas, tap);
    
    wGesture*doupleTap=wGestureNew(wGestureDoubleTap);
    wGestureSetFunction(doupleTap, GL_TapToZoom);
    wViewAddGesture(GL_Canvas, doupleTap);
    
    wGesture*drag=wGestureNew(wGestureSwipe);
    wGestureSetFunction(drag, GL_Drag);
    wViewAddGesture(GL_Canvas, drag);
    
    wGesture*startPan=wGestureNew(wGestureTouchDown);
    wGestureSetFunction(startPan, GL_StartZoom);
    wViewAddGesture(GL_Canvas, startPan);
    
    wGesture*pan=wGestureNew(wGestureDoubleSwipe);
    wGestureSetFunction(pan, GL_Zoom);
    wViewAddGesture(GL_Canvas, pan);
    
    wGesture*EndPan=wGestureNew(wGestureTouchUp);
    wGestureSetFunction(EndPan, GL_EndZoom);
    wViewAddGesture(GL_Canvas, EndPan);
    
    GL_Canvas_Image=wViewNew();
    wViewAddSubview(GL_Canvas, GL_Canvas_Image);
    GL_Canvas_Image_Temp=wViewNew();
    wViewHide(GL_Canvas_Image_Temp);
    wViewAddSubview(GL_Canvas, GL_Canvas_Image_Temp);
    
    float buttonSize=GLM.BarButtonSize+(GLM.OutlineSize*2);
    float buttonY=(screenFrame.size.height-buttonSize)/2.0;
    wAutoLayoutOptions options=wAutoLayoutNone;
    options.anchor.y=0.5;
    GL_Canvas_LeftButton=wButtonNewWithFrame(wRectNew(0, buttonY,buttonSize,buttonSize));
    wViewSetAutoLayoutOptions(GL_Canvas_LeftButton, options);
    wButtonSetFunction(GL_Canvas_LeftButton, GL_Canvas_LoadPreviousPhoto);
    wViewAddSubview(GL_Canvas, GL_Canvas_LeftButton);
    
    options.anchor.x=1.0;
    GL_Canvas_RightButton=wButtonNewWithFrame(wRectNew(screenFrame.size.width-buttonSize, buttonY, buttonSize, buttonSize));
    wViewSetAutoLayoutOptions(GL_Canvas_RightButton, options);
    wButtonSetFunction(GL_Canvas_RightButton, GL_Canvas_LoadNextPhoto);
    wViewAddSubview(GL_Canvas, GL_Canvas_RightButton);
    
    
    NSString*Suffix=nil;
    switch (deviceType) {
        case deviceType_iPhone:{Suffix=@"35.png";break;}
        case deviceType_iPhoneRetina:{Suffix=@"70.png";break;}
        case deviceType_iPad:{Suffix=@"45.png";break;}
        case deviceType_iPadRetina:{Suffix=@"90.png";break;}
        default:{Suffix=@"45.png";break;}
    }
    
    wView*LeftArrow=wViewNewWithFrame(wRectNew(GLM.OutlineSize,GLM.OutlineSize,GLM.BarButtonSize, GLM.BarButtonSize));
    wViewSetIsOpaque(LeftArrow, false);
    wViewAddSubview(GL_Canvas_LeftButton, LeftArrow);
    wFont font=[iOSwTextureExt wFontNewFromUIFont:[UIFont systemFontOfSize:30.0]];
    wTexture*LeftTexture=[iOSwTextureExt wTextureFromImageWithName:[@"GL_Canvas_Previous" stringByAppendingString:Suffix]];
    wViewSetBackgroundMask(LeftArrow, LeftTexture);
    
    
    wView*RightArrow=wViewNewWithFrame(wRectNew(GLM.OutlineSize,GLM.OutlineSize,GLM.BarButtonSize,GLM.BarButtonSize));
    wViewSetIsOpaque(RightArrow, false);
    wViewAddSubview(GL_Canvas_RightButton, RightArrow);
    
    wTexture*RightTexture=[iOSwTextureExt wTextureFromImageWithName:[@"GL_Canvas_Next" stringByAppendingString:Suffix]];
    wViewSetBackgroundMask(RightArrow, RightTexture);
    
    [iOSwTextureExt wFontRelease:font];
}

