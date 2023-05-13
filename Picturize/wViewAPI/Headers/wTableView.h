//
//  wTableView.h
//  PhotoKit
//
//  Created by Wael Youssef on 7/4/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wTableView_h
#define wTableView_h

typedef struct _wTableViewContext{
    wView*tableView;
    wArray*offScreenItems;
    wArray*onScreenItems;
    void(*selectedItemAtIndex)(int index);
    wView*(*viewForItem)();
    void(*presentItemAtIndex)(wView*item, int index);
    void(*cleanItemAtIndex)(wView*item, int index);

    float itemWidth;
    float itemHeight;
    float outlineSize;
    float rowSize;
    float columnSize;
    float offsetFromTop;
    float offsetFromBottom;
    
    float yOffset;
    float gridHeight;
    int start,end;
    int minRowCount;
    
    int itemCount;
    int itemsPerRow;
    int rowCount;
    
    bool squaredItems;
}wTableViewContext;

wView*wTableViewNew();
wView*wTableViewNewWithFrame(wRect frame);

void wTableViewLoad(wView*tableView);
void wTableViewRemoveAllItems(wView*tableView);
void wTableViewSetItemsPerRow(wView*tableView, int itemsPerRow);
void wTableViewSetItemHeight(wView*tableView, float itemHeight);
void wTableViewSetSquaredItems(wView*tableView, bool squaredItems);
void wTableViewSetItemsOutlineSize(wView*tableView, float outlineSize);
void wTableViewSetItemCount(wView*tableView, int itemCount);

wPoint wTableViewGetLocationOfItemAtIndex(wView*tableView, int index);
wSize wTableViewGetItemSize(wView*tableView);
int wTableViewGetItemCount(wView*tableView);

void wTableViewSetOffsetFromTop(wView*tableView, float offsetFromTop);
void wTableViewSetOffsetFromBottom(wView*tableView, float offsetFromBottom);

void wTableViewUpdateItemAtIndex(wView*tableView, int index);

void wTableViewSetSelectedItemAtIndexFunction(wView*tableView, void(*selectedItemAtIndex)(int index));
void wTableViewSetViewForItemAtIndexFunction(wView*tableView, wView*(*viewForItem)());
void wTableViewSetPresentItemAtIndexFunction(wView*tableView, void(*presentItemAtIndex)(wView*item, int index));
void wTableViewSetCleanItemAtIndexFunction(wView*tableView, void(*cleanItemAtIndex)(wView*item, int index));

#endif
