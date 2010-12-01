#ifndef TT_DS_IMAGE_H
#define TT_DS_IMAGE_H

#include <string>
#include <cv.h>

namespace tt
{

/**
 * @brief Namespace for all tt data structures.
 */
namespace ds
{

/**
 * @class Image Image.h tt/ds/Image.h
 * @brief Class for Image Storage
 * @author Martin Wojtczyk <wojtczyk@in.tum.de>
 * 
 * Image represents an Image and provides converter functions to the OpenCV
 * IplImage data type.
 */
class Image
{
public:
	enum Channels
	{
		GREYSCALE = 1,
		RGB = 3,
		RGBA = 4
	};
	
	enum BitsPerChannel
	{
		BPC8 = 8
	};

	enum LineAlignment
	{
		A4 = 4
	};
	
	
	/**
	 * @brief Default constructor
	 * 
	 * Creates and empty image without allocating a frame buffer.
	 */
	Image();

	/**
	 * @brief Create an Image with the given dimensions and number of channels
	 * @param initWidth Width of the new image
	 * @param initHeight Height of the new image
	 * @param initChannels Number of channels (supported: GREYSCALE = 1, RGB = 3; default = RGB) 
	 */ 
	Image(int initWidth, int initHeight, Channels initChannels = RGB);

	/**
	 * @brief Create an Image based on an OpenCV image
	 * @param image The OpenCV Source image
	 */ 
	Image(const IplImage* image);
	
	Image(std::string filename);

	/**
	 * @brief Destroy an Image object and release the image buffer.
	 */
	virtual ~Image();

	/**
	 * @brief Return the number of channels of this image.
	 */
	Channels getChannels() const;
	 
	 /**
	  * @brief Return the number of bits per channel.
	  */ 
	BitsPerChannel getBitsPerChannel() const;

	/**
	 * @brief Return a pointer to the internal image buffer
	 */
	unsigned char* getImageBuffer() const;

	/**
	 * @brief Return a pointer to this image in a form which is compatible with OpenCV
	 * 
	 * The memory of the IplImage is released on destruction of this Image Object.
	 */	
	IplImage* getIplImage();

	/**
	 * @brief Return the width of the picture
	 */
	int getWidth() const;
	
	/**
	 * @brief Return the height of the picture
	 */
	int getHeight() const;
	
	/**
	 * @brief Return the number of bytes for a horizontal picture line
	 * 
	 * The allocated width is >= the width of a picture and aligned to the align
	 * parameter.
	 */
	int getAllocatedWidth() const;

	/**
	 * @brief This value should be equal to the height of the picture.
	 */
	int getAllocatedHeight() const;

	/**
	 * @brief Return the number of bytes allocated for this Image.
	 */
	int getAllocatedBytes() const;
	
	/**
	 * @brief Resize the image buffer of an existing image to the new dimensions
	 * @param newWidth The new width of the image
	 * @param newHeight The new height of the image
	 * 
	 * Release an existing image buffer and allocate a new image buffer for the 
	 * given dimensions with appropriate pixel line alignment. The content of the
	 * new image buffer is undefined.
	 */
	void resizeMemory(const int newWidth, const int newHeight);
	
	/**
	 * @brief Random Pixel Access.
	 * @param x x-coordinate of the pixel starting at 0
	 * @param y y-coordinate of the pixel starting at 0
	 * @param channel Select the desired channel (0, 1, 2 or 3)
	 * 
	 * This operator doesn't check any bounds, yet. For performance reasons
	 * this function was declared inline, but still it involves 2
	 * multiplications. Please do not use this function for sequential pixel 
	 * access.
	 */
	unsigned char& operator() (unsigned x, unsigned y, unsigned channel);
	
	/**
	 * @brief Random Pixel Access.
	 * @param x x-coordinate of the pixel starting at 0
	 * @param y y-coordinate of the pixel starting at 0
	 * @param channel Select the desired channel (0, 1, 2 or 3)
	 * 
	 * This operator doesn't check any bounds, yet. For performance reasons
	 * this function was declared inline, but still it involves 2
	 * multiplications. Please do not use this function for sequential pixel 
	 * access.
	 */
	unsigned char operator() (unsigned x, unsigned y, unsigned channel) const;
	
	/* overload the = operator 
	 *TODO check the memory!!
	 */
	void operator = (const Image &img);

	virtual Image* clone() const;		
	
protected:
	/** @brief number of allocated bytes in total for this image */
	int allocatedBytes;
	/** @brief buffer for this image */
	unsigned char* imageBuffer;
	/** @brief allocated width for this Image, usually is aligned to sth */
	int allocatedWidth;
	/** @brief allocated height for this Image */
	int allocatedHeight;
	
	/** @brief width of the stored image in pixel (may be smaller than allocatedWidth and widthStep) */
	int width;
	/** @brief height of the stored image in pixel (may be smaller than allocatedHeight) */
	int height;
	/** @brief number of channels per pixel (1 for greyscale, 3 for RGB) */
	Channels channels;
	/** @brief number of bits per channels (default 8) */
	BitsPerChannel bitsPerChannel;
	/** @brief Alignment of image lines (default 4) */
	LineAlignment lineAlignment;
	/** @brief For compatibility provide an OpenCV Header */
	IplImage* opencvHeader;
	
	/** @brief Updates the internal opencvHeader attribute */
	void updateOpencvHeader();
};

} // namespace ds

} // namespace tt

#endif /*TT_DS_IMAGE_H*/
