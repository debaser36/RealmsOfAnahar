#include "pch.h"
#include "ERRORTypeUtil.h"
// ReSharper disable once CppInconsistentNaming
void ROA::DEBUG::PRINTERRORMESSAGE(RETURNCODE code, std::ofstream& file) {
	if(!file.is_open()) return;
	file << "Code " << static_cast<unsigned int>(code) << ": ";
	switch(code)
	{
	case e_ReturnCode::OK: file << "EVERYTHING OK!"; break;
	case e_ReturnCode::UNKNOWN: file << "UNKNOWN ERROR!"; break;
	case e_ReturnCode::PTR_INVALID_FUN: file << "ERROR: UNKNOWN FUNCTION POINTER!"; break;
	case e_ReturnCode::PTR_INVALID_TYPE: file << "ERROR: UNKNOWN TYPE POINTER!"; break;
	case e_ReturnCode::WARNING_VEC_CAPACITY: file << "WARNING: Vector needs Reallocation. References may be mixed!"; break;
	case e_ReturnCode::ID_NOT_FOUND: file << "ERROR: ID NOT FOUND!"; break;
	case e_ReturnCode::VEC_INDEX: file << "ERROR: INDEX NOT VALID!"; break;
	case e_ReturnCode::OK_WITH_BINWARNINGS: file << "OK, BUT BINARY WARNINGS!"; break;
	case e_ReturnCode::OK_WITH_REFERENCEWARNING: file << "OK, BUT REFERENCE WARNINGS! ";break;
	case e_ReturnCode::NULLERROR: file << "ERROR: NULL ERROR";break;
	case e_ReturnCode::BIN_VALIDATION_ERROR: file << "ERROR: BINARY VALIDATION FAILED"; break;
	default: file << "INTERNAL ERROR: CODE STRING NOT IMPLEMENTED!";  // NOLINT(clang-diagnostic-covered-switch-default)
	}
	file << "\n";
}

void ROA::DEBUG::PRINTERRORMESSAGE(RETURNCODE code, FILE * stream) {
	std::ofstream file = std::ofstream(stream);
	ROA::DEBUG::PRINTERRORMESSAGE(code, file);
}
