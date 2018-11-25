//###############################################################
//BMP FILE READING LIBRARY FOR C
//
//NOVEMBER 25 2018
//
//ANKIT BHATNAGAR
//ANKIT.BHATNAGARINDIA@GMAIL.COM
//###############################################################

#include "c_bmp.h"

//INTERNAL VARIABLES


//INTERNAL FUNCTIONS


bool C_BMP_Init(void)
{
    //INITIALIZE C_BMP

    //NOTHING TO DO
    printf("%s OK\n", __FUNCTION__);
    return true;
}

bool C_BMP_PrintBmpInformation(char* file_name)
{
    //OPEN SPECIFIED BITMAP FILE AND PRINT
    //ITS BITMAP INFORMATION

    FILE* fp;
    bitmapfileheader_t bmp_header;
    bitmapinfoheader_t bmp_info_header;
    pixel_data_t bmp_pixel_data;
    uint32_t x;
    uint32_t y;

    //OPEN FILE
    if ((fp = fopen(file_name, "rb")) == NULL)
    {
        return false;
    }
    
    //READ FILE BMP HEADER
    if (fread(&bmp_header, sizeof(bitmapfileheader_t), 1, fp) < 1)
    {
        fclose(fp);
        return false;
    }

    //MAKE SURE ITS BITMAP FILE
    if(bmp_header.bfType != BF_TYPE)
    {
        //NOT A BMP FILE
        fclose(fp);
        return false;
    }

    printf("BMP HEADER :\n");
    printf("Magic Bytes(bytes) : %04x\n", bmp_header.bfType);
    printf("Bitmap File Size(bytes) : %u\n", bmp_header.bfSize);
    printf("Image Data Offset(bytes) : %u\n", bmp_header.bfOffBits);

    printf("\n");

    //READ FILE BMP HEADER
    if (fread(&bmp_info_header, sizeof(bitmapinfoheader_t), 1, fp) < 1)
    {
        fclose(fp);
        return false;
    }

    printf("BITMAP INFO HEADER :\n");
    printf("Image Width(pixel) : %u\n", bmp_info_header.biWidth);
    printf("Image Height(pixel) : %u\n", bmp_info_header.biHeight);
    printf("Bits Per Pixel : %u\n", bmp_info_header.biBitCount);
    printf("Image Compression : ");
    switch(bmp_info_header.biCompression)
    {
        case BI_RGB:
            printf("RGB - No Compression\n");
            break;
        
        case BI_RLE8:
            printf("8 Bit Run Length Compression\n");
            break;
        
        case BI_RLE4:
            printf("4 Bit Run Length Compression\n");
            break;

        case BI_BITFIELDS:
            printf("RGB Bitmap With RGB Masks\n");
            break;

        default:
            printf("Other\n");
    }
    printf("Image Data Size(bytes) : %u\n", bmp_info_header.biSizeImage);

    printf("\n");

    //PRINT IMAGE DATA
    //BMP STORES PIXELS FROM LEFT -> RIGHT, BOTTOM -> TOP
    printf("IMAGE DATA : \n");
    for(y = 0; y < bmp_info_header.biHeight; y++)
    {
        for(x = 0; x < bmp_info_header.biWidth; x++)
        {
            fread(&bmp_pixel_data, sizeof(pixel_data_t), 1, fp);
            printf("%02x%02x%02x ", bmp_pixel_data.b, bmp_pixel_data.r, bmp_pixel_data.g);
        }
        printf("\n");
    }
    printf("\n");

    fclose(fp);
    return true;
}

bool C_BMP_ReadData(char* file_name, 
                    bitmap_pixel_color_format_t format, 
                    uint32_t* image_data)
{
    //RETURN BMP IMAGE DATA ARRAY WITH TOTAL SIZE
    //BMP STORES PIXELS FROM LEFT -> RIGHT, BOTTOM -> TOP
    //BMP CAN HAVE MAX 24 BITS / PIXEL
    //SO IN RETURN UIN32_T ARRAY, HIGHEST BYTE ALWAYS 0
    //RETURN PIXEL DATA IN SPECIFIED COLOR RGB ORDER

    FILE* fp;
    bitmapfileheader_t bmp_header;
    bitmapinfoheader_t bmp_info_header;
    pixel_data_t bmp_pixel_data;
    uint32_t x;
    uint32_t y;
    uint32_t color;

    //OPEN FILE
    if ((fp = fopen(file_name, "rb")) == NULL)
    {
        return false;
    }
    
    //READ FILE BMP HEADER
    if (fread(&bmp_header, sizeof(bitmapfileheader_t), 1, fp) < 1)
    {
        fclose(fp);
        return false;
    }

    //MAKE SURE ITS BITMAP FILE
    if(bmp_header.bfType != BF_TYPE)
    {
        //NOT A BMP FILE
        fclose(fp);
        return false;
    }

    //READ FILE BMP HEADER
    if (fread(&bmp_info_header, sizeof(bitmapinfoheader_t), 1, fp) < 1)
    {
        fclose(fp);
        return false;
    }

    //READ IMAGE DATA
    //BMP STORES PIXELS FROM LEFT -> RIGHT, BOTTOM -> TOP
    for(y = 0; y < bmp_info_header.biHeight; y++)
    {
        for(x = 0; x < bmp_info_header.biWidth; x++)
        {
            fread(&bmp_pixel_data, sizeof(pixel_data_t), 1, fp);

            //CONVERT TO APPROPRIATE FORMAT
            switch(format)
            {
                case PIXEL_FORMAT_RGB:
                    color = bmp_pixel_data.r;
                    color = color << 8 | bmp_pixel_data.g;
                    color = color << 8 | bmp_pixel_data.b;
                    break;
                
                case PIXEL_FORMAT_BRG:
                    color = bmp_pixel_data.b;
                    color = color << 8 | bmp_pixel_data.r;
                    color = color << 8 | bmp_pixel_data.g;
                    break;
                
                case PIXEL_FORMAT_GRB:
                    color = bmp_pixel_data.g;
                    color = color << 8 | bmp_pixel_data.r;
                    color = color << 8 | bmp_pixel_data.b;
                    break;
                
                default:
                    color = 0;
            }
            //printf("color = %08x\n", color);
            image_data[(y * bmp_info_header.biWidth) + x] = color;
        }
    }

    fclose(fp);
    return true;
}