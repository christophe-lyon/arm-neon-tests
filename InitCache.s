; Copyright ARM Ltd 2005. All rights reserved.

;==================================================================
; This code provides basic global enable for a Cortex-A8 cache
; and program flow prediction
; This code must be run from a privileged mode
;==================================================================

        AREA   CORTEXA8CACHE, CODE, READONLY
        EXPORT core_init

core_init

;==================================================================
; Global Enable for Cortex-A8 Instruction and Data Caching
;==================================================================

        MRC     p15, 0, r0, c1, c0, 0       ; read CP15 register 1 into r0
        ORR     r0, r0, #(0x1  <<12)        ; enable I Cache
	;BIC   r0, r0, #(0x1  <<12)         ; Clear bit 0
        ORR     r0, r0, #(0x1  <<2)         ; enable D Cache
	;BIC   r0, r0, #(0x1  << 2)         ; Clear bit 0
	ORR     r0, r0, #0x1                ; enable MMU
        MCR     p15, 0, r0, c1, c0, 0       ; write CP15 register 1

;==================================================================
; Enable Cortex-A8 Level2 Unified Cache
;==================================================================

        MRC p15, 0, r0, c1, c0, 1           ; Read Auxiliary Control Register
        ORR r0, r0, #2                      ; L2EN bit, enable L2 cache
	;BIC   r0, r0, #(0x1  << 1)         ; L2EN bit, disable L2 cache
	;ORR     r0, r0, #(0x1  << 4)        ;Enables speculative accesses on AXI
	ORR     r0, r0, #(0x1  << 4)        ;Enables speculative accesses on AXI
	ORR     r0, r0, #(0x1  << 5)        ;Enables caching NEON data within the L1 data cache
        MCR p15, 0, r0, c1, c0, 1           ; Write Auxiliary Control Register

;==================================================================
; Cortex-A8 program flow prediction
;==================================================================

        MRC     p15, 0, r0, c1, c0, 0       ; read CP15 register 1 into r0
        ORR     r0, r0, #(0x1  <<11)        ; Enable all forms of branch prediction
	;BIC   	r0, r0, #(0x1  << 11)	    ; Disable all forms of branch prediction
        MCR     p15, 0, r0, c1, c0, 0       ; write CP15 register 1

;==================================================================

        BX    lr

        END                                 ; mark the end of this file

