.include "header.inc"

.define DELAY  0
.define ADDR   $FF05
.define RESULT $00

main:
  nops DELAY
  ld a, (ADDR)
  test_finish_a RESULT
