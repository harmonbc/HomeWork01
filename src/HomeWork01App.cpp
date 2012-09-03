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
	void fadingBackground(uint8_t* pixels);
	void drawInvertSquares(uint8_t* pixels);
	void drawInvertCircle(uint8_t* pixels, int center_x, int center_y, int r, Color8u c);
	void addTint(uint8_t* pixels, Color8u c);
	void addBlur(uint8_t* pixels);
	void hands(uint8_t* pixels);

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
	console() << event.getX()<<endl;
}

void HomeWork01App::drawInvertCircle(uint8_t* pixels, int center_x, int center_y, int r, Color8u c)
{
	if(r <= 0) return;

	for(int y=center_y-r; y<=center_y+r; y++){
		for(int x=center_x-r; x<=center_x+r; x++){


			if((y < 0 || x < 0 || x >= kAppWidth || y >= kAppHeight)) break;

			int dist = (int)sqrt((double)((x-center_x)*(x-center_x) + (y-center_y)*(y-center_y)));

			if(dist <= r)
			{

				int curBlock = 3*(x + y*kTextureSize);

				pixels[curBlock] = 255-pixels[curBlock];
				pixels[curBlock+1] = 255-pixels[curBlock+1];
				pixels[curBlock+2] = 255-pixels[curBlock+2];

			}
		}
	}


}

void HomeWork01App::addTint(uint8_t* pixels, Color8u c)
{
	Surface clonedSurface = (*mySurface_).clone();
	uint8_t* clonedPixels = clonedSurface.getData();

	int size =(kAppHeight+kAppWidth*kTextureSize);

	for(int y=0; y<=kAppHeight; y++){

		for(int x=0; x<=kAppWidth; x++){

			int curBlock = 3*(x + y*kTextureSize);

			pixels[curBlock] = clonedPixels[curBlock]/2+c.r/2;
			pixels[curBlock+1] = clonedPixels[curBlock+1]/2+c.g/2;
			pixels[curBlock+2] = clonedPixels[curBlock+2]/2+c.b/2;
		}
	}
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

void HomeWork01App::hands(uint8_t* pixles)
{

}
void HomeWork01App::update()
{
	uint8_t* dataArray = (*mySurface_).getData();
	Color8u fill1 = Color8u(128,128,192);
	Color8u tint = Color8u(255,0,0);

	fadingBackground(dataArray);
	drawInvertSquares(dataArray);
	drawInvertCircle(dataArray, kAppWidth/2, kAppHeight/2, 200, fill1);
	drawInvertCircle(dataArray, kAppWidth/2, kAppHeight/2, 5, fill1);
	addTint(dataArray, tint);
	frame_number_++;
}

void HomeWork01App::draw()
{
	// clear out the window with black
	gl::draw(*mySurface_); 
}

CINDER_APP_BASIC( HomeWork01App, RendererGl )
