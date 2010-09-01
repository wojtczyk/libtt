#ifndef TRAX_OUTPUT_MOVIERECORDER_H
#define TRAX_OUTPUT_MOVIERECORDER_H

#include "OutputDevice.h"

namespace tt
{

namespace output
{

class MovieRecorder : public tt::output::OutputDevice
{
public:
	MovieRecorder();
	virtual ~MovieRecorder();
};

} // namespace output

} // namespace trax

#endif /*TRAX_OUTPUT_MOVIERECORDER_H*/
