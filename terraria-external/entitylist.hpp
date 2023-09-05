#pragma once

#include "globals.hpp"
#include "offsets.hpp"
#include "game_structs.hpp"

namespace EntityList
{
	[[nodiscard]] inline auto get_entities_count() noexcept
	{
		return Globals::g_pProcess->read< int32_t >( Offsets::entity_list + offsetof( entity_list, m_iEntityCount ) );
	}

	[[nodiscard]] inline auto get_player_ptr() noexcept
	{
		return Globals::g_pProcess->read< std::uintptr_t >( Offsets::player_list + 8 );
	}

	[[nodiscard]] inline auto get_player() noexcept
	{
		return Globals::g_pProcess->read< entity >( get_player_ptr() );
	}

	[[nodiscard]] inline auto get_entity_ptr( const uint16_t idx ) noexcept
	{
		return Globals::g_pProcess->read< std::uintptr_t >( Offsets::entity_list + 0x8 + idx * sizeof( std::uintptr_t )
		);
	}

	[[nodiscard]] inline auto get_entity( const uint16_t idx ) noexcept
	{
		return Globals::g_pProcess->read< entity >( get_entity_ptr( idx ) );
	}

	[[nodiscard]] inline auto is_friendly_entity( const std::uintptr_t ptr ) noexcept
	{
		return Globals::g_pProcess->read< int32_t >( ptr + 0x18 ) == 0x12 && Globals::g_pProcess->read<
			int32_t >( ptr + 0x1C ) == 0x28 && Globals::g_pProcess->read< int32_t >(
			ptr + offsetof( entity, m_iMaximumHealth )
		) == 250;
	}
}
