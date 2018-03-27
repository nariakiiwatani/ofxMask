#include "ofxMask.h"
#include "ofGraphics.h"

using namespace std;
using namespace ofx::mask;

void Shader::begin(const ofTexture &masker, const ofTexture &maskee)
{
	ofShader::begin();
	setUniformTexture("masker", masker, 0);
	setUniformTexture("maskee", maskee, 1);
}
AlphaShader::AlphaShader()
{
#define _S(a) #a
	string shader_src = _S(
						   uniform sampler2DRect masker;
						   uniform sampler2DRect maskee;
						   void main()
						   {
							   gl_FragColor = texture2DRect(maskee, gl_TexCoord[0].st);
							   gl_FragColor.a *= texture2DRect(masker, gl_TexCoord[0].st).a;
						   }
						   );
#undef _S
	setupShaderFromSource(GL_FRAGMENT_SHADER, shader_src);
	linkProgram();
}
LuminanceShader::LuminanceShader()
{
#define _S(a) #a
	string shader_src = _S(
						   uniform sampler2DRect masker;
						   uniform sampler2DRect maskee;
						   void main()
						   {
							   gl_FragColor = texture2DRect(maskee, gl_TexCoord[0].st);
							   vec4 rgb = texture2DRect(masker, gl_TexCoord[0].st);
							   gl_FragColor.a *= 0.298912*rgb.r + 0.586611*rgb.g + 0.114478*rgb.b;
						   }
						   );
#undef _S
	setupShaderFromSource(GL_FRAGMENT_SHADER, shader_src);
	linkProgram();
}
namespace {
void makeVertices(float *dst, const ofTextureData& texture_data) 
{
	dst[0] =
	dst[1] =
	dst[3] =
	dst[6] = 0;
	dst[2] =
	dst[4] = texture_data.width;
	dst[5] =
	dst[7] = texture_data.height;
}
void makeVertices(float *dst, float x, float y, float w, float h) 
{
	dst[0] =
	dst[6] = x;
	dst[1] =
	dst[3] = y;
	dst[2] =
	dst[4] = w;
	dst[5] =
	dst[7] = h;
}
void makeTexCoords(float *dst, const ofTextureData& texture_data)
{
	dst[0] =
	dst[1] =
	dst[3] =
	dst[6] = 0;
#ifndef TARGET_OPENGLES
	if( texture_data.textureTarget==GL_TEXTURE_RECTANGLE_ARB && ofGLSupportsNPOTTextures() ){
		dst[2] =
		dst[4] = texture_data.width;
		dst[5] =
		dst[7] = texture_data.height;
	}else
#endif
	{
		dst[2] =
		dst[4] = texture_data.tex_t;
		dst[5] =
		dst[7] = texture_data.tex_u;
	}
}
}

void ofxMask::allocate(const ofFbo::Settings &settings, Type type)
{
	fbo_.allocate(settings);
	switch(type) {
		case ALPHA:		shader_ = make_shared<AlphaShader>();		break;
		case LUMINANCE:	shader_ = make_shared<LuminanceShader>();	break;
		default:
			ofLogError("ofxMask allocate failed.");
			break;
	}
	makeTexCoords(tex_coords_, fbo_.getTexture().getTextureData());
}

void ofxMask::allocate(int width, int height, Type type)
{
	ofFbo::Settings s;
	s.width = width;
	s.height = height;
	s.numColorbuffers = 2;
	s.colorFormats = {GL_RGBA, GL_RGBA};
	s.internalformat = GL_RGBA;
	allocate(s, type);
}

void ofxMask::beginMask(bool clear)
{
	fbo_.begin();
	fbo_.setActiveDrawBuffer(BufferIndex::MASKER);
	if(clear) {
		ofClear(0);
	}
}

void ofxMask::endMask()
{
	fbo_.end();
}

void ofxMask::clearMask()
{
	fbo_.begin();
	fbo_.setActiveDrawBuffer(BufferIndex::MASKER);
	ofClear(0);
	fbo_.end();
}

void ofxMask::begin(bool clear)
{
	fbo_.begin();
	fbo_.setActiveDrawBuffer(BufferIndex::MASKEE);
	if(clear) {
		ofClear(0);
	}
}

void ofxMask::end()
{
	fbo_.end();
}

void ofxMask::draw() const
{
	draw(0,0);
}

void ofxMask::draw(float x, float y) const
{
	draw(x,y,getWidth(),getHeight());
}
void ofxMask::draw(float x, float y, float w, float h) const
{
	float vertices[8];
	makeVertices(vertices, x,y,w,h);
	ofPushStyle();
	glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	shader_->begin(getMaskerTexture(), getMaskeeTexture());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords_);
	glEnableClientState(GL_VERTEX_ARRAY);		
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	shader_->end();
	ofPopStyle();
}


void ofxMask::drawMasker() const
{
	drawMasker(0,0);
}
void ofxMask::drawMasker(float x, float y) const
{
	drawMasker(x,y,getWidth(),getHeight());
}
void ofxMask::drawMasker(float x, float y, float w, float h) const
{
	ofPushStyle();
	glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	getMaskerTexture().draw(x,y,w,h);
	ofPopStyle();
}
void ofxMask::drawMaskee() const
{
	drawMaskee(0,0);
}
void ofxMask::drawMaskee(float x, float y) const
{
	drawMaskee(x,y,getWidth(),getHeight());
}
void ofxMask::drawMaskee(float x, float y, float w, float h) const
{
	ofPushStyle();
	glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	getMaskeeTexture().draw(x,y,w,h);
	ofPopStyle();
}

/* EOF */
