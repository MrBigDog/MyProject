#ifndef _US_COMMON_FILE_H_
#define _US_COMMON_FILE_H_

#include <string>
#include <algorithm>

namespace uniscope_globe
{
	//
	// platform dependent
	//
#ifdef LINUX26
	typedef long long interior_index;


#else

	typedef __int64 interior_index;

#define AUTO_ALLOC_MEMORY(size) (byte*)::CoTaskMemAlloc( size );
#define AUTO_FREE_MEMORY(p) { if(p){ ::CoTaskMemFree(p); (p)=NULL;} }
#define AUTO_ALIGNED_ALLOC_MEMORY(size, alignment) (byte*)_aligned_malloc( size, alignment );
#define AUTO_ALIGNED_FREE_MEMORY(p) { if(p){ _aligned_free(p); (p)=NULL;} }
#define AUTO_CLOSE_HANDLE( handle ) if ( handle ) { CloseHandle( handle); handle = 0 ; }
#define AUTO_RELEASE(ptr) {if(ptr){(ptr)->Release();(ptr)=NULL;} }
#define	AUTO_RELEASE_SHARED_DATA(ptr) {if(ptr){(ptr)->release();(ptr)=NULL;} }
#define US_STD_CALL __stdcall

#endif //LINUX26

	//
	// internationalization
	//
#ifdef _UNICODE
	typedef std::wstring ustring;
	typedef wchar_t* pstr;
	typedef const wchar_t* cpstr;
#define _gstr(x) L##x

#define US_STRING_LOWER(str) std::transform( str.begin(), str.end(), str.begin(), ::towlower );
#define US_STRING_UPPER(str) std::transform( str.begin(), str.end(), str.begin(), ::towupper );

#else
	typedef std::string ustring;
	typedef char* pstr;
	typedef const char* cpstr;
#define _gstr(x) x

#define US_STRING_LOWER(str) std::transform( str.begin(), str.end(), str.begin(), ::tolower );
#define US_STRING_UPPER(str) std::transform( str.begin(), str.end(), str.begin(), ::toupper );

#endif // _UNICODE


	typedef unsigned int uint;
	typedef unsigned long ulong;
	typedef unsigned long dword;
	typedef unsigned short ushort;
	typedef double time_value;

#ifndef USE_SYS_BYTE
	typedef unsigned char byte;
#endif 

#ifdef _DEBUG
#define US_TRACE0(format) { wchar_t str[1024]; swprintf( str, format ); OutputDebugString(str);}
#define US_TRACE1(format, val1) { wchar_t str[1024]; swprintf( str, format, val1 ); OutputDebugString(str);}
#define US_TRACE2(format, val1, val2) { wchar_t str[1024]; swprintf( str, format, val1, val2 ); OutputDebugString(str);}
#define US_TRACE3(format, val1, val2, val3) { wchar_t str[1024]; swprintf( str, format, val1, val2, val3 ); OutputDebugString(str);}
#else
#define US_TRACE0(format)
#define US_TRACE1(format, val1) 
#define US_TRACE2(format, val1, val2) 
#define US_TRACE3(format, val1, val2, val3) 
#endif //_DEBUG

#define MAX_FILE_PATH 1024

#define MIN(a,b) ( (a)<(b) ? (a):(b))
#define MAX(a,b) ( (a)>(b) ? (a):(b))

#define AUTO_DELETE(ptr) {if(ptr){delete ptr;(ptr)=NULL;} }
#define AUTO_DELETE_ARRAY(ptr) {if(ptr){delete[] ptr;(ptr)=NULL;} }

#define AUTO_DISPOSE(ptr) {if(ptr){ptr->dispose();(ptr)=NULL;} }

#define CLAMP( in_value, min_value, max_value ) (MIN( max_value, MAX(in_value, min_value)))

#define PI (3.1415926535897932384626433832795)

#define DOUBLE_PI (6.283185307179586476925286766559)

#define HALF_PI (1.5707963267948966192313216916398)

#define DEGREE (57.295779513082320876798154814105)

#define RADIAN (0.017453292519943295769236907684886)

#define MIN_TIME_INTERVAL 0.01

#define FAST_ULONG_F(f)	(*(ulong*)&(f))

#define FAST_SIGN_F(f)	((*(ulong*)&(f))&0x80000000)

#define FAST_SIGN_I(f)	((f)&0x80000000)

#define USCOLOR_ARGB(a,r,g,b) \
	( (ulong)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)) )

#define USCOLOR_COLORVALUE(a, r,g,b) \
	USCOLOR_ARGB( (ulong)((a)*255.f), (ulong)((r)*255.f), (ulong)((g)*255.f), (ulong)((b)*255.f) )


	// rank
#define SYSTEM_RANK_MASK 0x00ffffff
#define OBSERVER_RANK_MASK 0xff0000ff
#define INSERT_SYSTEM_RANK( rank, val ) {	rank &= SYSTEM_RANK_MASK; \
											rank|= (ulong)(val)<<24;\
										}
#define INSERT_OBSERVER_RANK( rank, val ) {	if ( val < 0.0 ) val = 0.0f;\
											if ( val > 1.0 ) val = 1.0f;\
											rank &= OBSERVER_RANK_MASK;\
											ulong obs_rank = (ulong)(val * 65535 )<<8;\
											rank |= obs_rank;\
											}

#define NORMALIZE_RADIAN( rad ) {   if ( rad > DOUBLE_PI )\
										rad -= math<double>::floor_( math<double>::fabs_(rad) / DOUBLE_PI ) * DOUBLE_PI;\
									else if ( rad < DOUBLE_PI )\
										rad += math<double>::floor_( math<double>::fabs_(rad) / DOUBLE_PI ) * DOUBLE_PI;\
								}

	typedef std::pair< interior_index, interior_index > us_guid_pair;

#define US_GUID_TO_PAIR( gu_id, guid_key ) {	guid_key.first = (__int64)((unsigned long *) &gu_id)[0]<<32 | ((unsigned long *) &gu_id)[1]; \
												guid_key.second = (__int64)((unsigned long *) &gu_id)[2]<<32 | ((unsigned long *) &gu_id)[3];}

#define US_GUID_FROM_PAIR( gu_id, guid_key ) {	((unsigned long *) &gu_id)[0] = (guid_key.first >> 32) & 0x00000000ffffffff;\
												((unsigned long *) &gu_id)[1] = guid_key.first & 0x00000000ffffffff;\
												((unsigned long *) &gu_id)[2] = (guid_key.second >> 32) & 0x00000000ffffffff;\
												((unsigned long *) &gu_id)[3] = guid_key.second & 0x00000000ffffffff;}

#define GUID_TO_INT64( gu_id, part1, part2 ) {	part1 = (__int64)((unsigned long *) &gu_id)[0]<<32 | ((unsigned long *) &gu_id)[1]; \
												part2 = (__int64)((unsigned long *) &gu_id)[2]<<32 | ((unsigned long *) &gu_id)[3];}


#define GUID_FROM_INT64( gu_id, part1, part2 ) {((unsigned long *) &gu_id)[0] = (part1 >> 32) & 0x00000000ffffffff;\
												((unsigned long *) &gu_id)[1] = part1 & 0x00000000ffffffff;\
												((unsigned long *) &gu_id)[2] = (part2 >> 32) & 0x00000000ffffffff;\
												((unsigned long *) &gu_id)[3] = part2 & 0x00000000ffffffff;}

#define NEW_GUID( strGuid ) {	RPC_WSTR wsGuid = NULL;\
								GUID guid;\
								::CoCreateGuid(&guid);\
								::UuidToString( &guid, &wsGuid );\
								strGuid = (cpstr)wsGuid;\
								::RpcStringFree( &wsGuid );}

}

#endif