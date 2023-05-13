//
//  wTableView.c
//  PhotoKit
//
//  Created by Wael Youssef on 7/4/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "wViewAPI.h"

wPoint GridIndexToLocation(int index, wTableViewContext*TVC){
    int Yindex=index/TVC[0].itemsPerRow;
    int rowStart=Yindex*TVC[0].itemsPerRow;
    int Xindex=index-rowStart;
    wPoint location=wPointNew(Xindex*TVC[0].columnSize, Yindex*TVC[0].rowSize);
    location.x+=TVC[0].outlineSize;
    location.y+=TVC[0].outlineSize+TVC[0].offsetFromTop;
    return location;
}

int GridLocationToIndex(wPoint location, wTableViewContext*TVC){
    wPoint offset=wScrollViewGetContentOffset(TVC[0].tableView);
    location=wPointAddPoints(location, offset);
    
    int x=location.x/TVC[0].columnSize;
    int y=(location.y-TVC[0].offsetFromTop)/TVC[0].rowSize;
    
    return (y*TVC[0].itemsPerRow)+x;
}

void wTableViewUpdateContentSize(wTableViewContext*TVC){
    if (TVC[0].itemsPerRow>0) {
        TVC[0].rowCount=TVC[0].itemCount/TVC[0].itemsPerRow;}
    else{TVC[0].rowCount=0;}
    if ((TVC[0].itemCount-(TVC[0].rowCount*TVC[0].itemsPerRow))>0) {TVC[0].rowCount++;}
    wScrollViewSetContentSize(TVC[0].tableView, wSizeNew(0, (TVC[0].rowSize*TVC[0].rowCount)+TVC[0].outlineSize+TVC[0].offsetFromTop+TVC[0].offsetFromBottom));
    
}


wView*wTableViewGetItemSlot(wTableViewContext*TVC){
    wView*itemSlot=NULL;
    if (wArrayGetCount(TVC[0].offScreenItems)>0){
        itemSlot=wArrayGetItemAtIndex(TVC[0].offScreenItems, 0);
        wArrayRemoveItemAtIndex(TVC[0].offScreenItems, 0);
    }
    return itemSlot;
}

void wTableViewAddItem(wTableViewContext*TVC, int index){
    if (index>=0 && index<TVC[0].itemCount) {
        wView*itemSlot=wTableViewGetItemSlot(TVC);
        if (!itemSlot) {
            if (TVC[0].viewForItem) {itemSlot=TVC[0].viewForItem();}
            else{itemSlot=wViewNew();}
        }
        
        wPoint loc=GridIndexToLocation(index, TVC);
        wRect frame=wRectNew(loc.x, loc.y, TVC[0].itemWidth, TVC[0].itemHeight);
        wViewSetFrame(itemSlot, frame);
        wViewSetTag(itemSlot, index);
        if (TVC[0].presentItemAtIndex) {
            TVC[0].presentItemAtIndex(itemSlot, index);}
        
        wArrayAddItem(TVC[0].onScreenItems, itemSlot);
        wViewAddSubviewAtIndex(TVC[0].tableView, itemSlot,0);
    }
}

void wTableViewAddItems(wTableViewContext*TVC, int Af,int At){
    for (int i=Af; i<=At; i++) {
        wTableViewAddItem(TVC, i);
    }
}

void wTableViewRemoveAllItems(wView*tableView){
    wTableViewContext*TVC=tableView[0].tableContext;
    for (int i=wArrayGetCount(TVC[0].onScreenItems)-1;i>=0;i--){
        wView*itemSlot=wArrayGetItemAtIndex(TVC[0].onScreenItems, i);
        wArrayAddItem(TVC[0].offScreenItems, itemSlot);
        wArrayRemoveItemAtIndex(TVC[0].onScreenItems, i);
        wViewRemoveFromSuperView(itemSlot);
        if (TVC[0].cleanItemAtIndex) {TVC[0].cleanItemAtIndex(itemSlot, i);}
    }
}

void wTableViewRemoveItems(wTableViewContext*TVC, int Rf,int Rt){
    for (int i=wArrayGetCount(TVC[0].onScreenItems)-1;i>=0;i--){
        wView*itemSlot=wArrayGetItemAtIndex(TVC[0].onScreenItems, i);
        if(wViewGetTag(itemSlot)>=Rf&&wViewGetTag(itemSlot)<=Rt){
            wArrayAddItem(TVC[0].offScreenItems, itemSlot);
            wArrayRemoveItemAtIndex(TVC[0].onScreenItems, i);
            wViewRemoveFromSuperView(itemSlot);
            if (TVC[0].cleanItemAtIndex) {
                TVC[0].cleanItemAtIndex(itemSlot, itemSlot[0].tag);}
        }
    }
}

void wTableViewScroll(void*scrollView){
    wView*tableView=scrollView;
    wTableViewContext*TVC=tableView[0].tableContext;
    float NYOffset=wScrollViewGetContentOffset(tableView).y-TVC[0].offsetFromTop;
    int NStart=NYOffset/TVC[0].rowSize;
    int NEnd=NStart+TVC[0].minRowCount;
    while((NEnd+1)*TVC[0].rowSize<(NYOffset+TVC[0].gridHeight))NEnd++;
    int RF=0,RT=0,AF=0,AT=0;
    bool Remove=false,Add=false;
    
    if (NYOffset>TVC[0].yOffset){
        if (NStart>TVC[0].start){Remove=true,RF=TVC[0].start;
            if (TVC[0].end<NStart)RT=TVC[0].end;else RT=NStart-1;}
        if (NEnd>TVC[0].end){Add=true,AT=NEnd;
            if(TVC[0].end<NStart)AF=NStart;else AF=TVC[0].end+1;}}
    else{
        if (TVC[0].end>NEnd){Remove=true,RT=TVC[0].end;
            if (NEnd<TVC[0].start)RF=TVC[0].start;else RF=NEnd+1;}
        if (TVC[0].start>NStart){Add=true,AF=NStart;
            if (NEnd<TVC[0].start)AT=NEnd;else AT=TVC[0].start-1;}}
    
    if (Remove){
        int from=RF*TVC[0].itemsPerRow;
        int to=(RT*TVC[0].itemsPerRow)+(TVC[0].itemsPerRow-1);
        wTableViewRemoveItems(TVC, from, to);
    }
    if (Add){
        int from=AF*TVC[0].itemsPerRow;
        int to=(AT*TVC[0].itemsPerRow)+(TVC[0].itemsPerRow-1);
        wTableViewAddItems(TVC, from, to);
    }
    
    TVC[0].yOffset=NYOffset, TVC[0].start=NStart, TVC[0].end=NEnd;
}

void wTableViewLoad(wView*tableView){
    wTableViewContext*TVC=tableView[0].tableContext;
    wRect tableViewFrame=wViewGetFrame(TVC[0].tableView);
    TVC[0].itemWidth=(tableViewFrame.size.width-((TVC[0].itemsPerRow+1)*TVC[0].outlineSize))/TVC[0].itemsPerRow;
    
    TVC[0].itemWidth=floorf(TVC[0].itemWidth);
    
    if (TVC[0].squaredItems) {TVC[0].itemHeight=TVC[0].itemWidth;}
    
    TVC[0].rowSize=TVC[0].itemHeight+TVC[0].outlineSize;
    TVC[0].columnSize=TVC[0].itemWidth+TVC[0].outlineSize;
    
    TVC[0].yOffset=wScrollViewGetContentOffset(TVC[0].tableView).y-TVC[0].offsetFromTop;
    TVC[0].gridHeight=wViewGetFrame(TVC[0].tableView).size.height+TVC[0].offsetFromTop;
    TVC[0].minRowCount=TVC[0].gridHeight/TVC[0].rowSize;
    TVC[0].start=TVC[0].yOffset/TVC[0].rowSize;
    TVC[0].end=TVC[0].start+TVC[0].minRowCount;
    while((TVC[0].end+1)*TVC[0].rowSize<(TVC[0].yOffset+TVC[0].gridHeight))TVC[0].end++;
    
    wTableViewRemoveAllItems(tableView);
    int from=TVC[0].start*TVC[0].itemsPerRow;
    int to=(TVC[0].end*TVC[0].itemsPerRow)+(TVC[0].itemsPerRow-1);
    wTableViewAddItems(TVC, from, to);
    
    wTableViewUpdateContentSize(TVC);
}

void wTableViewSelectedItemAtIndex(void*gesture){
    wPoint location=wGestureGetLocationAtIndex(gesture, 0);
    wTableViewContext*TVC=wGestureGetView(gesture)[0].tableContext;
    int index=GridLocationToIndex(location, TVC);

    if (index>=0&&index<TVC[0].itemCount) {
        if (TVC[0].selectedItemAtIndex) {
            TVC[0].selectedItemAtIndex(index);
        }
    }
}

int wTableViewGetItemCount(wView*tableView){
    return tableView[0].tableContext[0].itemCount;
}

void wTableViewUpdateItemAtIndex(wView*tableView, int index){
    wTableViewContext*TVC=tableView[0].tableContext;
    if (TVC[0].itemCount>index) {
        if (TVC[0].start<=index || TVC[0].end>=index) {
            wTableViewRemoveItems(TVC, index, index);
            wTableViewAddItem(TVC, index);
        }
    }
}

void wViewSetTableVariables(wView*view){
    view[0].tableContext=malloc(sizeof(wTableViewContext));
    memset(view[0].tableContext, 0, sizeof(wTableViewContext));
    view[0].tableContext[0].tableView=view;
    view[0].tableContext[0].offScreenItems=wArrayNewWithCapacity(10);
    view[0].tableContext[0].onScreenItems=wArrayNewWithCapacity(10);
    wGesture*tap=wGestureNew(wGestureTap);
    wGestureSetFunction(tap, wTableViewSelectedItemAtIndex);
    wViewAddGesture(view, tap);
    wViewSetUserInteractionEnabled(view, true);
    wScrollViewSetDidScrollFunction(view, wTableViewScroll);
    
    view[0].clipsSubviews=true;
    view[0].type=wViewTypeTableView;
    view[0].tableContext[0].itemHeight=100.0;
    view[0].tableContext[0].presentItemAtIndex=NULL;
    view[0].tableContext[0].viewForItem=NULL;
    view[0].tableContext[0].selectedItemAtIndex=NULL;
    view[0].tableContext[0].cleanItemAtIndex=NULL;
}

wView*wTableViewNew(){
    wView*view=wScrollViewNew();
    wViewSetTableVariables(view);
    return view;
}

wView*wTableViewNewWithFrame(wRect frame){
    wView*view=wScrollViewNewWithFrame(frame);
    wViewSetTableVariables(view);
    return view;
}



#pragma mark - wTableViewGetters.
wPoint wTableViewGetLocationOfItemAtIndex(wView*tableView, int index){
    return GridIndexToLocation(index, tableView[0].tableContext);
}

wSize wTableViewGetItemSize(wView*tableView){
    return wSizeNew(tableView[0].tableContext[0].itemWidth,
                    tableView[0].tableContext[0].itemHeight);
}


#pragma mark - wTableViewSetters.
void wTableViewSetItemsPerRow(wView*tableView, int itemsPerRow){
    tableView[0].tableContext[0].itemsPerRow=itemsPerRow;
    wTableViewUpdateContentSize(tableView[0].tableContext);
}

void wTableViewSetItemHeight(wView*tableView, float itemHeight){
    tableView[0].tableContext[0].itemHeight=itemHeight;
    wTableViewUpdateContentSize(tableView[0].tableContext);
}

void wTableViewSetSquaredItems(wView*tableView, bool squaredItems){
    tableView[0].tableContext[0].squaredItems=squaredItems;
    wTableViewUpdateContentSize(tableView[0].tableContext);
}

void wTableViewSetItemsOutlineSize(wView*tableView, float outlineSize){
    tableView[0].tableContext[0].outlineSize=outlineSize;
    wTableViewUpdateContentSize(tableView[0].tableContext);
}
void wTableViewSetItemCount(wView*tableView, int itemCount){
    tableView[0].tableContext[0].itemCount=itemCount;
    wTableViewUpdateContentSize(tableView[0].tableContext);
}
void wTableViewSetSelectedItemAtIndexFunction(wView*tableView, void(*selectedItemAtIndex)(int index)){
    tableView[0].tableContext[0].selectedItemAtIndex=selectedItemAtIndex;
}

void wTableViewSetViewForItemAtIndexFunction(wView*tableView, wView*(*viewForItem)()){
    tableView[0].tableContext[0].viewForItem=viewForItem;
}
void wTableViewSetPresentItemAtIndexFunction(wView*tableView, void(*presentItemAtIndex)(wView*item, int index)){
    tableView[0].tableContext[0].presentItemAtIndex=presentItemAtIndex;
}
void wTableViewSetCleanItemAtIndexFunction(wView*tableView, void(*cleanItemAtIndex)(wView*item, int index)){
    tableView[0].tableContext[0].cleanItemAtIndex=cleanItemAtIndex;
}
void wTableViewSetOffsetFromTop(wView*tableView, float offsetFromTop){
    tableView[0].tableContext[0].offsetFromTop=offsetFromTop;
}
void wTableViewSetOffsetFromBottom(wView*tableView, float offsetFromBottom){
    tableView[0].tableContext[0].offsetFromBottom=offsetFromBottom;
}
