
#define BOOST_TEST_MODULE TestSignalProcessing
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "SignalProcessing.h"


BOOST_AUTO_TEST_CASE (sign)
{

	int pos = 5;
	int posSign = sgn(pos);
	BOOST_CHECK(posSign == 1);

	int neg = -5;
	int negSign = sgn(neg);

	BOOST_CHECK(negSign == -1);

}	

BOOST_AUTO_TEST_CASE (zeroCrossingRate)
{

	float exampleFrame [10] = {-1, 1, 1, -1, 1, 1, -1, 1, 1, -1};
	int frameSize = 10;
	float *ptr = exampleFrame;

	double zcr = Signal::zeroCrossingRate(ptr, frameSize);
	BOOST_CHECK(zcr == 0.6);
	
}	

