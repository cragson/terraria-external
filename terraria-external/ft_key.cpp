#include "ft_key.hpp"

#include "globals.hpp"
#include "offsets.hpp"

void ft_key::on_enable()
{
	Globals::g_pProcess->nop_bytes( Offsets::key_patch, 6 );
}

void ft_key::on_disable()
{
	// FF 88 B4 00 00 00 dec [ eax + 0B4 ]
	Globals::g_pProcess->write_to_protected_memory< uint32_t >( Offsets::key_patch, 0x00B488FF );
	Globals::g_pProcess->write_to_protected_memory< uint16_t >( Offsets::key_patch + 4, 0x0000 );
}
