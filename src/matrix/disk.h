int rev_cat(char,char);
long rev_cat(int,int);

struct Dir_Entries
  {
   char name[11],attribute;
   int date;
   long size;
  };

struct Entry_details
  {
  char file_name[11],attribute,nt[2],create_time[2],create_date[2],
       access_date[2],cluster_high[2],time_date[4],cluster_low[2],size[4];
       long lba;
  };

	//********* File Class ***********//

class File
{
private:
  long dir_lba;
  int filepointer;

   long current_char_ptr;
  int current_clus_ptr;

public:
  int drive;//Which partition the file is
  int file_open(char* ,char);
  struct Entry_details file_details;
  File();
   long next_lba;//Points to the next cluster address
  char eof;//It is a Flag, 1 if eof and 0 if not eof
  char* file_read();//Read a file in batches of 2048 bytes
  char file_write(char*);//Write a file by its buf & ret stat.
  char file_delete();//Del a file by its name
long get_file_size();
char get_file_attr();
int get_creation_date();
void set_file_size(long);
void set_file_attr(char);
void set_creation_date(int);
char fget_char();
};

		//********* Directory Class ***********//

class Directory
{
public:
    int drive;//Which partition the dir is
  Directory();
  Directory(char);
  Directory(char*);
   long dir_lba;
  Directory get_dir(char *dir_name);

  long enter_new_entry(Dir_Entries,long,int);// enter into directory the given entries
  void remove_entry(char*,int);
  Dir_Entries* dir_read();
  struct Entry_details dir_details;
  File get_entry(char *path);
  struct Entry_details get_lba(char*,struct Entry_details,int);
  struct Entry_details get_lba_abs(char*,struct Entry_details,int);
  char found;
  void add_dir(char*);
};

		   //********* Disk Class ***********//

class Disk
{
public:
//Constructor used to find all the drives available and put it to partition
Disk();

public:
  char partition[4][16]; //Max 4 drives possible.
  int num_of_drives;
  struct Entry_details validate_drive(char*);//Returns -1 if no dir or dir number
  struct Entry_details validate_drive_abs(char*);//Returns -1 if no dir or dir number
  int get_sector(long lba,char num_sectors,char *buf);
  int put_sector(long lba,char num_sectors,char *buf);
  long chs_to_lba(char track,char head,char sector);
  long chs_to_lba(int track,int head,int sector);
};

		      //********* Drive Class ***********//

class Drive //Stores Drive Info
{
public:
/*   struct Drive_Header
 {
   char jump[3],oem_id[8],bps[2],spc,reserved_sect[2],
      num_fat,root_entries[2],small_sect[2],media_desc,
      spf_older[2],spt[2],heads[2],hidden_sect[4],sect_in_partition[4],
      sect_per_fat[4],flag[2],ver_fat32[2],clust_start_root[4],sect_no_FSLS[2],
      sec_no_Backup_Boot_Sec[2],Reserved[12],logical_drive_no,unused,
      ext_sign,serial_no_part[4],volumenameof_part[11],fatname[8],
      code[420],sign[2];
 }*drive_header;*/
  long rootdir_lba,fat1_lba;
  char drive_buf[512];
  //char *drive_buf;
  void initialise(int drive_num);//Read the drive header , fat hts , rootdir hts and store it
  char* dataread(long);//Read data by its cluster no.
  long get_fat_entry(long,int);
  long get_free_fat_entry();
  int set_fat_entry(long,long,int);
  void clear_fat_entry(long,int);
 // long validate_entry(char*,long,int);
  void datawrite(long,char*);//Write data by its cluster no.
  char get_spc();
  struct Entry_details validate_entry(char* path,struct Entry_details file_details,int dr_num);
  struct Entry_details validate_entry_abs(char* path,struct Entry_details file_details,int dr_num);

};
