.syntax unified                             /* Use modern assembler syntax */
                                            /* Auto-generate IT instructions */
.thumb                                      /* Thumb/Thumb2 encoding */

/* Start of context switch handler*/
.text
.align 2

.global PendSV_Handler                      /* Define PendSv handler */
.thumb_func                                 /* Thumb mode function entry point */

PendSV_Handler:
  mrs r0, PSP                               /* Grab task's current stack pointer */
  stmdb r0!, {r4-r11}                       /* Write r4 - r11 into the task's current stack pointer */
  ldr r1, =current                          /* Grab the address of the "current" pointer */
  ldr r1, [r1]                              /* Load the old TCB */
  str r0, [r1]                              /* Store the current stack pointer into the tasks SP. Note: SP offset is 0 */
  push {r3, lr}                             /* Save LR */
  bl rr_select_next                         /* Calls rr_select_next to select the next TCB */
  pop {r3, lr}                              /* Restores LR */
  ldr r1, =current                          /* Grab the address of the "new" "current" pointer */
  ldr r1, [r1]                              /* Load the new TCB */
  ldr r1, [r1]                              /* Load the stack pointer value from where it left off. Note: SP offset is 0 */
  ldmia r1!, {r4-r11}                       /* Loads r4-r11 from lowest address upward. Replace r0 with highest address once done */
  msr PSP, r1                               /* Write SP with new task's stack */
  bx lr                                     /* Branch to where we left off */

.global SVC_Handler                         /* Define PendSv handler */
.thumb_func                                 /* Thumb mode function entry point */

SVC_Handler:
  ldr r1, =current                          /* Grab the address of the "current" pointer */
  ldr r1, [r1]                              /* Grab the current TCB */
  ldr r1, [r1]                              /* Grab the current TCB's SP. Note: SP offset is 0 */
  ldmia r1!, {r4-r11}                       /* Loads r4-r11 from lowest address upward. Replace r1 with highest address once done */
  msr PSP, r1                               /* Point PSP at hardware frame */
  ldr lr, =0xFFFFFFFD                       /* Return to Thread mode, using the PSP */
  bx lr                                     /* Load hardware frame */
