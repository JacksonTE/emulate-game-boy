.include "header.inc"

main:
  cycle_init

  ld a, %00001000
  ldh (STAT), a
  ld a, %00000010
  ldh (IE), a
  ld a, %00000000
  ldh (IF), a

  //cycle_count
  //test_display_a
  // 0b00011000 24

  // IF is _not_ set, so we _do_ halt.

  ld a, 0
  halt          // halt takes two cycles
                // but it's only one byte so we read the next op here
  inc a         // inc takes one cycle

  test_finish_a 1

.org STAT_INT_VECTOR
  test_fail
