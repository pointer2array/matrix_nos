enum type{folder,file,floppy,drive,bmp,txt,unknown,system};
enum select_type{goup=-7,exit,reader,bmpviewer,find,matrixshell,turnoff};
//Global properties of active object//
int Active_Position[30][2];
char Active_List[30][9];
type Active_Type[30];
int Active_Count;
char Active_Column[5];
//

long max=65536,xmax=640,ymax=480,opt;
int mode=0x0101,bankmax=0,bank=0;
int winy;
/*char keyhit()
{
asm mov ah,0x01
asm int 0x16
asm jz re
return 0;
re:
return 1;
} */
void opensvga(int m)
{
	asm {
	       //push es
	       mov ax,012h
	       int 10h
	       mov ax,0x4F02;
	       mov bx,m;
	       int 10h
	       //pop es
	    }
}
void init(int m)
{	switch(m)
	{	case 1: xmax=800;
			ymax=600;
			mode=0x0103;
			break;
		case 2: xmax=1024;
			ymax=768;
			mode=0x0105;
			break;
		default:xmax=640;
			ymax=480;
			mode=0x0101;
			break;
	}
	bankmax=(unsigned int)(xmax*ymax/max)+1;
	winy=ymax-30;
	opt=(xmax*ymax)-(max*(bank-1));
	opensvga(mode);
}

void setbank(int b)
{   asm{
	//push es
	mov ax,0x4F05;
	mov bx,0x00;
	mov dx,b;
	int 10h;
	//pop es
    }
    bank=b;
}

void closesvga(void)
{
  asm {
	 //push es
	 mov ax,3;
	 int 10h;
	 //pop es
      }
}

void putpixel(unsigned int x,unsigned int y,unsigned char color)
{
  long p=y*xmax+x;
  long temp;
  char p2=(char)(p>>16);
  //asm push es
  setbank(p2);
  p=p%max;
  temp = 0xA0000000l + p;
  //*((char far *)0xA0000000+p)=color;
  *((char far *)(temp))=color;
  //asm pop es
}

void  Paint(char c)
{     long j,jmax=max;
	unsigned long temp;
      unsigned int i;
      //asm push es
      for(i=0;i<bankmax;i++)
	{
		setbank(i);
		if(i==bankmax-1)  jmax=opt;
		for(j=0;j<jmax;j++)
		{
		 temp = 0xA0000000l + j;
		 *((char far *)(temp))=c;
		    //	*((char far *)0xA0000000+j)=c;
			}
	}
    //asm pop es
}

void  Shade()
{     long j,jmax=max,temp;
      int i;
      for(i=0;i<bankmax;i++)
	{
		setbank(i);
		if(i==bankmax-1)  jmax=opt;
		for(j=1;j<jmax;j++)
		{       temp = 0xA0000000l + j;
			*((char far *)(temp))=0;
			j++;
		}

	}

}
unsigned char getpixel(unsigned int x,unsigned int y)
{
	long p=(long)(y)*xmax+(long)(x);
	unsigned long temp;
	unsigned char   m;
	char p2=(char)p>>16;
	//asm push es
	setbank(p2);
	p=p%max;
	temp = 0xA0000000l + p;
	m=*((char far *)temp);
	//asm pop es
	return m;
}

