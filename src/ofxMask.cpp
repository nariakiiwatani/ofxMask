#include "ofxMask.h"
#include "ofGraphics.h"

namespace {
void makeVertices(float *dst, ofTextureData& texture_data)
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
void makeTexCoords(float *dst, ofTextureData& texture_data)
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
void ofxMask::setup(int width, int height, Type type)
{
#define _S(a) #a
	switch(type) {
		case ALPHA: {
			string shader_src = _S(
								   uniform sampler2DRect masker;
								   uniform sampler2DRect maskee;
								   void main()
								   {
									   gl_FragColor = texture2DRect(maskee, gl_TexCoord[0].st);
									   gl_FragColor.a *= texture2DRect(masker, gl_TexCoord[0].st).a;
								   }
								   );
			shader_.setupShaderFromSource(GL_FRAGMENT_SHADER, shader_src);
			shader_.linkProgram();
		}	break;
		case LUMINANCE: {
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
			shader_.setupShaderFromSource(GL_FRAGMENT_SHADER, shader_src);
			shader_.linkProgram();
		}	break;
	}
#undef _S
	ofFbo::Settings s;
	s.width = width;
	s.height = height;
	s.numColorbuffers = 2;
	s.colorFormats = {GL_RGBA, GL_RGBA};
	s.internalformat = GL_RGBA;
	fbo_.allocate(s);
	makeTexCoords(tex_coords_, fbo_.getTexture().getTextureData());
	makeVertices(vertices_, fbo_.getTexture().getTextureData());
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

void ofxMask::draw()
{
	ofPushStyle();
	glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	shader_.begin();
	shader_.setUniformTexture("masker", getMaskerTexture(), 0);
	shader_.setUniformTexture("maskee", getMaskeeTexture(), 1);
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords_ );
	glEnableClientState(GL_VERTEX_ARRAY);		
	glVertexPointer(2, GL_FLOAT, 0, vertices_ );
	glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	shader_.end();
	ofPopStyle();
}

void ofxMask::drawMasker()
{
	ofPushStyle();
	fbo_.setDefaultTextureIndex(BufferIndex::MASKER);
	glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	fbo_.draw(0,0,fbo_.getWidth(),fbo_.getHeight());
	ofPopStyle();
}
void ofxMask::drawMaskee()
{
	ofPushStyle();
	fbo_.setDefaultTextureIndex(BufferIndex::MASKEE);
	glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	fbo_.draw(0,0,fbo_.getWidth(),fbo_.getHeight());
	ofPopStyle();
}

/* EOF */