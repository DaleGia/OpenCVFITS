#include "../OpenCVFITS.hpp"

void createTestPattern(cv::Mat &buffer, int height, int width);

int main()
{
    /* Variables for writing the fits image */
    OpenCVFITS fits;
    cv::Mat image1;
    cv::Mat image2;
    cv::Mat image3;
    cv::Mat readImage;
    /* Create a test pattern to save */
    createTestPattern(image1, 480, 640);
    /* Create some duplicates so we can write multiple images to a fits file */
    image2 = image1.clone();
    image3 = image2.clone();

    /* Variables for reading the fits image to and the keys and values as well */
    FITSOpenCV fitsread;
    std::string keyString;
    std::string valueString;
 
    /* Create the FITS file*/
    if(false == fits.createFITS("testfits.fit", true))
    {
        std::cerr << "Could not create fits file testfits1.fit" << std::endl;
        return 1;
    }

    /* Add an image to it */
    if(false == fits.addMat2FITS(image1))
    {
        std::cerr << "Could not add image to fits file" << std::endl;
        return 1;
    }
    
    /* Add some keys to it */
    if(false == fits.addKey2FITS("TestDouble", 14.3))
    {
        std::cerr << "Could not double key to image" << std::endl;
        return 1;
    }

    if(false == fits.addKey2FITS("TestString", "string"))
    {
        std::cerr << "Could not string key to image" << std::endl;
        return 1;
    }
    
    if(false == fits.addKey2FITS("testunsigned", (uint64_t)8461292))
    {
        std::cerr << "Could not add unsigned key to image" << std::endl;
        return 1;
    }

    if(false == fits.addKey2FITS("testsigned", (int64_t)-8461292))
    {
        std::cerr << "Could not add signed key to image" << std::endl;
        return 1;
    }

    /* Add another images and some more keys */
    if(false == fits.addMat2FITS(image2))
    {
        std::cerr << "Could not add image to fits file" << std::endl;
        return 1;
    }
    
    if(false == fits.addKey2FITS("TestDouble", 14.3))
    {
        std::cerr << "Could not double key to image" << std::endl;
        return 1;
    }

    if(false == fits.addKey2FITS("TestString", "string"))
    {
        std::cerr << "Could not string key to image" << std::endl;
        return 1;
    }
    
    if(false == fits.addKey2FITS("testunsigned", (uint64_t)8461292))
    {
        std::cerr << "Could not add unsigned key to image" << std::endl;
        return 1;
    }

    if(false == fits.addKey2FITS("testsigned", (int64_t)-8461292))
    {
        std::cerr << "Could not add signed key to image" << std::endl;
        return 1;
    }

    /* Add another images and some more keys */
    if(false == fits.addMat2FITS(image3))
    {
        std::cerr << "Could not add image to fits file" << std::endl;
        return 1;
    }
    
    if(false == fits.addKey2FITS("TestDouble", 14.3))
    {
        std::cerr << "Could not double key to image" << std::endl;
        return 1;
    }

    if(false == fits.addKey2FITS("TestString", "string"))
    {
        std::cerr << "Could not string key to image" << std::endl;
        return 1;
    }
    
    if(false == fits.addKey2FITS("testunsigned", (uint64_t)8461292))
    {
        std::cerr << "Could not add unsigned key to image" << std::endl;
        return 1;
    }

    if(false == fits.addKey2FITS("testsigned", (int64_t)-8461292))
    {
        std::cerr << "Could not add signed key to image" << std::endl;
        return 1;
    }

    /* Close the file */
    if(false == fits.closeFITS())
    {
        std::cerr << "Could not close" << std::endl;
        return 1;
    }

    /* Open the FITS file we just created */
    if(false == fitsread.openFITS("testfits.fit"))
    {
        std::cerr << "Could not open fits to read" << std::endl;
        return 1;
    }

    int imageCount = 0;
    /* Read images until the end */
    while(true == fitsread.getNextImage(readImage))
    {
        imageCount++;
        /* Image got successfully */
        /* Read some keys */
        while(true == fitsread.getNextKey(keyString, valueString))
        {
            std::cout << keyString << ": " << valueString << std::endl;
        }
        
        /* Wait for any key before reading the next image*/
        cv::imshow(std::to_string(imageCount), readImage);
    }
    
    cv::waitKey(0);

    /* Close the file */
    if(false == fitsread.closeFITS())
    {
        std::cerr << "Could not close" << std::endl;
        return 1;
    }


    return 0;
}

void createTestPattern(cv::Mat &buffer, int height, int width)
{
    buffer = cv::Mat::zeros(cv::Size(width, height), CV_8U);

    // Fill with gradient (white to black)
    for (int y = 0; y < height; y++) 
    {
        uchar intensity = 255 - y * 255 / (height - 1);
        for (int x = 0; x < width; x++) 
        {
            buffer.at<uchar>(y, x) = intensity;
        }
    }
}