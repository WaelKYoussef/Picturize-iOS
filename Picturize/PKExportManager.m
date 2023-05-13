//
//  PKExportManager.m
//  PhotoKit
//
//  Created by Wael on 9/3/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import "PKExportManager.h"
#import "PKGalleryLayer.h"
#import "iOSwTextureExt.h"
#import "PKGallery_Canvas.h"



wView*ExportLayer;
wArray*EM_Edges;
wArray*EM_Buttons;

wView*EM_FacebookButton;
wView*EM_TwitterButton;
wView*EM_MailButton;
wView*EM_iTunesButton;
float EM_Width, EM_Height;

void EM_SetColorTheme(){
    wColor edgeColor=PKBarColor;
    edgeColor.a=1.0;
    for (unsigned int i=0; i<wArrayGetCount(EM_Edges); i++) {
        wView*Edge=wArrayGetItemAtIndex(EM_Edges, i);
        wViewSetBackgroundColor(Edge, edgeColor);
    }
    wViewSetBackgroundColor(ExportLayer, PKBarColor);
}

unsigned char serviceType;
void EM_PostImagesUsingControler(NSMutableArray*images, SLComposeViewController*controller){
    
    if([SLComposeViewController isAvailableForServiceType:[controller serviceType]])
    {
        SLComposeViewControllerCompletionHandler __block completionHandler=^(SLComposeViewControllerResult result){
            
            [controller dismissViewControllerAnimated:YES completion:nil];
            
            if (result==SLComposeViewControllerResultDone) {
                if (serviceType==0) {[AM displayAlert:PKAlertType_Export_Facebook];}
                else{[AM displayAlert:PKAlertType_Export_Twitter];}
            }
        };
        
        
        [controller setInitialText:@"Shared using PhotoKit !"];
        
        
        for (UIImage*image in images) {
            if (![controller addImage:image]) {
                break;
            }
        }
        
        [controller setCompletionHandler:completionHandler];
    }
    else{
        if (serviceType==0) {[AM displayAlert:PKAlertType_Create_Facebook];}
        else{[AM displayAlert:PKAlertType_Create_Twitter];}
    }

}

NSMutableArray*EM_CreateImageList(){
    NSMutableArray*itemsToShare=[NSMutableArray arrayWithCapacity:10];
    if (!wViewGetIsHidden(GL_PhotoGrid)) {
        unsigned int count=wTableViewGetItemCount(GL_PhotoGrid);
        for (unsigned int i=0; i<count; i++) {
            if (selectedFlags[i]) {
                [itemsToShare addObject:[SelectedAlbum objectAtIndex:i]];
            }
        }
    }else if (!wViewGetIsHidden(GL_Canvas)) {
        [itemsToShare addObject:[SelectedAlbum objectAtIndex:IndexOfLoadedAsset]];
    }
    
    PHImageRequestOptions*opt=[[PHImageRequestOptions alloc] init];
    opt.synchronous=true;
    opt.deliveryMode=PHImageRequestOptionsDeliveryModeFastFormat;
    opt.resizeMode=PHImageRequestOptionsResizeModeFast;
    
    NSMutableArray*imagesToShare=[NSMutableArray arrayWithCapacity:10];
    for (PHAsset*asset in itemsToShare) {
        [[PHImageManager defaultManager] requestImageForAsset:asset targetSize:PHImageManagerMaximumSize contentMode:PHImageContentModeAspectFill options:opt resultHandler:^(UIImage *result, NSDictionary *info) {
            [imagesToShare addObject:result];
        }];
    }

    return imagesToShare;
}

void EM_FacebookButton_Action(){
    serviceType=0;
    SLComposeViewController *fbController=[SLComposeViewController composeViewControllerForServiceType:SLServiceTypeFacebook];
    EM_PostImagesUsingControler(EM_CreateImageList(), fbController);
}

void EM_TwitterButton_Action(){
    serviceType=1;
    SLComposeViewController *fbController=[SLComposeViewController composeViewControllerForServiceType:SLServiceTypeTwitter];
    EM_PostImagesUsingControler(EM_CreateImageList(), fbController);
}

void EM_MailButton_Action(){
    [UIView setAnimationsEnabled:true];
    if ([MFMailComposeViewController canSendMail]) {
        MFMailComposeViewController*MailView = [[MFMailComposeViewController alloc] init];
      //  [MailView setToRecipients:[NSArray arrayWithObjects:@"Apps@nplusw.com", nil]];
        [MailView setSubject:@"Shared using PhotoKit !"];
        
        NSMutableArray*imageList=EM_CreateImageList();
        
        int counter=0;
        for (UIImage*image in imageList) {
            counter++;
            NSData*imageData=UIImageJPEGRepresentation(image, 1.0);
            [MailView addAttachmentData:imageData mimeType:@"image/jpeg" fileName:[NSString stringWithFormat:@"Photo%02d.jpg", counter]];
        }
        
       
    }
    else {
        [AM displayAlert:PKAlertType_Create_Email];
    }
}
#import "PKGalleryFileManager.h"
void EM_iTunesButton_Action_Confirmed(){
    NSMutableArray*imageList=EM_CreateImageList();
    int counter=0;
    
    wDate date=wDateGet();
    NSString*nameTail=[NSString stringWithFormat:@"%d%d%d%d%d%d.jpg", date.Year,date.Month,date.Day,date.Hour,date.Minute,date.Second];
    
    for (UIImage*image in imageList) {
        counter++;
        NSData*imageData=UIImageJPEGRepresentation(image, 1.0);
        [imageData writeToFile:GM_PathForImageNameForITunes([NSString stringWithFormat:@"Photo%02d%@", counter, nameTail]) atomically:true];
    }
    
    [AM displayAlert:PKAlertType_Export_iTunes];
}

void EM_iTunesButton_Action(){
    [AM displayAlert:PKAlertType_Export_iTunes_Confirm];
}

BOOL isDisplayed=false;
BOOL EM_IsDisplayed(){
    return isDisplayed;
}

void EM_Display(){
    wRect frame=wRectNew((wScreenGetWidth()-EM_Width)/2.0, GLM.BarSize, EM_Width, 0.0);
    wViewSetFrame(ExportLayer, frame);
    wViewShow(ExportLayer);
    frame.size.height=EM_Height;
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateFrame(ExportLayer, frame);
    wAnimationApplyContext();
    isDisplayed=true;
}

void EM_Hide(){
    wRect frame=wRectNew((wScreenGetWidth()-EM_Width)/2.0, GLM.BarSize, EM_Width, EM_Height);
    wViewSetFrame(ExportLayer, frame);
    frame.size.height=0;
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetEndFunction(wViewHide, ExportLayer);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateFrame(ExportLayer, frame);
    wAnimationApplyContext();
    isDisplayed=false;
}

void EM_Setup(){
    EM_Width=(ELM.NumberOfColumns*(ELM.ButtonSize+ELM.OutlineSize))+(ELM.OutlineSize*3.0);
    EM_Height=(ELM.NumberOfRows*(ELM.ButtonSize+ELM.OutlineSize))+(ELM.OutlineSize*2.0);
    
    wRect frame=wRectNew(0.0,0.0,EM_Width, EM_Height);
    ExportLayer=wViewNewWithFrame(frame);
    wViewSetIsOpaque(ExportLayer, false);
    wViewAddSubview(GalleryLayer, ExportLayer);
    wAutoLayoutOptions opt=wAutoLayoutNone;
    opt.anchor.x=0.5;
    wViewSetAutoLayoutOptions(ExportLayer, opt);
    wViewSetClipsSubviews(ExportLayer, true);
    wViewHide(ExportLayer);

    EM_Edges=wArrayNewWithCapacity(5);
    wView*Edge1=wViewNewWithFrame(wRectNew(0, 0, ELM.OutlineSize, frame.size.height-ELM.OutlineSize));
    wViewAddSubview(ExportLayer, Edge1);
    wArrayAddItem(EM_Edges, Edge1);
    
    wView*Edge2=wViewNewWithFrame(wRectNew(frame.size.width-ELM.OutlineSize, 0, ELM.OutlineSize, frame.size.height-ELM.OutlineSize));
    wViewAddSubview(ExportLayer, Edge2);
    wArrayAddItem(EM_Edges, Edge2);
    
    wView*Edge3=wViewNewWithFrame(wRectNew(0, frame.size.height-ELM.OutlineSize, frame.size.width, ELM.OutlineSize));
    opt.anchor.y=1.0;
    wViewSetAutoLayoutOptions(Edge3, opt);
    wViewAddSubview(ExportLayer, Edge3);
    wArrayAddItem(EM_Edges, Edge3);
    
    
    EM_Buttons=wArrayNewWithCapacity(5);
    
    wRect buttonFrame=wRectNew(ELM.OutlineSize*2.0, ELM.OutlineSize, ELM.ButtonSize, ELM.ButtonSize);
    EM_FacebookButton=wButtonNewWithFrame(buttonFrame);
    wViewSetBackgroundImage(EM_FacebookButton, [iOSwTextureExt wTextureFromImageWithName:@"EM_Facebook.png"]);
    wViewSetBackgroundMask(EM_FacebookButton, GL_AlbumMask);
    wViewAddSubview(ExportLayer, EM_FacebookButton);
    wButtonSetFunction(EM_FacebookButton, EM_FacebookButton_Action);
    wArrayAddItem(EM_Buttons, EM_FacebookButton);
    
    buttonFrame.origin.x+=(ELM.ButtonSize+ELM.OutlineSize);
    EM_TwitterButton=wButtonNewWithFrame(buttonFrame);
    wViewSetBackgroundImage(EM_TwitterButton, [iOSwTextureExt wTextureFromImageWithName:@"EM_Twitter.png"]);
    wViewSetBackgroundMask(EM_TwitterButton, GL_AlbumMask);
    wViewAddSubview(ExportLayer, EM_TwitterButton);
    wButtonSetFunction(EM_TwitterButton, EM_TwitterButton_Action);
    wArrayAddItem(EM_Buttons, EM_TwitterButton);
    
    buttonFrame.origin.x+=(ELM.ButtonSize+ELM.OutlineSize);
    EM_MailButton=wButtonNewWithFrame(buttonFrame);
    wViewSetBackgroundImage(EM_MailButton, [iOSwTextureExt wTextureFromImageWithName:@"EM_Mail.png"]);
    wViewSetBackgroundMask(EM_MailButton, GL_AlbumMask);
    wViewAddSubview(ExportLayer, EM_MailButton);
    wButtonSetFunction(EM_MailButton, EM_MailButton_Action);
    wArrayAddItem(EM_Buttons, EM_MailButton);
    
    buttonFrame.origin.x+=(ELM.ButtonSize+ELM.OutlineSize);
    EM_iTunesButton=wButtonNewWithFrame(buttonFrame);
    wViewSetBackgroundImage(EM_iTunesButton, [iOSwTextureExt wTextureFromImageWithName:@"EM_iTunes.png"]);
    wViewSetBackgroundMask(EM_iTunesButton, GL_AlbumMask);
    wViewAddSubview(ExportLayer, EM_iTunesButton);
    wButtonSetFunction(EM_iTunesButton, EM_iTunesButton_Action);
    wArrayAddItem(EM_Buttons, EM_iTunesButton);
    
    
    
    opt=wAutoLayoutNone;
    opt.anchor.y=1.0;
    wViewSetAutoLayoutOptions(EM_FacebookButton, opt);
    wViewSetAutoLayoutOptions(EM_TwitterButton, opt);
    wViewSetAutoLayoutOptions(EM_MailButton, opt);
    wViewSetAutoLayoutOptions(EM_iTunesButton, opt);

    
}
