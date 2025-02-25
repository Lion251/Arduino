#include <xtensa/coreasm.h>
#include <xtensa/corebits.h>
#include <xtensa/config/system.h>
#include "freertos/xtensa_context.h"
#include "esp_private/panic_reason.h"
#include "sdkconfig.h"
#include "soc/soc.h"
#include "soc/gpio_reg.h"
#include "soc/dport_reg.h"

#define GPIO_BASE GPIO_OUT_REG
#define GPIO_STATUS_W1TC_OFFS (GPIO_STATUS_W1TC_REG - GPIO_BASE)
#define GPIO_OUT_W1TS_OFFS    (GPIO_OUT_W1TS_REG - GPIO_BASE)
#define GPIO_OUT_W1TC_OFFS    (GPIO_OUT_W1TC_REG - GPIO_BASE)

#define LX_INTR_STACK_SIZE  12
#define LX_INTR_A1_OFFSET 0
#define LX_INTR_A2_OFFSET 4
#define LX_INTR_A3_OFFSET 8
    .data
_lx_intr_stackxxx:
    .space      LX_INTR_STACK_SIZE

    .section .iram1,"ax"
    .global     xt_highint4
    .type       xt_highint4,@function
    .align      4
xt_highint4xxx:

    movi    a0, _lx_intr_stack // assuming a0 saved restored by EXCSAVE_5 at the end of this
    s32i    a1, a0, LX_INTR_A1_OFFSET
    s32i    a2, a0, LX_INTR_A2_OFFSET
    s32i    a3, a0, LX_INTR_A3_OFFSET

    /* Clear interupt on GPIO26 */
    movi    a0, GPIO_BASE
    movi    a1, (1 << 17)
    s32i    a1, a0, GPIO_STATUS_W1TC_OFFS
    memw

    // toggle output reg
    movi    a1, (1 << 18)
    s32i    a1, a0, GPIO_OUT_W1TS_OFFS
    s32i    a1, a0, GPIO_OUT_W1TC_OFFS

    /* Increment intr_cnt */
    movi    a1, intCounter
    l32i    a2, a1, 0
    addi    a2, a2, 1
    s32i    a2, a1, 0
    memw



    l32i    a3, a0, LX_INTR_A3_OFFSET
    l32i    a2, a0, LX_INTR_A2_OFFSET
    l32i    a1, a0, LX_INTR_A1_OFFSET

    rsr     a0, EXCSAVE_4 // restore a0
    rfi     4

    .global ld_include_xt_highintxxx
ld_include_xt_highintxxx:
