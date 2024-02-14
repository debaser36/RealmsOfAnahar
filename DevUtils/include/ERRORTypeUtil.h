#pragma once

namespace ROA
{
	namespace DEBUG
	{
		enum class e_ReturnCode
		{
			OK = 0,
			OK_WITH_BINWARNINGS = 1,
			OK_WITH_REFERENCEWARNING = 2,
			NULLERROR = 10,
			PTR_INVALID_TYPE = 20,
			PTR_INVALID_FUN = 21,
			ID_NOT_FOUND = 40,
			BIN_VALIDATION_ERROR = 50,
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

