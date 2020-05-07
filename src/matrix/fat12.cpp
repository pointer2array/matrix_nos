#ifndef _file_
#define _file_
#include "inout.cpp"
#include "fs.cpp"
char* datread(unsigned int);
unsigned int getcnum(unsigned int cnum);
void display(char buffer[512]);

//Reset a floppy
void resetflop()
{
 asm xor ah,ah;
 asm mov dl,0x00;
 asm push ds
 asm int 0x13;
 asm pop es
}

//Reads from a floppy
int flopread(char h,char t,char s,char n,char *p)
{
 char num;
 asm mov cx,0x3;
 lp:
 asm push cx;
 asm mov ah,0x02
 asm mov al,n
 asm mov ch,t
 asm mov cl,s
 asm mov dh,h
 asm mov dl,0x0
 asm mov bx,p
 asm push ds
 asm int 0x13
 asm pop es
 asm jnc nerror;
 num=0;
 asm mov num,ah
 resetflop();
 asm pop cx;
 asm dec cx;
 asm jnz lp;
 asm mov num,ah
 return num;
 nerror:
 asm mov num,ah
 return num;
 }

//Writes to a floppy
int flopwrite(char h,char t,char s,char n,char *p)
{
 char num;
 resetflop();
 asm mov cx,0x03;
 lp:
 asm push cx;
 asm mov ah,0x03
 asm mov al,n
 asm mov ch,t
 asm mov cl,s
 asm mov dh,h
 asm mov dl,0x0
 asm mov bx,p
 asm push ds
 asm int 0x13
 asm pop es
 asm pop cx;
 asm jnc nerror;
 asm clc
 asm loop lp;
 nerror:
 asm mov num,ah
 return num;
}


//Checks a floppy
unsigned char flopcheck()
{
 unsigned char c;
 resetflop();
 asm mov ah,0x01
 asm mov dl,0x00
 asm push ds
 asm int 0x13
 asm pop es
 asm mov c,al
 if(c)
  {
  asm mov ah,0x0
  asm mov dl,0x0
  asm int 0x13
  asm mov ah,0x01
  asm mov dl,0x00
  asm int 0x13
  asm mov c,al
  }
 return c;
}

//To get data
void fat(unsigned int index)
 {
  static unsigned int cnum=0;
  cnum = index;
  do
   {
    display(datread(cnum));
    cnum = getcnum(cnum);
   }
  while(cnum<0xff8);
}


//To display data
void display(char buffer[512])
{
 static count=0;
 int i,j;
 for(i=0;i<512;i++)
 {
  if(buffer[i]==0) {count=0;break;}
  if(buffer[i]!='\n') {count++;putchar(buffer[i]);}
  else {count+=(80-x);putchar('\r');}
  if(count>1600) {count=0;getchar();}
 }
}


//To get cluster number
unsigned int getcnum(unsigned int cnum)
{
 static	int check;
 static	char buffer[512];
 static	char t=0,s=2,n=1,h=0;
 t=0,s=2,n=1,h=0;
 flopread(h,t,s,n,buffer);
 check=cnum%2;
 if(check > 0)
  {
  cnum=(unsigned int)((((unsigned int)buffer[((cnum*3)/2)+1]<<8)+((unsigned char)buffer[((cnum*3)/2)]))>>4);
  }
 else
  {
  cnum=(unsigned int)((((unsigned int)buffer[((cnum*3)/2)+1]<<8)+((unsigned char)buffer[((cnum*3)/2)]))&0x0fff);
  }
 return cnum;
}


//Read a fat entry
int fatread(int cnt,unsigned int buf[])
{
 static unsigned char buffer[1536];
 static int check;
 unsigned int t1,t2,snum,num,c=0;
 flopread(0,0,(cnt/3)*3+2,3,buffer);
 if((cnt%3)==0) {snum = 0;check=0;num=341;}
 if((cnt%3)==1) {snum = 511;check=1;num=341;}
 if((cnt%3)==2) {snum = 1023;check=0;num=342;}
 while(snum<=num)
 {
  if(check > 0)
  {
   buf[c++]=(unsigned int)((((unsigned int)buffer[((snum*3)/2)+1]<<8)+((unsigned char)buffer[((snum*3)/2)]))>>4);
  }
  else
  {
   buf[c++]=(unsigned int)((((unsigned int)buffer[((snum*3)/2)+1]<<8)+((unsigned char)buffer[((snum*3)/2)]))&0x0fff);
  }
  snum++;
  check=check>0?0:1;
 }
return(num);
}


//Set a fat entry
void fatwrite(unsigned int cl1,unsigned int cl2)
{
 static unsigned char buffer[1536];
 static int check;
 static unsigned int cnt;
 cnt=cl1/1024;
 cl1%=1024;
 flopread(0,0,(cnt*3)+2,3,buffer);
 if((cnt%3)==0) check=0;
 else if((cnt%3)==1) check=1;
 else if((cnt%3)==2) check=0;
 if(cl1%2) check=check>0?0:1;
 if(check > 0)
 {
  buffer[((cl1*3)/2)+1]=cl2>>4;
  buffer[((cl1*3)/2)]|=((cl2&0x000f)<<4);
 }
 else
 {
  buffer[((cl1*3)/2)]=cl2;
  buffer[((cl1*3)/2)+1]|=((cl2&0x0f00)>>8);
 }
 flopwrite(0,0,(cnt*3)+2,3,buffer);
}

//Read data from floppy
char* datread(unsigned int cnum)
{
 static char buffer[512];
 static int i,h,s,t,n=1;
 static unsigned int spt = 18,noh = 2;
 static unsigned int secaddress;
 static unsigned int laddress = 33;
 for(i=0;i<512;i++)buffer[i] = 0;
 secaddress = laddress + cnum-1;
 s = ((secaddress)%spt);
 h = (secaddress/spt)%noh;
 t = (secaddress/(spt*noh));
 flopread(h,t,s,n,buffer);
 return buffer;
}


//Write data to floppy
void datwrite(unsigned int cnum,unsigned char buffer[512])
{
 static int h,s,t,n=1;
 static unsigned int spt = 18,noh = 2;
 static unsigned int secaddress;
 static unsigned int laddress = 33;
 secaddress = laddress + cnum-1;
 s = ((secaddress)%spt);
 h = (secaddress/spt)%noh;
 t = (secaddress/(spt*noh));
 flopwrite(h,t,s,n,buffer);
}


//Clear a fat entry
void fatclear(unsigned int index)
{
 static unsigned int cnum=0;
 static unsigned int tcnum=0;
 tcnum = cnum = index;
 do
 {
  tcnum = getcnum(cnum);
  fatwrite(cnum,0);
  cnum = tcnum;
 }
 while(cnum<0xff8);
}


//To get free fat entry
int findfreefatentries(int n,unsigned int *cnum)
{
 static int cnt = 0;
 static int tfatentries=0;
 static unsigned int fatbuf[342];
 static int numfat;
 static int j = 0;
 static int i;
 cnt = 0;
 tfatentries = 0;
 j=0;
 for(i=0;i<15;i++)cnum[i] = 0;
 while(cnt<9)
 {
  numfat=fatread(cnt++,fatbuf);
  for(i=0;i<numfat;i++)
  {
   if(fatbuf[i]==0)
   {
    cnum[j++]  = tfatentries+i;
    if(j==n){return 1;}
   }
  }
  tfatentries+=numfat;
 }
 return 0;
}

//Copy data
unsigned int copydata(unsigned int index)
{
 static unsigned int cnum=0,dcnum=0,*tdcnum;
 static unsigned int fcnum = 0;
 dcnum = 0;
 fcnum = 0;
 cnum = index;
 do
 {
  findfreefatentries(1,tdcnum);
  if(dcnum != 0) fatwrite(dcnum,*tdcnum);
  else fcnum = *tdcnum;
  dcnum = *tdcnum;
  datwrite(dcnum,datread(cnum));
  cnum = getcnum(cnum);
 }
 while(cnum<0xff8);
 fatwrite(dcnum,0xfff);
 return fcnum;
}

#endif