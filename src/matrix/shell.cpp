#ifndef _shell_
#define _shell_

# include "fs.cpp"
# include "inout.cpp"
# include "enigma.cpp"
# include "rs232.cpp"
//# include "splendor\desktop.cpp"
//# include "run.c"
# include "thd.cpp"
# include "pm2.cpp"
# define buff_limit 50
# define kill 1
# define reboot 2
# define FLOPPY 1
# define HARDD 2
extern "C" void run13();
char floppy_hardd_tog = FLOPPY;
void change(char*);
char *cprompt=" ",
	*head="                         MATRIX OPERATING SYSTEM",
	*path="A:\\";
int cmd=-1,cnt=0,mnt=0;
//int ccs,cip;
char cmd_buff[buff_limit][70],cmd_parser_buff[300];
char prompt();
void get_time();
void get_date();
int up(int,char*);
int down(int,char*);
int command();
void version();
void credits();
void help();
void dir();
void ls();
void shell();
void load();
void gui();

void thread(void)   ;

			    /* ---------------- RUN ______________*/
			    //	void shell();
//void load();
extern "C" void run13();
int ccs,cip;
int cret();


void run()//run
{
	    //floppy reset----------
		asm xor ah,ah;
		asm mov dl,0x00;
		asm int 0x13;
		//---------------------

    asm  push si        // req by tcc asm
    asm  push di        //""
    //asm  pusha
    ccs=_CS;
    cip=(int)load;
    run13();
}

 cret()
 {
    asm pop ax
    asm pop ax
    asm pop di
    asm  pop si

	//check return------------
	asm mov ah , 0x09
	asm mov al , 't'
	asm mov bh , 0
	asm mov bl , 0x16
	asm mov cx , 34
	asm int 0x10
	//---------------------------

    return 0;
}

			    /* ---------------- RUN ______________*/

//void gui();

// To display the shell
void shell()
{
  x = 1;
  y = 3;
  clear(color(0,15));
  setattrib(color(0,2));
  printxy(head,1,1);
  drawhline(1,2,78,color(0,2));
  cmd_parser_buff[0] = 0;
}

// To input and process command
void load()
{
	  Error_Code = 0;
	  char message = 0;
	  while((message=prompt())!=0)
	{
		 if(message==kill)
		{
			clear(color(0,15));
			printxy("Terminating system ",1,1);
			printxy("Breaking to main()..",1,2);
			break;
		}
		 else if(message==reboot)
		{
			clear(color(0,15));
			printxy("Terminating system ",1,1);
			printxy("Rebooting..",1,2);
			mtx_reboot();
		}
		else if(message==100)
		{
			error("Not A Valid Command. Type `help'.");
				putchar('\r');
		}
		if(Error_Code)
		{
		switch(Error_Code)
		{
		case 1: error("File_Not_Found");break;
		case 2: error("Folder_Not_Found");break;
		case 3: error("Destination_File_Exist");break;
		case 4: error("Destination_Folder_Exist");break;
		case 5: error("Access_Violation");break;
		case 6: error("No_Disk_Space");break;
		case 7: error("Des_Path_Not_Exist");break;
		case 8: error("Hard_Disk_Not_Mounted");break;
		}
		putchar(13);
		Error_Code = 0;
		}
	}
 }


// To input a command and return corresponding result
char  prompt()
{
  char    c = 0 , *cmdptr =  cmd_buff[++cmd] ;
  int   counter = 0,ix;
  setattrib(color(0,2));
  //if(y>25) {x=1;y=3;}
  if( y>=24)
       {
	mtx_scrollup(1,0,3,79,24);
	y = 24;
       }
  printxy(path, x=1, y);
  x+=(unsigned int)strlen(path);
    putchar('@');
  printxy(curr_user, x , y);
  x+=(unsigned int)strlen(curr_user);
  printxy(cprompt, x, y);
  x+=(unsigned int)strlen(cprompt);
  ix=x+1;
  gotoxy(++x, y);
  if(cmd == buff_limit) {cmd = 0;}
  while(c!=13) /* EOL */
	{
	c = getch();
	if(c==8) /* Back space */
	 {
	 if((counter>0) && (x>1))
		{
		 cmdptr[--counter]=0;
		 gotoxy(-- x, y);
		 mtx_printchar(0);
		 }
	  else if( x==0)
		{
		 gotoxy(( x=79),-- y);
		 mtx_printchar(0);
		 }
	  }
	 else if(c==13) /* ENTER */
		{
		 x=1;
		 cnt=0;
		 break;
		 }
	 else if((c==0)&&(scancode==72))
		{
		counter=up(ix,cmdptr);
		}
	 else if((c==0)&&(scancode==80))
		{
		counter=down(ix,cmdptr);
		}
	 else
		{
		if( x<79)
			{
			   mtx_printchar(c);
			   gotoxy(++ x, y);
			   cmdptr[counter++] = c;
			 }
		else
			{
			   if( y<24)
			       {
				x = 0;
				y+= 1;
				gotoxy(++ x, y);
				mtx_printchar(c);
				cmdptr[counter++] = c;
				gotoxy(++ x, y);
				}
			    else
				{
				x = 0;
				mtx_scrollup(1,0,3,79,24);
				gotoxy(++ x, y=24);
				}
			  }
		}
	}
  if( y>24)
	{
	 mtx_scrollup(1,0,3,79,24);
	 y = 24;
	}
  else  y++;
return  command();
}

// To view previous command
int up(int ix,char *cmdptr)
{
 static int i;
 if(++cnt==buff_limit)cnt = 0;
 if((cmd-cnt)>=0)
       {
	  i=0; x=ix;
       printxy("                      ",x,y);
       gotoxy(x, y);
       while(cmd_buff[cmd-cnt][i]!=0)
	       {
		cmdptr[i]=cmd_buff[cmd-cnt][i];
		i++;
		}
	cmdptr[i]=0;
	printxy(cmdptr,x,y);
	x+=(unsigned int)strlen(cmdptr);gotoxy(x,y);
	return (unsigned int)strlen(cmdptr);
	}
return 0;
}

// To view next command
int down(int ix,char *cmdptr)
{
 static int i;
 if(--cnt<0) cnt = 0;
 if(cnt<=cmd)
       {
       i=0; x=ix;
	  printxy("                      ",x,y);
       gotoxy(x, y);
       while(cmd_buff[cmd-cnt][i]!=0)
	       {
		cmdptr[i]=cmd_buff[cmd-cnt][i];
		i++;
	       }
       cmdptr[i]=0;
       printxy(cmdptr,x,y);
       x+=(unsigned int)strlen(cmdptr);gotoxy(x,y);
       return (unsigned int)strlen(cmdptr);
       }
return 0;
}

// To identify and process command
int  command()
 {
  int i=0;
  char *fhad;
  char command[100];
  char *temp;
  for(i=0;i<100;i++) command[i] = 0;
  i = 0;
  fhad = command;
  for(;i<100 && cmd_buff[cmd][i]!=13 && cmd_buff[cmd][i];++i)
	  {      command[i] =  cmd_buff[cmd][i];      }
  command[i] = 0;
  if(command[0]==0)
    {
   // putchar(13);
    return 10;
    }
 else if(streq(command,"clear")==1||streq(command,"cls")==1)
	{
	 shell();
	 return 10;
	}
  else if(streq(command,"dir")==1)
	{
	 dir();
	 return 10;
	}
	else if(streq(command,"ls")==1)
	{
	 ls();
	 return 10;
	}
	else if(streq(command,"ver")==1)
	{
	 version();
	 return 10;
	}
	else if(streq(command,"gui")==1)
	{
	 asm push es
	 gui();
	 page=0;
	 Error_Code=0;
	 asm pop es
	 shell();
	 //putchar(13);
	 return 10;

	}
	else if(streq(command,"adduser")==1)
	{
	 adduser();
	 return 10;
	}
	else if(streq(command,"remusers")==1)
	{
	 remusers();
	 return 10;
	}

	else if(streq(command,"remuser")==1)
	{
	 remuser();
	 return 10;
	}
	else if(streq(command,"thread")==1)
	{
	 thread();
	 shell();
	 return 10;
	}
	else if(streq(command,"credits")==1)
	{
	 credits();
	 return 10;
	}
	else if(streq(command,"time")==1)
	{
	 get_time();
	 return 10;
	}
	else if(streq(command,"date")==1)
	{
	 get_date();
	 return 10;
	}
	else if(streq(command,"type")==1)
	{

	 fhad+=5;
	 if(strlen(fhad)==0)
	     {
	     error("No filename specified");
		putchar('\r');
	     }
	 else types(fhad);
	 return 10;
	}
	else if(streq(command,"cp")==1)
	{
	 temp = command;
	 temp += 3;
	 while(*temp != ' ') temp++;
	 temp++;
	 if(protect(temp)&&protect(fhad+3))
		 copy(fhad+=3,temp);
	 return 10;
	}
	else if(streq(command,"mv")==1)
	{
	 temp = command;
	 temp += 3;
	 while(*temp != ' ') temp++;
	 temp++;
	 if(protect(temp)&&protect(fhad+3))
	 move(fhad+=3,temp);
	 return 10;
	}
	else if(streq(command,"ren")==1)
	{
	 temp = command;
	 temp += 4;
	 while(*temp != ' ') temp++;
	 temp++;
	 if(protect(temp)&&protect(fhad+4))
	 move(fhad+=4,temp);
	 return 10;
	}
else if(streq(command,"run")==1)
	{    	run();
		return 10;
			}
	else if(streq(command,"switch")==1)
	{
	 pmrm();
	 getch();
	 clear(color(0,15)) ;
	 getchar();
	 shell();
	 load();
	 return 10;
	}
	else if(streq(command,"make")==1)
	{
	 fhad+=5;
	 if(protect(fhad))
	 if(strlen(fhad)==0)
		{
		error("No filename specified");
		putchar('\r');
		}
	 else edit(fhad);
	 return 10;
	}
	else if(streq(command,"C:") == 1)
	{
	   //putchar(13);
	   if(mnt==0)
	   {
	   Error_Code =8;
	   return 10;
	   }
	   else
	   {
	    floppy_hardd_tog  = HARDD;
	    curr_dir.drive = 2;
	    curr_dir.dir_lba = d[0].rootdir_lba;
	    curr_dir.dir_details.lba =  curr_dir.dir_lba ;
	    curr_dir.found = 1;
	    path[0]='C';
	    path[3] = 0;
	    return 10;
	   }
	}
	else if(streq(command,"A:") == 1)
	{
	     //		putchar(13);
			floppy_hardd_tog  = FLOPPY;
			ch = 1,ct = 0,cs = 2;
			ccls = 0x0000;
			 path[0]='A';
			 path[3] = 0;
			 return 10;

	}
	else if(streq(command,"cd")==1)
	{

	 change(fhad+=3);
	 return 10;
	}
	else if(streq(command,"cd..")==1)
	{
	 change("..");
	 return 10;
	}
	else if(streq(command,"mount")==1)
	{
	 //putchar(13);
	 if(streq(command+6,"hd0")==1)
	  {
	  Drive_No=0x80;
	  mount();
	  mnt=1;
	  }
	 else if(streq(command+6,"hd1")==1)
	  {
	  Drive_No=0x81;
	  mount();
	  mnt=2;
	  }
	 else return 100;
	 /*curr_dir.drive = 2;
	 curr_dir.dir_lba = d[0].rootdir_lba;
	 curr_dir.dir_details.lba =  curr_dir.dir_lba ;
	 curr_dir.found = 1;
	 path[0]='C';
	 path[3] = 0; */
	 return 10;
	}
	else if(streq(command,"rm")==1)
	{
	if(protect(fhad+3))
	 rem(fhad+=3);
	 return 10;
	}
	else if(streq(command,"md")==1)
	{
	 md(fhad+=3);
	 return 10;
	}
	else if(streq(command,"setsid")==1)
	{
	 set_sid(fhad+=7);
	 return 10;
	}
	else if(streq(command,"getsid")==1)
	{
	 get_sid();

	 return 10;
	}

	else if(streq(command,"listen")==1)
	{
	 listen();
	 return 10;
	}

	else if(streq(command,"ping")==1)
	{
	 ping(fhad+=5);
	 return 10;
	}
	else if(streq(command,"send")==1)
	{
	 send_msg();
	 return 10;
	}
	else if(streq(command,"get")==1)
	{
	 receive_msg();
	 return 10;
	}
	else if(streq(command,"help")==1)
	{
	 help();
	 return 11;
	}
	else if(streq(command,"shutdown")==1)
	{
	 return 1;
	}
	else if(streq(command,"sd")==1)
	{
	 return 1;
	}
	else if(streq(command,"reboot")==1)
	{
	 return 2;
	}
	return 100;
}

// To view Matrix OS commands integrated
void  help()
{
 int xx,yy;
 xx=x;yy=y;
 page=1;
 x=1,y=1;
 set_display_page(page);
 clear(color(0,2));
 setattrib(color(0,130));
 putmsg("                           Commands");x=12;
 drawhline(1,2,78,color(0,2));
 setattrib(color(0,13));
 x=1;y=4;
       putmsg("cls                       - clears and refreshes the console.");
   y++;putmsg("help                      - shows this screen");
   y++;putmsg("shutdown/sd               - shuts down the system");
   y++;putmsg("reboot                    - reboot the system");
   y++;putmsg("dir                       - list files and directories");
   y++;putmsg("ls                        - list files");
   y++;putmsg("md                        - make directory");
   y++;putmsg("cd                        - change directory");
   y++;putmsg("type <file name>          - display file");
   y++;putmsg("make <file name>          - create file");
   y++;putmsg("rm <file name>            - delete file");
   y++;putmsg("cp <destination> <source> - copy file");
   y++;putmsg("mv <destination> <source> - move file");
   y++;putmsg("ren <new> <old>           - rename file");
   y++;putmsg("mount <disk>              - mounts hard disk");
   y++;putmsg("setsid <sid>              - sets system ID");
   y++;putmsg("getsid                    - gets system ID");
   y++;putmsg("adduser                   - add user");
   y++;putmsg(" 					");
   y++;putmsg("MORE");
   getchar();
   clear(color(0,2));
   setattrib(color(0,130));
   x=1,y=1;
  putmsg("                            Commands");x=12;
  drawhline(1,2,78,color(0,2));
  setattrib(color(0,13));
  x=1;y=4;
   y++;putmsg("remuser                   - remove user");
   y++;putmsg("remusers                  - remove all users");
   y++;putmsg("listen                    - listern to com port");
   y++;putmsg("ping                      - detect other system by system id");
   y++;putmsg("gui                       - switch to graphical mode");
   y++;putmsg("time                      - get time");
   y++;putmsg("date                      - get date");
   y++;putmsg("thread                    - start threaded program");
   y++;putmsg("run                       - run external executable");
   y++;putmsg("switch                    - switch between PM AND REAL");
   y++;putmsg("credits                   - display credits");
   y++;putmsg("ver                       - display version");
   y++;putmsg("send                      - send messages");
   y++;putmsg("get                       - receive messages");
   y++;putmsg(" ");
 getchar();
 x=xx;y=yy;
 page=0;
 set_display_page(page);
 }

// To set the path
void change(char *str)
 {
 if(cd(str))
    {
    if(streq(str,"..")==1)  substring(path,'\\');
    else joinstring(path,str);
    }
  else Error_Code = 2;
 }

// To display the version of MATRIX OS
void version()
 {
  setattrib(color(0,10));
  putmsg("MATRIX 0.0.1");
  putchar('\r');
 }

// To view the Credits of MATRIX OS
void credits()
 {
  setattrib(color(0,10));
  putmsg("MATRIX Operating System Development Team :-");putchar('\r');
  setattrib(color(0,4));
  putmsg("|T|hiruvambalam Sreenivas");putchar('\r');
  setattrib(color(0,11));
  putmsg("|T|harun Jacob John");putchar('\r');
  setattrib(color(0,13));
  putmsg("|T|ony D.Titus");putchar('\r');
  setattrib(color(0,6));
  putmsg("Bidhun K.");putchar('\r');
 }

// To convert BCD to character
char convert(char c)
{
return(c+'0');
}

// To display the CMOS time
void get_time()
{
char ch,cl,dh;
  asm{
  mov ah,0x02
  int 0x1a
     }
ch=_CH;cl=_CL;dh=_DH;
putmsg("Time:: ");x+=8;
putchar(((ch&0xf0)>>4)+'0');putchar((ch&0x0f)+'0');
putchar('h');putchar(':');
putchar(((cl&0xf0)>>4)+'0');putchar((cl&0x0f)+'0');
putchar('m');putchar(':');
putchar(((dh&0xf0)>>4)+'0');putchar((dh&0x0f)+'0');putchar('s');
putchar(13);
}

// To print the CMOS date
void get_date()
{
char cl,dh,dl;
  asm{
  mov ah,0x04
  int 0x1a
     }
cl=_CL;dh=_DH;dl=_DL;
putmsg("Date:: ");x+=8;
putchar(((dl&0xf0)>>4)+'0');putchar((dl&0x0f)+'0');
putchar('/');
putchar(((dh&0xf0)>>4)+'0');putchar((dh&0x0f)+'0');
putchar('/');
putchar('2');putchar('0');
putchar(((cl&0xf0)>>4)+'0');putchar((cl&0x0f)+'0');
putchar(13);
}

#endif