#pragma once
struct Range
{
	int start = 0;
	int end = 0;

	int difference() const;
	bool isRangeNatural() const;
	bool isRangeLeftToRight() const;
};
