#include"splendor\pop.cpp"
#include"splendor\copy.cpp"
#include"shell.cpp"
char curr_directory[50],clipboard[50]={'0'};
Directory curry_dir("C:\\DESKTOP");
extern char Desktop_Color,Resolution;
struct win_properties
{	char fcolor,bcolor;
}win;
int tempx;
void Button(int x,int y,char press)
{       char c1,c2,c,fc;
	int width=70,height=35;
	if(!press)
	{	c1=0;c2=15;
		c=8;fc=15;
	}
	else
	{   c1=15;c2=0;c=7;fc=0;}
	bar(x,y,width,height,c);
	vline(x,y,height,c2);
	hline(x,y,width,c2);
	vline(x+width,y,height,c1);
	hline(x,y+height,width,c1);
}
void Message_Window(int x,int y,int wd,int ht,char *name)
{   int tht=30;
	bar(x,y,wd,tht,1);      //titlebar
	bar(x,y,wd,5,7); //border of titlebar
	hline(x,y+2,wd,15);
	hline(x,y+3,wd,0);
   //	load_font(1);
	write(x+11,y+11,15,name);      //address
 //	load_font(3);
	bar(x,y+tht,wd,ht-(tht+1),28);        //body
	hline(x,y+tht,wd,15);
	bar(x,y+ht-5,wd,5,7); //bar after body
	hline(x,y+ht-2,wd,0);
	hline(x,y+ht-4,wd,15);
	// vlines of window border
	bar(x,y,5,ht,7); //border of titlebar
	vline(x+1,y+2,ht-2,15);
	vline(x+3,y+3,ht-3,0);
	bar(x+wd-5,y,5,ht,7); //border of titlebar
	vline(x+wd-4,y+2,ht-2,0);
	vline(x+wd-2,y+3,ht-3,15);

}
void Error(char *mess)
{
	       int x=xmax/2-150,y=ymax/2-100,ht=120,wd=400,i=0;
		char w[50]={'e'},c1,c2;
		Message_Window(x,y,wd,ht,"OPEN");
		write(x+40,y+50,0,mess);
		asm{ mov ax,0h;
		int 16h;
		}

}
char* Get_File()
{       int x=xmax/2-150,y=ymax/2-100,ht=120,wd=400,startx,starty,i=0;
	static char w[50]={'e'},c1,c2;
	Message_Window(x,y,wd,ht,"OPEN");
	write(x+40,y+50,0,"L o c a t i o n :");
	bar(x+30,y+65,350,20,15);
	rectangle(x+30,y+65,350,20,0);
	startx=x+32;
	starty=y+70;
	while(1)
	{
		asm{   mov  ah,00h;
		       int  16h;
		       mov c1,al;
		       mov c2,ah;
		    }
		if(c2==0x1c||c2==0x01)
		{      if(c2==0x01) w[0]='e';
			return w;
		}
		if(c1==0x08&&c2==0x0E)
		    {
		      if(i>0) { i--; w[i]='\0'; }
		    }
		 else
		   {
			w[i++]=c1;
			w[i]='\0';
		    }
		if(startx+(i*8)<x+372)
		write(startx,starty,0,w);
	}
      //	return("hjfgs");
}

int Turn_Off()
{       char c1,*t1[2]={"Turn off","Reboot"};
	int x=xmax/2-150,y=ymax/2-100,ht=120,wd=300,s=0;
	Message_Window(x,y,wd,ht,"TURN OFF SPLENDOR");
	bar(x+30,y+50,100,20,1);
	rectangle(x+30,y+50,100,20,15);
	write(x+50,y+55,43,t1[0]);
	while(1)
	{
		asm{    mov  ah,00h;
			int  16h;
			mov c1,ah;
		   }
		if(c1==0x50||c1==0x48)
		{
			if(c1==0x50)//downarrow
			{	if(s==0)s=1;}
			else if(c1==0x48)//uparrow
			{       if(s==1)s=0;}
			bar(x+30,y+50,100,20,1);
			rectangle(x+30,y+50,100,20,15);
			write(x+35,y+55,43,t1[s]);
		}
		else if(c1==0x01) return -1;
		else if(c1==0x1c)
		return s;

	}
}

void TXT_Reader(char* path)
{       char *buffer,c1;
	buffer=new char[2];
	int startx=100,starty=50,m;
	Message_Window(0,0,xmax,winy,"SPLENDOR READER");
	      //	Message_Window(0,0,xmax,winy,path);
       //	bar(15);
       //	bar(15);
       //	write(200,200,0,path);
	File f;
	m=f.file_open(path,RO);
	buffer[0]=f.fget_char();
	buffer[1]='\0';
	while(buffer[0]==13)
	{	*buffer=f.fget_char();
		*buffer=f.fget_char();
		*(buffer+1)='\0';
		starty+=16;
		startx=100;
	}
	while(!f.eof)
	{      Message_Window(0,0,xmax,winy,"SPLENDOR READER");
		startx=100;
		starty=50;
		while(!f.eof&&starty<winy-30)
		{       write(startx,starty,1,buffer);
			buffer[0]=f.fget_char();
			buffer[1]='\0';
			while(buffer[0]==13)
			{	*buffer=f.fget_char();
				*buffer=f.fget_char();
				*(buffer+1)='\0';
				starty+=16;
				startx=100;
			}
	      //	if(flag==0)index++;
			startx+=8;
			if(startx>=xmax-100)
			{	starty+=20;
				startx=100;
			}
		}

		asm{	mov ax,0;
			int 16h;
			mov c1,ah;
			}
		if(c1==0x01||f.eof)break;
	}
}
void Change_Color(char desktop)
{       char *name,c1,color;
	int x=200,y=200,ht=200,wd=350;
	if(desktop)
	{	name="Desktop Color";
		color=Desktop_Color;
	}
	else
	{       name="Frame Settings";
		 color=win.bcolor;
	}
	Message_Window(x,y,wd,ht,name);
	rectangle(x+wd/2-1,y+75-1,102,102,15);
	bar(x+wd/2,y+75,100,100,color);
	while(1)
	{
		asm{    mov  ah,00h;
			int  16h;
			mov c1,ah;
		   }
		if(c1==0x50||c1==0x48)
		{
			if(c1==0x50)//downarrow
			color++;
			else if(c1==0x48)//uparrow
			color--;
			bar(x+wd/2,y+75,100,100,color);
		}
		else if(c1==0x01) break;
		else if(c1==0x1c)
		{       if(desktop)
			Desktop_Color=color;
			else win.bcolor=color;
			break;
		}
	}
}
void Change_Resolution()
{       char *name="Set Resolution",c1,color,*t1[3]={"640 x 480","800 x 600","1024 x 768"};
	int x=200,y=200,ht=100,wd=350,res;

	res=Resolution;
	Message_Window(x,y,wd,ht,name);
	bar(x+30,y+50,100,20,1);
	rectangle(x+30,y+50,100,20,15);
	write(x+35,y+55,43,t1[res]);
	while(1)
	{
		asm{    mov  ah,00h;
			int  16h;
			mov c1,ah;
		   }
		if(c1==0x50||c1==0x48)
		{
			if(c1==0x50)//downarrow
				res=(res+1)%3;
			else if(c1==0x48)//uparrow
			if(res>0)res=(res-1)%3;
			else res=2;
			bar(x+30,y+50,100,20,1);
			rectangle(x+30,y+50,100,20,15);
			write(x+35,y+55,43,t1[res]);
		}
		else if(c1==0x01) break;
		else if(c1==0x1c)
		{       Resolution=res;
			init(Resolution);
			break;
		}
	}
}
void System_Icon(int x,int y,int select)
{ 	char color,color1;
	if(select==0)
	{	color=7;
		color1=20;
	}
	else
	{	color=1;
		color1=1;
	}
	bar(x,y,40,40,color);
	rectangle(x,y,40,40,23);
	bar(x+5,y+5,30,30,54);
	bar(x+7,y+8,5,5,color1);
	bar(x+7,y+18,5,5,color1);
	bar(x+7,y+28,5,5,color1);
	bar(x+15,y+8,5,5,color1);
	bar(x+15,y+18,5,5,color1);
	rectangle(x+5,y+5,30,30,23);
	bar(x+5,y+40,30,5,color);
	rectangle(x+4,y+39,32,7,23);
	fill_par(x,y+45,40,15,-1,color);
	fill_par(x+8,y+48,30,1,-1,23);
	fill_par(x+12,y+52,30,1,-1,23);
	fill_par(x+16,y+56,30,1,-1,23);
	bar(x+16,y+61,40,2,color);
}

class Tool_Bar
{	public :char height,color,fcolor,bcolor,button_width;
			Tool_Bar();
			void Draw_Bar(char);
			void Select_Deselect(char,int);
};

Tool_Bar::Tool_Bar()
{	height=30;
	color=7;
	fcolor=15;
	button_width=44;
}

void Tool_Bar::Draw_Bar(char obj)
{   int x,y,i,jmax;
	if(obj=='t')
	{	 jmax=1;
		 x=0;
		 y=winy;
		 bcolor=8;
	}
	else
	{	 jmax=3;
		 bcolor=7;
		 i=Active_Count+1;
		 x=Active_Position[i][0];
		 y=Active_Position[i][1];
	}
	bar(x,y,xmax-1,height,color);
 //	vline(x,y,height,15);
	hline(x,y,xmax-1,15);
//	vline(,y+5,height-10,0);
	hline(x,y+height-1,xmax-1,15);
	bar(x,y+height,xmax-1,3,7); //bar after filebar
	hline(x,y+height+1,xmax-1,0);
	for(i=0;i<jmax;i++)
	{       bar(x+2,y+5,button_width,height-10,bcolor);
		vline(x+2,y+5,height-10,15);
		hline(x+2,y+5,button_width,15);
		vline(x+button_width+2,y+5,height-10,0);
		hline(x+2,y+height-5,button_width,0);
		Active_Position[Active_Count+1+i][0]=x;
		Active_Position[Active_Count+1+i][1]=y;
		x+=button_width+2;
	 }
	 if(obj=='t')
	 {  	x=0;
		load_font(1);
		write(x+5,y+9,fcolor,"TASKS");
		load_font(3);
	 }
	 else
	 {  load_font(3);
	    x=Active_Position[Active_Count+1][0]+2;
		write(x+6,y+12,fcolor,"File");  x+=button_width+2;
	    write(x+6,y+12,fcolor,"Edit");  x+=button_width+2;
		write(x+6,y+12,fcolor,"View");  x+=button_width+2;
	 }

}

void Tool_Bar::Select_Deselect(char key,int select)
{       int x,y,i;
		char c1,c2,c,fc;
		if(!select)
		{	c1=0;c2=15;c=8;fc=15;}
		else
		{   c1=15;c2=0;c=7;fc=0;}
		if(key=='t')
		{	x=Active_Position[Active_Count][0];
			y=Active_Position[Active_Count][1];
		}
		else
		{   if(!select)c=7;
			if(key=='f') i=0;
			else if(key=='e') i=1;
			else if(key=='v') i=2;
			x=Active_Position[Active_Count+1][0]+(i*(button_width+2));
			y=Active_Position[Active_Count+1][1];
		}
		bar(x+2,y+5,button_width,height-10,c);
		vline(x+2,y+5,height-10,c2);
		hline(x+2,y+5,button_width,c2);
		vline(x+button_width+2,y+5,height-10,c1);
		hline(x+2,y+height-5,button_width,c1);

		if(key=='t')
		{   load_font(1);
			write(x+5,y+9,fc,"TASKS");
			load_font(3);
		}
		else
		{   load_font(3);
			if(key=='f')write(x+6,y+12,fc,"File");
			else if(key=='e')write(x+6,y+12,fc,"Edit");
			else if(key=='v')write(x+6,y+12,fc,"View");
		   //	if(select)
		}

}


class Icons
{
	public :char fcolor,bcolor,hfcolor,hbcolor,obj;
		int height,width;
		Icons(char);
		void Draw(int,char);
		void Icon_Draw(int,int,int);
		void List_Draw(int,char,char);
};

Icons::Icons(char o)
{   obj=o;
	if(obj==0) bcolor=56; //object is desktop
	else if(obj==1)bcolor=15; //object is window
	else if(obj==2)
	{	bcolor=7;
		fcolor=7;
	}
	//setting fontcolor,highlighted background and font color
	fcolor=0;
	hbcolor=1;
	hfcolor=15;
}

void Icons::Draw(int i,char select)  // for desktop & windows
{   int x=Active_Position[i][0],j;
	int y=Active_Position[i][1];
	char color,fc,*dt;
	type t=Active_Type[i];
	if(t==0)                  // Folders
	{	height=20;
		width=40;
		if(select==0) color=43;
		else color=1;
		if(obj) y+=30;
		fill_par(x,y+2,30,18,1,color);
		rectangle(x-17,y+5,30,16,8);
		bar(x-16,y+6,28,14,color);
		x-=height;
	}
	else if(t==1)             // Files
	{	height=50;
		width=40;
		if(select==0) color=4;
		else color=1;
		bar(x,y,1,50,color); //v
		bar(x,y,20,1,color); //h
		//triangle
		bar(x+20,y,1,15,color);  //v
		bar(x+20,y+15,15,1,color);//h
		line(x+20,y,x+35,y+15,color);
		//----------
		bar(x+33,y+15,3,38,color);   //v
		bar(x,y+50,35,3,color);  //h
		// three lines

		bar(x+5,y+23,25,1,0);   //h
		bar(x+8,y+30,22,1,0);  //h
		bar(x+11,y+37,19,1,0);//h

	}
	else if(t==system)
	{	height=62;
		width=40;
		System_Icon(x,y,select);
		x+=8;
	}
	else if(t==floppy)
	{       height=42;
		width=50;
		if(select==0) color=20;
		else color=1;
		bar(x,y,50,40,color);
		bar(x+10,y-2,30,15,23);
		if(select==0) color=0;
		else color=1;
		bar(x+32,y+1,5,10,color);
		bar(x+7,y+17,36,20,15);
		bar(x+2,y+30,3,5,25);
		bar(x+46,y+30,3,5,25);
		line(x+10,y+20,x+40,y+20,0);
		line(x+10,y+24,x+40,y+24,0);
		line(x+10,y+28,x+40,y+28,0);
		line(x+10,y+32,x+40,y+32,0);
	}
	else if(t==drive)
	{   height=20;
	    width=45;
	    if(select==0) color=7;
	    else color=1;
	    bar(x+1,y-1,width,height,color);
	    bar(x,y,width,height,color);           //background
	    rectangle(x,y,width,height,19);
	    bar(x+5,y+14,width-10,1,15);    // lines
	    bar(x+5,y+12,width-10,1,15);
	    bar(x+5,y+10,width-10,1,15);
	    bar(x+width/2-2,y+height,5,7,color);
	    bar(x,y+height+8,width+2,5,color);
	    rectangle(x,y+height+8,width+2,5,23);
	    bar(x,y+height+10,width,1,15);
	    bar(x+width/2-6,y+height+7,15,8,color);
	    rectangle(x+width/2-6,y+height+6,15,10,23);
	    height+=16;
       }
       else if(t==bmp)
       {        y+=20;
		height=25;
		width=53;
		int wd=45,ht=20;
		if(select==0) color=15;
		else color=1;
		bar(x+5,y-15,wd-15,ht+20,color); // paper
		rectangle(x+5,y-15,wd-15,ht+20,8);
		bar(x+7,y-11,wd-20,ht+12,4);       // frame
		line(x+13,y+18,x+17,y-9,10);    // picture
		line(x+25,y+18,x+20,y-9,10);
		hline(x+13,y+18,wd-34,10);
		fill_par(x,y,wd,ht-5,1,color);   // palette background
		bar(x,y+2,wd-38,ht-15,2);                // palette
		fill_par(x+25,y-20,wd-41,ht+5,1,24);   //brush
		bar(x+9,y+2,wd-40,ht-15,0);           // palette
		bar(x+17,y+2,wd-40,ht-15,6);
		bar(x+24,y+2,wd-40,ht-15,4);
		bar(x+32,y+2,wd-40,ht-15,1);
		bar(x-5,y+8,wd-40,ht-15,5);
		bar(x+2,y+8,wd-40,ht-15,8);
		bar(x+9,y+8,wd-40,ht-15,12);
		bar(x+17,y+8,wd-40,ht-15,43);
		bar(x+25,y+8,wd-40,ht-15,13);
	     //	height+=20;
       }
       else                              //Unknown files
       {	height=30;
		width=40;
		if(obj) y+=20;
		if(!select) { color=47;fc=0;}
		else {color=1;fc=15;}
		/*fill_circle(x+19,y+15,15,0);
		fill_circle(x+15,y+15,15,color);     */
		fill_par(x+19,y+10,5,15,1,color);
		fill_par(x+5,y+10,5,15,-1,color);
	}
	if(select==0)                 // If selected
	{      color=bcolor;
	       fc=fcolor;
	}
	else                           // If not yet selected or deselected
	{      color=hbcolor;
	       fc=hfcolor;
	}

	dt=Active_List[i];
	for(j=0;dt[j]!='\0';j++);
	size=j*8;
	if(size<=width)
		x+=(width-size)/2;
	else x-=(size-width)/2;
	bar(x-2,y+height+10,size+4,12,color);
	load_font(3);
	write(x,y+height+12,fc,Active_List[i]);
		  /*if(select==1)
		  rectangle_shade(x-10,y+height+10,width+20,12,15);  */
}

void Icons::Icon_Draw(int x,int y,int i)  // for properties
{
	char color;
	type t=Active_Type[i];
	if(t==0)                  // Folders
	{	height=20;
		width=40;
		color=43;
		x+=10;
		fill_par(x,y,35,20,1,color);
		rectangle(x-20,y+5,35,15,8);
		bar(x-19,y+6,33,13,color);
	}
	else if(t==1)             // Files
	{	height=50;
		width=40;
		color=4;
		bar(x,y,1,50,color); //v
		bar(x,y,20,1,color); //h
		//triangle
		bar(x+20,y,1,15,color);  //v
		bar(x+20,y+15,15,1,color);//h
		line(x+20,y,x+35,y+15,color);
		//----------
		bar(x+33,y+15,3,38,color);   //v
		bar(x,y+50,35,3,color);  //h
		// three lines

		bar(x+5,y+23,25,1,0);   //h
		bar(x+8,y+30,22,1,0);  //h
		bar(x+11,y+37,19,1,0);//h
	}
	else if(t==floppy)
	{       height=42;
		width=50;
		color=20;
		bar(x,y,50,40,color);
		bar(x+10,y-2,30,15,23);
		color=0;
		bar(x+32,y+1,5,10,color);
		bar(x+7,y+17,36,20,15);
		bar(x+2,y+30,3,5,25);
		bar(x+46,y+30,3,5,25);
		line(x+10,y+20,x+40,y+20,0);
		line(x+10,y+24,x+40,y+24,0);
		line(x+10,y+28,x+40,y+28,0);
		line(x+10,y+32,x+40,y+32,0);
	}
	else if(t==drive)
	{   height=20;
	    width=45;
	    color=8;
	    bar(x+1,y-1,width,height,color);
	    bar(x,y,width,height,color);           //background
	    rectangle(x,y,width,height,19);
	    bar(x+5,y+14,width-10,1,15);    // lines
	    bar(x+5,y+12,width-10,1,15);
	    bar(x+5,y+10,width-10,1,15);
	    bar(x+width/2-2,y+height,5,7,color);
	    bar(x,y+height+8,width+2,5,color);
	    rectangle(x,y+height+8,width+2,5,23);
	    bar(x,y+height+10,width,1,15);
	    bar(x+width/2-6,y+height+7,15,8,color);
	    rectangle(x+width/2-6,y+height+6,15,10,23);
	    height+=16;
       }
	else if(t==bmp)
       {        y+=20;
		height=25;
		width=53;
		int wd=45,ht=20;
		color=15;
		bar(x+5,y-15,wd-15,ht+20,color); // paper
		rectangle(x+5,y-15,wd-15,ht+20,8);
		bar(x+7,y-11,wd-20,ht+12,4);       // frame
		line(x+13,y+18,x+17,y-9,10);    // picture
		line(x+25,y+18,x+20,y-9,10);
		hline(x+13,y+18,wd-34,10);
		fill_par(x,y,wd,ht-5,1,color);   // palette background
		bar(x,y+2,wd-38,ht-15,2);                // palette
		fill_par(x+25,y-20,wd-41,ht+5,1,24);   //brush
		bar(x+9,y+2,wd-40,ht-15,0);           // palette
		bar(x+17,y+2,wd-40,ht-15,6);
		bar(x+24,y+2,wd-40,ht-15,4);
		bar(x+32,y+2,wd-40,ht-15,1);
		bar(x-5,y+8,wd-40,ht-15,5);
		bar(x+2,y+8,wd-40,ht-15,8);
		bar(x+9,y+8,wd-40,ht-15,12);
		bar(x+17,y+8,wd-40,ht-15,43);
		bar(x+25,y+8,wd-40,ht-15,13);
	    //	height+=20;
       }
	else                              //Unknown files
	{	height=30;
		width=40;
		color=47;
		fill_par(x+19,y+10,5,15,1,color);
		fill_par(x+5,y+10,5,15,-1,color);
	}
}

void Icons::List_Draw(int i,char select,char whom)
{   int x=Active_Position[i][0];
	int y=Active_Position[i][1],j;
	char color,fc,*temp;
	type t=Active_Type[i];
	height=10;
	width=15;
	if(t==0)                         // Folders in list
	{       if(select==0) color=43;
		else color=1;
		bar(x,y,width,height,color);
		rectangle(x-1,y+1,width+2,height+2,0);
	}
	else if(t==1)       // Files  in list
	{	if(select==0) color=4;
		else color=1;
		rectangle(x,y,height,width,color);
		hline(x+2,y+5,6,0);
		hline(x+3,y+7,5,0);
		hline(x+4,y+9,4,0);
	}
	else if(t==floppy)
	{       height=14;
		width=20;
		if(select==0) color=20;
		else color=1;
		bar(x,y,width,height,color);
		bar(x+3,y-2,width-6,height-8,23);
		bar(x+2,y+6,width-4,height-8,15);
		hline(x+4,y+8,width-8,0);
		hline(x+4,y+10,width-8,0);
	}
	else if(t==drive)
	{   height=10;
	    width=20;
	    if(select==0) color=7;
	    else color=1;
	    bar(x+1,y-1,width,height,color);
	    bar(x,y,width,height,color);           //background
	    rectangle(x,y,width,height,19);
	    bar(x+3,y+6,width-8,1,15);    // lines
	    bar(x+3,y+4,width-8,1,15);
	    bar(x+3,y+2,width-8,1,15);
	    bar(x+width/2-2,y+height,3,5,color);
	    bar(x,y+height+5,width,3,color);       // bottom rod
	    if(select==0) color=23;
	    else color=1;
	    rectangle(x,y+height+5,width,3,color);
	    bar(x,y+height+6,width-2,1,15);        // white line across rod
	    bar(x+width/2-4,y+height+5,8,4,color);     // bottom block
	    rectangle(x+width/2-4,y+height+4,8,5,23);
	    height+=16;
       }
       else if(t==bmp)
       {        y+=10;
		x-=5;
		height=15;
		width=20;
		int wd=45,ht=20;
		if(!select) color=15;
		else color=1;
		bar(x+5,y-5,wd-35,ht-5,color); // paper
		rectangle(x+5,y-5,wd-35,ht-5,8);
		bar(x+6,y-3,wd-38,ht-9,4);        // frame
		fill_par(x+4,y,wd-30,ht-15,1,color);   // palette background
		bar(x+4,y+2,wd-42,ht-18,4);                // palette
		fill_par(x+15,y-8,wd-43,ht-9,1,24);   //brush
		bar(x+8,y+2,wd-43,ht-18,1);
		bar(x+11,y+2,wd-43,ht-18,5);
		y-=6;
	     //	height+=10;
		width-=5;
       }
	else                  //  Unknown files
	{	if(!select)  color=47;
		else color=1;
		fill_par(x+5,y+5,4,7,1,color);
		fill_par(x-2,y+5,4,7,-1,color);
	}
	if(select==0)                // If selected
	{      color=bcolor;
		   fc=fcolor;
	}
	else
	{      color=hbcolor;       // If not yet selected or deselected
		   fc=hfcolor;
	}
	y-=4;

	if(whom==1)
	{   for(j=0;Active_List[i][j]!='\0';j++);
		size=j*8;
		bar(x+width+2,y+height/2,size+2,12,color);
		load_font(3);
		write(x+width+2,y+height/2+2,fc,Active_List[i]);
	}
	else if(whom==2)
	{   bar(x+width+2,y+height/2,width+340,12,color);
		write(x+width+2,y+height/2+2,fc,Active_List[i]);
		x+=150;
		if(Active_Type[i]==file)write(x+width+2,y+height/2+2,fc,"Text File");  // Type
		else if(Active_Type[i]==folder)write(x+width+2+2,y+height/2+2,fc,"Folder");
		else if(Active_Type[i]==unknown)write(x+width+2+2,y+height/2+2,fc,"Unknown");
		x+=150;
		if(Active_Type[i]==file)
		{
			for(i=0;curr_directory[i]!='\0';i++)
			temp[i]=curr_directory[i];
			temp[i] ='\0';
			Dir_Entries *entries;
			Directory dir(temp);
			entries=dir.dir_read();
			long s=entries[i+2].size;
	      /*	for(i=0;curr_directory[i]!='\0';i++)
			temp[i]=curr_directory[i];
		temp[i] ='\0';
		temp[i++]=':';
		for(j=0;j<=11;i++,j++)
		temp[i]=entries[x+2].name[j];
		temp[i] ='\0';
		File f;
		f.file_open(temp,RO);
		s=f.get_file_size();    */
			char *z=To_String(1298);
			for(i=0;z[i]!='\0';i++)
				temp[i]=z[i];
			temp[i] ='\0';
			write(x+width+2,y+height/2+2,fc,temp);
	       }    // Size
	}

}

Tool_Bar taskbar;

class Window
{	public :char *name,fcolor,bcolor,border,view;
		int startx,starty,unwanted;
		Tool_Bar filebar;
		Window(char *);
		void Winini();
		void Draw();
		void Change_Title(char *);
		int Event_Handler();
		void Plot_All(char);
		char Body_Handler(char&,char&,char);
		void Properties(int);
};
void Window ::Winini()
{	fcolor=win.fcolor;
	bcolor=win.bcolor;
}
Window ::Window(char* s)
{	name=s;
	view=0;
	Winini();
	border=7;
}

void Window ::Properties(int index)
{   	int x,y,wd=300,ht=250;
	long i;
	char c1,c2,*temp;
	Icons icon(2);
	x=xmax/2-wd/2;
	y=ymax/2-ht/2;
	Message_Window(x,y,wd,ht,"Properties");
	icon.Icon_Draw(x+30,y+40,index);
	//if (Active_List[index])
	write(x+icon.width+50,y+50,0,Active_List[index]);
	//else write(x+icon.width+50,y+50,0,Active_List[index-1]);
	write(x+15,y+icon.height+80,0,"Type :");
	if(Active_Type[index]==file)write(x+100,y+icon.height+80,0,"Text File");
	else if(Active_Type[index]==folder)write(x+100,y+icon.height+80,0,"Folder");
	else if(Active_Type[index]==bmp)write(x+100,y+icon.height+80,0,"BMP File");
	else if(Active_Type[index]==floppy)write(x+100,y+icon.height+80,0,"3.5 Floppy");
	else if(Active_Type[index]==drive)write(x+100,y+icon.height+80,0,"Disk Drive");
	else if(Active_Type[index]==unknown)write(x+100,y+icon.height+80,0,"Unknown");
	write(x+15,y+icon.height+110,0,"Location :");
	write(x+100,y+icon.height+110,0,curr_directory);
	if(Active_Type[index]==file)
	{	for(i=0;curr_directory[i]!='\0';i++)
			temp[i]=curr_directory[i];
		temp[i] ='\0';
		Dir_Entries *entries;
		Directory dir(temp);
		entries=dir.dir_read();
		long s=entries[i+2].size;
		char *z=To_String(s);
		for(i=0;z[i]!='\0';i++)
			temp[i]=z[i];
		temp[i] ='\0';

		write(x+15,y+icon.height+140,0,"Size :");
		write(x+50,y+icon.height+140,0,temp);
	}
	else if(Active_Type[index]==folder)
	write(x+15,y+icon.height+140,0,"Contains :");
	write(x+15,y+icon.height+170,0,"Created on :");
	while(1)
	{	asm{		mov ax,0h
				int 16h
				mov c1,ah
				mov c2,al
		    }
		if(c1==0x01&&c2==0x1b)    break;
	}
	Plot_All(view);
	if(view==0) icon.Draw(index,1);
	else if(view==1|| view==2) icon.List_Draw(index,1,view);
}
void Window ::Change_Title(char *s)
{       int i,k,j,ht=30,count=0;
	char d1[9]={'F','l','o','p','p','y','\0'},
	     d2[9]={'D','r','i','v','e', ' ','C','\0'},temp[50];
	name=s;
	for(i=0;name[i]!='\0';i++)
		temp[i]=name[i];
	temp[i]='\0';
	if(name[0]!='S')
	{   	// reading into active list
		Dir_Entries *entries;
		Directory dir(temp);
		entries=dir.dir_read();
		char n[4];
		if(entries[0].name[0]=='.')count++;
		if(entries[1].name[0]=='.')count++;
		if(curr_directory[3]==0)count=4;
		for(j=count;j<30;j++ )
		{      //Active_List[j-2]=new(char[8]);
			for(i=0;i<=7&&entries[j].name[i]!=' ';i++)
			Active_List[j-count][i]=entries[j].name[i];
			Active_List[j-count][i]='\0';
			if(Active_List[j-count][0]=='\x0')break;
			for(i=8,k=0;i<=10;i++,k++)
			n[k]=entries[j].name[i];
			n[3]='\0';
			if((n[0]=='T'&&n[1]=='X'&&n[2]=='T')||(n[0]=='t'&&n[1]=='x'&&n[2]=='t')) Active_Type[j-count]=file;//t=file;
			else if((n[0]=='B'&&n[1]=='M'&&n[2]=='P')||(n[0]=='b'&&n[1]=='m'&&n[2]=='p')) Active_Type[j-count]=bmp;
			else if(n[0]==' ')Active_Type[j-count]=folder;//t=folder;
			else Active_Type[j-count]=unknown;//t=unknown;
		}
		if(Active_List[0][0]=='\x0')
		Active_Count=0;
		else
		Active_Count=j-count;
		unwanted=count;
		count=0;
	}
	else
	{   for(i=0;i<9;i++)
		{	Active_List[0][i]=d1[i];
			Active_List[1][i]=d2[i];
		}
		Active_Type[0]=floppy;
		Active_Type[1]=drive;
		Active_Count=2;
	}
	bar(0,0,xmax-1,ht,1);      //titlebar
	load_font(1);
	write(11,11,15,name);      //address
	load_font(3);
}
void Window ::Draw()
{   int curr_x,curr_y,i,k,j,count=0,ht=30,height=14; //currx & y rep icon locations & count rep no of entries;
	Icons icon(1);
	type t;
	Change_Title(name);
	k=0;
	//end of reading to active list
	bar(0,0,xmax-1,5,7); //border of titlebar
	hline(0,2,xmax-1,15);
	hline(0,3,xmax-1,0);
				//position of taskbar
	Active_Position[Active_Count][0]=0;
	Active_Position[Active_Count][1]=winy;
				//position of filebar
	Active_Position[Active_Count+1][0]=5;
	Active_Position[Active_Count+1][1]=ht;
	filebar.Draw_Bar('f');                //filebar
	curr_x=0;
	curr_y=ht+filebar.height+3;
	bar(curr_x,curr_y,xmax-1,winy-curr_y,bcolor);        //body
	bar(curr_x,winy-5,xmax-1,5,7); //bar after body
	hline(curr_x,winy-2,xmax-1,0);
	hline(curr_x,winy-4,xmax-1,15);
	// vlines of window border
	bar(0,0,5,winy,7); //border of titlebar
	vline(1,2,winy-2,15);
	vline(3,3,winy-3,0);

	bar(xmax-5,0,5,winy,7); //border of titlebar
	vline(xmax-4,2,winy-2,0);
	vline(xmax-2,3,winy-3,15);

	starty=curr_y+30;

	if(view==0)
	{       curr_x=40;
		curr_y+=30;
		for (i=0;i<Active_Count;i++)
		{      //Active_Position[i]=new(int[2]);
			Active_Position[i][0]=curr_x;
			Active_Position[i][1]=curr_y;
			icon.Draw(i,0);
			count++;
			curr_x+=icon.width+50;  //updating x co-ordinate of icon
			if(curr_x+icon.width+10>=xmax-1)  // check for xmax of icon
			{	curr_x=30;
				curr_y+=icon.height+60;
				Active_Column[k]=count;
				k++;
				count=0;
			}
		}
		if(curr_y+icon.height+20<winy)  Active_Column[k]=count;
	}
	else if(view==2||view==1)
	{	starty=curr_y+30;
		curr_x=5;
		if(view==2)
		{  	bar(curr_x,curr_y,xmax-10,height,7);
			hline(curr_x,curr_y,xmax-10,15);
			hline(curr_x,curr_y+height-1,xmax-10,15);
			bar(curr_x,curr_y+height,xmax-10,3,7); //bar after filebar
			hline(curr_x,curr_y+height+1,xmax-10,0);
		}
		k=0;
		curr_x=40;
		curr_y+=30;
		for (i=0;i<Active_Count;i++)
		{     //Active_Position[i]=new(int[2]);
			Active_Position[i][0]=curr_x;
			Active_Position[i][1]=curr_y;
			icon.List_Draw(i,0,view);
			count++;
			curr_y+=icon.height+10;  //updating y co-ordinate of icon
			if(curr_y+icon.height+30>=winy)  // check for ymax of icon
			{	curr_y=starty;
				curr_x+=icon.width+50;
				Active_Column[k]=count;
				k++;
				count=0;
			}
		}
		if(curr_y+icon.height+30<winy)  Active_Column[k]=count;

	}
	//taskbar.Draw_Bar('t');

}

void Window ::Plot_All(char what)
{   int curr_x,curr_y,count=0,k=0,i,height=14;
	Icons icon(1);
	curr_x=5;
	curr_y=filebar.height+32;
	bar(curr_x,curr_y,xmax-10,winy-curr_y-5,bcolor);
	starty=curr_y+30;
	if(what==2)
	{  	bar(curr_x,curr_y,xmax-10,height,7);
		hline(curr_x,curr_y,xmax-10,15);
		hline(curr_x,curr_y+height-1,xmax-10,15);
		bar(curr_x,curr_y+height,xmax-10,3,7); //bar after filebar
		hline(curr_x,curr_y+height+1,xmax-10,0);

	}
	curr_x=40;
	curr_y=starty;
	if(what==0) //icons
	{

		for (i=0;i<Active_Count;i++)
		{      //Active_Position[i]=new(int[2]);
			Active_Position[i][0]=curr_x;
			Active_Position[i][1]=curr_y;
			icon.Draw(i,0);
			count++;
			curr_x+=icon.width+50;  //updating x co-ordinate of icon
			if(curr_x+icon.width+10>=xmax-1)  // check for xmax of icon
			{	curr_x=30;
				curr_y+=icon.height+60;
				Active_Column[k]=count;
				k++;
				count=0;
			}
		}
		if(curr_y+icon.height+20<winy)  Active_Column[k]=count;
	}//end of icons

	else if(what==1 ||what==2)//list
	{ 	k=0;
		for (i=0;i<Active_Count;i++)
		{     //Active_Position[i]=new(int[2]);
			Active_Position[i][0]=curr_x;
			Active_Position[i][1]=curr_y;
			icon.List_Draw(i,0,what);
			count++;
			curr_y+=icon.height+10;  //updating y co-ordinate of icon
			if(curr_y+icon.height+30>=winy)  // check for ymax of icon
			{	curr_y=starty;
				curr_x+=icon.width+50;
				Active_Column[k]=count;
				k++;
				count=0;
			}
		}
		if(curr_y+icon.height+30<winy)  Active_Column[k]=count;

	}//end of list

}
void remove_file(int x,int unwanted)
{       int i,j,flag=0;
	char s[50];
	for(i=0;curr_directory[i]!='\0';i++)
	s[i]=curr_directory[i];
	s[i] ='\0';
	Dir_Entries *entries;
	Directory dir(s);
	entries=dir.dir_read();
	for(i=0;curr_directory[i]!='\0';i++)
	s[i]=curr_directory[i];
	if(i>4)s[i++]=':';
	for(j=0;j<=10;i++,j++)
	{
		if((entries[x+unwanted].name[j]==' '||j==8)&&flag==0)
		{	s[i]='.';
			if(j!=8)j=7;
			else{i++;s[i]=entries[x+unwanted].name[j];}
			flag=1;
		}
		else
		s[i]=entries[x+unwanted].name[j];
	}
	s[i] ='\0';
	write(300,100,4,s);
	getchar();
	File f;
	int file_flag;
	file_flag= f.file_open(s,RO);
	/*if(file_flag==2)
	{
		Error_Code = File_Not_Found;
	} */
	dir.remove_entry(f.file_details.file_name,f.drive);
	d[f.drive-2].clear_fat_entry(f.next_lba,f.drive);
}

void make_directory(char *z)
{       char *dt;
	int i,j;
	for(i=0,j=0;curr_directory[j]!='\0';i++,j++)
		 dt[i]=curr_directory[j];
	dt[i]='\0';
	write(100,100,0,dt);
	char temp_dirname[11];
	//if(z[0]!='e')curry_dir.add_dir(dt);//call md
	if(z[0]!='e')
	{
		for(i=0;z[i]!=0&&i<11;i++) temp_dirname[i]=z[i];
		temp_dirname[i]=0;
		Directory dir(dt);
		dir.add_dir(temp_dirname);//call md
	}
}

void copy_to_clipboard(int x1,char shift,int unwanted)
{       int i,j;
	char s[50];
	clipboard[0]='1';
	if(shift)clipboard[1]='0';
	else clipboard[1]='1';
	for(i=0;curr_directory[i]!='\0';i++)
		s[i]=curr_directory[i];
	s[i] ='\0';
	Dir_Entries *entries;
	Directory dir(s);
	entries=dir.dir_read();
	for(i=0;curr_directory[i]!='\0';i++)
	s[i]=curr_directory[i];
	if(i>4)s[i++]=':';
	for(j=0;j<=10;i++,j++)
	{
		if(entries[x1+unwanted].name[j]==' ')
		{       s[i]='.';
			j=7;
		}
	       else
		s[i]=entries[x1+unwanted].name[j];
	}
	s[i] ='\0';
	for(i=0,j=2;s[i]!='\0';i++,j++)
		clipboard[j]=s[i];
	clipboard[j]='\0';
}

void init_shell()
{	cmd_buff[cmd][0]='C';
	cmd_buff[cmd][1]=':';
	cmd_buff[cmd][2]=0;
	int r=command();
}

int get_index(int &u1)
{       char s[50],name[11],*c,*d;
	int i,j,count=0,k,x1;
	for(i=0,j=2;clipboard[j]!='\0';j++,i++)
		s[i]=clipboard[j];
	for(;clipboard[j]!=':'&&clipboard[j]!='\\';j--);
	s[j-2]='\0';
	j++;
	for(i=0;clipboard[j]!='\0';j++,i++)
	name[i]=clipboard[j];
	name[i]='\0';
       /*	write(50,200,0,name);
	write(50,300,0,s);
	asm{	mov ax,0
		int 16h;
	}*/
	Dir_Entries *entries;
	Directory dir(s);
	entries=dir.dir_read();
	if(entries[0].name[0]=='.')count++;
	if(entries[1].name[0]=='.')count++;
	if(s[3]==0)count=4;
	u1=count;
	d=To_String(count);
     //	write(100,400,0,d);
	for(j=count;j<30;j++ )
	{       write(100,100,0,entries[j].name);
		getchar();
		 write(100,100,15,entries[j].name);
		getchar();
		for(i=0;i<8;i++)
			if(name[i]!=entries[j].name[i])break;
		if((name[i]=='.'&&entries[j].name[i]==' ')||(i==8))
		{       i++;
			for(k=8;k<=10;k++,i++)
				if(name[i]!=entries[j].name[k])break;
			if(k==11)
			{
				x1=j-count;
				return (x1);
			      /*	c=To_String(j);
				write(100,400,0,c);
				asm{mov ax,0
					int 16h;
				}*/
			}
		}
	    //	if(entries[0].name[0]=='\x0')break;
	}

      return(-1);
}

void gui_paste()
{       int i,j,k;
	char s[50],ch[50];
	init_shell();
	if(clipboard[0]=='1')
	{	for(i=0;curr_directory[i]!='\0';i++)
			ch[i]=curr_directory[i];
			ch[i]='\0';
		for(k=0,j=2;clipboard[j]!='\0';j++,k++)
			s[k]=clipboard[j];
		s[k]='\0';
		for(;clipboard[j]!=':';j--);
		if(i<=2)j--;
		for(;clipboard[j]!='\0';j++,i++)
		ch[i]=clipboard[j];
		ch[i]='\0';
	      /*	write(100,200,0,ch);
		write(100,300,0,s);
		asm{mov ax,0
		int 16h;
		} */
		copy(ch,s);//call rename
	}
}
void copy_name(int x,int unwanted,char *name)
{       int i,j,k,flag=0;
	char s[50],ch[50];
	init_shell();
	for(i=0;curr_directory[i]!='\0';i++)
	{	ch[i]=curr_directory[i];
		s[i]=curr_directory[i];
	}
	s[i]=0;
	k=i;
	ch[k++]=':';
	Dir_Entries *entries;
	Directory dir(s);
	entries=dir.dir_read();
	for(i=0;curr_directory[i]!='\0';i++)
	s[i]=curr_directory[i];
	if(i>4)s[i++]=':';
	int c=i;
	for(j=0;j<=10;i++,j++)
	{
		if(entries[x+unwanted].name[j]==' ')
		{	s[i]='.';
			j=7;
		}
		else
		s[i]=entries[x+unwanted].name[j];
	}
	s[i] ='\0';
	for(i=0;name[i]!='.';i++,k++)
		ch[k]=name[i];
	if(i==8)i++;
	for(;name[i]!='\0';i++,k++)
		ch[k]=name[i];
	ch[k]=0;
	write(100,200,1,entries[x+unwanted].name);
	write(100,200,0,ch);
	write(100,300,0,s);
	asm{mov ax,0
	int 16h;
	}
	copy(ch,s);//call rename

}
int Window ::Event_Handler()
{   	char c1,c2,val,x=0,t=1,*z,s[50],dt[50],*d1;
	//select_type select;
	int i,j,k,x1,u1;
	char ch[20];
	Icons icon(1);
	Popupmenu *p;
	if(Active_Count!=0)
	icon.Draw(x,1);
	while(1)
	{	asm{   	mov ax,00h
			int 16h
			mov c1,ah
			mov c2,al
		   }
		   while(1)
		   {	if(c1==0x14)//Tasks key
			{   	taskbar.Select_Deselect('t',1);
					p=new Popupmenu(Active_Count,0);
					val=p->Init();
					delete p;
					taskbar.Select_Deselect('t',0);
					if(val!=-1)
					{	switch(val)
						{	case 0: Shade();
								int h=Turn_Off();
								if(h==0||h==1)
								{       int x=xmax/2-150,y=ymax/2-100,ht=120,wd=350;
									Message_Window(x,y,wd,ht,"TURNING OFF....... ");
									write(x+50,y+65,4,"QUITTING SPLENDOR , BYE !!!!!!");
									asm{    mov  ah,00h;
										int  16h;
									    }
									closesvga();
									return turnoff;
								}
								Draw();
								taskbar.Draw_Bar('t');
								break;
							case 1:closesvga();shell();load();init(Resolution);Draw();/*taskbar.Draw_Bar('t');*/break;
							case 2:break;
							case 3: z=Get_File();
								if(z[0]!='e')
								{
									for(i=0;z[i]!='\0';i++)
									ch[i]=z[i];
									ch[i]='\0';
									if(ch[i-1]=='T'&&ch[i-2]=='X'&&ch[i-3]=='T'&&ch[i-4]=='.')
									TXT_Reader(ch);
									else
									{       int x=xmax/2-150,y=ymax/2-100,ht=120,wd=400;
										Message_Window(x,y,wd,ht,"ERROR !!!");
										load_font(1);
										write(x+30,y+50,4,"Format  Not  Supported ");
										load_font(3);
										asm{   	mov ax,00h
											int 16h
										   }
									}
								}
								 break; //readerbreak;
							case 4:Change_Color(1);
								Plot_All(view);
								if(view==0) icon.Draw(x,1);
								else if(view==1|| view==2) icon.List_Draw(x,1,view);
								break;
							case 5: Change_Resolution();
								taskbar.Draw_Bar('t');
								Draw();
								break;
						}
					}
					break;
				}
				else if(c1==0x21)
				{	taskbar.Select_Deselect('f',1);
					p=new Popupmenu(Active_Count+1,t);//t represents obj selected or not in window
					val=p->Init();
					delete p;
					taskbar.Select_Deselect('f',0);
					if((t==0&&val==2)||(t==1&&val==6)) return(exit);
					else if(val==0)//create folder
					{
						z=Get_File();
						make_directory(z);
						Draw();
					}
					else if(t==1&&val==1)//open
					{ 	if((Active_Type[x]==folder||Active_Type[x]==drive)&&(Active_Count!=0))
						{	view=0;Body_Handler(0,0,1);return x; }
						else if((Active_Type[x]==file)&&(Active_Count!=0))
						{       for(i=0;curr_directory[i]!='\0';i++)
							ch[i]=curr_directory[i];
							ch[i++]=':';
							for(j=0;Active_List[x][j]!='\0';i++,j++)
								ch[i]=Active_List[x][j];
							ch[i]='\0';
							ch[i++]='.';
							ch[i++]='T';
							ch[i++]='X';
							ch[i++]='T';
							ch[i]='\0';
								//write(200,200,0,ch);
							TXT_Reader(ch);
						}
						else if((Active_Type[x]==bmp)&&(Active_Count!=0))
						{	for(i=0;curr_directory[i]!='\0';i++)
							ch[i]=curr_directory[i];
							ch[i++]=':';
							for(j=0;Active_List[x][j]!='\0';i++,j++)
								ch[i]=Active_List[x][j];
							ch[i]='\0';
							ch[i++]='.';
							ch[i++]='B';
							ch[i++]='M';
							ch[i++]='P';
							ch[i]='\0';
								//write(200,200,0,ch);
							//PIC_Viewer(ch);
						}
						else if(Active_Type[x]==unknown)
						{ for(i=0;curr_directory[i]!='\0';i++)
							ch[i]=curr_directory[i];
							ch[i++]=':';
							for(j=0;Active_List[x][j]!='\0';i++,j++)
								ch[i]=Active_List[x][j];
							ch[i]='\0';
							ch[i++]='.';
							ch[i++]='T';
							ch[i++]='X';
							ch[i++]='T';
							ch[i]='\0';
								//write(200,200,0,ch);
							TXT_Reader(ch);
						}
						Draw();
					}
					else if(t==1&&val==2)//save as
					{       if(Active_Type[x]!=folder)
						{	z=Get_File();
							if(z[0]!='e')
							{	for(i=0;z[i]!='\0';i++)
									ch[i]=z[i];
								ch[i]='\0';
								copy_name(x,unwanted,ch);
								Fly();
							}
							Draw();
						}
					}
					else if(t==1&&val==3)//rename
					{       if(Active_Type[x]!=folder)
						{	z=Get_File();
							if(z[0]!='e')
							{	for(i=0;z[i]!='\0';i++)
									ch[i]=z[i];
								ch[i]='\0';
								copy_name(x,unwanted,ch);
								remove_file(x,unwanted);
							}
							Draw();
						}

					}
					else if(t==1&&val==4)//remove
					{	//call remove
						remove_file(x,unwanted);
						Draw();
					}
					else if((t==0&&val==1)||(t==1&&val==5)) Properties(x);
				    //	t=1;
					break;
				}
				else if(c1==0x12)
				{  	taskbar.Select_Deselect('e',1);
					p=new Popupmenu(Active_Count+2,t);
					val=p->Init();
					delete p;
					taskbar.Select_Deselect('e',0);
					if(t==1&&val==0)//shift
					{       if(Active_Type[x]!=folder)
						{	tempx=x;
							copy_to_clipboard(x,1,unwanted);
						}
						else{Error("NOT SUPPORTED IN THIS VERSION !!");Draw();}

					}
					else if(t==1&&val==1)//copy
					{       if(Active_Type[x]!=folder)
						copy_to_clipboard(x,0,unwanted);
						else{Error("NOT SUPPORTED IN THIS VERSION !!");Draw();}
					}
					else if(val==2)//place
					{       gui_paste();
						if(clipboard[1]=='0')
						{       for(i=0;curr_directory[i]!='\0';i++)
								ch[i]=curr_directory[i];
							ch[i]=0;
							for(k=0,j=2;clipboard[j]!='\0';j++,k++)
							curr_directory[k]=clipboard[j];
							for(;clipboard[j]!=':';j--,k--);
								if(j<=4){j--;k--;}
							curr_directory[k]='\0';
							write(300,400,0,curr_directory);
							x1=get_index(u1);
							if(x1==-1)Error("Already moved or removed");
						       /*	d1=To_String(u1);
							write(400,400,10,d1);
							asm{	mov ax,0
								int 16h;
							   } */
							   else
							remove_file(x1,u1);
							for(i=0;ch[i]!='\0';i++)
							       curr_directory[i]=curr_directory[i];
							curr_directory[i]=0;

						}
						Fly();
						Draw();
					}          //	t=1;
					break;
				}
				else if(c1==0x2f)
				{  	taskbar.Select_Deselect('v',1);
					p=new Popupmenu(Active_Count+3,t);
					val=p->Init();
					delete p;
					taskbar.Select_Deselect('v',0);
					if(val>=0&&val<=2)
					{	Plot_All(val);
						view=val;
						if(view==0) icon.Draw(x,1);
						else if(view==1|| view==2) icon.List_Draw(x,1,view);
					}
					else if(val==3&& curr_directory[0]!='S')
					{	view=0;
						Body_Handler(0,0,1);   //reset now
						return(goup);
					}
					else if(val==4) //settings
					{       Change_Color(0);
						Winini();
						Plot_All(view);
						if(view==0) icon.Draw(x,1);
						else if(view==1|| view==2) icon.List_Draw(x,1,view);
					}
				      //	t=1;
				      break;
				}
				else if(c1==0x4b||c1==0x4d ||c1==0x50||c1==0x48)
				{       if(Active_Count!=0)
					{       t=1;
						x=Body_Handler(c1,c2,0);
						if(c1==0x1c)
						{	if(Active_Type[x]==folder||Active_Type[x]==drive)
							{	view=0;Body_Handler(0,0,1);return x; }
							else if(Active_Type[x]==file)
							{       for(i=0;curr_directory[i]!='\0';i++)
									ch[i]=curr_directory[i];
								ch[i++]=':';
								for(j=0;Active_List[x][j]!='\0';i++,j++)
									ch[i]=Active_List[x][j];
								ch[i]='\0';
								ch[i++]='.';
								ch[i++]='T';
								ch[i++]='X';
								ch[i++]='T';
								ch[i]='\0';
								//write(200,200,0,ch);
								TXT_Reader(ch);
							}
							else if(Active_Type[x]==bmp){}
							else if(Active_Type[x]==unknown){}
							Draw();
						}
						else if(c1==0x01&&c2==0x1b)
						{	t=0;
							break;
						}
						break;
				}
			 else break;
		   }
		   else if(c1==0x01&&c2==0x1b)
		   {	if(view==0) icon.Draw(x,0);
			else if(view==1|| view==2) icon.List_Draw(x,0,view);
			break;
		   }
		   else if(c1==0x1c)
		   {	if(Active_Count!=0)
			{	if(Active_Type[x]==folder||Active_Type[x]==drive)
				{view=0;Body_Handler(0,0,1);return x; }
				else if(Active_Type[x]==file)
				{       for(i=0;curr_directory[i]!='\0';i++)
						ch[i]=curr_directory[i];
					ch[i++]=':';
					for(j=0;Active_List[x][j]!='\0';i++,j++)
						ch[i]=Active_List[x][j];
					ch[i]='\0';
					ch[i++]='.';
					ch[i++]='T';
					ch[i++]='X';
					ch[i++]='T';
					ch[i]='\0';
					//write(200,200,0,ch);
					TXT_Reader(ch);
				}
				else if(Active_Type[x]==bmp){}
				else if(Active_Type[x]==unknown){}
				Draw();
			}
			break;
		   }
		   else break;
	}
	/*	if(c1==0x0e&&c2==0x08)
			break;  */
}
}

char Window ::Body_Handler(char &c1,char&c2,char reset)
{static char now=0;
 char x;
 Icons icon(1);
 char c3=c1,c4=c2,row=0,max=Active_Column[0]-1,min=0,col,i;
 int sum=0;
 if(reset) now=0;
 else
 { for(i=0;;i++)
   {	sum+=Active_Column[i];
	if(sum>now) break;
   }
   min=sum-Active_Column[i];
   max=sum-1;
   if(view==0)
   {    row=i;
	while(1)
	{	if(c3==0x4b &&now>min)//left arrow
		{	icon.Draw(now,0);
			now--;
			icon.Draw(now,1);
		}
		else if(c3==0x4d &&now<max)//Active_Count-1)//right arrow
		{       icon.Draw(now,0);
			now++;
			icon.Draw(now,1);
		//	if(now>max)
		}
		else if(c3==0x50)//down arrow
		{     if(now+Active_Column[row]<=max+Active_Column[row+1]
				&&now+Active_Column[row]<Active_Count )
			{     	icon.Draw(now,0);
				now+=Active_Column[row];
				icon.Draw(now,1);
				row++;
				min=max+1;
				max+=Active_Column[row];
			}
		}
		else if(c3==0x48&&row!=0)//up arrow
		{     if(now-Active_Column[row-1]<min)
			{       icon.Draw(now,0);
				row--;
				now-=Active_Column[row];
				icon.Draw(now,1);
				max=min-1;
				min=min-Active_Column[row];
			}
		}
		asm{   	mov ax,00h
			int 16h
			mov c3,ah
			mov c4,al
		}

		if(c3==0x14||c3==0x21 ||c3==0x12||c3==0x2f||c3==0x01||c3==0x1c)
		{       if(c3==0x01)
			{	icon.Draw(now,0);
				now=0;
			}

			x=now;
			if(c3==0x1c && Active_Type[x]==folder)
			now=0;
			c1=c3;
			c2=c4;
			return x;
		}
	}
   }
   else //if(view==1)
   {    col=i;
	while(1)
	{
		if(c3==0x48&&now>min)//up arrow
		{	icon.List_Draw(now,0,view);
			now--;
			icon.List_Draw(now,1,view);
		 }
		 else if(c3==0x50&&now<max)//Active_Count-1)//down arrow
		 {       icon.List_Draw(now,0,view);
			now++;
			icon.List_Draw(now,1,view);
			   //	if(now>max)
		 }
		 else if(c3==0x4d)//right arrow
		 {     if(now+Active_Column[col]<=max+Active_Column[col+1])
			{	icon.List_Draw(now,0,view);
				now+=Active_Column[col];
				icon.List_Draw(now,1,view);
				col++;
				min=max+1;
				max+=Active_Column[col];
			  }
		 }
		 else if(c3==0x4b&&col!=0)//left arrow
		 {     if(now-Active_Column[col-1]<min)
			{	icon.List_Draw(now,0,view);
				col--;
				now-=Active_Column[col];
				icon.List_Draw(now,1,view);
				max=min-1;
				min=min-Active_Column[col];
			  }
		  }
		  asm{   	mov ax,00h
			int 16h
			mov c3,ah
			mov c4,al
			  }
		if(c3==0x14||c3==0x21 ||c3==0x12||c3==0x2f||c3==0x01||c3==0x1c)
		{       if(c3==0x01)
			{	icon.List_Draw(now,0,view);
				now=0;
			}
			x=now;
			if(c3==0x1c && Active_Type[x]==folder) now=0;
			c1=c3;
			c2=c4;
			return x;
		}
	}
   }
 }
}
/*else */

