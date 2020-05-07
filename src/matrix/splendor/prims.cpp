#include"splendor\ddp.cpp"
#include"splendor\fonts.cpp"
#define up 0x48
#define down 0x50
#define left 0x50
#define right 0x50
#define esc 0x011b

void line(int x1,int y1,int x2,int y2,char c)
{
 int dx,dy,i,temp;
 //float xincr,yincr,steps,x,y;
 int xincr,yincr,steps,x,y;
 dx=x2-x1;
 dy=y2-y1;
 if(dy<0)temp=-dy;
 else temp=dy;
 if(dx>temp) steps=dx;
  else steps=temp;
 xincr=dx/steps;
 yincr=dy/steps;
 x=x1;
 y=y1;
 putpixel((int)x,(int)y,c);
 for(i=0;i<steps;i++)
  {
   x+=xincr;
   y+=yincr;
   putpixel((int)x,(int)y,c);
  }
}

void plotpoints(unsigned int xc,unsigned int yc,unsigned int x,unsigned int y,char c)
{
 putpixel(xc+x,yc+y,c);
 putpixel(xc-x,yc+y,c);
 putpixel(xc+x,yc-y,c);
 putpixel(xc-x,yc-y,c);
 putpixel(xc+y,yc+x,c);
 putpixel(xc-y,yc+x,c);
 putpixel(xc+y,yc-x,c);
 putpixel(xc-y,yc-x,c);
}
void plot(unsigned int xc,unsigned int yc,unsigned int x,unsigned int y,char c)
{
 line(xc-x,yc-y,xc+x,yc-y,c);
 line(xc-x,yc+y,xc+x,yc+y,c);
 line(xc-y,yc+x,xc+y,yc+x,c);
 line(xc-y,yc-x,xc+y,yc-x,c);
}

void circle(unsigned int xc,unsigned int yc,int r,unsigned char color)
{
 int p ;
 unsigned int x,y;
 p=1-r;
 x=0;y=r;
 plotpoints(xc,yc,x,y,color);
 while(x<=y)
  {
   if(p<0)
    {
     x++;
     p+=2*x+1;
    }
   else
    {
     x++;
     y--;
     p+=2*x-2*y+1;
    }
   plotpoints(xc,yc,x,y,color);
  }
}
void fill_circle(unsigned int xc,unsigned int yc,int r,unsigned char color)
{  	int p ;
	unsigned int x,y;
	p=1-r;
	x=0;y=r;
	plot(xc,yc,x,y,color);
	while(x<=y)
	{
		if(p<0)
		{
			x++;
			p+=2*x+1;
		}
		else
		{
			x++;
			y--;
			p+=2*x-2*y+1;
		}
		plot(xc,yc,x,y,color);
	}
}
void bar(int x,int y,int width,int height,int color)
{ long p=(long)(y)*xmax+(long)(x),t;
  char p2=p>>16;
  int i,j;
  long temp;
  if (p2!=bank) setbank(p2);
  p=p%max;
//  asm push es
  for(i=0;i<height;i++)
  {
	  for(j=0;j<width;j++)
	  {
		temp=0xA0000000l+p;
		*((char far *)temp)=color;
		if(p>=max-1){ p2++;setbank(p2);  p=0;}
		else
		p=p+1;
	  }
	   p=p+xmax-width;
	  if(p>=max-1) {p2++;setbank(p2);p=p%max;}
  }
 // asm pop es
}

void fill_par(int x,int y,int width,int height,int slide,int c)
{ long p=(long)(y)*xmax+(long)(x),t;
  char p2=p>>16,color;
  int i,j;
  long temp;
//  asm push es
  if (p2!=bank) setbank(p2);
  p=p%max;
  for(i=0;i<=height;i++)
  {
	  for(j=0;j<width;j++)
	  {     if(i==0||i==height) color=8;else color=c;
		if(j==0||j==width-1) color=8;
		temp=0xA0000000l+p;
		*((char far *)temp)=color;
		if(p>=max-1){ p2++;setbank(p2);  p=0;}
		else
		p=p+1;
	  }
	   p=p+xmax-width-slide;
	  if(p>=max-1) {p2++;setbank(p2);p=p%max;}
  }
//  asm pop es
}

void hline(int x,int y,int width,int color)
{ long p=(long)(y)*xmax+(long)(x),t;
  char p2=p>>16;
  long temp;
  int i,j;
//  asm push es
  if (p2!=bank) setbank(p2);
  for(j=0;j<width;j++)
  {
	p=p%max;
	temp=0xA0000000l+p;
	*((char far *)temp)=color;
	p++;
	if(p>=max) setbank(bank+1);
  }
//  asm pop es
}

void rectangle(int x,int y,int width,int height,int color)
{ long p=(long)(y)*xmax+(long)(x),t;
  char p2=p>>16;
  int i,j,inc,jmax,q;
  long temp;
  if (p2!=bank) setbank(p2);
  p=p%max;
//  asm push es
  for(i=0;i<height;i++)
  {     if(i==0||i==(height-1))
	{	inc=1;
		jmax=width;
	}
	else
	{	inc=width;
		jmax=2;
	}
	 for(j=0;j<jmax;j++)
	  {
		temp=0xA0000000l+p;
		*((char far *)temp)=color;
		p=p+inc;
		if(p>=max){p2++;setbank(p2);  p=p%max;}
	  }
	  p=p+xmax-width-inc;
	  if(p>=max-1) {p2++;setbank(p2);p=p%max;}
  }
//  asm pop es
}
void rectangle_shade(int x,int y,int width,int height,int color)
{ long p=(long)(y)*xmax+(long)(x),t;
  char p2=p>>16;
  long temp;
  int i,j,inc,jmax,q;
//  asm push es
  if (p2!=bank) setbank(p2);
  p=p%max;
  for(i=0;i<=height/2;i++)
  {     if(i==0||i==(height/2))
	{	inc=2;
		jmax=width/2;
	}
	else
	{	inc=width;
		jmax=2;
	}
	t=p;
	 for(j=0;j<jmax;j++)
	  {
	  temp=0xA0000000l+p;
	  *((char far *)temp)=color;
		p=p+inc;
		if(p>=max){p2++;setbank(p2);  p=p%max;}
	  }
		asm{    mov  ah,00h;
		int  16h
	   }
	  p=p+2*xmax-width-inc;
	  if(p>=max-1) {p2++;setbank(p2);p=p%max;}
  }
//  asm pop es
}

void vline(int x,int y,int height,int color)
{ long p=(long)(y)*xmax+(long)(x),t;
  char p2=p>>16;
  long temp;
  int j;
//  asm push es
  if (p2!=bank) setbank(p2);
  p=p%max;
  for(j=0;j<height;j++)
  {

	temp=0xA0000000l+p;
	*((char far *)temp)=color;
	p=p+xmax;
	if(p>=max) {p2++;setbank(p2);p=p%max;}
  }
//  asm pop es
}
char* To_String(long x)
{	int count=0;
	int i,j;
	static char a[10],b[10];
	while(x!=0)
	{       i=x%10;
		x=x/10;
		a[count++]='0'+i;
	}
	count--;
	for(j=0;count>=0;j++,count--)b[j]=a[count];
	b[j]='\0';
	return b;
}
