//
//  PKShiftFilters.h
//  PhotoKit
//
//  Created by Wael Youssef on 6/25/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "wViewAPI.h"


typedef struct _ShiftPoint{
    float x,y;
}ShiftPoint;

typedef struct _ShiftVertex{
    ShiftPoint location;
    ShiftPoint textureCoordinates;
}ShiftvVertex;

typedef struct _ShiftVertexArray{
    ShiftvVertex*list;
    unsigned int count, capacity;
    bool drawStripes;
}ShiftVertexArray;

void PKShiftInitFilters();
ShiftVertexArray*PKShiftFilterGetVerticiesForShiftID(int filterID);