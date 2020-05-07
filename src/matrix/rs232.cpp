#include "inout.cpp"
char sid[10];
struct pack
{
char flag;
char source_address[10];
char destination_address[10];
};
/*- Initiliase the RS232 serial card -*/
	#define INP rec
	#define OUTP send

	/* Defines for RS232 communications */
	#define DLL 0 /* divisor latch low byte */
	#define DLH 1 /* divisor latch high byte */
	#define THR 0 /* transmit hold register */
	#define RBR 0 /* recieve buffer register */
	#define IER 1 /* interrupt enable register */
	#define LCR 3 /* line control register */
	#define MCR 4 /* modem control register */
	#define LSR 5 /* line status register */
	#define MSR 6 /* modem status register */
	#define RTS 0x02 /* request to send */
	#define CTS 0x10 /* clear to send */
	#define DTR 0x01 /* data terminal ready */
	#define DSR 0x20 /* data set ready */
	#define RBF 0x01 /* bit 0 of LSR, rec buf full */
	#define THRE 0x20 /* bit 5 of LSR, trans reg 0 */
	#define DISINT 0x00 /* disable interrupts in IER */
	#define ABRG 0x83 /* access baud rate generator */
	/**/
	unsigned int divisorh, divisorl, format, acia[2];

    char rec(unsigned int add)
     {
     _DX=add;
     asm in al,dx
     return(_AL);
     }

     void send(unsigned int add,int val)
     {
     _DX=add;
     _AL=val;
     asm out dx,al
     }
	void rs232_init( com_port, baud_rate, parity, stops, word_size )
	int com_port, baud_rate, word_size, stops;
	char *parity;
	{

		int far *bios = (int far *)0x00400000l;
		acia[0] = *bios; /* pick up address of com1 routine */
		acia[1] = *(bios + 1); /* pick up address of com2 routine */
		OUTP(acia[com_port] + IER, DISINT ); /* disable ints */
		OUTP(acia[com_port] + LCR, ABRG ); /* access baud rate gen*/
		switch( baud_rate ) {
			/* rem case 75, 110, 135, 150, 200, 1800, 19200 */
			case 300 : divisorh = 01; divisorl = 0x80; break;
			case 600 : divisorh = 00; divisorl = 0xc0; break;
			case 1200 : divisorh = 00; divisorl = 0x60; break;
			case 2400 : divisorh = 00; divisorl = 0x30; break;
			case 4800 : divisorh = 00; divisorl = 0x18; break;
			case 9600 : divisorh = 00; divisorl = 0x0c; break;
			//default: printf("\nrs232_init: Error: Baud rate invalid.\n");
			      //	return -1;
		} /* end of switch */
		OUTP(acia[com_port] + DLL, divisorl );
		OUTP(acia[com_port] + DLH, divisorh );
		format = 0; /* This sets bit 6 and 7 to zero */
		if( (streq( parity, "E" ) == 0) || (streq( parity, "O" ) == 0) ) {
			format = format | 0x28; /* set bit 3 and 5 */
			if( streq( parity, "E" ) == 0 )
				format = format | 0x10; /* set bit 4 */
		}
		if( stops == 2 )
			format = format | 0x04;
		switch( word_size ) {
			case 8 : format = format | 0x03; break;
			case 7 : format = format | 0x02; break;
			case 6 : format = format | 0x01; break;
			case 5 : break;
			default: putmsg("\nrs232_init: Unsupported word length.\n");
			    //	return -1;
		} /* end of switch */
		OUTP(acia[com_port] + LCR, format );
		//return 0;
	}

	/* Transmit a single character to RS232 card -*/
	void transmit( byte,comport )
	char byte;
	int comport;
	{
		OUTP(acia[comport] + MCR, (RTS | DTR) ); /* assert RTS and DTR */
	while((INP(acia[comport] + LSR) & THRE)==0) /* trans reg empty? */
			;
		OUTP(acia[comport] + THR, byte ); /* write character to THR */
		OUTP(acia[comport] + MCR, 0 );
	}

	/* Receive a single character from RS232 card */
	char receive(int comport) 	{
		char byte;
		OUTP(acia[comport] + MSR, (RTS | DTR) );
		while((INP(acia[comport]+LSR) & RBF)==0) /* has Data arrived? */
			;
		OUTP(acia[comport] + MCR,0); /* stop all data */
		byte = INP(acia[comport] + RBR); /* get byte RBR */
		return( byte );
	}
int get_port_status(int port)
{
_AH=3;
_DX=port;
asm int 0x14
return _AX;
}

void receive_msg()
{
char ch=1;
rs232_init(0,9600,0,1,8);
//clear(color(0,2));
putchar(13);
while((ch!=26)&&(ch!=0))
{
ch=receive(0);
putchar(ch);
}
putchar(13);
}



void ping(char des_address[10])
{
char *buff;
rs232_init(0,9600,0,1,8);
static pack pack1;
pack1.flag = 0x01;
for(int i = 0;i<10;i++)pack1.source_address[i] = sid[i];
for(i = 0;i<10;i++)pack1.destination_address[i] = des_address[i];
//transmit(pack1.flag,0);
transmit(1,0);
for(i=0;i<10;i++)transmit(pack1.source_address[i],0);
for(i=0;i<10;i++)transmit(pack1.destination_address[i],0);
if(receive(0))putmsg("Destination Found within  1 hops");
else putmsg("Destination Not Found within  1 hops");
putchar(13);
}

void listen()
{
char c[10];
char ch=0;
pack pack1;
rs232_init(0,9600,0,1,8);
for(int j=0;j<10;j++) c[j]=0;
while(ch!=0x01)
{
ch=receive(0);
}
if( ch == 1){
putchar(13);
 putmsg("Pinging  From ");
 x+=15;gotoxy(x,y);
 //putchar(13);
 for(int i=0;i<10;i++)
   {
   ch=receive(0);
   if(ch!=0) putchar(ch);
   }
 putmsg(" To ");
 x+=5;gotoxy(x,y);
 for(j=0;j<10;j++)putchar(c[j]=receive(0));
 if(streq(c,sid)) transmit(1,0);
 else transmit(0,0);
 putchar(13);
 //break;
//}
}
}
void send_msg()
{
char c=0;
rs232_init(0,9600,0,1,8);
//clear(color(0,2));
putchar(13);
while(c!=26)
{
c = getch();
putchar(c);
transmit(c,0);
}
send(0,0);
putchar(13);
}

void set_sid(char *sids)
{
char buf[512];
char dr;
dr=Drive_No;
Drive_No=0x80;
disk.get_sector(49,1,buf);
Drive_No=dr;
for(int i=0;i<10;i++)sid[i] = 0;
for(i=0;i<10&&sids[i];i++)sid[i] = sids[i];
for( i=0;i<10;i++)buf[i] = 0;
for(i=0;i<10&&sid[i];i++)buf[i] = sids[i];
dr=Drive_No;
Drive_No=0x80;
disk.put_sector(49,1,buf);
Drive_No=dr;
}

void set_sid()
{
 char buf[512];
char dr;
dr=Drive_No;
Drive_No=0x80;
disk.get_sector(49,1,buf);
Drive_No=dr;
for(int i=0;i<10;i++)sid[i] = 0;
for(i=0;i<10&&buf[i];i++)sid[i] = buf[i];
}

void get_sid()
{
putmsg("System ID ==>> ");x+=15;
putmsg(sid);
putchar(13);
}
