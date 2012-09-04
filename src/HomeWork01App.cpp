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
	void drawInvertSquares(uint8_t* pixels, int side);
	void drawInvertCircle(uint8_t* pixels, int center_x, int center_y, int r);
	void addTint(uint8_t* pixels, Color8u c);
	void addBlur(uint8_t* pixels);
	void spinningHands(uint8_t* pixels);
	void drawLine(int x1, int x2, int y1, int y2);

private:
	Surface* mySurface_;
	int frame_number_;
	int hand_position;
	int kCircleRadius_;
	bool rotate_clockwise_;
	double rotate_pos_;
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
	rotate_pos_ = 1;
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
				pixels[3*(x+y*kTextureSize)]   = abs(sin(frame_number_*.03f))*255;
				pixels[3*(x+y*kTextureSize)+1] = abs(sin(frame_number_*.05f))*255;
				pixels[3*(x+y*kTextureSize)+2] = abs(sin(frame_number_*.04f))*255;

			}
		}
	}
}

void HomeWork01App::addTint(uint8_t* pixels, Color8u c)
{


	int size =(kAppHeight+kAppWidth*kTextureSize);

	for(int y=0; y<=kAppHeight; y++)
	{
		for(int x=0; x<=kAppWidth; x++)
		{
			int curBlock = 3*(x + y*kTextureSize);

			pixels[curBlock] = pixels[curBlock]/2+c.r/2;
			pixels[curBlock+1] = pixels[curBlock+1]/2+c.g/2;
			pixels[curBlock+2] = pixels[curBlock+2]/2+c.b/2;
		}
	}
}

void HomeWork01App::drawInvertSquares(uint8_t* pixels, int side)
{
	//Draw a rectangle. I'll make this rectangle be the inverted color of the BG
	int x,y;

	for(y=kAppHeight/2-side; y < kAppHeight/2+side; y++)
	{
		for(x=kAppWidth/2-side; x < kAppWidth/2+side; x++)
		{
			int offset = 3*(x+y*kTextureSize);
			pixels[offset]   = 255-pixels[offset];
			pixels[offset+1] = 255-pixels[offset+1];
			pixels[offset+2] = 255-pixels[offset+2];
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

		}
	}
}

void HomeWork01App::spinningHands(uint8_t* pixels)
{
	int spinnerLength = kCircleRadius_;

	time_t now = time(0);

	tm *ltm = localtime(&now);

	rotate_pos_ = (ltm->tm_sec/30.0)+.25;

	console() << rotate_pos_ << endl;
	int startx = kAppWidth/2;
	int starty = kAppHeight/2;

		for(int y=starty-spinnerLength; y<starty+spinnerLength; y++)
	{
		for(int x=startx-spinnerLength; x<startx+spinnerLength; x++)
		{
			double denominator = x-startx;
			double currentSlope = y-starty;

			int dist = (int)sqrt((double)(denominator*denominator+currentSlope*currentSlope));

			if(dist<=spinnerLength)
			{
				currentSlope =  currentSlope/denominator;

				if(abs(currentSlope-rotate_pos_)<.1)
				{
					int curBlock = 3*(x + y*kTextureSize);
					Color8u c = Color8u(255,255,255);

					pixels[curBlock] = c.r;
					pixels[curBlock+1] = c.g;
					pixels[curBlock+2] = c.b;
				}

			}
		}
	}
}
/**
 *Retrived sudo code from wiki - http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 **/
void HomeWork01App::drawLine(int x1, int x2, int y1, int y2)
{
     bool steep = abs(y2 - y1) > abs(x2 - x1);

     if(steep)
	 {
         swap(x2, y2);
         swap(x1, y1);
	 }

     if (x2 > x1)
	 {
         swap(x2, x1);
         swap(y2, y1);
	 }

     int deltax = x2 - x1;
     int deltay = abs(y2 - y1);
     int error = deltax/2;
     
     int ystep;
     int y = y1;

     ystep = (y1 < y2) ? 1 : -1;

     for (int x=x1; x<x2; x++)
	 {
         (steep) ? plot(y,x) : plot(x,y);

         error = error + deltaerr;
         if (error >= 0.5) then
             y := y + ystep
error := error - 1.0
	 }
}
void plot(int x, int y)
{

}

void HomeWork01App::update()
{
	uint8_t* dataArray = (*mySurface_).getData();
	Color8u fill1 = Color8u(128,128,192);
	Color8u tint = Color8u(255,0,0);

	fadingBackground(dataArray);
	drawInvertSquares(dataArray, kCircleRadius_);
	drawInvertCircle(dataArray, kAppWidth/2, kAppHeight/2, kCircleRadius_);
	addTint(dataArray, tint);
	spinningHands(dataArray);
	frame_number_++;
}

void HomeWork01App::draw()
{
	// clear out the window with black
	gl::draw(*mySurface_); 
}

CINDER_APP_BASIC( HomeWork01App, RendererGl )
