
/***
 * @Author          : lihugang
 * @Date            : 2022-07-07 22:37:55
 * @LastEditTime    : 2022-07-08 21:58:15
 * @LastEditors     : lihugang
 * @Description     :
 * @FilePath        : \environment_detect.h
 * @Copyright (c) lihugang
 * @长风破浪会有时 直挂云帆济沧海
 * @There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 * @ * * *
 * @是非成败转头空 青山依旧在 几度夕阳红
 * @Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
#ifndef _WIN32
    #error Pig Web Server only support platform: Win32
#endif

#ifndef __STDC_VERSION__
    #error Failed to get the C Standard which compiler uses
#endif

#if __STDC_VERSION__ != 199901L
    #error Please use C99 standard to compile this file
#endif

// #pragma message("Pig Web Server")
// #ifdef DEBUG
//     #pragma message("DEBUG MODE")
// #endif
// #ifdef RELEASE
//     #pragma message("RELEASE MODE")
//     #pragma message(RELEASE)
// #endif
// #pragma message("C Standard:" __STDC_VERSION__)

// #pragma message("Official Website: https://lihugang.github.io/pig-web-server/");