#pragma once
#include "ERRORTypeUtil.h"

namespace ROA::DEBUG {
	enum class e_ReturnCode;
}

namespace ROA::UTIL
{
	class CallableFunction
	{
		friend class FunctionCollection;


		std::string m_id;
		DEBUG::e_ReturnCode (*m_fun)(std::vector<void*>&);

	public:
		/**
			 * \brief Please tell us more Information about the functions
			 */
		CallableFunction() = delete;
		CallableFunction(std::string id, DEBUG::e_ReturnCode(* fun)(std::vector<void*>&));

		bool operator==(const CallableFunction& other) const;
		bool operator!=(const CallableFunction& other) const;

		DEBUG::e_ReturnCode operator()(std::vector<void*>& args) const;
	};

	class FunctionCollection
	{
		friend class CallableFunction;
		std::vector<CallableFunction> m_functions;

	public:
		FunctionCollection();
		explicit FunctionCollection(unsigned long long reserveAmount);
		explicit FunctionCollection(const std::vector<CallableFunction>& functions);
		DEBUG::RETURNCODE addFunction(CallableFunction& fun);
		DEBUG::RETURNCODE addFunction(const std::string& id, DEBUG::RETURNCODE(* fun)(std::vector<void*>&));

		[[nodiscard]] const CallableFunction * findFunction(const std::string& id) const;

		DEBUG::e_ReturnCode operator()(const std::string& id, std::vector<void*>& args) const;
		DEBUG::e_ReturnCode operator()(const unsigned index, std::vector<void*>& args) const;

		[[nodiscard]] unsigned long long size() const;
	};
}


