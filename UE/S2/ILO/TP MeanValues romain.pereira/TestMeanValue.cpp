/*
 * TestMeanValue.cpp
 *
 *  Created on: 6 avr. 2016
 *      Author: davidroussel
 */

#include "MeanValue.h"

#include <csignal>	// for signal
#include <sstream>	// for std::[i/o]stringstream
#include <string>	// for std::string
#include <random>	// for random numbers dist. & generator
#include <functional> // for function template and bind
#include <numeric> // for numeric algos
#include <limits>	// for numeric limits
#include <typeinfo>	// for typeid
#include <cmath>	// for sqrt
using namespace std;

#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/test/output_test_stream.hpp>
using boost::test_tools::output_test_stream;
#include <boost/mpl/list.hpp>
using namespace boost::unit_test;

int maxVal = numeric_limits<int>::max() / 10;
int minVal = maxVal / 10;

/**
 * Number of values to accumulate
 */
size_t nbtests = 1000000;

/**
 * Global tolerance for close numbers (in %)
 */
double globalTolerance = 0.3;

#include <cxxabi.h>	// for demangling names

/**
 * Demangle typeid's names
 * @param mangledName the mangled name
 * @return the demangled name
 */
string demangle(const char * mangledName)
{
	int status;
	string result;
	char * demangledName = abi::__cxa_demangle(mangledName, nullptr, 0, &status);
	if (status == 0)
	{
		result = demangledName;
	}
	free(demangledName);
	return result;
}

/**
 * Function template to accumulate "nb" values of type T in the range
 * [min...max] into the mean value "mv" using a random number generator
 * @param mv the mean value to accumulate values into
 * @param nb the number of values to accumulate
 * @param min the minimum value for the random number generator
 * @param max the maximum value for the random number generator
 * @param useIncrement if true then use operator += of MeanValue to accumulate
 * values into mv, otherwise use operator () to accumulate values into mv.
 */
template<typename T, typename R>
void accumulate(MeanValue<T, R> & mv,
                const size_t nb,
                const T & min,
                const T & max,
                const bool useIncrement = false)
{
	void (MeanValue<T, R>::*accum)(const T &) = NULL;
	if (useIncrement)
	{
		accum = &MeanValue<T, R>::operator +=;
	}
	else
	{
		accum = &MeanValue<T, R>::operator ();
	}
	default_random_engine engine;
	function<T(void)> random;
	if (typeid(T) == typeid(int))
	{
		random = bind(uniform_int_distribution<int>(min, max), engine);
	}
	else if (typeid(T) == typeid(float))
	{
		random = bind(uniform_real_distribution<float>(min, max), engine);
	}
	else if (typeid(T) == typeid(double))
	{
		random = bind(uniform_real_distribution<double>(min, max), engine);
	}
	else
	{
		string dname = demangle(typeid(T).name());
		cerr << "accumulate(...) : Unknown value type" << dname
		     << endl;
	}

	if (accum != NULL)
	{
		for (size_t i = 0; i < nb; ++i)
		{
			(mv.*accum)(random());
		}
	}
}

/**
 * Default constructor test
 * @see MeanValue<T>::MeanValue()
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( DefaultConstructor_, T )
{
	MeanValue<T> mv;

	BOOST_CHECK_EQUAL(mv.count(), 0L);
	BOOST_CHECK_EQUAL(mv.mean(), 0.0);
	BOOST_CHECK_EQUAL(mv.std(), 0.0);
	BOOST_CHECK_EQUAL(mv.min(), T(0));
	BOOST_CHECK_EQUAL(mv.max(), T(0));
}

/**
 * Copy constructor test
 * @see MeanValue<T>::MeanValue(const MeanValue<T,R> & m)
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( CopyConstructor_, T )
{
	MeanValue<T> mv;
	MeanValue<T> mv2(mv);

	BOOST_CHECK_EQUAL(mv.count(), mv2.count());
	BOOST_CHECK_EQUAL(mv.mean(), mv2.mean());
	BOOST_CHECK_EQUAL(mv.std(), mv2.std());
	BOOST_CHECK_EQUAL(mv.min(), mv2.min());
	BOOST_CHECK_EQUAL(mv.max(), mv2.max());

	T max = T(maxVal);
	T min = T(minVal);
	accumulate(mv, nbtests, min, max);

	MeanValue<T> mv3(mv);

	BOOST_CHECK_EQUAL(mv.count(), mv3.count());
	BOOST_CHECK_EQUAL(mv.mean(), mv3.mean());
	BOOST_CHECK_EQUAL(mv.std(), mv3.std());
	BOOST_CHECK_EQUAL(mv.min(), mv3.min());
	BOOST_CHECK_EQUAL(mv.max(), mv3.max());
}

/**
 * Move constructor test
 * @see MeanValue<T>::MeanValue(MeanValue<T,R> && m)
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( MoveConstructor_, T )
{
	MeanValue<T> mv;

	T max = T(numeric_limits<int>::max()) / T(10);
	T min = max / T(2);
	accumulate(mv, nbtests, min, max);

	double mvMean = mv.mean();
	double mvStd = mv.std();
	double mvMin = mv.min();
	double mvMax = mv.max();
	long mvCount = mv.count();

	MeanValue<T> mv2(std::move(mv));

	BOOST_CHECK_EQUAL(mv.count(), 0);
	BOOST_CHECK_EQUAL(mv.mean(), 0.0);
	BOOST_CHECK_EQUAL(mv.std(), 0.0);
	BOOST_CHECK_EQUAL(mv.min(), T(0));
	BOOST_CHECK_EQUAL(mv.max(), T(0));

	BOOST_CHECK_EQUAL(mvCount, mv2.count());
	BOOST_CHECK_EQUAL(mvMean, mv2.mean());
	BOOST_CHECK_EQUAL(mvStd, mv2.std());
	BOOST_CHECK_EQUAL(mvMin, mv2.min());
	BOOST_CHECK_EQUAL(mvMax, mv2.max());
}

/**
 * Constant MeanValue<T> factory method test
 * @see MeanValue<T>::get(const R & mean,
 *                        const R & std,
 *                        const T & min,
 *                        const T & max)
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( get_, T )
{
	// 60 ± 28.8675 [10...110]
	double meanValue = 60;
	double stdValue = 28.8675;
	T minValue = T(10);
	T maxValue = T(110);
	long countValue = -1;

	const MeanValue<T> cmv = MeanValue<T>::get(meanValue,
	                                           stdValue,
	                                           minValue,
	                                           maxValue);

	BOOST_CHECK_EQUAL(cmv.mean(), meanValue);
	BOOST_CHECK_CLOSE(cmv.std(), stdValue, globalTolerance);
	BOOST_CHECK_EQUAL(cmv.min(), minValue);
	BOOST_CHECK_EQUAL(cmv.max(), maxValue);
	BOOST_CHECK_EQUAL(cmv.count(), countValue);
}

/**
 * Mean method test
 * @see MeanValue<T>::mean()
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( mean_, T )
{
	MeanValue<T> mv;

	T max = T(maxVal);
	T min = T(minVal);
	const double expectedMean = double(min + max) / 2.0;

	/*
	 * Before accumulation mean == 0
	 */
	BOOST_CHECK_EQUAL(mv.mean(), 0.0);

	accumulate(mv, nbtests, min, max);

	/*
	 * Check difference between expected value and accumulated mean value
	 * which should be less than tolerance %
	 */
	BOOST_CHECK_CLOSE(mv.mean(), expectedMean, globalTolerance);
}

/**
 * Std method test
 * @see MeanValue<T>::std()
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( std_, T )
{
	MeanValue<T> mv;

	T max = T(maxVal);
	T min = T(minVal);
	const double range = double(max - min);
	const double expectedStd = sqrt((range * range) / 12.0);

	/*
	 * Before accumulation std == 0
	 */
	BOOST_CHECK_EQUAL(mv.std(), 0.0);

	accumulate(mv, nbtests, min, max);

	/*
	 * Check difference between expected value and accumulated mean value
	 * which should be less than tolerance %
	 */
	BOOST_CHECK_CLOSE(mv.std(), expectedStd, globalTolerance);
}

/**
 * Count method test
 * @see MeanValue<T>::count()
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( count_, T )
{
	MeanValue<T> mv;

	T max = T(maxVal);
	T min = T(minVal);

	/*
	 * Before accumulation count == 0
	 */
	BOOST_CHECK_EQUAL(mv.count(), 0);

	accumulate(mv, nbtests, min, max);

	/*
	 * After accumulation count == nbtests
	 */
	BOOST_CHECK(mv.count() == (long)nbtests);
}

/**
 * Min method test
 * @see MeanValue<T>::min()
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( min_, T )
{
	MeanValue<T> mv;

	T max = T(maxVal);
	T min = T(minVal);

	/*
	 * Before accumulation min == 0
	 */
	BOOST_CHECK_EQUAL(mv.min(), T(0));

	accumulate(mv, nbtests, min, max);

	/*
	 * After accumulation min() == min
	 */
	BOOST_CHECK_CLOSE(double(mv.min()), double(min), globalTolerance);
}

/**
 * Max method test
 * @see MeanValue<T>::max()
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( max_, T )
{
	MeanValue<T> mv;

	T max = T(maxVal);
	T min = T(minVal);

	/*
	 * Before accumulation max == 0
	 */
	BOOST_CHECK_EQUAL(mv.max(), 0);

	accumulate(mv, nbtests, min, max);

	/*
	 * After accumulation max() == max
	 */
	BOOST_CHECK_CLOSE(double(mv.max()), double(max), globalTolerance);
}

/**
 * Reset method test
 * @see MeanValue<T>::reset()
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( reset_, T )
{
	MeanValue<T> mv;

	T max = T(maxVal);
	T min = T(minVal);

	accumulate(mv, nbtests, min, max);

	BOOST_CHECK_NE(mv.mean(), 0.0);
	BOOST_CHECK_NE(mv.std(), 0.0);
	BOOST_CHECK_NE(mv.min(), numeric_limits<T>::max());
	BOOST_CHECK_NE(mv.max(), numeric_limits<T>::lowest());
	BOOST_CHECK_EQUAL(mv.count(), (long)nbtests);

	mv.reset();

	BOOST_CHECK_EQUAL(mv.count(), 0);
	BOOST_CHECK_EQUAL(mv.mean(), 0.0);
	BOOST_CHECK_EQUAL(mv.std(), 0.0);
	BOOST_CHECK_EQUAL(mv.min(), T(0));
	BOOST_CHECK_EQUAL(mv.max(), T(0));
}

/**
 * Function call (parentheses) operator test
 * @see MeanValue<T>::operator ()(const T &)
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( parenthesesOperator_, T )
{
	MeanValue<T> mv;
	size_t nb = 1000;

	T max = T(maxVal);
	T min = T(minVal);

	default_random_engine engine;
	function<T(void)> random;
	bool ready = true;
	if (typeid(T) == typeid(int))
	{
		random = bind(uniform_int_distribution<int>(min, max), engine);
	}
	else if (typeid(T) == typeid(float))
	{
		random = bind(uniform_real_distribution<float>(min, max), engine);
	}
	else if (typeid(T) == typeid(double))
	{
		random = bind(uniform_real_distribution<double>(min, max), engine);
	}
	else
	{
		ready = false;
	}

	if (ready)
	{
		double a = 0.0;
		double b = 0.0;
		double c = 0.0;
		T d = numeric_limits<T>::max();
		T e = numeric_limits<T>::lowest();

		for(size_t i = 0; i < nb; ++i)
		{
			T f = random();
			double g = double(f);
			double j = double(i+1);
			a = ((a * double(i)) + g) / j;
			b = ((b * double(i)) + (g * g)) / j;
			c = sqrt(abs(b - (a * a)));

			if (f < d)
			{
				d = f;
			}
			if (f > e)
			{
				e = f;
			}

			mv(f);

			BOOST_CHECK_CLOSE(mv.mean(), a, globalTolerance);
			BOOST_CHECK_CLOSE(mv.std(), c, globalTolerance);
			BOOST_CHECK_CLOSE(double(mv.min()), double(d), globalTolerance);
			BOOST_CHECK_CLOSE(double(mv.max()), double(e), globalTolerance);
			BOOST_CHECK_EQUAL(mv.count(), (long)(i+1));
		}
	}
	else
	{
		BOOST_FAIL("unknown value type");
	}
}

/**
 * Function call (parentheses) operator test
 * @see MeanValue<T>::operator ()(const T & value, const U & ... others)
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( multiParenthesesOperator_, T )
{
	double expectedMean = 3.0;
	double expectedStd = sqrt(2.0);

	MeanValue<T> mv;

	mv(T(1), T(2), T(3), T(4), T(5));

	BOOST_CHECK_CLOSE(mv.mean(), expectedMean, globalTolerance);
	BOOST_CHECK_CLOSE(mv.std(), expectedStd, globalTolerance);
	BOOST_CHECK_EQUAL(mv.min(), T(1));
	BOOST_CHECK_EQUAL(mv.max(), T(5));
	BOOST_CHECK_EQUAL(mv.count(), (long)(5));
}

/**
 * Self increment operator test
 * @note should provide the same results as the parentheses operator
 * @see MeanValue<T>::operator +=(const T &)
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( selfIncrementOperator_, T )
{
	MeanValue<T> mv;

	T max = T(maxVal);
	T min = T(minVal);

	size_t nb = 1000;

	default_random_engine engine;
	function<T(void)> random;
	bool ready = true;
	if (typeid(T) == typeid(int))
	{
		random = bind(uniform_int_distribution<int>(min, max), engine);
	}
	else if (typeid(T) == typeid(float))
	{
		random = bind(uniform_real_distribution<float>(min, max), engine);
	}
	else if (typeid(T) == typeid(double))
	{
		random = bind(uniform_real_distribution<double>(min, max), engine);
	}
	else
	{
		ready = false;
	}

	if (ready)
	{
		double a = 0.0;
		double b = 0.0;
		double c = 0.0;
		T d = numeric_limits<T>::max();
		T e = numeric_limits<T>::lowest();

		for(size_t i = 0; i < nb; ++i)
		{
			T f = random();
			double g = double(f);
			double j = double(i+1);
			a = ((a * double(i)) + g) / j;
			b = ((b * double(i)) + (g * g)) / j;
			c = sqrt(abs(b - (a * a)));

			if (f < d)
			{
				d = f;
			}
			if (f > e)
			{
				e = f;
			}

			mv += f;

			BOOST_CHECK_CLOSE(mv.mean(), a, globalTolerance);
			BOOST_CHECK_CLOSE(mv.std(), c, globalTolerance);
			BOOST_CHECK_CLOSE(double(mv.min()), double(d), globalTolerance);
			BOOST_CHECK_CLOSE(double(mv.max()), double(e), globalTolerance);
			BOOST_CHECK_EQUAL(mv.count(), (long)(i+1));
		}
	}
	else
	{
		BOOST_FAIL("unknown value type");
	}
}

/**
 * Copy operator test
 * @see MeanValue<T>::operator =(const MeanValue<T> & m)
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( copyOperator_, T )
{
	MeanValue<T> mv;
	MeanValue<T> mv2;

	T max = T(maxVal);
	T min = T(minVal);

	accumulate(mv, nbtests, min, max);

	BOOST_CHECK_NE(mv.mean(), mv2.mean());
	BOOST_CHECK_NE(mv.std(), mv2.std());
	BOOST_CHECK_NE(mv.min(), mv2.min());
	BOOST_CHECK_NE(mv.max(), mv2.max());
	BOOST_CHECK_NE(mv.count(), mv2.count());

	mv2 = mv;

	BOOST_CHECK_CLOSE(mv.mean(), mv2.mean(), globalTolerance);
	BOOST_CHECK_CLOSE(mv.std(), mv2.std(), globalTolerance);
	BOOST_CHECK_EQUAL(mv.min(), mv2.min());
	BOOST_CHECK_EQUAL(mv.max(), mv2.max());
	BOOST_CHECK_EQUAL(mv.count(), mv2.count());
}

/**
 * Move operator test
 * @see MeanValue<T>::operator =(MeanValue<T> && m)
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( moveOperator_, T )
{
	MeanValue<T> mv;
	MeanValue<T> mv2;

	T max = T(maxVal);
	T min = T(minVal);

	accumulate(mv, nbtests, min, max);

	double meanValue = mv.mean();
	double stdValue = mv.std();
	T minValue = mv.min();
	T maxValue = mv.max();
	long countValue = mv.count();

	BOOST_CHECK_NE(mv2.mean(), meanValue);
	BOOST_CHECK_NE(mv2.std(), stdValue);
	BOOST_CHECK_NE(mv2.min(), minValue);
	BOOST_CHECK_NE(mv2.max(), maxValue);
	BOOST_CHECK_NE(mv2.count(), countValue);

	mv2 = std::move(mv);

	BOOST_CHECK_CLOSE(mv2.mean(), meanValue, globalTolerance);
	BOOST_CHECK_CLOSE(mv2.std(), stdValue, globalTolerance);
	BOOST_CHECK_EQUAL(mv2.min(), minValue);
	BOOST_CHECK_EQUAL(mv2.max(), maxValue);
	BOOST_CHECK_EQUAL(mv2.count(), countValue);

	BOOST_CHECK_EQUAL(mv.mean(), 0.0);
	BOOST_CHECK_EQUAL(mv.std(), 0.0);
	BOOST_CHECK_EQUAL(mv.min(), T(0));
	BOOST_CHECK_EQUAL(mv.max(), T(0));
	BOOST_CHECK_EQUAL(mv.count(), 0);
}

/**
 * Cast operator (convert to R as mean value) test
 * @see MeanValue<T>::operator R() const
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( castOperator_, T )
{
	MeanValue<T> mv;

	T max = T(maxVal);
	T min = T(minVal);

	BOOST_CHECK_EQUAL((double)mv, mv.mean());

	accumulate(mv, nbtests, min, max);

	BOOST_CHECK_EQUAL((double)mv, mv.mean());
}

/**
 * Compare operator test
 * @see MeanValue<T>::operator ==(const MeanValue<T> &) const
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( equalityOperator_, T )
{
	MeanValue<T> mv;
	MeanValue<T> mv2;

	BOOST_CHECK(mv == mv2);

	T max = T(maxVal);
	T min = T(minVal);
	accumulate(mv, nbtests, min, max);

	BOOST_CHECK(!(mv == mv2));

	const double expectedMean = double(min + max) / 2.0;
	const double range = double(max - min);
	const double expectedStd = sqrt((range * range) / 12.0);
	const MeanValue<T> mvr = MeanValue<T>::get(expectedMean,
	                                           expectedStd,
	                                           min,
	                                           max);

	BOOST_CHECK(mv == mvr);

	mv2 = mv;

	BOOST_CHECK(mv == mv2);
}

/**
 * Difference operator test
 * @see MeanValue<T>::operator !=(const MeanValue<T> &) const
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( differenceOperator_, T )
{
	MeanValue<T> mv;
	MeanValue<T> mv2;

	BOOST_CHECK(!(mv != mv2));

	T max = T(maxVal);
	T min = T(minVal);
	accumulate(mv, nbtests, min, max);

	BOOST_CHECK(mv != mv2);

	const double expectedMean = double(min + max) / 2.0;
	const double range = double(max - min);
	const double expectedStd = sqrt((range * range) / 12.0);
	const MeanValue<T> mvr = MeanValue<T>::get(expectedMean,
		expectedStd,
		min,
		max);

	BOOST_CHECK(!(mv != mvr));

	mv2 = mv;

	BOOST_CHECK(!(mv != mv2));
}

/**
 * Output operator test
 * @see operator <<(ostream & out, const MeanValue<T, R> & mv)
 */
BOOST_TEST_CASE_TEMPLATE_FUNCTION( outputOperator_, T )
{
	MeanValue<T> mv;

	T max = T(maxVal);
	T min = T(minVal);
	accumulate(mv, nbtests, min, max);

	ostringstream oss;
	oss << mv.mean() << " ± " << mv.std() << " [" << mv.min() << "..."
	    << mv.max() << "](" << mv.count() << ")";
	string stringOutput = oss.str();

	output_test_stream output;
	output << mv;

	BOOST_CHECK(output.check_length(stringOutput.length(), false));
	BOOST_CHECK(output.is_equal(stringOutput));


}
// ----------------------------------------------------------------------------
// Test suite
// ----------------------------------------------------------------------------

/**
 * Tests suite initialisation
 * @return 0
 */
test_suite* init_unit_test_suite(int, char**)
{
	// types to test
	typedef boost::mpl::list<int, float, double> test_types;

	// MeanValue<T> test set
	test_suite * cts = BOOST_TEST_SUITE("MeanValue<T>");
	cts->add(BOOST_TEST_CASE_TEMPLATE(DefaultConstructor_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(CopyConstructor_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(MoveConstructor_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(get_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(mean_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(std_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(count_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(min_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(max_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(reset_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(parenthesesOperator_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(multiParenthesesOperator_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(selfIncrementOperator_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(copyOperator_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(moveOperator_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(equalityOperator_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(differenceOperator_, test_types));
	cts->add(BOOST_TEST_CASE_TEMPLATE(outputOperator_, test_types));

	framework::master_test_suite().add(cts);

	return 0;
}
