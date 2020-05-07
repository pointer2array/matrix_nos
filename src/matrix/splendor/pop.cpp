#include"splendor\prims.cpp"
char *f1[]={"Create Folder","Properties","Exit"};
char *f2[]={"Create Folder","Open","Save As","Rename","Remove","Properties","Exit"};
char *e[]={"Shift","Copy","Paste"};
char *v[]={"Icons","List","Details","Go up","Settings"};
char *start[]={"Turn Off","Matrix Shell","PIC Viewer","Reader","Change Color","Set Resolution"};
      //
extern int Active_Count,Active_Position[30][2];
	//copied

void Store(int startx,int starty,int wd,int ht,char *buffer)
{ long p=(long)(starty)*xmax+(long)(startx),t;
  int i,j,k=0;
  char p2=p>>16;
  if (p2!=bank) setbank(p2);
  p=p%max;
  for(i=0;i<=ht;i++)
  {
	  for(j=0;j<wd;j++)
	  {
		buffer[k]=*((char far *)0xA0000000+p);
		k++;
		if(p>=max-1){ p2++;setbank(p2);  p=0;}
		else
		p=p+1;
	  }
	   p=p+xmax-wd;
	  if(p>=max-1) {p2++;setbank(p2);p=p%max;}
  }
}

void Restore(int startx,int starty,int wd,int ht,char *buffer)
{ long p=(long)(starty)*xmax+(long)(startx),t;
  int i,j,k=0;
  char p2=p>>16;
  if (p2!=bank) setbank(p2);
  p=p%max;

  for(i=0;i<=ht;i++)
  {
	  for(j=0;j<wd;j++)
	  {
		*((char far *)0xA0000000+p)=buffer[k];
		k++;
		if(p>=max-1){ p2++;setbank(p2);  p=0;}
		else
		p=p+1;
	  }
	   p=p+xmax-wd;
	  if(p>=max-1) {p2++;setbank(p2);p=p%max;}
  }

}
class Popupmenu
{       public :char *name;
		int count,flag,now,startx,starty,height,width;  // flag up/down popupmenu & now represents present highlighted position
		char **s;
		char bcolor,fcolor,hbcolor,hfcolor;
		Popupmenu(char,char);
		char Writebar();
		char Init();
};

Popupmenu::Popupmenu(char index,char select)
{   	bcolor=7;
	fcolor=0;
	hbcolor=1;
	hfcolor=15;
	flag=0;
	height=18;
	width=122;
	startx=Active_Position[index][0];
	starty=Active_Position[index][1];
	if(index==Active_Count+1)//file
	{
		if(select==0) //default
		{	s=f1;
			count=3;
		}
		else //file or folder
		{	s=f2;
			count=7;
		}
	}
	else  if(index==Active_Count+2)  //edit
	{       name="Edit";
		s=e;
		count=3;

	}

	else if (index==Active_Count+3)//view
	{       name="View";
		s=v;
		count=5;
	}
	else      //start
	{	name="TASKS";
		flag=1;
		s=start;
		count=6;
	}
}

char Popupmenu::Init()
{               char c,*buffer;
	int ht,wd,x=startx,y,i;
	 if(flag==0)
	{		ht=height*count+2;
			y=starty+29;
	}
	else
	{ 	ht=height*count+3;
		y=starty-ht;
	}
	i=wd*ht;
	buffer=new char[i];
	wd=width+2;
	Store(x,y,wd,ht,buffer);
	c=Writebar();
	Restore(x,y,wd,ht,buffer);
	delete buffer;
	return c;
}

char Popupmenu::Writebar()
{   int y,x=startx+1,now=0,f,i,temp,ht;
	char c1,c2,p,m,c;
	ht=height*count;
	load_font(3);
	if(flag==0)
	{	p=down;
		m=up;
		y=starty+30;
	}
	else
	{	p=up;
		m=down;
		y=starty-height;
	}
	bar(x,y,width,height,hbcolor);
	write(x+5,y+5,hfcolor,s[0]);
	for(i=1;i<count;i++)
	 {     if(flag==0)y+=height;else y-=height;
		bar(x,y,width,height,bcolor);
		write(x+5,y+5,fcolor,s[i]);
	 }
	 if(flag==0)
	 {	y=starty+30;
		rectangle(x,y-1,width+1,ht+2,8);
	 }
	 else
	 {
		rectangle(x,y-1,width+1,ht+2,8);
		y=starty-height;
	 }
	 while(1)
	 {
		asm{       mov ax,00h
			   int 16h
			   mov c1,ah
			   mov c2,al
			}
		temp=now;

		if(c1==0x01&&c2==0x1b){ c=-1;break;}
		else if(c1==0x1c&&c2==0x0d){c=now;break;}
		else
		{
		if(c1==p&&now<count-1)now++;
		else if(c1==m&&now>0) now--;
		bar(x,y,width,height,bcolor);
		write(x+5,y+5,fcolor,s[temp]);
		if(flag==0)y=starty+now*height+30;
		else y=starty-now*height-height;
		bar(x,y,width,height,hbcolor);
		write(x+5,y+5,hfcolor,s[now]);
		}
	 }
	 return c;
}
	//copied
