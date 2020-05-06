

#ifndef _I915_COMPONENT_H_
#define _I915_COMPONENT_H_

#include "drm_audio_component.h"

enum i915_component_type {
	I915_COMPONENT_AUDIO = 1,
	I915_COMPONENT_HDCP,
};


#define MAX_PORTS 6


struct i915_audio_component {
	
	struct drm_audio_component	base;

	
	int aud_sample_rate[MAX_PORTS];
};

#endif 
