#include "ft_godmode.hpp"

#include "globals.hpp"
#include "offsets.hpp"

void ft_godmode::on_enable()
{
	Globals::g_pProcess->nop_bytes( Offsets::godmode_patch, 6 );
}

void ft_godmode::on_disable()
{
	// 29 82 08 04 00 00 sub [ edx + 0408 ], eax
	Globals::g_pProcess->write_to_protected_memory< uint32_t >( Offsets::godmode_patch, 0x04088229 );
	Globals::g_pProcess->write_to_protected_memory< uint16_t >( Offsets::godmode_patch + 4, 0x0000 );
}
