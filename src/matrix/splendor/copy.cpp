void draw(int,int);
void Fly();
void paper(int,int);
void folders(int,int);
int wd=250,tht=30,ht=150,Color=43;

void draw(int x,int y)
{      bar(x,y,wd,ht,7);           //background
       rectangle(x,y,wd,ht,0);                  //border
       rectangle(x+1,y+1,wd-2,ht-2,15);
       bar(x+2,y+2,wd-4,2,22);
       bar(x+2,y+ht-4,wd-4,2,22);
       bar(x+2,y+2,2,ht-4,22);
       bar(x+wd-4,y+2,2,ht-4,22);
       bar(x+4,y+4,wd-8,20,1);
       folders(x,y);
       load_font(3);
       write(x+10,y+10,15,"COPYING YOUR FILE......");
     /*  asm{	mov ax,0
		int 16h
	  }                  */
}
void paper(int x,int y)
{
       fill_par(x+120,y+50,tht,18,1,15); // flying file
       fill_par(x+120,y+54,2,11,1,0);
       fill_par(x+125,y+54,2,11,1,0);
       fill_par(x+130,y+54,2,11,1,0);
       fill_par(x+135,y+54,2,11,1,0);
       fill_par(x+140,y+54,2,11,1,0);
       /*asm{mov ax,0h
       int 16h;
       } */
       wait(10000);
       bar(x+100,y+45,tht+20,30,7);       //restore
}
void Fly()
{       int x=200,y=100,i=0;
	draw(x,y);
	while(i<10)
	{       paper(x-80,y+30);
		folders(x,y);
		paper(x-50,y);
		paper(x,y);
		paper(x+50,y);
		paper(x+80,y+30);
		folders(x,y);
		i++;
	}
}
void folders(int x,int y)
{      fill_par(x+34,y+90,tht,18,1,Color);         //folders
       fill_par(x+35,y+86,tht-5,15,1,15);  	// file inside
       rectangle(x+17,y+93,tht,16,22);
       bar(x+17,y+94,29,14,Color);
       fill_par(x+200,y+90,tht,18,1,Color);      //folders
       fill_par(x+200,y+86,tht-5,15,1,15);  	// file inside
       rectangle(x+183,y+93,tht,16,22);
       bar(x+183,y+94,29,14,Color);
}