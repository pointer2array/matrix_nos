#ifndef _inout_
#define _inout_

char page=0;
char attrib=15;
char scancode;
int x=1,y=3;
char priority=0;

const char  horiz_line = 'Ä';
const char  vert_line  = '³';

// To get the String length in bytes
unsigned long int strlen(char *s1)
 {
	  unsigned long int counter =0;
	while(*s1)
	{
		    counter++;
		s1++;
	}
	return counter;
 }

// To compare two strings [ 0 for <> & 1 for == ]
char streq(char *s1,char *s2)
{
 int i=0;
 if(strlen(s1)==strlen(s2))
  {
  for(;i<strlen(s1);++i)
  {
  if((s1[i]!=s2[i])&&(s1[i]!=s2[i]-0x20))
  return 0;
  }
  return 1;
  }
 else if(strlen(s1) > strlen(s2))
  {
  for(;i<strlen(s2)/* && s1[i] != ' '*/;++i)
  {
  if(s1[i] != ' ' && (s1[i]!=s2[i] &&(s1[i]!=s2[i]-0x20)))
  return 0;
  }
 if(s2[0]!=0) return 1;
 }
 return 0;
}

// To join 2nd string with 1st string
void joinstring(char a[50],char b[10])
{
int i,j=0;
i=(unsigned int) strlen(a);
while(b[j]!=0&&j<10) {a[i]=b[j];i++;j++;}
a[i++]= '\\';
a[i++]=0;
}

// To remove characters from a source string
// till an input character is reached from reverse
void substring(char a[50],char b)
{
int l;
l=(unsigned int)strlen(a)-2;
while(a[l]!=b) {a[l]=0;l--;}
}

// To specify the attribute of the text
void setattrib(int color)
 {
 attrib = color;
 }

// To return the color
char color(char bg,char fg)
  {
  return (bg*16) + fg;
  }

// To reboot OS
void mtx_reboot()
 {
  asm int 0x19
 }

// To set the video mode
void mtx_setvideo(char m) /* m = the mode */
 {    	asm	mov ah , 0
	asm	mov al , m
	asm	int 0x10
 }
// To print a character
void mtx_printchar(char c)
 {
	asm mov ah , 0x09
	asm mov al , c
	asm mov bh , page
	asm mov bl , attrib
	asm mov cx , 1
	asm int 0x10
 }

// To set active display page
void set_display_page(char pg)
{
asm mov ah,0x05
asm mov al,pg
asm int 0x10
}

// To perform delay
void wait(unsigned long int d)
 {
    unsigned int a=0,b;
       b=(unsigned int)d;
	  if(d>65535l)
       {
       a=(unsigned int)(d/65535l);
	  d=65535l;
       }
       asm    mov ah,0x86
       asm    mov dx,b
       asm    mov cx,a
       asm    int 0x15
 }

// To get a character
void mtx_getkey(char *key,char *scancode)
 {
       char a,b;
	asm	mov ah , 0
	asm	int 0x16
	asm	mov a, al
	asm	mov b, ah
	*key = a ;
	*scancode = b;
 }

// To set cursor position
void mtx_setcursor(char x,char y)
 {
       asm    mov ah , 0x02
	asm	mov bh , page
	asm	mov dh , y
	asm	mov dl , x
	asm	int 0x10

 }

// Go to cursor position specified
void gotoxy(char x,char y)
 {
      x = (x>=1&&x<=80) ? x : 1 ;
      y = (y>=1&&y<=25) ? y : 1 ;
      mtx_setcursor(x,y);
 }

// To draw a horizontal line
void   drawhline(char  x,char  y,char  l,char  color)
 {	int i=0;
	gotoxy(x,y);
	setattrib(color);
	for(;i<l;++i){
		gotoxy(x++,y);
		mtx_printchar((char )horiz_line);
	}
 }
// To draw a vertical line
void   drawvline(char  x,char  y,char  l,char  color)
 {	int i=0;
	gotoxy(x,y++);
	setattrib(color);
	for(;i<l;++i){
		mtx_printchar((char )vert_line);
		gotoxy(x,y++);
	}
 }

// To draw a rectangle
void   box(char  x1,char  y1,char  x2,char  y2,char  lb,char  db)
 {		drawhline(x1+1,y1  ,x2-x1-1,lb);
		drawvline(x1  ,y1+1,y2-y1-1,lb);
		drawhline(x1+1,y2  ,x2-x1-1,db);
		drawvline(x2  ,y1+1,y2-y1-1,db);
		setattrib(lb);
		gotoxy(x1,y1);
		mtx_printchar('Ú');
		gotoxy(x1,y2);
		mtx_printchar('À');
		setattrib(db);
		gotoxy(x2,y1);
		mtx_printchar('¿');
		gotoxy(x2,y2);
		mtx_printchar('Ù');
 }


// To scrollup the screen
void mtx_scrollup(char nol,char x1,char y1,char x2,char y2)
 {	asm	mov ah , 0x06
	asm	mov al , nol
	asm	mov cl , x1
	asm	mov ch , y1
	asm	mov dl , x2
	asm	mov dh , y2
	asm	mov bh , attrib
	asm	int 0x10
 }

// To get response from the user
void getchar()
 {
       char k ;
	mtx_getkey(&k,&scancode);
 }

// To input a character
char getch()
 {
       char k ;
	mtx_getkey(&k,&scancode);
	return k;
 }

//To detect a keyboard hit  [ 0 for No keyhit & 1 for Keyhit ]
char keyhit()
{
asm mov ah,0x01
asm int 0x16
asm jz re
return 0;
re:
return 1;
}

// To print the String at corresponding (x,y) coordinates
void printxy(char *s,char x,char y)
 {
       while(*s)
	{
	      if(*s=='\n')
		{
		     mtx_printchar('\n');
			mtx_printchar('\r');
			++s;
			gotoxy((x=0),++y);
		}
		if(*s=='\t')
		{
		     gotoxy(x++,y);mtx_printchar(' ');
		     gotoxy(x++,y);mtx_printchar(' ');
		     gotoxy(x++,y);mtx_printchar(' ');
		     gotoxy(x++,y);mtx_printchar(' ');
		     gotoxy(x++,y);mtx_printchar(' ');
		     gotoxy(x++,y);mtx_printchar(' ');
		     gotoxy(x++,y);mtx_printchar(' ');
		     ++s;
		}
		else
	      {
			gotoxy(x++,y);
			mtx_printchar(*s);
			s++;
		}
	}
 }

 // To clear screen
void clear(int color)
 {
	setattrib(color);
	mtx_scrollup(0,0,0,79,24);
 }

// To display the character
void putchar(char s)
 {
	gotoxy(x,y);
	if(s!='\t')
	if(s==13)
	{
	x = 0;
	if( y>=24)
		{
		 mtx_scrollup(1,0,3,79,24);
		 y = 24;
		}
		else  y++;
		gotoxy(++x, y);
	}
	else
	{
	if( x<79)
	{
	mtx_printchar(s);
	gotoxy(++ x, y);
	}
	else
	{
		if( y<24)
		{
		x = 0;
		y+= 1;
		gotoxy(++ x, y);
		mtx_printchar(s);
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

 // To display the string
void putmsg(char *s)
 {
	  gotoxy(x,y);
	  if( y<=24)
		 printxy(s,x,y);
	else
	  {
	   mtx_scrollup(1,0,3,79,24);
	   printxy(s,1, y=24);
	}
 }

 // To display error message
void error(char *msg)
{
setattrib(color(0,14));
putmsg(msg);
}

// To get a string with specified maximum length
void get_string(char len,char str[10])
{
char c,cnt=0;
c=getch();
while((cnt<len)&&(c!=13))
 {
 putchar(c);
 str[cnt]=c;
 c=getch();
 cnt++;
 };
str[cnt]=0;
}

// To get a string with wild-card character display
void get_stringw(char len,char str[10],char wild)
{
char c,cnt=0;
c=getch();
while((cnt<len)&&(c!=13))
 {
 putchar(wild);
 str[cnt]=c;
 c=getch();
 cnt++;
 };
str[cnt]=0;
}

// To get user name & password
void get_pas(char user[10],char password[10])
{
box(27,9,49,11,8,7);
box(27,12,49,14,8,7);
printxy("USER",22,10);
printxy("PASSWORD",18,13);
x=31;y=10;
gotoxy(x,y);
get_string(10,user);
x=31;y=13;
gotoxy(x,y);
get_stringw(10,password,'*');
}

#endif