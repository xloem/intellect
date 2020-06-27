#pragma once

template <typename location_type>
struct range
{
	location_type range_begin;
	location_type range_end;
	location_type begin() { return range_begin; }
	location_type end() { return range_end; }
};
