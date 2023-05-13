//
//  PKAppDelegate.m
//  PhotoKit
//
//  Created by Wael Youssef on 1/8/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import <AssetsLibrary/AssetsLibrary.h>
#import "PKGalleryLayer.h"
#import "PKAppDelegate.h"
#import "PKCamera.h"
#include "PKFilterManager.h"
#import "PKExportManager.h"

#import "PKGalleryFileManager.h"
@implementation PKAppViewController
-(BOOL)shouldAutorotate{return false;}
@end

@implementation PKAppDelegate
@synthesize MainWindow,MainViewController;
iOSwOpenGLView*MainView;
-(void)setMesurementsForiPhone{
    SLM.BarSize=50.0;
    SLM.BarBorderSize=5.0;
    SLM.OptionSize=40.0;
    SLM.OutlineSize=5.0;
    SLM.BarButtonSize=(SLM.BarSize-(SLM.OutlineSize*2.0)-SLM.BarBorderSize);
    SLM.TitleFontSize=26.0;
    SLM.SettingFontSize=18.0;
    SLM.SelectionMarkSize=20.0;
    SLM.ArrowMarkSize=15.0;
    
    CLM.ShutterSize=75.0;
    CLM.OutlineSize=5.0;
    CLM.BarButtonSize=60.0;
    CLM.BarSize=CLM.BarButtonSize+(CLM.OutlineSize*2);
    CLM.FiltersPerRow=3;
    CLM.GridBarSize=5.0;
    CLM.StoreFontSize=25.0;
    CLM.StoreIconSize=60.0;
    CLM.restoreButtonSize=70.0;

    GLM.BarSize=50.0;
    GLM.BarBorderSize=5.0;
    GLM.AlbumsPerRow_Portrait=2;
    GLM.AlbumsPerRow_Landscape=3;
    GLM.PhotosPerRow_Portrait=4;
    GLM.PhotosPerRow_Landscape=5;
    GLM.OutlineSize=5.0;
    GLM.TitleFontSize=26.0;
    GLM.AlbumFontSize=18.0;
    GLM.AlbumCounterFontSize=16.0;
    GLM.AlbumIconSize=20.0;
    GLM.MinThumbnailSize=100.0;
    GLM.BarButtonSize=(GLM.BarSize-(GLM.OutlineSize*2.0)-GLM.BarBorderSize);
    GLM.PhotoSelectorSize=25.0;
    GLM.AlbumEmptyIconSize=50.0;

    ELM.ButtonSize=60.0;
    ELM.NumberOfColumns=4;
    ELM.NumberOfRows=1;
    ELM.OutlineSize=5.0;
}

-(void)setMesurementsForiPad{
    SLM.BarSize=60.0;
    SLM.BarBorderSize=5.0;
    SLM.OptionSize=50.0;
    SLM.OutlineSize=5.0;
    SLM.BarButtonSize=(SLM.BarSize-(SLM.OutlineSize*2.0)-SLM.BarBorderSize);
    SLM.TitleFontSize=35.0;
    SLM.SettingFontSize=25.0;
    SLM.SelectionMarkSize=25.0;
    SLM.ArrowMarkSize=20.0;
    CLM.restoreButtonSize=100.0;
    
    CLM.ShutterSize=100.0;
    CLM.OutlineSize=5.0;
    CLM.BarButtonSize=100.0;
    CLM.BarSize=CLM.BarButtonSize+(CLM.OutlineSize*2);
    CLM.FiltersPerRow=3;
    CLM.GridBarSize=5.0;
    CLM.StoreFontSize=40.0;
    CLM.StoreIconSize=100.0;
    
    GLM.BarSize=60.0;
    GLM.BarBorderSize=5.0;
    GLM.AlbumsPerRow_Portrait=4;
    GLM.AlbumsPerRow_Landscape=5;
    GLM.PhotosPerRow_Portrait=6;
    GLM.PhotosPerRow_Landscape=7;
    GLM.OutlineSize=5.0;
    GLM.TitleFontSize=35.0;
    GLM.AlbumFontSize=18.0;
    GLM.AlbumCounterFontSize=16.0;
    GLM.AlbumIconSize=20.0;
    GLM.MinThumbnailSize=150.0;
    GLM.BarButtonSize=(GLM.BarSize-(GLM.OutlineSize*2.0)-GLM.BarBorderSize);
    GLM.PhotoSelectorSize=30.0;
    GLM.AlbumEmptyIconSize=80.0;

    ELM.ButtonSize=75.0;
    ELM.NumberOfColumns=4;
    ELM.NumberOfRows=1;
    ELM.OutlineSize=5.0;
}
-(uint64_t)getFreeDiskspace {
    uint64_t totalSpace = 0;
    uint64_t totalFreeSpace = 0;
    NSError *error = nil;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSDictionary *dictionary = [[NSFileManager defaultManager] attributesOfFileSystemForPath:[paths lastObject] error: &error];
    
    if (dictionary) {
        NSNumber *fileSystemSizeInBytes = [dictionary objectForKey: NSFileSystemSize];
        NSNumber *freeFileSystemSizeInBytes = [dictionary objectForKey:NSFileSystemFreeSize];
        totalSpace = [fileSystemSizeInBytes unsignedLongLongValue];
        totalFreeSpace = [freeFileSystemSizeInBytes unsignedLongLongValue];
        NSLog(@"Memory Capacity of %llu KiB with %llu KiB Free memory available.", ((totalSpace/1024ll)), ((totalFreeSpace/1024ll)));
    } else {
        NSLog(@"Error Obtaining System Memory Info: Domain = %@, Code = %@", [error domain], [error code]);
    }
    
    return totalFreeSpace;
}
-(BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions{

    CGRect screenFrame=[UIScreen mainScreen].bounds;
    MainWindow=[[UIWindow alloc] initWithFrame:screenFrame];
    [MainWindow makeKeyAndVisible];
    
    MainViewController=[[UIViewController alloc] init];
    
    CGRect MainFrame=MainWindow.bounds;
    MainView=[[iOSwOpenGLView alloc] initWithFrame:MainFrame];
    [MainView setFrame:MainFrame];
    [MainWindow addSubview:MainView];

    PKAppViewController*vc=[[PKAppViewController alloc] init];
    [vc setView:MainView];
    [MainWindow setRootViewController:vc];
    
    wViewDefaults.backgroundColor=wColorClear();
    
    wViewSetBackgroundColor(wRootView, wColorBlack());

    
    UIUserInterfaceIdiom idiom=[[UIDevice currentDevice] userInterfaceIdiom];
    if (idiom == UIUserInterfaceIdiomPad) {[self setMesurementsForiPad];}
    else{[self setMesurementsForiPhone];}
    
    AM=[[PKAlertManager alloc] init];

    
    SM_Setup();
    SL_Setup();
    
    wTexture*TargetTexture=wTextureNewEmpty();
  //  TargetTexture=[iOSwTextureExt wTextureFromImageWithName:@"Model.jpg"];
  //  TargetTexture=[iOSwTextureExt wTextureFromImageWithName:@"Three-pink-daisies.jpeg"];

    PKFilterManagerSetupWithTargetTexture(TargetTexture);
    CM_Setup(TargetTexture);
    CL_Setup();

    GL_Setup();
    
    EM_Setup();

    
    SM_SetApplicationColorTheme(SM_GetSelectionForSetting(PKSetting_ColorTheme));
    
    CM_StartCamera();
    
  //  wViewSetRenderIndefinitly(true);
    
    wAutoRotationSetAnimateRotation(false);
    [iOSwOpenGLView overrideUserInputOrientation:true];
    wAutoRotationSetAllowsOrientation(wDeviceOrientationPortraitUp, true);
    wAutoRotationSetAllowsOrientation(wDeviceOrientationPortraitDown, true);
    wAutoRotationSetAllowsOrientation(wDeviceOrientationLandscapeLeft, true);
    wAutoRotationSetAllowsOrientation(wDeviceOrientationLandscapeRight, true);
    wAutoRotationSetBeginFunction(willrotate);

    setupGL();
        
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(RotateInterface) name: UIDeviceOrientationDidChangeNotification object: nil];

    if (isFirstTimeRun) {
       // CL_DisplayTutorial();
        wTimer*tutorialTimer=wTimerNew(5.0, CL_DisplayTutorial, false, nil);
        wTimerStart(tutorialTimer);
    }
    
    return YES;
}

-(void)RotateInterface{
    [UIView setAnimationsEnabled:true];
}


void willrotate(wDeviceOrientation orientation){
   // [manager rotateToOrientation:orientation];
    
    wMatrix4 mat=wMat4Identity;
    wSize screenSize=wScreenGetPortraitFrame().size;
    switch (orientation) {
        case wDeviceOrientationPortraitDown:{
            mat=wMat4RotateOnZAxis(mat, 180.0);
            mat=wMat4Translate(mat, screenSize.width, screenSize.height, 0.0);break;}
        case wDeviceOrientationLandscapeLeft:{
            mat=wMat4RotateOnZAxis(mat, 90.0);
            mat=wMat4Translate(mat, screenSize.height, 0.0, 0.0);break;}
        case wDeviceOrientationLandscapeRight:{
            mat=wMat4RotateOnZAxis(mat, -90.0);
            mat=wMat4Translate(mat, 0.0, screenSize.width, 0.0);break;}
        default:break;
    }
   wViewSetMatrix(CameraLayer, mat);

    CL_Bar_RotateButtons(orientation);
    
    if (!wViewGetIsHidden(GalleryLayer)) {
        GL_AdjustOrientation(orientation);
    }
    if (!wViewGetIsHidden(SettingsLayer)) {
        SL_AdjustOrientation(orientation);
    }
}

-(void)applicationWillResignActive:(UIApplication*)application{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

-(void)applicationDidEnterBackground:(UIApplication*)application{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

-(void)applicationWillEnterForeground:(UIApplication*)application{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

-(void)applicationDidBecomeActive:(UIApplication*)application{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

-(void)applicationWillTerminate:(UIApplication*)application{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end

