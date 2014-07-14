#	pragma once

namespace stdex
{
	const uint32_t hash_base = 0xABCDEF01;

	template<class T>
	inline uint32_t hash_binary( uint32_t _hash, T _value )
	{
		uint32_t hash_value = (uint32_t)_value;
		uint32_t new_hash = ((_hash << 5) + _hash) + hash_value * hash_value;

		return new_hash;			 
	}
}