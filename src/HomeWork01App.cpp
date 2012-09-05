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

	void drawSquare(uint8_t* pixels, int side, Color8u c);
	void drawInvertCircle(uint8_t* pixels, int center_x, int center_y, int r);
	void drawCircle(uint8_t* pixels, int center_x, int center_y, int r, Color8u c);
	void addTint(uint8_t* pixels, Color8u c);
	void drawLine(uint8_t* pixels,int x1, int x2, int y1, int y2, int width);
	void plot(uint8_t* pixels,int x, int y, int width);
	void blur(uint8_t* pixels);

private:
	Surface* mySurface_;
	int frame_number_;
	int hand_position;
	int kCircleRadius_;
	bool firstClick;
	bool secondClick;
	int xOne;
	int xTwo;
	int yOne;
	int yTwo;
	int circle_x;
	int circle_y;
	int circle2_x;

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
	firstClick = false;
	secondClick = false;
	circle_x = 15;
	circle_y = 15;
	circle2_x = 0;
}

/**
 *Satisfies A.6 mouse interaction
 **/
void HomeWork01App::mouseDown( MouseEvent event )
{
	if(firstClick && secondClick)
	{
		firstClick = false;
		secondClick = false;
	}
	if(!firstClick)
	{
		firstClick = true;
		xOne = event.getX();
		yOne = event.getY();
	}else
	{
		secondClick = true;
		xTwo = event.getX();
		yTwo = event.getY();
	}
}

/**
 *Satisfies A.2
 **/
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
/**
 *Satisfies A.2
 **/
void HomeWork01App::drawCircle(uint8_t* pixels, int center_x, int center_y, int r, Color8u c)
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
				pixels[3*(x+y*kTextureSize)]   = c.r;
				pixels[3*(x+y*kTextureSize)+1] = c.g;
				pixels[3*(x+y*kTextureSize)+2] = c.b;

			}
		}
	}
}

/**
 * Satisfies A.6, but is not implemented
 **/
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

/**
 *Satisfies A.1
 **/
void HomeWork01App::drawSquare(uint8_t* pixels, int side, Color8u c)
{
	//Draw a rectangle. I'll make this rectangle be the inverted color of the BG
	int x,y;

	for(y=kAppHeight/2-side; y < kAppHeight/2+side; y++)
	{
		for(x=kAppWidth/2-side; x < kAppWidth/2+side; x++)
		{
			if(!(x<0||y<0||x>kAppWidth||y>kAppHeight))
			{

			int offset = 3*(x+y*kTextureSize);

			pixels[offset]   = c.r;
			pixels[offset+1] = c.g;
			pixels[offset+2] = c.b;
			}
		}
	}
}


/**
 *Retrived sudo code from wiki - http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 *Satisfies A.3
 **/
void HomeWork01App::drawLine(uint8_t* pixels, int x0, int x1, int y0, int y1, int width)
{
	if(x0<0||y0<0||y1<0||x1<0||x0>kAppHeight||x1>kAppHeight||y1>kAppHeight||y0>kAppHeight||
		x0>kAppWidth||x1>kAppWidth||y0>kAppWidth||y1>kAppWidth) return;
	bool steep = abs(y1-y0) > abs(x1-x0);

	if(steep)
	{
		swap(x0, y0);
		swap(x1, y1);
	}

	if(x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = abs(y1-y0);
	double error = dx/2;

	int ystep;

	int y = y0;

	ystep = (y0<y1) ? 1 : -1;

	for(int x=x0; x<x1; x++)
	{
		int dist = (int)sqrt((double)((x0-x)*(x0-x)+(y0-y)*(y0-y)));


		(steep) ? plot(pixels, y,x, 5) : plot(pixels, x,y, 5);
		error = error - dy;

		if(error<0)
		{
			y = y + ystep;
			error = error + dx;
		}
	}

}
/**
 *Used for the line plotting
 **/
void HomeWork01App::plot(uint8_t* pixels, int x, int y, int width)
{
	//Cannot be an even number or lessthan 3
	int halfWidth = (width<3) ? 1 : width/2;

	int op1 = (width<3) ? 1 : (x-width);
	int op2 = (width<3) ? 1 : (x+width);

	for( x = op1 ; x < op2; x++)
	{
		int offset = 3*(x+y*kTextureSize);
		pixels[offset] = 0;
		pixels[offset+1] = 0;
		pixels[offset+2] = 0;
	}
}

/**
 *Satisfies B.1
 **/
void HomeWork01App::blur(uint8_t* pixels)
{
	int x,y,p,q;
	Surface clone = (*mySurface_).clone();
	uint8_t* cloneP = clone.getData();

	for(y = 1; y < kAppHeight; y++)
	{
		for(x = 1;  x < kAppWidth; x++)
		{
			int offset = 3*(x+y*kTextureSize);

			pixels[offset] = pixels[offset]/9;
			pixels[offset+1] = pixels[offset+1]/9;
			pixels[offset+2] = pixels[offset+2]/9;
			for(p = y-1; p < y+1; p++)
			{
				for(q = x-1; q<x+1; q++)
				{
					int offset2 = 3*(q+p*kTextureSize);

					pixels[offset] += cloneP[offset2]/9;
					pixels[offset+1] +=cloneP[offset2+1]/9;
					pixels[offset+2] +=cloneP[offset2+2]/9;
				}
			}

		}
	}
}

void HomeWork01App::update()
{
	uint8_t* dataArray = (*mySurface_).getData();
	Color8u fill1 = Color8u(128,128,192);

	Color8u face = Color8u(0,0,0);
	Color8u white = Color8u(255,255,254);
	drawSquare(dataArray, kAppHeight, white);
	drawSquare(dataArray, (kAppWidth<kAppHeight) ? kAppWidth/2-30 : kAppHeight/2-30, fill1);
	drawInvertCircle(dataArray, kAppWidth/2, kAppHeight/2, kCircleRadius_);
	/**
	 *The following method calls just make a simple face
	 **/
	drawCircle(dataArray, kAppWidth/2-100, kAppHeight/2-50, 10, face);
	drawCircle(dataArray, kAppWidth/2+100, kAppHeight/2-50, 10, face);
	drawLine(dataArray, kAppWidth/2-150, kAppWidth/2+150, kAppHeight/2+30, kAppHeight/2+80, 5);
	/**
	 *The Following 2 method calls move a ball around the screen
	 **/
	drawInvertCircle(dataArray, circle_x, circle_y, 15);
	drawCircle(dataArray, circle2_x, (sin(frame_number_+.0)*60)+500, 10, face);

	blur(dataArray);
	/**
	*tint works, but I don't like it and did A.1, A.2, A.3, A.4, so it is not needed
	**/
	//addTint(dataArray, tint);
	if(circle2_x<=kAppWidth-15)
	{
		circle2_x+=10;
	}else{

		circle2_x=15;
	}
	if(firstClick&&secondClick)
	{
		drawLine(dataArray,xOne, xTwo, yOne, yTwo, 5);
	}
	if(circle_y==15&&circle_x!=kAppWidth-15)
	{
		circle_x+=10;
	}else if(circle_x==kAppWidth-15&&circle_y!=kAppHeight-15)
	{
		circle_y+=10;
	}else if(circle_y==kAppHeight-15&&circle_x!=15)
	{
		circle_x-=10;
	}else{
		circle_y-=10;
	}

	frame_number_++;
}

void HomeWork01App::draw()
{
	gl::draw(*mySurface_); 
}

CINDER_APP_BASIC( HomeWork01App, RendererGl )
