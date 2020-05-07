#ifndef _fs_
#define _fs_

#include "disk.cpp"
#include "shell.cpp"
#include "fat12.cpp"

#define FLOPPY 1
#define File_Not_Found 1
#define Folder_Not_Found 2
#define Destination_File_Exist 3
#define Destination_Folder_Exist 4
#define Access_Violation    5
#define No_Disk_Space 6
#define Des_Path_Not_Exist  7
#define RW 33
#define RO 32

struct flop
{
char h;
char s;
char t;
int noe;
int found;
int error;
unsigned int cnum;
char name[12];
};

char file_valid1;
char file_name1[11];
unsigned int file_next_lba1;
char *file_buffer1;
char file_pointer1;
char feof1;
long file_size1;
char ch = 1,ct = 0,cs = 2;
unsigned int  ccls = 0x0000;
char cwd[100];

extern char Error_Code;
extern  char floppy_hardd_tog;

void fclose();
void fopen(char*);
char fget_char();
void make1(char*,char*);
void ls1();
void ls2();
void rem1(char *str);
void rem2(char *str);
void types(char *str);
void rem2(char*);
void setcwd(unsigned int);
void dir();
void dir2();
void dir1();
void rem(char *str);
unsigned int copydata(unsigned int);
void ele(char[],char[]);

typedef struct flop hts;
Directory curr_dir;


void chmod(char buffer[32],char attr)
{
 buffer[11] = attr;
}

void sethts(unsigned int cnum,hts *temp)
{
 static unsigned int spt = 18,noh = 2;
 static unsigned int secaddress;
 static unsigned int laddress = 33;
 secaddress = laddress + cnum-1;
 temp->s = ((secaddress)%spt);
 temp->h = (secaddress/spt)%noh;
 temp->t = (secaddress/(spt*noh));
}

hts validate(char* path,int dorf)
{
 static hts temp,temp1;
 static hts temp2;
 static char *vp = 0;
 static int dirc;
 static int icnt;
 static char buffer[512];
 static char name[12],fname[11];
 static unsigned int tcnum;
 static int i,j,found,w,flag;
 static int curr;
 dirc=0;
 found = 0;
 curr = 0;
 vp = path;
 temp.error = 0;
 for(i=0;i<11;i++)temp.name[i]  =  0;
 temp.found = 0;
 for(i=0;i<12;i++)fname[i] = 0;
 if(streq(path,"A:\\"))
 {
  temp.h = 1;
  temp.s = 2;
  temp.t = 0;
  temp.cnum = 0x0;
  temp1.cnum = 0x0;
 }
 else
 {
  temp.h = ch;
  temp.s = cs;
  temp.t = ct;
  curr = 1;
  temp.cnum = ccls;
  temp1.cnum = ccls;
 }
 if(!curr)
 {
  while(*path != '\\' && *path != 0 && *path != 32)path++;
  path++;
 }
 i = 0;
 while(*path != '\\' && *path != 0 && *path != 32)
 {
  fname[i++] = *path;path++;
 }
 ele(fname,name);
 name[11] = 0;
 temp1.h = temp.h;
 temp1.t = temp.t;
 temp1.s = temp.s;
 temp2.s = temp.s;temp2.t = temp.t;temp2.h = temp.h;
 temp2.noe = 0;
 while(!found)
 {
  dirc = 0;
  flopread(temp1.h,temp1.t,temp1.s,1,buffer);
  for(i = 0;i<512&&buffer[i] != 0&&(!found);i+=32)
  {
   flag  = 0;
   for(w=0;w<11;w++)
    if(buffer[i+w] != name[w])
    {
     flag = 1;
     break;
    }
   if(!flag)
   {
    if((buffer[i+11]&16)!=0)
    {
     dorf=0;
     dirc = 1;
     j=0;
     for(icnt=0;icnt<11;icnt++)temp2.name[icnt] = name[icnt];
     if(*path == 0)fname[j]=0;
     if(*path == '\\')path++;
     while(*path != '\\' && *path != 0 && *path != 32)
     {
      fname[j++] = *path;
      path++;
     }
     ele(fname,name);
     tcnum =  (unsigned int)(buffer[i+27]<<8) + (unsigned char)(buffer[i+26]);
     temp2.s = temp.s;temp2.t = temp.t;temp2.h = temp.h;temp2.noe = i/32;
     if(tcnum == 0x000)
     {
      temp.s = 2;temp.h = 0;temp.t = 1;
     }
     else
     {
      sethts(tcnum,&temp);
     }
     temp1.h = temp.h;
     temp1.s = temp.s;
     temp1.t = temp.t;
     break;
    }
    else if(*path == 0)
    {
     temp.found = 1;
     temp.noe = i/32;
     found=1;
     break;
    }
   }
  }
  if(!dirc && !found)
  {
   if(i != 512 && buffer[i] == 0)
   {
    if(strlen(name) == 0)
    {
     temp.s = temp2.s;temp.t = temp2.t;temp.h = temp2.h;temp.noe = temp2.noe;
     for(icnt=0;icnt<11;icnt++)temp.name[icnt] = temp2.name[icnt];
    }
    if(((path[0] != 0) && (path[0] != 32))||(dorf)) temp.error = 1;
    for(i=0;i<11;i++) temp.name[i]  =  name[i];
    found = 1;
   }
   else if(i == 512)
   {
    if(temp1.cnum == 0)
    {
     temp1.s++;
     if(temp1.s>18)
     {
      temp1.s=0;
      if(temp1.h==1)
      {
       temp1.t++;
       temp1.h=0;
      }
      else
      {
       temp1.h = 1;
      }
     }
    }
    else
    {
     temp1.cnum = getcnum(temp1.cnum);
     if(temp1.cnum != 0xfff) sethts(temp1.cnum,&temp1);
     else found = 1;
     continue;
    }
   }
  }
 }
 path  = vp;
 return temp;
}


hts findentries(hts thts)
{
 static int h,t,s;
 static int i,j,found;
 static unsigned int *cnum;
 static char buffer[512];
 found = 0;
 for(i=0;i<14;i++)
 {
  h = thts.h;
  s = thts.s;
  t = thts.t;
  flopread(h,t,s,1,buffer);
  for(j=0;j<512;j+=32)
  {
   if((buffer[j] == 0 || buffer[j] == 'å'))
   {
    found = 1;
    break;
   }
  }
  if(found)
  {
   thts.h = h;
   thts.s = s;
   thts.t = t;
   thts.found = 1;
   thts.noe = j/32;
   break;
  }
  if(thts.cnum == 0)
  {
   thts.s++;
   if(thts.s>18)
   {
    thts.s=0;
    if(thts.h==1)
    {
     thts.t++;
     thts.h=0;
    }
    else
    {
     thts.h = 1;
    }
   }
  }
  else
  {
   thts.cnum = getcnum(thts.cnum);
   if(thts.cnum == 0xfff)
   {
    findfreefatentries(1,cnum);
    fatwrite(thts.cnum,*cnum);
    fatwrite(*cnum,0xfff);
    sethts(*cnum,&thts);
   }
  }
 }
 return thts;
}



void ele(char fname[11],char name[11])
{
 static int x,length,len,cnt;
 static char* temp;
 temp = fname;
 length=len=strlen(fname);
 cnt=0;
 x = 0;
 while(temp[x] != '.' && x < strlen(fname))
 x++;
 if(x < strlen(fname) && !streq(fname,".."))
 {
  while(fname[len]!='.')
  {
   len--;
  }
  for(x=0;x<len;x++)
  {
   name[x]=fname[cnt++];
  }
  for(;x<8;x++)	{name[x]=32;length++;}
  length--;//for point
  if(fname[cnt]!='.')
  {
   name[x-2]='`';
   name[x-1]='1';
  }
  len++;
  for(;x<length;x++) name[x]=fname[len++];
  for(;x<11;x++) name[x]=0;
 }
 else
 {
  for(x=0;x<strlen(fname);x++)name[x] = fname[x];
  for(;x<11;x++)name[x] = 0;
 }
}


char protect(char *str)
{
 int len;
 len = strlen(str);
 if(priority==1) return 1;
 if((str[len-1]=='x')&&(str[len-2]=='t')&&(str[len-3]=='m'))
 {
  Error_Code=Access_Violation;
  return 0;
 }
 return 1;
}


void copy1(char* des,char* source)
{
 static int i,j;
 static unsigned int cnum;
 static unsigned char buffer[512];
 static unsigned char name[32];
 static char choice;
 static hts dhts,shts;
 dhts = validate(des,0);
 if(dhts.error)
 {
  Error_Code =  Folder_Not_Found;
  return;
 }
 else if(dhts.found)
 {
  Error_Code =  Destination_File_Exist;
  return;
 }
 else
 {
  dhts = findentries(dhts);
 }
 shts = validate(source,0);
 if(shts.found)
 {
  flopread(shts.h,shts.t,shts.s,1,buffer);
  shts.noe*=32;
  for(j = 0,i = shts.noe;i< shts.noe+32;i++,j++) name[j] = buffer[i];
  cnum = copydata((unsigned int)(name[27]<<8) + name[26]);
  if(cnum == 0){/*error("ERROR: NO DISK SPACE");*/Error_Code = No_Disk_Space; return ;}
  flopread(dhts.h,dhts.t,dhts.s,1,buffer);
  dhts.noe*=32;
  i = dhts.noe;j=0;
  for(j=0,i=dhts.noe;i< dhts.noe+11;i++,j++) buffer[i] = dhts.name[j];
  j+=11;
  i+=11;
  for(;i< dhts.noe+32;i++,j++) buffer[i] = name[j];
  buffer[dhts.noe+27]=cnum>>8;
  buffer[dhts.noe+26]=cnum;
  flopwrite(dhts.h,dhts.t,dhts.s,1,buffer);
 }
 else
 {
  Error_Code =  File_Not_Found;
 }
}


void copy2(char* des,char* source)
{
 char temp[50],temp1[50];
 File f1,f2,f3;
 static char f_ch[4096];
 char choice;
 int file_flag,i,j;
 i = 0;
 while(des[i] != ' ')i++;
 des[i] = 0;
 for(int y=0;y<50 && des[y] != 0;y++)temp[y] = des[y];
 for(y=0;y<50 && source[y] != 0;y++)temp1[y] = source[y];
 temp1[y] = 0;
 file_flag= f1.file_open(des,RW);
 if(file_flag==3)
 {
  Error_Code =  Destination_File_Exist;
  return;
 }
 i=0;j=0;
 for( y=0;y<50 && temp[y] != 0;y++)des[y] = temp[y];
 while(temp[i]!=0)
 {
  if(temp[i]==':') j=i;
  i++;
 }
 temp[j]=0;
 Directory dir(des);
 if(!dir.found)
 {
  Error_Code = Destination_Folder_Exist;
  return;
 }
 file_flag= f2.file_open(temp1,RO);
 if(file_flag==2)
 {
  Error_Code = File_Not_Found;
  return;
 }
 i=0;
 while(f2.eof==0) f_ch[i++]=f2.fget_char();
 f1.file_write(f_ch);
}


void copy(char *des,char *source)
{
 if(floppy_hardd_tog == FLOPPY) copy1(des,source);
 else copy2(des,source);
}


/*void copyac(char *des,char *source)
{
char *buf;
long cnt=0;
fopen(source);
while(!feof1) buf[cnt++]=fget_char();
fclose();
File f;
f.file_open(des,RW);
f.file_write(buf);
}

void copyca(char *des,char *source)
{
char *buf;
long cnt=0;
File f;
int file_flag;
file_flag= f.file_open(source,RO);
if(file_flag==0)
 {
  while(f.eof==0) buf[cnt++] = f.fget_char();
  make1(des,buf);
  }
} */


void make2(char *str,char *buf)
{
 File f;
 f.file_open(str,RW);
 f.file_write(buf);
}

void make1(char *str,char *buf)
{
 static char str1[11];
 static char buffer[512];
 static hts finddir;
 static char choice;
 static unsigned long int clustno,size;
 static int j,k,i;
 static unsigned int cluster[20];
 setattrib(color(0,4));
 putchar('\r');
 clustno=strlen(buf)/512;
 if(strlen(buf)%512) clustno++;
 findfreefatentries(clustno,cluster);
 finddir =  validate(str,0);
 if(finddir.found)
 {
  Error_Code = Destination_File_Exist;
  return;
 }
 else if(finddir.error)
 {
  Error_Code = Folder_Not_Found;
  return;
 }
 else
 {
  finddir = findentries(finddir);
 }
 flopread(finddir.h,finddir.t,finddir.s,1,buffer);
 j = finddir.noe*32;
 ele(str,str1);
 str = str1;
 for(i=j,k=0;i<j+11;i++,k++)buffer[i] = str[k];
 for(;i<j+32;i++)buffer[i] = 0;
 buffer[j+27]=cluster[0]>>8;
 buffer[j+26]=cluster[0];
 size=strlen(buf);
 buffer[j+31]=size>>24;
 buffer[j+30]=size>>16;
 buffer[j+29]=size>>8;
 buffer[j+28]=size;
 flopwrite(finddir.h,finddir.t,finddir.s,1,buffer);
 for(i=0;i<(clustno-1);i++)
 {
  fatwrite(cluster[i],cluster[i+1]);
 }
 fatwrite(cluster[i],0x0fff);
 for(i=0;i<clustno;i++)
 {
  datwrite(cluster[i],buf);
  buf += 512;
 }
}



void make(char *str,char *buf)
{
 if(floppy_hardd_tog == FLOPPY) make1(str,buf);
 else make2(str,buf);
}



void display_dir(Dir_Entries *dir_entries)
{
 int h=1,xx=0;
 setattrib(color(0,5));
 for(int i=0;dir_entries[i].name[0]!=0;i++)
 {
  if(dir_entries[i].name[0] == 0) break;
  if((dir_entries[i].attribute&0x10)==0) continue;
  xx+=14;
  for(int j=0;j<11;j++)
  {
   if(dir_entries[i].name[j] == 32) continue;
   if((j==8)) putchar('.');
   putchar(dir_entries[i].name[j]);
  }
  if(h%5) x=xx;
  else {putchar(13);xx=0;}
  h++;
 }
 setattrib(color(0,4));
 for(i=0;dir_entries[i].name[0]!=0;i++)
 {
  if(dir_entries[i].name[0] == 0) break;
  if((dir_entries[i].attribute&0x10)!=0) continue;
  xx+=14;
  for(int j=0;j<11;j++)
  {
   if(dir_entries[i].name[j] == 32) continue;
   if((j==8)) putchar('.');
   putchar(dir_entries[i].name[j]);
  }
  if(h%5) x=xx;
  else {putchar(13);xx=0;}
  h++;
 }
 putchar(13);
}


void dis_size(long size)
{
 int i,f=0;
 long j;
 long s=1000000000;
 for(i=10;i>0;i--)
 {
  j = size/s;
  j=j%10;
  if(j!=0) f=1;
  if(f==1) putchar((char)j+'0');
  else putchar(' ');
  if(s==0) continue;
  s/=10;
 }
 putchar(' ');putchar('b');
 putchar('y');putchar('t');
 putchar('e');putchar('s');
}



void display_fil(Dir_Entries *dir_entries)
{
 int h=1,xx=0;
 setattrib(color(0,4));
 for(int i=0;dir_entries[i].name[0]!=0;i++)
 {
  if(dir_entries[i].name[0] == 0) break;
  if((dir_entries[i].attribute&0x10)!=0) continue;
  xx+=16;
  for(int j=0;j<11;j++)
  {
   if(dir_entries[i].name[j] == 32) continue;
   if((j==8)) putchar('.');
   putchar(dir_entries[i].name[j]);
  }
  x=xx;xx+=20;
  dis_size(dir_entries[i].size);
  if(h%2) x=xx;
  else {putchar(13);xx=0;}
  h++;
 }
 putchar(13);
}


void ls()
{
 if(floppy_hardd_tog == FLOPPY) ls1();
 else ls2();
}


void dir()
{
 if(floppy_hardd_tog == FLOPPY) dir1();
 else dir2();
}


void dir2()
{
 Dir_Entries *dir_entries;
 dir_entries = curr_dir.dir_read();
 if(curr_dir.found)
 {
  display_dir(dir_entries);
 }
 else putchar('E');
 putchar(13);
}


void ls2()
{
 Dir_Entries *dir_entries;
 dir_entries = curr_dir.dir_read();
 if(curr_dir.found)
 {
  display_fil(dir_entries);
 }
 else putchar('E');
 putchar(13);
}


Dir_Entries* fdir_read()
{
 Dir_Entries dir_entries[16];
 static char name[32];
 static char buf[512];
 int j,k,x,i,t,s,n,h,flag,cnt=0;
 t=ct;s=cs;n=1;h=ch;flag=0;
 flopread(h,t,s,n,buf);
 for(i=0;i<16;i++)
 {
  j=0;
  if(buf[i*32+j] == 0) break;
  if((buf[i*32+j]=='å')||(buf[i*32+11]==0x0f)) continue;
  for(k=0;k<11;k++) dir_entries[cnt].name[k] = buf[i*32+(j++)];
  dir_entries[cnt].attribute = buf[i*32+j];
  j=15;
  dir_entries[cnt].date = rev_cat(buf[i*32+j],buf[i*32+(j+1)]);
  j=28;
  dir_entries[cnt].size =
  rev_cat(rev_cat(buf[i*32+j],buf[i*32+(j+1)]),rev_cat(buf[i*32+(j+2)],buf[i*32+(j+3)]));
  cnt++;
 }
 dir_entries[cnt].name[0]=0;
 return(dir_entries);
}


void dir1()
{
 Dir_Entries *dir_entries;
 dir_entries = fdir_read();
 display_dir(dir_entries);
}


void ls1()
{
 Dir_Entries *dir_entries;
 dir_entries = fdir_read();
 display_fil(dir_entries);
}


int md1(char dirn[11])
{
 static unsigned char buf[512];
 static char buffer[512];
 static hts mdhts;
 static unsigned int *cluster;
 static int j,i;
 mdhts = validate(dirn,1);
 if(mdhts.error == 0)
 {
  Error_Code = Destination_Folder_Exist;
  return 0;
 }
 mdhts = findentries(mdhts);
 flopread(mdhts.h,mdhts.t,mdhts.s,1,buffer);
 findfreefatentries(1,cluster);
 fatwrite(*cluster,0x0fff);
 for(i=0,j=mdhts.noe*32;i<11;i++,j++)buffer[j] = mdhts.name[i];
 for(;j<mdhts.noe*32+32;j++)buffer[j] = 0;
 buffer[mdhts.noe*32+27]=*cluster>>8;
 buffer[mdhts.noe*32+26]=*cluster;
 buffer[j+11] |=16;
 buffer[mdhts.noe*32+11] |=16;
 flopwrite(mdhts.h,mdhts.t,mdhts.s,1,buffer);
 for(i=0;i<512;i++)buf[i] = 0;
 buf[0] = '.';
 buf[1] = '.';
 for(i=2;i<11;i++)buf[i] = 32;
 buf[11] = 0x10;
 buf[27]=ccls>>8;
 buf[26]=ccls;
 datwrite(*cluster,buf);
 return 1;
}


int md2(char dirn[11])
{
 curr_dir.add_dir(dirn);
}


int md(char dirn[11])
{
 if(floppy_hardd_tog == FLOPPY) md1(dirn);
 else md2(dirn);
}



int cd2(char dirn[11])
{
 Directory dir1;
 dir1 = curr_dir.get_dir(dirn);
 if(dir1.found)
 {
  curr_dir=dir1;
 }
 return dir1.found;
}


int cd1(char dirn[11])
{
 static char name[32];
 static char buffer[512];
 static int found;
 static unsigned int temp;
 static hts cdhts;
 static  int j,i;
 found = 0;
 setattrib(color(0,4));
 cdhts = validate(dirn,1);
 if(cdhts.error || cdhts.found == 1)
 {
  Error_Code = Folder_Not_Found;
  return found;
 }
 found = 1;
 flopread(cdhts.h,cdhts.t,cdhts.s,1,buffer);
 for(i=0,j=cdhts.noe*32;i<32;i++,j++)name[i] = buffer[j];
 temp =  (unsigned int)(name[27]<<8) + (unsigned int)(name[26]);
 if(temp == 0x0){cs = 2;ct = 0;ch = 1;ccls = 0x0;}
 else
 {
  setcwd(temp);
 }
 return found;
}


int cd(char dirn[11])
{
 if(floppy_hardd_tog == FLOPPY) return cd1(dirn);
 else return cd2(dirn);
}


void setcwd(unsigned int cnum)
{
 static unsigned int spt = 18,noh = 2;
 static unsigned int secaddress;
 static unsigned int laddress = 33;
 secaddress = laddress + cnum-1;
 cs = ((secaddress)%spt);
 ch = (secaddress/spt)%noh;
 ct = (secaddress/(spt*noh));
 ccls = cnum;
}


void rem(char *str)
{
 if(floppy_hardd_tog == FLOPPY) rem1(str);
 else rem2(str);
}

void rem1(char *str)
{
 static char buffer[512];
 static hts findfile;
 static int j;
 setattrib(color(0,4));
 findfile = validate(str,0);
 if(findfile.found)
 {
  flopread(findfile.h,findfile.t,findfile.s,1,buffer);
  j = findfile.noe*32;
  buffer[j] = 'å';
  flopwrite(findfile.h,findfile.t,findfile.s,1,buffer);
  fatclear((unsigned int)(buffer[j+27]<<8) + buffer[j+26]);
 }
 else
 {
  Error_Code = File_Not_Found;
 }
}


void rem2(char *str)
{
 File f;
 int file_flag;
 char f_ch;
 file_flag= f.file_open(str,RO);
 if(file_flag==2)
 {
  Error_Code = File_Not_Found;
  return;
 }
 curr_dir.remove_entry(f.file_details.file_name,f.drive);
 d[f.drive-2].clear_fat_entry(f.next_lba,f.drive);
}


void edit(char *str)
{
 static char c;
 static int i;
 static char bluf[512];
 i=0;
 setattrib(color(0,8));
 printxy("To stop typing press Ctrl^z",x,y);
 putchar('\r');
 setattrib(color(0,12));
 c = getch();
 while(c != 26)
 {
  if(c==13)
  {
   gotoxy(x=1,++y);
   bluf[i++] = c;
  }
  else if(c==8) /* Back space */
  {
   if((i>0) && (x>1))
   {
    bluf[--i]=0;
    gotoxy(-- x, y);
    mtx_printchar(0);
   }
   else if( x<=1)
   {
    gotoxy(( x=79),-- y);
    mtx_printchar(0);
   }
  }
  else if( x<79)
  {
   mtx_printchar(c);
   gotoxy(++ x, y);
   bluf[i++] = c;
  }
  else
  {
   if( y<24)
   {
    x = 0;
    y+= 1;
    gotoxy(++ x, y);
    mtx_printchar(c);
    bluf[i++] = c;
    gotoxy(++ x, y);
   }
   else
   {
    x = 0;
    mtx_scrollup(1,0,3,79,24);
    gotoxy(++ x, y=24);
   }
  }
  if( y>=24)
  {
   mtx_scrollup(1,0,3,79,24);
   y = 24;
  }
  c = getch();
 }
 bluf[i] = 0;
 make(str,bluf);
}


void type1(char *str)
{
 static hts thts;
 static char lcnum,ucnum;
 static  char buffer[512];
 thts = validate(str,0);
 if(thts.found == 0)
 {
  Error_Code = File_Not_Found;
  return;
 }
 else if(thts.error == 1)
 {
  Error_Code = Folder_Not_Found;
  return;
 }
 flopread(thts.h,thts.t,thts.s,1,buffer);
 lcnum = buffer[thts.noe*32+26];
 ucnum = buffer[thts.noe*32+27];
 fat( (unsigned int)(ucnum<<8) + lcnum);
}


void type2(char *path)
{
 File f;
 int file_flag,cnt=0;
 char f_ch;
 file_flag= f.file_open(path,RO);
 if(file_flag!=0) Error_Code = File_Not_Found;
 else
 while(f.eof==0)
 {
  f_ch=f.fget_char();
  putchar(f_ch);
  if(f_ch==13) cnt++;
  if(cnt==20) {f_ch=getch();if(f_ch==27) return;cnt=0;}
 }
}


void types(char *path)
{
 int xx,yy;
 xx=x;yy=y;
 page=1;
 x=1,y=1;
 set_display_page(page);
 clear(color(0,2));
 putmsg("Filename : ");x=12;
 setattrib(color(0,130));
 putmsg(path);
 drawhline(1,2,78,color(0,2));
 setattrib(color(0,13));
 x=1;y=4;
 if(floppy_hardd_tog == FLOPPY) type1(path);
 else type2(path);
 setattrib(color(0,13));
 if(Error_Code!=0) putmsg("No such file exist");
 getchar();
 x=xx;y=yy;
 page=0;
 set_display_page(page);
}


void fopen(char *str)
{
 static hts thts;
 static char lcnum,ucnum;
 static  char buffer[512];
 if(file_valid1 == 0)
 {
  feof1 = 0;
  setattrib(color(0,13));
  thts = validate(str,0);
  if(thts.found == 0)
  {
   Error_Code = File_Not_Found;
   fclose();
   return;
  }
  else if(thts.error == 1)
  {
   Error_Code = Des_Path_Not_Exist;
   fclose();
   return;
  }
  flopread(thts.h,thts.t,thts.s,1,buffer);
  lcnum = buffer[thts.noe*32+26];
  ucnum = buffer[thts.noe*32+27];
  file_size1 = rev_cat(
		rev_cat(buffer[thts.noe*32+28],buffer[thts.noe*32+29]),
		rev_cat(buffer[thts.noe*32+30],buffer[thts.noe*32+31]));
  file_next_lba1 = (unsigned int)(ucnum<<8) + lcnum;
  file_buffer1 = datread(file_next_lba1);
  file_next_lba1 = getcnum(file_next_lba1);
  file_pointer1 = 0;
  file_valid1 = 1;
 }
}


char fget_char()
{
 static long cont=0;
 if(file_valid1)
 if(cont <= file_size1 && file_pointer1 <512)
 {
  cont++;return(file_buffer1[file_pointer1++]);
 }
 else if( file_next_lba1 <= 0xff8)
 {
  file_buffer1 = datread(file_next_lba1);
  file_next_lba1 = getcnum(file_next_lba1);
  file_pointer1 = 1;
  cont++;
  return (file_buffer1[0]);
 }
 else
 {
  feof1 = 1;cont = 0;
 }
}


void fclose()
{
 file_valid1 = 0;
 file_pointer1 = 0;
 feof1 = 0;
}


void move2(char* des,char* source)
{
 char temp[50];
 for(y=0;y<50 && source[y] != 0;y++)temp[y] = source[y];
 copy2(des,source);
 rem2(temp);
}


void move1(char* des,char* source)
{
 static int i,j;
 static unsigned char buffer[512];
 static unsigned char name[32];
 static char choice;
 static hts dhts,shts;
 dhts = validate(des,0);
 if(dhts.error)
 {
  Error_Code = Des_Path_Not_Exist;
  return;
 }
 else if(dhts.found)
 {
  Error_Code = Destination_File_Exist;
  return;
 }
 else
 {
  dhts = findentries(dhts);
 }
 shts = validate(source,0);
 if(shts.found)
 {
  flopread(shts.h,shts.t,shts.s,1,buffer);
  shts.noe*=32;
  for(j = 0,i = shts.noe;i< shts.noe+32;i++,j++) name[j] = buffer[i];
  buffer[shts.noe] = 'å';
  flopwrite(shts.h,shts.t,shts.s,1,buffer);
  flopread(dhts.h,dhts.t,dhts.s,1,buffer);
  dhts.noe*=32;
  if(dhts.found)
  {
   buffer[dhts.noe] = *des;
  }
  else if(dhts.error)
  {
   for(j=0,i=dhts.noe;i< dhts.noe+11;i++,j++) buffer[i] = dhts.name[j];
   j=11;
   i=dhts.noe+11;
  }
  for(;i< dhts.noe+32;i++,j++) buffer[i] = name[j];
  flopwrite(dhts.h,dhts.t,dhts.s,1,buffer);
 }
 else
 {
  Error_Code = File_Not_Found;
 }
}



void move(char* des,char* source)
{
 if(floppy_hardd_tog == FLOPPY) move1(des,source);
 else move2(des,source);
}

#endif