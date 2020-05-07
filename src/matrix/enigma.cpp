#ifndef _enigma_
#define _enigma_

# include "inout.cpp"
# include "disk.cpp"

void send_to_array_e(char);
void send_to_array_d(char);
void encrypt(char []);
void decrypt(char []);
void encrypt1();
void decrypt1();
void encrypt2();
void decrypt2();
int get_num_e();
int get_num_d();
void get_key();
int fib(int);
int num_d=1,num_e=1,i_d=0,i_e=0,j_d=0,j_e=0;
static char *curr_user;
unsigned char eng1[50][6],eng2[50][6],enc[50][6],dec[50][6];
static unsigned char key[6][2];

			//Encryption//

void encrypt(char *e)
{
 int r,c;
 for(r=0;r<=50;r++)
  for(c=0;c<6;c++)
  {
   eng1[r][c]=0;
   eng2[r][c]=0;
   enc[r][c]=0;
  }
 for(int i=0;i<300;i++)
 {
 send_to_array_e(e[i]);
 }
}

int gen_num_e()
{
 if(num_e>9) num_e=1;
 return(fib(num_e++));
}

void encrypt2()
{
 int num;
 int r,c;
 for(r=0;r<=50;r++)
  for(c=0;c<6;c++)
   {
    num = gen_num_e();
    /*if((num%2)==0) enc[r][c]+=num;
    else enc[r][c]-=num;   */
    enc[r][c]+=num;
    }
}


void encrypt1()
{
 int r,c;
 for(r=0;r<=50;r++)
  for(c=0;c<6;c++)
   enc[r][c]=eng1[r][key[c][0]];
   //enc[r][c]=eng1[r][c];
}


void send_to_array_e(char ch)
{
 eng1[i_e][j_e]=ch;
 j_e++;
 if(j_e==6)
  {
   i_e++;
   if(i_e==50)
   {
    i_e--;
    encrypt1();
    encrypt2();
   }
 j_e=0;
 }
}

		    //Decryption//

void decrypt(char d[300])
{
 int r,c;
 for(r=0;r<=50;r++)
  for(c=0;c<6;c++)
  {
   eng1[r][c]=0;
   eng2[r][c]=0;
   enc[r][c]=0;
  }
 for(int i=0;i<300;i++)
 {
  send_to_array_d(d[i]);
 };
}


int gen_num_d()
{
 if(num_d>9) num_d=1;
 return(fib(num_d++));
}


void decrypt1()
{
 int r,c;
 for(r=0;r<=50;r++)
  for(c=0;c<6;c++)
   dec[r][key[c][0]]=eng2[r][c];
   //dec[r][c]=eng2[r][c];
}


void decrypt2()
{
 int num;
 int r,c;
 for(r=0;r<=50;r++)
  for(c=0;c<6;c++)
  {
   num = gen_num_d();
   /*if((num%2)==0) eng2[r][c]-=num;
   else eng2[r][c]+=num; */
   eng2[r][c]-=num;
  }
}


void send_to_array_d(char ch)
{
 eng2[i_d][j_d]=ch;
 j_d++;
 if(j_d==6)
 {
  i_d++;
  if(i_d==50)
  {
   decrypt2();
   decrypt1();
  }
  j_d=0;
  }
}


			//Key//

void get_key()
{
 char temp1,temp2;
 int s,t;
 key[0][1]='M';key[0][0]=0;
 key[1][1]='A';key[1][0]=1;
 key[2][1]='T';key[2][0]=2;
 key[3][1]='R';key[3][0]=3;
 key[4][1]='I';key[4][0]=4;
 key[5][1]='X';key[5][0]=5;
 for(t=0;t<5;t++)
  for(s=0;s<5;s++)
   if(key[s][1]>key[s+1][1])
    {
     temp1=key[s][0];temp2=key[s][1];
     key[s][0]=key[s+1][0];key[s][1]=key[s+1][1];
     key[s+1][0]=temp1;key[s+1][1]=temp2;
    }
}


int fib(int n)
{
 if ((n==1)||(n==2)) return(1);
 else n=fib(n-1)+fib(n-2);
 return(n);
}


char user_password(char *user,char *password)
{
 static char buf[512],uspas[15][20];
 char *nm1,*nm2;
 int t=0;
 int cnt=0;
 int a=0,b=0;
 if((streq(user,"root"))&&(streq(password,"matrix"))){priority=1;curr_user=user;return 1;}
 else
 {
  get_key();
  char dr;
  dr=Drive_No;
  Drive_No=0x80;
  disk.get_sector(50,1,buf);
  Drive_No=dr;
  decrypt(buf);
  cnt=0;
  for(a=0;a<50;a++)
   for(b=0;b<6;b++)
    buf[cnt++]=dec[a][b];
  cnt=0;
  int j=0;
  for(int i=0;i<15;i++)
  {
   if(buf[i*20]==0) continue;
   else
   {
    for(j=1;j<11;j++) uspas[cnt][j-1]=buf[i*20+j];
    for(j=11;j<20;j++) uspas[cnt][j-1]=buf[i*20+j];
    cnt++;
   }
  }
  while(cnt>0)
  {
   nm1=uspas[t];
   nm2=uspas[t]+10;
   if((streq(user,nm1))&&(streq(password,nm2))) {priority=2;curr_user=user;return 1;}
   cnt--;
   t++;
  }
 }
 return(0);
}


void adduser()
{
 static char buf[512];
 static char user[10],pass[9],rpass[9];
 int a,b,cnt=0;
 if(priority!=1)
 {
  putmsg("Not authorised to add users");
  putchar(13);
  return;
 }
 get_key();
 gotoxy(x+1,y);
 putmsg("User_name :  ");
 x+=20;
 gotoxy(x,y);
 get_string(10,user);
 putchar(13);
 putmsg("Password :  ");
 x+=20;gotoxy(x,y);
 get_stringw(9,pass,'*');
 putchar(13);
 putmsg("Re-enter Password :  ");
 x+=20;gotoxy(x,y);
 get_stringw(9,rpass,'*');
 putchar(13);
 if(streq(pass,rpass))
 {
  char dr;
  dr=Drive_No;
  Drive_No=0x80;
  disk.get_sector(50,1,buf);
  Drive_No=dr;
  num_d=1;num_e=1;i_d=0;i_e=0;j_d=0;j_e=0;
  decrypt(buf);
  cnt=0;
  for(a=0;a<50;a++)
   for(b=0;b<6;b++)
    buf[cnt++]=dec[a][b];
  int j=0,count;
  cnt=0;
  for(int i=0;i<15;i++)
  {
   if(j==0)
   {
    if(buf[i*20+j]==0)
    {
     count=i*20+j;
     buf[count]=1;
     for(j=1;j<11;j++) buf[count+j]=user[j-1];
     for(j=11;j<20;j++) buf[count+j]=pass[j-11];
     break;
    }
    cnt++;
   }
   j=0;
  }
 if(cnt<=14)
 {
  get_key();
  cnt=0;
  encrypt(buf);
  for(a=0;a<50;a++)
   for(b=0;b<6;b++)
    buf[cnt++]=enc[a][b];
  char dr;
  dr=Drive_No;
  Drive_No=0x80;
  disk.put_sector(50,1,buf);
  Drive_No=dr;
 }
  else
  {
   putmsg("No more user can be added");
   putchar(13);
  }
 }
 else
 {
  putmsg("Passwords don't match");
  putchar(13);
 }
}


void remuser()
{
 static char buf[512];
 static char user[10],pass[9],rpass[9],us[10],pas[9];
 int a,b,cnt=0;
 if(priority!=1)
 {
  putmsg("Not authorised to remove user");
  putchar(13);
  return;
 }
 get_key();
 gotoxy(x+1,y);
 putmsg("User_name :  ");
 x+=20;
 gotoxy(x,y);
 get_string(10,user);
 putchar(13);
 putmsg("Password :  ");
 x+=20;gotoxy(x,y);
 get_stringw(9,pass,'*');
 putchar(13);
 putmsg("Re-enter Password :  ");
 x+=20;gotoxy(x,y);
 get_stringw(9,rpass,'*');
 putchar(13);
 if(streq(pass,rpass))
 {
  char dr;
  dr=Drive_No;
  Drive_No=0x80;
  disk.get_sector(50,1,buf);
  Drive_No=dr;
  num_d=1;num_e=1;i_d=0;i_e=0;j_d=0;j_e=0;
  decrypt(buf);
  cnt=0;
  for(a=0;a<50;a++)
   for(b=0;b<6;b++)
    buf[cnt++]=dec[a][b];
  int j=0;
  cnt=0;
  for(int i=0;i<15;i++)
  {
   cnt++;
   if(buf[i*20]==0) {continue;}
   else
   {
    for(j=1;j<11;j++) us[j-1]=buf[i*20+j];
    for(j=11;j<19;j++) pas[j-11]=buf[i*20+j];
    if((streq(us,user))&&(streq(pas,pass)))
    {
     buf[i*20]=0;
     break;
    }
   }
  }
  if(cnt<=14)
  {
   get_key();
   cnt=0;
   encrypt(buf);
   for(a=0;a<50;a++)
    for(b=0;b<6;b++)
     buf[cnt++]=enc[a][b];
   char dr;
   dr=Drive_No;
   Drive_No=0x80;
   disk.put_sector(50,1,buf);
   Drive_No=dr;
  }
  else
  {
   putmsg("Wrong user-name or password");
   putchar(13);
  }
 }
 else
 {
  putmsg("Passwords don't match");
  putchar(13);
 }
}


void remusers()
{
 int a,cnt=0,b;
 char buf[512];
 if(priority!=1)
 {
  putmsg("Not authorised to remove users");
  putchar(13);
  return;
 }
 for(a=0;a<512;a++) buf[a]=0;
 get_key();
 encrypt(buf);
 for(a=0;a<50;a++)
  for(b=0;b<6;b++)
   buf[cnt++]=enc[a][b];
 buf[509]='m';buf[510]='t';buf[511]='x';
 char dr;
 dr=Drive_No;
 Drive_No=0x80;
 disk.put_sector(50,1,buf);
 Drive_No=dr;
}


void clearusers()
{
 int a,cnt=0,b;
 char buf[512];
 char dr;
 dr=Drive_No;
 Drive_No=0x80;
 disk.get_sector(50,1,buf);
 Drive_No=dr;
 if(buf[509]=='m'&&buf[510]=='t'&&buf[511]=='x') return;
 for(a=0;a<509;a++) buf[a]=0;
 get_key();
 encrypt(buf);
 cnt=0;
 for(a=0;a<50;a++)
  for(b=0;b<6;b++)
   buf[cnt++]=enc[a][b];
 dr=Drive_No;
 Drive_No=0x80;
 buf[509]='m';buf[510]='t';buf[511]='x';
 disk.put_sector(50,1,buf);
 Drive_No=dr;
}

#endif