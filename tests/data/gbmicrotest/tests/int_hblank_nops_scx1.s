.include "header.inc"

main:
  lcd_off_unsafe
  ld a, 1
  ldh (SCX), a
  cycle_init
  set_stat_int_hblank
  set_ie_stat
  clear_if
  lcd_on
  ei         // 30
  xor a      // 31
  nops 61    // 92    // int fires on 816 A

  // 93
  // 94
  // 95
  // 96
  // 97

  di
  test_fail

.org STAT_INT_VECTOR
  test_finish_cycle 98
