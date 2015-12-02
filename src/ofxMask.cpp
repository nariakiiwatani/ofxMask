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
			masker_.allocate(width, height, GL_RGBA);
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
			masker_.allocate(width, height, GL_RGB);
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
	width_ = width;
	height_ = height;
	maskee_.allocate(width, height, GL_RGBA);
	makeTexCoords(tex_coords_, masker_.getTexture().getTextureData());
	makeVertices(vertices_, masker_.getTexture().getTextureData());
}

void ofxMask::beginMask(bool clear)
{
	masker_.begin();
	if(clear) {
		ofClear(0);
	}
}

void ofxMask::endMask()
{
	masker_.end();
}

void ofxMask::clearMask()
{
	masker_.begin();
	ofClear(0);
	masker_.end();
}

void ofxMask::begin(bool clear)
{
	maskee_.begin();
	if(clear) {
		ofClear(0);
	}
}

void ofxMask::end()
{
	maskee_.end();
}

void ofxMask::draw()
{
	ofPushStyle();
	glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	shader_.begin();
	shader_.setUniformTexture("masker", masker_, 0);
	shader_.setUniformTexture("maskee", maskee_, 1);
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
	glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	masker_.draw(0,0,masker_.getWidth(),masker_.getHeight());
	ofPopStyle();
}
void ofxMask::drawMaskee()
{
	ofPushStyle();
	glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	maskee_.draw(0,0,maskee_.getWidth(),maskee_.getHeight());
	ofPopStyle();
}

/* EOF */