#include "ft_max_stat.hpp"

#include "entitylist.hpp"

void ft_max_stat::tick()
{
	if( const auto player_ptr = EntityList::get_player_ptr(); player_ptr )
	{
		Globals::g_pProcess->write< int32_t >( player_ptr + offsetof( player, m_iHealthMaximum ), 1337 );
		Globals::g_pProcess->write< int32_t >( player_ptr + offsetof( player, m_iHealth ), 1337 );

		Globals::g_pProcess->write< int32_t >( player_ptr + offsetof( player, m_iManaMaximum ), 1337 );
		Globals::g_pProcess->write< int32_t >( player_ptr + offsetof( player, m_iMana ), 1337 );
	}
}
