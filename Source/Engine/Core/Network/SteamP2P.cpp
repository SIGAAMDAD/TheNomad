#include "SteamP2P.h"
#include <steam/isteamnetworking.h>

using namespace SIREngine;
using namespace SIREngine::Network;

SteamP2P::SteamP2P()
{
	memset( m_szPlayerIds, 0, sizeof( m_szPlayerIds ) );
	m_nLobbyMemberCount = 0;
}

SteamP2P::~SteamP2P()
{
}

void SteamP2P::ReadPacket( uint32 nPacketSize )
{
	byte szRecieveBuffer[ MaxPacketSize ];
	CSteamID hSenderID;

	SteamNetworking()->ReadP2PPacket( szRecieveBuffer, MaxPacketSize, &nPacketSize, &hSenderID, 0 );
}

void SteamP2P::ReadPackets( void )
{
	uint32 nPacketSize;
	if ( SteamNetworking()->IsP2PPacketAvailable( &nPacketSize, 0 ) ) {
		Instance.ReadPacket( nPacketSize );
		ReadPackets();
	}
}

void SteamP2P::SendPacket( byte *pData, uint32_t nLength )
{
	for ( int i = 0; i < Instance.m_nLobbyMemberCount; ++i ) {
		SteamNetworking()->SendP2PPacket( Instance.m_szPlayerIds[i], pData, nLength, k_EP2PSendReliable, 0 );
	}
}
