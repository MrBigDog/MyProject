///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_bidirectional_chain.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : bidirectional_chain class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_bidirectional_chain.h"

namespace uniscope_globe
{
	bidirectional_chain::bidirectional_chain(void)
	{
		chain_size = 0;

		chain_head = 0;

		chain_tail = 0;
	}

	bidirectional_chain::~bidirectional_chain(void)
	{

	}

	void bidirectional_chain::move_to_head(chain_iterator* iter)
	{
		// is already the head
		if (chain_head == iter)
			return;

		// when size of the chain is zero , do this
		if (chain_head == NULL)
		{
			chain_head = iter;
			chain_tail = iter;
			iter->next_iter = NULL;
			iter->prev_iter = NULL;
			iter->__parent_chain__ = this;

			chain_size++;
			return;
		}

		// when size of the chain is one  and add new, do this
		if (chain_head == chain_tail)
		{
			chain_tail->prev_iter = iter;
			chain_head = iter;

			iter->next_iter = chain_tail;
			iter->__parent_chain__ = this;

			chain_size++;
			return;
		}

		if (iter->next_iter != NULL || iter->prev_iter != NULL)
		{
			erase(iter);
		}
		// 		else
		// 		// add new
		// 		{
		// 			chain_size++;
		// 		}

		iter->prev_iter = NULL;
		iter->next_iter = chain_head;
		iter->__parent_chain__ = this;

		chain_head->prev_iter = iter;
		chain_head = iter;

		chain_size++;

	}

	void bidirectional_chain::move_to_tail(chain_iterator* iter)
	{

	}

	// we always believe that the in node is valid
	void bidirectional_chain::erase(chain_iterator* iter)
	{
		if (iter == NULL)
			return;

		iter->__parent_chain__ = NULL;

		// when size of the chain is zero, do nothing
		if (chain_head == NULL)
			return;

		// when size of the chain is one, do this
		if (chain_head == chain_tail)
		{
			if (chain_head == iter)
			{
				chain_tail = NULL;
				chain_head = NULL;
			}
		}
		// the pointer is dirty
		else if (iter->next_iter == NULL && iter->prev_iter == NULL)
		{
			return;
		}
		// when the node is the head, do this
		else if (iter == chain_head)
		{
			chain_head = iter->next_iter;
			chain_head->prev_iter = NULL;
		}
		// when the node is the tail, do this
		else if (iter == chain_tail)
		{
			chain_tail = iter->prev_iter;
			chain_tail->next_iter = NULL;
		}
		// when the node is between head and tail, do this
		else
		{
			iter->prev_iter->next_iter = iter->next_iter;
			iter->next_iter->prev_iter = iter->prev_iter;
		}

		// to zero
		iter->next_iter = NULL;
		iter->prev_iter = NULL;

		chain_size--;
	}

	void bidirectional_chain::clear()
	{
		chain_head = NULL;
		chain_tail = NULL;
	}

	void bidirectional_chain::push_head(chain_iterator* iter)
	{
		move_to_head(iter);
		//chain_size++;
	}

	void bidirectional_chain::push_back(chain_iterator* iter)
	{
		//move_to_tail( iter );
		//chain_size++;
	}

	chain_iterator*	bidirectional_chain::pop_head(void)
	{
		chain_iterator* ret = chain_head;
		erase(chain_head);

		return ret;
	}

	chain_iterator*	bidirectional_chain::pop_tail(void)
	{
		chain_iterator* ret = chain_tail;
		erase(chain_tail);

		return ret;
	}

	chain_iterator* bidirectional_chain::head(void)
	{
		return chain_head;
	}

	chain_iterator* bidirectional_chain::tail(void)
	{
		return chain_tail;
	}

	uint bidirectional_chain::size()
	{
		return chain_size;
	}


}