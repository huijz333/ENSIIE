/*
 * main.cpp
 *
 *  Created on: 20 mars 2016
 *      Author: davidroussel
 */

#include <cstdlib>	// for EXIT_SUCCESS
#include <cassert>

#include <iostream>	// for cout, cerr, clog
#include <string>	// for strings
#include <sstream>	// for ostringstream
#include <typeinfo>	// for typeid
#include <random>	// for random numbers dist. & generator
#include <functional> // for function template and bind
#include <limits>	// for numeric limits
#include <typeinfo>	// for typeid
using namespace std;

#include <cxxabi.h>	// for demangling names
string demangle(const char * mangledName)
{
	int status;
	string result;
	char * demangledName = abi::__cxa_demangle(mangledName, nullptr, 0, &status);
	if (status == 0)
	{
		result = demangledName;
	}
	return result;
}

#include "MeanValue.h"

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
 * Main program for MeanValue<T>
 * @param argc argument count
 * @param argv arguments
 * @par Usage :
 * main -min <minvalue> -max <maxvalue> -nb <number of iterations>
 * @return EXIT_SUCCESS if everything went well, any other value means failure.
 */
int main(int argc, char ** argv)
{
	int maxValue = numeric_limits<int>::max() / 10;
	int minValue = maxValue / 10;
	long nbtests = 10000;

	// use arguments if any
	if (argc >= 2)
	{
		for (int i = 1; i < argc; ++i)
		{
			if (argv[i][0] == '-') // there might be a command
			{
				string argString(argv[i]);
				long value = 0;
				bool valueRead = false;
				if (i < argc)
				{
					// next argument should be a value
					string valueString(argv[i + 1]);
					try
					{
						value = stol(valueString);
						valueRead = true;
					}
					catch (invalid_argument & e)
					{
						cerr << e.what() << ": invalid argument value after "
						     << argString << endl;
					}

					if (valueRead)
					{
						if (argString == "-min")
						{
							minValue = value;
						}
						else if (argString == "-max")
						{
							maxValue = value;
						}
						else if (argString == "-nb")
						{
							if (value >= 10000)
							{
								nbtests = value;
							}
							else
							{
								cerr << "number of tests should be at least 10000"
									 << endl;
							}
						}
						else
						{
							cerr << "Invalid argument " << argString
							     << " encountered" << endl;
						}
						i++;
					}
				}
				else
				{
					cerr << "No further argument after" << argString << endl;
				}
			}
		}
	}

	MeanValue<int, double> mvi;
	MeanValue<float, double> mvf;
	MeanValue<double, double> mvd;

	// ------------------------------------------------------------------------
	// Test Default constructors
	// ------------------------------------------------------------------------
	assert(mvi.mean() == 0.0);
	assert(mvi.std() == 0.0);
	assert(mvi.count() == 0);
	assert(mvi.min() == 0);
	assert(mvi.max() == 0);
	assert(mvf.mean() == 0.0);
	assert(mvf.std() == 0.0);
	assert(mvf.count() == 0);
	assert(mvf.min() == 0.0f);
	assert(mvf.max() == 0.0f);
	assert(mvd.mean() == 0.0);
	assert(mvd.std() == 0.0);
	assert(mvd.count() == 0);
	assert(mvd.min() == 0.0);
	assert(mvd.max() == 0.0);

	// ------------------------------------------------------------------------
	// Expected mean and std values
	// ------------------------------------------------------------------------
	const double expectedMean = double(minValue + maxValue) / 2.0;
	const double range = double(maxValue - minValue);
	const double expectedStd = sqrt((range * range) / 12.0);

	// ------------------------------------------------------------------------
	// MeanValue references (for comparing with ==)
	// Test MeanValue#get and MeanValue Private constructor
	// ------------------------------------------------------------------------
	const MeanValue<int> cmvi = MeanValue<int>::get(expectedMean,
	                                                expectedStd,
	                                                minValue,
	                                                maxValue);
	const MeanValue<float> cmvf = MeanValue<float>::get(expectedMean,
	                                                    expectedStd,
	                                                    minValue,
	                                                    maxValue);
	const MeanValue<double> cmvd = MeanValue<double>::get(expectedMean,
	                                                      expectedStd,
	                                                      minValue,
	                                                      maxValue);
	assert(cmvi.mean() == expectedMean);
	assert(checkClose<double>(cmvi.std(), expectedStd, 0.0001, 0.0000001));
	assert(cmvi.min() == minValue);
	assert(cmvi.max() == maxValue);
	assert(cmvi.count() == -1);
	assert(cmvf.mean() == expectedMean);
	assert(checkClose<double>(cmvf.std(), expectedStd, 0.0001, 0.0000001));
	assert(cmvf.min() == minValue);
	assert(cmvf.max() == maxValue);
	assert(cmvf.count() == -1);
	assert(cmvd.mean() == expectedMean);
	assert(checkClose<double>(cmvd.std(), expectedStd, 0.0001, 0.0000001));
	assert(cmvd.min() == minValue);
	assert(cmvd.max() == maxValue);
	assert(cmvd.count() == -1);

	// ------------------------------------------------------------------------
	// Test Accumulate "nbtests" random values between "minValue" & "maxValue"
	// into MeanValue<?, double> with operator ()
	// ------------------------------------------------------------------------
	clock_t tic = clock();
	accumulate(mvi, (size_t)nbtests, minValue, maxValue);
	accumulate(mvf, (size_t)nbtests, (float)minValue, (float)maxValue);
	accumulate(mvd, (size_t)nbtests, (double)minValue, (double)maxValue);
	clock_t toc = clock();

	cout << "elapsed time for operator() = " << (toc - tic) / 1000 << " ms"
	     << endl;
	cout << "expected values were " << expectedMean << " ± " << expectedStd
	     << " [" << minValue << "..." << maxValue << "]" << endl;
	cout << "MeanValue<int,    double> mvi = " << mvi << endl;
	cout << "MeanValue<float,  double> mvf = " << mvf << endl;
	cout << "MeanValue<double, double> mvd = " << mvd << endl;

	// ------------------------------------------------------------------------
	// Test resulting values for :
	//	- mean
	//	- std
	//	- min
	//	- max
	//	- count
	// ------------------------------------------------------------------------
	assert(checkClose<double>(mvi.mean(), cmvi.mean(), 0.01, 1));
	assert(checkClose<double>(mvi.std(), cmvi.std(), 0.01, 1));
	assert(checkClose<double>(mvi.min(), cmvi.min(), 0.01, 1));
	assert(checkClose<double>(mvi.max(), cmvi.max(), 0.01, 1));
	assert(mvi.count() == nbtests);
	assert(checkClose<double>(mvf.mean(), cmvf.mean(), 0.01, 0.01));
	assert(checkClose<double>(mvf.std(), cmvf.std(), 0.01, 0.01));
	assert(checkClose<double>(mvf.min(), cmvf.min(), 0.01, 0.01));
	assert(checkClose<double>(mvf.max(), cmvf.max(), 0.01, 0.01));
	assert(mvf.count() == nbtests);
	assert(checkClose<double>(mvd.mean(), cmvd.mean(), 0.01, 0.001));
	assert(checkClose<double>(mvd.std(), cmvd.std(), 0.01, 0.001));
	assert(checkClose<double>(mvd.min(), cmvd.min(), 0.01, 0.001));
	assert(checkClose<double>(mvd.max(), cmvd.max(), 0.01, 0.001));
	assert(mvd.count() == nbtests);

	// ------------------------------------------------------------------------
	// Test copy constructors
	// ------------------------------------------------------------------------
	MeanValue<int> mvi2(mvi);
	MeanValue<float> mvf2(mvf);
	MeanValue<double> mvd2(mvd);

	assert(mvi.mean() == mvi2.mean());
	assert(mvi.std() == mvi2.std());
	assert(mvi.min() == mvi2.min());
	assert(mvi.max() == mvi2.max());
	assert(mvi.count() == mvi2.count());
	assert(mvf.mean() == mvf2.mean());
	assert(mvf.std() == mvf2.std());
	assert(mvf.min() == mvf2.min());
	assert(mvf.max() == mvf2.max());
	assert(mvf.count() == mvf2.count());
	assert(mvd.mean() == mvd2.mean());
	assert(mvd.std() == mvd2.std());
	assert(mvd.min() == mvd2.min());
	assert(mvd.max() == mvd2.max());
	assert(mvd.count() == mvd2.count());

	// ------------------------------------------------------------------------
	// Test == operator
	// ------------------------------------------------------------------------
	assert(mvi == cmvi);
	assert(mvf == cmvf);
	assert(mvd == cmvd);
	assert(mvi == mvi2);
	assert(mvf == mvf2);
	assert(mvd == mvd2);

	// ------------------------------------------------------------------------
	// Test reset
	// ------------------------------------------------------------------------
	mvi.reset();
	mvf.reset();
	mvd.reset();

	assert(mvi.mean() == 0.0);
	assert(mvi.std() == 0.0);
	assert(mvi.min() == 0);
	assert(mvi.max() == 0);
	assert(mvi.count() == 0);
	assert(mvf.mean() == 0.0);
	assert(mvf.std() == 0.0);
	assert(mvf.min() == 0.0f);
	assert(mvf.max() == 0.0f);
	assert(mvf.count() == 0);
	assert(mvd.mean() == 0.0);
	assert(mvd.std() == 0.0);
	assert(mvd.min() == 0.0);
	assert(mvd.max() == 0.0);
	assert(mvd.count() == 0);

	// ------------------------------------------------------------------------
	// test != operator
	// ------------------------------------------------------------------------
	assert(mvi != mvi2);
	assert(mvf != mvf2);
	assert(mvd != mvd2);

	// ------------------------------------------------------------------------
	// Test Accumulate "nbtests" random values between "minValue" & "maxValue"
	// into MeanValue<?, double> with operator +=
	// ------------------------------------------------------------------------
	tic = clock();
	accumulate(mvi, (size_t)nbtests, minValue, maxValue, true);
	accumulate(mvf, (size_t)nbtests, (float)minValue, (float)maxValue, true);
	accumulate(mvd, (size_t)nbtests, (double)minValue, (double)maxValue, true);
	toc = clock();

	cout << "elapsed time for operator+= = " << (toc - tic) / 1000 << " ms"
	     << endl;
	cout << "expected values were " << expectedMean << " ± " << expectedStd
	     << " [" << minValue << "..." << maxValue << "]" << endl;
	cout << "MeanValue<int,    double> mvi = " << mvi << endl;
	cout << "MeanValue<float,  double> mvf = " << mvf << endl;
	cout << "MeanValue<double, double> mvd = " << mvd << endl;

	// ------------------------------------------------------------------------
	// Test resulting values for :
	//	- mean
	//	- std
	//	- min
	//	- max
	//	- count
	// ------------------------------------------------------------------------
	assert(checkClose<double>(mvi.mean(), cmvi.mean(), 0.01, 1));
	assert(checkClose<double>(mvi.std(), cmvi.std(), 0.01, 1));
	assert(checkClose<double>(mvi.min(), cmvi.min(), 0.01, 1));
	assert(checkClose<double>(mvi.max(), cmvi.max(), 0.01, 1));
	assert(mvi.count() == nbtests);
	assert(checkClose<double>(mvf.mean(), cmvf.mean(), 0.01, 0.01));
	assert(checkClose<double>(mvf.std(), cmvf.std(), 0.01, 0.01));
	assert(checkClose<double>(mvf.min(), cmvf.min(), 0.01, 0.01));
	assert(checkClose<double>(mvf.max(), cmvf.max(), 0.01, 0.01));
	assert(mvf.count() == nbtests);
	assert(checkClose<double>(mvd.mean(), cmvd.mean(), 0.01, 0.001));
	assert(checkClose<double>(mvd.std(), cmvd.std(), 0.01, 0.001));
	assert(checkClose<double>(mvd.min(), cmvd.min(), 0.01, 0.001));
	assert(checkClose<double>(mvd.max(), cmvd.max(), 0.01, 0.001));
	assert(mvd.count() == nbtests);

	// ------------------------------------------------------------------------
	// Test Move constructors
	// ------------------------------------------------------------------------
	MeanValue<int> mvi3(mvi2);
	MeanValue<float> mvf3(mvf2);
	MeanValue<double> mvd3(mvd2);
	MeanValue<int> mvi4(std::move(mvi3));
	MeanValue<float> mvf4(std::move(mvf3));
	MeanValue<double> mvd4(std::move(mvd3));

	assert(mvi4 == mvi2);
	assert(mvi4 != mvi3);
	assert(mvf4 == mvf2);
	assert(mvf4 != mvf3);
	assert(mvd4 == mvd2);
	assert(mvd4 != mvd3);

	// ------------------------------------------------------------------------
	// Test copy operator
	// ------------------------------------------------------------------------
	mvi3 = mvi4;
	assert(mvi3 == mvi4);
	mvf3 = mvf4;
	assert(mvf3 == mvf4);
	mvd3 = mvd4;
	assert(mvd3 == mvd4);

	// ------------------------------------------------------------------------
	// Test move operator
	// ------------------------------------------------------------------------
	mvi4 = std::move(mvi3);
	assert(mvi4 != mvi3);
	assert(mvi4 == mvi2);
	mvf4 = std::move(mvf3);
	assert(mvf4 != mvf3);
	assert(mvf4 == mvf2);
	mvd4 = std::move(mvd3);
	assert(mvd4 != mvd3);
	assert(mvd4 == mvd2);

	// ------------------------------------------------------------------------
	// Test Cast to R operator
	// ------------------------------------------------------------------------
	assert(mvi.mean() == (double)mvi);
	assert(mvf.mean() == (double)mvf);
	assert(mvd.mean() == (double)mvd);

	// ------------------------------------------------------------------------
	// Test Output opererator
	// ------------------------------------------------------------------------
	ostringstream testI, testF, testD;

	testI << mvi.mean() << " ± " << mvi.std() << " [" << mvi.min() << "..."
	      << mvi.max() << "](" << mvi.count() << ")";
	testF << mvf.mean() << " ± " << mvf.std() << " [" << mvf.min() << "..."
	      << mvf.max() << "](" << mvf.count() << ")";
	testD << mvd.mean() << " ± " << mvd.std() << " [" << mvd.min() << "..."
	      << mvd.max() << "](" << mvd.count() << ")";

	ostringstream outI, outF, outD;
	outI << mvi;
	assert(testI.str() == outI.str());
	outF << mvf;
	assert(testF.str() == outF.str());
	outD << mvd;
	assert(testD.str() == outD.str());

//	// ------------------------------------------------------------------------
//	// Initializer list constructor
//	// ------------------------------------------------------------------------
//	MeanValue<int, double> m5 = { 4, 6, 3, 7, 2, 8 };
//
//	assert(m5.mean() == double(5));
//
//	int n = 6;
//	MeanValue<int, double> m6 = { n - 1, n + 1, n - 2, n + 2, n - 3, n + 3 };
//	assert(m6.mean() == double(n));

//	// ------------------------------------------------------------------------
//	// Variadic constructors
//	// ------------------------------------------------------------------------
//	MeanValue<int, double> m43(4, 3, 5);
//	MeanValue<int, double> m47(4, 3, 5, 2, 6, 1, 7);
//
//	assert(m43.mean() == double(4));
//	assert(m47.mean() == double(4));

	cout << "Everything went fine" << endl;

	return EXIT_SUCCESS;
}
