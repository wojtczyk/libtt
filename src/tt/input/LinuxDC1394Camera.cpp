/*
 * LinuxDC1394Camera
 * by Martin Wojtczyk <wojtczyk@in.tum.de>
 */

#ifdef LINUX // Build LinuxDC1394Camera only if compiled on a Linux platform

#include <string>
#include <sstream>
#include <assert.h>
#include <tracking/cvprocess/Bayer.h>
#include "LinuxDC1394Camera.h"

#include <iostream>

using namespace std;
using namespace tt::process;
using namespace tt::ds;

namespace tt
{

namespace input
{

int LinuxDC1394Camera::currentChannel = 0;

/**
 * @brief Initializes the members of the LinuxDC1394Camera object.
 */
LinuxDC1394Camera::LinuxDC1394Camera()
{
	cameraIndex = 0;
	card = 0;
	rawHandle = NULL;
	cameraNode = 0;
	capturing = false;
	channel = currentChannel++; // Where is the documentation for channel?
	cout << "iso channel = " << channel << endl;
	speed = SPEED_400;
	dmaBufNum = 16;
	imageWidth = 640;
	imageHeight = 480;
	currentFrame = NULL;
	currentRGBFrame = NULL;
	colorMode = FirewireCamera::COLOR_RGB;
}

/**
 * @brief Delete the allocated Image Frame Objects.
 */
LinuxDC1394Camera::~LinuxDC1394Camera()
{
	if (currentFrame != NULL)
	{
		delete currentFrame;
	}
	if (currentRGBFrame != NULL)
	{
		delete currentRGBFrame;
	}
}

/**
 * @brief Open camera connection.
 *
 * The found camera is then initialised and made ready for image capturing.
 */
void LinuxDC1394Camera::open()
{
	string functionSignature = "void LinuxDC1394Camera::open()";

	// Set rawHandle and card according to the cameraIndex
	raw1394handle_t countHandle = raw1394_new_handle();
	if (countHandle)
	{
		int cards = raw1394_get_port_info(countHandle, NULL, 0);
		raw1394_destroy_handle(countHandle);
		
		nodeid_t* cameraNodes;
		int currentCameraIndexSum = 0;
		int showCameras = 0;
		bool scanCameras = true;
		bool cameraFound = false;
		this->card = 0;
		int camerasOnCard = 0;
		while (scanCameras == true)
		{
			this->rawHandle = dc1394_create_handle(card);
			if (!(this->rawHandle))
			{
				// Could not get raw1394 handle
				throw std::runtime_error(functionSignature +
					" could not get raw1394 handle.");
			}
			
			cameraNodes = dc1394_get_camera_nodes(this->rawHandle, &camerasOnCard,
				showCameras);

			cout << "card = " << card << " has " << camerasOnCard << " cameras " << endl;

			// we found the right card (port) get the right cameraNode
			if (this->cameraIndex <= currentCameraIndexSum + camerasOnCard - 1)
			{
				scanCameras = false;
				this->cameraNode = cameraNodes[this->cameraIndex - currentCameraIndexSum];

				// double check if the node is in fact a camera
				dc1394bool_t isCamera;
				dc1394_is_camera(this->rawHandle, this->cameraNode, &isCamera);
				
				if (!isCamera)
				{	// it's no camera -> error
					throw std::runtime_error(functionSignature + 
						" given node is not a camera");
				}
				else
				{	// rawHandle and cameraNode identify the right cameraIndex
					cameraFound = true;
					cout << "found camera for index " << this->cameraIndex << " on card " << card << " node " << this->cameraIndex - currentCameraIndexSum << endl;
				}
			}
			else // check the next card
			{
				currentCameraIndexSum += camerasOnCard;
			}
			
			// next card (port)
			if (scanCameras && !cameraFound)
			{
				card++;
				if (card >= cards)
				{
					scanCameras = false;
					// We scanned all the cards and didn't find the appropriate camera
					throw std::runtime_error(functionSignature +
						" no camera found.");
				}
			}
		}
				
		// initialize camera
		dc1394_init_camera(this->rawHandle, this->cameraNode);
		
		// switch camera on
		dc1394_camera_on(this->rawHandle, this->cameraNode);
	}
	else
	{
		// Could not get raw1394 handle
		throw std::runtime_error(functionSignature +
			" could not get raw1394 handle.");
	}
}

/**
 * @brief Closes the camera connection
 * 
 * Switches the camera off and closes the connection.
 */
void LinuxDC1394Camera::close()
{
	// switch camera off
	dc1394_camera_off(this->rawHandle, this->cameraNode);
	
	// release firewire handle
	dc1394_destroy_handle(this->rawHandle);
}

/**
 * @brief Initialise camera to factory default settings.
 */
void LinuxDC1394Camera::init()
{
	// Initialize camera to factory default settings
	dc1394_init_camera(this->rawHandle, this->cameraNode);
}

/**
 * @brief Put camera into capturing mode and start data transmission.
 */
void LinuxDC1394Camera::captureStart()
{
	string functionSignature = "void LinuxDC1394Camera::captureStart()";

	if (capturing)
	{
		throw std::runtime_error(functionSignature + 
			" already in capture mode.");
	}

	stringstream video1394devname;
	video1394devname << "/dev/video1394/" << this->card;
	
	cout << "capture device: " << video1394devname.str() << endl;
	cout << "camera node: " << cameraNode << endl;

	unsigned int dc1394Format;
	unsigned int dc1394Mode;
	unsigned int dc1394Framerate;
	
	// setup Format 7 dma capture
	if (this->videoFormat == FirewireCamera::FORMAT7)
	{
		switch (this->videoMode)
		{
			case FirewireCamera::MODE0:
				dc1394Mode = MODE_FORMAT7_0;
				break;
				
			case FirewireCamera::MODE1:
				dc1394Mode = MODE_FORMAT7_1;
				break;
				
			default:
				throw std::runtime_error(functionSignature 
					+ " Format " + FirewireCamera::getVideoFormatString(this->videoFormat)
					+ ", Mode " + FirewireCamera::getVideoModeString(this->videoMode)
					+ " not supported."); 
				break;
		}
		
		// we took care of 
		// videoFormat
		// videoMode
		// videoFramerate (ignored in Format7)
		// now setup capture
		
		if (dc1394_dma_setup_format7_capture(this->rawHandle,
			this->cameraNode,
			this->channel,
			dc1394Mode,
			this->speed,
			USE_MAX_AVAIL, /* bytes per packet*/
			QUERY_FROM_CAMERA, /* left */
			QUERY_FROM_CAMERA, /* top */
			USE_MAX_AVAIL, /* width */
			USE_MAX_AVAIL, /* height */
			this->dmaBufNum,
			1, /* drop frames */
			video1394devname.str().c_str(),
			&(this->camera)) != DC1394_SUCCESS)
		{
			throw std::runtime_error(functionSignature + 
				" unable to setup camera in Format 7. Please check if the kernel module "
				"video1394 is loaded and that the video mode, framerate and format "
				"are supported by your camera");
		}
		
		// set image width and height according to capture mode
		dc1394_query_format7_image_size(this->rawHandle, this->cameraNode,
			MODE_FORMAT7_0, (unsigned int *) &(this->imageWidth), 
			(unsigned int *) &(this->imageHeight));
	
		cout << "format7_0 width: " << this->imageWidth << " height: " << this->imageHeight << endl;
	
		dc1394_query_format7_total_bytes(this->rawHandle, this->cameraNode,
			MODE_FORMAT7_0, (unsigned long long int*) &(this->bufferSize));
	
		cout << "bufferSize: " << this->bufferSize << endl;
	
		unsigned int color_id;
		dc1394_query_format7_color_coding_id(this->rawHandle, this->cameraNode,
			MODE_FORMAT7_0, (unsigned int*) &color_id);
	
		cout << "color_id = " << color_id << ", COLOR_FORMAT7_MONO8 = " << COLOR_FORMAT7_MONO8 << endl; 
	
		// support is FORMAT7 MODE0, 8bpp mono
		assert(color_id == COLOR_FORMAT7_MONO8);
		assert(this->bufferSize == this->imageWidth * this->imageHeight);
		this->colorMode = FirewireCamera::COLOR_GREYSCALE;
	}
	else
	{ // setup the other formats
		// Translate FirewireCamera::Formats into libdc1394 formats
		switch (this->videoFormat)
		{
			case FirewireCamera::FORMAT0:
				dc1394Format = ::FORMAT_VGA_NONCOMPRESSED;
				break;
				
			case FirewireCamera::FORMAT1:
				dc1394Format = ::FORMAT_SVGA_NONCOMPRESSED_1;
				break;
				
			case FirewireCamera::FORMAT2:
				dc1394Format = ::FORMAT_SVGA_NONCOMPRESSED_2;
				break;
								
			default:
				throw std::runtime_error(functionSignature 
					+ " Format " + FirewireCamera::getVideoFormatString(this->videoFormat)
					+ " not supported."); 
				break;
		}

		// Translate FirewireCamera::Modes into libdc1394 modes
		switch (this->videoMode)
		{
			case FirewireCamera::MODE_160x120_YUV444:
				dc1394Mode = ::MODE_160x120_YUV444;
				break;
				
			case FirewireCamera::MODE_320x240_YUV422:
				dc1394Mode = ::MODE_320x240_YUV422;
				break;
				
			case FirewireCamera::MODE_640x480_YUV411:
				dc1394Mode = ::MODE_640x480_YUV411;
				break;
				
			case FirewireCamera::MODE_640x480_YUV422:
				dc1394Mode = ::MODE_640x480_YUV422;
				break;
				
			case FirewireCamera::MODE_640x480_RGB:
				dc1394Mode = ::MODE_640x480_RGB;
				break;
				
			case FirewireCamera::MODE_640x480_MONO8:
				dc1394Mode = ::MODE_640x480_MONO;
				break;
				
			case FirewireCamera::MODE_640x480_MONO16:				
				dc1394Mode = ::MODE_640x480_MONO16;
				break;
				
			case FirewireCamera::MODE_800x600_YUV422:
				dc1394Mode = ::MODE_800x600_YUV422;
				break;
				
			case FirewireCamera::MODE_800x600_RGB:
				dc1394Mode = ::MODE_800x600_RGB;
				break;
				
			case FirewireCamera::MODE_800x600_MONO8:
				dc1394Mode = ::MODE_800x600_MONO;
				break;
				
			case FirewireCamera::MODE_1024x768_YUV422:
				dc1394Mode = ::MODE_1024x768_YUV422;
				break;
				
			case FirewireCamera::MODE_1024x768_RGB:
				dc1394Mode = ::MODE_1024x768_RGB;
				break;
				
			case FirewireCamera::MODE_1024x768_MONO8:
				dc1394Mode = ::MODE_1024x768_MONO;
				break;
				
			case FirewireCamera::MODE_800x600_MONO16:
				dc1394Mode = ::MODE_800x600_MONO16;
				break;
				
			case FirewireCamera::MODE_1024x768_MONO16:
				dc1394Mode = ::MODE_1024x768_MONO16;
				break;
				
			case FirewireCamera::MODE_1280x960_YUV422:
				dc1394Mode = ::MODE_1280x960_YUV422;
				break;
				
			case FirewireCamera::MODE_1280x960_RGB:
				dc1394Mode = ::MODE_1280x960_RGB;
				break;
				
			case FirewireCamera::MODE_1280x960_MONO8:
				dc1394Mode = ::MODE_1280x960_MONO;
				break;
				
			case FirewireCamera::MODE_1600x1200_YUV422:
				dc1394Mode = ::MODE_1600x1200_YUV422;
				break;
				
			case FirewireCamera::MODE_1600x1200_RGB:
				dc1394Mode = ::MODE_1600x1200_RGB;
				break;
				
			case FirewireCamera::MODE_1600x1200_MONO8:
				dc1394Mode = ::MODE_1600x1200_MONO;
				break;
				
			case FirewireCamera::MODE_1280x960_MONO16:
				dc1394Mode = ::MODE_1280x960_MONO16;
				break;
				
			case FirewireCamera::MODE_1600x1200_MONO16:
				dc1394Mode = ::MODE_1600x1200_MONO16;
				break;
								
			default:
				throw std::runtime_error(functionSignature 
					+ " Format " + FirewireCamera::getVideoFormatString(this->videoFormat)
					+ ", Mode " + FirewireCamera::getVideoModeString(this->videoMode)
					+ " not supported."); 
				break;
		}
		
		switch (this->videoFramerate)
		{
			case FirewireCamera::FRAMERATE_1_875:
				dc1394Framerate = ::FRAMERATE_1_875;
				break;
								
			case FirewireCamera::FRAMERATE_3_75:
				dc1394Framerate = ::FRAMERATE_3_75;
				break;
								
			case FirewireCamera::FRAMERATE_7_5:
				dc1394Framerate = ::FRAMERATE_7_5;
				break;
								
			case FirewireCamera::FRAMERATE_15:
				dc1394Framerate = ::FRAMERATE_15;
				break;
								
			case FirewireCamera::FRAMERATE_30:
				dc1394Framerate = ::FRAMERATE_30;
				break;
								
			case FirewireCamera::FRAMERATE_60:
				dc1394Framerate = ::FRAMERATE_60;
				break;
								
			case FirewireCamera::FRAMERATE_120:
				dc1394Framerate = ::FRAMERATE_120;
				break;
								
			case FirewireCamera::FRAMERATE_240:
				dc1394Framerate = ::FRAMERATE_240;
				break;
								
			default:
				throw std::runtime_error(functionSignature 
					+ " Format " + FirewireCamera::getVideoFormatString(this->videoFormat)
					+ ", Mode " + FirewireCamera::getVideoModeString(this->videoMode)
					+ ", Framerate " + FirewireCamera::getVideoFramerateString(this->videoFramerate)
					+ " not supported."); 
				break;
		}
		
		// we took care of 
		// videoFormat
		// videoMode
		// videoFramerate
		// now setup capture

		if (dc1394_dma_setup_capture(this->rawHandle,
			this->cameraNode,
			this->channel,
			dc1394Format,
			dc1394Mode,
			this->speed,
			dc1394Framerate,
			this->dmaBufNum,
			1, // drop frames
			video1394devname.str().c_str(),
			&(this->camera)) != DC1394_SUCCESS)
		{
			throw std::runtime_error(functionSignature 
				+ " Format " + FirewireCamera::getVideoFormatString(this->videoFormat)
				+ ", Mode " + FirewireCamera::getVideoModeString(this->videoMode)
				+ ", Framerate " + FirewireCamera::getVideoFramerateString(this->videoFramerate)
				+ " not supported."); 
		}

		// get dimensions out of the internal dc1394_cameracapture struct, since
		// there doesn't seem to be any other way in these modes.
		this->imageWidth = this->camera.frame_width;
		this->imageHeight = this->camera.frame_height;
		this->bufferSize = this->camera.dma_frame_size;

		// Check the video mode for color conversion support
		switch (this->videoMode)
		{
			case FirewireCamera::MODE_320x240_YUV422:
			case FirewireCamera::MODE_640x480_YUV422:
			case FirewireCamera::MODE_800x600_YUV422:
			case FirewireCamera::MODE_1024x768_YUV422:
			case FirewireCamera::MODE_1280x960_YUV422:
			case FirewireCamera::MODE_1600x1200_YUV422:
				this->colorMode = FirewireCamera::COLOR_YUV422;
				break;
				
			case FirewireCamera::MODE_640x480_MONO8:
			case FirewireCamera::MODE_800x600_MONO8:
			case FirewireCamera::MODE_1024x768_MONO8:
			case FirewireCamera::MODE_1280x960_MONO8:
			case FirewireCamera::MODE_1600x1200_MONO8:
				this->colorMode = FirewireCamera::COLOR_GREYSCALE;
				break;
				
			case FirewireCamera::MODE_640x480_RGB:
			case FirewireCamera::MODE_800x600_RGB:
			case FirewireCamera::MODE_1024x768_RGB:
			case FirewireCamera::MODE_1280x960_RGB:
			case FirewireCamera::MODE_1600x1200_RGB:
				this->colorMode = FirewireCamera::COLOR_RGB;
				break;
				
			case FirewireCamera::MODE_160x120_YUV444:
			case FirewireCamera::MODE_640x480_YUV411:
			case FirewireCamera::MODE_640x480_MONO16:				
			case FirewireCamera::MODE_800x600_MONO16:
			case FirewireCamera::MODE_1024x768_MONO16:
			case FirewireCamera::MODE_1280x960_MONO16:
			case FirewireCamera::MODE_1600x1200_MONO16:
			default:
				throw std::runtime_error(functionSignature
					+ " Sorry, color conversion for "
					+ " Mode " + FirewireCamera::getVideoModeString(this->videoMode)
					+ " not implemented, yet.");
				break;
		}
	}
	
	// allocate Images to store frames
	if (currentFrame != NULL)
	{
		delete currentFrame;
	}
	switch (this->colorMode)
	{
		case FirewireCamera::COLOR_GREYSCALE:
			currentFrame = new Image(this->imageWidth, this->imageHeight, Image::GREYSCALE);
			break;

		case FirewireCamera::COLOR_RGB:
		case FirewireCamera::COLOR_YUV422: /* jaja, this actually takes 2 bytes only, but we just need the buffer */
			currentFrame = new Image(this->imageWidth, this->imageHeight, Image::RGB);
			break;
	}

	if (currentRGBFrame != NULL)
	{
		delete currentRGBFrame;
	}
	currentRGBFrame = new Image(this->imageWidth, this->imageHeight, Image::RGB);
	
	if (dc1394_start_iso_transmission(this->rawHandle, this->cameraNode) != DC1394_SUCCESS)
	{
		throw std::runtime_error(functionSignature + 
			" unable to start iso transmission from camera.");
	}

	capturing = true;
}

/**
 * @brief Stop image capturing and data transmission from the camera.
 */
void LinuxDC1394Camera::captureStop()
{
	string functionSignature = "void LinuxDC1394Camera::captureStop()";

	if (!capturing)
	{
		throw std::runtime_error(functionSignature + " not in capture mode.");
	}

	if (dc1394_dma_single_capture(&(this->camera)) != DC1394_SUCCESS)
	{
		throw std::runtime_error(functionSignature + " unable to capture a single frame.");	
	}
	
	if (dc1394_stop_iso_transmission(this->rawHandle, this->cameraNode) != DC1394_SUCCESS)
	{
		throw std::runtime_error(functionSignature + " could not stop camera.");
	}
	
	dc1394_dma_release_camera(this->rawHandle, &(this->camera));
	capturing = false;
}

/**
 * @brief Capture the next image.
 * 
 * This method needs to be called along with getImage to avoid to overflow
 * the dma ring buffer. captureNext makes the dma buffer of the last read
 * picture available again for a new picture.
 */ 
void LinuxDC1394Camera::captureNext()
{
	string functionSignature = "void LinuxDC1394Camera::captureNext()";

	if (!capturing)
	{
		throw std::runtime_error(functionSignature + " not in capture mode.");
	}
	
	if (dc1394_dma_done_with_buffer(&(this->camera)) != DC1394_SUCCESS)
	{
		throw std::runtime_error(functionSignature + 
			" dc1394_dma_done_with_buffer() failed.");
	}
}

/**
 * @brief Return a pointer to the next image.
 * @return Pointer to the image raw data within the dma ring buffer.
 * 
 * Use this pointer to read the image data from the camera. You may also
 * need to know the image size and format to avoid memory access violations.
 */
tt::ds::Image* LinuxDC1394Camera::getImage()
{
	string functionSignature = "tracking::ds::Image* LinuxDC1394Camera::getImage()";

	if (!capturing)
	{
		throw std::runtime_error(functionSignature + " not in capture mode.");
	}
	
	if (dc1394_dma_single_capture(&(this->camera)) != DC1394_SUCCESS)
	{
		throw std::runtime_error(functionSignature + " unable to capture a single frame.");	
	}

	unsigned char* rgbImage;
	unsigned char* greyImage;
	switch (this->colorMode)
	{
		case FirewireCamera::COLOR_RGB:
			// just copy directly to currentRGBFrame
			rgbImage = this->currentRGBFrame->getImageBuffer();
			memcpy(rgbImage, (unsigned char*)(this->camera.capture_buffer), this->bufferSize); 
			break;
			
		case FirewireCamera::COLOR_GREYSCALE:
			// copy from camera buffer to grey image
			greyImage = this->currentFrame->getImageBuffer();
			memcpy(greyImage, (unsigned char*)(this->camera.capture_buffer), this->bufferSize); 
			Bayer::deBayer(this->currentFrame, this->currentRGBFrame, this->bayerFilter);
			break;

		case FirewireCamera::COLOR_YUV422:
			break;
	}

	return this->currentRGBFrame;
}

/**
 * @brief Returns the width of the captured image.
 * @return Width in pixel.
 * 
 * Returns the width of the last image whose pointer was returned by 
 * getImage().
 */
const int LinuxDC1394Camera::getImageWidth() const
{
	return this->imageWidth;
}

/**
 * @brief Returns the height of the captured image.
 * @return Height in pixel.
 * 
 * Returns the height of the last image whose pointer was returned by 
 * getImage().
 */
const int LinuxDC1394Camera::getImageHeight() const
{
	return this->imageHeight;
}

void LinuxDC1394Camera::getCaptureParameters(int& width, int& height, 
	ds::Image::Channels& channels, ds::Image::BitsPerChannel& bpc)
{
	width = this->imageWidth;
	height = this->imageHeight;
	channels = ds::Image::RGB;
	bpc = ds::Image::BPC8;
}

void LinuxDC1394Camera::enableWhiteBalanceOnePush(bool enable)
{
	// TODO
}

void LinuxDC1394Camera::enableWhiteBalanceAuto(bool enable)
{
	dc1394_auto_on_off(this->rawHandle, this->cameraNode, FEATURE_WHITE_BALANCE, enable);
}

void LinuxDC1394Camera::getWhiteBalance(unsigned int* ubValue, unsigned int* vrValue)
{
	// TODO test
	dc1394_get_white_balance(this->rawHandle, this->cameraNode, ubValue, vrValue);
}

void LinuxDC1394Camera::setWhiteBalance(unsigned int ubValue, unsigned int vrValue)
{
	// TODO test
	dc1394_set_white_balance(this->rawHandle, this->cameraNode, ubValue, vrValue);
}

void LinuxDC1394Camera::enableShutterAuto(bool enable)
{
	dc1394_auto_on_off(this->rawHandle, this->cameraNode, FEATURE_SHUTTER, enable);
}

void LinuxDC1394Camera::enableGainAuto(bool enable)
{
	dc1394_auto_on_off(this->rawHandle, this->cameraNode, FEATURE_GAIN, enable);
}

// LinuxDC1394Camera specific functions

/**
 * @brief Return the number of cameras
 */
int LinuxDC1394Camera::getNumberOfLinuxDC1394Cameras()
{
	int numCameras = 0;
	int showCameras = 1;

	// Get the number of available ieee1394 cards (or ports in libraw1394 terminology)
	raw1394handle_t rawHandle = raw1394_new_handle();
	if (rawHandle)
	{
		int cards = raw1394_get_port_info(rawHandle, NULL, 0);
		raw1394_destroy_handle(rawHandle);
		
		// sum up all cameras per card (port)
		for (int card = 0; card < cards; card++)
		{
			rawHandle = dc1394_create_handle(card);
			int camerasOnCard;
			
			// get the first firewire camera on the given port
			nodeid_t* cameraNodes;
			cameraNodes = dc1394_get_camera_nodes(rawHandle, &camerasOnCard, showCameras);
			
			if (camerasOnCard > 0)
			{
				numCameras += camerasOnCard;
			}
			
			// release firewire handle
			dc1394_destroy_handle(rawHandle);
		}
	}
	
	return numCameras;
}

void LinuxDC1394Camera::selectCamera(int index)
{
	this->cameraIndex = index;
}

} // namespace input

} // namespace tt

#endif // LINUX
