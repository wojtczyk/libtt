#ifndef TRAX_PROCESS_BAYER_H
#define TRAX_PROCESS_BAYER_H

#include <tt/ds/Image.h>

namespace tt
{

namespace process
{

class Bayer
{
public:
	enum Filter
	{
		NONE = 0,
		BayerBG2BGR = 46,
		BayerGB2BGR = 47,
		BayerRG2BGR = 48,
		BayerGR2BGR = 49,
		BayerBG2RGB = BayerRG2BGR,
		BayerGB2RGB = BayerGR2BGR,
		BayerRG2RGB = BayerBG2BGR,
		BayerGR2RGB = BayerGB2BGR
	};
	
public:
	static void deBayer(tt::ds::Image* source, tt::ds::Image* destination, Filter filter);
};

} // namespace process

} // namespace trax

#endif /*TRAX_PROCESS_BAYER_H*/
