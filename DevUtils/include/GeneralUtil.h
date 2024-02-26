#pragma once
namespace ROA::UTIL
{

struct Range
{
	int start = 0;
	int end = 0;

	int difference() const;
	bool isRangeNatural() const;
	bool isRangeLeftToRight() const;
};

struct ResolutionInformation
{
	unsigned width;
	unsigned height;
	unsigned bbP;
	unsigned frequenzy;

	ResolutionInformation(unsigned w, unsigned h, unsigned bbP, unsigned freq);
};

std::vector<ResolutionInformation> getAllResolutions();
ResolutionInformation getCurrentResolution();

}