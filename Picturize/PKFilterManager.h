//
//  PKFilterManager.h
//  PhotoKit
//
//  Created by Wael Youssef on 6/25/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//


#include "wViewAPI.h"


void PKFilterManagerSetupWithTargetTexture(wTexture*texture);
void PKFilterManagerCaptureFrame(unsigned int filterTag);
void PKFilterManagerSetRenderTransformations(bool mirrorX, bool mirrorY, wDeviceOrientation orientation);
void PKFilterManagerSetDeviceDimentions(wSize dimentions);
void PKFilterManagerRenderToFrame(wRect frame, unsigned int filterTag);
void PKFilterManagerRenderFunction(void*data);

int PKFilterManagerNumberOfPacks();
int PKFilterManagerNumberOfFilters();
int PKFilterManagerNumberOfFiltersPerPack();
bool PKFilterManagerIsPackPurchased(int packID);
bool PKFilterManagerIsFilterPurchased(int filterID);
int PKFilterManagerPackIDFromFilterID(int filterID);
