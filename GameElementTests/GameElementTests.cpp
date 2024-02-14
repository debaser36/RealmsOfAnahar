#include "pch.h"
#include "CppUnitTest.h"
#include "../GameElements/include/GameElement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ROA;

#define BUFFER_SIZE 10000000
static char * DEBUG_BUFFER;

#define LOG(...) memset(DEBUG_BUFFER, 0, BUFFER_SIZE); sprintf_s(DEBUG_BUFFER, BUFFER_SIZE, __VA_ARGS__); (Logger::WriteMessage(DEBUG_BUFFER))




namespace GameElementTests
{
	TEST_CLASS(TestsBasic)
	{

	public:
			ROA::GameElement gameElement;
			ROA::Building building;
			ROA::Unit unit;

			
		TEST_CLASS_INITIALIZE(Init)
		{
			DEBUG_BUFFER = static_cast<char*>(malloc(BUFFER_SIZE * sizeof(char)));
			memset(DEBUG_BUFFER, 0, BUFFER_SIZE);
			
		}
		
		TEST_METHOD(ValidInheritanceOffsets)
		{
			LOG("Adresse Start: %p, Adresse Unit: %p", &unit, unit.getAddrOfInheritanceStart());
			Assert::IsTrue(unit.getAddrOfInheritanceStart() > (&unit));
		}

		TEST_CLASS_CLEANUP(cleanup)
		{
			free(DEBUG_BUFFER);
		}
	};
}
