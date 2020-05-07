#define a2 100
#define a4 10000
#define B 3141
# include "inout.cpp"
int _ran;

int Mult(int p, int q)
{
 int p1,p0,q1,q0;
 p1 = p / a2;
 p0 = p % a2;
 q1 = q / a2;
 q0 = q % a2;
 return ((( p0 * q1 + p1 *q0)%a2) * a2 + p0 * q0) %  a4;
}

int _rand()
{
 _ran = (Mult(_ran,B) + 1) % a4;
 return _ran%128;
}


void matrix()
{
 int i,j;
 char mat[21][16];
 clear(color(0,15));
 for(j=0;j<16;j++)
  for(i=0;i<=20;i++)
    mat[i][j]=' ';
 box(27,1,49,17,8,7);
 setattrib(color(0,2));
 printxy("M A T R I X",32,19);
 setattrib(color(0,2));
 while(keyhit())
 {
  for(j=2;j<17;j++)
  {
   for(i=28;i<=48;i+=2)
   {
    gotoxy(i,j);
    mtx_printchar(mat[i-28][j-2]);
   }
  }
 wait(80000l);
 for(i=0;i<=20;i++)
  for(j=15;j>0;j--)
  {
   mat[i][j]=mat[i][j-1];
  }
 for(i=0;i<=20;i++) mat[i][0]=_rand();
 }
 getch();
}

