#include "ft_disable_consume.hpp"

#include "globals.hpp"
#include "offsets.hpp"

void ft_disable_consume::on_enable()
{
	Globals::g_pProcess->nop_bytes( Offsets::item_patch_1, 6 );

	Globals::g_pProcess->write_to_protected_memory< uint8_t >( Offsets::item_patch_2 + 1, 0x81 );
}

void ft_disable_consume::on_disable()
{
	// FF 88 B4 00 00 00 dec [ eax + 0B4 ]
	Globals::g_pProcess->write_to_protected_memory< uint32_t >( Offsets::item_patch_1, 0x00B488FF );
	Globals::g_pProcess->write_to_protected_memory< uint16_t >( Offsets::item_patch_1 + 4, 0x0000 );

	// FF 89 B4 00 00 00 dec [ ecx + 0B4 ]
	Globals::g_pProcess->write_to_protected_memory< uint8_t >( Offsets::item_patch_2 + 1, 0x89 );
}
