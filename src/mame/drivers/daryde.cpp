// license:BSD-3-Clause
// copyright-holders:AJR
/*******************************************************************************

    Skeleton driver for Daryde darts machines.

*******************************************************************************/

#include "emu.h"
#include "cpu/z180/z180.h"
#include "machine/timekpr.h"

class daryde_state : public driver_device
{
public:
	daryde_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
	{
	}

	void pandart(machine_config &config);

private:
	void mem_map(address_map &map);
	void io_map(address_map &map);
};

void daryde_state::mem_map(address_map &map)
{
	map(0x00000, 0x7ffff).rom().region("program", 0);
	map(0xef000, 0xef005).unmapw();
	map(0xf0000, 0xf1fff).rw("timekpr", FUNC(mk48t08_device::read), FUNC(mk48t08_device::write));
}

void daryde_state::io_map(address_map &map)
{
	map(0x0000, 0x00ff).noprw();
	map(0x4000, 0x4000).unmapr();
	map(0xc000, 0xc000).unmaprw();
}

static INPUT_PORTS_START(pandart)
INPUT_PORTS_END

void daryde_state::pandart(machine_config &config)
{
	cpu_device &maincpu(Z180(config, "maincpu", XTAL(18'432'000)));
	maincpu.set_addrmap(AS_PROGRAM, &daryde_state::mem_map);
	maincpu.set_addrmap(AS_IO, &daryde_state::io_map);

	MK48T08(config, "timekpr");
}

/* Daryde Panther Darts PCB
 ____________________________________________________________________________
 | ____________________   _____________________   _______   _______          |
 | ||||CN8 (16 pins)|||   ||||CN5 (16 pins)||||   ||CN6||   |CN15||     ___  |
 | ___                           ___________  ___________  __________   |__| |
 | |CN3                          |_74HC273N_| |_74HC273N_| |_ULN2803A|  |__| |
 | |__|                          ___________  ___________  __________   |C_| |
 |  __                           |_74HC273N_| |_74HC273N_| |_ULN2803A|  |N_| |
 | |CN7                          ___________  ___________  __________   |4_| |
 |  ____                         |_74HC273N_| |_74HC273N_| |_ULN2803A|  |__| |
 | |CN12|                          _________  ___________               |__| |
 | |DB9 |   _________              74HC138AN| |_74HC273N_|                   |
 | |    |   |MAX232N_|             _________  ___________               ___  |
 | |____| ___________              CD4051BCN| |_74HC244N_|              |__| |
 |  ___   |PALCE16V8H|       _______________                            |__| |
 |  |CN14 ______________     |              | ___________               |C_| |
 |  |__|  |            |     |M48T08-150PCI | |_74HC244N_|              |N_| |
 |  ___   |  CPU (IC6) |     |______________|                           |9_| |
 |  |CN11 |            |     _______________  ___________  ___          |__| |
 |  |  |  |            |     |AM27C040 (IC5)| |_74HC244N_| |H606016     |__| |
 |  |  |  |____________|     |______________|                           |__| |
 |  |__|  _____       _________   _________  ___________     ____            |
 |        |XTAL|      |||CN2|||   |||CN1|||  |||CN13||||     |CN10           |
 |___________________________________________________________________________|

Xtal = 18.432 MHz

CN1 = 6 pins
CN2 = 8 pins
CN4 = 20 pins
CN5 = 16 pins
CN6 = 6 pins
CN8 = 16 pins
CN9 = 26 pins
CN10 = 4 pins
CN11 = 10 pins
CN12 = 9 pins (DB9)
CN13 = 16 pins, but no connector, 2200μF25V capacitor between last pins
CN14 = 6 pins
CN15 = 5 pins */
ROM_START(pandart)
	ROM_REGION(0x80000, "program", 0)
	ROM_LOAD("27c040.ic5", 0x00000, 0x80000, CRC(b1bd5c14) SHA1(7164dcaebf0f23f5330b225e44ee87d9a8c79f4f))

	ROM_REGION(0x117, "pal", 0)
	ROM_LOAD("palce16v8h.ic1", 0x000, 0x117, NO_DUMP) // protected
ROM_END

/* Daryde Cricket PCB
 __________________________________________________
 |    ___________________  ___________________     |
 |    |_CN13 (12 pins)__|  |___CN (12 pins)__|     | 
 |                            ___               __ |
 |                _______    |H606016   ____    |C||
 |__              |74LS273N             |XTAL   |N||
 ||CN       _____________                          |
 |__        | DS1225Y-150|     ____________     __ |
 ||C|       |____________|     |           |    | ||
 ||N|       _____________      |Z8018010VSC|    |C||
 ||6|       |IC3 27C040  |     |Z180 MPU   |    |N||
 ||_|       |____________|     |___________|    |9||
 |__     _______  _______   _______             |_||
 ||C|   TDG2083AP |74LS273N |PALCE16B8             |
 ||N|    _______  _______   _______             __ |
 ||5|   |74LS273N |74LS273N |PALCE16B8          |C||
 ||_|             _______   _______             |N||
 |__              |74HCT244N|74HC244N           |8||
 ||CN             _______   _______             |_||
 ||7|             |74HC244N |74HC244N              |
 ||_|  ______ _____ _____  _____       _____       |
 |     |CN14 ||JP1| |CN11| |CN2 |      |CN10|      |
 |_________________________________________________|

XTAL = 18.432MHz
JP1 (4 bridges):
 2: VDD
 1: GND
 2: VDD
 1: GND */
ROM_START(cricket)
	ROM_REGION(0x80000, "program", 0)
	ROM_LOAD("daryde_cricket_red_1.0.ic3", 0x00000, 0x80000, CRC(6abaa50e) SHA1(f128ed9cd9926684bd77ec708a5d3edf2736e39c)) // AM27C040

	ROM_REGION(0x22e, "pal", 0)
	ROM_LOAD("a_palce16v8h.ic4", 0x000, 0x117, NO_DUMP)
	ROM_LOAD("b_palce16v8h.ic5", 0x117, 0x117, NO_DUMP)
ROM_END

GAME(1995, cricket, 0, pandart, pandart, daryde_state, empty_init, ROT0, "Daryde S. L.", "Cricket",       MACHINE_IS_SKELETON_MECHANICAL)
GAME(1999, pandart, 0, pandart, pandart, daryde_state, empty_init, ROT0, "Daryde S. L.", "Panther Darts", MACHINE_IS_SKELETON_MECHANICAL)
