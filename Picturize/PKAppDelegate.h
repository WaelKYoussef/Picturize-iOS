//
//  PKAppDelegate.h
//  PhotoKit
//
//  Created by Wael Youssef on 1/8/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Photos/Photos.h>
#import <AVFoundation/AVFoundation.h>
#import "iOSwOpenGLView.h"
#import "PKCamera.h"
#import "PKSettings.h"


@interface PKAppViewController:UIViewController{
}
@end
@interface PKAppDelegate:UIResponder<UIApplicationDelegate>{
    UIWindow*MainWindow;
    UIViewController*MainViewController;
}
@property(strong,nonatomic)UIWindow*MainWindow;
@property(strong,nonatomic)UIViewController*MainViewController;
@end
