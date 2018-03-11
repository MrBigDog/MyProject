///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_bidirectional_chain.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : bidirectional_chain class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_BIDIRECTIONAL_CHAIN_H_
#define _US_BIDIRECTIONAL_CHAIN_H_

#include <usUtil\Export.h>
#include <usUtil\us_common_file.h>

namespace uniscope_globe
{
	class bidirectional_chain;

	class USUTIL_EXPORT chain_iterator
	{
	public:
		friend class bidirectional_chain;

	public:
		chain_iterator(void)
		{
			prev_iter = 0;
			next_iter = 0;
		}

		virtual ~chain_iterator(void)
		{
			prev_iter = 0;
			next_iter = 0;
		}

		chain_iterator* next(void) { return next_iter; }

		chain_iterator* previous(void) { return prev_iter; }

	protected:
		chain_iterator* prev_iter;
		chain_iterator* next_iter;

		bidirectional_chain* __parent_chain__;
	};

	class  USUTIL_EXPORT bidirectional_chain
	{
	public:
		bidirectional_chain(void);

		virtual ~bidirectional_chain(void);

		friend class chain_iterator;

	public:
		void	move_to_head(chain_iterator* iter);

		void	move_to_tail(chain_iterator* iter);

		void	erase(chain_iterator* iter);

		void	clear();

		void	push_head(chain_iterator* iter);

		void	push_back(chain_iterator* iter);

		chain_iterator*	pop_head(void);

		chain_iterator*	pop_tail(void);

		chain_iterator* head(void);

		chain_iterator* tail(void);

		uint	size();

	protected:
		uint			chain_size;

		chain_iterator* chain_head;
		chain_iterator* chain_tail;
	};
}

#endif // _US_BIDIRECTIONAL_CHAIN_H_