������������������������������������������������������������������������������������������
MATRIX NOS V.1(matrix network os)

Final Year Project.

Team
TTT

Government Engineering College(GEC),Barton Hill,
Trivandrum,India. 1999-2003

������������������������������������������������������������������������������������������



We are extremely grateful to ( Internal Project guide, Staff Advisors ) 
and various resources on the net which enabled us to do our project successfully.
Credits to Brian Brown,Alexei A. Frounze,Iman Ghasr-e-fakhri.
������������������������������������������������������������������������������������������

Features

Real Mode
16 bit    
x86

I/O
File Handling
File system
Fat 12 
Fat 32

Booting 
Memory
Program Execution

Security
- protect bin files
- encryption
- priority level

IO control
     Key Board
     Monitor
     Floppy
     Hard disk
     Ports

Networking -
message transfer
ping
listen

Error handling

Shell        
- CLI
- Multi page
- Multi user Environment

GUI(splendor-done as another project in GEC)
multithreading

������������������������������������������������������������������������������������������

Applications


1)   This operating system can be used by students for gaining a thorough understanding of the concepts of 
Operating Systems.

2)   This can also be used by Teachers to give the students a hands-on exposure to the various concepts related to OS like Bootstrap Loader, Bios handling,Shell                             
development, I/O handling etc�

3)   When developed fully this OS can be used just like any other  Commercial Operating System.

4)   The understanding of this OS will help the user to understand the PC hardware better.

5)    This source code is available to students as OPEN SOURCE - CODE
 so that they can modify/upgrade  it to get a thorough knowledge.

******************************************************************************************

Other works that can be done over this project or by changing the source
1.protocol stacks (networks).
2.applications.
3.optimize the source to develop a very fast switching s/w on Network dev.
4.file systems eg ext2...
5.support for other executables like elf
6.more works on GUI. 
7.drivers 
8.protected mode
9.Memory,proceess
10.message systems and IPC.
******************************************************************************************
Compiling and exectuing:

Tools needed: nasm,(turbo)TCC,bootable.exe

>nasm -f -obj <filename.asm>  // to get obj

main.obj(compiled main.cpp)

make project using turbo(TCC) using these objs.
make(link) to get an exe.

Make the Floppy Bootable.
>Bootable boot12.bin a:

>copy main.exe a:\kerimage.mtx
 
kerimage.mtx(11 char name) this is searched by the Boot12 pgm.

boot13.bin can be any com or exe pgm used for loading example.so copy it to a:
(only used when "run" command is executed).

Connect COM1 of two systems for networking.(check rs232.gif, with handshake)

Restart and check out MATRIX NOS
******************************************************************************************
******************************************************************************************

login as 
     _____________
usr |root         |
     ------------- 
     _____________ 
pwd |matrix       |
     -------------

!READ IMPORTANT.txt!


MATRIX
Goal


To implement a basic structure of a kernel (generic) incorporating 
Essential features.




Real Mode
16 bit    
 x86



I/O
File Handling
File system
Fat 12 
Fat 32


Networking -
- message transfer
- file transfer
- ping
- listen


IO control
Key Board
Monitor
Floppy
Hard disk
Port






Error handling
Shell        
- CLI
- Multi page
- Multi user Environment



    support for GUI

    multithreading



Boot Process
Supply On
Bios code at FFF0h is loaded
POST
Video Card detect 
Device detect 
Startup Screen
System Inventory
Boot sequence BIOS setting
Master Boot Record
Load Boot Loader at 7C00h



Registers

Segment Registers
		CS Code, DS Data ,SS Stack, ES Extra Segment.
Pointer Registers
		IP,SP,BP
General-Purpose Registers
		AX Accumulator BX Base 
		CX Count       DX Data 
		Index Registers
		SI
		DI
Memory Map

Interrupt Vector Table 		00000                          
BIOS Data Area 			00400
PrtScr Status / Unused 		00500                          
Image Load Address 		00600
Global Data Area 		02000
Application Space 		06000
Boot Loader 			07C00
Available Memory 		nnnn                          
2KB Boot Stack 			A0000 - 512 - 2KB              
Boot Sector 			A0000 – 512
Boot Loader 			A0000
Boot Loader
Loaded at 7c00h
Control passed by bios
Basic Stack Allocation 
Load OS at 50h
Control Transfer
Support for both bin, com and exe format
Application Loading

Aim
  To load an executable(binary)
  
I 
	 Procedure
       Save CS ,IP and Registers
       si di for tcc
II
       Call loader
       parm   CS, IP



III
       Search the file 
       Copy to memory (pre defined memory loc) MM
IV
      Pass control to MM

V   
        Restore CS   IP  //int
        IP = addr(exit funtion cret())
        Cret() performs restoring of
        Registers ,si and di 



Loader

Define the Load segment -600h
Save CS & IP                   -200h
Copy file to Load segment
Check the file Signature
if COM or binary
     -
if Exe
 -perform relocation     
 -load sp, ss, cs, ip
 
Restore Registers and return

IO Control
Support for IO primitives
Support for Higher level IO features
CMOS
History

Bios Routines For IO Control
Interrupt

10H  
16H  
15H  
19H
1AH



Functions

Video Support Routines
Keyboard Support Routines
Delay Support Routines
Reboot Functions
CMOS Support Routines

Memory Map

Interrupt Vector Table 		00000                          
BIOS Data Area 			00400
PrtScr Status / Unused 		00500                          
Image Load Address 		00600
Global Data Area 		02000
Application Space 		06000
Boot Loader 			07C00
Available Memory 		nnnn                          
2KB Boot Stack 			A0000 - 512 - 2KB              
Boot Sector 			A0000 – 512
Boot Loader 			A0000






Features supported
Similar features which is supported by fat12
Support for multi-partition
Multi-hard disk support
Developed in OOP Technology
Serial Communication
Uses RS232 com ports for communication
Transfer of messages
Ping

RS232 Connections 
Null Modems 
A Null Modem is used to connect two DTE's together 
It mainly only requires 3 wires (TD, RD & SG) to be wired straight through. The aim is to make to computer think it is talking to a modem rather than another computer. Any data transmitted from the first computer must be received by the second thus TD is connected to RD. The second computer must have the same set-up thus RD is connected to TD. Signal Ground (SG) must also be connected so both grounds are common to each computer.
The connection of other wires depends on whether handshake is needed or not.



Applications
Send messages from one computer to another computer
Check for connection with another computer (ping) using sid

Security


Encryption
Priority Levels
Protection


,

				
Encryption

Combination of 2 classical encryption techniques are used
Transposition
Substitution
In transposition technique the ordering of characters in the text is changed without changing the character.
In substitution technique the character is changed without changing there relative order in the text
In our encryption algorithm the relative ordering as well as the characters are changed
The process starts by first arranging the text into blocks and then within each block the order of the letters is changed according to the alphabetic order of the key.
Then polyalphabetic substitution is done by adding to each character a specific number given by the pseudorandom series.
By following the above said procedure encryption of user name and passwords are done.
Encrypted user name and password are stored in a fixed place in the hard disk.
No user is given direct access to this place.


Multithreading

Class
*process
-param(no:,function specific values)
-status(active,exit)
-next
-identification
-function

Eg: function()-Display  Random Number Generator 



Class
*process_scheduler

-no_process,
-deact
-cur_proc_no,
-halt,
-*cur_process;
  
struct {
		int      process_quantom,process_counter;
		bool     act;
	   }process_table[X];




		   
 void quantom_checker(PROC *proc)
 PROC *next_process(PROC *proc)


Function-Thread()
Set no. of porcesses
Create linked-list
Call quantom_checker()


               Mode  and Enhancements

Real And Protected Modes

Features:32 bit,4 GB

Benifits
-Multitasking,Virtual Memory
-security:process and memory


Conclusion

The source code is available as OPEN SOURCE - CODE so that OS develepers   can modify/upgrade and enhance the project in the future


 Future Enhancements that can be done:
       -Networking
       -File systems
       -Optimisation
       -Device Drivers

Network Booting in devolopment phases


