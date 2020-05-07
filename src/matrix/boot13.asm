;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;This code loads   start13.bin(binary,com,exe) to mem loc 600h     ;;
;;;;;;;;;;;;extern valuesused to save states          ;;  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


extern _cip
extern _ccs
extern _load

global _run13; may be used as the function calling this routine

SEGMENT _TEXT PUBLIC CLASS=CODE USE16
_run13:         
              
jmp short start

ImageLoadSeg            equ     600h


;bpstore DW 0
spstore DW 0

bpbBytesPerSector       DW      0x0200                
bpbSectorsPerCluster    DB      0x01             
bpbReservedSectors      DW      0x0001               
bpbNumberOfFATs         DB      0x02              
bpbRootEntries          DW      0x00E0             
bpbTotalSectors         DW      0x0B40              
bpbMedia                DB      0xF0              
bpbSectorsPerFAT        DW      0x0009               
bpbSectorsPerTrack      DW      0x0012              
bpbHeadsPerCylinder     DW      0x0002             
bpbHiddenSectors        DD      0x00000000              
bpbTotalSectorsBig      DD      0x00000000             

bsDriveNumber           DB      0x00            


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Boot sector code starts here ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;***********save at 200h ****************************************
start:

  mov ax,200h ; save ip cs
  mov es,ax
  mov ax ,word [_cip]
  mov [es:0], ax
  mov ax,word [_ccs] 
  mov [es:10],ax
	
;----------------------------
;********************     67h -to restore reg values****
;---------------------------ISR
	
 xor  ax,ax;
  mov ds,ax  ; to mark bottom of mem
mov ax,_cret : ip
  cli
  xchg  [ds:21h*4],ax
  xchg [ds:67h*4],ax 
  mov ax,cs
  xchg  [ds:21h*4+2],ax
  xchg [ds:67h*4+2],ax
  sti

jmp foll
	_cret:
        cmp ah,4ch
                 
        je fow
            
        int 67h
        iret

       fow:
        
	mov ax,200h
	mov es,ax
	mov ax,[es:10]
	push ax
	mov ax, word [es:0]
	push ax   
jmp ir
        
ir: 
 sti 
retf          
       

foll:             
        
      cld

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; new loc
;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        
       mov ax, 1024


        mov     es, ax          ; es:0 -> top 

        sub     ax, 2048 / 16   ; reserve 2048 bytes for the stack
        mov     ss, ax          ; ss:0 -> top - 512 - 2048
        mov     sp, 2048        ; 2048 bytes for the stack


        push    cs
        pop     ds

        mov     [bsDriveNumber], dl     ; store boot drive number

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reserve some memory for a FAT12 image (6KB max) and load it whole ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        mov     ax, [bpbBytesPerSector]
        shr     ax, 4                   ; ax = sector size in paragraphs
        mov     cx, [bpbSectorsPerFAT]  ; cx = FAT size in sectors
        mul     cx                      ; ax = FAT size in paragraphs

        mov     di, ss
        sub     di, ax
        mov     es, di
        xor     bx, bx                  ; es:bx -> buffer for the FAT

        mov     ax, [bpbHiddenSectors]
        mov     dx, [bpbHiddenSectors+2]
        add     ax, [bpbReservedSectors]
        adc     dx, bx                  ; dx:ax = LBA

        call    ReadSector

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reserve some memory for a root directory and load it whole ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        mov     bx, ax
        mov     di, dx                  ; save LBA to di:bx

        mov     ax, 32
        mov     si, [bpbRootEntries]
        mul     si
        div     word [bpbBytesPerSector]
        mov     cx, ax                  ; cx = root directory size in sectors

        mov     al, [bpbNumberOfFATs]
        cbw
        mul     word [bpbSectorsPerFAT]
        add     ax, bx
        adc     dx, di                  ; dx:ax = LBA

        push    es                      ; push FAT segment (2nd parameter)

        push    word ImageLoadSeg
        pop     es
        xor     bx, bx                  ; es:bx -> buffer for root directory

        call    ReadSector

        add     ax, cx
        adc     dx, bx                  ; adjust LBA for cluster data

        push    dx
        push    ax                      ; push LBA for data (1st parameter)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Look for a COM/EXE program to be load and run ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        mov     di, bx                  ; es:di -> root entries array
        mov     dx, si                  ; dx = number of root entries
        mov     si, ProgramName           ; ds:si -> program name

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Looks for a file with particular name ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Input:  DS:SI -> file name (11 chars) ;;
;;         ES:DI -> root directory array ;;
;;         DX = number of root entries   ;;
;; Output: SI = cluster number           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

FindName:
        mov     cx, 11
FindNameCycle:
        cmp     byte [es:di], ch
        je      FindNameFailed          ; end of root directory
        pusha
        repe    cmpsb
        popa
        je      FindNameFound
        add     di, 32
        dec     dx
        jnz     FindNameCycle           ; next root entry
FindNameFailed:
        jmp     ErrFind
FindNameFound:
        mov     si, [es:di+1Ah]         ; si = cluster no.

;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load entire a program ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;

ReadNextCluster:
        call    ReadCluster
        cmp     si, 0FF8h
        jc      ReadNextCluster         ; if not End Of File

;;;;;;;;;;;;;;;;;;;
;; Type checking ;;
;;;;;;;;;;;;;;;;;;;

;        cli                             ; for stack adjustments

        mov     ax, ImageLoadSeg
        mov     es, ax

        cmp     word [es:0], 5A4Dh      ; "MZ" signature?
        
       
       
 je      RelocateEXE             ; yes, it's an EXE program

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Setup and Run COM program ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


 mov ax,300h
	mov es,ax
	mov [es:0h], word return2
        mov ax,cs
	mov [es:10h],ax


        mov     ax, es
        sub     ax, 10h                 ; "org 100h" stuff :)
        mov     es, ax
        mov     ds,ax
        mov ss,ax   
        

        push cs                   ;return cs address// to be returned from the calling prog
        push word return2           ;return ip

        push    es
        push    word 100h
        
jmp     Run

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Relocate, setup and run EXE program ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

RelocateEXE:
        mov     ds, ax

        add     ax, [ds:08h]            ; ax = image base
        mov     cx, [ds:06h]            ; cx = reloc items
        mov     bx, [ds:18h]            ; bx = reloc table pointer

        jcxz    RelocationDone

ReloCycle:
        mov     di, [ds:bx]             ; di = item ofs
        mov     dx, [ds:bx+2]           ; dx = item seg (rel)
        add     dx, ax                  ; dx = item seg (abs)

        push    ds
        mov     ds, dx                  ; ds = dx
        add     [ds:di], ax             ; fixup
        pop     ds

        add     bx, 4                   ; point to next entry
        loop    ReloCycle

RelocationDone:

        mov     bx, ax
        add     bx, [ds:0Eh]
        mov     ss, bx                  ; ss for EXE
        mov     sp, [ds:10h]            ; sp for EXE

        add     ax, [ds:16h]            ; cs
        push    ax
        push    word [ds:14h]           ; ip
Run:
        mov     dl, [cs:bsDriveNumber]  ; let program know boot drive
;        sti
       ;mov ax,200h
	;mov es,ax
	;mov ax,[es:10]
	;push ax
	;mov ax, word [es:0]

	;push ax
	;retf

      
        retf 
   

;///////////////////////////////////////////////////////

return2:  ;for binary


       mov ax,200h
	mov es,ax
	mov ax,[es:10]
	push ax
	mov ax, word [es:0]
	push ax
	retf


;////////////////////////////////////////////////////////
        

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reads a FAT12 cluster      ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Inout:  ES:BX -> buffer    ;;
;;         SI = cluster no    ;;
;; Output: SI = next cluster  ;;
;;         ES:BX -> next addr ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ReadCluster:
        mov     bp, sp

        lea     ax, [si-2]
        xor     ch, ch
        mov     cl, [bpbSectorsPerCluster]
                ; cx = sector count
        mul     cx

        add     ax, [ss:bp+1*2]
        adc     dx, [ss:bp+2*2]
                ; dx:ax = LBA

        call    ReadSector

        mov     ax, [bpbBytesPerSector]
        shr     ax, 4                   ; ax = paragraphs per sector
        mul     cx                      ; ax = paragraphs read

        mov     cx, es
        add     cx, ax
        mov     es, cx                  ; es:bx updated

        mov     ax, 3
        mul     si
        shr     ax, 1
        xchg    ax, si                  ; si = cluster * 3 / 2

        push    ds
        mov     ds, [ss:bp+3*2]         ; ds = FAT segment
        mov     si, [ds:si]             ; si = next cluster
        pop     ds

        jnc     ReadClusterEven

        shr     si, 4

ReadClusterEven:
        and     si, 0FFFh               ; mask cluster value
ReadClusterDone:
        ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reads a sector using BIOS Int 13h fn 2 ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Input:  DX:AX = LBA                    ;;
;;         CX    = sector count           ;;
;;         ES:BX -> buffer address        ;;
;; Output: CF = 1 if error                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ReadSector:
        pusha

ReadSectorNext:
        mov     di, 15h                   ; attempts to read

ReadSectorRetry:
        pusha

        div     word [bpbSectorsPerTrack]
                ; ax = LBA / SPT
                ; dx = LBA % SPT         = sector - 1

        mov     cx, dx
        inc     cx
                ; cx = sector no.

        xor     dx, dx
        div     word [bpbHeadsPerCylinder]
                ; ax = (LBA / SPT) / HPC = cylinder
                ; dx = (LBA / SPT) % HPC = head

        mov     ch, al
                ; ch = LSB 0...7 of cylinder no.
        shl     ah, 6
        or      cl, ah
                ; cl = MSB 8...9 of cylinder no. + sector no.

        mov     dh, dl
                ; dh = head no.

        mov     dl, [bsDriveNumber]
                ; dl = drive no.

        mov     ax, 201h
                                        ; al = sector count
                                        ; ah = 2 = read function no.

        int     13h                     ; read sectors
        jnc     ReadSectorDone          ; CF = 0 if no error

        xor     ah, ah                  ; ah = 0 = reset function
        int     13h                     ; reset drive

        popa
        dec     di
        jnz     ReadSectorRetry         ; extra attempt
        jmp     short ErrRead

ReadSectorDone:
        popa
        dec     cx
        jz      ReadSectorDone2         ; last sector

        add     bx, [bpbBytesPerSector] ; adjust offset for next sector
        add     ax, 1
        adc     dx, 0                   ; adjust LBA for next sector
        jmp     short ReadSectorNext

ReadSectorDone2:
        popa
        ret

;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error Messaging Code ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;

ErrRead:
        mov     si, MsgErrRead
        jmp     short Error
ErrFind:
        mov     si, MsgErrFind
Error:
        mov     ah, 0Eh
        mov     bx, 7

        lodsb
        int     10h                     ; 1st char
        lodsb
        int     10h                     ; 2nd char

        jmp     short $                 ; hang

;;;;;;;;;;;;;;;;;;;;;;
;; String constants ;;
;;;;;;;;;;;;;;;;;;;;;;

MsgErrRead      db      "RE"
MsgErrFind      db      "NF"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Name of a program to be load and run ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ProgramName     db      "START13 BIN"   ; name and extension must be padded
                                        ; with spaces (11 bytes total)


              
