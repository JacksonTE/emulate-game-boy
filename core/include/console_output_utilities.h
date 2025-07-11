#pragma once

#include <functional>
#include <iomanip>
#include <iostream>
#include <string_view>

#include "bitwise_utilities.h"
#include "register_file.h"

namespace GameBoyCore
{

inline void print_bytes_in_range(std::function<uint8_t(uint16_t, bool)> read_byte, uint16_t start_address, uint16_t end_address)
{
    std::cout << std::hex << std::setfill('0');
    std::cout << "=========== Memory Range 0x" << std::setw(4) << start_address << " - 0x" << std::setw(4) << end_address << " ============\n";

    for (uint32_t address = start_address; address <= end_address; address++)
    {
        const uint16_t remainder = address % 0x10;

        if (address == start_address || remainder == 0)
        {
            const uint16_t line_offset = address - remainder;
            std::cout << std::setw(4) << line_offset << ": ";

            for (uint16_t i = 0; i < remainder; i++)
                std::cout << "   ";
        }
        std::cout << std::setw(2) << static_cast<int>(read_byte(address, false)) << " ";

        if ((address + 1) % 0x10 == 0)
            std::cout << "\n";
    }

    if ((end_address + 1) % 0x10 != 0)
        std::cout << "\n";

    std::cout << "=====================================================\n";
}

inline void print_register_file_state(RegisterFile<std::endian::native> register_file)
{
    std::cout << "=================== CPU Registers ===================\n";
    std::cout << std::hex << std::setfill('0');

    std::cout << "AF: 0x" << std::setw(4) << register_file.af << "   "
              << "(A: 0x" << std::setw(2) << static_cast<int>(register_file.a) << ","
              << " F: 0x" << std::setw(2) << static_cast<int>(register_file.flags) << ")   "
              << "Flags ZNHC: " << (is_flag_set(register_file.flags, FLAG_ZERO_MASK) ? "1" : "0")
              << (is_flag_set(register_file.flags, FLAG_SUBTRACT_MASK) ? "1" : "0")
              << (is_flag_set(register_file.flags, FLAG_HALF_CARRY_MASK) ? "1" : "0")
              << (is_flag_set(register_file.flags, FLAG_CARRY_MASK) ? "1" : "0") << "\n";

    std::cout << "BC: 0x" << std::setw(4) << register_file.bc << "   "
              << "(B: 0x" << std::setw(2) << static_cast<int>(register_file.b) << ","
              << " C: 0x" << std::setw(2) << static_cast<int>(register_file.c) << ")\n";

    std::cout << "DE: 0x" << std::setw(4) << register_file.de << "   "
              << "(D: 0x" << std::setw(2) << static_cast<int>(register_file.d) << ","
              << " E: 0x" << std::setw(2) << static_cast<int>(register_file.e) << ")\n";

    std::cout << "HL: 0x" << std::setw(4) << register_file.hl << "   "
              << "(H: 0x" << std::setw(2) << static_cast<int>(register_file.h) << ","
              << " L: 0x" << std::setw(2) << static_cast<int>(register_file.l) << ")\n";

    std::cout << "Stack Pointer: 0x" << std::setw(4) << register_file.stack_pointer << "\n";
    std::cout << "Program Counter: 0x" << std::setw(4) << register_file.program_counter << "\n";
    std::cout << "=====================================================\n";
}

inline bool set_error_message_and_fail(std::string_view error_message_text, std::string &output_error_message)
{
    std::cerr << std::string("Error: ") << error_message_text << "\n";
    output_error_message = error_message_text;
    return false;
}

static const char *central_processing_unit_instruction_mnemonics[256] =
{
    "00: NOP",
    "01: LD BC, u16",
    "02: LD (BC), A",
    "03: INC BC",
    "04: INC B",
    "05: DEC B",
    "06: LD B,u8",
    "07: RLCA",
    "08: LD (u16), SP",
    "09: ADD HL, BC",
    "0a: LD A, (BC)",
    "0b: DEC BC",
    "0c: INC C",
    "0d: DEC C",
    "0e: LD C, u8",
    "0f: RRCA",
    "10: STOP",
    "11: LD DE, u16",
    "12: LD (DE), A",
    "13: INC DE",
    "14: INC D",
    "15: DEC D",
    "16: LD D, u8",
    "17: RLA",
    "18: JR i8",
    "19: ADD HL, DE",
    "1a: LD A, (DE)",
    "1b: DEC DE",
    "1c: INC E",
    "1d: DEC E",
    "1e: LD E, u8",
    "1f: RRA",
    "20: JR NZ, i8",
    "21: LD HL, u16",
    "22: LDI (HL), A",
    "23: INC HL",
    "24: INC H",
    "25: DEC H",
    "26: LD H, u8",
    "27: DAA",
    "28: JR Z, i8",
    "29: ADD HL, HL",
    "2a: LDI A, (HL)",
    "2b: DEC HL",
    "2c: INC L",
    "2d: DEC L",
    "2e: LD L, u8",
    "2f: CPL",
    "30: JR NC, i8",
    "31: LD SP, u16",
    "32: LDD (HL), A",
    "33: INC SP",
    "34: INC (HL)",
    "35: DEC (HL)",
    "36: LD (HL), u8",
    "37: SCF",
    "38: JR C, i8",
    "39: ADD HL, SP",
    "3a: LDD A, (HL)",
    "3b: DEC SP",
    "3c: INC A",
    "3d: DEC A",
    "3e: LD A, u8",
    "3f: CCF",
    "40: LD B, B",
    "41: LD B, C",
    "42: LD B, D",
    "43: LD B, E",
    "44: LD B, H",
    "45: LD B, L",
    "46: LD B, (HL)",
    "47: LD B, A",
    "48: LD C, B",
    "49: LD C, C",
    "4a: LD C, D",
    "4b: LD C, E",
    "4c: LD C, H",
    "4d: LD C, L",
    "4e: LD C, (HL)",
    "4f: LD C, A",
    "50: LD D, B",
    "51: LD D, C",
    "52: LD D, D",
    "53: LD D, E",
    "54: LD D, H",
    "55: LD D, L",
    "56: LD D, (HL)",
    "57: LD D, A",
    "58: LD E, B",
    "59: LD E, C",
    "5a: LD E, D",
    "5b: LD E, E",
    "5c: LD E, H",
    "5d: LD E, L",
    "5e: LD E, (HL)",
    "5f: LD E, A",
    "60: LD H, B",
    "61: LD H, C",
    "62: LD H, D",
    "63: LD H, E",
    "64: LD H, H",
    "65: LD H, L",
    "66: LD H, (HL)",
    "67: LD H, A",
    "68: LD L, B",
    "69: LD L, C",
    "6a: LD L, D",
    "6b: LD L, E",
    "6c: LD L, H",
    "6d: LD L, L",
    "6e: LD L, (HL)",
    "6f: LD L, A",
    "70: LD (HL), B",
    "71: LD (HL), C",
    "72: LD (HL), D",
    "73: LD (HL), E",
    "74: LD (HL), H",
    "75: LD (HL), L",
    "76: HALT",
    "77: LD (HL), A",
    "78: LD A, B",
    "79: LD A, C",
    "7a: LD A, D",
    "7b: LD A, E",
    "7c: LD A, H",
    "7d: LD A, L",
    "7e: LD A, (HL)",
    "7f: LD A, A",
    "80: ADD A, B",
    "81: ADD A, C",
    "82: ADD A, D",
    "83: ADD A, E",
    "84: ADD A, H",
    "85: ADD A, L",
    "86: ADD A, (HL)",
    "87: ADD A",
    "88: ADC B",
    "89: ADC C",
    "8a: ADC D",
    "8b: ADC E",
    "8c: ADC H",
    "8d: ADC L",
    "8e: ADC (HL)",
    "8f: ADC A",
    "90: SUB B",
    "91: SUB C",
    "92: SUB D",
    "93: SUB E",
    "94: SUB H",
    "95: SUB L",
    "96: SUB (HL)",
    "97: SUB A",
    "98: SBC B",
    "99: SBC C",
    "9a: SBC D",
    "9b: SBC E",
    "9c: SBC H",
    "9d: SBC L",
    "9e: SBC (HL)",
    "9f: SBC A",
    "a0: AND B",
    "a1: AND C",
    "a2: AND D",
    "a3: AND E",
    "a4: AND H",
    "a5: AND L",
    "a6: AND (HL)",
    "a7: AND A",
    "a8: XOR B",
    "a9: XOR C",
    "aa: XOR D",
    "ab: XOR E",
    "ac: XOR H",
    "ad: XOR L",
    "ae: XOR (HL)",
    "af: XOR A",
    "b0: OR B",
    "b1: OR C",
    "b2: OR D",
    "b3: OR E",
    "b4: OR H",
    "b5: OR L",
    "b6: OR (HL)",
    "b7: OR A",
    "b8: CP B",
    "b9: CP C",
    "ba: CP D",
    "bb: CP E",
    "bc: CP H",
    "bd: CP L",
    "be: CP (HL)",
    "bf: CP A",
    "c0: RET NZ",
    "c1: POP BC",
    "c2: JP NZ, u16",
    "c3: JP u16",
    "c4: CALL NZ, u16",
    "c5: PUSH BC",
    "c6: ADD A, u8",
    "c7: RST 0x00",
    "c8: RET Z",
    "c9: RET",
    "ca: JP Z, u16",
    "cb: CB %02X",
    "cc: CALL Z, u16",
    "cd: CALL u16",
    "ce: ADC u16",
    "cf: RST 0x08",
    "d0: RET NC",
    "d1: POP DE",
    "d2: JP NC, u16",
    "d3: UNUSED",
    "d4: CALL NC, u16",
    "d5: PUSH DE",
    "d6: SUB u8",
    "d7: RST 0x10",
    "d8: RET C",
    "d9: RETI",
    "da: JP C, u16",
    "db: UNUSED",
    "dc: CALL C, u16",
    "dd: UNUSED",
    "de: SBC u8",
    "df: RST 0x18",
    "e0: LD (0xFF00 + u8), A",
    "e1: POP HL",
    "e2: LD (0xFF00 + C), A",
    "e3: UNUSED",
    "e4: UNUSED",
    "e5: PUSH HL",
    "e6: AND u8",
    "e7: RST 0x20",
    "e8: ADD SP, i8",
    "e9: JP HL",
    "ea: LD (u16), A",
    "eb: UNUSED",
    "ec: UNUSED",
    "ed: UNUSED",
    "ee: XOR u8",
    "ef: RST 0x28",
    "f0: LD A, (0xFF00 + u8)",
    "f1: POP AF",
    "f2: LD A, (0xFF00 + C)",
    "f3: DI",
    "f4: UNUSED",
    "f5: PUSH AF",
    "f6: OR u8",
    "f7: RST 0x30",
    "f8: LD HL, SP+i8",
    "f9: LD SP, HL",
    "fa: LD A, (u16)",
    "fb: EI",
    "fc: UNUSED",
    "fd: UNUSED",
    "fe: CP u8",
    "ff: RST 0x38"
};

static const char *central_processing_unit_prefixed_instruction_mnemonics[256] =
{
    "cb00: RLC B",
    "cb01: RLC C",
    "cb02: RLC D",
    "cb03: RLC E",
    "cb04: RLC H",
    "cb05: RLC L",
    "cb06: RLC (HL)",
    "cb07: RLC A",
    "cb08: RRC B",
    "cb09: RRC C",
    "cb0a: RRC D",
    "cb0b: RRC E",
    "cb0c: RRC H",
    "cb0d: RRC L",
    "cb0e: RRC (HL)",
    "cb0f: RRC A",
    "cb10: RL B",
    "cb11: RL C",
    "cb12: RL D",
    "cb13: RL E",
    "cb14: RL H",
    "cb15: RL L",
    "cb16: RL (HL)",
    "cb17: RL A",
    "cb18: RR B",
    "cb19: RR C",
    "cb1a: RR D",
    "cb1b: RR E",
    "cb1c: RR H",
    "cb1d: RR L",
    "cb1e: RR (HL)",
    "cb1f: RR A",
    "cb20: SLA B",
    "cb21: SLA C",
    "cb22: SLA D",
    "cb23: SLA E",
    "cb24: SLA H",
    "cb25: SLA L",
    "cb26: SLA (HL)",
    "cb27: SLA A",
    "cb28: SRA B",
    "cb29: SRA C",
    "cb2a: SRA D",
    "cb2b: SRA E",
    "cb2c: SRA H",
    "cb2d: SRA L",
    "cb2e: SRA (HL)",
    "cb2f: SRA A",
    "cb30: SWAP B",
    "cb31: SWAP C",
    "cb32: SWAP D",
    "cb33: SWAP E",
    "cb34: SWAP H",
    "cb35: SWAP L",
    "cb36: SWAP (HL)",
    "cb37: SWAP A",
    "cb38: SRL B",
    "cb39: SRL C",
    "cb3a: SRL D",
    "cb3b: SRL E",
    "cb3c: SRL H",
    "cb3d: SRL L",
    "cb3e: SRL (HL)",
    "cb3f: SRL A",
    "cb40: BIT 0, B",
    "cb41: BIT 0, C",
    "cb42: BIT 0, D",
    "cb43: BIT 0, E",
    "cb44: BIT 0, H",
    "cb45: BIT 0, L",
    "cb46: BIT 0, (HL)",
    "cb47: BIT 0, A",
    "cb48: BIT 1, B",
    "cb49: BIT 1, C",
    "cb4a: BIT 1, D",
    "cb4b: BIT 1, E",
    "cb4c: BIT 1, H",
    "cb4d: BIT 1, L",
    "cb4e: BIT 1, (HL)",
    "cb4f: BIT 1, A",
    "cb50: BIT 2, B",
    "cb51: BIT 2, C",
    "cb52: BIT 2, D",
    "cb53: BIT 2, E",
    "cb54: BIT 2, H",
    "cb55: BIT 2, L",
    "cb56: BIT 2, (HL)",
    "cb57: BIT 2, A",
    "cb58: BIT 3, B",
    "cb59: BIT 3, C",
    "cb5a: BIT 3, D",
    "cb5b: BIT 3, E",
    "cb5c: BIT 3, H",
    "cb5d: BIT 3, L",
    "cb5e: BIT 3, (HL)",
    "cb5f: BIT 3, A",
    "cb60: BIT 4, B",
    "cb61: BIT 4, C",
    "cb62: BIT 4, D",
    "cb63: BIT 4, E",
    "cb64: BIT 4, H",
    "cb65: BIT 4, L",
    "cb66: BIT 4, (HL)",
    "cb67: BIT 4, A",
    "cb68: BIT 5, B",
    "cb69: BIT 5, C",
    "cb6a: BIT 5, D",
    "cb6b: BIT 5, E",
    "cb6c: BIT 5, H",
    "cb6d: BIT 5, L",
    "cb6e: BIT 5, (HL)",
    "cb6f: BIT 5, A",
    "cb70: BIT 6, B",
    "cb71: BIT 6, C",
    "cb72: BIT 6, D",
    "cb73: BIT 6, E",
    "cb74: BIT 6, H",
    "cb75: BIT 6, L",
    "cb76: BIT 6, (HL)",
    "cb77: BIT 6, A",
    "cb78: BIT 7, B",
    "cb79: BIT 7, C",
    "cb7a: BIT 7, D",
    "cb7b: BIT 7, E",
    "cb7c: BIT 7, H",
    "cb7d: BIT 7, L",
    "cb7e: BIT 7, (HL)",
    "cb7f: BIT 7, A",
    "cb80: RES 0, B",
    "cb81: RES 0, C",
    "cb82: RES 0, D",
    "cb83: RES 0, E",
    "cb84: RES 0, H",
    "cb85: RES 0, L",
    "cb86: RES 0, (HL)",
    "cb87: RES 0, A",
    "cb88: RES 1, B",
    "cb89: RES 1, C",
    "cb8a: RES 1, D",
    "cb8b: RES 1, E",
    "cb8c: RES 1, H",
    "cb8d: RES 1, L",
    "cb8e: RES 1, (HL)",
    "cb8f: RES 1, A",
    "cb90: RES 2, B",
    "cb91: RES 2, C",
    "cb92: RES 2, D",
    "cb93: RES 2, E",
    "cb94: RES 2, H",
    "cb95: RES 2, L",
    "cb96: RES 2, (HL)",
    "cb97: RES 2, A",
    "cb98: RES 3, B",
    "cb99: RES 3, C",
    "cb9a: RES 3, D",
    "cb9b: RES 3, E",
    "cb9c: RES 3, H",
    "cb9d: RES 3, L",
    "cb9e: RES 3, (HL)",
    "cb9f: RES 3, A",
    "cba0: RES 4, B",
    "cba1: RES 4, C",
    "cba2: RES 4, D",
    "cba3: RES 4, E",
    "cba4: RES 4, H",
    "cba5: RES 4, L",
    "cba6: RES 4, (HL)",
    "cba7: RES 4, A",
    "cba8: RES 5, B",
    "cba9: RES 5, C",
    "cbaa: RES 5, D",
    "cbab: RES 5, E",
    "cbac: RES 5, H",
    "cbad: RES 5, L",
    "cbae: RES 5, (HL)",
    "cbaf: RES 5, A",
    "cbb0: RES 6, B",
    "cbb1: RES 6, C",
    "cbb2: RES 6, D",
    "cbb3: RES 6, E",
    "cbb4: RES 6, H",
    "cbb5: RES 6, L",
    "cbb6: RES 6, (HL)",
    "cbb7: RES 6, A",
    "cbb8: RES 7, B",
    "cbb9: RES 7, C",
    "cbba: RES 7, D",
    "cbbb: RES 7, E",
    "cbbc: RES 7, H",
    "cbbd: RES 7, L",
    "cbbe: RES 7, (HL)",
    "cbbf: RES 7, A",
    "cbc0: SET 0, B",
    "cbc1: SET 0, C",
    "cbc2: SET 0, D",
    "cbc3: SET 0, E",
    "cbc4: SET 0, H",
    "cbc5: SET 0, L",
    "cbc6: SET 0, (HL)",
    "cbc7: SET 0, A",
    "cbc8: SET 1, B",
    "cbc9: SET 1, C",
    "cbca: SET 1, D",
    "cbcb: SET 1, E",
    "cbcc: SET 1, H",
    "cbcd: SET 1, L",
    "cbce: SET 1, (HL)",
    "cbcf: SET 1, A",
    "cbd0: SET 2, B",
    "cbd1: SET 2, C",
    "cbd2: SET 2, D",
    "cbd3: SET 2, E",
    "cbd4: SET 2, H",
    "cbd5: SET 2, L",
    "cbd6: SET 2, (HL)",
    "cbd7: SET 2, A",
    "cbd8: SET 3, B",
    "cbd9: SET 3, C",
    "cbda: SET 3, D",
    "cbdb: SET 3, E",
    "cbdc: SET 3, H",
    "cbdd: SET 3, L",
    "cbde: SET 3, (HL)",
    "cbdf: SET 3, A",
    "cbe0: SET 4, B",
    "cbe1: SET 4, C",
    "cbe2: SET 4, D",
    "cbe3: SET 4, E",
    "cbe4: SET 4, H",
    "cbe5: SET 4, L",
    "cbe6: SET 4, (HL)",
    "cbe7: SET 4, A",
    "cbe8: SET 5, B",
    "cbe9: SET 5, C",
    "cbea: SET 5, D",
    "cbeb: SET 5, E",
    "cbec: SET 5, H",
    "cbed: SET 5, L",
    "cbee: SET 5, (HL)",
    "cbef: SET 5, A",
    "cbf0: SET 6, B",
    "cbf1: SET 6, C",
    "cbf2: SET 6, D",
    "cbf3: SET 6, E",
    "cbf4: SET 6, H",
    "cbf5: SET 6, L",
    "cbf6: SET 6, (HL)",
    "cbf7: SET 6, A",
    "cbf8: SET 7, B",
    "cbf9: SET 7, C",
    "cbfa: SET 7, D",
    "cbfb: SET 7, E",
    "cbfc: SET 7, H",
    "cbfd: SET 7, L",
    "cbfe: SET 7, (HL)",
    "cbff: SET 7, A"
};

} // namespace GameBoyCore
