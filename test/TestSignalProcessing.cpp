
#define BOOST_TEST_MODULE TestSignalProcessing
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "SignalProcessing.h"
#include <iostream>


BOOST_AUTO_TEST_CASE (zeroCrossingRate)
{

	float exampleFrame [8] = {-1, 1, -1, 1, -1, 1, -1, 1};
	int frameSize = 8;
	float *ptr = exampleFrame;

	std::cout << Signal::zeroCrossingRate(ptr, frameSize);

	BOOST_CHECK(Signal::zeroCrossingRate(ptr, frameSize) == 1);
}	

