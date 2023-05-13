//
//  PKAlertManager.h
//  PhotoKit
//
//  Created by Wael on 9/6/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef PhotoKit_PKAlertManager_h
#define PhotoKit_PKAlertManager_h
#import "wViewAPI.h"

typedef unsigned char PKAlertType;
#define PKAlertType_Remove_Photos_Confirm 0
#define PKAlertType_Delete_Photos_Confirm 1
#define PKAlertType_Export_Facebook 2
#define PKAlertType_Export_Twitter 3
#define PKAlertType_Export_Email 4
#define PKAlertType_Export_iTunes_Confirm 5
#define PKAlertType_Export_iTunes 6

#define PKAlertType_Create_Facebook 7
#define PKAlertType_Create_Twitter 8
#define PKAlertType_Create_Email 9

#define PKAlertType_Store_UnableToMakePayment 10
#define PKAlertType_Store_ProductUnAvailable 11

#define PKAlertType_Store_RemoveAds 12
#define PKAlertType_Store_UnlockPackForAds 13

#define PKAlertType_Gallery_SavePhotosToITunes 14

@interface PKAlertManager : NSObject<UIAlertViewDelegate>
-(void)displayAlert:(PKAlertType)alert;
@end

PKAlertManager*AM;

#endif
