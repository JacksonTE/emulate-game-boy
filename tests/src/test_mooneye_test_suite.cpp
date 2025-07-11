#include <algorithm>
#include <filesystem>
#include <gtest/gtest.h>
#include <vector>

#include "emulator.h"

static std::filesystem::path get_test_directory_path()
{
    return std::filesystem::path(PROJECT_ROOT) / "tests" / "data" / "mooneye-test-suite" / "mts-20240926-1737-443f6e1";
}

static std::vector<std::filesystem::path> get_test_rom_paths_in_directory(const std::filesystem::path &directory)
{
    if (!std::filesystem::exists(directory))
        return { directory };

    std::vector<std::filesystem::path> test_rom_paths;

    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        // Skip tests that are specific to other Game Boy models than DMG
        if (entry.is_regular_file() && entry.path().extension() == ".gb" &&
            entry.path().filename() != "unused_hwio-GS.gb" && // TODO add this test after APU is done
            entry.path().filename() != "boot_div-dmg0.gb" &&
            entry.path().filename() != "boot_div-S.gb" &&
            entry.path().filename() != "boot_div2-S.gb" &&
            entry.path().filename() != "boot_hwio-dmgABCmgb.gb" && // TODO add this test after APU is done
            entry.path().filename() != "boot_hwio-dmg0.gb" &&
            entry.path().filename() != "boot_hwio-S.gb" &&
            entry.path().filename() != "boot_regs-dmg0.gb" &&
            entry.path().filename() != "boot_regs-mgb.gb" &&
            entry.path().filename() != "boot_regs-sgb.gb" &&
            entry.path().filename() != "boot_regs-sgb2.gb")
        {
            test_rom_paths.push_back(entry.path());
        }
    }
    std::sort(test_rom_paths.begin(), test_rom_paths.end());
    return test_rom_paths;
}

class MooneyeTest : public testing::TestWithParam<std::filesystem::path>
{
protected:
    GameBoyCore::Emulator game_boy_emulator;
    std::string error_message{};

    void SetUp() override
    {
        ASSERT_TRUE(std::filesystem::exists(GetParam())) << "ROM file not found: " << GetParam();
        game_boy_emulator.try_load_file_to_memory(GetParam(), GameBoyCore::FileType::GameRom, error_message);
        game_boy_emulator.set_post_boot_state();
    }
};

TEST_P(MooneyeTest, TestRom)
{
    const std::filesystem::path test_rom_path = GetParam();
    SCOPED_TRACE("Test ROM: " + test_rom_path.string());

    constexpr size_t max_instructions_before_timeout = 10'000'000;
    bool did_test_succeed = false;

    for (size_t _ = 0; _ < max_instructions_before_timeout; _++)
    {
        game_boy_emulator.step_central_processing_unit_single_instruction();
        auto r = game_boy_emulator.get_register_file();

        if (r.b == 0x42 && r.c == 0x42 && r.d == 0x42 && r.e == 0x42 && r.h == 0x42 && r.l == 0x42)
        {
            FAIL();
        }
        else if (r.b == 3 && r.c == 5 && r.d == 8 && r.e == 13 && r.h == 21 && r.l == 34)
        {
            did_test_succeed = true;
            break;
        }
    }
    ASSERT_TRUE(did_test_succeed) << "Test didn't reach a finished state within " << max_instructions_before_timeout << " instructions";
}

INSTANTIATE_TEST_SUITE_P
(
    MooneyeAcceptanceTestsBits,
    MooneyeTest,
    testing::ValuesIn(get_test_rom_paths_in_directory(get_test_directory_path() / "acceptance" / "bits")),
    [](auto info) { return info.param.stem().string(); }
);

INSTANTIATE_TEST_SUITE_P
(
    MooneyeAcceptanceTestsInstructions,
    MooneyeTest,
    testing::ValuesIn(get_test_rom_paths_in_directory(get_test_directory_path() / "acceptance" / "instr")),
    [](auto info) { return info.param.stem().string(); }
);

INSTANTIATE_TEST_SUITE_P
(
    MooneyeAcceptanceTestsInterrupts,
    MooneyeTest,
    testing::ValuesIn(get_test_rom_paths_in_directory(get_test_directory_path() / "acceptance" / "interrupts")),
    [](auto info) { return info.param.stem().string(); }
);

INSTANTIATE_TEST_SUITE_P
(
    MooneyeAcceptanceTestsObjectAttributeMemoryDirectMemoryAccess,
    MooneyeTest,
    testing::ValuesIn(get_test_rom_paths_in_directory(get_test_directory_path() / "acceptance" / "oam_dma")),
    [](auto info)
    {
        std::string test_rom_file_name = info.param.stem().string();
        std::replace(test_rom_file_name.begin(), test_rom_file_name.end(), '-', '_');
        return test_rom_file_name;
    }
);

INSTANTIATE_TEST_SUITE_P
(
    MooneyeAcceptanceTestsPixelProcessingUnit,
    MooneyeTest,
    testing::ValuesIn(get_test_rom_paths_in_directory(get_test_directory_path() / "acceptance" / "ppu")),
    [](auto info) 
    { 
        std::string test_rom_file_name = info.param.stem().string();
        std::replace(test_rom_file_name.begin(), test_rom_file_name.end(), '-', '_');
        return test_rom_file_name;
    }
);

INSTANTIATE_TEST_SUITE_P
(
    MooneyeAcceptanceTestsTimer,
    MooneyeTest,
    testing::ValuesIn(get_test_rom_paths_in_directory(get_test_directory_path() / "acceptance" / "timer")),
    [](auto info) { return info.param.stem().string(); }
);

INSTANTIATE_TEST_SUITE_P
(
    MooneyeAcceptanceTestsMiscellaneous,
    MooneyeTest,
    testing::ValuesIn(get_test_rom_paths_in_directory(get_test_directory_path() / "acceptance")),
    [](auto info)
    {
        std::string test_rom_file_name = info.param.stem().string();
        std::replace(test_rom_file_name.begin(), test_rom_file_name.end(), '-', '_');
        return test_rom_file_name;
    }
);

INSTANTIATE_TEST_SUITE_P
(
    MooneyeEmulatorOnlyTestsMBC1,
    MooneyeTest,
    testing::ValuesIn(get_test_rom_paths_in_directory(get_test_directory_path() / "emulator-only" / "mbc1")),
    [](auto info) { return info.param.stem().string(); }
);

INSTANTIATE_TEST_SUITE_P
(
    MooneyeEmulatorOnlyTestsMBC2,
    MooneyeTest,
    testing::ValuesIn(get_test_rom_paths_in_directory(get_test_directory_path() / "emulator-only" / "mbc2")),
    [](auto info) { return info.param.stem().string(); }
);

INSTANTIATE_TEST_SUITE_P
(
    MooneyeEmulatorOnlyTestsMBC5,
    MooneyeTest,
    testing::ValuesIn(get_test_rom_paths_in_directory(get_test_directory_path() / "emulator-only" / "mbc5")),
    [](auto info) { return info.param.stem().string(); }
);
