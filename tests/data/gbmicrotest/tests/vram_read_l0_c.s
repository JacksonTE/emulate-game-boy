; pass - dmg

.include "header.inc"

.define DELAY  60
.define ADDR   $9FFF
.define RESULT $FF

main:
  ld hl, ADDR

  ld a, $0
  ldh (LCDC), a

  ld a, $F0
  ld (hl), a

  ld a, $91
  ldh (LCDC), a

  nops DELAY
  ld a, (hl)

  test_finish_a RESULT