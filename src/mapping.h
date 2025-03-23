#pragma once

#include <memory>
#include <vector>

#include "pagedadrs.h"
#include "romentryspec.h"
#include "rom512.h"

/// A mapping between a set of romentry spec and a rom512
class mapping
{
	typedef struct
	{
		std::shared_ptr<romentryspec> entry_;
		pagedadrs_t adrs_;
	} entrymap;

	std::vector<entrymap> entries_;

	rom512 &rom_;

	public:
		mapping( rom512 &rom ) : rom_(rom) {}

		void add_entry( std::shared_ptr<romentryspec> entry )
		{
			entrymap em{ entry, pagedadrs_t( 0, adrs_t(0) ) };
			entries_.push_back( em );

			rom_.add( *entry, em.adrs_ );
		}
};
