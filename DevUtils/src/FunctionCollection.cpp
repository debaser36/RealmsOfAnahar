#include "pch.h"
#include "FunctionCollection.h"

#include "ERRORTypeUtil.h"

using namespace ROA;
using namespace ROA::UTIL;

CallableFunction::CallableFunction(std::string id, DEBUG::e_ReturnCode(* fun)(std::vector<void*>&)) : m_id(std::move(id)), m_fun(fun) {}

bool CallableFunction::operator==(const CallableFunction& other) const
{
	return (this->m_id == other.m_id);
}

bool CallableFunction::operator!=(const CallableFunction& other) const { return !(*this == other); }

DEBUG::e_ReturnCode CallableFunction::operator()(std::vector<void*>& args) const
{
	if(this->m_fun) return this->m_fun(args);
	return DEBUG::e_ReturnCode::PTR_INVALID_FUN;
}

FunctionCollection::FunctionCollection() :  m_functions(std::vector<CallableFunction>()) {}

FunctionCollection::FunctionCollection(const unsigned long long reserveAmount): m_functions(std::vector<CallableFunction>())
{
	m_functions.reserve(reserveAmount);
}

FunctionCollection::FunctionCollection(const std::vector<CallableFunction>& functions): m_functions(functions) {}

DEBUG::RETURNCODE FunctionCollection::addFunction(CallableFunction& fun)
{
	DEBUG::e_ReturnCode retMe = DEBUG::e_ReturnCode::OK;
	if(this->m_functions.size() == this->m_functions.capacity()) retMe =DEBUG::e_ReturnCode::WARNING_VEC_CAPACITY;

	this->m_functions.emplace_back(fun);
	return retMe;
}

DEBUG::RETURNCODE FunctionCollection::addFunction(const std::string& id, DEBUG::RETURNCODE(* fun)(std::vector<void*>&))
{
	if(!fun) return DEBUG::RETURNCODE::PTR_INVALID_FUN;
	auto addMe = CallableFunction(id, fun);
	this->addFunction(addMe);
	return DEBUG::e_ReturnCode::OK;
}

/**
 * \brief tries to find the function in the collection
 * \param id the function Name to find
 * \return nullptr, if not found, else reference to the Element
 */
const CallableFunction * FunctionCollection::findFunction(const std::string& id) const
{
	for (auto& m_function : this->m_functions)
	{
		if(m_function.m_id == id) return &m_function;
	}
	return nullptr;
}

/**
 * \brief preferred way of calling a function
 * \param id the name of the function to be called
 * \param args arguments to call the function with
 * \return whatever the function returns, or ID_NOT_FOUND Error
 */
DEBUG::e_ReturnCode FunctionCollection::operator()(const std::string& id, std::vector<void*>& args) const
{
	const auto fun = this->findFunction(id);
	if(!fun) return DEBUG::RETURNCODE::ID_NOT_FOUND;
	return (*fun)(args);
}

/**
 * \brief Use with caution! Instead find with ID!
 * \param index the index of the array
 * \param args arguments to call the function with
 * \returns whatever the function returns, or VEC_INDEX Error!
 */
DEBUG::e_ReturnCode FunctionCollection::operator()(const unsigned index, std::vector<void*>& args) const
{
	if(index >= this->m_functions.size()) return DEBUG::e_ReturnCode::VEC_INDEX;
	return  this->m_functions[index](args);
}

unsigned long long FunctionCollection::size() const
{ 
	return this->m_functions.size();
}
