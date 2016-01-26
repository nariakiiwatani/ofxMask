#pragma once

#include "ofFbo.h"
#include "ofShader.h"

class ofxMask
{
public:
	enum Type
	{
		ALPHA,
		LUMINANCE
	};
	void allocate(int width, int height, Type type);
	void setup(int width, int height, Type type);
	int getWidth() { return fbo_.getWidth(); }
	int getHeight() { return fbo_.getHeight(); }

	void beginMask(bool clear=true);
	void endMask();
	void clearMask();

	void begin(bool clear=true);
	void end();

	void draw();
	
	void drawMasker();
	void drawMaskee();

	ofTexture& getMaskerTexture() { return fbo_.getTexture(BufferIndex::MASKER); }
	ofTexture& getMaskeeTexture() { return fbo_.getTexture(BufferIndex::MASKEE); }

private:
	enum BufferIndex : int {
		MASKER = 0,
		MASKEE = 1
	};
	ofFbo fbo_;
	ofShader shader_;
	float vertices_[8];
	float tex_coords_[8];
};

/* EOF */