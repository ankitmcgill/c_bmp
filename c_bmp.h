//###############################################################
//BMP FILE READING LIBRARY FOR C
//
//NOVEMBER 25 2018
//
//ANKIT BHATNAGAR
//ANKIT.BHATNAGARINDIA@GMAIL.COM
//###############################################################

#ifndef _C_BMP_
#define _C_BMP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

//BMP MAGIC BYTE
#define BF_TYPE         0x4D42      //"MB"

//CONSTANTS FOR THE BICOMPRESSION FIELD
#define BI_RGB          0           //NO COMPRESSION - STRAIGHT BGR DATA
#define BI_RLE8         1           //8-BIT RUN-LENGTH COMPRESSION
#define BI_RLE4         2           //4-BIT RUN-LENGTH COMPRESSION
#define BI_BITFIELDS    3           //RGB BITMAP WITH RGB MASKS

typedef enum
{
    PIXEL_FORMAT_RGB = 0,
    PIXEL_FORMAT_BRG,
    PIXEL_FORMAT_GRB
}bitmap_pixel_color_format_t;

//BMP FILE HEADER STRUCTURE
typedef struct __attribute__((__packed__))
{
    uint16_t bfType;          //MAGIC NUMBER FOR FILE
    uint32_t bfSize;          //SIZE OF FILE
    uint16_t bfReserved1;     //RESERVED
    uint16_t bfReserved2;     //...
    uint32_t bfOffBits;       //OFFSET TO BITMAP DATA
}bitmapfileheader_t;

//BMP FILE INFO STRUCTURE
typedef struct __attribute__((__packed__))
{
    uint32_t biSize;           //SIZE OF INFO HEADER
    uint32_t biWidth;          //WIDTH OF IMAGE
    uint32_t biHeight;         //HEIGHT OF IMAGE
    uint16_t biPlanes;         //NUMBER OF COLOR PLANES
    uint16_t biBitCount;       //NUMBER OF BITS PER PIXEL
    uint32_t biCompression;    //TYPE OF COMPRESSION TO USE
    uint32_t biSizeImage;      //SIZE OF IMAGE DATA
    uint32_t biXPelsPerMeter;  //X PIXELS PER METER
    uint32_t biYPelsPerMeter;  //Y PIXELS PER METER
    uint32_t biClrUsed;        //NUMBER OF COLORS USED
    uint32_t biClrImportant;   //NUMBER OF IMPORTANT COLORS
}bitmapinfoheader_t;

//PIXEL COLOR DATA (24 BIT, BRG ORDER)
typedef struct __attribute__((__packed__))
{
    uint8_t  b;     //BLUE VALUE
    uint8_t  r;     //GREEN VALUE
    uint8_t  g;     //RED VALUE
}pixel_data_t;

bool C_BMP_Init(void);

bool C_BMP_PrintBmpInformation(char* file_name);

bool C_BMP_ReadData(char* file_name, 
                    bitmap_pixel_color_format_t format, 
                    uint32_t* image_data);

#endif