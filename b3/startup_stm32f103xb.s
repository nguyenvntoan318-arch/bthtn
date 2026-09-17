.syntax unified
  .cpu cortex-m3
  .thumb

.global Reset_Handler
.extern main

  .section .text.Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0
  ldr   r0, =main
  blx   r0
  bx    lr
.size Reset_Handler, .-Reset_Handler

  .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
g_pfnVectors:
  .word _estack
  .word Reset_Handler
  .size g_pfnVectors, .-g_pfnVectors
