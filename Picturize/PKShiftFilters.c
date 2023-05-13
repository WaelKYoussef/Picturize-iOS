//
//  PKShiftFilters.c
//  PhotoKit
//
//  Created by Wael Youssef on 6/25/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "PKShiftFilters.h"

ShiftVertexArray Shifts[42];

void InitVertexArray(int index, bool drawStripes, int capacity){
    Shifts[index].drawStripes=drawStripes;
    Shifts[index].count=0;
    Shifts[index].capacity=capacity;
    Shifts[index].list=malloc(sizeof(ShiftvVertex)*capacity);
}

void VertexArrayAddVertex(int index, float locX, float locY, float tcX, float tcY){
    int v=Shifts[index].count;
    Shifts[index].list[v].location.x=locX;
    Shifts[index].list[v].location.y=locY;
    Shifts[index].list[v].textureCoordinates.x=tcX;
    Shifts[index].list[v].textureCoordinates.y=tcY;
    Shifts[index].count++;
}

void InitShift0(){
    int i=0;
    InitVertexArray(i, true, 4);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
}
void InitShift1(){
    int i=1;
    InitVertexArray(i, true, 6);
    VertexArrayAddVertex(i, 0.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 1.0,0.0);
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
}

void InitShift2(){
    int i=19;
    InitVertexArray(i, true, 4);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 0.0,1.0);
}

void InitShift4(){
    int i=13;
    InitVertexArray(i, true, 6);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.5 , 0.0,0.5);
    VertexArrayAddVertex(i, 1.0,0.5 , 1.0,0.5);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,1.0);
}

void InitShift5(){
    int i=26;
    InitVertexArray(i, true, 6);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,0.0);
    VertexArrayAddVertex(i, 0.0,0.5 , 0.0,0.5);
    VertexArrayAddVertex(i, 1.0,0.5 , 1.0,0.5);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
}

void InitShift6(){
    int i=6;
    float diff1=0.333333;
    float diff2=0.666666;
    InitVertexArray(i, false, 18);
    VertexArrayAddVertex(i, 0.0,1.0 , diff1,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , diff1,0.0);
    VertexArrayAddVertex(i, diff1,1.0 , diff2,1.0);
    
    VertexArrayAddVertex(i, 0.0,0.0 , diff1,0.0);
    VertexArrayAddVertex(i, diff1,1.0 , diff2,1.0);
    VertexArrayAddVertex(i, diff1,0.0 , diff2,0.0);
    
    
    VertexArrayAddVertex(i, diff1,1.0 , diff1,1.0);
    VertexArrayAddVertex(i, diff1,0.0 , diff1,0.0);
    VertexArrayAddVertex(i, diff2,1.0 , diff2,1.0);
    
    VertexArrayAddVertex(i, diff1,0.0 , diff1,0.0);
    VertexArrayAddVertex(i, diff2,1.0 , diff2,1.0);
    VertexArrayAddVertex(i, diff2,0.0 , diff2,0.0);
    
    
    VertexArrayAddVertex(i, diff2,1.0 , diff1,1.0);
    VertexArrayAddVertex(i, diff2,0.0 , diff1,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , diff2,1.0);
    
    VertexArrayAddVertex(i, diff2,0.0 , diff1,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , diff2,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , diff2,0.0);
}

void InitShift7(){
    int i=12;
    float diff1=0.333333;
    float diff2=0.666666;
    InitVertexArray(i, false, 18);
    VertexArrayAddVertex(i, 0.0,1.0 , diff2,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , diff2,0.0);
    VertexArrayAddVertex(i, diff1,1.0 , 1.0,1.0);
    
    VertexArrayAddVertex(i, 0.0,0.0 , diff2,0.0);
    VertexArrayAddVertex(i, diff1,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, diff1,0.0 , 1.0,0.0);
    
    
    VertexArrayAddVertex(i, diff1,1.0 , diff1,1.0);
    VertexArrayAddVertex(i, diff1,0.0 , diff1,0.0);
    VertexArrayAddVertex(i, diff2,1.0 , diff2,1.0);
    
    VertexArrayAddVertex(i, diff1,0.0 , diff1,0.0);
    VertexArrayAddVertex(i, diff2,1.0 , diff2,1.0);
    VertexArrayAddVertex(i, diff2,0.0 , diff2,0.0);
    
    
    VertexArrayAddVertex(i, diff2,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, diff2,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , diff1,1.0);
    
    VertexArrayAddVertex(i, diff2,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , diff1,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , diff1,0.0);
}

void InitShift8(){
    int i=18;
    InitVertexArray(i, false, 24);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.25,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.25,0.0);
    VertexArrayAddVertex(i, 0.25,1.0 , 0.5,1.0);
    
    VertexArrayAddVertex(i, 0.0,0.0 , 0.25,0.0);
    VertexArrayAddVertex(i, 0.25,1.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 0.25,0.0 , 0.5,0.0);
    
    
    VertexArrayAddVertex(i, 0.25,1.0 , 0.25,1.0);
    VertexArrayAddVertex(i, 0.25,0.0 , 0.25,0.0);
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,1.0);
    
    VertexArrayAddVertex(i, 0.25,0.0 , 0.25,0.0);
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,0.0);
    
    
    VertexArrayAddVertex(i, 0.5,1.0 , 0.25,1.0);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.25,0.0);
    VertexArrayAddVertex(i, 0.75,1.0 , 0.5,1.0);
    
    VertexArrayAddVertex(i, 0.5,0.0 , 0.25,0.0);
    VertexArrayAddVertex(i, 0.75,1.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 0.75,0.0 , 0.5,0.0);
    
    VertexArrayAddVertex(i, 0.75,1.0 , 0.25,1.0);
    VertexArrayAddVertex(i, 0.75,0.0 , 0.25,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 0.5,1.0);
    
    VertexArrayAddVertex(i, 0.75,0.0 , 0.25,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 0.5,0.0);
}

void InitShift9(){
    int i=24;
    InitVertexArray(i, false, 24);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.5,0.0);
    VertexArrayAddVertex(i, 0.25,1.0 , 0.75,1.0);
    
    VertexArrayAddVertex(i, 0.0,0.0 , 0.5,0.0);
    VertexArrayAddVertex(i, 0.25,1.0 , 0.75,1.0);
    VertexArrayAddVertex(i, 0.25,0.0 , 0.75,0.0);
    
    
    VertexArrayAddVertex(i, 0.25,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.25,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 0.5,1.0 , 0.25,1.0);
    
    VertexArrayAddVertex(i, 0.25,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 0.5,1.0 , 0.25,1.0);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.25,0.0);
    
    
    VertexArrayAddVertex(i, 0.5,1.0 , 0.75,1.0);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.75,0.0);
    VertexArrayAddVertex(i, 0.75,1.0 , 1.0,1.0);
    
    VertexArrayAddVertex(i, 0.5,0.0 , 0.75,0.0);
    VertexArrayAddVertex(i, 0.75,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.75,0.0 , 1.0,0.0);
    
    
    VertexArrayAddVertex(i, 0.75,1.0 , 0.25,1.0);
    VertexArrayAddVertex(i, 0.75,0.0 , 0.25,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 0.5,1.0);
    
    VertexArrayAddVertex(i, 0.75,0.0 , 0.25,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 0.5,0.0);
}

void InitShift10(){
    int i=30;
    float diff1=0.333333;
    float diff2=0.666666;
    InitVertexArray(i, false, 18);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,diff1);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,diff1);
    VertexArrayAddVertex(i, 0.0,diff1 , 0.0,diff2);
    
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,diff1);
    VertexArrayAddVertex(i, 0.0,diff1 , 0.0,diff2);
    VertexArrayAddVertex(i, 1.0,diff1 , 1.0,diff2);
    
    
    VertexArrayAddVertex(i, 0.0,diff1 , 0.0,diff1);
    VertexArrayAddVertex(i, 1.0,diff1 , 1.0,diff1);
    VertexArrayAddVertex(i, 0.0,diff2 , 0.0,diff2);
    
    VertexArrayAddVertex(i, 1.0,diff1 , 1.0,diff1);
    VertexArrayAddVertex(i, 0.0,diff2 , 0.0,diff2);
    VertexArrayAddVertex(i, 1.0,diff2 , 1.0,diff2);
    
    
    VertexArrayAddVertex(i, 0.0,diff2 , 0.0,diff1);
    VertexArrayAddVertex(i, 1.0,diff2 , 1.0,diff1);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,diff2);
    
    VertexArrayAddVertex(i, 1.0,diff2 , 1.0,diff1);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,diff2);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,diff2);
}

void InitShift11(){
    int i=36;
    float diff1=0.333333;
    float diff2=0.666666;
    InitVertexArray(i, false, 18);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,diff2);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,diff2);
    VertexArrayAddVertex(i, 0.0,diff1 , 0.0,1.0);
    
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,diff2);
    VertexArrayAddVertex(i, 0.0,diff1 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,diff1 , 1.0,1.0);
    
    
    VertexArrayAddVertex(i, 0.0,diff1 , 0.0,diff1);
    VertexArrayAddVertex(i, 1.0,diff1 , 1.0,diff1);
    VertexArrayAddVertex(i, 0.0,diff2 , 0.0,diff2);
    
    VertexArrayAddVertex(i, 1.0,diff1 , 1.0,diff1);
    VertexArrayAddVertex(i, 0.0,diff2 , 0.0,diff2);
    VertexArrayAddVertex(i, 1.0,diff2 , 1.0,diff2);
    
    
    VertexArrayAddVertex(i, 0.0,diff2 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,diff2 , 1.0,0.0);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,diff1);
    
    VertexArrayAddVertex(i, 1.0,diff2 , 1.0,0.0);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,diff1);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,diff1);
}

void InitShift12(){
    int i=29;
    InitVertexArray(i, false, 24);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.25);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.25);
    VertexArrayAddVertex(i, 0.0,0.25 , 0.0,0.5);
    
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.25);
    VertexArrayAddVertex(i, 0.0,0.25 , 0.0,0.5);
    VertexArrayAddVertex(i, 1.0,0.25 , 1.0,0.5);
    
    
    VertexArrayAddVertex(i, 0.0,0.25 , 0.0,0.25);
    VertexArrayAddVertex(i, 1.0,0.25 , 1.0,0.25);
    VertexArrayAddVertex(i, 0.0,0.5 , 0.0,0.5);
    
    VertexArrayAddVertex(i, 1.0,0.25 , 1.0,0.25);
    VertexArrayAddVertex(i, 0.0,0.5 , 0.0,0.5);
    VertexArrayAddVertex(i, 1.0,0.5 , 1.0,0.5);
    
    
    VertexArrayAddVertex(i, 0.0,0.5 , 0.0,0.25);
    VertexArrayAddVertex(i, 1.0,0.5 , 1.0,0.25);
    VertexArrayAddVertex(i, 0.0,0.75 , 0.0,0.5);
    
    VertexArrayAddVertex(i, 1.0,0.5 , 1.0,0.25);
    VertexArrayAddVertex(i, 0.0,0.75 , 0.0,0.5);
    VertexArrayAddVertex(i, 1.0,0.75 , 1.0,0.5);
    
    
    VertexArrayAddVertex(i, 0.0,0.75 , 0.0,0.25);
    VertexArrayAddVertex(i, 1.0,0.75 , 1.0,0.25);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,0.5);
    
    VertexArrayAddVertex(i, 1.0,0.75 , 1.0,0.25
                         );
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,0.5);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,0.5);
}

void InitShift13(){
    int i=35;
    InitVertexArray(i, false, 24);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.5);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.5);
    VertexArrayAddVertex(i, 0.0,0.25 , 0.0,0.75);
    
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.5);
    VertexArrayAddVertex(i, 0.0,0.25 , 0.0,0.75);
    VertexArrayAddVertex(i, 1.0,0.25 , 1.0,0.75);
    
    
    VertexArrayAddVertex(i, 0.0,0.25 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,0.25 , 1.0,0.0);
    VertexArrayAddVertex(i, 0.0,0.5 , 0.0,0.25);
    
    VertexArrayAddVertex(i, 1.0,0.25 , 1.0,0.0);
    VertexArrayAddVertex(i, 0.0,0.5 , 0.0,0.25);
    VertexArrayAddVertex(i, 1.0,0.5 , 1.0,0.25);
    
    
    VertexArrayAddVertex(i, 0.0,0.5 , 0.0,0.75);
    VertexArrayAddVertex(i, 1.0,0.5 , 1.0,0.75);
    VertexArrayAddVertex(i, 0.0,0.75 , 0.0,1.0);
    
    VertexArrayAddVertex(i, 1.0,0.5 , 1.0,0.75);
    VertexArrayAddVertex(i, 0.0,0.75 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,0.75 , 1.0,1.0);
    
    
    VertexArrayAddVertex(i, 0.0,0.75 , 0.0,0.25);
    VertexArrayAddVertex(i, 1.0,0.75 , 1.0,0.25);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,0.5);
    
    VertexArrayAddVertex(i, 1.0,0.75 , 1.0,0.25);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,0.5);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,0.5);
}

void InitShift14(){
    int i=7;
    InitVertexArray(i, true, 6);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 0.0,0.0);
}

void InitShift15(){
    int i=25;
    InitVertexArray(i, false, 24);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.25,0.75);
    VertexArrayAddVertex(i, 0.5,1.0 , 0.75,0.75);
    VertexArrayAddVertex(i, 0.0,0.5 , 0.25,0.25);
    
    VertexArrayAddVertex(i, 0.5,1.0 , 0.75,0.75);
    VertexArrayAddVertex(i, 0.0,0.5 , 0.25,0.25);
    VertexArrayAddVertex(i, 0.5,0.5 , 0.75,0.25);
    
    
    VertexArrayAddVertex(i, 0.5,1.0 , 0.25,0.75);
    VertexArrayAddVertex(i, 1.0,1.0 , 0.75,0.75);
    VertexArrayAddVertex(i, 0.5,0.5 , 0.25,0.25);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 0.75,0.75);
    VertexArrayAddVertex(i, 0.5,0.5 , 0.25,0.25);
    VertexArrayAddVertex(i, 1.0,0.5 , 0.75,0.25);
    
    
    VertexArrayAddVertex(i, 0.0,0.5 , 0.25,0.75);
    VertexArrayAddVertex(i, 0.5,0.5 , 0.75,0.75);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.25,0.25);
    
    VertexArrayAddVertex(i, 0.5,0.5 , 0.75,0.75);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.25,0.25);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.75,0.25);
    
    
    VertexArrayAddVertex(i, 0.5,0.5 , 0.25,0.75);
    VertexArrayAddVertex(i, 1.0,0.5 , 0.75,0.75);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.25,0.25);
    
    VertexArrayAddVertex(i, 1.0,0.5 , 0.75,0.75);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.25,0.25);
    VertexArrayAddVertex(i, 1.0,0.0 , 0.75,0.25);
}

void InitShift16(){
    int i=31;
    InitVertexArray(i, false, 24);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.5,0.5);
    VertexArrayAddVertex(i, 0.5,1.0 , 1.0,0.5);
    VertexArrayAddVertex(i, 0.0,0.5 , 0.5,0.0);
    
    VertexArrayAddVertex(i, 0.5,1.0 , 1.0,0.5);
    VertexArrayAddVertex(i, 0.0,0.5 , 0.5,0.0);
    VertexArrayAddVertex(i, 0.5,0.5 , 1.0,0.0);
    
    
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.5,0.5 , 0.5,0.5);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.5,0.5 , 0.5,0.5);
    VertexArrayAddVertex(i, 1.0,0.5 , 1.0,0.5);
    
    
    VertexArrayAddVertex(i, 0.0,0.5 , 0.0,0.5);
    VertexArrayAddVertex(i, 0.5,0.5 , 0.5,0.5);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    
    VertexArrayAddVertex(i, 0.5,0.5 , 0.5,0.5);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,0.0);
    
    
    VertexArrayAddVertex(i, 0.5,0.5 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,0.5 , 0.5,1.0);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.0,0.5);
    
    VertexArrayAddVertex(i, 1.0,0.5 , 0.5,1.0);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.0,0.5);
    VertexArrayAddVertex(i, 1.0,0.0 , 0.5,0.5);
}

void InitShift17(){
    int i=34;
    InitVertexArray(i, false, 12);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);

    
    VertexArrayAddVertex(i, 0.2,0.8 , 0.3,0.7);
    VertexArrayAddVertex(i, 0.8,0.8 , 0.7,0.7);
    VertexArrayAddVertex(i, 0.2,0.2 , 0.3,0.3);
    
    VertexArrayAddVertex(i, 0.8,0.8 , 0.7,0.7);
    VertexArrayAddVertex(i, 0.2,0.2 , 0.3,0.3);
    VertexArrayAddVertex(i, 0.8,0.2 , 0.7,0.3);
}

void InitShift18(){
    int i=40;
    InitVertexArray(i, false, 12);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.2,0.8);
    VertexArrayAddVertex(i, 1.0,1.0 , 0.8,0.8);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.2,0.2);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 0.8,0.8);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.2,0.2);
    VertexArrayAddVertex(i, 1.0,0.0 , 0.8,0.2);
    
    
    VertexArrayAddVertex(i, 0.2,0.8 , 0.2,0.8);
    VertexArrayAddVertex(i, 0.8,0.8 , 0.8,0.8);
    VertexArrayAddVertex(i, 0.2,0.2 , 0.2,0.2);
    
    VertexArrayAddVertex(i, 0.8,0.8 , 0.8,0.8);
    VertexArrayAddVertex(i, 0.2,0.2 , 0.2,0.2);
    VertexArrayAddVertex(i, 0.8,0.2 , 0.8,0.2);
}

void InitShift19(){
    int i=14;
    InitVertexArray(i, false, 24);
    VertexArrayAddVertex(i, 0.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.25,0.5 , 0.75,0.5);
    VertexArrayAddVertex(i, 0.0,0.0 , 1.0,0.0);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.75,0.5 , 0.25,0.5);
    VertexArrayAddVertex(i, 1.0,0.0 , 0.0,0.0);
    
    
    
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.25,0.5 , 0.25,0.5);
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,1.0);
    
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 0.25,0.5 , 0.25,0.5);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,0.0);
    
    
    
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,0.0);
    VertexArrayAddVertex(i, 0.25,0.5 , 0.25,0.5);
    VertexArrayAddVertex(i, 0.75,0.5 , 0.75,0.5);
    
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 0.25,0.5 , 0.25,0.5);
    VertexArrayAddVertex(i, 0.75,0.5 , 0.75,0.5);
    
    
    
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 0.75,0.5 , 0.75,0.5);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,0.0);
    VertexArrayAddVertex(i, 0.75,0.5 , 0.75,0.5);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
}

void InitShift20(){
    int i=2;
    InitVertexArray(i, false, 24);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.25,0.5 , 0.25,0.5);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.75,0.5 , 0.75,0.5);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
    
    
    
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 0.25,0.5 , 0.25,0.5);
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,0.0);
    
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.25,0.5 , 0.25,0.5);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,1.0);
    
    
    
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 0.25,0.5 , 0.25,0.5);
    VertexArrayAddVertex(i, 0.75,0.5 , 0.75,0.5);
    
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,0.0);
    VertexArrayAddVertex(i, 0.25,0.5 , 0.25,0.5);
    VertexArrayAddVertex(i, 0.75,0.5 , 0.75,0.5);
    
    
    
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,0.0);
    VertexArrayAddVertex(i, 0.75,0.5 , 0.75,0.5);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,0.0);
    
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 0.75,0.5 , 0.75,0.5);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,1.0);
}

void InitShift21(){
    int i=3;
    InitVertexArray(i, true, 8);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    
    VertexArrayAddVertex(i, 0.25,1.0 , 0.35,1.0);
    VertexArrayAddVertex(i, 0.25,0.0 , 0.35,0.0);
    
    VertexArrayAddVertex(i, 0.75,1.0 , 0.65,1.0);
    VertexArrayAddVertex(i, 0.75,0.0 , 0.65,0.0);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
}

void InitShift22(){
    int i=9;
    InitVertexArray(i, true, 8);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    
    VertexArrayAddVertex(i, 0.35,1.0 , 0.25,1.0);
    VertexArrayAddVertex(i, 0.35,0.0 , 0.25,0.0);
    
    VertexArrayAddVertex(i, 0.65,1.0 , 0.75,1.0);
    VertexArrayAddVertex(i, 0.65,0.0 , 0.75,0.0);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
}

void InitShift23(){
    int i=15;
    InitVertexArray(i, true, 8);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    
    VertexArrayAddVertex(i, 0.0,0.75 , 0.0,0.65);
    VertexArrayAddVertex(i, 1.0,0.75 , 1.0,0.65);
    
    VertexArrayAddVertex(i, 0.0,0.25 , 0.0,0.35);
    VertexArrayAddVertex(i, 1.0,0.25 , 1.0,0.35);
    
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
}

void InitShift24(){
    int i=21;
    InitVertexArray(i, true, 8);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    
    VertexArrayAddVertex(i, 0.0,0.65 , 0.0,0.75);
    VertexArrayAddVertex(i, 1.0,0.65 , 1.0,0.75);
    
    VertexArrayAddVertex(i, 0.0,0.35 , 0.0,0.25);
    VertexArrayAddVertex(i, 1.0,0.35 , 1.0,0.25);
    
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
}

void InitShift25(){
    int i=27;
    InitVertexArray(i, true, 5);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.35,1.0);
    
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,0.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 0.65,1.0);
    
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
}

void InitShift26(){
    int i=33;
    InitVertexArray(i, true, 5);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.35,0.0);
    
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 0.65,0.0);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
}


void InitShift27(){
    int i=39;
    InitVertexArray(i, false, 12);
    float cpx=0.25;
    float cpy=0.25;
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, cpx,cpy , 0.5,0.5);

    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, cpx,cpy , 0.5,0.5);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
    VertexArrayAddVertex(i, cpx,cpy , 0.5,0.5);
    
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, cpx,cpy , 0.5,0.5);
}

void InitShift28(){
    int i=41;
    InitVertexArray(i, false, 12);
    float cpx=0.75;
    float cpy=0.75;
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, cpx,cpy , 0.5,0.5);
    
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, cpx,cpy , 0.5,0.5);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
    VertexArrayAddVertex(i, cpx,cpy , 0.5,0.5);
    
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, cpx,cpy , 0.5,0.5);
}

void InitShift29(){
    int i=4;
    InitVertexArray(i, false, 1086);
    
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
    
    
    wPoint centerPoint =wPointNew(0.5,0.5);
    
    float inc=1.0;
    inc*=0.0174532925;
    double angle      = 0.0; //between 0 and 2 * PI, angle is in radians
    float distance      = 0.3;
    
    
    wPoint allp[361];
    
    for (int u=0; u<361; u++) {
        angle+=inc;
        allp[u].y = centerPoint.y + (distance * sin( angle ));
        allp[u].x = centerPoint.x + (distance * cos( angle ));
    }
    
    
    for (int u=0; u<360; u++) {
        int x=u+180;
        if (x>=360) {x-=360;}
        VertexArrayAddVertex(i, centerPoint.x,centerPoint.y , centerPoint.x,centerPoint.y);
        VertexArrayAddVertex(i, allp[u].x,allp[u].y , allp[x].x,allp[x].y);
        VertexArrayAddVertex(i, allp[u+1].x,allp[u+1].y , allp[x+1].x,allp[x+1].y);
    }
}

void InitShift30(){
    int i=16;
    InitVertexArray(i, false, 66);
    
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
    
    
    wPoint centerPoint =wPointNew(0.5,0.5);
    
    float inc=360/20;
    inc*=0.0174532925;
    double angle      = 0.0; //between 0 and 2 * PI, angle is in radians
    float distance      = 0.3;
    
    
    wPoint allp[31];
    
    for (int u=0; u<31; u++) {
        angle+=inc;
        allp[u].y = centerPoint.y + (distance * sin( angle ));
        allp[u].x = centerPoint.x + (distance * cos( angle ));
    }
    
    
    for (int u=10; u<31-1; u++) {
        VertexArrayAddVertex(i, centerPoint.x,centerPoint.y , centerPoint.x,centerPoint.y);
        VertexArrayAddVertex(i, allp[u].x,allp[u].y , allp[u].x,allp[u].y);
        VertexArrayAddVertex(i, allp[u+1].x,allp[u+1].y , allp[u].x,allp[u+1].y);
    }
}

void InitShift31(){
    int i=10;
    InitVertexArray(i, false, 1086);
    
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
    
    
    wPoint centerPoint =wPointNew(0.5,0.5);
    
    float inc=1.0;
    inc*=0.0174532925;
    double angle      = 0.0; //between 0 and 2 * PI, angle is in radians
    float distance      = 0.3;
    float distance2      = 0.5;
    
    wPoint allp[361];
    wPoint allp2[361];
    for (int u=0; u<361; u++) {
        angle+=inc;
        allp[u].y = centerPoint.y + (distance * sin( angle ));
        allp[u].x = centerPoint.x + (distance * cos( angle ));
        
        allp2[u].y = centerPoint.y + (distance2 * sin( angle ));
        allp2[u].x = centerPoint.x + (distance2 * cos( angle ));
    }
    
    for (int u=0; u<360; u++) {
        VertexArrayAddVertex(i, centerPoint.x,centerPoint.y , centerPoint.x,centerPoint.y);
        VertexArrayAddVertex(i, allp[u].x,allp[u].y , allp2[u].x,allp2[u].y);
        VertexArrayAddVertex(i, allp[u+1].x,allp[u+1].y , allp2[u+1].x,allp2[u+1].y);
    }
}

void InitShift32(){
    int i=22;
    InitVertexArray(i, false, 1086);
    
    VertexArrayAddVertex(i, 0.0,0.0 , 0.1,0.1);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.1,0.9);
    VertexArrayAddVertex(i, 1.0,1.0 , 0.9,0.9);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 0.9,0.9);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.1,0.1);
    VertexArrayAddVertex(i, 1.0,0.0 , 0.9,0.1);
    
    
    wPoint centerPoint =wPointNew(0.5,0.5);
    
    float inc=1.0;
    inc*=0.0174532925;
    double angle      = 0.0; //between 0 and 2 * PI, angle is in radians
    float distance      = 0.3;
    
    wPoint allp[361];
    for (int u=0; u<31; u++) {
        angle+=inc;
        allp[u].y = centerPoint.y + (distance * sin( angle ));
        allp[u].x = centerPoint.x + (distance * cos( angle ));
    }
    
    
    for (int u=0; u<360; u++) {
        VertexArrayAddVertex(i, centerPoint.x,centerPoint.y , centerPoint.x,centerPoint.y);
        VertexArrayAddVertex(i, allp[u].x,allp[u].y , allp[u].x,allp[u].y);
        VertexArrayAddVertex(i, allp[u+1].x,allp[u+1].y , allp[u+1].x,allp[u+1].y);
    }
}

void InitShift33(){
    int i=20;
    InitVertexArray(i, false, 30);
    
    int tpr=2;
    int rowCount=2;
    
    float xShift=1.0/(float)tpr;
    float yShift=1.0/(float)rowCount;
    
    float tXShift=(float)(tpr/2)*xShift;
    float tYShift=(float)(rowCount/2)*yShift;
    
    wPoint p1=wPointNew(0.0, 0.0);
    wPoint p2=wPointNew(xShift/2.0,yShift);
    wPoint p3=wPointNew(xShift,0.0);
    
    wPoint tx1=wPointNew(p1.x+(tXShift), p1.y+(tYShift));
    wPoint tx2=wPointNew(p2.x+(tXShift), p2.y+(tYShift));
    wPoint tx3=wPointNew(p3.x+(tXShift), p3.y+(tYShift));
    
    float x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , p1.x+(tXShift),p1.y+(tYShift));
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , p2.x+(tXShift),p2.y+(tYShift));
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , p3.x+(tXShift),p3.y+(tYShift));
            x+=xShift;
        }
        y+=yShift;
    }
    
    p1=wPointNew(xShift, 0.0);
    p2=wPointNew((xShift/2.0)+xShift,yShift);
    p3=wPointNew(xShift/2.0,yShift);
    
    tpr--;
    x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
            x+=xShift;
        }
        y+=yShift;
    }
    
    p1=wPointNew(0.0, 0.0);
    p2=wPointNew(0.0,yShift);
    p3=wPointNew(xShift/2.0,yShift);
    
    tx3=wPointNew(tx3.x-(xShift/2.0), tx3.y);
    
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
        y+=yShift;
    }
    
    
    tx1=wPointNew(tx1.x+(xShift/2.0), tx1.y);
    tx3=wPointNew(tx3.x+(xShift/2.0), tx3.y);
    
    
    p1=wPointNew(1.0, 0.0);
    p2=wPointNew(1.0-(xShift/2.0),yShift);
    p3=wPointNew(1.0,yShift);
    
    x=0.0,y=0.0;;
    for (int m=0; m<rowCount; m++) {
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
        y+=yShift;
    }
}

void InitShift34(){
    int i=11;
    InitVertexArray(i, false, 108);
    
    int tpr=4;
    int rowCount=4;
    
    float xShift=1.0/(float)tpr;
    float yShift=1.0/(float)rowCount;
    
    float tXShift=(float)(tpr/2)*xShift;
    float tYShift=(float)(rowCount/2)*yShift;
    
    wPoint p1=wPointNew(0.0, 0.0);
    wPoint p2=wPointNew(xShift/2.0,yShift);
    wPoint p3=wPointNew(xShift,0.0);
    
    wPoint tx1=wPointNew(p1.x+(tXShift), p1.y+(tYShift));
    wPoint tx2=wPointNew(p2.x+(tXShift), p2.y+(tYShift));
    wPoint tx3=wPointNew(p3.x+(tXShift), p3.y+(tYShift));
    
    float x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , p1.x+(tXShift),p1.y+(tYShift));
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , p2.x+(tXShift),p2.y+(tYShift));
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , p3.x+(tXShift),p3.y+(tYShift));
            x+=xShift;
        }
        y+=yShift;
    }
    
    p1=wPointNew(xShift, 0.0);
    p2=wPointNew((xShift/2.0)+xShift,yShift);
    p3=wPointNew(xShift/2.0,yShift);
    
    tpr--;
    x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
            x+=xShift;
        }
        y+=yShift;
    }
    
    p1=wPointNew(0.0, 0.0);
    p2=wPointNew(0.0,yShift);
    p3=wPointNew(xShift/2.0,yShift);
    
    tx3=wPointNew(tx3.x-(xShift/2.0), tx3.y);
    
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
        y+=yShift;
    }
    
    
    tx1=wPointNew(tx1.x+(xShift/2.0), tx1.y);
    tx3=wPointNew(tx3.x+(xShift/2.0), tx3.y);
    
    
    p1=wPointNew(1.0, 0.0);
    p2=wPointNew(1.0-(xShift/2.0),yShift);
    p3=wPointNew(1.0,yShift);
    
    x=0.0,y=0.0;;
    for (int m=0; m<rowCount; m++) {
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
        y+=yShift;
    }
}

void InitShift35(){
    int i=23;
    InitVertexArray(i, false, 312);
    
    int tpr=6;
    int rowCount=8;
    
    float xShift=1.0/(float)tpr;
    float yShift=1.0/(float)rowCount;
    
    float tXShift=(float)(tpr/2)*xShift;
    float tYShift=(float)(rowCount/2)*yShift;
    
    wPoint p1=wPointNew(0.0, 0.0);
    wPoint p2=wPointNew(xShift/2.0,yShift);
    wPoint p3=wPointNew(xShift,0.0);
    
    wPoint tx1=wPointNew(p1.x+(tXShift), p1.y+(tYShift));
    wPoint tx2=wPointNew(p2.x+(tXShift), p2.y+(tYShift));
    wPoint tx3=wPointNew(p3.x+(tXShift), p3.y+(tYShift));
    
    float x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , p1.x+(tXShift),p1.y+(tYShift));
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , p2.x+(tXShift),p2.y+(tYShift));
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , p3.x+(tXShift),p3.y+(tYShift));
            x+=xShift;
        }
        y+=yShift;
    }
    
    p1=wPointNew(xShift, 0.0);
    p2=wPointNew((xShift/2.0)+xShift,yShift);
    p3=wPointNew(xShift/2.0,yShift);
    
    tpr--;
    x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
            x+=xShift;
        }
        y+=yShift;
    }
    
    p1=wPointNew(0.0, 0.0);
    p2=wPointNew(0.0,yShift);
    p3=wPointNew(xShift/2.0,yShift);
    
    tx3=wPointNew(tx3.x-(xShift/2.0), tx3.y);
    
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
        y+=yShift;
    }
    
    
    tx1=wPointNew(tx1.x+(xShift/2.0), tx1.y);
    tx3=wPointNew(tx3.x+(xShift/2.0), tx3.y);
    
    
    p1=wPointNew(1.0, 0.0);
    p2=wPointNew(1.0-(xShift/2.0),yShift);
    p3=wPointNew(1.0,yShift);
    
    x=0.0,y=0.0;;
    for (int m=0; m<rowCount; m++) {
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
        y+=yShift;
    }
}

void InitShift36(){
    int i=8;
    InitVertexArray(i, false, 30);
    
    int tpr=2;
    int rowCount=2;
    
    float xShift=1.0/(float)tpr;
    float yShift=1.0/(float)rowCount;
    
    float tXShift=(float)(tpr/2)*xShift;
    float tYShift=(float)(rowCount/2)*yShift;
    
    wPoint p1=wPointNew(0.0, 0.0);
    wPoint p2=wPointNew(xShift/2.0,yShift);
    wPoint p3=wPointNew(xShift,0.0);
    
    wPoint tx1=wPointNew(p1.x+(tXShift), p1.y+(tYShift));
    wPoint tx2=wPointNew(p2.x+(tXShift), p2.y+(tYShift));
    wPoint tx3=wPointNew(p3.x+(tXShift), p3.y+(tYShift));
    
    float x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , p1.x+(tXShift),p1.y+(tYShift));
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , p2.x+(tXShift),p2.y+(tYShift));
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , p3.x+(tXShift),p3.y+(tYShift));
            x+=xShift;
        }
        y+=(yShift*2.0);
    }
    
    p1=wPointNew(xShift/2.0, yShift);
    p2=wPointNew(xShift,yShift*2);
    p3=wPointNew(0.0,yShift*2);
    
   // tpr--;
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
            x+=xShift;
        }
        y+=(yShift*2.0);
    }
    
    
    
    tpr--;
    p1=wPointNew(xShift/2.0, yShift);
    p2=wPointNew(xShift,yShift*2.0);
    p3=wPointNew(xShift+(xShift/2.0),yShift);
    
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx1.x,tx1.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx3.x,tx3.y);
            x+=xShift;
        }
        y+=(yShift*2.0);
    }
    
    
    p1=wPointNew(xShift, 0.0);
    p2=wPointNew(xShift+(xShift/2.0),yShift);
    p3=wPointNew(xShift/2.0,yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
            x+=xShift;
        }
        y+=(yShift*2.0);
    }
    
    
    
    
    
    p1=wPointNew(0.0, 0.0);
    p2=wPointNew(xShift/2.0,yShift);
    p3=wPointNew(0.0,yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x+(xShift/2.0),tx1.y);
            x+=xShift;
        y+=(yShift*2.0);
    }
    
    
    p1=wPointNew(1.0, 0.0);
    p2=wPointNew(1.0,yShift);
    p3=wPointNew(1.0-(xShift/2.0),yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x-(xShift/2.0),tx3.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
            x+=xShift;
        y+=(yShift*2.0);
    }
    
    
    
    
    
    p1=wPointNew(0.0, yShift);
    p2=wPointNew(0.0,yShift*2.0);
    p3=wPointNew(xShift/2.0,yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx1.x+(xShift/2.0),tx1.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx3.x,tx3.y);
            x+=xShift;
        y+=(yShift*2.0);
    }

    
    
    p1=wPointNew(1.0-(xShift/2.0), yShift);
    p2=wPointNew(1.0,yShift*2.0);
    p3=wPointNew(1.0,yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx1.x,tx1.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx3.x-(xShift/2.0),tx3.y);
        x+=xShift;
        y+=(yShift*2.0);
    }
    
}

void InitShift37(){
    int i=5;
    InitVertexArray(i, false, 108);
    
    int tpr=4;
    int rowCount=4;
    
    float xShift=1.0/(float)tpr;
    float yShift=1.0/(float)rowCount;
    
    float tXShift=(float)(tpr/2)*xShift;
    float tYShift=(float)(rowCount/2)*yShift;
    
    wPoint p1=wPointNew(0.0, 0.0);
    wPoint p2=wPointNew(xShift/2.0,yShift);
    wPoint p3=wPointNew(xShift,0.0);
    
    wPoint tx1=wPointNew(p1.x+(tXShift), p1.y+(tYShift));
    wPoint tx2=wPointNew(p2.x+(tXShift), p2.y+(tYShift));
    wPoint tx3=wPointNew(p3.x+(tXShift), p3.y+(tYShift));
    
    float x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , p1.x+(tXShift),p1.y+(tYShift));
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , p2.x+(tXShift),p2.y+(tYShift));
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , p3.x+(tXShift),p3.y+(tYShift));
            x+=xShift;
        }
        y+=(yShift*2.0);
    }
    
    p1=wPointNew(xShift/2.0, yShift);
    p2=wPointNew(xShift,yShift*2);
    p3=wPointNew(0.0,yShift*2);
    
    // tpr--;
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
            x+=xShift;
        }
        y+=(yShift*2.0);
    }
    
    
    
    tpr--;
    p1=wPointNew(xShift/2.0, yShift);
    p2=wPointNew(xShift,yShift*2.0);
    p3=wPointNew(xShift+(xShift/2.0),yShift);
    
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx1.x,tx1.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx3.x,tx3.y);
            x+=xShift;
        }
        y+=(yShift*2.0);
    }
    
    
    p1=wPointNew(xShift, 0.0);
    p2=wPointNew(xShift+(xShift/2.0),yShift);
    p3=wPointNew(xShift/2.0,yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
            x+=xShift;
        }
        y+=(yShift*2.0);
    }
    
    
    
    
    
    p1=wPointNew(0.0, 0.0);
    p2=wPointNew(xShift/2.0,yShift);
    p3=wPointNew(0.0,yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x+(xShift/2.0),tx1.y);
        x+=xShift;
        y+=(yShift*2.0);
    }
    
    
    p1=wPointNew(1.0, 0.0);
    p2=wPointNew(1.0,yShift);
    p3=wPointNew(1.0-(xShift/2.0),yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x-(xShift/2.0),tx3.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
        x+=xShift;
        y+=(yShift*2.0);
    }
    
    
    
    
    
    p1=wPointNew(0.0, yShift);
    p2=wPointNew(0.0,yShift*2.0);
    p3=wPointNew(xShift/2.0,yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx1.x+(xShift/2.0),tx1.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx3.x,tx3.y);
        x+=xShift;
        y+=(yShift*2.0);
    }
    
    
    
    p1=wPointNew(1.0-(xShift/2.0), yShift);
    p2=wPointNew(1.0,yShift*2.0);
    p3=wPointNew(1.0,yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx1.x,tx1.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx3.x-(xShift/2.0),tx3.y);
        x+=xShift;
        y+=(yShift*2.0);
    }
    
}

void InitShift38(){
    int i=17;
    InitVertexArray(i, false, 312);
    
    int tpr=6;
    int rowCount=8;
    
    float xShift=1.0/(float)tpr;
    float yShift=1.0/(float)rowCount;
    
    float tXShift=(float)(tpr/2)*xShift;
    float tYShift=(float)(rowCount/2)*yShift;
    
    wPoint p1=wPointNew(0.0, 0.0);
    wPoint p2=wPointNew(xShift/2.0,yShift);
    wPoint p3=wPointNew(xShift,0.0);
    
    wPoint tx1=wPointNew(p1.x+(tXShift), p1.y+(tYShift));
    wPoint tx2=wPointNew(p2.x+(tXShift), p2.y+(tYShift));
    wPoint tx3=wPointNew(p3.x+(tXShift), p3.y+(tYShift));
    
    float x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , p1.x+(tXShift),p1.y+(tYShift));
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , p2.x+(tXShift),p2.y+(tYShift));
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , p3.x+(tXShift),p3.y+(tYShift));
            x+=xShift;
        }
        y+=(yShift*2.0);
    }
    
    p1=wPointNew(xShift/2.0, yShift);
    p2=wPointNew(xShift,yShift*2);
    p3=wPointNew(0.0,yShift*2);
    
    // tpr--;
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
            x+=xShift;
        }
        y+=(yShift*2.0);
    }
    
    
    
    tpr--;
    p1=wPointNew(xShift/2.0, yShift);
    p2=wPointNew(xShift,yShift*2.0);
    p3=wPointNew(xShift+(xShift/2.0),yShift);
    
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx1.x,tx1.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx3.x,tx3.y);
            x+=xShift;
        }
        y+=(yShift*2.0);
    }
    
    
    p1=wPointNew(xShift, 0.0);
    p2=wPointNew(xShift+(xShift/2.0),yShift);
    p3=wPointNew(xShift/2.0,yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
            x+=xShift;
        }
        y+=(yShift*2.0);
    }
    
    
    
    
    
    p1=wPointNew(0.0, 0.0);
    p2=wPointNew(xShift/2.0,yShift);
    p3=wPointNew(0.0,yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x,tx3.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x+(xShift/2.0),tx1.y);
        x+=xShift;
        y+=(yShift*2.0);
    }
    
    
    p1=wPointNew(1.0, 0.0);
    p2=wPointNew(1.0,yShift);
    p3=wPointNew(1.0-(xShift/2.0),yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx3.x-(xShift/2.0),tx3.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx1.x,tx1.y);
        x+=xShift;
        y+=(yShift*2.0);
    }
    
    
    
    
    
    p1=wPointNew(0.0, yShift);
    p2=wPointNew(0.0,yShift*2.0);
    p3=wPointNew(xShift/2.0,yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx1.x+(xShift/2.0),tx1.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx3.x,tx3.y);
        x+=xShift;
        y+=(yShift*2.0);
    }
    
    
    
    p1=wPointNew(1.0-(xShift/2.0), yShift);
    p2=wPointNew(1.0,yShift*2.0);
    p3=wPointNew(1.0,yShift);
    
    x=0.0,y=0.0;
    for (int m=0; m<rowCount/2; m++) {
        x=0.0;
        VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx1.x,tx1.y);
        VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx2.x,tx2.y);
        VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx3.x-(xShift/2.0),tx3.y);
        x+=xShift;
        y+=(yShift*2.0);
    }
}


void InitShift39(){
    int i=38;
    InitVertexArray(i, false, 54);
    
    int tpr=3;
    int rowCount=3;
    
    float xShift=1.0/(float)tpr;
    float yShift=1.0/(float)rowCount;
    
    float tXShift=(float)(tpr/2)*xShift;
    float tYShift=(float)(rowCount/2)*yShift;
    
    wPoint p1=wPointNew(0.0, 0.0);
    wPoint p2=wPointNew(0.0,yShift);
    wPoint p3=wPointNew(xShift,0.0);
    
    wPoint tx1=wPointNew(p1.x+(tXShift), p1.y+(tYShift));
    wPoint tx2=wPointNew(p2.x+(tXShift), p2.y+(tYShift));
    wPoint tx3=wPointNew(p3.x+(tXShift), p3.y+(tYShift));
    
    float x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , p1.x+(tXShift),p1.y+(tYShift));
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , p2.x+(tXShift),p2.y+(tYShift));
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , p3.x+(tXShift),p3.y+(tYShift));
            x+=xShift;
        }
        y+=yShift;
    }
    
    p1=wPointNew(xShift, 0.0);
    p2=wPointNew(0.0,yShift);
    p3=wPointNew(xShift,yShift);
    
    tx1=wPointNew(p1.x+(tXShift), p1.y+(tYShift));
    tx2=wPointNew(p2.x+(tXShift), p2.y+(tYShift));
    tx3=wPointNew(p3.x+(tXShift), p3.y+(tYShift));
    
    // tpr--;
    x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        x=0.0;
        for (int k=0; k<tpr; k++) {
            VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx1.x,tx1.y);
            VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx2.x,tx2.y);
            VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx3.x,tx3.y);
            x+=xShift;
        }
        y+=yShift;
    }
}

void InitShift3(){
    int i=32;
    InitVertexArray(i, false, 96);
    
    int tpr=4;
    int rowCount=4;
    
    float xShift=1.0/(float)tpr;
    float yShift=1.0/(float)rowCount;
    
    float tXShift=(float)(tpr/2)*xShift;
    float tYShift=(float)(rowCount/2)*yShift;
    
    wPoint p1=wPointNew(0.0, 0.0);
    wPoint p2=wPointNew(0.0,yShift);
    wPoint p3=wPointNew(xShift,0.0);
    
    wPoint tx1=wPointNew(p1.x+(tXShift), p1.y+(tYShift));
    wPoint tx2=wPointNew(p2.x+(tXShift), p2.y+(tYShift));
    wPoint tx3=wPointNew(p3.x+(tXShift), p3.y+(tYShift));
    
    wPoint p4=wPointNew(xShift, 0.0);
    wPoint p5=wPointNew(0.0,yShift);
    wPoint p6=wPointNew(xShift,yShift);
    
    wPoint tx4=wPointNew(p4.x+(tXShift), p4.y+(tYShift));
    wPoint tx5=wPointNew(p5.x+(tXShift), p5.y+(tYShift));
    wPoint tx6=wPointNew(p6.x+(tXShift), p6.y+(tYShift));
    
    bool oddX=false;
    bool oddY=false;
    float x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        x=0.0;
        oddX=false;
        for (int k=0; k<tpr; k++) {
            if (oddX!=oddY) {
                VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx1.x,tx1.y);
                VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx2.x,tx2.y);
                VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx3.x,tx3.y);
            }
            else{
                VertexArrayAddVertex(i, p1.x+(x),p1.y+(y) , tx6.x,tx6.y);
                VertexArrayAddVertex(i, p2.x+(x),p2.y+(y) , tx4.x,tx4.y);
                VertexArrayAddVertex(i, p3.x+(x),p3.y+(y) , tx5.x,tx5.y);
            }
            oddX=!oddX;
            x+=xShift;
        }
        oddY=!oddY;
        y+=yShift;
    }
    

    // tpr--;
    oddX=false;
    oddY=false;
    x=0.0,y=0.0;
    for (int m=0; m<rowCount; m++) {
        x=0.0;
        oddX=false;
        for (int k=0; k<tpr; k++) {
            if (oddX!=oddY) {
                VertexArrayAddVertex(i, p4.x+(x),p4.y+(y) , tx4.x,tx4.y);
                VertexArrayAddVertex(i, p5.x+(x),p5.y+(y) , tx5.x,tx5.y);
                VertexArrayAddVertex(i, p6.x+(x),p6.y+(y) , tx6.x,tx6.y);
            }
            else{
                VertexArrayAddVertex(i, p4.x+(x),p4.y+(y) , tx2.x,tx2.y);
                VertexArrayAddVertex(i, p5.x+(x),p5.y+(y) , tx3.x,tx3.y);
                VertexArrayAddVertex(i, p6.x+(x),p6.y+(y) , tx1.x,tx1.y);
            }
            oddX=!oddX;
            x+=xShift;
        }
        oddY=!oddY;
        y+=yShift;
    }
}



void InitShift40(){
    int i=28;
    InitVertexArray(i, false, 1086);
    
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 0.0,1.0 , 0.0,1.0);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,1.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 0.0,0.0);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.0);
    
    
    wPoint centerPoint =wPointNew(0.5,0.5);
    
    float inc=1.0;
    inc*=0.0174532925;
    double angle      = 0.0; //between 0 and 2 * PI, angle is in radians
    float distance      = 0.3;
    
    
    wPoint allp[361];
    
    for (int u=0; u<361; u++) {
        angle+=inc;
        allp[u].y = centerPoint.y + (distance * sin( angle ));
        allp[u].x = centerPoint.x + (distance * cos( angle ));
    }
    
    
    for (int u=0; u<360; u+=2) {
        int x=u+180;
        if (x>=360) {x-=360;}
        VertexArrayAddVertex(i, centerPoint.x,centerPoint.y , centerPoint.x,centerPoint.y);
        VertexArrayAddVertex(i, allp[u].x,allp[u].y , allp[x].x,allp[x].y);
        VertexArrayAddVertex(i, allp[u+1].x,allp[u+1].y , allp[x+1].x,allp[u].y);
    }
}


void InitShift41(){
    int i=37;
    InitVertexArray(i, false, 24);
    VertexArrayAddVertex(i, 0.0,1.0 , 1.0,0.5);
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,0.5);
    VertexArrayAddVertex(i, 0.0,0.5 , 1.0,0.0);
    
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,0.5);
    VertexArrayAddVertex(i, 0.0,0.5 , 1.0,0.0);
    VertexArrayAddVertex(i, 0.5,0.5 , 0.5,0.0);
    
    
    VertexArrayAddVertex(i, 0.5,1.0 , 0.5,0.5);
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,0.5);
    VertexArrayAddVertex(i, 0.5,0.5 , 0.5,0.0);
    
    VertexArrayAddVertex(i, 1.0,1.0 , 1.0,0.5);
    VertexArrayAddVertex(i, 0.5,0.5 , 0.5,0.0);
    VertexArrayAddVertex(i, 1.0,0.5 , 1.0,0.0);
    
    
    VertexArrayAddVertex(i, 0.0,0.5 , 1.0,0.0);
    VertexArrayAddVertex(i, 0.5,0.5 , 0.5,0.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 1.0,0.5);
    
    VertexArrayAddVertex(i, 0.5,0.5 , 0.5,0.0);
    VertexArrayAddVertex(i, 0.0,0.0 , 1.0,0.5);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,0.5);
    
    
    VertexArrayAddVertex(i, 0.5,0.5 , 0.5,0.0);
    VertexArrayAddVertex(i, 1.0,0.5 , 1.0,0.0);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,0.5);
    
    VertexArrayAddVertex(i, 1.0,0.5 , 1.0,0.0);
    VertexArrayAddVertex(i, 0.5,0.0 , 0.5,0.5);
    VertexArrayAddVertex(i, 1.0,0.0 , 1.0,0.5);
}


void PKShiftInitFilters(){
    InitShift0();
    InitShift1();
    InitShift2();
    InitShift3();
    InitShift4();
    InitShift5();
    InitShift6();
    InitShift7();
    InitShift8();
    InitShift9();
    InitShift10();
    InitShift11();
    InitShift12();
    InitShift13();
    InitShift14();
    InitShift15();
    InitShift16();
    InitShift17();
    InitShift18();
    InitShift19();
    InitShift20();
    InitShift21();
    InitShift22();
    InitShift23();
    InitShift24();
    InitShift25();
    InitShift26();
    InitShift27();
    InitShift28();
    InitShift29();
    InitShift30();
    InitShift31();
    InitShift32();
    InitShift33();
    InitShift34();
    InitShift35();
    InitShift36();
    InitShift37();
    InitShift38();
    InitShift39();
    InitShift40();
    InitShift41();
}

ShiftVertexArray*PKShiftFilterGetVerticiesForShiftID(int filterID){
    return &Shifts[filterID];
}