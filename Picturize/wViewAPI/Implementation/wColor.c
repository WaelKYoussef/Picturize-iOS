//
//  wColor.c
//  PhotoKit
//
//  Created by Wael Youssef on 4/6/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "wViewAPI.h"

#pragma mark -
#pragma mark - wColor.
float max(float r,float g, float b){
    float result=r;
    if (result<g) {result=g;}
    if (result<b) {result=b;}
    return result;
}



#pragma mark -
#pragma mark - wColorNew.
wColor wColorNewRGBA(unsigned char R,unsigned char G,
                     unsigned char B,unsigned char A){
    wColor color={R/255.0,G/255.0,B/255.0,A/255.0};
    return color;
}
wColor wColorNewRGBAFloat(float r, float g, float b, float a){
    wColor color={r,g,b,a};
    return color;
}
wColor wColorNewCMYK(float c, float m, float y, float k){
    wColor color={(1-c)*(1-k),(1-m)*(1-k),(1-y)*(1-k),1.0};
    return color;
}
wColor wColorNewGreyScale(unsigned char G, unsigned char A){
    float g=G/255.0;
    wColor color={g,g,g,A/255.0};
    return color;
}
wColor wColorNewGreyScaleFloat(float g, float a){
    wColor color={g,g,g,a};
    return color;
}



#pragma mark -
#pragma mark - wColorGetter.
wColorRGBAComponents wColorGetRGBAComponents(wColor color){
    wColorRGBAComponents components={
        color.r*255,color.g*255,color.b*255,color.a*255};
    return components;
}
wColorCMYKComponents wColorGetCMYKComponents(wColor color){
    float k=1-max(color.r, color.g, color.b);
    wColorCMYKComponents components={
        (1-color.r-k)/(1-k),
        (1-color.g-k)/(1-k),
        (1-color.b-k)/(1-k),
        k};
    return components;
}



#pragma mark -
#pragma mark - wColorConstants.
wColor wColorClear(){ return wColorNewRGBAFloat(0.0, 0.0, 0.0, 0.0);}
wColor wColorIndianRed(){ return wColorNewRGBAFloat(0.804, 0.361, 0.361, 1.0);}
wColor wColorLightCoral(){ return wColorNewRGBAFloat(0.941, 0.502, 0.502, 1.0);}
wColor wColorSalmon(){ return wColorNewRGBAFloat(0.980, 0.502, 0.447, 1.0);}
wColor wColorDarkSalmon(){ return wColorNewRGBAFloat(0.914, 0.588, 0.478, 1.0);}
wColor wColorLightSalmon(){ return wColorNewRGBAFloat(1.000, 0.627, 0.478, 1.0);}
wColor wColorCrimson(){ return wColorNewRGBAFloat(0.863, 0.078, 0.235, 1.0);}
wColor wColorRed(){ return wColorNewRGBAFloat(1.000, 0.000, 0.000, 1.0);}
wColor wColorFireBrick(){ return wColorNewRGBAFloat(0.698, 0.133, 0.133, 1.0);}
wColor wColorDarkRed(){ return wColorNewRGBAFloat(0.545, 0.000, 0.000, 1.0);}
wColor wColorPink(){ return wColorNewRGBAFloat(1.000, 0.753, 0.796, 1.0);}
wColor wColorLightPink(){ return wColorNewRGBAFloat(1.000, 0.714, 0.757, 1.0);}
wColor wColorHotPink(){ return wColorNewRGBAFloat(1.000, 0.412, 0.706, 1.0);}
wColor wColorDeepPink(){ return wColorNewRGBAFloat(1.000, 0.078, 0.576, 1.0);}
wColor wColorMediumVioletRed(){ return wColorNewRGBAFloat(0.780, 0.082, 0.522, 1.0);}
wColor wColorPaleVioletRed(){ return wColorNewRGBAFloat(0.859, 0.439, 0.576, 1.0);}
wColor wColorCoral(){ return wColorNewRGBAFloat(1.000, 0.498, 0.314, 1.0);}
wColor wColorTomato(){ return wColorNewRGBAFloat(1.000, 0.388, 0.278, 1.0);}
wColor wColorOrangeRed(){ return wColorNewRGBAFloat(1.000, 0.271, 0.000, 1.0);}
wColor wColorDarkOrange(){ return wColorNewRGBAFloat(1.000, 0.549, 0.000, 1.0);}
wColor wColorOrange(){ return wColorNewRGBAFloat(1.000, 0.647, 0.000, 1.0);}
wColor wColorGold(){ return wColorNewRGBAFloat(1.000, 0.843, 0.000, 1.0);}
wColor wColorYellow(){ return wColorNewRGBAFloat(1.000, 1.000, 0.000, 1.0);}
wColor wColorLightYellow(){ return wColorNewRGBAFloat(1.000, 1.000, 0.878, 1.0);}
wColor wColorLemonChion(){ return wColorNewRGBAFloat(1.000, 0.980, 0.804, 1.0);}
wColor wColorLightGoldenrodYellow(){ return wColorNewRGBAFloat(0.980, 0.980, 0.824, 1.0);}
wColor wColorPapayaWhip(){ return wColorNewRGBAFloat(1.000, 0.937, 0.835, 1.0);}
wColor wColorMoccasin(){ return wColorNewRGBAFloat(1.000, 0.894, 0.710, 1.0);}
wColor wColorPeachPu(){ return wColorNewRGBAFloat(1.000, 0.855, 0.725, 1.0);}
wColor wColorPaleGoldenrod(){ return wColorNewRGBAFloat(0.933, 0.910, 0.667, 1.0);}
wColor wColorKhaki(){ return wColorNewRGBAFloat(0.941, 0.902, 0.549, 1.0);}
wColor wColorDarkKhaki(){ return wColorNewRGBAFloat(0.741, 0.718, 0.420, 1.0);}
wColor wColorLavender(){ return wColorNewRGBAFloat(0.902, 0.902, 0.980, 1.0);}
wColor wColorThistle(){ return wColorNewRGBAFloat(0.847, 0.749, 0.847, 1.0);}
wColor wColorPlum(){ return wColorNewRGBAFloat(0.867, 0.627, 0.867, 1.0);}
wColor wColorViolet(){ return wColorNewRGBAFloat(0.933, 0.510, 0.933, 1.0);}
wColor wColorOrchid(){ return wColorNewRGBAFloat(0.855, 0.439, 0.839, 1.0);}
wColor wColorFuchsia(){ return wColorNewRGBAFloat(1.000, 0.000, 1.000, 1.0);}
wColor wColorMagenta(){ return wColorNewRGBAFloat(1.000, 0.000, 1.000, 1.0);}
wColor wColorMediumOrchid(){ return wColorNewRGBAFloat(0.729, 0.333, 0.827, 1.0);}
wColor wColorMediumPurple(){ return wColorNewRGBAFloat(0.576, 0.439, 0.859, 1.0);}
wColor wColorBlueViolet(){ return wColorNewRGBAFloat(0.541, 0.169, 0.886, 1.0);}
wColor wColorDarkViolet(){ return wColorNewRGBAFloat(0.580, 0.000, 0.827, 1.0);}
wColor wColorDarkOrchid(){ return wColorNewRGBAFloat(0.600, 0.196, 0.800, 1.0);}
wColor wColorDarkMagenta(){ return wColorNewRGBAFloat(0.545, 0.000, 0.545, 1.0);}
wColor wColorPurple(){ return wColorNewRGBAFloat(0.502, 0.000, 0.502, 1.0);}
wColor wColorIndigo(){ return wColorNewRGBAFloat(0.294, 0.000, 0.510, 1.0);}
wColor wColorSlateBlue(){ return wColorNewRGBAFloat(0.416, 0.353, 0.804, 1.0);}
wColor wColorDarkSlateBlue(){ return wColorNewRGBAFloat(0.282, 0.239, 0.545, 1.0);}
wColor wColorGreenYellow(){ return wColorNewRGBAFloat(0.678, 1.000, 0.184, 1.0);}
wColor wColorChartreuse(){ return wColorNewRGBAFloat(0.498, 1.000, 0.000, 1.0);}
wColor wColorLawnGreen(){ return wColorNewRGBAFloat(0.486, 0.988, 0.000, 1.0);}
wColor wColorLime(){ return wColorNewRGBAFloat(0.000, 1.000, 0.000, 1.0);}
wColor wColorLimeGreen(){ return wColorNewRGBAFloat(0.196, 0.804, 0.196, 1.0);}
wColor wColorPaleGreen(){ return wColorNewRGBAFloat(0.596, 0.984, 0.596, 1.0);}
wColor wColorLightGreen(){ return wColorNewRGBAFloat(0.565, 0.933, 0.565, 1.0);}
wColor wColorMediumSpringGreen(){ return wColorNewRGBAFloat(0.000, 0.980, 0.604, 1.0);}
wColor wColorSpringGreen(){ return wColorNewRGBAFloat(0.000, 1.000, 0.498, 1.0);}
wColor wColorMediumSeaGreen(){ return wColorNewRGBAFloat(0.235, 0.702, 0.443, 1.0);}
wColor wColorSeaGreen(){ return wColorNewRGBAFloat(0.180, 0.545, 0.341, 1.0);}
wColor wColorForestGreen(){ return wColorNewRGBAFloat(0.133, 0.545, 0.133, 1.0);}
wColor wColorGreen(){ return wColorNewRGBAFloat(0.000, 0.502, 0.000, 1.0);}
wColor wColorDarkGreen(){ return wColorNewRGBAFloat(0.000, 0.392, 0.000, 1.0);}
wColor wColorYellowGreen(){ return wColorNewRGBAFloat(0.604, 0.804, 0.196, 1.0);}
wColor wColorOliveDrab(){ return wColorNewRGBAFloat(0.420, 0.557, 0.137, 1.0);}
wColor wColorOlive(){ return wColorNewRGBAFloat(0.502, 0.502, 0.000, 1.0);}
wColor wColorDarkOliveGreen(){ return wColorNewRGBAFloat(0.333, 0.420, 0.184, 1.0);}
wColor wColorMediumAquamarine(){ return wColorNewRGBAFloat(0.400, 0.804, 0.667, 1.0);}
wColor wColorDarkSeaGreen(){ return wColorNewRGBAFloat(0.561, 0.737, 0.561, 1.0);}
wColor wColorLightSeaGreen(){ return wColorNewRGBAFloat(0.125, 0.698, 0.667, 1.0);}
wColor wColorDarkCyan(){ return wColorNewRGBAFloat(0.000, 0.545, 0.545, 1.0);}
wColor wColorTeal(){ return wColorNewRGBAFloat(0.000, 0.502, 0.502, 1.0);}
wColor wColorAqua(){ return wColorNewRGBAFloat(0.000, 1.000, 1.000, 1.0);}
wColor wColorCyan(){ return wColorNewRGBAFloat(0.000, 1.000, 1.000, 1.0);}
wColor wColorLightCyan(){ return wColorNewRGBAFloat(0.878, 1.000, 1.000, 1.0);}
wColor wColorPaleTurquoise(){ return wColorNewRGBAFloat(0.686, 0.933, 0.933, 1.0);}
wColor wColorAquamarine(){ return wColorNewRGBAFloat(0.498, 1.000, 0.831, 1.0);}
wColor wColorTurquoise(){ return wColorNewRGBAFloat(0.251, 0.878, 0.816, 1.0);}
wColor wColorMediumTurquoise(){ return wColorNewRGBAFloat(0.282, 0.820, 0.800, 1.0);}
wColor wColorDarkTurquoise(){ return wColorNewRGBAFloat(0.000, 0.808, 0.820, 1.0);}
wColor wColorCadetBlue(){ return wColorNewRGBAFloat(0.373, 0.620, 0.627, 1.0);}
wColor wColorSteelBlue(){ return wColorNewRGBAFloat(0.275, 0.510, 0.706, 1.0);}
wColor wColorLightSteelBlue(){ return wColorNewRGBAFloat(0.690, 0.769, 0.871, 1.0);}
wColor wColorPowderBlue(){ return wColorNewRGBAFloat(0.690, 0.878, 0.902, 1.0);}
wColor wColorLightBlue(){ return wColorNewRGBAFloat(0.678, 0.847, 0.902, 1.0);}
wColor wColorSkyBlue(){ return wColorNewRGBAFloat(0.529, 0.808, 0.922, 1.0);}
wColor wColorLightSkyBlue(){ return wColorNewRGBAFloat(0.529, 0.808, 0.980, 1.0);}
wColor wColorDeepSkyBlue(){ return wColorNewRGBAFloat(0.000, 0.749, 1.000, 1.0);}
wColor wColorDodgerBlue(){ return wColorNewRGBAFloat(0.118, 0.565, 1.000, 1.0);}
wColor wColorCornlowerBlue(){ return wColorNewRGBAFloat(0.392, 0.584, 0.929, 1.0);}
wColor wColorMediumSlateBlue(){ return wColorNewRGBAFloat(0.482, 0.408, 0.933, 1.0);}
wColor wColorRoyalBlue(){ return wColorNewRGBAFloat(0.255, 0.412, 0.882, 1.0);}
wColor wColorBlue(){ return wColorNewRGBAFloat(0.000, 0.000, 1.000, 1.0);}
wColor wColorMediumBlue(){ return wColorNewRGBAFloat(0.000, 0.000, 0.804, 1.0);}
wColor wColorDarkBlue(){ return wColorNewRGBAFloat(0.000, 0.000, 0.545, 1.0);}
wColor wColorNavy(){ return wColorNewRGBAFloat(0.000, 0.000, 0.502, 1.0);}
wColor wColorMidnightBlue(){ return wColorNewRGBAFloat(0.098, 0.098, 0.439, 1.0);}
wColor wColorCornsilk(){ return wColorNewRGBAFloat(1.000, 0.973, 0.863, 1.0);}
wColor wColorBlanchedAlmond(){ return wColorNewRGBAFloat(1.000, 0.922, 0.804, 1.0);}
wColor wColorBisque(){ return wColorNewRGBAFloat(1.000, 0.894, 0.769, 1.0);}
wColor wColorNavajoWhite(){ return wColorNewRGBAFloat(1.000, 0.871, 0.678, 1.0);}
wColor wColorWheat(){ return wColorNewRGBAFloat(0.961, 0.871, 0.702, 1.0);}
wColor wColorBurlyWood(){ return wColorNewRGBAFloat(0.871, 0.722, 0.529, 1.0);}
wColor wColorTan(){ return wColorNewRGBAFloat(0.824, 0.706, 0.549, 1.0);}
wColor wColorRosyBrown(){ return wColorNewRGBAFloat(0.737, 0.561, 0.561, 1.0);}
wColor wColorSandyBrown(){ return wColorNewRGBAFloat(0.957, 0.643, 0.376, 1.0);}
wColor wColorGoldenrod(){ return wColorNewRGBAFloat(0.855, 0.647, 0.125, 1.0);}
wColor wColorDarkGoldenrod(){ return wColorNewRGBAFloat(0.722, 0.525, 0.043, 1.0);}
wColor wColorPeru(){ return wColorNewRGBAFloat(0.804, 0.522, 0.247, 1.0);}
wColor wColorChocolate(){ return wColorNewRGBAFloat(0.824, 0.412, 0.118, 1.0);}
wColor wColorSaddleBrown(){ return wColorNewRGBAFloat(0.545, 0.271, 0.075, 1.0);}
wColor wColorSienna(){ return wColorNewRGBAFloat(0.627, 0.322, 0.176, 1.0);}
wColor wColorBrown(){ return wColorNewRGBAFloat(0.647, 0.165, 0.165, 1.0);}
wColor wColorMaroon(){ return wColorNewRGBAFloat(0.502, 0.000, 0.000, 1.0);}
wColor wColorWhite(){ return wColorNewRGBAFloat(1.000, 1.000, 1.000, 1.0);}
wColor wColorSnow(){ return wColorNewRGBAFloat(1.000, 0.980, 0.980, 1.0);}
wColor wColorHoneydew(){ return wColorNewRGBAFloat(0.941, 1.000, 0.941, 1.0);}
wColor wColorMintCream(){ return wColorNewRGBAFloat(0.961, 1.000, 0.980, 1.0);}
wColor wColorAzure(){ return wColorNewRGBAFloat(0.941, 1.000, 1.000, 1.0);}
wColor wColorAliceBlue(){ return wColorNewRGBAFloat(0.941, 0.973, 1.000, 1.0);}
wColor wColorGhostWhite(){ return wColorNewRGBAFloat(0.973, 0.973, 1.000, 1.0);}
wColor wColorWhiteSmoke(){ return wColorNewRGBAFloat(0.961, 0.961, 0.961, 1.0);}
wColor wColorSeashell(){ return wColorNewRGBAFloat(1.000, 0.961, 0.933, 1.0);}
wColor wColorBeige(){ return wColorNewRGBAFloat(0.961, 0.961, 0.863, 1.0);}
wColor wColorOldLace(){ return wColorNewRGBAFloat(0.992, 0.961, 0.902, 1.0);}
wColor wColorFloralWhite(){ return wColorNewRGBAFloat(1.000, 0.980, 0.941, 1.0);}
wColor wColorIvory(){ return wColorNewRGBAFloat(1.000, 1.000, 0.941, 1.0);}
wColor wColorAntiqueWhite(){ return wColorNewRGBAFloat(0.980, 0.922, 0.843, 1.0);}
wColor wColorLinen(){ return wColorNewRGBAFloat(0.980, 0.941, 0.902, 1.0);}
wColor wColorLavenderBlush(){ return wColorNewRGBAFloat(1.000, 0.941, 0.961, 1.0);}
wColor wColorMistyRose(){ return wColorNewRGBAFloat(1.000, 0.894, 0.882, 1.0);}
wColor wColorGainsboro(){ return wColorNewRGBAFloat(0.863, 0.863, 0.863, 1.0);}
wColor wColorLightGray(){ return wColorNewRGBAFloat(0.827, 0.827, 0.827, 1.0);}
wColor wColorSilver(){ return wColorNewRGBAFloat(0.753, 0.753, 0.753, 1.0);}
wColor wColorDarkGray(){ return wColorNewRGBAFloat(0.663, 0.663, 0.663, 1.0);}
wColor wColorGray(){ return wColorNewRGBAFloat(0.502, 0.502, 0.502, 1.0);}
wColor wColorDimGray(){ return wColorNewRGBAFloat(0.412, 0.412, 0.412, 1.0);}
wColor wColorLightSlateGray(){ return wColorNewRGBAFloat(0.467, 0.533, 0.600, 1.0);}
wColor wColorSlateGray(){ return wColorNewRGBAFloat(0.439, 0.502, 0.565, 1.0);}
wColor wColorDarkSlateGray(){ return wColorNewRGBAFloat(0.184, 0.310, 0.310, 1.0);}
wColor wColorBlack(){ return wColorNewRGBAFloat(0.000, 0.000, 0.000, 1.0);}
