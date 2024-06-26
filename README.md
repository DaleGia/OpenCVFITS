# OpenCVFITS
OpenCVFITS is a single header file C++ library designed to write and read FITS
files with cv::Mat objects. It has basic functionality to write keys to images, and read image keys. 
It can write multiple images to a single FITS file, and read multiple images from a single FITS file.

## Installation
Install the following packages:
- libopencv-dev
- libcfitsio-dev
  
Then place the header file in your project. An example on how to build a project using OpenCVFITS with cmake is available. This 
library has been tested on Ubuntu 22.04 LTS, but should work on other distributions.

## Building and running the example
```
mkdir build
cd build
cmake ..
cmake --build .
./example
```

The example creates a FITS file with three images inside. It opens each image, 
displays it, and prints the keys on the console. Do not run the example from a VSCode terminal, the images will not display.

# Usage
## Create, add images, add keys, and close
```
/* Assume this is actually full of data */
cv::Mat image1;
cv::Mat image2;
cv::Mat image3;
/* Use false for second argument if you do not wish to overwrite existing files */
fits.createFITS("testfits.fit", true);
fits.addMat2FITS(image1);
fits.addKey2FITS("TestDouble", 14.3);
fits.addKey2FITS("testsigned", (int64_t)-8461292);
fits.addMat2FITS(image2);
fits.addKey2FITS("TestDouble", 7.3);
fits.addKey2FITS("testsigned", (int64_t)-234292);
fits.addMat2FITS(image3);
fits.addKey2FITS("TestDouble", 3.3);
fits.addKey2FITS("testsigned", (int64_t)5231292);
fits.closeFITS();
```

## Open, get images and keys, and close
```
cv::Mat image;
std::string key;
std::string value;
fits.openFITS("testfits.fit");
while(true == fits.getNextImage(image))
{
    /* Image got successfully */
    /* Read some keys */
    while(true == fits.getNextKey(key, value))
    {
        std::cout << key << ": " << value << std::endl;
    }
}
fits.closeFITS();
```

## Limitations
At the moment it only works with CV_8U and CV_16U data. Help me add other types!
