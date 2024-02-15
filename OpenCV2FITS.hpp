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
#include <mutex>

extern "C" 
{
    #include <fitsio.h>

}
class OpenCV2FITS
{
    public:
        bool openFITS(std::string filepath)
        {
            int status = 0;
            fitsfile *fptr;
            int ret;

            std::lock_guard guard(this->lock);
            if(this->open)
            {
                ret = fits_close_file(this->file, &status);
                if(ret) 
                {
                    // Handle error
                }
                else
                {
                    this->open = false;
                }
            }

            status = 0;
            ret = fits_create_file(&this->file, filepath.c_str(), &status);
            if(ret) 
            {
                status = 0;
                ret = fits_open_file(&this->file, filepath.c_str(), 1, &status);
                if(ret) 
                {
                    // Handle error
                    std::cerr << "FITS create/open file error: " << filepath.c_str() << ":" << status << std::endl; 
                    this->open = false;
                    return false;
                }
            }
            this->open = true;
            
            return true;
        }

        bool addMat2FITS(cv::Mat &image, std::vector<std::pair<std::string, double>> &keys)
        {
            int status = 0;
            int ret;
            int BITPIX;

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
                case CV_32F:
                {
                    BITPIX = FLOAT_IMG;
                    break;
                }
                case CV_64F:
                {
                    BITPIX = DOUBLE_IMG;
                    break;
                }
                default:
                {
                    std::cerr << "CV::Mat image type not currently supported: " <<  image.type() << std::endl;
                    return false;
                }
            }

            long shape[2] = {image.cols, image.rows};

            std::lock_guard guard(this->lock);
            if(false == this->open)
            {
                std::cerr << "fits file is not opened yet... " << std::endl;
                return false;
            }
            ret = fits_create_img(file, BITPIX, 2, shape, &status);
            if(ret) 
            {
                // Handle error
                std::cerr << "FITS create image error: " << status << std::endl; 
                return false;
            }

            if(BYTE_IMG == BITPIX)
            {
                ret = fits_write_img(
                    file, 
                    TBYTE, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else if(SHORT_IMG)
            {
                ret = fits_write_img(
                    file, 
                    TSHORT, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else if(FLOAT_IMG)
            {
                ret = fits_write_img(
                    file, 
                    TFLOAT, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else if(DOUBLE_IMG)
            {
                ret = fits_write_img(
                    file, 
                    TDOUBLE, 
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

            for(auto key : keys)
            {
                ret = fits_write_key(
                    this->file, 
                    TDOUBLE, 
                    key.first.c_str(), 
                    &key.second,
                    "", 
                    &status);
                if(ret) 
                {
                    // Handle error
                    std::cerr << "FITS write key error: " << status << std::endl; 
                    fits_close_file(this->file, &status);
                    return false;
                }
            }

            return true;
        }

        bool addMat2FITS(cv::Mat &image)
        {
            int status = 0;
            int ret;
            int BITPIX;

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
                case CV_32F:
                {
                    BITPIX = FLOAT_IMG;
                    break;
                }
                case CV_64F:
                {
                    BITPIX = DOUBLE_IMG;
                    break;
                }
                default:
                {
                    std::cerr << "CV::Mat image type not currently supported: " <<  image.type() << std::endl;
                    return false;
                }
            }

            long shape[2] = {image.cols, image.rows};

            std::lock_guard guard(this->lock);
            if(false == this->open)
            {
                std::cerr << "fits file is not opened yet... " << std::endl;
                return false;
            }
            ret = fits_create_img(file, BITPIX, 2, shape, &status);
            if(ret) 
            {
                // Handle error
                std::cerr << "FITS create image error: " << status << std::endl; 
                return false;
            }

            if(BYTE_IMG == BITPIX)
            {
                ret = fits_write_img(
                    file, 
                    TBYTE, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else if(SHORT_IMG)
            {
                ret = fits_write_img(
                    file, 
                    TSHORT, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else if(FLOAT_IMG)
            {
                ret = fits_write_img(
                    file, 
                    TFLOAT, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else if(DOUBLE_IMG)
            {
                ret = fits_write_img(
                    file, 
                    TDOUBLE, 
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

            return true;
        }

        bool closeFITS(void)
        {
            int status = 0;
            int ret;

            std::lock_guard guard(this->lock);
            if(false == this->open)
            {
                return false;
            }
            
            ret = fits_close_file(this->file, &status);
            if(ret) 
            {
                // Handle error
                std::cerr << "FITS close file error: " << status << std::endl; 
                return false;
            }
            this->open = false;
            return true;
        }

        bool saveMat2FITS(
            cv::Mat &image, 
            std::string filepath)
        {
            fitsfile *fptr;
            int status = 0;
            int ret;
            int BITPIX;

            std::lock_guard guard(this->lock);
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
                case CV_32F:
                {
                    BITPIX = FLOAT_IMG;
                    break;
                }
                case CV_64F:
                {
                    BITPIX = DOUBLE_IMG;
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
                    file, 
                    TBYTE, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else if(SHORT_IMG)
            {
                ret = fits_write_img(
                    file, 
                    TSHORT, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else if(FLOAT_IMG)
            {
                ret = fits_write_img(
                    file, 
                    TFLOAT, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else if(DOUBLE_IMG)
            {
                ret = fits_write_img(
                    file, 
                    TDOUBLE, 
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

        bool saveMat2FITS(
            cv::Mat &image, 
            std::string filepath,
            std::vector<std::pair<std::string, double>> &keys)
        {
            fitsfile *fptr;
            int status = 0;
            int ret;
            int BITPIX;

            std::lock_guard guard(this->lock);
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
                case CV_32F:
                {
                    BITPIX = FLOAT_IMG;
                    break;
                }
                case CV_64F:
                {
                    BITPIX = DOUBLE_IMG;
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
                    file, 
                    TBYTE, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else if(SHORT_IMG)
            {
                ret = fits_write_img(
                    file, 
                    TSHORT, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else if(FLOAT_IMG)
            {
                ret = fits_write_img(
                    file, 
                    TFLOAT, 
                    1, 
                    image.rows * image.cols, 
                    image.data, 
                    &status); 
            }
            else if(DOUBLE_IMG)
            {
                ret = fits_write_img(
                    file, 
                    TDOUBLE, 
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

        bool isOpen()
        {
            return open;
        }
    private:
        std::mutex lock;
        fitsfile *file;
        bool open = false;
};

#endif 