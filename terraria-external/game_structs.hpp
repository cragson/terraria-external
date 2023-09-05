#pragma once
#include <cstdint>

class vector2
{
public:
	float x, y;
};

class player_list
{
public:
	char pad_0000[4]; //0x0000
	int32_t m_iPlayerCount; //0x0004
}; 

class player
{
public:
	char pad_0000[12]; //0x0000
	int32_t m_iIndex; //0x000C
	char pad_0010[24]; //0x0010
	vector2 m_vecPosWritable; //0x0028
	char pad_0030[8]; //0x0030
	vector2 m_vecPosReadable; //0x0038
	char pad_0040[956]; //0x0040
	int32_t m_iArmor; //0x03FC
	int32_t m_iHealthMaximum; //0x0400
	char pad_0404[4]; //0x0404
	int32_t m_iHealth; //0x0408
	int32_t m_iMana; //0x040C
	int32_t m_iManaMaximum; //0x0410
	char pad_0414[4]; //0x0414
}; 

class entity_list
{
public:
	char pad_0000[4]; //0x0000
	int32_t m_iEntityCount; //0x0004
}; 

class entity
{
public:
	char pad_0000[12]; //0x0000
	int32_t m_iIndex; //0x000C
	char pad_0010[24]; //0x0010
	vector2 m_vecPos1; //0x0028
	char pad_0030[8]; //0x0030
	vector2 m_vecPos2; //0x0038
	char pad_0040[204]; //0x0040
	int32_t m_iHealth; //0x010C
	int32_t m_iMaximumHealth; //0x0110
	char pad_0114[224]; //0x0114
}; 
