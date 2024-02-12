#include "pch.h"
#include "GameElement.h"

ROA::GameElementEffect::GameElementEffect(std::string id, const bool permanent, const unsigned int rounds, const UTIL::FunctionCollection& functions) :
	m_id(std::move(id))
{
	m_effectType.effectType = e_GameEffectType::functionBased;
	m_effectType.b_isPermanent = permanent;
	m_effectType.functionsToCall = functions;
	m_effectType.roundsLeft = rounds;
}

ROA::GameElementEffect::GameElementEffect(std::string& id, const bool permanent, const unsigned rounds, const bool absolutValues,
                                          const std::vector<e_GameElementValues>& effectedValues_Names, const std::vector<unsigned char>& effectedValues_Values) : m_id(std::move(id))
{
	m_effectType.effectType = absolutValues ? e_GameEffectType::valueBasedAbsolut : e_GameEffectType::valueBasedPercentage;
	m_effectType.roundsLeft = rounds;
	m_effectType.b_isPermanent = permanent;
	m_effectType.effectedValues_Names = effectedValues_Names;
	m_effectType.effectedValues_Values = effectedValues_Values;
}

bool ROA::GameElementEffect::operator==(const GameElementEffect& other) const
{
	return this->m_id == other.m_id;
}

bool ROA::GameElementEffect::operator!=(const GameElementEffect& other) const
{
	return !(*this == other);
}

const std::string& ROA::GameElementEffect::id()
{
	return this->m_id;
}

ROA::GameElement::GameElement() : m_id("INVALID"), m_name("NONAME"), m_description("NODESCRIPTION"),
                                  m_type(e_GameElementTypes::UNKNOWN) {}
ROA::GameElement::GameElement(std::string id) : m_id(std::move(id)), m_type(e_GameElementTypes::UNKNOWN) {}

const std::string& ROA::GameElement::id() { return this->m_id; }
ROA::e_GameElementTypes ROA::GameElement::type() const { return this->m_type; }
ROA::UnitStats::UnitStats()
{
	memset(this, 0, sizeof(UnitStats));
}
