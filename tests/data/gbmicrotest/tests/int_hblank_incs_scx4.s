.include "header.inc"

main:
  lcd_off_unsafe
  ld a, 4
  ldh (SCX), a
  cycle_init
  set_stat_int_hblank
  set_ie_stat
  clear_if
  ei
  lcd_on

  xor a
  incs 1000
  di
  test_fail

.org STAT_INT_VECTOR
  test_finish_a 62
  //test_display_a