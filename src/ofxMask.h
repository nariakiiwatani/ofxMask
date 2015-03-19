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
	void setup(int width, int height, Type type);
	int getWidth() { return width_; }
	int getHeight() { return height_; }

	void beginMask(bool clear=true);
	void endMask();
	void clearMask();

	void begin(bool clear=true);
	void end();

	void draw();
	
	void drawMasker();
	void drawMaskee();

	void getMaskerTextureReeference() { return masker_.getTextureReference(); }
	void getMaskeeTextureReeference() { return maskee_.getTextureReference(); }

private:
	ofFbo masker_, maskee_;
	ofShader shader_;
	float vertices_[8];
	float tex_coords_[8];
	float width_, height_;
};

/* EOF */