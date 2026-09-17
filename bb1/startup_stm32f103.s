.syntax unified
.cpu cortex-m3
.thumb

.global _estack
.global Reset_Handler

.extern main

.section .isr_vector, "a", %progbits

.word _estack
.word Reset_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler
.word Default_Handler

.section .text.Reset_Handler
.type Reset_Handler, %function

Reset_Handler:
    bl main

loop:
    b loop

.type Default_Handler, %function

Default_Handler:
    b Default_Handler
