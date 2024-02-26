#include "pch.h"
#include "GeneralUtil.h"

using namespace ROA::UTIL;

/**
 * \brief 
 * \return absolut difference between start and end
 */
int Range::difference() const
{
	int ret = start - end;
	if(ret < 0) ret *= -1;
	return ret;
}

/**
 * \brief 
 * \return true, if start and end index are positive
 */
bool Range::isRangeNatural() const
{
	if(start < 0 || end < 0) return false;
	return true;
}

/**
 * \brief 
 * \return true, if start <= end
 */
bool Range::isRangeLeftToRight() const { return (end - start) >= 0;  }


std::vector<ResolutionInformation> ROA::UTIL::getAllResolutions()
{
	std::vector<ResolutionInformation> allResolutions;
	
	DEVMODEA m;
	m.dmSize = sizeof(DEVMODE);
	m.dmDriverExtra = 1024;

	int i = 0;
	while(EnumDisplaySettingsA(nullptr, i, &m))
	{
		i++;
		allResolutions.emplace_back(m.dmPelsWidth, m.dmPelsHeight, m.dmBitsPerPel, m.dmDisplayFrequency);
	}
	return allResolutions;
}

ResolutionInformation ROA::UTIL::getCurrentResolution()
{
	DEVMODEA m;
	m.dmSize = sizeof(DEVMODE);
	m.dmDriverExtra = 1024;

	EnumDisplaySettingsA(nullptr, ENUM_CURRENT_SETTINGS, &m);
	return {m.dmPelsWidth, m.dmPelsHeight, m.dmBitsPerPel, m.dmDisplayFrequency};
}

ResolutionInformation::ResolutionInformation(unsigned w, unsigned h, unsigned bbP, unsigned freq) : 
	width(w), height(h), bbP(bbP), frequenzy(freq)
{

}