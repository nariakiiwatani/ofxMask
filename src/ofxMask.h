#pragma once

#include "ofFbo.h"
#include "ofShader.h"

class ofxMask
{
public:
	void setup(int width, int height);

	void beginMask(bool clear=true);
	void endMask();
	void clearMask();

	void begin(bool clear=true);
	void end();

	void draw();

private:
	ofFbo masker_, maskee_;
	ofShader shader_;
	float vertices_[8];
	float tex_coords_[8];
};

/* EOF */