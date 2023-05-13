//
//  wView.c
//  PhotoKit
//
//  Created by Wael Youssef on 1/12/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "wViewAPI.h"

wPoint wScreenCenter;
float wScreenWidth;
float wScreenHeight;
float wScreenDepth;
wRect wScreenPortraitFrame;
wRect wScreenLandscapeFrame;


GLuint wViewOGLColorProgram;
GLuint wViewOGLTextureProgram;
GLuint wViewOGLColorAndTextureProgram;
GLint colorLocationC, colorLocationT, colorLocationCaT;
GLint alphaLocationC ,alphaLocationT ,alphaLocationCaT;
GLint projectionEquationLocationC, projectionEquationLocationT, projectionEquationLocationCaT;

wView wRootViewContainer;
wView*const wRootView=&wRootViewContainer;

const wVec2 orthEquation={0.0,1.0};
const wVec2 persEquation={1.0,0.0};


wVertexArray wVertexArrayTemp;
GLuint baseTexture,baseMask;

const wAutoLayoutOptions wAutoLayoutNone={0.0f,0.0f,wAutoLayoutStaticWidth,wAutoLayoutStaticHeight};



float roundFloat(float d){
    return floor(d+0.5f);
}

float wScreenGetWidth(){
    if (wDeviceOrientationIsPortrait(wDeviceGetOrientation()))
        return wScreenWidth;
    else return wScreenHeight;
}
float wScreenGetHeight(){
    if (wDeviceOrientationIsPortrait(wDeviceGetOrientation()))
        return wScreenHeight;
    else return wScreenWidth;
}
wSize wScreenGetSize(){
    if (wDeviceOrientationIsPortrait(wDeviceGetOrientation()))
        return wScreenPortraitFrame.size;
    else return wScreenLandscapeFrame.size;
}
wPoint wScreenGetCenter(){
    if (wDeviceOrientationIsPortrait(wDeviceGetOrientation()))
        return wScreenCenter;
    else return wPointNew(wScreenCenter.y, wScreenCenter.x);
}
wRect wScreenGetFrame(){
    if (wDeviceOrientationIsPortrait(wDeviceGetOrientation()))
        return wScreenPortraitFrame;
    else return wScreenLandscapeFrame;
}
wRect wScreenGetPortraitFrame(){return wScreenPortraitFrame;}
wRect wScreenGetLandscapeFrame(){return wScreenLandscapeFrame;}

GLuint createOnePixelTexture(float r, float g ,float b, float a){
    GLuint texture;
    GLfloat cValues[4]={r,g,b,a};
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, cValues);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    return texture;
}

#pragma mark - wNew.

wVec4 wPointToVec4(wPoint p){
    wVec4 vec={p.x,p.y,0.0,1.0};
    return vec;
}


void wVec4Round(wVec4*vec){
    vec[0].x=roundFloat(vec[0].x),vec[0].y=roundFloat(vec[0].y);
    vec[0].z=roundFloat(vec[0].z),vec[0].w=roundFloat(vec[0].w);
}



#pragma mark - wMatrix4




wMatrix4 wMat4AnchorToPoint(wMatrix4 mat, wPoint anchor){
    wMatrix4 Result;
    
    wMatrix4 t1={1.0f,0.0f,0.0f,anchor.x,
                 0.0f,1.0f,0.0f,anchor.y,
                 0.0f,0.0f,1.0f,  0.0f,
                 0.0f,0.0f,0.0f,  1.0f};
    
    wMatrix4 t2={1.0f,0.0f,0.0f,-anchor.x,
                 0.0f,1.0f,0.0f,-anchor.y,
                 0.0f,0.0f,1.0f,  0.0f,
                 0.0f,0.0f,0.0f,  1.0f};
    

    Result=wMat4MultiplyByMat4(t1, mat);
    
    Result=wMat4MultiplyByMat4(Result, t2);

    return Result;
}



#pragma mark - wVertex-Array.
bool wVertexIsLeftOfLine(wVertex*point,wVertex*lineA,wVertex*lineB){
    float crossProduct=((lineB[0].location.x-lineA[0].location.x)*(point[0].location.y-lineA[0].location.y))-((lineB[0].location.y-lineA[0].location.y)*(point[0].location.x-lineA[0].location.x));
    
    if(roundFloat(crossProduct)>0.0f){return true;}
    else {return false;}
}

wVertex wVertexComputeIntersection(wVertex Line1A, wVertex Line1B,wVertex Line2A,wVertex Line2B){
    wVertex intersection;
    
    float line1xDiff=Line1A.location.x-Line1B.location.x;
    float line2xDiff=Line2A.location.x-Line2B.location.x;
    
    if (line1xDiff==0.0) {
        float line2m=(Line2A.location.y-Line2B.location.y)/line2xDiff;
        float line2b=Line2A.location.y-(line2m*Line2A.location.x);
        
        intersection.location.x=Line1A.location.x;
        intersection.location.y=(line2m*intersection.location.x)+line2b;
    }
    else{
        if (line2xDiff==0.0) {
            float line1m=(Line1A.location.y-Line1B.location.y)/line1xDiff;
            float line1b=Line1A.location.y-(line1m*Line1A.location.x);
            
            intersection.location.x=Line2A.location.x;
            intersection.location.y=(line1m*intersection.location.x)+line1b;
        }
        else{
            float line1m=(Line1A.location.y-Line1B.location.y)/line1xDiff;
            float line2m=(Line2A.location.y-Line2B.location.y)/line2xDiff;
            
            float line1b=Line1A.location.y-(line1m*Line1A.location.x);
            float line2b=Line2A.location.y-(line2m*Line2A.location.x);
            
            intersection.location.x=(line2b-line1b)/(line1m-line2m);
            intersection.location.y=(line1m*intersection.location.x)+line1b;
        }
    }
    
    float t;
    if (Line1B.location.x==Line1A.location.x) {
    t=(intersection.location.y-Line1A.location.y)/(Line1B.location.y-Line1A.location.y);}
    else{t=(intersection.location.x-Line1A.location.x)/(Line1B.location.x-Line1A.location.x);}
    intersection.textureCoordinates.x=Line1A.textureCoordinates.x+(t*(Line1B.textureCoordinates.x-Line1A.textureCoordinates.x));
    intersection.textureCoordinates.y=Line1A.textureCoordinates.y+(t*(Line1B.textureCoordinates.y-Line1A.textureCoordinates.y));
    
    intersection.location.z=Line1A.location.z+(t*(Line1B.location.z-Line1A.location.z));
    
    //wVec4Round(&intersection.location);
    return intersection;
}

void wVertexArrayMultiplyByMat4(wVertexArray*array,wMatrix4 mat4){
    for (int i=0; i<array[0].count; i++) {
    array[0].list[i].location=wMat4MultiplyByVec4(mat4, array[0].list[i].location);}
}

void wVertexArrayApplyPerspective(wVertexArray*array, wPoint worldCenter){
    for (int i=0; i<array[0].count; i++) {
        float r=(array[0].list[i].location.z/wScreenDepth)+1.0;
        
        if (array[0].list[i].location.z!=0.0) {
            array[0].list[i].location.x=((array[0].list[i].location.x-worldCenter.x)*r)+worldCenter.x;
            array[0].list[i].location.y=((array[0].list[i].location.y-worldCenter.y)*r)+worldCenter.y;
            
            array[0].list[i].location.x=roundFloat(array[0].list[i].location.x);
            array[0].list[i].location.y=roundFloat(array[0].list[i].location.y);
        }
    }
}

float wVertexArrayArea(wVertexArray*array){
    float area=0.0;
    wVec4 Point1=array[0].list[array[0].count-1].location;
    wVec4 Point2;
    for (int i=0; i<array[0].count; i++) {
        Point2=array[0].list[i].location;
        area+=((Point2.x-Point1.x)*(Point2.y+Point1.y));
        Point1=Point2;
    }
    
    return roundFloat(area);
}

bool wVertexArrayIsClockwise(wVertexArray*array){
    if (roundFloat(wVertexArrayArea(array))>0.0) {return true;}
    else{return false;}
}

bool wVertexArrayIsAStraightLine(wVertexArray*array){
    if (roundFloat(wVertexArrayArea(array))==0.0) {return true;}
    else{return false;}
}

void wVertexArrayClone(wVertexArray*source,wVertexArray*target){
    target[0].count=source[0].count;
    
    if (target[0].count>=target[0].capacity) {
    target[0].capacity=target[0].count+10;
    target[0].list=realloc(target[0].list, sizeof(wVertex)*target[0].capacity);}
    
    memcpy(target[0].list, source[0].list, sizeof(wVertex)*target[0].count);
}

bool wVertexArrayReverse(wVertexArray*array){
    wVertexArrayClone(array, &wVertexArrayTemp);
    
    int count=array[0].count-1;
    for (int i=0; i<array[0].count; i++) {
        array[0].list[i]=wVertexArrayTemp.list[count];count--;
    }
    return true;
}

void wVertexArrayAddVertex(wVertexArray*vertices,wVertex vertex){
    
    if (vertices[0].count==vertices[0].capacity) {
    vertices[0].capacity=vertices[0].count+10;
    vertices[0].list=realloc(vertices[0].list, sizeof(wVertex)*vertices[0].capacity);}
    
    vertices[0].list[vertices[0].count]=vertex;
    vertices[0].count++;
}

void wVertexArrayClip(wVertexArray*subject, wVertexArray*clipper){
    bool El,Sl;
    wVertex inters,E,S,clipper1;
    wVertex clipper2=clipper[0].list[clipper[0].count-1];
    for (int i=0; i<clipper[0].count; i++) {
        clipper1=clipper[0].list[i];
        wVertexArrayClone(subject, &wVertexArrayTemp);
        subject[0].count=0;
        S=wVertexArrayTemp.list[wVertexArrayTemp.count-1];
        for (int y=0; y<wVertexArrayTemp.count; y++) {
            E=wVertexArrayTemp.list[y];
            El=wVertexIsLeftOfLine(&clipper2, &clipper1, &E);
            Sl=wVertexIsLeftOfLine(&clipper2, &clipper1, &S);
            if (!El) {if (Sl){
            inters=wVertexComputeIntersection(S, E, clipper2, clipper1);
            wVertexArrayAddVertex(subject, inters);}
            wVertexArrayAddVertex(subject, E);}
            else if (!Sl){
            inters=wVertexComputeIntersection(S, E, clipper2, clipper1);
            wVertexArrayAddVertex(subject, inters);}
            S=E;
        }
        clipper2=clipper1;
    }
}

void wViewClipTextureCoordinates(wSize textureSize, wSize targetSize, float*minX, float*maxX, float*minY, float*maxY){
    
    float newSize;
    float ScreenHeight=targetSize.height;
    float ScreenWidth=targetSize.width;
    float ImageWidth=textureSize.width;
    float ImageHeight=textureSize.height;
    
    float minTexX=0.0,maxTexX=1.0,minTexY=0.0,maxTexY=1.0;
    
    if (ScreenWidth>ScreenHeight) {
        if (ImageWidth>ImageHeight) {
            if (ImageWidth/ImageHeight < ScreenWidth/ScreenHeight) {
                newSize=ScreenWidth*(ImageHeight/ImageWidth);
                float scale=ScreenHeight/newSize;
                minTexY=((minTexY-0.5)*scale)+0.5;
                maxTexY=((maxTexY-0.5)*scale)+0.5;
            }
            else {newSize=ScreenHeight*(ImageWidth/ImageHeight);
                float scale=ScreenWidth/newSize;
                minTexX=((minTexX-0.5)*scale)+0.5;
                maxTexX=((maxTexX-0.5)*scale)+0.5;
            }
        }
        else {
            if (ImageHeight/ImageWidth < ScreenHeight/ScreenWidth) {
                newSize=ScreenHeight*(ImageWidth/ImageHeight);
                float scale=ScreenWidth/newSize;
                minTexX=((minTexX-0.5)*scale)+0.5;
                maxTexX=((maxTexX-0.5)*scale)+0.5;
            }
            else {newSize=ScreenWidth*(ImageHeight/ImageWidth);
                float scale=ScreenHeight/newSize;
                minTexY=((minTexY-0.5)*scale)+0.5;
                maxTexY=((maxTexY-0.5)*scale)+0.5;
            }
        }
    }
    else {
        if (ImageWidth>ImageHeight) {
            if (ImageWidth/ImageHeight < ScreenWidth/ScreenHeight) {
                newSize=ScreenWidth*(ImageHeight/ImageWidth);
                float scale=ScreenHeight/newSize;
                minTexY=((minTexY-0.5)*scale)+0.5;
                maxTexY=((maxTexY-0.5)*scale)+0.5;
            }
            else {newSize=ScreenHeight*(ImageWidth/ImageHeight);
                float scale=ScreenWidth/newSize;
                minTexX=((minTexX-0.5)*scale)+0.5;
                maxTexX=((maxTexX-0.5)*scale)+0.5;
            }
        }
        else {
            if (ImageHeight/ImageWidth < ScreenHeight/ScreenWidth) {
                newSize=ScreenHeight*(ImageWidth/ImageHeight);
                float scale=ScreenWidth/newSize;
                minTexX=((minTexX-0.5)*scale)+0.5;
                maxTexX=((maxTexX-0.5)*scale)+0.5;
            }
            else {newSize=ScreenWidth*(ImageHeight/ImageWidth);
                float scale=ScreenHeight/newSize;
                minTexY=((minTexY-0.5)*scale)+0.5;
                maxTexY=((maxTexY-0.5)*scale)+0.5;
            }
        }
    }
    
    minX[0]=minTexX,  maxX[0]=maxTexX,  minY[0]=minTexY,  maxY[0]=maxTexY;
}

bool wVertexArraySimpleClip(wVertexArray*array, wRect subject, float*texCoord, wRect clipper){
    float minTexX=texCoord[0],maxTexX=texCoord[1],minTexY=texCoord[2],maxTexY=texCoord[3];
    
    float SminX=subject.origin.x;
    float SminY=subject.origin.y;
    float SmaxX=SminX+subject.size.width;
    float SmaxY=SminY+subject.size.height;
    
    float OriginalSminX=SminX, OriginalSminY=SminY, OriginalSmaxX=SmaxX, OriginalSmaxY=SmaxY;
    
    float CminX=clipper.origin.x;
    float CminY=clipper.origin.y;
    float CmaxX=CminX+clipper.size.width;
    float CmaxY=CminY+clipper.size.height;
    
    if (SminX>=CmaxX) {array[0].count=0;return false;}
    else if (SmaxX<=CminX){array[0].count=0;return false;}
    else if (SminY>=CmaxY){array[0].count=0;return false;}
    else if (SmaxY<=CminY){array[0].count=0;return false;}
    
    if (SminX<CminX) {SminX=CminX;
    minTexX=((CminX-OriginalSminX)/(OriginalSmaxX-OriginalSminX));}
    if (SmaxX>CmaxX){SmaxX=CmaxX;
    maxTexX=((CmaxX-OriginalSminX)/(OriginalSmaxX-OriginalSminX));}
    if (SminY<CminY){SminY=CminY;
        minTexY=((CminY-OriginalSminY)/(OriginalSmaxY-OriginalSminY));}
    if (SmaxY>CmaxY){SmaxY=CmaxY;
    maxTexY=((CmaxY-OriginalSminY)/(OriginalSmaxY-OriginalSminY));}
    
    array[0].count=4;
    array[0].list[0].location=wPointToVec4(wPointNew(SminX, SminY));
    array[0].list[1].location=wPointToVec4(wPointNew(SminX, SmaxY));
    array[0].list[2].location=wPointToVec4(wPointNew(SmaxX, SmaxY));
    array[0].list[3].location=wPointToVec4(wPointNew(SmaxX, SminY));
    
    array[0].list[0].textureCoordinates=wVec2New(minTexX, minTexY);
    array[0].list[1].textureCoordinates=wVec2New(minTexX, maxTexY);
    array[0].list[2].textureCoordinates=wVec2New(maxTexX, maxTexY);
    array[0].list[3].textureCoordinates=wVec2New(maxTexX, minTexY);
    return true;
}

void wVertexArrayCreateFromRect(wVertexArray*array,wRect rect, float*texCoord){
    float minX=rect.origin.x;
    float minY=rect.origin.y;
    float maxX=minX+rect.size.width;
    float maxY=minY+rect.size.height;
    
    array[0].count=4;
    array[0].list[0].location=wPointToVec4(wPointNew(minX, minY));
    array[0].list[1].location=wPointToVec4(wPointNew(minX, maxY));
    array[0].list[2].location=wPointToVec4(wPointNew(maxX, maxY));
    array[0].list[3].location=wPointToVec4(wPointNew(maxX, minY));
    
    float minTexX=texCoord[0],maxTexX=texCoord[1],minTexY=texCoord[2],maxTexY=texCoord[3];
    array[0].list[0].textureCoordinates=wVec2New(minTexX, minTexY);
    array[0].list[1].textureCoordinates=wVec2New(minTexX, maxTexY);
    array[0].list[2].textureCoordinates=wVec2New(maxTexX, maxTexY);
    array[0].list[3].textureCoordinates=wVec2New(maxTexX, minTexY);
}

#pragma mark - wView.
wView* wViewNew(){
    wView*view=malloc(sizeof(wView));
    memset(view, 0, sizeof(wView));
    
    view[0].matrix=wViewDefaults.matrix;
    view[0].backgroundColor=wViewDefaults.backgroundColor;
    view[0].autoLayoutOptions=wViewDefaults.autoLayoutOptions;
    view[0].alpha=wViewDefaults.alpha;
    view[0].perspectiveOption=wViewDefaults.perspectiveOption;
    view[0].isOpaque=wViewDefaults.isOpaque;
    view[0].isHidden=wViewDefaults.isHidden;
    view[0].clipsSubviews=wViewDefaults.clipsSubviews;
    view[0].perspectiveProjection=wViewDefaults.perspectiveProjection;
    view[0].resizesSubviews=wViewDefaults.resizesSubviews;
    view[0].userInteractioEnabled=wViewDefaults.userInteractioEnabled;
    view[0].multiTouchEnabled=wViewDefaults.multiTouchEnabled;
    view[0].shouldDraw=wViewDefaults.shouldDraw;
    view[0].backgroundImageDisplayMode=wViewDefaults.backgroundImageDisplayMode;
    
    view[0].subviews=wArrayNew();
    view[0].gestureContext=NULL;
    view[0].scrollContext=NULL;
    view[0].tableContext=NULL;
    view[0].renderingContext.vertices.capacity=5;
    view[0].renderingContext.vertices.list=malloc(sizeof(wVertex)*5);
    
    view[0].backgroundImage=NULL;
    view[0].backgroundMask=NULL;
    view[0].superView=NULL;
    view[0].renderOpenGLCode=NULL;
    
    return view;
}

wView* wViewNewWithFrame(wRect frame){
    wView*view=wViewNew();
    
    view[0].frame=frame;
   
    view[0].centerPoint.x=view[0].frame.origin.x+(view[0].frame.size.width/2.0);
    view[0].centerPoint.y=view[0].frame.origin.y+(view[0].frame.size.height/2.0);
    return view;
}

void wViewAddSubviewAtIndex(wView*view,wView*subview,int index){
    wViewRemoveFromSuperView(subview);
    subview[0].superView=view;
    wArrayAddItemAtIndex(view[0].subviews, subview, index);
    if (wViewIsOnScreen(subview)) {wViewRequestFrames(1);}}
void wViewAddSubview(wView*view,wView*subview){
    wViewAddSubviewAtIndex(view, subview, wArrayGetCount(view[0].subviews));
}
void wViewRemoveSubviewAtIndex(wView*view,int index){
    wView*subview=wArrayGetItemAtIndex(view[0].subviews, index);
    subview[0].superView=NULL;
    wArrayRemoveItemAtIndex(view[0].subviews, index);
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}
void wViewRemoveSubview(wView*view,wView*subview){
    for (int i=wArrayGetCount(view[0].subviews)-1; i>=0; i--) {
        if (subview==wArrayGetItemAtIndex(view[0].subviews, i)) {
            wViewRemoveSubviewAtIndex(view, i);break;
        }
    }
}
void wViewRemoveAllSubviews(wView*view){
    wView*subview=NULL;
    for (int i=wArrayGetCount(view[0].subviews)-1; i>=0; i--) {
        subview=wArrayGetItemAtIndex(view[0].subviews, i);
        subview[0].superView=NULL;}
    wArrayRemoveAllItems(view[0].subviews);
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}

void wViewRemoveFromSuperView(wView*view){
    if (view[0].superView) {
        wViewRemoveSubview(view[0].superView, view);
        view[0].superView=NULL;}
}

void wViewRotateOnXAxis(wView*view, float angle){
    wMatrix4 rMat=wMat4RotateOnXAxis(view[0].matrix, angle);
    view[0].matrix=rMat;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}
}

void wViewRotateOnYAxis(wView*view, float angle){
    wMatrix4 rMat=wMat4RotateOnYAxis(view[0].matrix, angle);
    view[0].matrix=rMat;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}
}

void wViewRotateOnZAxis(wView*view, float angle){
    wMatrix4 rMat=wMat4RotateOnZAxis(view[0].matrix, angle);
    view[0].matrix=rMat;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}
}

void wViewHide(wView*view){
    wViewSetIsHidden(view, true);
}

void wViewShow(wView*view){
    wViewSetIsHidden(view, false);
}

void wViewRelease(wView*view){
    wViewRemoveFromSuperView(view);
}

#pragma mark - wViewSetter.
void wViewSetBackgroundColor(wView*view, wColor backgroundColor){
    view[0].backgroundColor=backgroundColor;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}


wRect wRectAutoLayoutUsingOptions(wRect subjectFrame,wAutoLayoutOptions options, wRect oldFrame, wRect newFrame,bool resizeWidth, bool resizeHeight){
    wRect newSubjectFrame=subjectFrame;
    
    if (resizeWidth) {
        bool widthChanged=false;
        float widthDiff=newFrame.size.width-oldFrame.size.width;
        if (options.widthOption==wAutoLayoutStretchToSuperView) {
            newSubjectFrame.size.width+=widthDiff;
            widthChanged=true;
        }else if (options.widthOption==wAutoLayoutProportionateToSuperView){
            newSubjectFrame.size.width*=newFrame.size.width;
            newSubjectFrame.size.width/=oldFrame.size.width;
            widthChanged=true;
        }
                
        newSubjectFrame.origin.x+=(options.anchor.x*widthDiff);
        if (widthChanged) {
            newSubjectFrame.origin.x-=(options.anchor.x*(newSubjectFrame.size.width-subjectFrame.size.width));
            if (options.heightOption==wAutoLayoutProportionateToWidth){
                newSubjectFrame.size.height*=newSubjectFrame.size.width;
                newSubjectFrame.size.height/=subjectFrame.size.width;
                newSubjectFrame.origin.y-=(options.anchor.y*(newSubjectFrame.size.height-subjectFrame.size.height));
            }
        }
    }
            
    if (resizeHeight) {
        bool heightChanged=false;
        float heightDiff=newFrame.size.height-oldFrame.size.height;
        if (options.heightOption==wAutoLayoutStretchToSuperView) {
            newSubjectFrame.size.height+=heightDiff;
            heightChanged=true;
        }else if (options.heightOption==wAutoLayoutProportionateToSuperView){
            newSubjectFrame.size.height*=newFrame.size.height;
            newSubjectFrame.size.height/=oldFrame.size.height;
            heightChanged=true;
        }
        
        newSubjectFrame.origin.y+=(options.anchor.y*heightDiff);
        if (heightChanged) {
            newSubjectFrame.origin.y-=(options.anchor.y*(newSubjectFrame.size.height-subjectFrame.size.height));
            if (options.widthOption==wAutoLayoutProportionateToHeight) {
                newSubjectFrame.size.width*=newSubjectFrame.size.height;
                newSubjectFrame.size.width/=subjectFrame.size.height;
                newSubjectFrame.origin.x-=(options.anchor.x*(newSubjectFrame.size.width-subjectFrame.size.width));
            }
        }
    }
    
    return newSubjectFrame;
}

void wViewSetFrame(wView*view, wRect frame){
    if (view[0].resizesSubviews) {
        bool resizeWidth=false,resizeHeight=false;
        if (view[0].frame.size.width!=frame.size.width) {resizeWidth=true;}
        if (view[0].frame.size.height!=frame.size.height) {resizeHeight=true;}
        
        if (resizeWidth||resizeHeight) {
            for (int i=wArrayGetCount(view[0].subviews)-1; i>=0; i--) {
                wView*subView=wArrayGetItemAtIndex(view[0].subviews, i);
                wRect Frame=wRectAutoLayoutUsingOptions(subView[0].frame, subView[0].autoLayoutOptions, view[0].frame, frame, resizeWidth, resizeHeight);
                wViewSetFrame(subView, Frame);
            }
        }
    }
    
    view[0].frame=frame;
    view[0].centerPoint.x=view[0].frame.origin.x+(view[0].frame.size.width/2.0);
    view[0].centerPoint.y=view[0].frame.origin.y+(view[0].frame.size.height/2.0);
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}
}
void wViewSetCenterPoint(wView*view, wPoint centerPoint){
    view[0].centerPoint=centerPoint;
    view[0].frame.origin.x=view[0].centerPoint.x-(view[0].frame.size.width/2.0);
    view[0].frame.origin.y=view[0].centerPoint.y-(view[0].frame.size.height/2.0);
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}
void wViewSetAnchorPoint(wView*view, wPoint anchorPoint){
    view[0].anchorPoint=anchorPoint;}
void wViewSetIsHidden(wView*view, bool hidden){
    view[0].isHidden=hidden;
    if (wViewIsOnScreen(view[0].superView)) {wViewRequestFrames(1);}}
void wViewSetClipsSubviews(wView*view, bool clipsSubviews){
    view[0].clipsSubviews=clipsSubviews;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}
void wViewSetIsOpaque(wView*view, bool isOpaque){
    view[0].isOpaque=isOpaque;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}
void wViewSetAlpha(wView*view, float alpha){
    view[0].alpha=alpha;
    if (view[0].alpha<0.0) {view[0].alpha=0.0;}
    else if (view[0].alpha>1.0){view[0].alpha=1.0;}
    
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}
void wViewSetMatrix(wView*view, wMatrix4 mat){
    view[0].matrix=mat;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}
void wViewSetPerspectiveProjection(wView*view, bool perspectiveProjection){
    view[0].perspectiveProjection=perspectiveProjection;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}
void wViewSetPerspectiveOption(wView*view, int perspectiveOption){
    view[0].perspectiveOption=perspectiveOption;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}
void wViewSetTag(wView*view, int tag){
    view[0].tag=tag;}
void wViewSetResizesSubviews(wView*view, bool resizesSubviews){
    view[0].resizesSubviews=resizesSubviews;
}
void wViewSetAutoLayoutOptions(wView*view, wAutoLayoutOptions options){
    view[0].autoLayoutOptions=options;
}
void wViewSetRenderOpenGLFunction(wView*view, void(*renderOpenGLFunction)()){
    view[0].renderOpenGLCode=renderOpenGLFunction;
}
void wViewSetOpenGLData(wView*view, void*data){
    view[0].openGLData=data;
}
void wViewSetUserInteractionEnabled(wView*view, bool userInteractioEnabled){
    view[0].userInteractioEnabled=userInteractioEnabled;
}
void wViewSetBackgroundImage(wView*view, wTexture*image){
     view[0].backgroundImage=image;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}
void wViewSetBackgroundMask(wView*view, wTexture*mask){
    view[0].backgroundMask=mask;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}
void wViewSetMultiTouchEnabled(wView*view, bool multiTouchEnabled){
    view[0].multiTouchEnabled=multiTouchEnabled;}
void wViewSetShouldDraw(wView*view, bool shouldDraw){
    view[0].shouldDraw=shouldDraw;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}}
void wViewSetBackgroundImageDisplayMode(wView*view, wViewBackgroundImageDisplayMode mode){
    view[0].backgroundImageDisplayMode=mode;
    if (wViewIsOnScreen(view)) {wViewRequestFrames(1);}
}


#pragma mark - wViewGetter.
wPoint wViewGetCenterPoint(wView*view){return view[0].centerPoint;}
wPoint wViewGetAnchorPoint(wView*view){return view[0].anchorPoint;}
wColor wViewGetBackgroundColor(wView*view){return view[0].backgroundColor;}
wRect wViewGetFrame(wView*view){return view[0].frame;}
wRect wViewGetAbsoluteFrame(wView*view){
    wRect frame=view[0].renderingContext.frame;
    if (wDeviceOrientationIsLandscape(wDeviceGetOrientation())) {
        wRect screenFrame=wScreenGetLandscapeFrame();
        float offset=(screenFrame.size.width-screenFrame.size.height)/2.0;
        frame.origin.x+=offset;
        frame.origin.y-=offset;
    }
    return frame;
}
bool wViewGetIsHidden(wView*view){return view[0].isHidden;}
bool wViewGetClipsSubviews(wView*view){return view[0].clipsSubviews;}
bool wViewGetIsOpaque(wView*view){return view[0].isOpaque;}
bool wViewGetAlpha(wView*view){return view[0].alpha;}
wMatrix4 wViewGetMatrix(wView*view){return view[0].matrix;}
wMatrix4 wViewGetAbsoluteMatrix(wView*view){
    return view[0].renderingContext.matrix;}
bool wViewGetPerspectiveProjection(wView*view){
    return view[0].perspectiveProjection;}
int wViewGetPerspectiveOption(wView*view){return view[0].perspectiveOption;}
int wViewGetType(wView*view){return view[0].type;}
int wViewGetTag(wView*view){return view[0].tag;}
bool wViewGetResizesSubviews(wView*view){return view[0].resizesSubviews;}
wAutoLayoutOptions wViewGetAutoLayoutOptions(wView*view){return view[0].autoLayoutOptions;}
bool wViewGetUserInteractionEnabled(wView*view){
    return view[0].userInteractioEnabled;}
int wViewGetSubviewCount(wView*view){return wArrayGetCount(view[0].subviews);}
wView*wViewGetSubviewAtIndex(wView*view, int index){
    return wArrayGetItemAtIndex(view[0].subviews, index);}
wTexture*wViewGetBackgroundImage(wView*view){
    return view[0].backgroundImage;}
wTexture*wViewGetBackgroundMask(wView*view){
    return view[0].backgroundMask;}
wArray*wViewGetSubviewArray(wView*view){
    return view[0].subviews;}
void*wViewGetOpenGLData(wView*view){
    return view[0].openGLData;}
bool wViewGetMultiTouchEnabled(wView*view){
    return view[0].multiTouchEnabled;}
bool wViewIsOnScreen(wView*view){
    if (!view) {return false;}
    wView*CurrentView=view;
    while (CurrentView!=wRootView) {
        if (CurrentView[0].isHidden) {return false;}
        if (!CurrentView[0].superView) {return false;}
        CurrentView=CurrentView[0].superView;
    }
    return true;
}
bool wViewGetShouldDraw(wView*view){
    return view[0].shouldDraw;}
wViewBackgroundImageDisplayMode wViewGetBackgroundImageDisplayMode(wView*view){
    return view[0].backgroundImageDisplayMode;
}


#pragma mark - wViewRendering.
GLuint wViewLoadShader(const char*shaderSrc, GLenum shaderType) {
    GLuint Shader=glCreateShader(shaderType);
    glShaderSource(Shader, 1, &shaderSrc, NULL);
    glCompileShader(Shader);
    
    GLint Compiled;
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &Compiled);
    if(!Compiled) {
    if (shaderType==GL_VERTEX_SHADER) {
        printf("\nError: Failed to compile wView vertex shader!");}
    else if (shaderType==GL_FRAGMENT_SHADER){
        printf("\nError: Failed to compile wView fragment shader!");}
    glDeleteShader(Shader);return 0;}
    
    return Shader;
}

bool wViewInitializeProgram(){
    char VSSrc[]=
    "uniform vec4 deviceDimentions;\n"
    "uniform vec2 projectionEquation;\n"
    "attribute vec4 vertexLocation;\n"
    "attribute vec2 textureCoordinates;\n"
    "varying vec2 textureData;\n"
    "void main(){\n"
    "    vec4 point=vertexLocation/deviceDimentions;\n"
    "    point.xyz-=1.0,point.y=-point.y;\n"
    "    point.w=((-point.z)*projectionEquation[0])+projectionEquation[1];\n"
    "    point.z*=projectionEquation[0];\n"
    "    point.xy*=point.w;\n"
    "    gl_Position=point;\n"
    "    textureData=textureCoordinates;\n"
    "}";
 
    char FSColorSrc[]=
    "precision highp float;\n"
    "uniform sampler2D texture;\n"
    "uniform sampler2D mask;\n"
    "uniform vec4 color;\n"
    "uniform float alpha;\n"
    "varying vec2 textureData;\n"
    "void main(){\n"
    "    vec4 msk=texture2D(mask, textureData);\n"
    "    gl_FragColor=color;\n"
    "    gl_FragColor.a*=(msk.a*alpha);\n"
    "}";
    
    char FSTextureSrc[]=
    "precision highp float;\n"
    "uniform sampler2D texture;\n"
    "uniform sampler2D mask;\n"
    "uniform vec4 color;\n"
    "uniform float alpha;\n"
    "varying vec2 textureData;\n"
    "void main(){\n"
    "    vec4 tex=texture2D(texture, textureData);\n"
    "    vec4 msk=texture2D(mask, textureData);\n"
    "    gl_FragColor=tex;\n"
    "    gl_FragColor.a*=(msk.a*alpha);\n"
    "}";
    
    char FSColorAndTextureSrc[]=
    "precision highp float;\n"
    "uniform sampler2D texture;\n"
    "uniform sampler2D mask;\n"
    "uniform vec4 color;\n"
    "uniform float alpha;\n"
    "varying vec2 textureData;\n"
    "void main(){\n"
    "    vec4 tex=texture2D(texture, textureData);\n"
    "    vec4 msk=texture2D(mask, textureData);\n"
    
    "    vec4 fore=tex, back=color, result=vec4(0.0);\n"
    "    float OneMfA=1.0-fore.a;\n"
    "    result.a=fore.a+(back.a*OneMfA);\n"
    "    result.rgb=((fore.rgb*fore.a)+(back.rgb*back.a*OneMfA))/result.a;\n"
    
    "    gl_FragColor=result;\n"
    "    gl_FragColor.a*=(msk.a*alpha);\n"
    "}";
    
    GLuint VShader=wViewLoadShader(VSSrc, GL_VERTEX_SHADER);
    GLuint FColorShader=wViewLoadShader(FSColorSrc, GL_FRAGMENT_SHADER);
    GLuint FTextureShader=wViewLoadShader(FSTextureSrc, GL_FRAGMENT_SHADER);
    GLuint FColorAndTextureShader=wViewLoadShader(FSColorAndTextureSrc, GL_FRAGMENT_SHADER);
    
    wViewOGLColorProgram=glCreateProgram();
    glAttachShader(wViewOGLColorProgram, VShader);
    glAttachShader(wViewOGLColorProgram, FColorShader);
    glBindAttribLocation(wViewOGLColorProgram, 0, "vertexLocation");
    glBindAttribLocation(wViewOGLColorProgram, 1, "textureCoordinates");
    glLinkProgram(wViewOGLColorProgram);
    
    wViewOGLTextureProgram=glCreateProgram();
    glAttachShader(wViewOGLTextureProgram, VShader);
    glAttachShader(wViewOGLTextureProgram, FTextureShader);
    glBindAttribLocation(wViewOGLTextureProgram, 0, "vertexLocation");
    glBindAttribLocation(wViewOGLTextureProgram, 1, "textureCoordinates");
    glLinkProgram(wViewOGLTextureProgram);
    
    wViewOGLColorAndTextureProgram=glCreateProgram();
    glAttachShader(wViewOGLColorAndTextureProgram, VShader);
    glAttachShader(wViewOGLColorAndTextureProgram, FColorAndTextureShader);
    glBindAttribLocation(wViewOGLColorAndTextureProgram,0,"vertexLocation");
    glBindAttribLocation(wViewOGLColorAndTextureProgram,1,"textureCoordinates");
    glLinkProgram(wViewOGLColorAndTextureProgram);
    
    GLint linked;
    glGetProgramiv(wViewOGLColorProgram, GL_LINK_STATUS, &linked);
    if(!linked){
    printf("\nError: Failed to link wView Color program!");
    glDeleteProgram(wViewOGLColorProgram);
    return false;}
    
    glGetProgramiv(wViewOGLTextureProgram, GL_LINK_STATUS, &linked);
    if(!linked){
    printf("\nError: Failed to link wView Texture program!");
    glDeleteProgram(wViewOGLTextureProgram);
    return false;}
    
    glGetProgramiv(wViewOGLColorAndTextureProgram, GL_LINK_STATUS, &linked);
    if(!linked){
    printf("\nError: Failed to link wView Color & Texture program!");
    glDeleteProgram(wViewOGLColorAndTextureProgram);
    return false;}
    
    return true;
}

void wViewSetupRenderingComponents(wSize screenSize){
    wViewInitializeProgram();
    
    wViewDefaults.matrix=wMat4Identity;
    wViewDefaults.backgroundColor=wColorBlack();
    wViewDefaults.autoLayoutOptions=wAutoLayoutNone;
    wViewDefaults.alpha=1.0;
    wViewDefaults.perspectiveOption=wPerspectiveOptionLocal;
    wViewDefaults.isOpaque=true;
    wViewDefaults.isHidden=false;
    wViewDefaults.clipsSubviews=false;
    wViewDefaults.perspectiveProjection=false;
    wViewDefaults.resizesSubviews=true;
    wViewDefaults.userInteractioEnabled=false;
    wViewDefaults.multiTouchEnabled=false;
    wViewDefaults.shouldDraw=true;
    
    wScreenWidth=screenSize.width;
    wScreenHeight=screenSize.height;
    wScreenDepth=wScreenHeight*2.0;
    wScreenCenter=wPointNew(wScreenWidth/2.0, wScreenHeight/2.0);
    wScreenPortraitFrame=wRectNew(0.0, 0.0, wScreenWidth, wScreenHeight);
    wScreenLandscapeFrame=wRectNew(0.0, 0.0, wScreenHeight, wScreenWidth);

    colorLocationC=glGetUniformLocation(wViewOGLColorProgram, "color");
    alphaLocationC=glGetUniformLocation(wViewOGLColorProgram, "alpha");
    projectionEquationLocationC=glGetUniformLocation(wViewOGLColorProgram, "projectionEquation");
    
    colorLocationT=glGetUniformLocation(wViewOGLTextureProgram, "color");
    alphaLocationT=glGetUniformLocation(wViewOGLTextureProgram, "alpha");
    projectionEquationLocationT=glGetUniformLocation(wViewOGLTextureProgram, "projectionEquation");
    
    colorLocationCaT=glGetUniformLocation(wViewOGLColorAndTextureProgram, "color");
    alphaLocationCaT=glGetUniformLocation(wViewOGLColorAndTextureProgram, "alpha");
    projectionEquationLocationCaT=glGetUniformLocation(wViewOGLColorAndTextureProgram, "projectionEquation");

    
    GLint deviceDimensionsLocation, textureLocation, maskLocation;
    glUseProgram(wViewOGLColorProgram);
    deviceDimensionsLocation=glGetUniformLocation(wViewOGLColorProgram, "deviceDimentions");
    textureLocation=glGetUniformLocation(wViewOGLColorProgram, "texture");
    maskLocation=glGetUniformLocation(wViewOGLColorProgram, "mask");
    glUniform1i(textureLocation, 0);
    glUniform1i(maskLocation, 1);
    glUniform4f(deviceDimensionsLocation, wScreenCenter.x, wScreenCenter.y, wScreenDepth, 1.0);
    
    glUseProgram(wViewOGLTextureProgram);
    deviceDimensionsLocation=glGetUniformLocation(wViewOGLTextureProgram, "deviceDimentions");
    textureLocation=glGetUniformLocation(wViewOGLTextureProgram, "texture");
    maskLocation=glGetUniformLocation(wViewOGLTextureProgram, "mask");
    glUniform1i(textureLocation, 0);
    glUniform1i(maskLocation, 1);
    glUniform4f(deviceDimensionsLocation, wScreenCenter.x, wScreenCenter.y, wScreenDepth, 1.0);
    
    glUseProgram(wViewOGLColorAndTextureProgram);
    deviceDimensionsLocation=glGetUniformLocation(wViewOGLColorAndTextureProgram, "deviceDimentions");
    textureLocation=glGetUniformLocation(wViewOGLColorAndTextureProgram, "texture");
    maskLocation=glGetUniformLocation(wViewOGLColorAndTextureProgram, "mask");
    glUniform1i(textureLocation, 0);
    glUniform1i(maskLocation, 1);
    glUniform4f(deviceDimensionsLocation, wScreenCenter.x, wScreenCenter.y, wScreenDepth, 1.0);
    
    baseTexture=createOnePixelTexture(0.0,0.0,0.0,0.0);
    baseMask=createOnePixelTexture(0.0,0.0,0.0,1.0);
    wVertexArrayTemp.list=malloc(sizeof(wVertex)*10);
    wVertexArrayTemp.count=0;
    wVertexArrayTemp.capacity=10;
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, baseTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, baseMask);
    
    wView*rootPointer=wViewNewWithFrame(wScreenPortraitFrame);
    wRootViewContainer=rootPointer[0];
    free(rootPointer);
    wViewRequestFrames(1);
    
    wAutoRotationSetAllowsOrientation(wDeviceOrientationPortraitUp, true);
    wAutoRotationSetAllowsOrientation(wDeviceOrientationPortraitDown, false);
    wAutoRotationSetAllowsOrientation(wDeviceOrientationLandscapeLeft, false);
    wAutoRotationSetAllowsOrientation(wDeviceOrientationLandscapeRight, false);
    
    wTimerMasterArray=wArrayNewWithCapacity(10);
}

unsigned int requiredFrames=0;
bool renderUnconditionally=false;

void wViewRequestFrames(unsigned int frameCount){
    if (frameCount>requiredFrames) {
        unsigned int diff=frameCount-requiredFrames;
        requiredFrames+=diff;
    }
}
void wViewSetRenderIndefinitly(bool renderIndefinitly){
    renderUnconditionally=renderIndefinitly;
}
bool wViewGetRenderIndefinitly(){
    return renderUnconditionally;
}

#define wViewNilProgram 0
#define wViewColorProgram 1
#define wViewTextureProgram 2
#define wViewColorAndTextureProgram 3
int wViewProgramInUse=wViewNilProgram;
GLint projectionEquationLocation, colorLocation, alphaLocation;
void setProgramInUse(int program){
    if (wViewProgramInUse!=program) {
        wViewProgramInUse=program;
        switch (program) {
            case wViewColorProgram:{
                projectionEquationLocation=projectionEquationLocationC;
                colorLocation=colorLocationC;
                alphaLocation=alphaLocationC;
                glUseProgram(wViewOGLColorProgram);
                break;}
            case wViewTextureProgram:{
                projectionEquationLocation=projectionEquationLocationT;
                colorLocation=colorLocationT;
                alphaLocation=alphaLocationT;
                glUseProgram(wViewOGLTextureProgram);
                break;}
            case wViewColorAndTextureProgram:{
                projectionEquationLocation=projectionEquationLocationCaT;
                colorLocation=colorLocationCaT;
                alphaLocation=alphaLocationCaT;
                glUseProgram(wViewOGLColorAndTextureProgram);
                break;}
            default:{
                projectionEquationLocation=projectionEquationLocationCaT;
                colorLocation=colorLocationCaT;
                alphaLocation=alphaLocationCaT;
                glUseProgram(wViewOGLColorAndTextureProgram);
                wViewProgramInUse=wViewColorAndTextureProgram;
                break;}
        }
    }
}

bool wViewRender(wView*view){
    if (view[0].isHidden) {return false;}
    if (!view[0].isOpaque&&view[0].alpha<=0.0) {return false;}
    
    wView*superView=view[0].superView;
    wViewRenderingContext*svRC=&view[0].superView[0].renderingContext;
    wViewRenderingContext*RC=&view[0].renderingContext;
    RC[0].frame=view[0].frame;
    RC[0].centerPoint=view[0].centerPoint;
    RC[0].matrix=view[0].matrix;
    RC[0].anchorPoint=view[0].anchorPoint;
    RC[0].alpha=view[0].alpha;
    if (!superView[0].isOpaque) {RC[0].alpha*=svRC[0].alpha;}
    
    
    bool shouldBeClipped=false;
    if (superView[0].clipsSubviews) {RC[0].clippingView=superView;}
    else{RC[0].clippingView=svRC[0].clippingView;}
    if (RC[0].clippingView) {
    if (RC[0].clippingView[0].clipsSubviews) {shouldBeClipped=true;}}
    
    
    RC[0].frame.origin=wPointAddPoints(RC[0].frame.origin,svRC[0].frame.origin);
    RC[0].centerPoint=wPointAddPoints(RC[0].centerPoint, svRC[0].frame.origin);
    
    if (superView[0].type==wViewTypeScrollView ||
        superView[0].type==wViewTypeTableView) {
        RC[0].frame.origin.x-=superView[0].scrollContext[0].contentOffset.x;
        RC[0].frame.origin.y-=superView[0].scrollContext[0].contentOffset.y;
        RC[0].centerPoint.x-=superView[0].scrollContext[0].contentOffset.x;
        RC[0].centerPoint.y-=superView[0].scrollContext[0].contentOffset.y;
    }
    
    RC[0].anchorPoint.x*=RC[0].frame.size.width;
    RC[0].anchorPoint.y*=RC[0].frame.size.height;
    RC[0].anchorPoint=wPointAddPoints(RC[0].anchorPoint, RC[0].frame.origin);
    
    RC[0].texCoordinates[0]=0.0;
    RC[0].texCoordinates[1]=1.0;
    RC[0].texCoordinates[2]=0.0;
    RC[0].texCoordinates[3]=1.0;
    
    if (view[0].backgroundImage) {
        if (view[0].backgroundImageDisplayMode == wViewBackgroundImageDisplayModeCrop) {
            wViewClipTextureCoordinates(wSizeNew(view[0].backgroundImage[0].width, view[0].backgroundImage[0].height),  RC[0].frame.size, &RC[0].texCoordinates[0], &RC[0].texCoordinates[1], &RC[0].texCoordinates[2], &RC[0].texCoordinates[3]);
        }
    }

    bool hasTransformation=!wMat4IsIdentity(view[0].matrix);
    RC[0].branchHasTransformation=hasTransformation ||
                                  svRC[0].branchHasTransformation;
    
    bool simpleClipped=false;
    if (!RC[0].branchHasTransformation) {
        if (shouldBeClipped) {simpleClipped=true;
        wVertexArraySimpleClip(&RC[0].vertices,RC[0].frame, RC[0].texCoordinates,
                               RC[0].clippingView[0].renderingContext.frame);}
    }
    if (!simpleClipped){
        wVertexArrayCreateFromRect(&RC[0].vertices,RC[0].frame, RC[0].texCoordinates);
    }
    
    
    if (hasTransformation) {
        RC[0].matrix=wMat4AnchorToPoint(RC[0].matrix, RC[0].anchorPoint);
        RC[0].matrix=wMat4MultiplyByMat4(svRC[0].matrix, RC[0].matrix);}
    else{RC[0].matrix=svRC[0].matrix;}
    wVertexArrayMultiplyByMat4(&RC[0].vertices, RC[0].matrix);
    

    
    wVec2 projectionEquation=orthEquation;
    if (RC[0].branchHasTransformation) {
        if (view[0].perspectiveProjection) {
            wPoint worldCenter=wScreenCenter;
            if (view[0].perspectiveOption==wPerspectiveOptionLocal) {
                worldCenter=RC[0].centerPoint;}
            else if (view[0].perspectiveOption==wPerspectiveOptionSuperView) {
                worldCenter=svRC[0].centerPoint;}
            
            wVertexArrayApplyPerspective(&RC[0].vertices,worldCenter);
            projectionEquation=persEquation;
        }
        
        if (shouldBeClipped) {
            wVertexArrayClip(&RC[0].vertices,
                             &RC[0].clippingView[0].renderingContext.vertices);}
    }
    

    bool hasArea=RC[0].vertices.count>2;
    if (hasArea) {hasArea=!wVertexArrayIsAStraightLine(&RC[0].vertices);}
    bool renders=hasArea;
    bool renderSubviews=true;
    
    if (view[0].clipsSubviews) {
        renderSubviews=renders;
        if (!wVertexArrayIsClockwise(&RC[0].vertices) && renderSubviews) {
            wVertexArrayReverse(&RC[0].vertices);}
    }
    
    bool hasColor=view[0].backgroundColor.a>0.0;
    bool hasTexture=view[0].backgroundImage;
    if (hasColor && hasTexture) {setProgramInUse(wViewColorAndTextureProgram);}
    else if (hasColor){setProgramInUse(wViewColorProgram);}
    else if (hasTexture){setProgramInUse(wViewTextureProgram);}
    else{renders=false;}
    
    if (renders && view[0].shouldDraw) {
        if (view[0].isOpaque) {glDisable(GL_BLEND);}
        else{glEnable(GL_BLEND);}
        
        if(view[0].backgroundImage){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, view[0].backgroundImage[0].texture);}
        else{glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, baseTexture);}
        
        if(view[0].backgroundMask){
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, view[0].backgroundMask[0].texture);}
        else{glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, baseMask);}
        
        glUniform2f(projectionEquationLocation, projectionEquation.x,projectionEquation.y);
        
        wColor color=view[0].backgroundColor;
        glUniform4f(colorLocation,color.r,color.g,color.b, color.a);
    
        glUniform1f(alphaLocation, RC[0].alpha);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(wVertex), &RC[0].vertices.list[0].location.x);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(wVertex), &RC[0].vertices.list[0].textureCoordinates.x);
        
        glDrawArrays(GL_TRIANGLE_FAN, 0, RC[0].vertices.count);
    }
       
    if (view[0].renderOpenGLCode) {
        view[0].renderOpenGLCode(view[0].openGLData);
        int currentProgram=wViewProgramInUse;
        wViewProgramInUse=wViewNilProgram;
        setProgramInUse(currentProgram);}
    
    if (renderSubviews){
    wView*subview=NULL;
    int subviewCount=wArrayGetCount(view[0].subviews);
    for (int i=0; i<subviewCount; i++) {
    subview=wArrayGetItemAtIndex(view[0].subviews, i);
    wViewRender(subview);}}
        
    return true;
}

typedef struct _wTimer{
    pthread_t thread;
    float duration;
    wFunction function;
    void*userData;
    bool stop;
    bool isRunning;
    bool repeat;
    bool conditionMet;
    bool shouldRelease;
}wTimer;

bool wViewRenderRoot(){
    int timerCount=wArrayGetCount(wTimerMasterArray);
    for (int i=timerCount-1; i>=0; i--) {
        wTimer*timer=wArrayGetItemAtIndex(wTimerMasterArray, i);
        if (timer[0].conditionMet) {
            wTimerExecuteFunction(timer);
            timer[0].conditionMet=false;
        }
        if (timer[0].shouldRelease) {
            pthread_join(timer[0].thread, NULL);
            free(timer),timer=NULL;
            wArrayRemoveItemAtIndex(wTimerMasterArray, i);
        }
    }
    
    if (requiredFrames>0) {requiredFrames--;}
    else if (!renderUnconditionally) {return false;}
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthRangef(1.0, 0.0); /// far waz -1.0
    wColor color=wRootView[0].backgroundColor;
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
    
    wAnimationUpdate();
    wAutoRotationAnimation();
    wRootView[0].renderingContext.frame=wRootView[0].frame;
    wRootView[0].renderingContext.centerPoint=wScreenCenter;
    wRootView[0].renderingContext.alpha=1.0;
    wRootView[0].renderingContext.matrix=wMat4AnchorToPoint(wAutoRotationMatrix, wScreenCenter);
    
    wView*subview=NULL;
    int subviewCount=wArrayGetCount(wRootView[0].subviews);
    for (int i=0; i<subviewCount; i++) {
    subview=wArrayGetItemAtIndex(wRootView[0].subviews, i);
    wViewRender(subview);}
    return true;
}





void printfmat4(wMatrix4 mat){
    printf("%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n\n\n",
           mat.m11,mat.m12,mat.m13,mat.m14,
           mat.m21,mat.m22,mat.m23,mat.m24,
           mat.m31,mat.m32,mat.m33,mat.m34,
           mat.m41,mat.m42,mat.m43,mat.m44);
}