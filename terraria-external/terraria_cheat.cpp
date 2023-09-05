#include "terraria_cheat.hpp"

#include "ft_disable_consume.hpp"
#include "ft_godmode.hpp"
#include "ft_key.hpp"
#include "ft_max_stat.hpp"
#include "ft_tp_enemies.hpp"
#include "ft_tp_npc.hpp"
#include "ft_weak_enemies.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include "offsets.hpp"

bool terraria_cheat::setup_features()
{
	auto stat = std::make_unique< ft_max_stat >();
	stat->set_activation_delay( 420 );
	stat->set_name( L"Infinite Health & Mana" );
	stat->set_print_status( true );
	stat->set_virtual_key_code( VK_NUMPAD1 );

	this->m_features.push_back( std::move( stat ) );

	auto weak = std::make_unique< ft_weak_enemies >();
	weak->set_activation_delay( 420 );
	weak->set_name( L"Weak Enemies" );
	weak->set_print_status( true );
	weak->set_virtual_key_code( VK_NUMPAD2 );

	this->m_features.push_back( std::move( weak ) );

	auto tp = std::make_unique< ft_tp_npc >();
	tp->set_activation_delay( 420 );
	tp->set_name( L"Teleport NPC's to player" );
	tp->set_print_status( false );
	tp->set_virtual_key_code( VK_NUMPAD3 );

	this->m_features.push_back( std::move( tp ) );

	auto tp_en = std::make_unique< ft_tp_enemies >();
	tp_en->set_activation_delay( 420 );
	tp_en->set_name( L"Teleport Enemie's to player" );
	tp_en->set_print_status( true );
	tp_en->set_virtual_key_code( VK_NUMPAD4 );

	this->m_features.push_back( std::move( tp_en ) );

	auto god = std::make_unique< ft_godmode >();
	god->set_activation_delay( 420 );
	god->set_name( L"Godmode" );
	god->set_print_status( true );
	god->set_virtual_key_code( VK_NUMPAD5 );

	this->m_features.push_back( std::move( god ) );

	auto item = std::make_unique< ft_disable_consume >();
	item->set_activation_delay( 420 );
	item->set_name( L"Infinite Items" );
	item->set_print_status( true );
	item->set_virtual_key_code( VK_NUMPAD6 );

	this->m_features.push_back( std::move( item ) );

	auto key = std::make_unique< ft_key >();
	key->set_activation_delay( 420 );
	key->set_name( L"Infinite Keys" );
	key->set_print_status( true );
	key->set_virtual_key_code( VK_NUMPAD7 );

	this->m_features.push_back( std::move( key ) );

	return true;
}

bool terraria_cheat::setup_offsets()
{
	const auto find_pattern = [&]( const std::vector< uint8_t >& bytes, const std::wstring& pattern )
	{
		std::vector< uint8_t > m_vecPattern;

		std::string Temp = std::string();

		std::string strPattern = std::string( pattern.begin(), pattern.end() );
		std::erase_if( strPattern, isspace );

		// Convert string pattern to byte pattern
		for( const auto& pattern_byte : strPattern )
		{
			if( pattern_byte == '?' )
			{
				m_vecPattern.emplace_back( WILDCARD_BYTE );
				continue;
			}

			if( Temp.length() != 2 )
				Temp += pattern_byte;

			if( Temp.length() == 2 )
			{
				std::erase_if( Temp, isspace );
				auto converted_pattern_byte = strtol( Temp.c_str(), nullptr, 16 ) & 0xFFF;
				m_vecPattern.emplace_back( converted_pattern_byte );
				Temp.clear();
			}
		}
		const auto vector_size = m_vecPattern.size();

		// m_vecPattern contains now the converted byte pattern
		// Search now the memory area

		bool           found      = false;
		std::uintptr_t found_addr = 0;

		for( std::uintptr_t current_addr = 0; current_addr < bytes.size(); current_addr++ )
		{
			if( found )
				break;

			for( uint8_t i = 0; i < vector_size; i++ )
			{
				if( current_addr + i >= bytes.size() )
					break;

				const auto current_byte = bytes.at( current_addr + i );

				const auto pattern_byte = m_vecPattern.at( i );

				if( static_cast< uint8_t >( pattern_byte ) == WILDCARD_BYTE )
				{
					if( i == vector_size - 1 )
					{
						found      = true;
						found_addr = current_addr;
						break;
					}
					continue;
				}

				if( static_cast< uint8_t >( current_byte ) != pattern_byte )
					break;

				if( i == vector_size - 1 )
				{
					found_addr = current_addr;
					found      = true;
				}
			}
		}

		return found_addr;
	};

	MEMORY_BASIC_INFORMATION mbi;
	RtlZeroMemory( &mbi, sizeof(mbi) );

	std::uintptr_t read_page_sizes = {};

	while( VirtualQueryEx( Globals::g_pProcess->get_process_handle(), reinterpret_cast< LPCVOID >( read_page_sizes ),
	                       &mbi, sizeof( mbi )
	) != 0 )
	{
		// read can fail because read pe is .NET assembly which has uncommitted pages which causes trouble
		// cc: https://www.unknowncheats.me/forum/903841-post3.html - thanks!

		if( !( mbi.State & MEM_COMMIT ) || !( mbi.Protect & PAGE_EXECUTE_READWRITE ) || !( mbi.Type & MEM_PRIVATE ) )
		{
			read_page_sizes += mbi.RegionSize;

			continue;
		}

		std::vector< uint8_t > temp = {};
		temp.resize( mbi.RegionSize );

		if( !ReadProcessMemory( Globals::g_pProcess->get_process_handle(), mbi.AllocationBase, temp.data(),
		                        mbi.RegionSize, nullptr
		) )
		{
			read_page_sizes += mbi.RegionSize;

			continue;
		}

		if( !Offsets::godmode_patch )
			if( const auto god_addr = find_pattern( temp, L"29 82 ? ? ? ? 83 7D" ); god_addr )
				Offsets::godmode_patch = reinterpret_cast< std::uintptr_t >( mbi.AllocationBase ) + god_addr;

		if( !Offsets::player_list )
		{
			if( const auto pllist = find_pattern( temp, L"C0 75 44 A1 ? ? ? ? 8B 15" ); pllist )
			{
				std::uintptr_t list_ptr = {};

				memcpy( &list_ptr, &temp.at( pllist + 4 ), sizeof( std::uintptr_t ) );

				Offsets::player_list = Globals::g_pProcess->read< std::uintptr_t >( list_ptr );
			}
		}

		if( !Offsets::entity_list )
		{
			if( const auto entlist = find_pattern( temp, L"D9 00 A1 ? ? ? ? 3B 58 04 0F 83" ); entlist )
			{
				std::uintptr_t list_ptr = {};

				memcpy( &list_ptr, &temp.at( entlist + 3 ), sizeof( std::uintptr_t ) );

				Offsets::entity_list = Globals::g_pProcess->read< std::uintptr_t >( list_ptr );
			}
		}

		if( !Offsets::item_patch_1 )
			if( const auto item_addr = find_pattern( temp, L"FF 88 B4 00 00 00 8B 85 10 FD FF FF 83" ); item_addr )
				Offsets::item_patch_1 = reinterpret_cast< std::uintptr_t >( mbi.AllocationBase ) + item_addr;

		if( !Offsets::item_patch_2 )
			if( const auto item_addr = find_pattern( temp, L"FF 89 B4 00 00 00 8B 45 F0" ); item_addr )
				Offsets::item_patch_2 = reinterpret_cast< std::uintptr_t >( mbi.AllocationBase ) + item_addr;

		if( !Offsets::key_patch )
			if( const auto key_addr = find_pattern( temp, L"FF 88 B4 00 00 00 8B 86 D8 00 00 00 3B 58" ); key_addr )
				Offsets::key_patch = reinterpret_cast< std::uintptr_t >( mbi.AllocationBase ) + key_addr;

		read_page_sizes += mbi.RegionSize;
	}

	return true;
}

void terraria_cheat::print_features()
{
	printf( "\n" );

	printf( "Feature-Name -> Feature-Hotkey\n" );

	for( const auto& feature : this->m_features )
		printf( "[>] %-35ws -> %s\n", feature->get_name().c_str(),
		        utils::virtual_key_as_string( feature->get_virtual_key_code() ).c_str()
		);

	printf( "\n" );
}

void terraria_cheat::print_offsets()
{
	printf( "\n" );

	const auto msg = []( const std::string& name, const std::uintptr_t value )
	{
		printf( "[>] %-35s -> 0x%08X\n", name.c_str(), value );
	};

	msg( "Godmode Patch", Offsets::godmode_patch );
	msg( "Item Patch 1", Offsets::item_patch_1 );
	msg( "Item Patch 2", Offsets::item_patch_2 );
	msg( "Key Patch", Offsets::key_patch );
	msg( "PlayerList", Offsets::player_list );
	msg( "EntityList", Offsets::entity_list );


	printf( "\n" );
}

void terraria_cheat::run()
{
	for( const auto& feature : this->m_features )
	{
		// before tick'ing the feature, check first if the state will eventually change
		if( GetAsyncKeyState( feature->get_virtual_key_code() ) & 0x8000 )
			feature->toggle();

		// let the feature tick() when active
		if( feature->is_active() )
			feature->tick();
	}
}

void terraria_cheat::shutdown()
{
	// disable every feature here
	for( const auto& feature : this->m_features )
		if( feature->is_active() )
			feature->disable();

	// clear image map here
	if( Globals::g_pProcess )
		Globals::g_pProcess->clear_image_map();
}
