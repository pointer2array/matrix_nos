#define BLACK          0
#define BLUE           1
#define GREEN          2
#define CYAN           3
#define RED            4
#define MAGENTA        5
#define BROWN          6
#define LIGHTGRAY      7
#define DARKGRAY       8
#define LIGHTBLUE      9
#define LIGHTGREEN    10
#define LIGHTCYAN     11
#define LIGHTRED      12
#define LIGHTMAGENTA  13
#define YELLOW        14
#define WHITE         15


#define T 255
#define F   0


#define PROC process1

typedef  unsigned char bool;

class process1{
	   public:
		   friend          process_scheduler;
		   unsigned char   cl,act;
		   process1        *next;//,*back;
		   char            name[256];

 process1(int x,int y,bool iwork,char name[50]){

			   ext  = F;

			   ii = 0;
			   i49 = y;
			   i27 = x;
			   work = iwork;
				}

		   ~process1(void){ext=F;ii=0;
		   }

		void draw(void){

				  ii+=1;
				  os.process_tabl[work].pp++;

				  box(i27,1,i49,17,8,7);
				  setattrib(color(0,2));
			       //	  print("M A T R I X",i27-2,19);


					for(int j=2;j<17;j++)
						{
					for(int i=i27+1;i<=i49-1;i+=2)
						  {
						  gotoxy(i,j);
						  mtx_printchar(_rand());
						  }
						}  wait(800);


			    if(ii==400) {ext=T;return;} //exiit!!

			   return;
		   }//DRAW
	   private:

		   //float           ii;//time
		   int           ii;//time
		   bool            ext,work;
		   int i49,i27;
		   //int (* func)()

};

/////////////////
class process_scheduler{
	   public:

		   bool            no_process,deact,i;
		   PROC            *first,*last;
		   struct {
				int      p_qt,pp;
				bool     act;
		   }process_tabl[256];
		   process_scheduler(void){}
		   ~process_scheduler(void){}
///*************************************************
		   void qt_checker(PROC *proc){

			   process_tabl[0].p_qt=10;
			   process_tabl[1].p_qt=10;
			   process_tabl[2].p_qt=500;
			   process_tabl[3].p_qt=500;
			   process_tabl[4].p_qt=50;
			   process_tabl[5].p_qt=10;
			   process_tabl[6].p_qt=1;
			   process_tabl[7].p_qt=1;
			   for(i=0;i<=no_process;i++)
				 process_tabl[i].act=T;
			   cur_process=proc;
			   deact=0;
			   halt=F;
			   while(!halt){
 for(process_tabl[cur_process->work].pp=0;process_tabl[cur_process->work].act&&process_tabl[cur_process->work].p_qt>=process_tabl[cur_process->work].pp;process_tabl[cur_process->work].pp++)
				   {    //  EXECUT PROCESS FOR OWN QUANTOM
					  cur_process->draw();}

				   ch_proc=0;
				   cur_process=next_process(cur_process);

			   }
		   }

/*****************************************************/
	   private:
		   bool  cur_proc_no,halt,ch_proc;
		   PROC *cur_process;

 PROC *next_process(PROC *proc){
  if(no_process>1){
  if(!proc->next->ext&&process_tabl[proc->next->work].act){
  return proc->next;
      }
       else{
       ch_proc++;
       if(ch_proc<=no_process){
		  return next_process(proc->next);
			      }
			     else{
				  if(!deact)
					halt=T;
				 else
				 no_act_process();
			 }
		}
	   }
       else
   if(!proc->ext)
	  return proc;
	   else
		  if(!deact)
			halt=T;
			  else
				 no_act_process();
			  // return NULL;
		   }
 void no_act_process(void){
								   }

}os;
/***********************************************************************/



/*myfunc1(){

	   matrix1(1,7,17,26,32);

	       }
	   myfunc2(){

	    matrix1(1,7,17,36,42);

	    }  * /
/********************************************************************/
//process_scheduler   os;
process1	                A(26,32,BLACK," A ");
process1                        B(26,32,BLUE," B ");
PROC				C(36,42,GREEN," C ");
PROC				D(36,42,CYAN," D ");
PROC				E(36,42,RED," E ");
PROC				G(36,42,MAGENTA," G ");
PROC				H(46,52,BROWN," H ");

void thread(void)
{

   //  ******************** O.S Quantom Cheker*********************

   clear(color(0,15)) ;
   os.no_process=7;



   A.next   = &B;
   B.next   = &C;
   C.next   = &D;
   D.next   = &E;
   E.next   = &G;
   G.next   = &H;

   H.next   = &A;
   os.qt_checker(&A);
   getchar();
   clear(color(0,15)) ;


}


