#include <LiquidCrystal.h>

#include "Led20_defs.h" // must be the first line
#include "Led20_prog.h" // must be the last line
//#include "Streaming.h"  // Needed for serial debugging on Arduino

Sub(SetColorsDefault) {
  SetColorVal(R, 20),
  SetColorVal(Y, 20),
  SetColorVal(G, 20),
  SetColorVal(B, 20),
  SetColorVal(V, 20),
  End
};

Sub(Spiraal) {
  R1,Y1,G1,B1,V1,R2,Y2,G2,B2,V2,R3,Y3,G3,B3,V3,R4,Y4,G4,B4,V4,End
};

Sub(SpiraalReverse) {
  V4,B4,G4,Y4,R4,V3,B3,G3,Y3,R3,V2,B2,G2,Y2,R2,V1,B1,G1,Y1,R1,End
};


Sub(SpiraalChase) {
  R14,Y14,G14,B14,V14,R12,Y12,G12,B12,V12,R23,Y23,G23,B23,V23,R34,Y34,G34,B34,V34,End
};

Sub(SpiraalChaseReverse) {
  V34,B34,G34,Y34,R34,V23,B23,G23,Y23,R23,V12,B12,G12,Y12,R12,V14,B14,G14,Y14,R14,End
};


Sub(Spiraal2) {
  R13,Y13,G13,B13,V13,R24,Y24,G24,B24,V24,End
};

Sub(Spiraal2Reverse) {
  V24,B24,G24,Y24,R24,V13,B13,G13,Y13,R13,End
};

Sub(Spiraal3) {
  R134,Y134,G134,B134,V134,R124,Y124,G124,B124,V124,R123,Y123,G123,B123,V123,R234,Y234,G234,B234,V234,End
};

Sub(Spiraal3Reverse) {
  V234,B234,G234,Y234,R234,V123,B123,G123,Y123,R123,V124,B124,G124,Y124,R124,V134,B134,G134,Y134,R134,End
};


Sub(Spiraal4) {
  R1234,Y1234,G1234,B1234,V1234,End
};

Sub(Spiraal4Reverse) {
  V1234,B1234,G1234,Y1234,R1234,End
};


Sub(Stars) {
  Y4,R2,R2,G3,Y2,G3,V3,Y4,Y1,Y1,B4,V4,R3,B3,R4,
  G2,B3,G2,Y2,G4,B2,V2,G1,V3,R4,Y2,R4,B1,R3,Y1,
  R2,G1,R1,V3,G2,R1,R3,G3,V3,R4,B4,B1,V1,Y4,V3,
  R1,B1,G4,Y3,V3,R4,B3,Y4,G2,G2,Y1,R2,Y3,V2,V1,
  V1,G4,Y2,R2,G2,Y4,B2,R3,B3,V4,Y1,V1,V4,B3,R1,
  V1,B1,R3,Y3,R2,B4,B1,R2,R2,V3,V1,Y4,Y4,R4,Y4,
  Y1,B2,G4,Y3,R3,G2,B4,B3,Y4,V1,
  End
};


Sub(Chase) {
  R1,R2,R3,R4,Y1,Y2,Y3,Y4,G1,G2,G3,G4,B1,B2,B3,B4,V1,V2,V3,V4,
  End
};


Sub(Chase2) {
  R1,G1,R2,G2,R3,G3,R4,G4, Y1,B1,Y2,B2,Y3,B3,Y4,B4, G1,V1,G2,V2,G3,V3,G4,V4,
  B1,R1,B2,R2,B3,R3,B4,R4, V1,Y1,V2,Y2,V3,Y3,V4,Y4,
  End
};

Sub(Chase3) {
  R1,R1,R2,R2,R3,R3,R4,R4, Y1,Y1,Y2,Y2,Y3,Y3,Y4,Y4, G1,G1,G2,G2,G3,G3,G4,G4, B1,B1,B2,B2,B3,B3,B4,B4, V1,V1,V2,V2,V3,V3,V4,V4,
  R1,R2,R2,R3,R3,R4,R4,Y1, Y1,Y2,Y2,Y3,Y3,Y4,Y4,G1, G1,G2,G2,G3,G3,G4,G4,B1, B1,B2,B2,B3,B3,B4,B4,V1, V1,V2,V2,V3,V3,V4,V4,R1,
  R1,R3,R2,R4,R3,Y1,R4,Y2, Y1,Y3,Y2,Y4,Y3,G1,Y4,G2, G1,G3,G2,G4,G3,B1,G4,B2, B1,B3,B2,B4,B3,V1,B4,V2, V1,V3,V2,V4,V3,R1,V4,R2,
  R1,R4,R2,Y1,R3,Y2,R4,Y3, Y1,Y4,Y2,G1,Y3,G2,Y4,G3, G1,G4,G2,B1,G3,B2,G4,B3, B1,B4,B2,V1,B3,V2,B4,V3, V1,V4,V2,R1,V3,R2,V4,R3,

  R1,Y1,R2,Y2,R3,Y3,R4,Y4, Y1,G1,Y2,G2,Y3,G3,Y4,G4, G1,B1,G2,B2,G3,B3,G4,B4, B1,V1,B2,V2,B3,V3,B4,V4, V1,R1,V2,R2,V3,R3,V4,R4,
  R1,Y2,R2,Y3,R3,Y4,R4,G1, Y1,G2,Y2,G3,Y3,G4,Y4,B1, G1,B2,G2,B3,G3,B4,G4,V1, B1,V2,B2,V3,B3,V4,B4,R1, V1,R2,V2,R3,V3,R4,V4,Y1,
  R1,Y3,R2,Y4,R3,G1,R4,G2, Y1,G3,Y2,G4,Y3,B1,Y4,B2, G1,B3,G2,B4,G3,V1,G4,V2, B1,V3,B2,V4,B3,R1,B4,R2, V1,R3,V2,R4,V3,Y1,V4,Y2,
  R1,Y4,R2,G1,R3,G2,R4,G3, Y1,G4,Y2,B1,Y3,B2,Y4,B3, G1,B4,G2,V1,G3,V2,G4,V3, B1,V4,B2,R1,B3,R2,B4,R3, V1,R4,V2,Y1,V3,Y2,V4,Y3,

  R1,G1,R2,G2,R3,G3,R4,G4, Y1,B1,Y2,B2,Y3,B3,Y4,B4, G1,V1,G2,V2,G3,V3,G4,V4, B1,R1,B2,R2,B3,R3,B4,R4, V1,Y1,V2,Y2,V3,Y3,V4,Y4,
  R1,G2,R2,G3,R3,G4,R4,B1, Y1,B2,Y2,B3,Y3,B4,Y4,V1, G1,V2,G2,V3,G3,V4,G4,R1, B1,R2,B2,R3,B3,R4,B4,Y1, V1,Y2,V2,Y3,V3,Y4,V4,G1,
  R1,G3,R2,G4,R3,B1,R4,B2, Y1,B3,Y2,B4,Y3,V1,Y4,V2, G1,V3,G2,V4,G3,R1,G4,R2, B1,R3,B2,R4,B3,Y1,B4,Y2, V1,Y3,V2,Y4,V3,G1,V4,G2,
  R1,G4,R2,B1,R3,B2,R4,B3, Y1,B4,Y2,V1,Y3,V2,Y4,V3, G1,V4,G2,R1,G3,R2,G4,R3, B1,R4,B2,Y1,B3,Y2,B4,Y3, V1,Y4,V2,G1,V3,G2,V4,G3,

  R1,B1,R2,B2,R3,B3,R4,B4, Y1,V1,Y2,V2,Y3,V3,Y4,V4, G1,R1,G2,R2,G3,R3,G4,R4, B1,Y1,B2,Y2,B3,Y3,B4,Y4, V1,G1,V2,G2,V3,G3,V4,G4,
  R1,B2,R2,B3,R3,B4,R4,V1, Y1,V2,Y2,V3,Y3,V4,Y4,R1, G1,R2,G2,R3,G3,R4,G4,Y1, B1,Y2,B2,Y3,B3,Y4,B4,G1, V1,G2,V2,G3,V3,G4,V4,B1,
  R1,B3,R2,B4,R3,V1,R4,V2, Y1,V3,Y2,V4,Y3,R1,Y4,R2, G1,R3,G2,R4,G3,Y1,G4,Y2, B1,Y3,B2,Y4,B3,G1,B4,G2, V1,G3,V2,G4,V3,B1,V4,B2,
  R1,B4,R2,V1,R3,V2,R4,V3, Y1,V4,Y2,R1,Y3,R2,Y4,R3, G1,R4,G2,Y1,G3,Y2,G4,Y3, B1,Y4,B2,G1,B3,G2,B4,G3, V1,G4,V2,B1,V3,B2,V4,B3,

  R1,V1,R2,V2,R3,V3,R4,V4, Y1,R1,Y2,R2,Y3,R3,Y4,R4, G1,Y1,G2,Y2,G3,Y3,G4,Y4, B1,G1,B2,G2,B3,G3,B4,G4, V1,B1,V2,B2,V3,B3,V4,B4,
  R1,V2,R2,V3,R3,V4,R4,R1, Y1,R2,Y2,R3,Y3,R4,Y4,Y1, G1,Y2,G2,Y3,G3,Y4,G4,G1, B1,G2,B2,G3,B3,G4,B4,B1, V1,B2,V2,B3,V3,B4,V4,V1,
  R1,V3,R2,V4,R3,R1,R4,R2, Y1,R3,Y2,R4,Y3,Y1,Y4,Y2, G1,Y3,G2,Y4,G3,G1,G4,G2, B1,G3,B2,G4,B3,B1,B4,B2, V1,B3,V2,B4,V3,V1,V4,V2,
  R1,V4,R2,R1,R3,R2,R4,R3, Y1,R4,Y2,Y1,Y3,Y2,Y4,Y3, G1,Y4,G2,G1,G3,G2,G4,G3, B1,G4,B2,Y1,B3,B2,B4,B3, V1,B4,V2,V1,V3,V2,V4,V3,

  End
};


Sub(TegenIn) {
  R1,R1,R2,R1, R3,V4,R4,V4, Y1,V3,Y2,V3, Y3,V2,Y4,V2, G1,V1,G2,V1, G3,B4,G4,B4, B1,B3,B2,B3, B3,B2,B4,B2, 
  V1,B1,V2,B1, V3,G4,V4,G4, R1,G3,R2,G3, R3,G2,R4,G2, Y1,G1,Y2,G1, Y3,Y4,Y4,Y4, G1,Y3,G2,Y3, G3,Y2,G4,Y2, 
  B1,Y1,B2,Y1, B3,R4,B4,R4, V1,R3,V2,R3, V3,R2,V4,R2,

  End
};


Sub(Main) {
  FadeOff,
  Call(SetColorsDefault),
  SetSpeed(5),
  FadeOn,
/*
  Repeat(10),
    Repeat(2),
      Repeat(10),
        Call(TegenIn),
      EndRep,
      Call(Chase3),
      FadeUp,
      SetSpeed(UP),

    Repeat(2),
      Repeat(10),
        Call(TegenIn),
      EndRep,
      Call(Chase3),
      FadeDown,
      SetSpeed(UP),

    EndRep,

  EndRep,

  Repeat(10),
    Repeat(2),
      Repeat(10),
        Call(TegenIn),
      EndRep,
      Call(Chase3),
      FadeUp,
      SetSpeed(DOWN),

    Repeat(2),
      Repeat(10),
        Call(TegenIn),
      EndRep,
      Call(Chase3),
      FadeDown,
      SetSpeed(DOWN),

    EndRep,

  EndRep,

  End
*/
// Spiraal

  Repeat(18),
    Repeat(3),
      Call(Spiraal),
    EndRep,
    SetSpeed(UP),
  EndRep,
  
  Repeat(8),
    Call(Stars),
    SetSpeed(DOWN),
  EndRep,
  Repeat(8),
    Call(Stars),
    SetSpeed(UP),
  EndRep,
  
  Repeat(18),
    Repeat(3),
      Call(SpiraalReverse),
    EndRep,
    SetSpeed(DOWN),
  EndRep,
  
// SpiraalChaseReverse    
  Repeat(14),
    Repeat(3),
      Call(SpiraalChaseReverse),
    EndRep,
    SetSpeed(UP),
  EndRep,
  
  Repeat(8),
    Call(Stars),
    SetSpeed(DOWN),
  EndRep,
  Repeat(8),
    Call(Stars),
    SetSpeed(UP),
  EndRep,
  
  Repeat(14),
    Repeat(3),
      Call(SpiraalChase),
    EndRep,
    SetSpeed(DOWN),
  EndRep,
  
// Spiraal2    
  Repeat(14),
    Repeat(6),
      Call(Spiraal2),
    EndRep,
    SetSpeed(UP),
  EndRep,
  
  Repeat(8),
    Call(Stars),
    SetSpeed(DOWN),
  EndRep,
  Repeat(8),
    Call(Stars),
    SetSpeed(UP),
  EndRep,
  
  Repeat(14),
    Repeat(6),
      Call(Spiraal2Reverse),
    EndRep,
    SetSpeed(DOWN),
  EndRep,
  
// Spiraal3Reverse    
  Repeat(14),
    Repeat(3),
      Call(Spiraal3Reverse),
    EndRep,
    SetSpeed(UP),
  EndRep,
  
  Repeat(8),
    Call(Stars),
    SetSpeed(DOWN),
  EndRep,
  Repeat(8),
    Call(Stars),
    SetSpeed(UP),
  EndRep,
  
  Repeat(14),
    Repeat(3),
      Call(Spiraal3),
    EndRep,
    SetSpeed(DOWN),
  EndRep,
  
// Spiraal4    
  Repeat(14),
    Repeat(12),
      Call(Spiraal4),
    EndRep,
    SetSpeed(UP),
  EndRep,
  
  Repeat(8),
    Call(Stars),
    SetSpeed(DOWN),
  EndRep,
  Repeat(8),
    Call(Stars),
    SetSpeed(UP),
  EndRep,
  
  Repeat(14),
    Repeat(12),
      Call(Spiraal4Reverse),
    EndRep,
    SetSpeed(DOWN),
  EndRep,
  
// Fading stars    
  FadeOn,
  SetSpeed(4),
  Repeat(12),
      Call(Stars),
      SetSpeed(UP),
  EndRep,
  
  End

};
