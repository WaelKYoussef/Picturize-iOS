//
//  wTimer.h
//  PhotoKit
//
//  Created by Wael Youssef on 5/15/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wTimer_H
#define wTimer_H

typedef  struct timeval wTimeStamp;

typedef struct _wDate{
    unsigned short Year;
    unsigned char Month,Day,Hour,Minute,Second;
}wDate;

typedef struct _wShortDate{
    unsigned short Year;
    unsigned char Month,Day;
}wShortDate;

typedef struct _wTime{
    unsigned char Hour,Minute,Second;
}wTime;

typedef void (*wTimerFunction)(void*userData);

wArray*wTimerMasterArray;

wTimer*wTimerNew(float duration, wFunction function, bool repeat, void*userData);
wTimeStamp wTimeStampGet();
float wTimerStampDifference(wTimeStamp timeStamp1, wTimeStamp timeStamp2);

void wTimerStart(wTimer*timer);
void wTimerStop(wTimer*timer);
void wTimerRelease(wTimer*timer);
bool wTimerGetIsRunning(wTimer*timer);
void wTimerExecuteFunction(wTimer*timer);

wDate wDateGet();
wShortDate wShortDateGet();
wTime wTimeGet();

#endif