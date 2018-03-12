
// us_ugp_filter_7z.h

#ifndef _US_UGP_FILTER_7Z_H_
#define _US_UGP_FILTER_7Z_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class LIB_FILTER_API ugp_filter_7z : public filter_base
	{
	public:
		ugp_filter_7z( void ){}

		virtual ~ugp_filter_7z( void ){}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );
		 
	};
}

#endif // _US_UGP_FILTER_7Z_H_ 