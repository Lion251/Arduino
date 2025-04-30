.cpu cortex-m33
.syntax unified
.thumb

.global Dither2

#define NLeds r8
#define Val16Pt r9
#define Rem8Pt r10
#define DstPt r11
#define i lr
#define LED_SIZE 3

#define DITHER(x,subled) \
  ldrh    r##x, [Val16Pt,2*(x*LED_SIZE+subled)] ;\
  ldrb    r12, [Rem8Pt] ;\
  add     r##x, r12 ;\
  strb    r##x, [Rem8Pt,x*LED_SIZE+subled] ;\
  mov     r##x, r##x, lsl 16 ;\

#define DITHER_LIST(subled) \
  DITHER(0, subled) ;\
  DITHER(1, subled) ;\
  DITHER(2, subled) ;\
  DITHER(3, subled) ;\
  DITHER(4, subled) ;\
  DITHER(5, subled) ;\
  DITHER(6, subled) ;\
  DITHER(7, subled) ;\

#define SHIFT_IN(x) \
  adds    r##x, r##x ;\
  adc     r12,r12 ;\

#define UPD_PTRS \
  add     Val16Pt, 8*LED_SIZE*2 ;\
  add     Rem8Pt, 8*LED_SIZE ;\


Dither2:
  push  { r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  
  mov   NLeds, r0       // NLeds. Must be a multiple of 8
  mov   Val16Pt, r1     // Val16Pt
  mov   Rem8Pt, r2      // Rem8Pt
  mov   DstPt, r3       // DstPt

l0:
  DITHER(0,0)
  DITHER(1,0)
  DITHER(2,0)
  DITHER(3,0)
  DITHER(4,0)
  DITHER(5,0)
  DITHER(6,0)
  DITHER(7,0)

  mov   i, #8
l1:
  mov   r12, #0
  SHIFT_IN(0)
  SHIFT_IN(1)
  SHIFT_IN(2)
  SHIFT_IN(3)
  SHIFT_IN(4)
  SHIFT_IN(5)
  SHIFT_IN(6)
  SHIFT_IN(7)
  strb    r12, [DstPt], #1
  subs    i, #1
  bne     l1

  DITHER(0,1)
  DITHER(1,1)
  DITHER(2,1)
  DITHER(3,1)
  DITHER(4,1)
  DITHER(5,1)
  DITHER(6,1)
  DITHER(7,1)

  mov   i, #8
l2:
  mov   r12, #0
  SHIFT_IN(0)
  SHIFT_IN(1)
  SHIFT_IN(2)
  SHIFT_IN(3)
  SHIFT_IN(4)
  SHIFT_IN(5)
  SHIFT_IN(6)
  SHIFT_IN(7)
  strb    r12, [DstPt], #1
  subs    i, #1
  bne     l2

  DITHER(0,2)
  DITHER(1,2)
  DITHER(2,2)
  DITHER(3,2)
  DITHER(4,2)
  DITHER(5,2)
  DITHER(6,2)
  DITHER(7,2)

  mov   i, #8
l3:
  mov   r12, #0
  SHIFT_IN(0)
  SHIFT_IN(1)
  SHIFT_IN(2)
  SHIFT_IN(3)
  SHIFT_IN(4)
  SHIFT_IN(5)
  SHIFT_IN(6)
  SHIFT_IN(7)
  strb    r12, [DstPt], #1
  subs    i, #1
  bne     l3

  UPD_PTRS

  subs    NLeds, #1
  bne     l0


  pop   { r4,r5,r6,r7,r8,r9,r10,r11,r12,pc} // return

  .end