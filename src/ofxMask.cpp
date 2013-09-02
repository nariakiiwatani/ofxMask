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
	dst[2] =
	dst[4] = texture_data.tex_t;
	dst[5] =
	dst[7] = texture_data.tex_u;
}
}
void ofxMask::setup(int width, int height)
{
	bool arb = ofGetUsingArbTex();
	if(arb) {
		ofDisableArbTex();
	}
	masker_.allocate(width, height, GL_RGB);
	maskee_.allocate(width, height, GL_RGBA);
	if(arb) {
		ofEnableArbTex();
	}
	makeTexCoords(tex_coords_, masker_.getTextureReference().getTextureData());
	makeVertices(vertices_, masker_.getTextureReference().getTextureData());
	string shader_src = 
	"uniform sampler2D masker;"
	"uniform sampler2D maskee;"
	"void main()"
	"{"
	"	gl_FragColor = texture2D(maskee, gl_TexCoord[0].st);"
	"	gl_FragColor.a *= texture2D(masker, gl_TexCoord[0].st).r;"
	"}";
	shader_.setupShaderFromSource(GL_FRAGMENT_SHADER, shader_src);
	shader_.linkProgram();
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
}

void ofxMask::drawMasker()
{
	masker_.draw(0,0,masker_.getWidth(),masker_.getHeight());
}
void ofxMask::drawMaskee()
{
	maskee_.draw(0,0,maskee_.getWidth(),maskee_.getHeight());
}

/* EOF */