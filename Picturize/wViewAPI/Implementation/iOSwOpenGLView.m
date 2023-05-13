//
//  PKOpenGLView.m
//  PhotoKit
//
//  Created by Wael Youssef on 1/8/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import "iOSwOpenGLView.h"

EAGLContext*OGLContext;
NSMutableArray*activeTouches;

@implementation iOSwOpenGLView
+(EAGLContext*)getOGLCOntext{
    return OGLContext;
}
+(Class)layerClass {
    return [CAEAGLLayer class];
}
-(BOOL)isOpaque {
    return YES;
}
float deviceScaleFactor=1.0;
-(id)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if (self) {
//        deviceScaleFactor=[[UIScreen mainScreen] scale];
        deviceScaleFactor = 1.0;
        [self setContentScaleFactor:deviceScaleFactor];
        [self setMultipleTouchEnabled:true];
        [self setupOpenGLRenderingContext];
        [self setupRenderingTimer];
        
        [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(RotateInterface) name: UIDeviceOrientationDidChangeNotification object: nil];
        wDeviceSetOrientation(wDeviceOrientationPortraitUp);
        
        activeTouches=[[NSMutableArray alloc] initWithCapacity:10];
    }
    return self;
}



GLuint colorRenderbuffer;
GLint width,height;
-(void)setupOpenGLRenderingContext{
    OGLContext=[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    [EAGLContext setCurrentContext:OGLContext];
    if(OGLContext==nil){
        printf("\nwView: Error setting up iOS EAGL Context!");
    }
    CAEAGLLayer*OGLLayer=(CAEAGLLayer*)self.layer;
    OGLLayer.opaque=YES;
    
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [OGLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:OGLLayer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER, colorRenderbuffer);
        
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH,&width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT,&height);
    glViewport(0, 0, width, height);
    GLenum stat = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
    if(stat != GL_FRAMEBUFFER_COMPLETE) {
        printf("\nwView: Error completeing FrameBuffer object, Code:%x!",stat);
    }
    
    wViewSetupRenderingComponents(wSizeNew(width, height));
}

+(void)rebindRenderBuffer{
    glViewport(0, 0, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER, colorRenderbuffer);
}

-(void)setupRenderingTimer{
    CADisplayLink*OGLDisplayLink=[CADisplayLink displayLinkWithTarget:self selector:@selector(renderFrame)];
    
    if (wFrameRate==60.0) {OGLDisplayLink.frameInterval=1;}
    else if (wFrameRate==30.0) {OGLDisplayLink.frameInterval=2;}
    else{OGLDisplayLink.frameInterval=2;}
    
    [OGLDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void)renderFrame{
    if(wViewRenderRoot()){
        [OGLContext presentRenderbuffer:GL_RENDERBUFFER];
    }
}

-(void)RotateInterface{
    switch ([[UIDevice currentDevice] orientation]) {
        case UIDeviceOrientationPortrait:{
            wDeviceSetOrientation(wDeviceOrientationPortraitUp);break;}
        case UIDeviceOrientationPortraitUpsideDown:{
            wDeviceSetOrientation(wDeviceOrientationPortraitDown);break;}
        case UIDeviceOrientationLandscapeLeft:{
            wDeviceSetOrientation(wDeviceOrientationLandscapeRight);break;}
        case UIDeviceOrientationLandscapeRight:{
            wDeviceSetOrientation(wDeviceOrientationLandscapeLeft);break;}
        default:break;
    }
}

#pragma mark - iOSUserInput.
typedef struct _wTouch{
    bool moved;
    wPoint startLocation;
    wPoint location;
    wView*view;
    wTimeStamp timeStamp;
}wTouch;
int touchesCount=0;
wTouch tS[10];

void touchesAddTouch(wTouch touch){
    if (touchesCount<10) {
        tS[touchesCount]=touch;
        touchesCount++;
    }
}
void touchesRemoveTouchAtIndex(int index){
    if (index>=0 && index<touchesCount) {
        memmove(&tS[index], &tS[index+1], sizeof(wTouch)*(touchesCount-index-1));
        touchesCount--;
    }
}

bool OverrideUserInputOrientation=false;
+(void)overrideUserInputOrientation:(BOOL)overrideUserInputOrientation{
    OverrideUserInputOrientation=overrideUserInputOrientation;
}

wPoint CGtoWPoint(CGPoint point){
    
    if (wAnimationIsAnimating()) {
        return wPointZero;
    }
    
    wSize screenSize=wScreenGetSize();
    wDeviceOrientation orient=wDeviceGetOrientation();
    
    wPoint location=wPointNew(point.x*deviceScaleFactor, point.y*deviceScaleFactor);
    
    if (OverrideUserInputOrientation) {orient=wDeviceOrientationPortraitUp;}
    
    switch (orient) {
        case wDeviceOrientationPortraitDown:{
            location=wPointNew(screenSize.width-location.x, screenSize.height-location.y);break;}
        case wDeviceOrientationLandscapeLeft:{
            location=wPointNew(screenSize.width-location.y, location.x);break;}
        case wDeviceOrientationLandscapeRight:{
            location=wPointNew(location.y, screenSize.height-location.x);break;}};
        
    return location;
}

wView*activeView=NULL;
wRect activeFrame;
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    for (UITouch*touch in touches) {
        wPoint location=CGtoWPoint([touch locationInView:self]);
        if (activeView) {
            if (wRectContainsPoint(activeFrame, location)) {
                if (wViewGetMultiTouchEnabled(activeView) && [activeTouches count]<2) {
                    location.x-=activeFrame.origin.x;
                    location.y-=activeFrame.origin.y;
                    wTouch t={false, location,location,activeView,wTimeStampGet()};
                    touchesAddTouch(t);
                    [activeTouches addObject:touch];
                    wGestureTouchDownProcess(activeView, location);
                }
            }
        }
        else{
            wView*currentRootView=wRootView;
            for (int i=wViewGetSubviewCount(currentRootView)-1; i>=0; i--) {
                wView*subview=wViewGetSubviewAtIndex(currentRootView, i);
                
                if (!wViewGetIsHidden(subview) && wViewGetAlpha(subview)>0.0) {
                    wRect subFrame=wViewGetAbsoluteFrame(subview);
                                        
                    if (wRectContainsPoint(subFrame, location)) {
                        currentRootView=subview;
                        i=wViewGetSubviewCount(currentRootView);
                        if (wViewGetUserInteractionEnabled(currentRootView)){
                            activeView=currentRootView;
                        }
                    }
                }
                
            }
            if (activeView) {activeFrame=wViewGetAbsoluteFrame(activeView);
                location.x-=activeFrame.origin.x;
                location.y-=activeFrame.origin.y;
                
                wTouch t={false, location,location,activeView,wTimeStampGet()};
                touchesAddTouch(t);
                [activeTouches addObject:touch];
                wGestureTouchDownProcess(activeView, location);
            }
        }
    }
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
    for (UITouch*touch in touches) {
        NSInteger index=[activeTouches indexOfObject:touch];
        if (index!=NSNotFound) {
            tS[index].location=CGtoWPoint([touch locationInView:self]);
            tS[index].location.x-=activeFrame.origin.x;
            tS[index].location.y-=activeFrame.origin.y;
            
            bool moved=tS[index].moved;
            if (!tS[index].moved) {
                if (wPointDistanceFromPoint(tS[index].location, tS[index].startLocation)>20.0) {
                    tS[index].moved=true;
                    moved=true;
                }
            }
            
            if (moved) {
                if ([activeTouches count]==1) {
                    wGestureSwipeProcess(tS[index].view, tS[index].location);
                }else if ([activeTouches count]==2){
                    wPoint activeLocations[2]={tS[0].location,tS[1].location};
                    wGestureDoubleSwipeProcess(tS[index].view, activeLocations);
                }
            }
        }
    }
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    wTimeStamp timeStamp=wTimeStampGet();
    for (UITouch*touch in touches) {
        NSInteger index=[activeTouches indexOfObject:touch];
        

        if (index!=NSNotFound) {
            wGestureTouchUpProcess(tS[index].view,  tS[index].location);
            
            if (wTimerStampDifference(tS[index].timeStamp, timeStamp)<1.0 &&
                !tS[index].moved) {
                if (touch.tapCount<=1) {
                    wGestureTapProcess(tS[index].view, tS[index].location);
                }else if (touch.tapCount==2){
                    wGestureDoubleTapProcess(tS[index].view, tS[index].location);
                }
            }
            [activeTouches removeObjectAtIndex:index];
            touchesRemoveTouchAtIndex(index);
            if ([activeTouches count]==0) {activeView=NULL;}
        }
    }
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event{
    for (UITouch*touch in touches) {
        NSInteger index=[activeTouches indexOfObject:touch];
        if (index!=NSNotFound) {
            [activeTouches removeObjectAtIndex:index];
            touchesRemoveTouchAtIndex(index);
            if ([activeTouches count]==0) {activeView=NULL;}
        }
    }
}
@end
