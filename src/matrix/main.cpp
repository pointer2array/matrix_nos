# include "inout.cpp"
# include "shell.cpp"
# include "matrix.cpp"
# include "enigma.cpp"
# include "splendor\desktop.cpp"


void main()
{

 char user[10],password[10],check=0,repeat=0,escc;
 clear(color(0,15)) ;
 printxy("MATRIX ",1,1);
 printxy("Loading.........",1,2);
// clearusers();
 while((repeat<5)&&(check!=1))
 {
 clear(color(0,15)) ;
 get_pas(user,password);
 priority=1;
 check=user_password(user,password);
 if(!check)
  {
  printxy("Wrong User-name or Password",1,1);
  printxy("Press Esc to quit",1,3);
  escc=getch();
  }
 if(escc==27) break;
 repeat++;
 }
 if(check)
 {
 //mount();
 set_sid();
 matrix();
 shell();
 load();
 }
}
