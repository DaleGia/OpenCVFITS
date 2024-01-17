#ifndef OPENCV2FITS_H_
#define OPENCV2FITS_H_
/**
 * @file OpenCV2FITS.hpp
 * Copyright (c) 2023 Dale Giancono All rights reserved.
 * 
 * @brief
 * TODO add me
 */

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
extern "C" 
{
    #include <fitsio.h>

}
class OpenCV2FITS
{
    public:
        static bool saveMat2FITS(
            cv::Mat &image, 
            std::string filepath)
        {
            fitsfile *fptr;
            int status = 0;
            int ret;
            int BITPIX;


            ret = fits_create_file(&fptr, filepath.c_str(), &status);
            if(ret) 
            {
                // Handle error
                std::cerr << "FITS create file error: " << filepath.c_str() << ":" << status << std::endl; 
                return false;
            }

            switch(image.type())
            {
                case CV_8U:
                {
                    BITPIX = BYTE_IMG;
                    break;
                }
                case CV_16U:
                {
                    BITPIX = SHORT_IMG;
                    break;
                }
                default:
                {
                    std::cerr << "CV::Mat image type not currently supported: " <<  image.type() << std::endl;
                    return false;
                }

            }

            long shape[2] = {image.cols, image.rows};
            ret = fits_create_img(fptr, BITPIX, 2, shape, &status);
            if(ret) 
            {
                // Handle error
                std::cerr << "FITS create image error: " << status << std::endl; 
                return false;
            }

            if(BYTE_IMG == BITPIX)
            {
                ret = fits_write_img(
                    fptr, 
                    TBYTE, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else
            {
                ret = fits_write_img(
                    fptr, 
                    TSHORT, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            if(ret) 
            {
                // Handle error
                std::cerr << "FITS write image error: " << status << std::endl; 
                return false;
            }


            ret = fits_close_file(fptr, &status);
            if(ret) 
            {
                // Handle error
                std::cerr << "FITS close file error: " << status << std::endl; 
                return false;
            }

            return true;
        }

        static bool saveMat2FITS(
            cv::Mat &image, 
            std::string filepath,
            std::vector<std::pair<std::string, double>> &keys)
        {
            fitsfile *fptr;
            int status = 0;
            int ret;
            int BITPIX;


            ret = fits_create_file(&fptr, filepath.c_str(), &status);
            if(ret) 
            {
                // Handle error
                std::cerr << "FITS create file error: " << filepath.c_str() << ":" << status << std::endl; 
                return false;
            }

            switch(image.type())
            {
                case CV_8U:
                {
                    BITPIX = BYTE_IMG;
                    break;
                }
                case CV_16U:
                {
                    BITPIX = SHORT_IMG;
                    break;
                }
                default:
                {
                    std::cerr << "CV::Mat image type not currently supported: " <<  image.type() << std::endl;
                    return false;
                }

            }

            long shape[2] = {image.cols, image.rows};
            ret = fits_create_img(fptr, BITPIX, 2, shape, &status);
            if(ret) 
            {
                // Handle error
                std::cerr << "FITS create image error: " << status << std::endl; 
                fits_close_file(fptr, &status);

                return false;
            }

            if(BYTE_IMG == BITPIX)
            {
                ret = fits_write_img(
                    fptr, 
                    TBYTE, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else
            {
                ret = fits_write_img(
                    fptr, 
                    TSHORT, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            if(ret) 
            {
                // Handle error
                std::cerr << "FITS write image error: " << status << std::endl; 
                fits_close_file(fptr, &status);
                return false;
            }

            for(auto key : keys)
            {
                ret = fits_write_key(
                    fptr, 
                    TDOUBLE, 
                    key.first.c_str(), 
                    &key.second,
                    "", 
                    &status);
                if(ret) 
                {
                    // Handle error
                    std::cerr << "FITS write key error: " << status << std::endl; 
                    fits_close_file(fptr, &status);
                    return false;
                }
            }

            ret = fits_close_file(fptr, &status);
            if(ret) 
            {
                // Handle error
                std::cerr << "FITS close file error: " << status << std::endl; 
                return false;
            }

            return true;
        }
};

#endif 