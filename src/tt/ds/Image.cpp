/* Author: Martin Wojtczyk <wojtczyk@in.tum.de> */

#include <iostream>

#include <assert.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include "Image.h"

namespace tt
{

namespace ds
{
	
Image::Image() :
	allocatedBytes(0),
	imageBuffer(NULL),
	allocatedWidth(0),
	allocatedHeight(0),
	width(0),
	height(0),
	channels(RGB),
	bitsPerChannel(BPC8),
	lineAlignment(A4),
	opencvHeader(NULL)
{
	//updateOpencvHeader(); // Create OpenCV Header
}

Image::Image(int initWidth, int initHeight, Channels initChannels) :
	width(initWidth),
	height(initHeight),
	channels(initChannels),
	bitsPerChannel(BPC8),
	lineAlignment(A4),
	opencvHeader(NULL)
{
	// get an imageBuffer with the appropriate line Alignment
	int remainder = (this->lineAlignment - ((this->width * this->channels) % this->lineAlignment)) % this->lineAlignment;
	this->allocatedWidth = this->width * this->channels + remainder; 
	this->allocatedHeight = this->height;

	this->allocatedBytes = this->allocatedWidth * this->allocatedHeight;
	imageBuffer = new unsigned char[this->allocatedBytes]; 

	updateOpencvHeader(); // Create OpenCV Header
}

Image::Image(const IplImage* image)
{
	// OpenCV image must be greyscale or RGB
	assert((image->nChannels == 1) || (image->nChannels == 3) || (image->nChannels == 4));
	// Pixel depth must be 8 bits per pixel 
	assert(image->depth == IPL_DEPTH_8U);
	
	this->width = image->width;
	this->height = image->height;
	this->channels = (Channels) image->nChannels;
	this->bitsPerChannel = BPC8; // only 8 bits per Pixel supported and checked by assert
	this->lineAlignment = (LineAlignment) image->align;
	this->allocatedWidth = image->widthStep;
	this->allocatedHeight = this->height;
	this->opencvHeader = NULL;

	this->allocatedBytes = this->allocatedWidth * this->allocatedHeight;
	imageBuffer = new unsigned char[this->allocatedBytes];
	memcpy(imageBuffer, image->imageData, this->allocatedBytes);

	updateOpencvHeader(); // Create OpenCV Header
}

Image::Image(std::string filename)
{
	IplImage *image = cvLoadImage(filename.c_str());
	assert(image);
	this->width = image->width;
	this->height = image->height;
	this->channels = (Channels) image->nChannels;
	this->bitsPerChannel = BPC8; // only 8 bits per Pixel supported and checked by assert
	this->lineAlignment = (LineAlignment) image->align;
	this->allocatedWidth = image->widthStep;
	this->allocatedHeight = this->height;
	this->opencvHeader = NULL;

	this->allocatedBytes = this->allocatedWidth * this->allocatedHeight;
	imageBuffer = new unsigned char[this->allocatedBytes];
	memcpy(imageBuffer, image->imageData, this->allocatedBytes); 

	updateOpencvHeader(); // Create OpenCV Header
} 	

Image::~Image()
{
	if (allocatedBytes > 0)
	{
		delete[] imageBuffer;
	};
	
	if (opencvHeader != NULL)
	{
		cvReleaseImageHeader(&opencvHeader);
	};
}

Image::Channels Image::getChannels() const
{
	return this->channels;
}
	 
Image::BitsPerChannel Image::getBitsPerChannel() const
{
	return this->bitsPerChannel;
}

Image* Image::clone() const
{
	Image* tmp = new Image(this->getWidth(), this->getHeight(), this->getChannels());
	*tmp=*this;
	return (tmp);	
}

unsigned char* Image::getImageBuffer() const
{
	return imageBuffer;
}

int Image::getWidth() const
{
	return this->width;
}

int Image::getHeight() const
{
	return this->height;
}

int Image::getAllocatedWidth() const
{
	return this->allocatedWidth;
}

int Image::getAllocatedHeight() const
{
	return this->allocatedHeight;
}

int Image::getAllocatedBytes() const
{
	return this->allocatedBytes;
}

// TODO test for memory correctness, should be ok, according to cxarray.cpp
IplImage* Image::getIplImage()
{
	return opencvHeader;
}

void Image::resizeMemory(const int newWidth, const int newHeight)
{
	// release memory if allocated before
	if (allocatedBytes > 0)
	{
		delete[] imageBuffer;
	};

	this->width = newWidth;
	this->height = newHeight;

	// get an imageBuffer with the appropriate line Alignment
	int remainder = (this->lineAlignment - ((this->width * this->channels) % this->lineAlignment)) % this->lineAlignment;
	this->allocatedWidth = this->width * this->channels + remainder; 
	this->allocatedHeight = this->height;

	this->allocatedBytes = this->allocatedWidth * this->allocatedHeight;
	imageBuffer = new unsigned char[allocatedBytes]; 

	updateOpencvHeader(); // Update OpenCV Header
}

//inline
unsigned char& Image::operator() (unsigned x, unsigned y, unsigned channel)
{
	// TODO check bounds, if wanted?
	return imageBuffer[(y * this->allocatedWidth) + (x * this->channels) + channel];
}

//inline
unsigned char Image::operator() (unsigned x, unsigned y, unsigned channel) const
{
	// TODO check bounds, if wanted?
	return imageBuffer[(y * this->allocatedWidth) + (x * this->channels) + channel];
}

void Image::operator = (const Image &img)
{
	lineAlignment = img.lineAlignment;
	if(getWidth() != img.getWidth() || getHeight() != img.getHeight())
	{
		resizeMemory(img.getWidth(), img.getHeight());
	}
	if(this->getBitsPerChannel() != img.getBitsPerChannel())
	{
		this->bitsPerChannel = img.getBitsPerChannel();
	}
	if(this->getChannels() != img.getChannels())
	{
		this->channels = img.getChannels();
	}
	int remainder = (lineAlignment - ((img.getWidth() * img.getChannels()) % lineAlignment)) % lineAlignment;
	this->allocatedWidth = img.getWidth() * img.getChannels() + remainder; 
	this->allocatedHeight = img.getHeight();
	this->allocatedBytes = this->allocatedWidth * this->allocatedHeight;

	// IMPORTANT!
	if (allocatedBytes > 0)
		delete [] imageBuffer;

	imageBuffer = new unsigned char[this->allocatedBytes]; 
	this->channels = img.getChannels();
	memcpy(this->imageBuffer, img.getImageBuffer(), this->allocatedBytes);

	updateOpencvHeader(); // Update OpenCV Header
}

void Image::updateOpencvHeader()
{
	// Allocate OpenCV Header if not done before, otherwise just initialise
	if (opencvHeader == NULL)
	{
		this->opencvHeader = cvCreateImageHeader(cvSize(this->width, this->height), 
			this->bitsPerChannel, this->channels);
	}
	else
	{
		cvInitImageHeader(this->opencvHeader, cvSize(this->width, this->height),
			this->bitsPerChannel, this->channels);
	};
	
	// Set the internal data pointer to the imageBuffer
	cvSetData(opencvHeader, this->imageBuffer, this->allocatedWidth);
}

} // namespace ds

} // namespace tt
