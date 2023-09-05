#include "ft_weak_enemies.hpp"

#include "entitylist.hpp"

void ft_weak_enemies::tick()
{
	const auto total_ents = EntityList::get_entities_count();

	auto player_pos = Globals::g_pProcess->read< vector2 >(
		EntityList::get_player_ptr() + offsetof( player, m_vecPosReadable )
	);
	player_pos.y -= 69;

	for( uint32_t idx = 0; idx < total_ents; idx++ )
	{
		const auto ent_ptr = EntityList::get_entity_ptr( idx );

		if( !ent_ptr )
			continue;

		const auto ent_health = Globals::g_pProcess->read< int32_t >( ent_ptr + offsetof( entity, m_iHealth ) );

		if( ent_health <= 0 || EntityList::is_friendly_entity( ent_ptr ) )
			continue;

		Globals::g_pProcess->write< int32_t >( ent_ptr + offsetof( entity, m_iHealth ), 1 );
	}
}
