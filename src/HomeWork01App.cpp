#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "Resources.h"
#include <math.h>
#include <ctime>

using namespace ci;
using namespace ci::app;
using namespace std;

class HomeWork01App : public AppBasic {
public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* settings);
	void fadingBackground(uint8_t* pixels);
	void drawInvertSquares(uint8_t* pixels);
	void drawInvertCircle(uint8_t* pixels, int center_x, int center_y, int r);
	void drawCircle(uint8_t* pixels, int center_x, int center_y, int r, Color8u c);
	void drawLine(uint8_t* pixels, int startx, int starty, int length, Color8u c, int units, int maxUnits);
	void addTint(uint8_t* pixels, Color8u c);
	void addBlur(uint8_t* pixels);
	void hands(uint8_t* pixels);

private:
	Surface* mySurface_;
	int frame_number_;
	int kCircleRadius_;
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
	kCircleRadius_ = 200;
}

void HomeWork01App::mouseDown( MouseEvent event )
{
	console() << event.getX() << ", " << event.getY() <<endl;
}

void HomeWork01App::drawInvertCircle(uint8_t* pixels, int center_x, int center_y, int r)
{
	if(r <= 0) return;

	for(int y=center_y-r; y<=center_y+r; y++){
		for(int x=center_x-r; x<=center_x+r; x++){


			if((y < 0 || x < 0 || x >= kAppWidth || y >= kAppHeight)) break;

			int dist = (int)sqrt((double)((x-center_x)*(x-center_x) + (y-center_y)*(y-center_y)));

			if(dist <= r)
			{
				int curBlock = 3*(x + y*kTextureSize);

				//Invert color by 255-current pixels
				pixels[curBlock] = 255-pixels[curBlock];
				pixels[curBlock+1] = 255-pixels[curBlock+1];
				pixels[curBlock+2] = 255-pixels[curBlock+2];

			}
		}
	}
}
void HomeWork01App::drawCircle(uint8_t* pixels, int center_x, int center_y, int r, Color8u c)
{
	if(r <= 0) return;

	for(int y=center_y-r; y<=center_y+r; y++){
		for(int x=center_x-r; x<=center_x+r; x++){

			if(y < 0 || x < 0 || x >= kAppWidth || y >= kAppHeight) break;

			int dist = (int)sqrt((double)((x-center_x)*(x-center_x) + (y-center_y)*(y-center_y)));

			if(dist <= r)
			{
				int curBlock = 3*(x + y*kTextureSize);

				pixels[curBlock] = c.r;
				pixels[curBlock+1] = c.g;
				pixels[curBlock+2] = c.b;

			}
		}
	}
}

void HomeWork01App::addTint(uint8_t* pixels, Color8u c)
{
	Surface clonedSurface = (*mySurface_).clone();
	uint8_t* clonedPixels = clonedSurface.getData();

	int size =(kAppHeight+kAppWidth*kTextureSize);

	for(int y=0; y<=kAppHeight; y++)
	{
		for(int x=0; x<=kAppWidth; x++)
		{
			int curBlock = 3*(x + y*kTextureSize);

			pixels[curBlock] = clonedPixels[curBlock]/2+c.r/2;
			pixels[curBlock+1] = clonedPixels[curBlock+1]/2+c.g/2;
			pixels[curBlock+2] = clonedPixels[curBlock+2]/2+c.b/2;
		}
	}
}

void HomeWork01App::drawInvertSquares(uint8_t* pixels)
{
	//Draw a rectangle. I'll make this rectangle be the inverted color of the BG
	int x,y;

	for(y=30; y < kAppHeight-30; y++)
	{
		for(x=30; x < kAppWidth-30; x++)
		{
			pixels[3*(x+y*kTextureSize)]   = 255-pixels[3*(x+y*kTextureSize)];
			pixels[3*(x+y*kTextureSize)+1] = 255-pixels[3*(x+y*kTextureSize)+1];
			pixels[3*(x+y*kTextureSize)+2] = 255-pixels[3*(x+y*kTextureSize)+2];
		}
	}
}

/**
*Gradient A.4
**/
void HomeWork01App::fadingBackground(uint8_t* pixels)
{
	int32_t something = (*mySurface_).getWidth()*(*mySurface_).getHeight();
	int y,x;

	for(y=0; y<kAppHeight; y++)
	{
		for(x=0; x<kAppWidth; x++)
		{
			pixels[3*(x+y*kTextureSize)]   = abs(sin(frame_number_*.03f))*255;
			pixels[3*(x+y*kTextureSize)+1] = abs(sin(frame_number_*.05f))*255;
			pixels[3*(x+y*kTextureSize)+2] = abs(sin(frame_number_*.04f))*255;
		}
	}
}

void HomeWork01App::hands(uint8_t* pixels)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);

	//get current time
	int hours = ltm->tm_hour;
	int mins = ltm->tm_min;
	int seconds = ltm->tm_sec;
	if(hours>12)
		hours -= 12;
	//set hands length
	int minHand = kCircleRadius_;
	int hourHand = minHand/2;
	int secHand = (minHand+hourHand)/2;

	//Set Center Point
	int centerX = (kAppWidth/2);
	int centerY = (kAppHeight/2);

	//drawLine(pixels, centerX, centerY, minHand, Color8u(0,0,0), mins, 60);
	//drawLine(pixels, centerX, centerY, hourHand, Color8u(0,0,0), hours, 12);
	drawLine(pixels, centerX, centerY, secHand, Color8u(0,0,0), seconds, 60);

}

void HomeWork01App::drawLine(uint8_t* pixels, int startx, int starty, int length, Color8u c, int units, int maxUnits)
{
	double slope = units/maxUnits;

	int flipUnit = (maxUnits == 60) ? 30 : 6;

	bool rightSegment = units<maxUnits;

	for(int x=startx-length; x<startx+length; x++)
	{
		for(int y=starty-length; y<starty+length; y++)
		{
			double denominator = x-startx;
			double currentSlope = y-starty;

			currentSlope = (denominator!=0) ? currentSlope/denominator : 0;

			if(abs (currentSlope-slope) <.01)
			{
				if((rightSegment&&(x-startx>=0))||(!rightSegment&&(x-startx<=0)))
				{
					double distance = sqrt(pow(x-startx,2.0)-pow(y-starty,2.0))<=length;

					if(true)//distance <=length)
					{
						pixels[3*(x+y*kTextureSize)]   = c.r;
						pixels[3*(x+y*kTextureSize)+1] = c.g;
						pixels[3*(x+y*kTextureSize)+2] = c.b;
					}
				}
			}
		}
	}
}

void HomeWork01App::update()
{
	uint8_t* dataArray = (*mySurface_).getData();
	Color8u fill1 = Color8u(128,128,192);
	Color8u tint = Color8u(255,0,0);

	fadingBackground(dataArray);
	drawInvertSquares(dataArray);
	drawInvertCircle(dataArray, kAppWidth/2, kAppHeight/2, kCircleRadius_);
	addTint(dataArray, tint);
	hands(dataArray);
	frame_number_++;
}

void HomeWork01App::draw()
{
	// clear out the window with black
	gl::draw(*mySurface_); 
}

CINDER_APP_BASIC( HomeWork01App, RendererGl )
