#ifndef TT_INPUT_MOVIEPLAYER_H
#define TT_INPUT_MOVIEPLAYER_H

#include <cv.h>
#include <highgui.h>
#include <string>
#include <time.h>

#include <tt/input/ImageDevice.h>
#include <tt/ds/Image.h>

namespace tt
{

namespace input
{
	
/**
 * @class MoviePlayer MoviePlayer.h tt/input/MoviePlayer.h
 * @brief class to capture images from a video file
 * @author Thomas Friedlhuber <friedlhu@in.tum.de>
 **/
class MoviePlayer : public tt::input::ImageDevice
{
public:

	/**
	 * constructor
	 **/
	MoviePlayer();
	
	/**
	 * destructor
	 **/
	virtual ~MoviePlayer();
	
	/**
	 * capture next image
	 **/
	virtual void captureNext();
	
	/**
	 * start capture process
	 **/
	virtual void captureStart();
	
	/** 
	 * stop capture process
	 **/
	virtual void captureStop();
	
	/**
	 * close video file
	 **/
	virtual void close();

	/**
	 * check if end video file has been reached
	 * @return true if video is finished else false
	 **/
	bool isFinished() const;
	
	/**
	 * get image data from last captured image
	 * @return image data
	 **/
	virtual tt::ds::Image* getImage();
	
	/**
	 * get height of images
	 * @return height
	 *
	 **/
	virtual const int getImageHeight() const;
	
	/**
	 * get width of images
	 * @return width
	 **/
	virtual const int getImageWidth() const;
	
	/** 
	 * get opencv image data from last captured image
	 * @return opencv image data
	 **/
	IplImage* getopencvImage();

	/** 
	 * get speed factor the frame rate of a video is multiplied with. The higher the speed, 
	 * the faster the video is running. A value of 1.0 means realtime.
	 * @return speed factor
	 **/
	double getSpeed();

	/**
	 * initialize video capturing process
	 **/
	virtual void init();
	
	/**
	 * open default video stream
	 **/
	virtual void open();
	
	/**
	 * open video stream
	 * @param filename video filename
	 **/
	void open(std::string filename);

	/**
	 * set speed factor the frame rate of a video is multiplied with. The higher the speed, 
	 * the faster the video is running. A value of 1.0 means realtime.
	 **/
	void setSpeed(double factor);
		
private:
	
	/** capture instance for opencv **/
	CvCapture *m_capture;
	
	/** timestamp of last captured image **/
	clock_t m_end;
	
	/** video filename **/
	std::string m_filename;

	/** states if video is finished **/
	bool m_finished;
	
	/** image data **/
	tt::ds::Image* m_image;

	/** milliseconds of video file **/
	double m_maxmsec;

	/** speed factor the frame rate of video is multiplied with **/
	double m_speed;
	  
	/** timestamp of first captured image **/
	clock_t m_start;
	
	/** default filename */
	static const std::string DEFAULT_FILENAME;
	
};

} // namespace input

} // namespace tt

#endif /*TT_INPUT_MOVIEPLAYER_H*/
