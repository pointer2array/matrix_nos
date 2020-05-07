#ifndef _disk_
#define _disk_

#include "disk.h"
#include "inout.cpp"
#include "shell.cpp"
#include "fs.cpp"
#define RW 33
#define RO 32


#define File_Not_Found 1
#define Folder_Not_Found 2
#define Destination_File_Exist 3
#define Destination_Folder_Exist 4
#define Access_Violation    5

char Error_Code;
char curr_dir_cnum;
extern Directory curr_dir;
extern char *path;
extern  char floppy_hardd_tog;
char Drive_No=0x80;

		  // Initialise Routine //
			Disk disk;
			Drive d[4];

//To check whether the entered drive is valid or not
struct Entry_details Disk::validate_drive(char* str)
{
int dr_num;
struct Entry_details file_details;
dr_num = path[0]-'A';
if((dr_num==2)||(dr_num==3))
   {
    file_details.lba = curr_dir.dir_lba;
    file_details   = d[dr_num-2].validate_entry(str,file_details,dr_num);
    return file_details;
   }
}


//Convertion of chs to lba addressing
long Disk::chs_to_lba(int track,int head,int sector)
{
  long lba;
  lba = (track*255*63) + head*63 + sector;
  return(lba);
}

//Convertion of chs to lba addressing
long Disk::chs_to_lba(char track,char head,char sector)
{
  int h,t,s,temp;
  long lba;
  h=head;
  s=(sector & 0x3f);
  temp = (sector & 0xc0)<<2;
  t=track+temp;
  lba =(255*63*(long)t);
  lba+= s+ (63*h);
  return(lba);
}


//Constructor for Disk class
Disk::Disk()
  {
  char head,track,sector,buf[512];
  int i,j,read_flag,inc=446;
  long lba;
  num_of_drives=2;//Number of drives assumed to be 2
  read_flag = get_sector(lba = 1,0x01,buf);//MBR is on t=0,h=0,s=1;
  for(i=0;i<4;i++)
    for(j=0;j<16;j++)
      partition[i][j] = buf[inc++];
  head=partition[1][1];
  sector=partition[1][2];
  track=partition[1][3];
  lba = chs_to_lba(track,head,sector);
  read_flag = get_sector(lba,0x01,buf);
  inc=446;
  for(i=1;i<4;i++)
    for(j=0;j<16;j++)
      partition[i][j] = buf[inc++];
  }

//Read sectors with a given lba
int Disk::get_sector(long lba,char num_sectors,char *buf)
{
 char num;
 int h,t,s,temp;
 char head,sector,track;
 if((lba==50)&&(priority!=1)) {Error_Code=Access_Violation;return 0;}
 t = lba/(255*63);
 h = (lba%(255*63))/63;
 s = lba%63;
 head = h;
 sector = (t & 0x300)>>2;
 sector+=s;
 track = t;
 asm mov cx,0x3;
 lp:
 asm push cx;
 asm mov ah,0x02
 asm mov al,num_sectors
 asm mov ch,track
 asm mov cl,sector
 asm mov dh,head
 asm mov dl,Drive_No
 asm mov bx,buf
 asm push ds
 asm int 0x13
 asm pop es
 asm jnc nerror;
 num=0;
 asm pop cx;
 asm dec cx;
 asm jnz lp;
 asm mov num,ah
 return num;
 nerror:
 asm mov num,ah
 return num;
}


//Write sectors with a given lba
int Disk::put_sector(long lba,char num_sectors,char *buf)
{
 char num;
 int h,t,s,temp;
 char head,sector,track;
 if((lba==50)&&(priority!=1)) {Error_Code=Access_Violation;return 0;}
 t = lba/(255*63);
 h = (lba%(255*63))/63;
 s = lba%63;
 head = h;
 sector = (t & 0x300)>>2;
 sector+=s;
 track = t;
 asm mov cx,0x3;
 lp:
 asm push cx;
 asm mov ah,0x03
 asm mov al,num_sectors
 asm mov ch,track
 asm mov cl,sector
 asm mov dh,head
 asm mov dl,Drive_No
 asm mov bx,buf
 asm push ds
 asm int 0x13
 asm pop es
 asm jnc nerror;
 num=0;
 asm pop cx;
 asm dec cx;
 asm jnz lp;
 asm mov num,ah
 return num;
 nerror:
 asm mov num,ah
 return num;
 }

//Validate a given relative path
struct Entry_details Drive::validate_entry(char* str,struct Entry_details file_details,int dr_num)
{
Directory dir;
file_details = dir.get_lba(str,file_details,dr_num);
return file_details;
}



//Validate a given absolute path
struct Entry_details Disk::validate_drive_abs(char* path)
{
int dr_num;
struct Entry_details file_details;
dr_num = *path - 'A';
if((dr_num==2)||(dr_num==3))
   {
    file_details.lba = d[dr_num-2].rootdir_lba;
    file_details   = d[dr_num-2].validate_entry_abs(path+3,file_details,dr_num);
    return file_details;
   }
}

//Get a fat entry with the given lba
long Drive::get_fat_entry(long lba_num,int dr_num)
{
int read_flag,fat_entry;
char buf[512];
long lba,cluster;
cluster=((lba_num-d[dr_num-2].rootdir_lba)/d[dr_num-2].get_spc())+2;
lba = fat1_lba+(cluster)/128;
read_flag = disk.get_sector(lba ,0x01,buf);
fat_entry = (cluster%128)*4;
cluster =
   rev_cat(
      rev_cat(buf[fat_entry],buf[fat_entry+1]),
      rev_cat(buf[fat_entry+2],buf[fat_entry+3])
	  );
if(cluster>=0x0ffffff8) return(cluster);
else
   return(d[dr_num-2].rootdir_lba+(cluster-2)*d[dr_num-2].get_spc());
}

//Read data from a given lba
char* Drive::dataread(long lba)
{
char buf[512];
int read_flag;
read_flag = disk.get_sector(lba ,0x01,buf);
return(buf);
}

//Initialise drive parameters
void Drive::initialise(int drive_num)
{
long lba;
char buf[512];
int read_flag;
lba=disk.chs_to_lba(disk.partition[drive_num-2][3],disk.partition[drive_num-2][1],
		disk.partition[drive_num-2][2]);
read_flag = disk.get_sector(lba ,0x01,drive_buf);
fat1_lba=lba + rev_cat(drive_buf[14],drive_buf[15]);
rootdir_lba = fat1_lba +
	(2 *
	(rev_cat
	     (rev_cat(drive_buf[36],drive_buf[37]),
		       rev_cat(drive_buf[38],drive_buf[39]))));
}

//Get free fat entries
long Drive::get_free_fat_entry()
{
int read_flag,fat_entry=0;
char buf[512];
long lba,cluster;
lba = fat1_lba;
while(1)
{
read_flag = disk.get_sector(lba++,0x01,buf);
fat_entry = 0;
while(fat_entry<128)
{
cluster =
   rev_cat(
      rev_cat(buf[fat_entry],buf[fat_entry+1]),
      rev_cat(buf[fat_entry+2],buf[fat_entry+3])
	  );
fat_entry += 4;
if(cluster==0)
{
fat_entry -= 4;
lba--;
cluster = (lba-fat1_lba)*128 + (fat_entry/4);
return(cluster);
}
}
}
}

//Get sector per cluster value for the partition
char Drive :: get_spc()
{
return drive_buf[13];
}

//Sets a fat entry to a given value
Drive::set_fat_entry(long l_lba,long r_lba,int drive)// set the link og l_lba to r_lba
{
int read_flag,fat_entry;
char* temp;
char buf[512];
long lba,clusterl,clusterr;
clusterl=((l_lba-d[drive-2].rootdir_lba)/d[drive-2].get_spc())+2;
if(r_lba<0x0ffffff8)
clusterr=((r_lba-d[drive-2].rootdir_lba)/d[drive-2].get_spc())+2;
else clusterr=r_lba;
lba = fat1_lba+(clusterl)/128;
read_flag = disk.get_sector(lba ,0x01,buf);
fat_entry = ((clusterl)%128)*4;
	temp = (char*)buf;
	temp+=fat_entry;
	temp[0] = clusterr;
	temp[1] = clusterr>>8;
	temp[2] = clusterr>>16;
	temp[3] = clusterr>>24;
disk.put_sector(lba ,0x01,buf);
}



//Validate a given absolute path
struct Entry_details Drive::validate_entry_abs(char* path,struct Entry_details file_details,int dr_num)
{
char *name;
Directory dir;
if(*path == 0) return(file_details);
name = path;
while((*path != ':')&&(*path != 0))path++;
if(*path != 0)
{
*path = 0;
path++;
}
file_details = dir.get_lba_abs(name,file_details,dr_num);
file_details = validate_entry_abs(path,file_details,dr_num);
return file_details;
}


//Get entry details
struct Entry_details Directory::get_lba_abs(char* name,struct Entry_details file_details,int dr_num)
{
char *f_d;
char buf[64][32];
int flag=1,k;
long cluster;
int index;
disk.get_sector(file_details.lba,0x04,(char *)buf);
for(int i=0;i<64;i++)
{
for(int j=0,k=-1;j<11;j++)
   {
      if(k==-1) k++;
     else if(name[k])k++;
   if(name[k] == '.') {j=8;k++;}

   if((buf[i][j] == 0x20 && name[k] == 0) || buf[i][j] == name[k])
       {flag=0;}
   else {flag = 1;break;}
   }
   if(!flag && (j == 11 || name[k] == 0))
   break;
 }
cluster =
   rev_cat(rev_cat(buf[i][26],buf[i][27]),rev_cat(buf[i][20],buf[i][21]));
if(!flag)
 {
 f_d =(char*) &file_details;
 index = 0;
 while(index<32){*f_d = buf[i][index];index++;f_d++;}
 file_details.lba = d[dr_num-2].rootdir_lba+(cluster-2)*d[dr_num-2].get_spc();
 return(file_details);
 }
else
{
file_details.lba = 0;
return file_details;
}
}



Directory::Directory(){}


Directory::Directory(char dr_num)
{
drive = dr_num;
}


Directory::Directory(char *path)
{
drive = *path-'A';
if(path[0] == 'C' && path[1] == ':')
dir_details = disk.validate_drive_abs(path);
else
dir_details = disk.validate_drive(path);
dir_lba = dir_details.lba;
found=1;
}

//Gets a directory with a given dir name
Directory Directory::get_dir(char *dir_name)
{
long cluster;
int j,k;
static Directory dir_ret(drive);
char buf[64][32];
int flag = 1;
disk.get_sector(dir_lba,0x04,(char *)buf);

for(int i=0;i<64;i++)
{
for(j=0,k=-1;j<11;j++)
   {
   if(k==-1) k++;
   else if(dir_name[k])k++;
   if((buf[i][j] == 0x20 && dir_name[k] == 0) || buf[i][j] == dir_name[k])
       {flag=0;}
   else {flag = 1;break;}
   }
   if(!flag && (j == 11 || dir_name[k] == 0))
   break;
 }
if(!flag)
{

cluster =
   rev_cat(rev_cat(buf[i][26],buf[i][27]),rev_cat(buf[i][20],buf[i][21]));
 dir_ret.dir_lba = d[drive-2].rootdir_lba+(cluster-2)*d[drive-2].get_spc();
 char *temp = (char*)(&dir_ret.dir_details);
 for(j=0;j<32;j++)temp[j] = buf[i][j];
 dir_ret.dir_details.lba = dir_ret.dir_lba;
 dir_ret.found=1;
 return(dir_ret);
}
else
{
dir_ret.found=0;
 return(dir_ret);
 }
}

//Gets entries of a dir
Dir_Entries* Directory::dir_read()
{
static char buf[64][32];
char cnt=0;
int j,i,read_flag;
Dir_Entries dir_entries[64];
for(i=0;i<64;i++)
 for(j=0;j<32;j++)
    buf[i][j]=0;
if(dir_details.lba==0) found=0;
else
{
read_flag=disk.get_sector(dir_details.lba,4,(char*)buf);
for(i=0;i<64;i++)
  {
  j=0;
  if(buf[i][j]==0) break;
  if((buf[i][j]=='å')||(buf[i][11]==0x0f)) continue;
  for(j=0;j<11;j++)  dir_entries[cnt].name[j]=buf[i][j];
  dir_entries[cnt].attribute = buf[i][j];j=15;
  dir_entries[cnt].date = rev_cat(buf[i][j],buf[i][j+1]);j=28;
  dir_entries[cnt].size =
	 rev_cat(rev_cat(buf[i][j],buf[i][j+1]),rev_cat(buf[i][j+2],buf[i][j+3]));
  cnt++;
  }
  dir_entries[cnt].name[0]=0;
}
return(dir_entries);
}

//Removes a entry from a dir
void Directory::remove_entry(char *ent_name,int  dr_num)
{
static char buf[64][32];
char *temp;
char cnt=0;
int j=0,i,k,flag;
Dir_Entries dir_entries[64];
if(dir_details.lba==0) found=0;
else
{
disk.get_sector(dir_details.lba,4,(char*)buf);

for(i=0;i<64;i++)
{
for(j=0,k=-1;j<11;j++)
   {
   if((buf[i][j]==0)||(buf[i][j]=='å')) continue;
   if(k==-1) k++;
   else if(ent_name[k])k++;
   if(ent_name[k] == '.') {j=8;k++;}

   if((buf[i][j] == 0x20 && ent_name[k] == 0) || buf[i][j] == ent_name[k])
       {flag=0;}
   else {flag = 1;break;}
   }
   if(!flag && (j == 11 || ent_name[k] == 0))
   break;
}
buf[i][0]='å';
disk.put_sector(dir_details.lba,4,(char*)buf);
}
}

//Clear a fat entry
void Drive::clear_fat_entry(long lba,int dr_num)
{
long next_lba;
do
{
next_lba=get_fat_entry(lba,dr_num);
set_fat_entry(lba,0,dr_num);
lba=next_lba;
}
while(lba< 0x0ffffff8);
}

//Add a dir
void Directory::add_dir(char *name)
{
int i;
static char buf[64][32];
long cluster1,cluster2,lba;
char *temp;
char cnt=0;
int j=0;
Dir_Entries dir_entries[64];
i = 0;
while(name[i])i++;
while(i<12){name[i] = 0x20;i++;}
if(dir_details.lba==0) found=0;
else
{
disk.get_sector(dir_details.lba,4,(char*)buf);
for(int i=0;i<64;i++)
  {
  if((buf[i][j]==0) ||(buf[i][j]=='å'))
	{
	  for(j=0;j<11;j++) buf[i][j] = name[j];
	  buf[i][j] = 0x10;
	  cluster1=d[drive-2].get_free_fat_entry();
	buf[i][26] = cluster1;
	buf[i][27] = cluster1>>8;
	buf[i][20] = cluster1>>16;
	buf[i][21] = cluster1>>24;
	buf[i][28] = 20;buf[i][29] = 0;buf[i][30] = 0;buf[i][31] = 0;
	disk.put_sector(dir_details.lba,4,(char*)buf);
	break;
	}
  }
for(i=0;i<64;i++)
 for(j=0;j<32;j++)
   buf[i][j]=0;

cluster2=((dir_details.lba-d[drive-2].rootdir_lba)/d[drive-2].get_spc())+2;
buf[0][0]='.';buf[0][1]='.';
buf[0][26] = cluster2;
buf[0][27] = cluster2>>8;
buf[0][20] = cluster2>>16;
buf[0][21] = cluster2>>24;
buf[0][12] = 0x10;
lba= d[drive-2].rootdir_lba+(cluster1-2)*d[drive-2].get_spc();
d[drive-2].set_fat_entry(lba,0x0ffffff8,drive);
disk.put_sector(lba,1,(char*)buf);
}
}


//Get entry details of a directory
struct Entry_details Directory::get_lba(char* name,struct Entry_details file_details,int dr_num)
{
char *f_d;
char buf[64][32];
int flag=1,k;
long cluster;
int index;
disk.get_sector(file_details.lba,0x04,(char *)buf);
for(int i=0;i<64;i++)
{
for(int j=0,k=-1;j<11;j++)
   {
   if(k==-1) k++;
   else if(name[k])k++;
   if(name[k] == '.') {j=8;k++;}

   if((buf[i][j] == 0x20 && name[k] == 0) || buf[i][j] == name[k])
       {flag=0;}
   else {flag = 1;break;}
   }
   if(!flag && (j == 11 || name[k] == 0))
   break;
 }
 cluster =
   rev_cat(rev_cat(buf[i][26],buf[i][27]),rev_cat(buf[i][20],buf[i][21]));
if(!flag)
 {
 f_d =(char*) &file_details;
 index = 0;
 while(index<32){*f_d = buf[i][index];index++;f_d++;}
 file_details.lba = d[dr_num-2].rootdir_lba+(cluster-2)*d[dr_num-2].get_spc();
 return(file_details);
 }
else
{
file_details.lba = 0;
return file_details;
}
}


//Add an entry to a dir
long Directory::enter_new_entry(Dir_Entries ent,long cluster,int dr_num)
{
static char buf[64][32];
char *temp;
char cnt=0;
int j=0;
Dir_Entries dir_entries[64];
if(dir_details.lba==0) found=0;
else
{
disk.get_sector(dir_details.lba,4,(char*)buf);
for(int i=0;i<64;i++)
  {
  if((buf[i][j]==0) ||(buf[i][j]=='å'))
	{
	for(j=0;j<11;j++) buf[i][j] = ent.name[j];
	buf[i][26] = cluster;
	buf[i][27] = cluster>>8;
	buf[i][20] = cluster>>16;
	buf[i][21] = cluster>>24;
	buf[i][28] = 20;buf[i][29] = 0;buf[i][30] = 0;buf[i][31] = 0;
	disk.put_sector(dir_details.lba,4,(char*)buf);
	break;
	}
  }
}
return(dir_details.lba);
}


File::File()
{
eof = 0;
filepointer = 0;
current_char_ptr = 0;
current_clus_ptr = 0;
}


//Write to a file
char File::file_write(char* buf)
  {
  int flag = 1;
  long old_lba;
  long size=0,cluster,num_clust;
  char *name,buffer[4096];
  name = buf;
  while(*(name++)) size++;
  num_clust = size/(d[drive-2].get_spc()*512);
  disk.put_sector(next_lba,d[drive-2].get_spc(),buf);
  old_lba=next_lba;
  while(num_clust)
  {
  cluster = d[drive-2].get_free_fat_entry();
  old_lba = next_lba;
  next_lba = d[drive-2].rootdir_lba+(cluster-2)*d[drive-2].get_spc();
  d[drive-2].set_fat_entry(old_lba,next_lba,drive);
  buf += d[drive-2].get_spc() * 512;
  num_clust--;
  disk.put_sector(next_lba,d[drive-2].get_spc(),buf);
  }
    d[drive-2].set_fat_entry(old_lba,0x0fffffffl ,drive);
  disk.get_sector(dir_lba,d[drive-2].get_spc(),buffer);
  for(int i=0;i<64;i++)
{
for(int j=0,k=-1;j<11;j++)
   {
   if(k==-1)k++;
   if(file_details.file_name[k])k++;
   if(file_details.file_name[k] == '.') {j=8;k++;}

   if((buffer[i*64 + j] == 0x20 && file_details.file_name[k] == 0) || buffer[i*64 + j] == file_details.file_name[k])
       {flag=0;}
   else {flag = 1;break;}
   }
   if(!flag && (j == 11 || file_details.file_name[k] == 0))
   break;
 }
if(!flag)
 {
 buffer[i*32 + 28] = size;
 buffer[i*32 + 29] = size>>8;
 buffer[i*32 + 30] = size>>16;
 buffer[i*32 + 31] = size>>24;
 disk.put_sector(dir_lba,d[drive-2].get_spc(),buffer);
  }
}


//Open a file
int File::file_open(char* name,char mode)
{
int i=0,n=0;
drive=path[0]-'A';
/* CHECK WHETHER ABSOLUTE OR RELATIVE PATH */
if(name[0]== 'C' && name[1] == ':' && name[2] == '\\')
{
if(mode == RW)
  {
  long cluster;
  Dir_Entries dir_entries;
  dir_entries.size=0;
  dir_entries.attribute=0;
  dir_entries.date=0;
  while(name[i])
  {
  if(name[i]==':') n=i;
  if(name[i+1]=='\\') {name[i+1]=0;n=++i;}
  i++;
  }
  name[n]=0;
  n++;
  i=0;
  while(i<11) dir_entries.name[i++]=32;
  i=0;
  while(i<12)
    {
    if((i<8)&&(name[n]=='.')) {i=8;n++;}
    //if(path[n]=='.') i=8;
    dir_entries.name[i++]=name[n];
    name[n++]=0;
    }
  Directory dir(name);
  cluster = d[drive-2].get_free_fat_entry();
  next_lba = d[drive-2].rootdir_lba+(cluster-2)*d[drive-2].get_spc();
  dir_lba = dir.enter_new_entry(dir_entries,cluster,drive);
  }
else
{
file_details = disk.validate_drive_abs(name);
if(file_details.lba == 0)
{
Error_Code =  File_Not_Found ;
return File_Not_Found;
}
else if (file_details.attribute <= mode)
{
next_lba =  file_details.lba;
return 0;
}
else
{
Error_Code = Access_Violation;
return Access_Violation;
}
}

}
else{
/* MOVE HERE FOR RELAIVE PATH */
if(mode == RW)
  {
  long cluster;
  Dir_Entries dir_entries;
  file_details = disk.validate_drive(name);
  if(file_details.lba != 0)return 3;
  dir_entries.size=0;
  dir_entries.attribute=0;
  dir_entries.date=0;
  n=0;
  i=0;
  while(i<11) dir_entries.name[i++]=32;
  i=0;
  while(i<12)
    {
    if((i<8)&&(name[n]=='.')) {i=8;n++;}
    dir_entries.name[i++]=name[n];
    name[n++]=0;
    }
  cluster = d[drive-2].get_free_fat_entry();
  next_lba = d[drive-2].rootdir_lba+(cluster-2)*d[drive-2].get_spc();
  dir_lba = curr_dir.enter_new_entry(dir_entries,cluster,drive);
  return 0;
  }
else
{
file_details = disk.validate_drive(name);
if(file_details.lba == 0)
{
Error_Code =  File_Not_Found;
return File_Not_Found;
}
else if (file_details.attribute <= mode)
{
next_lba =  file_details.lba;
return 0;
}
else
{
Error_Code = Access_Violation;
return Access_Violation;
}
}
}
}


//Read a file
char* File::file_read()
{
static char buf[4096];
int off;
if((filepointer == 0)&&(next_lba >= 0x0ffffff8l)) eof=1;
else if(filepointer == 0)
{
disk.get_sector(next_lba,d[drive-2].get_spc(),buf);
filepointer = d[drive-2].get_spc()*512;
next_lba = d[drive-2].get_fat_entry(next_lba,drive);
}
filepointer -= 2048;
off = (d[drive-2].get_spc()*512 - filepointer - 2048);
return buf + off;
}


//Get a character from a file
char File::fget_char()
{
static long size;
static char *file_buf;
if(current_char_ptr==0)
{
size=get_file_size();
file_buf = file_read();
}
if(!eof && current_char_ptr<=size)
{
++current_char_ptr;
if(current_clus_ptr>=2048)
{
current_clus_ptr=0;
file_buf = file_read();
}
}
else eof=1;
return(file_buf[current_clus_ptr++]);
}

//Get file size
long File::get_file_size()
{
return(rev_cat(
	rev_cat(file_details.size[0],file_details.size[1]),
	rev_cat(file_details.size[2],file_details.size[3])
		    )
      );
}

//Get file attribute
char File::get_file_attr()
{
return(file_details.attribute);
}

//Get file creation date
int File::get_creation_date()
{
return(rev_cat(file_details.create_date[0],file_details.create_date[1]));
}

//Set file size
void File::set_file_size(long size)
{
char *size_ch;
long l;
size_ch = (char *)&size;
file_details.size[0]=size_ch[3];
file_details.size[1]=size_ch[2];
file_details.size[2]=size_ch[1];
file_details.size[3]=size_ch[0];
}

//Set file attribute
void  File::set_file_attr(char attribute)
{
file_details.attribute = attribute;
}


//Set creation date
void File::set_creation_date(int date)
{
char *date_ch;
date_ch = (char *)&date;
file_details.create_date[0] = date_ch[1];
file_details.create_date[1] = date_ch[0];
}




//General Function
int rev_cat(char a,char b)//Reverse concatination
{
int temp=0;
temp=a&0x0ff;
temp+=(b<<8);
return(temp);
}


long rev_cat(int a, int b)//Reverse concatination
{
long temp;
temp=a&0xffff;
temp+=((long)b<<16);
return(temp);
}

//Mount a hard disk
void mount()
{
d[0].initialise(2);
d[1].initialise(3);
}

#endif