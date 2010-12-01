/*
 * MoviePlayer
 * by Martin Wojtczyk <wojtczyk@in.tum.de>
 */

#include "MoviePlayer.h"

#include <math.h>

namespace tt
{

namespace input
{

// default filename
const std::string MoviePlayer::DEFAULT_FILENAME = "video.avi";
 
MoviePlayer::MoviePlayer()
{
	m_image = NULL;
	m_capture = NULL;
	m_start = clock();
	m_speed = 1.0f;
	m_finished = true;
	m_image = NULL;
	m_maxmsec = 0.0f;
}

MoviePlayer::~MoviePlayer()
{
	if (m_capture)
	{
		cvReleaseCapture(&m_capture);
	}
	if (m_image)
	{
		delete m_image;
	}
}

void MoviePlayer::captureNext()
{
	std::string functionSignature = "void MoviePlayer::captureNext()";
	
	if (m_capture==NULL)
	{
		throw std::runtime_error(functionSignature + " no video opened");
	}
	
	if (m_image==NULL)
	{
		throw std::runtime_error(functionSignature + " capture process not started");
	}

	if (this->m_finished)
	{
		return;
	}
	
	m_end = clock();
	
	// film current position in milliseconds
	//double msec = (m_end - m_start) * 1000 / CLOCKS_PER_SEC * m_speed;
	
	// IT HAS NO USEFUL EFFECT ON THE SEQUENCE (it just skips frames!)
//	cvSetCaptureProperty(m_capture, CV_CAP_PROP_POS_MSEC, msec);
	
	if (!cvGrabFrame(m_capture))
	{
		m_finished = true;
	}
	
	IplImage *img = cvRetrieveFrame(m_capture);

	if(!img)
		m_finished = true;
	// FIXED BUG: at the end of the sequence, the next image is absent!
	else
		cvCopy(img, m_image->getIplImage());
}

void MoviePlayer::captureStart()
{
	std::string functionSignature = "void MoviePlayer::captureStart()";
	
	if (m_capture==NULL)
	{
		throw std::runtime_error(functionSignature + " no video opened");
	}
	
	m_start = clock();

	cvSetCaptureProperty(m_capture, CV_CAP_PROP_POS_MSEC, 0);
	cvGrabFrame(m_capture);
	
	IplImage *img = cvRetrieveFrame(m_capture);
	if (m_image)
	{
		delete m_image;
	}
	this->m_finished = false;
	m_image = new tt::ds::Image(img);
}

void MoviePlayer::captureStop()
{
	if (m_image)
	{
		delete m_image;
		m_image = NULL;
	}

	this->m_finished = true;
}

void MoviePlayer::close() 
{
}

bool MoviePlayer::isFinished() const
{
	return m_finished;
}

tt::ds::Image* MoviePlayer::getImage()
{
	std::string functionSignature = "tracking::ds::Image* MoviePlayer::getImage()";
	
	if (m_image==NULL)
	{
		throw std::runtime_error(functionSignature + " no image captured");
	}
	return m_image;  
}

const int MoviePlayer::getImageHeight() const
{
	std::string functionSignature = "const int MoviePlayer::getImageHeight()";
	
	if (m_image==NULL)
	{
		throw std::runtime_error(functionSignature + " no image captured");
	}
	return m_image->getHeight();
}

const int MoviePlayer::getImageWidth() const
{
	std::string functionSignature = "const int MoviePlayer::getImageWidth()";
	
	if (m_image==NULL)
	{
		throw std::runtime_error(functionSignature + " no image captured");
	}
	return m_image->getWidth();
}

IplImage* MoviePlayer::getopencvImage()
{
	return m_image->getIplImage();
}

double MoviePlayer::getSpeed() 
{
	return m_speed;
}

void MoviePlayer::init()
{
}

void MoviePlayer::open()
{
	if (m_filename=="")
	{
		m_filename = DEFAULT_FILENAME;
	}
	this->open(m_filename);	
}

void MoviePlayer::open(std::string filename)
{
	std::string functionSignature = "void MoviePlayer::open(std::string filename)";
	
	m_filename = filename;

	if (m_capture) {
		cvReleaseCapture(&m_capture);
	}
	
	m_capture = cvCreateFileCapture(m_filename.c_str());

	if (m_capture==NULL)
	{
		throw std::runtime_error(functionSignature + " file not found or video format is not supported");
	}
	
	double fps = cvGetCaptureProperty(m_capture, CV_CAP_PROP_FPS);
	double framecnt = cvGetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_COUNT);
	m_maxmsec = (framecnt-1) / fps * 1000.0f;

	captureStart();
}

void MoviePlayer::setSpeed(double factor)
{
	m_speed = factor;
}

} // namespace input

} // namespace tt
