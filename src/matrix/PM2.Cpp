
#include "pm_defs.h"
extern "C" unsigned int READ_MSW();
extern "C" unsigned long READ_CR0();
extern "C" void WRITE_CR0 (unsigned long value);
extern "C" void LGDT (GDTR *gdtr);

extern "C" void UPDATE_CS (unsigned int bew_cs);




DESCR_SEG gdt[5];			/* GDT */
GDTR gdtr;				/* GDTR */
word old_CS, old_DS, old_SS;

char msg[]="H e l l o   f r o m   P M o d e                   ! ";

void setup_GDT_entry (DESCR_SEG *item,
		      dword base, dword limit, byte access, byte attribs) {
  item->base_l = base & 0xFFFF;
  item->base_m = (base >> 16) & 0xFF;
  item->base_h = base >> 24;
  item->limit = limit & 0xFFFF;
  item->attribs = attribs | ((limit >> 16) & 0x0F);
  item->access = access;
}


int pmrm() {
  char far *scr;
  void far *mk;
  char *p, c;


  if (READ_MSW() & 1) {
    putmsg ("  The CPU is already in PMode \n  Aborting...");
    getchar();
    return 0;
  };

  putmsg("        This is a +386 system     \n");
  getch();
  putmsg("        Entering Protected Mode   \n");
  getch();

  /* 0x00 -- null descriptor */
  setup_GDT_entry (&gdt[0], 0, 0, 0, 0);

  /* 0x08 -- code segment descriptor */
  setup_GDT_entry (&gdt[1], ((dword)_CS)<<4, 0xFFFF, ACS_CODE, 0);

  /* 0x10 -- data segment descriptor */
  setup_GDT_entry (&gdt[2], ((dword)_DS)<<4, 0xFFFF, ACS_DATA, 0);

  /* 0x18 -- stack segment descriptor */
  setup_GDT_entry (&gdt[3], ((dword)_SS)<<4, 0xFFFF, ACS_STACK, 0);

  /* 0x20 -- text video mode segment descriptor */
  setup_GDT_entry (&gdt[4], 0xB8000L, 0xFFFF, ACS_DATA, 0);




  /* disable interrupts so that IRQs don't cause exceptions */
  asm cli

  /* setting up the GDTR register */
  gdtr.base = ((dword)_DS)<<4;
  gdtr.base += (word)&gdt;
  gdtr.limit = sizeof(gdt)-1;
  LGDT(&gdtr);

  /* saving real mode segment addresses */
  old_CS = _CS;
  old_DS = _DS;
  old_SS = _SS;

  /*  This switches us to PMode just setting up CR0.PM bit to 1 */
  WRITE_CR0 (READ_CR0() | 1);

  /* loading segment registers with PMode selectors */
  UPDATE_CS (0x08);
  _ES = _DS = 0x10;
  _SS = 0x18;

  /* writing a message to the screen */
 //  scr = MK_FP (0x20, 80*4);             /* selector for segment 0xB800 is 0x20 */
 mk =(void far*)0x200140;
 scr =(char far*)mk;
  p = msg;                        //0x200140
  for(;;) {
    c = *p++;
    if (c) *scr++=c;
    else break;
  }

  /* get out of PMode clearing CR0.PM bit to 0 */
  WRITE_CR0 (READ_CR0() & 0xFFFFFFFEL);

  /* restoring real mode segment values */
  UPDATE_CS (old_CS);
  _ES = _DS = old_DS;
  _SS = old_SS;

  /* enabling interrupts */
  asm sti
  putmsg("\n  Back in Real Mode  ");
  return 0;
}
