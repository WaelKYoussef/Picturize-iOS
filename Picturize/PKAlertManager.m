//
//  PKAlertManager.m
//  PhotoKit
//
//  Created by Wael on 9/6/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import "PKAlertManager.h"
#import "PKExportManager.h"
#import "PKGallery_TopBar.h"
PKAlertType currentAlertType;
@implementation PKAlertManager

-(void)displayAlert:(PKAlertType)alert{
    NSString*AlertBody=nil;
    NSString*CancelButton=nil;
    switch (alert) {
        case PKAlertType_Remove_Photos_Confirm:{
            AlertBody=@"Are you sure you want to remove the selected photo(s) from this album ?";
            CancelButton=@"cancel";
            break;}
        case PKAlertType_Delete_Photos_Confirm:{
            AlertBody=@"Are you sure you want to delete the selected photo(s) ?";
            CancelButton=@"cancel";
            break;}
        case PKAlertType_Export_Facebook:{
            AlertBody=@"Your photo(s) have been shared on Facebook !";
            break;}
        case PKAlertType_Export_Twitter:{
            AlertBody=@"Your photo have been shared on Twitter !";
            break;}
        case PKAlertType_Export_Email:{
            AlertBody=@"Your e-mail has been sent !";
            break;}
        case PKAlertType_Export_iTunes_Confirm:{
            AlertBody=@"Are you sure you want to export the selected photo(s) to the iTunes folder ?";
            CancelButton=@"cancel";
            break;}
        case PKAlertType_Export_iTunes:{
            AlertBody=@"Your photo(s) have been exported to iTunes !";
            break;}
            
        case PKAlertType_Create_Facebook:{
            AlertBody=@"Please sign into you Facebook account from iOS settings !";
            break;}
        case PKAlertType_Create_Twitter:{
            AlertBody=@"Please sign into you Twitter account from iOS settings !";
            break;}
        case PKAlertType_Create_Email:{
            AlertBody=@"Please sign into you e-mail account from iOS settings !";
            break;}
        case PKAlertType_Store_UnableToMakePayment:{
            AlertBody=@"Please make sure that your credit card information is set up and that in-app purchases are enabled from iOS settings !";
            break;}
        case PKAlertType_Store_ProductUnAvailable:{
            AlertBody=@"This item currently unavailable. Please try again later !";
            break;}
        case PKAlertType_Store_RemoveAds:{
            AlertBody=@"Thank you for buying your first pack, as a bonus, ADs are completely removed form your App !";
            break;}
        case PKAlertType_Store_UnlockPackForAds:{
            AlertBody=@"You have spent 30sec+ viewing an AD, as a thank you, a filter pack has been unlocked until the app is restarted !\nYou can view more ADs to unlock more packs !";
            break;}
        case PKAlertType_Gallery_SavePhotosToITunes:{
            AlertBody=@"Your photos will be saved to the App's iTunes folder !";
            break;}
        default:
            break;
    }
            
    
    if (AlertBody) {
        UIAlertView*Alert=[[UIAlertView alloc] initWithTitle:@"Hey" message:AlertBody delegate:self cancelButtonTitle:@"ok" otherButtonTitles:CancelButton, nil];
        [Alert show];
        currentAlertType=alert;
    }
}

-(void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex{
    if (buttonIndex==0) {
        switch (currentAlertType) {
            case PKAlertType_Remove_Photos_Confirm:
            case PKAlertType_Delete_Photos_Confirm:{
                GL_TopBar_LV2_DeleteButton_Action_Confirm();
                break;}
            case PKAlertType_Export_iTunes_Confirm:{
                EM_iTunesButton_Action_Confirmed();
                break;}
            default:
                break;
        }
    }
}

@end