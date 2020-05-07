//#include"disk.cpp"
#include"splendor\windows.cpp"
//#include"c:\tc\bin\os\project\bitmap\test2.cpp"
char Desktop_Color,Resolution;
class Desktop
{	public :char back_flag;   //flags whether back image or color
		char *b_properties; //stores color or address of image depending on flag
		int startx,starty;
		Desktop();
		void Desk_Paint();
		int Event_Handler();
};

Desktop::Desktop()
{	back_flag='n';
	*b_properties=Desktop_Color;
	startx=30;
	starty=30;
}

void Desktop::Desk_Paint()
{
	int curr_x=startx,curr_y=starty,i,k,j,count=0; //currx & y rep icon locations & count rep no of entries;
	char *c="C:\\DESKTOP",dt[]={'S','Y','S','T','E','M','\0'};
	Icons icon(0);
	type t;
	Dir_Entries *entries;
	Paint(*b_properties);
	Directory dir(c);
	entries=dir.dir_read();
	char n[4];
	for(i=0;dt[i]!='\0';i++)
	Active_List[0][i]=dt[i];
	Active_Type[0]=system;     //system
	if(entries[0].name[0]=='.')count++;
		if(entries[1].name[0]=='.')count++;
	for(j=count;j<30;j++ )
	{      // Active_List[j-2]=new(char[8]);
		   //
		for(i=0;i<=7&&entries[j].name[i]!=' ';i++)
			Active_List[j-count+1][i]=entries[j].name[i];
		Active_List[j-count+1][i]='\0';
		if(Active_List[j-count+1][0]=='\x0')break;
		for(i=8,k=0;i<=10;i++,k++)
		n[k]=entries[j].name[i];
		n[3]='\0';
		if(n[0]=='T'&&n[1]=='X'&&n[2]=='T') Active_Type[j-count+1]=file;//t=file;
		else if(n[0]=='B'&&n[1]=='M'&&n[2]=='P') Active_Type[j-count+1]=bmp;
		else if(n[0]==' ')Active_Type[j-count+1]=folder;//t=folder;
		else Active_Type[j-count+1]=unknown;//t=unknown;
	}

	Active_Count=j-count+1;
	count=0;
	k=0;
	for (i=0;i<Active_Count;i++)
	{     //Active_Position[i]=new(int[2]);
		  Active_Position[i][0]=curr_x;
		  Active_Position[i][1]=curr_y;
		  icon.Draw(i,0);
		  count++;
		  curr_y+=icon.height+50;  //updating y co-ordinate of icon
		  if(curr_y+icon.height+50>=winy)  // check for ymax of icon
		  {	curr_y=starty;
			curr_x+=icon.width+50;
			Active_Column[k]=count;
			k++;
			count=0;
		  }
	}
	if(curr_y+icon.height+30<winy)  Active_Column[k]=count;
	taskbar.Draw_Bar('t');
	Active_Position[Active_Count][0]=0;
	Active_Position[Active_Count][1]=winy;
}

int Desktop::Event_Handler()
{     	char c1,c2,col=0,max=Active_Column[0]-1,now,min=0,val;
	int t=0;
	Popupmenu *p;
	Icons icon(0);
	now=min;
	while(1)
	{
		 asm{    mov ax,00h
			int 16h
			mov c1,ah
			mov c2,al
		    }
		    if(c1==0x14) //Tasks key
		    {       taskbar.Select_Deselect('t',1);
			    p=new Popupmenu(Active_Count,0);
			    val=p->Init();
			    delete p;
			    taskbar.Select_Deselect('t',0);
			    if(val!=-1) return(-(val+1));
		    }
		    else if(c1==0x48&&now>min)//up arrow
		    {       t=1;
			    icon.Draw(now,0);
			    now--;
			    icon.Draw(now,1);
		    }
		    else if(c1==0x50&&now<max)//Active_Count-1)//down arrow
		    {   t=1;
			icon.Draw(now,0);
			now++;
			icon.Draw(now,1);
			   //	if(now>max)
		    }
		     else if(c1==0x4d)//right arrow
		     {  t=1;
			if(now+Active_Column[col]<=max+Active_Column[col+1])
			  {	icon.Draw(now,0);
				now+=Active_Column[col];
				icon.Draw(now,1);
				col++;
				min=max+1;
				max+=Active_Column[col];
			  }
		     }
		     else if(c1==0x4b&&col!=0)//left arrow
		     {   t=1;
			if(now-Active_Column[col-1]<min)
			  {	icon.Draw(now,0);
				col--;
				now-=Active_Column[col];
				icon.Draw(now,1);
				max=min-1;
				min=min-Active_Column[col];
			  }
		     }
		     else if(c1==0x1c&&t==1)//enter key
		     return now;
	}
}
void Initialize()
{	Desktop_Color=54;
	Resolution=2;
	win.fcolor=0;
	win.bcolor=15;
}

void gui()
//void main()
{   	int c,i,k=8;
	int x=xmax/2-150,y=ymax/2-100,ht=120,wd ;
	char n[]="C:\\DESKTOP:",s[]="SYSTEM",ch[50],*z;
	int flag=0;
	d[0].initialise(2);
	d[1].initialise(3);
	Initialize();//loading default values for settings
	init(Resolution); // setiing mode and initializing Graphics
	load_font(3);
      /*	curr_dir.drive = 2;
	curr_dir.dir_lba = d[0].rootdir_lba;
	 curr_dir.dir_details.lba =  curr_dir.dir_lba ;
	 curr_dir.found = 1;  */
	 path[0]='C';
	 path[3] = 0;
       //	PIC_Viewer();
	/*asm{   	mov ax,00h
		int 16h
	   }           */

	while(1)
	{       for(i=0;i<20;i++)
		curr_directory[i]=n[i];
		{	Desktop d;
			d.Desk_Paint();
			c=d.Event_Handler();
		}
		if(c>=0)
		{	if(Active_Type[c]==system ||Active_Type[c]==folder)
			{	if(c!=0)   k=11;
				else k=0;
				Window w(ch);
				//w=new Window(ch);
				while(1)
				{	if(c!=goup)
					{       if(curr_directory[0]=='S')
						{	for(i=0;i<3;i++)
							curr_directory[i]=n[i];
						       //	if(c==0) curr_directory[0]='A';
							curr_directory[i]='\0';
							k=i;

						}
						else
						{

							for(i=0;Active_List[c][i]!='\0';i++,k++)
							curr_directory[k]=Active_List[c][i];
							curr_directory[k]='\0';

						}
					}
					for(i=0;curr_directory[i]!='\0';i++)
					ch[i]=curr_directory[i];
					ch[i]='\0';
					if(flag==0)
					w.Draw();
					else
					{	w.Change_Title(ch);
						w.Plot_All(w.view);
						flag=1;
					}
					c=w.Event_Handler();
					if(c==goup)
					{      if(k==3)
						{	for(i=0;s[i]!='\0';i++)
							curr_directory[i]=s[i];
							curr_directory[i]='\0';
						}
						else
						{


							for(i=k;curr_directory[i]!=':'&&i>0;i--);
							if(i==1)
							{	curr_directory[++i]='\\';
								i++;
							}
							curr_directory[i]='\0';
						}
						k=i;
					}
				     /*	else if(Active_Type[c]==file)
					*/
					//else if(Active_Type[c]!=folder&&Active_Type[c]!=drive)break;
					else if (c==exit||c==turnoff)break;
					else
					{    if(curr_directory[k-1]!='\\')
					     {

						curr_directory[k]=':';
						k++;
					     }
					}
				}
			       //	delete w;
			}
			else if(Active_Type[c]==file)
			{       k=11;
				for(i=0;Active_List[c][i]!='\0';i++,k++)
					curr_directory[k]=Active_List[c][i];
				curr_directory[k]='\0';
				for(i=0;curr_directory[i]!='\0';i++)
					ch[i]=curr_directory[i];
				ch[i++]='.';
				ch[i++]='T';
				ch[i++]='X';
				ch[i++]='T';
				ch[i]='\0';
			     //write(200,200,0,ch);
				TXT_Reader(ch);
			}
		}
		else  //Selection from popup menu
		{   switch(c)
			{	case -1: Shade();
					 int h=Turn_Off();
					  x=xmax/2-150;
						y=ymax/2-100;
						ht=120;
						wd=350;
						Message_Window(x,y,wd,ht,"TURNING OFF....... ");
						write(x+50,y+65,4,"QUITTING SPLENDOR , BYE !!!!!!");
						asm{    mov  ah,00h;
							int  16h;
		       //	mov c1,ah;
						}
						closesvga();
						return;
					 break;  //turn off
				case -2: closesvga();shell();load();init(Resolution);break;  //matrix shell
				case -3: break;  //pic viewer
				case -4: z=Get_File();
					if(z[0]!='e')
					{
						for(i=0;z[i]!='\0';i++)
						ch[i]=z[i];
						ch[i]='\0';
						if(ch[i-1]=='T'&&ch[i-2]=='X'&&ch[i-3]=='T'&&ch[i-4]=='.')
						TXT_Reader(ch);
						else
						{       x=xmax/2-150;
							y=ymax/2-100;
							ht=120;
							wd=400;
							Message_Window(x,y,wd,ht,"ERROR !!!");
							load_font(1);
							write(x+110,y+60,4,"Format  Not  Supported ");
							load_font(3);
							asm{   	mov ax,00h
								int 16h
							   }
						}
					}
					 break; //reader
				case -5: Change_Color(1);
					break;
				case -6: Change_Resolution();
					 break;
			}
		}
		if(c==-1)break;
	}
    //	Button(100,170,0);
}
