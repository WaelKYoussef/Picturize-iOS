//
//  wColor.h
//  PhotoKit
//
//  Created by Wael Youssef on 4/6/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wColor_H
#define wColor_H

#pragma mark -
#pragma mark - wColor.
//Source http://prideout.net/archive/colors.php
typedef struct _wColor{float r,g,b,a;}wColor;
typedef struct _wColorRGBAComponents{unsigned char R,G,B,A;}wColorRGBAComponents;
typedef struct _wColorCMYKComponents{float c,m,y,k;}wColorCMYKComponents;


#pragma mark - wColorNew.
wColor wColorNewRGBA(unsigned char R,unsigned char G,
                     unsigned char B,unsigned char A);
wColor wColorNewRGBAFloat(float r, float g, float b, float a);
wColor wColorNewCMYK(float c, float m, float y, float k);
wColor wColorNewGreyScale(unsigned char G, unsigned char A);
wColor wColorNewGreyScaleFloat(float g, float a);

#pragma mark - wColorGetter.
wColorRGBAComponents wColorGetRGBAComponents(wColor color);
wColorCMYKComponents wColorGetCMYKComponents(wColor color);

#pragma mark - wColorConstants.
wColor wColorClear();
wColor wColorIndianRed();
wColor wColorLightCoral();
wColor wColorSalmon();
wColor wColorDarkSalmon();
wColor wColorLightSalmon();
wColor wColorCrimson();
wColor wColorRed();
wColor wColorFireBrick();
wColor wColorDarkRed();
wColor wColorPink();
wColor wColorLightPink();
wColor wColorHotPink();
wColor wColorDeepPink();
wColor wColorMediumVioletRed();
wColor wColorPaleVioletRed();
wColor wColorCoral();
wColor wColorTomato();
wColor wColorOrangeRed();
wColor wColorDarkOrange();
wColor wColorOrange();
wColor wColorGold();
wColor wColorYellow();
wColor wColorLightYellow();
wColor wColorLemonChion();
wColor wColorLightGoldenrodYellow();
wColor wColorPapayaWhip();
wColor wColorMoccasin();
wColor wColorPeachPu();
wColor wColorPaleGoldenrod();
wColor wColorKhaki();
wColor wColorDarkKhaki();
wColor wColorLavender();
wColor wColorThistle();
wColor wColorPlum();
wColor wColorViolet();
wColor wColorOrchid();
wColor wColorFuchsia();
wColor wColorMagenta();
wColor wColorMediumOrchid();
wColor wColorMediumPurple();
wColor wColorBlueViolet();
wColor wColorDarkViolet();
wColor wColorDarkOrchid();
wColor wColorDarkMagenta();
wColor wColorPurple();
wColor wColorIndigo();
wColor wColorSlateBlue();
wColor wColorDarkSlateBlue();
wColor wColorGreenYellow();
wColor wColorChartreuse();
wColor wColorLawnGreen();
wColor wColorLime();
wColor wColorLimeGreen();
wColor wColorPaleGreen();
wColor wColorLightGreen();
wColor wColorMediumSpringGreen();
wColor wColorSpringGreen();
wColor wColorMediumSeaGreen();
wColor wColorSeaGreen();
wColor wColorForestGreen();
wColor wColorGreen();
wColor wColorDarkGreen();
wColor wColorYellowGreen();
wColor wColorOliveDrab();
wColor wColorOlive();
wColor wColorDarkOliveGreen();
wColor wColorMediumAquamarine();
wColor wColorDarkSeaGreen();
wColor wColorLightSeaGreen();
wColor wColorDarkCyan();
wColor wColorTeal();
wColor wColorAqua();
wColor wColorCyan();
wColor wColorLightCyan();
wColor wColorPaleTurquoise();
wColor wColorAquamarine();
wColor wColorTurquoise();
wColor wColorMediumTurquoise();
wColor wColorDarkTurquoise();
wColor wColorCadetBlue();
wColor wColorSteelBlue();
wColor wColorLightSteelBlue();
wColor wColorPowderBlue();
wColor wColorLightBlue();
wColor wColorSkyBlue();
wColor wColorLightSkyBlue();
wColor wColorDeepSkyBlue();
wColor wColorDodgerBlue();
wColor wColorCornlowerBlue();
wColor wColorMediumSlateBlue();
wColor wColorRoyalBlue();
wColor wColorBlue();
wColor wColorMediumBlue();
wColor wColorDarkBlue();
wColor wColorNavy();
wColor wColorMidnightBlue();
wColor wColorCornsilk();
wColor wColorBlanchedAlmond();
wColor wColorBisque();
wColor wColorNavajoWhite();
wColor wColorWheat();
wColor wColorBurlyWood();
wColor wColorTan();
wColor wColorRosyBrown();
wColor wColorSandyBrown();
wColor wColorGoldenrod();
wColor wColorDarkGoldenrod();
wColor wColorPeru();
wColor wColorChocolate();
wColor wColorSaddleBrown();
wColor wColorSienna();
wColor wColorBrown();
wColor wColorMaroon();
wColor wColorWhite();
wColor wColorSnow();
wColor wColorHoneydew();
wColor wColorMintCream();
wColor wColorAzure();
wColor wColorAliceBlue();
wColor wColorGhostWhite();
wColor wColorWhiteSmoke();
wColor wColorSeashell();
wColor wColorBeige();
wColor wColorOldLace();
wColor wColorFloralWhite();
wColor wColorIvory();
wColor wColorAntiqueWhite();
wColor wColorLinen();
wColor wColorLavenderBlush();
wColor wColorMistyRose();
wColor wColorGainsboro();
wColor wColorLightGray();
wColor wColorSilver();
wColor wColorDarkGray();
wColor wColorGray();
wColor wColorDimGray();
wColor wColorLightSlateGray();
wColor wColorSlateGray();
wColor wColorDarkSlateGray();
wColor wColorBlack();

#endif