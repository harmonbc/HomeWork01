#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "Resources.h"
#include <math.h>

using namespace ci;
using namespace ci::app;
using namespace std;

#define PI 3.14159265

class HomeWork01App : public AppBasic {
public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* settings);
	void fadingBackground(uint8_t* pixlie);
	void drawInvertSquares(uint8_t* pixles);
	void drawInvertCircle(uint8_t* pixles, int r);
private:
	Surface* mySurface_;
	int frame_number_;
};
static const int kAppWidth=800;
static const int kAppHeight=600;
static const int kTextureSize=1024;

void HomeWork01App::prepareSettings(Settings* settings)
{
	(*settings).setWindowSize(kAppWidth,kAppHeight);
	(*settings).setResizable(false);
}

void HomeWork01App::setup()
{
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
	frame_number_ = 0;
}

void HomeWork01App::mouseDown( MouseEvent event )
{
}

void HomeWork01App::drawInvertCircle(uint8_t* pixles, int r)
{
	if(r<=0) return;

	int r2 = r*r;

}

void HomeWork01App::drawInvertSquares(uint8_t* pixles)
{
	int32_t something = (*mySurface_).getWidth()*(*mySurface_).getHeight();

	//Draw a rectangle. I'll make this rectangle be the inverted color of the BG
	int x,y;

	for(y=30; y < kAppHeight-30; y++)
	{
		for(x=30; x < kAppWidth-30; x++)
		{
			pixles[3*(x+y*kTextureSize)]   = 255-pixles[3*(x+y*kTextureSize)];
			pixles[3*(x+y*kTextureSize)+1] = 255-pixles[3*(x+y*kTextureSize)+1];
			pixles[3*(x+y*kTextureSize)+2] = 255-pixles[3*(x+y*kTextureSize)+2];
		}
	}
}
/**
*Gradient A.4
**/
void HomeWork01App::fadingBackground(uint8_t* pixles)
{
	int32_t something = (*mySurface_).getWidth()*(*mySurface_).getHeight();
	int y,x;

	for(y=0; y<kAppHeight; y++)
	{
		for(x=0; x<kAppWidth; x++)
		{
			pixles[3*(x+y*kTextureSize)]   = abs(sin(frame_number_*.03f))*255;
			pixles[3*(x+y*kTextureSize)+1] = abs(sin(frame_number_*.05f))*255;
			pixles[3*(x+y*kTextureSize)+2] = abs(sin(frame_number_*.04f))*255;
		}
	}
}

void HomeWork01App::update()
{
	uint8_t* dataArray = (*mySurface_).getData();
	Color8u fill1 = Color8u(128,128,192);

	fadingBackground(dataArray);
	drawInvertSquares(dataArray);

	frame_number_++;
}

void HomeWork01App::draw()
{
	// clear out the window with black
	gl::draw(*mySurface_); 
}

CINDER_APP_BASIC( HomeWork01App, RendererGl )
