//
//  PKFilterManager.c
//  PhotoKit
//
//  Created by Wael Youssef on 6/25/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "PKFilterManager.h"
#include "PKShiftFilters.h"

GLuint pkOGLProgram;
wTexture*targetTexture=NULL;

bool PurchasedPacks[30]={false};

GLuint loadShader(const char*shaderSrc, GLenum shaderType) {
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

int deviceDimensionsLocation;
bool initializeProgram(){
    char VSSrc[]=
    "uniform vec2 deviceDimentions;\n"
    "attribute vec2 vertexLocation;\n"
    "attribute vec2 textureCoordinates;\n"
    "varying vec2 textureData;\n"
    "void main(){\n"
    "    vec2 point=vertexLocation/deviceDimentions;\n"
    "    point.xy-=1.0,point.y=-point.y;\n"
    "    gl_Position=vec4(point,0.0,1.0);\n"
    "    textureData=textureCoordinates;\n"
    "}";
    
    char FSSrc[]=
    "precision highp float;\n"
    "uniform sampler2D texture;\n"
    "varying vec2 textureData;\n"
    "void main(){\n"
    "    vec4 tex=texture2D(texture, textureData);\n"
    "    gl_FragColor=tex;\n"
    "}";
    
    GLuint VShader=loadShader(VSSrc, GL_VERTEX_SHADER);
    GLuint FShader=loadShader(FSSrc, GL_FRAGMENT_SHADER);
    
    pkOGLProgram=glCreateProgram();
    glAttachShader(pkOGLProgram, VShader);
    glAttachShader(pkOGLProgram, FShader);
    glBindAttribLocation(pkOGLProgram, 0, "vertexLocation");
    glBindAttribLocation(pkOGLProgram, 1, "textureCoordinates");
    glLinkProgram(pkOGLProgram);
    
    glUseProgram(pkOGLProgram);
    
    GLint linked;
    glGetProgramiv(pkOGLProgram, GL_LINK_STATUS, &linked);
    if(!linked){
        printf("\nError: Failed to link wView program!");
        glDeleteProgram(pkOGLProgram);
        return false;}
    
    deviceDimensionsLocation=glGetUniformLocation(pkOGLProgram, "deviceDimentions");
    wRect portraitFrame=wScreenGetPortraitFrame();
    PKFilterManagerSetDeviceDimentions(portraitFrame.size);
    return true;
}

void PKFilterManagerSetDeviceDimentions(wSize dimentions){
    glUseProgram(pkOGLProgram);
    wPoint screenCenter=wPointNew(dimentions.width/2.0, dimentions.height/2.0);
    glUniform2f(deviceDimensionsLocation, screenCenter.x, screenCenter.y);
}

void PKFilterManagerSetupWithTargetTexture(wTexture*texture){
    targetTexture=texture;
    PKShiftInitFilters();
    initializeProgram();
}

void*PKFilterManagerGetTargetTexture(){
    return targetTexture;
}


void scaleTextureCoordinates(bool isWidth, float scale, ShiftVertexArray*vertices){
    if (isWidth) {
        for (int i=0; i<vertices[0].count; i++) {
            vertices[0].list[i].textureCoordinates.x=((vertices[0].list[i].textureCoordinates.x-0.5)*scale)+0.5;
        }
    }
    else {
        for (int i=0; i<vertices[0].count; i++) {
            vertices[0].list[i].textureCoordinates.y=((vertices[0].list[i].textureCoordinates.y-0.5)*scale)+0.5;
        }
    }
}

void clipTextureCoordinates(wSize textureSize, wSize targetSize, ShiftVertexArray*vertices){
    
    float newSize;
    float ScreenHeight=targetSize.height;
    float ScreenWidth=targetSize.width;
    float ImageWidth=textureSize.width;
    float ImageHeight=textureSize.height;
        
    if (ScreenWidth>ScreenHeight) {
        if (ImageWidth>ImageHeight) {
            if (ImageWidth/ImageHeight < ScreenWidth/ScreenHeight) {
                newSize=ScreenWidth*(ImageHeight/ImageWidth);
                scaleTextureCoordinates(false, ScreenHeight/newSize, vertices);}
            else {newSize=ScreenHeight*(ImageWidth/ImageHeight);
                scaleTextureCoordinates(true, ScreenWidth/newSize, vertices);}
        }
        else {
            if (ImageHeight/ImageWidth < ScreenHeight/ScreenWidth) {
                newSize=ScreenHeight*(ImageWidth/ImageHeight);
                scaleTextureCoordinates(true, ScreenWidth/newSize, vertices);}
            else {newSize=ScreenWidth*(ImageHeight/ImageWidth);
                scaleTextureCoordinates(false, ScreenHeight/newSize, vertices);}
        }
    }
    else {
        if (ImageWidth>ImageHeight) {
            if (ImageWidth/ImageHeight < ScreenWidth/ScreenHeight) {
                newSize=ScreenWidth*(ImageHeight/ImageWidth);
                scaleTextureCoordinates(false, ScreenHeight/newSize, vertices);}
            else {newSize=ScreenHeight*(ImageWidth/ImageHeight);
                scaleTextureCoordinates(true, ScreenWidth/newSize, vertices);}
        }
        else {
            if (ImageHeight/ImageWidth < ScreenHeight/ScreenWidth) {
                newSize=ScreenHeight*(ImageWidth/ImageHeight);
                scaleTextureCoordinates(true, ScreenWidth/newSize, vertices);}
            else {newSize=ScreenWidth*(ImageHeight/ImageWidth);
                scaleTextureCoordinates(false, ScreenHeight/newSize, vertices);}
        }
    }
}

bool MirrorX=false;
bool MirrorY=false;
wDeviceOrientation Orientation=wDeviceOrientationPortraitUp;
void PKFilterManagerSetRenderTransformations(bool mirrorX, bool mirrorY, wDeviceOrientation orientation){
    MirrorX=mirrorX;
    MirrorY=mirrorY;
    Orientation=orientation;
}

void PKFilterManagerRenderToFrame(wRect frame, unsigned int filterTag){
    if (targetTexture) {
        glUseProgram(pkOGLProgram);
        
        wSize textureSize=wSizeNew(targetTexture[0].width, targetTexture[0].height);
        if (wDeviceOrientationIsLandscape(Orientation)) {
            textureSize=wSizeNew(targetTexture[0].height, targetTexture[0].width);
        }
        
        float minX=frame.origin.x;
        float minY=frame.origin.y;
        float maxX=minX+frame.size.width;
        float maxY=minY+frame.size.height;
        
        if (MirrorX) {maxX=frame.origin.x;
            minX=maxX+frame.size.width;}
        if (MirrorY) {maxY=frame.origin.y;
            minY=maxY+frame.size.height;}
        
        
        glDisable(GL_BLEND);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, targetTexture[0].texture);
        
        ShiftVertexArray*ShiftVertices=PKShiftFilterGetVerticiesForShiftID(filterTag);
        
        ShiftVertexArray vertices;
        vertices.drawStripes=ShiftVertices[0].drawStripes;
        vertices.count=ShiftVertices[0].count;
        vertices.capacity=ShiftVertices[0].capacity;
        vertices.list=malloc(sizeof(ShiftvVertex)*vertices.count);
    
        float size=1.0;
        for (int i=0; i<vertices.count; i++) {

            switch (Orientation) {
                case wDeviceOrientationPortraitUp:{
                    vertices.list[i].location.x=ShiftVertices[0].list[i].location.x;
                    vertices.list[i].location.y=ShiftVertices[0].list[i].location.y;
                    break;}
                case wDeviceOrientationPortraitDown:{
                    vertices.list[i].location.x=size-ShiftVertices[0].list[i].location.x;
                    vertices.list[i].location.y=size-ShiftVertices[0].list[i].location.y;
                    break;}
                case wDeviceOrientationLandscapeLeft:{
                    ShiftPoint p=ShiftVertices[0].list[i].location;
                    vertices.list[i].location.x=size-p.y;
                    vertices.list[i].location.y=p.x;
                    
                    break;}
                case wDeviceOrientationLandscapeRight:{
                    ShiftPoint p=ShiftVertices[0].list[i].location;
                    vertices.list[i].location.x=p.y;
                    vertices.list[i].location.y=size-p.x;
                    break;}
                default:
                    break;
            }
            
        }
        
        
        for (int i=0; i<vertices.count; i++) {
            vertices.list[i].location.x=interpolateFloat(minX, maxX, vertices.list[i].location.x);
            vertices.list[i].location.y=interpolateFloat(minY, maxY,  vertices.list[i].location.y);
            
            vertices.list[i].textureCoordinates.x=ShiftVertices[0].list[i].textureCoordinates.x;
            vertices.list[i].textureCoordinates.y=ShiftVertices[0].list[i].textureCoordinates.y;
        }
        
        

        
        
        clipTextureCoordinates(textureSize, frame.size, &vertices);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ShiftvVertex), &vertices.list[0].location.x);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ShiftvVertex), &vertices.list[0].textureCoordinates.x);
        if (vertices.drawStripes) {glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.count);}
        else{glDrawArrays(GL_TRIANGLES, 0, vertices.count);}
        
        free(vertices.list);
    }
    
}

void PKFilterManagerRenderFunction(void*data){
    wView*TargetView=data;
    unsigned int filterTag=wViewGetTag(TargetView);
    wRect frame=wViewGetAbsoluteFrame(TargetView);
    PKFilterManagerRenderToFrame(frame, filterTag);
}



int PKFilterManagerNumberOfPacks(){
    return 7;
}
int PKFilterManagerNumberOfFilters(){
    return 42;
}
int PKFilterManagerNumberOfFiltersPerPack(){
    return 6;
}
bool PKFilterManagerIsPackPurchased(int packID){
    if (packID<PKFilterManagerNumberOfPacks()) {
        return PurchasedPacks[packID];}
    else{return false;}
    
}
bool PKFilterManagerIsFilterPurchased(int filterID){
    int packID=PKFilterManagerPackIDFromFilterID(filterID);
    return PKFilterManagerIsPackPurchased(packID);
}
int PKFilterManagerPackIDFromFilterID(int filterID){
    return filterID/PKFilterManagerNumberOfFiltersPerPack();
}
