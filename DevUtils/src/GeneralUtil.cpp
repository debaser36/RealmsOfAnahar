#include "pch.h"
#include "GeneralUtil.h"

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
