#ifndef OPENCVFITS_H_
#define OPENCVFITS_H_
/**
 * @file OpenCVFITS.hpp
 * Copyright (c) 2023 Dale Giancono All rights reserved.
 *
 * @brief
 * TODO add me
 */

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fitsio.h>

class OpenCVFITS
{
public:
    /**
     * @brief Opens a FITS file to write or read to. Does not overwrite
     * existing FITS files.
     *
     * @param filepath path of FITS file to create
     * @return true success
     * @return false failed
     */
    bool createFITS(std::string filepath, bool overwrite = false)
    {
        int status = 0;
        fitsfile *fptr;
        int ret = 0;

        /* Check if a FITS file is already open */
        if (this->open)
        {
            /* If a fits file is already open, close it */
            ret = fits_close_file(this->file, &status);
            if (0 != ret)
            {
                std::cerr << "OpenCVFITS: FITS file already open but unable to close it: " << status << std::endl;
                return false;
            }
            else
            {
                this->open = false;
            }
        }

        if (true == overwrite)
        {
            filepath = "!" + filepath;
        }

        /* Attempt to create a FITS file */
        status = 0;
        ret = fits_create_file(&this->file, filepath.c_str(), &status);
        if (0 != ret)
        {
            this->open = false;
            return false;
        }

        /* Successfully opened the FITS file */
        this->open = true;
        return true;
    }

    /**
     * @brief Add a CV::Mat object to FITS as image
     *
     * @param image CV::Mat to add to FITS file
     * @return true success
     * @return false failed
     */
    bool addMat2FITS(cv::Mat &image)
    {
        int status = 0;
        int ret = 0;
        int BITPIX;

        /* Only support unsigned monochrome image types for now */
        switch (image.type())
        {
        case CV_8U:
        {
            BITPIX = BYTE_IMG;
            break;
        }
        case CV_16U:
        {
            BITPIX = USHORT_IMG;
            break;
        }
        default:
        {
            std::cerr << "OpenCVFITS: CV::Mat image type not currently supported: " << image.type() << std::endl;
            return false;
        }
        }

        long shape[2] = {image.cols, image.rows};

        if (false == this->open)
        {
            std::cerr << "OpenCVFITS: FITS file is not opened yet... " << std::endl;
            return false;
        }
        ret = fits_create_img(this->file, BITPIX, 2, shape, &status);
        if (0 != ret)
        {
            std::cerr << "OpenCVFITS: FITS create image error: " << status << std::endl;
            return false;
        }

        if (BYTE_IMG == BITPIX)
        {
            ret = fits_write_img(
                this->file,
                TBYTE,
                1,
                image.rows * image.cols,
                image.data,
                &status);
        }
        else if (USHORT_IMG)
        {
            ret = fits_write_img(
                this->file,
                TUSHORT,
                1,
                image.rows * image.cols,
                image.data,
                &status);
        }

        if (0 != ret)
        {
            std::cerr << "OpenCVFITS: FITS write image error: " << status << std::endl;
            return false;
        }

        return true;
    }

    /**
     * @brief Add a key value to the last added image in FITS file.
     * Only call this after using addMat2FITS()
     *
     * @param key
     * @param value
     * @return true success
     * @return false failed
     */
    bool addKey2FITS(std::string key, double value)
    {
        int ret = 0;
        int status = 0;

        if (false == this->open)
        {
            std::cerr << "OpenCVFITS: FITS file is not opened yet... " << std::endl;
            return false;
        }

        ret = fits_write_key(
            this->file,
            TDOUBLE,
            key.c_str(),
            &value,
            "",
            &status);

        if (0 != ret)
        {
            std::cerr << "OpenCVFITS: FITS write key error: " << status << std::endl;
            fits_close_file(this->file, &status);
            return false;
        }

        return true;
    }

    /**
     * @brief Add a key value to the last added image in FITS file.
     * Only call this after using addMat2FITS()
     *
     * @param key
     * @param value
     * @return true success
     * @return false failed
     */
    bool addKey2FITS(std::string key, std::string value)
    {
        int ret = 0;
        int status = 0;

        /* Check FITS file is actually open */
        if (false == this->open)
        {
            std::cerr << "OpenCVFITS: FITS file is not opened yet... " << std::endl;
            return false;
        }

        ret = fits_write_key(
            this->file,
            TSTRING,
            key.c_str(),
            (void *)value.c_str(),
            "",
            &status);

        if (0 != ret)
        {
            std::cerr << "OpenCVFITS: FITS write key error: " << status << std::endl;
            fits_close_file(this->file, &status);
            return false;
        }

        return true;
    }

    /**
     * @brief Add a key value to the last added image in FITS file.
     * Only call this after using addMat2FITS()
     *
     * @param key
     * @param value
     * @return true success
     * @return false failed
     */
    bool addKey2FITS(std::string key, uint64_t value)
    {
        int ret = 0;
        int status = 0;

        /* Check FITS file is actually open */
        if (false == this->open)
        {
            std::cerr << "OpenCVFITS: FITS file is not opened yet... " << std::endl;
            return false;
        }

        ret = fits_write_key(
            this->file,
            TULONG,
            key.c_str(),
            &value,
            "",
            &status);

        if (0 != ret)
        {
            std::cerr << "OpenCVFITS: FITS write key error: " << status << std::endl;
            fits_close_file(this->file, &status);
            return false;
        }

        return true;
    }

    /**
     * @brief Add a key value to the last added image in FITS file.
     * Only call this after using addMat2FITS()
     *
     * @param key
     * @param value
     * @return true success
     * @return false failed
     */
    bool addKey2FITS(std::string key, int64_t value)
    {
        int ret = 0;
        int status = 0;

        /* Check FITS file is actually open */
        if (false == this->open)
        {
            std::cerr << "OpenCVFITS: FITS file is not opened yet... " << std::endl;
            return false;
        }

        ret = fits_write_key(
            this->file,
            TLONG,
            key.c_str(),
            &value,
            "",
            &status);

        if (0 != ret)
        {
            std::cerr << "OpenCVFITS: FITS write key error: " << status << std::endl;
            fits_close_file(this->file, &status);
            return false;
        }

        return true;
    }

    /**
     * @brief Closes the FITS file
     *
     * @return true success
     * @return false failed
     */
    bool closeFITS(void)
    {
        int status = 0;
        int ret = 0;

        /* Check the file */
        if (false == this->open)
        {
            return false;
        }

        ret = fits_close_file(this->file, &status);
        if (0 != ret)
        {
            std::cerr << "OpenCVFITS: FITS close file error: " << status << std::endl;
            return false;
        }
        this->open = false;
        return true;
    }

    /**
     * @brief Returns whether FITS file is open
     *
     * @return true
     * @return false
     */
    bool isOpen()
    {
        return open;
    }

private:
    /**
     * @brief fitsio file object
     *
     */
    fitsfile *file;

    /**
     * @brief fitsio file open status
     *
     */
    bool open = false;
};

class FITSOpenCV
{
public:
    bool openFITS(std::string filepath)
    {
        int status = 0;
        fitsfile *fptr;
        int ret;
        int hdutype;

        /** Close any file that might already be open **/
        if (this->open)
        {
            ret = fits_close_file(this->file, &status);
            if (ret)
            {
                // Handle error
            }
            else
            {
                this->open = false;
            }
        }

        /** Open the file **/
        status = 0;
        ret = fits_open_file(&this->file, filepath.c_str(), READONLY, &status);
        if (ret)
        {
            // Handle error
            std::cerr << "FITS create/open file error: " << filepath.c_str() << ":" << status << std::endl;
            this->open = false;
            return false;
        }

        this->open = true;

        this->hdu = 1;

        return true;
    }

    bool getNextImage(cv::Mat &image)
    {
        int status = 0;
        int ret;
        int bitpix;
        int datatype;
        int openCVType;
        int hdutype;

        long naxes[2];
        /** Check if file is open **/
        if (false == this->open)
        {
            std::cerr << "FITSOpenCV: file is not open to get image " << this->hdu << ":" << status << std::endl;
            return false;
        }

        // Move to the first HDU (header data unit)
        ret = fits_movabs_hdu(this->file, this->hdu, &hdutype, &status);

        if (END_OF_FILE == ret)
        {
            return false;
        }
        else if (0 != ret)
        {
            // Handle error
            std::cerr << "FITSOpenCV: error moving to image " << this->hdu << ":" << status << std::endl;
            return false;
        }

        // Check if it's an image HDU
        if (fits_get_hdu_type(this->file, &hdutype, &status))
        {
            std::cerr << "FITSOpenCV: error getting HDU type: " << status << std::endl;
            return false;
        }
        if (hdutype != IMAGE_HDU)
        {
            // Skip non-image HDUs
            this->hdu++;
            std::cerr << "FITSOpenCV: error not image... skipping: " << status << std::endl;
            return false;
        }

        // Read the image dimensions
        ret = fits_get_img_size(this->file, 2, naxes, &status);
        if (ret)
        {
            std::cerr << "FITSOpenCV: error getting image dimensions: " << status << std::endl;
            return false;
        }

        // Get the image data type from the BITPIX keyword
        ret = fits_read_key(this->file, TINT, "BITPIX", &bitpix, NULL, &status);
        if (ret)
        {
            std::cerr << "FITSOpenCV: error reading pixel type: " << status << std::endl;
            return false;
        }

        size_t data_size;

        // Define the C data type based on BITPIX
        // (adjust as needed for different data types)
        switch (bitpix)
        {
        case BYTE_IMG:
            datatype = TBYTE;
            data_size = naxes[0] * naxes[1] * 1;
            openCVType = CV_8U;
            break;
        case SHORT_IMG:
            datatype = TSHORT;
            data_size = naxes[0] * naxes[1] * 2;
            openCVType = CV_16U;
            break;
        default:
            std::cerr << "FITSOpenCV: Unsupported data type in BITPIX: " << bitpix << std::endl;
            return false;
        }

        // Allocate memory for the image data
        void *imageData;
        imageData = malloc(data_size);

        // Read the image data
        ret =
            fits_read_img(this->file, datatype, 1, naxes[0] * naxes[1], NULL, imageData, NULL, &status);
        if (ret)
        {
            std::cerr << "FITSOpenCV: error reading image data: " << status << std::endl;
            return false;
        }

        this->hdu++;

        cv::Mat mat(naxes[1], naxes[0], openCVType, imageData);
        image = mat.clone();
        free(imageData);
        this->keyn = 1;

        return true;
    }

    bool getNextKey(std::string &key, std::string &value)
    {

        // Print all the header keys and values
        int nkeys;
        int ret = 0;
        int status = 0;

        ret = fits_get_hdrspace(this->file, &nkeys, NULL, &status);
        char keyname[FLEN_KEYWORD];
        char values[FLEN_VALUE];
        ret = fits_read_keyn(this->file, this->keyn, keyname, values, NULL, &status);
        if (203 == ret)
        {
            /* Reached the end of the keys */
            return false;
        }
        else if (0 != ret)
        {
            std::cerr << "FITSOpenCV: error reading key data: " << status << std::endl;
            return false;
        }
        else
        {
            this->keyn++;
        }

        key = keyname;
        value = values;

        return true;
    }

    bool closeFITS(void)
    {
        int status = 0;
        int ret;

        if (false == this->open)
        {
            return false;
        }

        ret = fits_close_file(this->file, &status);

        if (ret)
        {
            std::cerr << "FITSOpenCV: FITS close file error: " << status << std::endl;
            return false;
        }

        this->open = false;
        return true;
    }

    bool isOpen()
    {
        return this->open;
    }

private:
    fitsfile *file;
    bool open = false;
    int hdu = 0;
    int keyn = 1;
};

#endif