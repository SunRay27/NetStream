#include "pch.h"
#include "CppUnitTest.h"
#include "../NetStream/FlowNetwork.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FlowStreamTests
{
	TEST_CLASS(FlowStreamTests)
	{
		
	public:
		TEST_METHOD(TestNegativeWeights)
		{
			List<String> input({"S O -3","S P 3","O Q 3","O P 2","P R 2","Q R -4","Q T 2","R T -3"});
			try
			{
				FlowNetwork network(input);
				network.GetMaxFlow();
				Assert::Fail();
			}
			catch (exception e)
			{
			
			}

		}
		TEST_METHOD(TestParseTokenMismatch)
		{
			List<String> input({ "S      P    3","S O 3 S O 3","O Q 3S O 3","O Q 3S O 3","O Q 3" });
			try
			{
				FlowNetwork network(input);
				network.GetMaxFlow();
				Assert::Fail();
			}
			catch (exception e)
			{

			}
		}
		TEST_METHOD(TestParseMaxFlow)
		{
			List<String> input({ "S O KL" });
			try
			{
				FlowNetwork network(input);
				network.GetMaxFlow();
				Assert::Fail();
			}
			catch (exception e)
			{

			}
		}
		TEST_METHOD(TestRepeatingValues)
		{
			List<String> input({ "S O 3","S P 3","O Q 3" ,"O P 2" ,"P R 2" ,"Q R 4" ,"Q T 2" ,"R T 3","S O 3","S P 3","O Q 3" ,"O P 2" ,"P R 2" ,"Q R 4" ,"Q T 2" ,"R T 3" });
			FlowNetwork network(input);
			Assert::AreEqual(5l, network.GetMaxFlow());
		}
		TEST_METHOD(TestMultipleDirections)
		{
			List<String> input({ "S 2 20","S 3 16","S 4 5" ,"2 4 14" ,"2 T 7","3 T 24" ,"3 4 7" ,"4 T 11" ,"4 3 17" });
			FlowNetwork network(input);
			Assert::AreEqual(41l, network.GetMaxFlow());
		}
		TEST_METHOD(TestSingleDirection)
		{
			List<String> input({ "S 2 7","S 3 11","S 4 5","S 6 5","S 7 5","S 8 11","S 21 1","2 3 5","3 9 15","4 12 11","5 10 17","6 7 5","7 12 11","8 9 5","9 5 15","10 13 17","10 16 18","12 10 20","12 16 14","12 20 17","13 T 27","16 13 14","16 20 11","20 T 22","21 T 14"});
			FlowNetwork network(input);
			Assert::AreEqual(31l, network.GetMaxFlow());
		}
	};
}
