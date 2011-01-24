;==================================================================
; Copyright ARM Ltd 2005. All rights reserved.
;
; Cortex-A8 Dhrystone example - Startup Code
;==================================================================

        PRESERVE8
        AREA   CORTEXA8, CODE, READONLY

        ENTRY

; Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_USR        EQU     0x10
Mode_FIQ        EQU     0x11
Mode_IRQ        EQU     0x12
Mode_SVC        EQU     0x13
Mode_ABT        EQU     0x17
Mode_UNDEF      EQU     0x1B
Mode_SYS        EQU     0x1F

I_Bit           EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40 ; when F bit is set, FIQ is disabled

;==================================================================
; Disable Cortex-A8 MMU if enabled
;==================================================================

        EXPORT Start

Start

        MRC     p15, 0, r0, c1, c0, 0       ; Read CP15 Control Register into r0
        TST     r0, #0x1                    ; Is the MMU enabled?
        BICNE   r0, r0, #0x1                ; Clear bit 0
        MCRNE   p15, 0, r0, c1, c0, 0       ; Write value back

;==================================================================
; Initialise Supervisor Mode Stack
; Note stack must be 8 byte aligned.
;==================================================================

        IMPORT  ||Image$$STACK$$ZI$$Limit|| ; Linker symbol from scatter file
        LDR     SP, =||Image$$STACK$$ZI$$Limit||

;==================================================================
; TLB maintenance, Invalidate Data and Instruction TLB's
;==================================================================

        MOV    r0,#0
        MCR    p15, 0, r0, c8, c7, 0 ; Cortex-A8 I-TLB and D-TLB invalidation

;==================================================================
; Cache Invalidation code for Cortex-A8
;==================================================================

        ; Invalidate L1 Instruction Cache

        MRC p15, 1, r0, c0, c0, 1   ; Read CLIDR
        TST r0, #0x3                ; Harvard Cache?
        MOV r0, #0
        MCRNE p15, 0, r0, c7, c5, 0 ; Invalidate Instruction Cache

        ; Invalidate Data/Unified Caches

        MRC p15, 1, r0, c0, c0, 1   ; Read CLIDR
        ANDS r3, r0, #&7000000
        MOV r3, r3, LSR #23         ; Total cache levels << 1
        BEQ Finished

        MOV r10, #0                 ; R10 holds current cache level << 1
Loop1   ADD r2, r10, r10, LSR #1    ; R2 holds cache "Set" position
        MOV r1, r0, LSR r2          ; Bottom 3 bits are the Cache-type for this level
        AND r1, R1, #7              ; Get those 3 bits alone
        CMP r1, #2
        BLT Skip                    ; No cache or only instruction cache at this level

        MCR p15, 2, r10, c0, c0, 0  ; Write the Cache Size selection register
        MOV r1, #0
        MCR p15, 0, r1, c7, c5, 4   ; PrefetchFlush to sync the change to the CacheSizeID reg
        MRC p15, 1, r1, c0, c0, 0   ; Reads current Cache Size ID register
        AND r2, r1, #&7             ; Extract the line length field
        ADD r2, r2, #4              ; Add 4 for the line length offset (log2 16 bytes)
        LDR r4, =0x3FF
        ANDS r4, r4, r1, LSR #3     ; R4 is the max number on the way size (right aligned)
        CLZ r5, r4                  ; R5 is the bit position of the way size increment
        LDR r7, =0x00007FFF
        ANDS r7, r7, r1, LSR #13    ; R7 is the max number of the index size (right aligned)

Loop2   MOV r9, r4                  ; R9 working copy of the max way size (right aligned)

Loop3   ORR r11, r10, r9, LSL r5    ; Factor in the Way number and cache number into R11
        ORR r11, r11, r7, LSL r2    ; Factor in the Set number
        MCR p15, 0, r11, c7, c14, 2 ; Clean and Invalidate by set/way
        SUBS r9, r9, #1             ; Decrement the Way number
        BGE Loop3
        SUBS r7, r7, #1             ; Decrement the Set number
        BGE Loop2
Skip    ADD r10, r10, #2            ; increment the cache number
        CMP r3, r10
        BGT Loop1

Finished


;===================================================================
; Cortex-A8 MMU Configuration
; Set translation table base
;===================================================================


        IMPORT ||Image$$TTB$$ZI$$Base||  ; from scatter file.;

        ; Cortex-A8 supports two translation tables
        ; Configure translation table base (TTB) control register cp15,c2
        ; to a value of all zeros, indicates we are using TTB register 0.

        MOV     r0,#0x0
        MCR     p15, 0, r0, c2, c0, 2

        ; write the address of our page table base to TTB register 0.;
        ; We are setting to outer-noncachable [4:3] is zero

        LDR     r0,=||Image$$TTB$$ZI$$Base||
        MCR     p15, 0, r0, c2, c0, 0


;===================================================================
; Cortex-A8 PAGE TABLE generation, using standard Arch v6 tables
;
; AP[11:10]   - Access Permissions = b11, Read/Write Access
; Domain[8:5] - Domain = b1111, Domain 15
; Type[1:0]   - Descriptor Type = b10, 1Mb descriptors
;
; TEX  C  B
; 000  0  0  Strongly Ordered
; 001  1  1  Outer and inner write back, write allocate Normal
;===================================================================

        LDR     r1,=0xfff                   ; loop counter
        LDR     r2,=2_00000000000000000000110111100010

        ; r0 contains the address of the translation table base
        ; r1 is loop counter
        ; r2 is level1 descriptor (bits 19:0)

        ; use loop counter to create 4096 individual table entries
        ; this writes from address 0x7FFC down to 0x4000 in word steps (4bytes).

init_ttb_1

        ORR     r3, r2, r1, LSL#20          ; r3 now contains full level1 descriptor to write
        STR     r3, [r0, r1, LSL#2]         ; str table entry at TTB base + loopcount*4
        SUBS    r1, r1, #1                  ; decrement loop counter
        BPL     init_ttb_1

        ; In this example we will change the cacheable attribute in the first descriptor.
        ; Virtual memory from 0 to 1MB will be cacheable (write back mode).
        ; TEX[14:12]=001 and CB[3:2]= 11, Outer and inner write back, write allocate.

        ORR     r3,r3,#2_0000000001100      ; Set CB bits
        ORR     r3,r3,#2_1000000000000      ; Set TEX bits
        STR     r3,[r0]

	ADD r2, r3, #0x100000               ; alter r3 to have correct base address for second descriptor (flat mapping)
	STR r2, [r0, #4]                    ; store the new descriptor at r0 + 4 (overwrite second section descriptor)

	ADD r2, r3, #0x200000               ; alter r3 to have correct base address for 3 descriptor (flat mapping)
	STR r2, [r0, #8]                    ; store the new descriptor at r0 + 4 (overwrite second section descriptor)

	ADD r2, r3, #0x300000               ; alter r3 to have correct base address for 4 descriptor (flat mapping)
	STR r2, [r0, #0xc]                    ; store the new descriptor at r0 + 4 (overwrite second section descriptor)

	ADD r2, r3, #0x400000               ; alter r3 to have correct base address for 5 descriptor (flat mapping)
	STR r2, [r0, #0x10]                    ; store the new descriptor at r0 + 4 (overwrite second section descriptor)

	ADD r2, r3, #0x500000               ; alter r3 to have correct base address for 6 descriptor (flat mapping)
	STR r2, [r0, #0x14]                    ; store the new descriptor at r0 + 4 (overwrite second section descriptor)

	ADD r2, r3, #0x600000               ; alter r3 to have correct base address for 7 descriptor (flat mapping)
	STR r2, [r0, #0x18]                    ; store the new descriptor at r0 + 4 (overwrite second section descriptor)

	ADD r2, r3, #0x700000               ; alter r3 to have correct base address for 8 descriptor (flat mapping)
	STR r2, [r0, #0x1c]                    ; store the new descriptor at r0 + 4 (overwrite second section descriptor)

	ADD r2, r3, #0x800000               ; alter r3 to have correct base address for 9 descriptor (flat mapping)
	STR r2, [r0, #0x20]                    ; store the new descriptor at r0 + 4 (overwrite second section descriptor)

	ADD r2, r3, #0x900000               ; alter r3 to have correct base address for 10 descriptor (flat mapping)
	STR r2, [r0, #0x24]                    ; store the new descriptor at r0 + 4 (overwrite second section descriptor)

	ADD r2, r3, #0xa00000               ; alter r3 to have correct base address for 11 descriptor (flat mapping)
	STR r2, [r0, #0x28]                    ; store the new descriptor at r0 + 4 (overwrite second section descriptor)

	ADD r2, r3, #0xb00000               ; alter r3 to have correct base address for 12 descriptor (flat mapping)
	STR r2, [r0, #0x2c]                    ; store the new descriptor at r0 + 4 (overwrite second section descriptor)

	ADD r2, r3, #0xc00000               ; alter r3 to have correct base address for 13 descriptor (flat mapping)
	STR r2, [r0, #0x30]                    ; store the new descriptor at r0 + 4 (overwrite second section descriptor)

;===================================================================
; Setup domain control register - Enable all domains to client mode
;===================================================================

        MRC     p15, 0, r0, c3, c0, 0     ; Read Domain Access Control Register
        LDR     r0, =0x55555555           ; Initialize every domain entry to b01 (client)
        MCR     p15, 0, r0, c3, c0, 0     ; Write Domain Access Control Register

;===================================================================
; Setup L2 Cache - L2 Cache Auxiliary Control
;===================================================================

        MOV     r0, #0
        ;MCR     p15, 1, r0, c9, c0, 2      ; Write L2 Auxilary Control Register

;==================================================================
; Enable access to NEON/VFP by enabling access to Coprocessors 10 and 11.
; Enables Full Access i.e. in both priv and non priv modes
;==================================================================

        MRC     p15, 0, r0, c1, c0, 2      ; read CP access register
        ORR     r0, r0, #(0x3  <<20)       ; enable access CP 10
        ORR     r0, r0, #(0x3  <<22)       ; enable access CP 11
        MCR     p15, 0, r0, c1, c0, 2      ; write CP access register back

;==================================================================
; Switch on the VFP and Neon Hardware
;=================================================================

        MOV     r0, #0                      ; Set up a register
        ORR     r0, r0, #(0x1 << 30)
        FMXR    FPEXC, r0                   ; Write FPEXC register, EN bit set.

;===================================================================
; Enable MMU and Branch to __main
;===================================================================

        IMPORT  __main                      ; before MMU enabled import label to __main
        LDR     r12,=__main                 ; save this in register for possible long jump


        MRC     p15, 0, r0, c1, c0, 0       ; read CP15 register 1 into r0
        ORR     r0, r0, #0x1                ; enable MMU before scatter loading
        MCR     p15, 0, r0, c1, c0, 0       ; write CP15 register 1


; Now the MMU is enabled, virtual to physical address translations will occur.
; This will affect the next instruction fetches.
;
; The two instructions currently in the ARM pipeline will have been fetched
; before the MMU was enabled. This property is useful because the next two
; instructions are safe even if new instruction fetches fail. If this routine
; was mapped out of the new virtual memory map, the branch to __main would
; still succeed.

        BX      r12                 ; branch to __main  C library entry point

        END                         ; mark the end of this file

