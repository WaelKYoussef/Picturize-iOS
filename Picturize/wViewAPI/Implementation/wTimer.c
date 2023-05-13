//
//  wTimer.c
//  PhotoKit
//
//  Created by Wael Youssef on 5/15/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "wViewAPI.h"

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

void*wTimerLoop(void*data){
    wTimer*timer=data;
    wTimeStamp nowTimeStamp, laterTimeStamp;
    
    nowTimeStamp=wTimeStampGet();
    laterTimeStamp=nowTimeStamp;
    while (wTimerStampDifference(nowTimeStamp, laterTimeStamp)<timer[0].duration) {
        if (timer[0].stop) {
            timer[0].conditionMet=false;
            timer[0].isRunning=false;
            pthread_exit(NULL);
        }
        laterTimeStamp=wTimeStampGet();
    }
    
    timer[0].conditionMet=true;
    timer[0].isRunning=false;
    pthread_exit(NULL);
    return NULL;
}

wTimer*wTimerNew(float duration, wFunction function, bool repeat, void*userData){
    wTimer*timer=malloc(sizeof(wTimer));
    timer[0].duration=duration;
    timer[0].function=function;
    timer[0].userData=userData;
    timer[0].stop=false;
    timer[0].repeat=repeat;
    timer[0].conditionMet=false;
    timer[0].shouldRelease=false;
    wArrayAddItem(wTimerMasterArray, timer);
    return timer;
}

void wTimerStart(wTimer*timer){
    if (timer[0].isRunning) {wTimerStop(timer);}
    pthread_create(&timer[0].thread, NULL, wTimerLoop, timer);
    timer[0].isRunning=true;
    timer[0].stop=false;
}

void wTimerStop(wTimer*timer){
    //if (timer[0].isRunning) {
        timer[0].stop=true;
    //}
}

void wTimerExecuteFunction(wTimer*timer){
    if (timer[0].function) {timer[0].function(timer[0].userData);}
    if (timer[0].repeat){
     //   wTimerStart(timer);
        if (timer[0].isRunning) {wTimerStop(timer);}
        pthread_create(&timer[0].thread, NULL, wTimerLoop, timer);
        timer[0].isRunning=true;
    }
}

void wTimerRelease(wTimer*timer){
    timer[0].shouldRelease=true;
}

bool wTimerGetIsRunning(wTimer*timer){
    return timer[0].isRunning;
}

wTimeStamp wTimeStampGet(){
    wTimeStamp timeStamp;gettimeofday(&timeStamp, NULL);
    return timeStamp;
}

float wTimerStampDifference(wTimeStamp timeStamp1, wTimeStamp timeStamp2){
    float diff=((timeStamp2.tv_sec-timeStamp1.tv_sec)*1000000.0)+(timeStamp2.tv_usec-timeStamp1.tv_usec);
    diff/=1000000.0;
    return diff;
}



wDate wDateGet(){
    time_t RawTime;time(&RawTime);
    struct tm ProcessedTime;ProcessedTime=*localtime(&RawTime);
    wDate date={ProcessedTime.tm_year+1900,ProcessedTime.tm_mon+1,
        ProcessedTime.tm_mday,ProcessedTime.tm_hour,ProcessedTime.tm_min,
        ProcessedTime.tm_sec};
    return date;
}

wShortDate wShortDateGet(){
    time_t RawTime;time(&RawTime);
    struct tm ProcessedTime;ProcessedTime=*localtime(&RawTime);
    wShortDate shortDate={ProcessedTime.tm_year+1900,ProcessedTime.tm_mon+1,
        ProcessedTime.tm_mday};
    return shortDate;
}

wTime wTimeGet(){
    time_t RawTime;time(&RawTime);
    struct tm ProcessedTime;ProcessedTime=*localtime(&RawTime);
    wTime time={ProcessedTime.tm_hour,ProcessedTime.tm_min,
        ProcessedTime.tm_sec};
    return time;
}