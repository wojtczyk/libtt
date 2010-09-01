#include <assert.h>
#include "Bayer.h"

namespace tt
{

namespace process
{

/**
 * @brief Converts a single cahnnel greyscale picture into an rgb image
 * @param source The source picture (must be GREYSCALE)
 * @param destination (must be RGB)
 * @param filter Use this filter for debayering
 */
void Bayer::deBayer(tracking::cvdata::Image* source, tracking::cvdata::Image* destination, Filter filter)
{
	assert(source->getChannels() == tracking::cvdata::Image::GREYSCALE);
	assert(destination->getChannels() == tracking::cvdata::Image::RGB);
	assert(source->getWidth() == destination->getWidth());
	assert(source->getHeight() == destination->getHeight());

	/*
	opencv  Bayer Pattern -> RGB conversion
	 
	int icvBayer2BGR_8u_C1C3R( const uchar* bayer0, int bayer_step,
	                       uchar *dst0, int dst_step,
	                       CvSize size, int code )
	*/
	const unsigned char* bayer0 = (const unsigned char*) source->getImageBuffer();
	int bayer_step = source->getAllocatedWidth();
	unsigned char* dst0 = (unsigned char*) destination->getImageBuffer();
	int dst_step = destination->getAllocatedWidth();
	CvSize size;
	size.width = source->getWidth();
	size.height = source->getHeight();
	int code = filter;

	int blue = code == CV_BayerBG2BGR || code == CV_BayerGB2BGR ? -1 : 1;
	int start_with_green = code == CV_BayerGB2BGR || code == CV_BayerGR2BGR;

	memset(dst0, 0, size.width*3*sizeof(dst0[0]));
	memset(dst0 + (size.height - 1)*dst_step, 0, size.width*3*sizeof(dst0[0]));
	dst0 += dst_step + 3 + 1;
	size.height -= 2;
	size.width -= 2;

	for (; size.height-- > 0; bayer0 += bayer_step, dst0 += dst_step)
	{
		int t0, t1;
		const uchar* bayer = bayer0;
		uchar* dst = dst0;
		const uchar* bayer_end = bayer + size.width;

		dst[-4] = dst[-3] = dst[-2] = dst[size.width*3-1] =
			dst[size.width*3] = dst[size.width*3+1] = 0;

		if (size.width <= 0)
			continue;

		if (start_with_green)
		{
			t0 = (bayer[1] + bayer[bayer_step*2+1] + 1) >> 1;
			t1 = (bayer[bayer_step] + bayer[bayer_step+2] + 1) >> 1;
			dst[-blue] = (uchar)t0;
			dst[0] = bayer[bayer_step+1];
			dst[blue] = (uchar)t1;
			bayer++;
			dst += 3;
		}

		if (blue > 0)
		{
			for (; bayer <= bayer_end - 2; bayer += 2, dst += 6)
			{
				t0 = (bayer[0] + bayer[2] + bayer[bayer_step*2] +
				      bayer[bayer_step*2+2] + 2) >> 2;
				t1 = (bayer[1] + bayer[bayer_step] +
				      bayer[bayer_step+2] + bayer[bayer_step*2+1] + 2) >> 2;
				dst[-1] = (uchar)t0;
				dst[0] = (uchar)t1;
				dst[1] = bayer[bayer_step+1];

				t0 = (bayer[2] + bayer[bayer_step*2+2] + 1) >> 1;
				t1 = (bayer[bayer_step+1] + bayer[bayer_step+3] + 1) >> 1;
				dst[2] = (uchar)t0;
				dst[3] = bayer[bayer_step+2];
				dst[4] = (uchar)t1;
			}
		}
		else
		{
			for (; bayer <= bayer_end - 2; bayer += 2, dst += 6)
			{
				t0 = (bayer[0] + bayer[2] + bayer[bayer_step*2] +
				      bayer[bayer_step*2+2] + 2) >> 2;
				t1 = (bayer[1] + bayer[bayer_step] +
				      bayer[bayer_step+2] + bayer[bayer_step*2+1] + 2) >> 2;
				dst[1] = (uchar)t0;
				dst[0] = (uchar)t1;
				dst[-1] = bayer[bayer_step+1];

				t0 = (bayer[2] + bayer[bayer_step*2+2] + 1) >> 1;
				t1 = (bayer[bayer_step+1] + bayer[bayer_step+3] + 1) >> 1;
				dst[4] = (uchar)t0;
				dst[3] = bayer[bayer_step+2];
				dst[2] = (uchar)t1;
			}
		}

		if (bayer < bayer_end)
		{
			t0 = (bayer[0] + bayer[2] + bayer[bayer_step*2] +
			      bayer[bayer_step*2+2] + 2) >> 2;
			t1 = (bayer[1] + bayer[bayer_step] +
			      bayer[bayer_step+2] + bayer[bayer_step*2+1] + 2) >> 2;
			dst[-blue] = (uchar)t0;
			dst[0] = (uchar)t1;
			dst[blue] = bayer[bayer_step+1];
			bayer++;
			dst += 3;
		}

		blue = -blue;
		start_with_green = !start_with_green;
	}
}

} // namespace process

} // namespace trax
