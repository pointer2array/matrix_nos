int size,s,off;
char font;
void write(int x,int y,char color,char *str)
{    int i,j,k=0,t1,temp,rows;
     char m=0x80,c,o,p2;
     long p,t;
     asm push es;
     temp=off;
	 if (font==3 && mode!=0x103)
     rows=size/2;
     else  rows=size;
     if ((font==2 || font==6)&& mode==0x103)
     rows=size*2;
     p=(long)(y)*xmax+(long)(x);
     p2=p>>16;
     setbank(p2);
     t=p%max;

    for(k=0;str[k]!='\0';k++)
    {   temp=off+(str[k]*rows);
	for(i=0;i<rows;i++)
	{	asm {	mov bx,s;
			mov es,bx;
			mov di,temp;
			mov cl,es:[di];
			mov c,cl;
		    }
		m=0x80;
		for(j=7;j>=0;j--)
		{       o=c&m;
			asm shr m,1;
			if(o)
			*((char far *)0xA0000000+t)=color;
			t++;
			if(t>=65536){p2++;setbank(p2);  t=0;}
		}
		temp++;
		t=t+xmax-8;
		if(t>=max)
		{p2++;setbank(p2);t=t%max;}
	}
	t=p+(k+1)*8;
	p2=t>>16;
	setbank(p2);
	t%=max;
     }
     asm pop es;
}

void load_font(char ftype)
{
	font=ftype;
	asm push es;
	asm {   mov ah,11h;
		mov al,30h;
		mov bh,ftype;
		int 10h;
		mov size,cx;
		mov bx,es;
		mov s,bx;
		mov off,bp;
	    }
       asm pop es;
}



