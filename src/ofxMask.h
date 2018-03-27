#pragma once

#include "ofFbo.h"
#include "ofShader.h"

namespace ofx { namespace mask {
class Shader : public ofShader
{
public:
	void begin(const ofTexture &masker, const ofTexture &maskee);
};
class AlphaShader : public Shader
{
public:
	AlphaShader();
};
class LuminanceShader : public Shader
{
public:
	LuminanceShader();
};
class Fbo : public ofBaseDraws
{
public:
	enum Type
	{
		ALPHA,
		LUMINANCE
	};
	void allocate(const ofFbo::Settings &settings, Type type);
	void allocate(int width, int height, Type type);
	float getWidth() const { return fbo_.getWidth(); }
	float getHeight() const { return fbo_.getHeight(); }

	void beginMask(bool clear=true);
	void endMask();
	void clearMask();

	void begin(bool clear=true);
	void end();

	void draw() const;
	void draw(float x, float y) const;
	void draw(float x, float y, float w, float h) const;

	void drawMasker() const;
	void drawMasker(float x, float y) const;
	void drawMasker(float x, float y, float w, float h) const;
	void drawMaskee() const;
	void drawMaskee(float x, float y) const;
	void drawMaskee(float x, float y, float w, float h) const;
	
	ofTexture& getMaskerTexture() { return fbo_.getTexture(BufferIndex::MASKER); }
	ofTexture& getMaskeeTexture() { return fbo_.getTexture(BufferIndex::MASKEE); }
	const ofTexture& getMaskerTexture() const { return fbo_.getTexture(BufferIndex::MASKER); }
	const ofTexture& getMaskeeTexture() const { return fbo_.getTexture(BufferIndex::MASKEE); }
	
	// deprecated
	OF_DEPRECATED_MSG("Use allocate instead",void setup(int width, int height, Type type){
		allocate(width, height, type);
	});	
	
private:
	enum BufferIndex : int {
		MASKER = 0,
		MASKEE = 1
	};
	ofFbo fbo_;
	std::shared_ptr<Shader> shader_;
	float tex_coords_[8];
};
}}
using ofxMask = ofx::mask::Fbo;
using ofxMaskAlphaShader = ofx::mask::AlphaShader;
using ofxMaskLuminanceShader = ofx::mask::LuminanceShader;
/* EOF */
