#ifndef __SIRENGINE_STEAM_P2P_H__
#define __SIRENGINE_STEAM_P2P_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Types.h>
#include <steam/steam_api.h>
#include <steam/isteamnetworking.h>

namespace SIREngine::Network {

inline constexpr uint32_t MaxLobbyPlayers = 16;
inline constexpr uint32_t MaxPacketSize = 8192;

class SteamP2P
{
public:
	SteamP2P();
	~SteamP2P();

	static void SendPacket( byte *pData, uint32_t nLength );
	static void ReadPackets( void );
private:
	void ReadPacket( uint32 nPacketSize );

	static SteamP2P Instance;

	CSteamID m_szPlayerIds[ MaxLobbyPlayers ];
	int m_nLobbyMemberCount;
};

};

#endif