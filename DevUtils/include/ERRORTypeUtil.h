#pragma once

namespace ROA
{
	namespace DEBUG
	{
		enum class e_ReturnCode
		{
			OK = 0,
			PTR_INVALID_TYPE = 20,
			PTR_INVALID_FUN = 21,
			ID_NOT_FOUND = 40,
			WARNING_VEC_CAPACITY = 80,
			VEC_INDEX = 81,
			/**
			 * \brief add new returnCodes here
			*/
			UNKNOWN = 99
		};
		typedef e_ReturnCode RETURNCODE;

		// ReSharper disable once CppInconsistentNaming
		static void PRINTERRORMESSAGE(RETURNCODE code, std::ofstream &file);
	}
	
}

