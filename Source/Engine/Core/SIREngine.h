#define SIRENGINE_MAKE_VERSION( major, minor, patch ) \
	((((uint32_t)(major)) << 22) | (((uint32_t)(minor)) << 12) | ((uint32_t)(patch)))
#define SIRENGINE_VERSION SIRENGINE_MAKE_VERSION( 2, 0, 0 )

#define SIRENGINE_STRING( x ) #x
#define SIRENGINE_VERSION_STRING SIRENGINE_STRING( SIRENGINE_VERSION )