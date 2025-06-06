#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

namespace GameBoyCore
{

constexpr uint8_t ROM_ONLY_BYTE = 0x00;
constexpr uint8_t MBC1_BYTE = 0x01;
constexpr uint8_t MBC1_WITH_RAM_BYTE = 0x02;
constexpr uint8_t MBC1_WITH_RAM_AND_BATTERY_BYTE = 0x03;

constexpr uint16_t ROM_BANK_SIZE = 0x4000;

constexpr uint8_t LOGO_SIZE = 48;
constexpr uint16_t LOGO_START_POSITION = 0x0104;

class MemoryBankControllerBase
{
public:
    static constexpr uint16_t no_mbc_file_size_bytes = 0x8000;

    MemoryBankControllerBase(std::vector<uint8_t> &rom, std::vector<uint8_t> &ram);

    virtual uint8_t read_byte(uint16_t address);
    virtual void write_byte(uint16_t address, uint8_t value);

protected:
    const std::vector<uint8_t> &cartridge_rom;
    std::vector<uint8_t> &cartridge_ram;
};

class MBC1 : public MemoryBankControllerBase
{
public:
    static constexpr uint32_t max_rom_size_in_default_configuration_bytes = 0x80000;
    static constexpr uint32_t max_rom_size_bytes = 0x200000;
    static constexpr uint32_t mbc1m_multi_game_compilation_cart_size_bytes = 0x100000;

    static constexpr uint32_t max_ram_size_in_large_configuration_bytes = 0x2000;

    MBC1(std::vector<uint8_t> &rom, std::vector<uint8_t> &ram);

    uint8_t read_byte(uint16_t address) override;
    void write_byte(uint16_t address, uint8_t value) override;

protected:
    uint8_t ram_enable{};
    uint8_t effective_rom_bank_number{0b00000001};
    uint8_t ram_bank_or_upper_rom_bank_number{};
    uint8_t banking_mode_select{};
};

class GameCartridgeSlot
{
public:
    GameCartridgeSlot();

    void reset_state();

    bool try_load_file(const std::filesystem::path &file_path, std::ifstream &file, std::streamsize file_length_in_bytes, std::string &error_message);

    uint8_t read_byte(uint16_t address) const;
    void write_byte(uint16_t address, uint8_t value);

private:
    std::vector<uint8_t> rom{};
    std::vector<uint8_t> ram{};
    std::unique_ptr<MemoryBankControllerBase> memory_bank_controller{};
};

} // namespace GameBoyCore
