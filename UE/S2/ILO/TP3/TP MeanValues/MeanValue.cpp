#include <cmath>
#include "MeanValue.h"

/*
 * Tolerance for comparing two values expressed in percent of these
 * values [default value is 1%]
 */
template<typename T, typename R>
double MeanValue<T, R>::_relTolerance = 0.01;

/*
 * Default Value to reset minimum value to
 * (a high value so that next value will have reasonable chances to be
 * less than this value), so the maximum values of type T should fit.
 */
template<typename T, typename R>
const T MeanValue<T, R>::_minDefault = numeric_limits<T>::max();

/*
 * Default value to reset maximum value to
 * (a low value so that next value will have reasonable chances to be
 * greater than this value), so the lowest value of type T should fit
 */
template<typename T, typename R>
const T MeanValue<T, R>::_maxDefault = numeric_limits<T>::lowest();

/*
 * PRIVATE Valued constructor in order to create a mean value to be
 * compared with other mean values (typically through a factory method
 * providing a constant mean value)
 * @param mean the mean value to set
 * @param std the std value to set
 * @param min the min value to set
 * @param max the max value to set
 * @post internal attributes have been initialized according to
 * arguments.
 * @see #get(const R &,const R &, const T &, const T &);
 */
template<typename T, typename R>
MeanValue<T, R>::MeanValue(const R & mean,
                           const R & std,
                           const T & min,
                           const T & max,
                           const long count) :
	_mean(R(0)/* TODO Replace with correct value ... */),
	_meanSquare(R(1)/* TODO Replace with correct value ... */),
	_count(2L/* TODO Replace with correct value ... */),
	_min(T(3) /* TODO Replace with correct value ... */),
	_max(T(4) /* TODO Replace with correct value ... */)
{
	// Ne rien écrire ici
}

/*
 * Default Constructor.
 * @post Attributes for computing mean & std are set to 0, min & max
 * attributes are set to their default values, count is set to 0
 */
template<typename T, typename R>
MeanValue<T, R>::MeanValue() :
	_mean(R(5) /* TODO Replace with correct value ... */),
	_meanSquare(R(6) /* TODO Replace with correct value ... */),
	_count(7L /* TODO Replace with correct value ... */),
	_min(T(8) /* TODO Replace with correct value ... */),
	_max(T(9) /* TODO Replace with correct value ... */)
//	 Or
//	 MeanValue(R(5), R(6), T(8), T(9), 7L) // TODO Replace with correct values
{
	// Ne rien écrire ici
}

/*
 * Copy constructor
 * @param mv the other mean value to copy
 */
template<typename T, typename R>
MeanValue<T, R>::MeanValue(const MeanValue<T, R> & mv) :
	_mean(R(10) /* TODO Replace with correct value ... */),
	_meanSquare(R(11) /* TODO Replace with correct value ... */),
	_count(12L /* TODO Replace with correct value ... */),
	_min(T(13) /* TODO Replace with correct value ... */),
	_max(T(14) /* TODO Replace with correct value ... */)
//	 Or
//	 MeanValue(R(10), R(11), T(13), T(14), 12L) // TODO Replace with correct values
{
	// Ne rien écrire ici
}

/*
 * Move constructor
 * @param mv the other mean value to move
 * @post reset has been performed on the moved mean value
 */
template<typename T, typename R>
MeanValue<T, R>::MeanValue(MeanValue<T, R> && mv) :
	_mean(R(15) /* TODO Replace with correct value ... */),
	_meanSquare(R(16) /* TODO Replace with correct value ... */),
	_count(17L /* TODO Replace with correct value ... */),
	_min(T(18) /* TODO Replace with correct value ... */),
	_max(T(19) /* TODO Replace with correct value ... */)
//	 Or
//	 MeanValue(R(15), R(16), T(18), T(19), 17L) // TODO Replace with correct values
{
	// TODO Complete ...
}

/*
 * Factory method to obtain a constant mean value to be evt compared
 * with other mean values
 * @param mean the mean value to set in this constant mean value
 * @param std the std value to set in this constant mean value
 * @param min the min value to set in this constant mean value
 * @param max the max value to set in this constant mean value
 * @return a constant mean value with specified mean, std, min & max
 * and -1 count in order to avoid min() & max() to return 0 values.
 * @note Since the returned MeanValue is constant no values can be
 * accumulated with opertator () or operator +=
 */
template<typename T, typename R>
const MeanValue<T, R> MeanValue<T, R>::get(const R & mean,
                                           const R & std,
                                           const T & min,
                                           const T & max)
{
	// TODO replace with correct implementation ...
	return MeanValue<T, R>();
}

/*
 * Destructor
 * Reset all values to default before destruction
 */
template<typename T, typename R>
MeanValue<T, R>::~MeanValue()
{
	// TODO Complete ...
}

/*
 * Compute mean value : E(X).
 * @return the mean value of all elements added to this MeanValue.
 */
template<typename T, typename R>
R MeanValue<T, R>::mean() const
{
	// TODO Replace by correct implementation ...
	return R(0);
}

/*
 * Compute standard deviation of values : sqrt(E(X^2) - E(X)^2).
 * @return the standard deviation of all elements added to this MeanValue.
 */
template<typename T, typename R>
R MeanValue<T, R>::std() const
{
	// TODO Replace by correct implementation : ...
	// Beware, sqrt(x) returns a NaN and generates a domain error for x < 0.
	return R(0);
}

/*
 * Minimum recorded value accessor
 * @return the minimum recorded value (until reset)
 * @note if no value has been taken into account yet (_count == 0)
 * then return 0;
 */
template<typename T, typename R>
T MeanValue<T, R>::min() const
{
	// TODO Replace by correct implementation ...
	return T(-1);
}

/*
 * Maximum recorded value accessor
 * @return the maximum recorded value (until reset)
 * @note if no value has been taken into account yet (_count == 0)
 * then return 0;
 */
template<typename T, typename R>
T MeanValue<T, R>::max() const
{
	// TODO Replace by correct implementation ...
	return T(1);
}

/*
 * Number of values added to this mean value so far (until reset)
 * @return the number of values taken into account in this mean value
 * @note constant MeanValues provided by #get method have a -1 count
 */
template<typename T, typename R>
long MeanValue<T, R>::count() const
{
	// TODO Replace by correct implementation ...
	return 0;
}

/*
 * Reset all accumulated values to 0 and (internal) min & max values
 * to their default values.
 * @see #_minDefault
 * @post after reset the following calls should provide the following
 * values:
 * 	- m.mean() should be 0
 * 	- m.std() should be 0
 * 	- m.count should be 0
 * 	- m.min() should be 0 (internal value should be set to _minDefault)
 * 	- m.max() should be 0 (internal value should be set to _maxDefault)
 */
template<typename T, typename R>
void MeanValue<T, R>::reset()
{
	// TODO Complete ...
}

/*
 * Sets a new relative tolerance percentage to be used in values
 * comparison in order to decide if theses values are close enough to
 * be considered equal
 * @param tol the new tolerance percentage to set
 */
template<typename T, typename R>
void MeanValue<T, R>::tolerance(const double tol)
{
	_relTolerance = tol;
}

/*
 * Function call operator with one value.
 * Computes un updated value of the mean and std:
 * 	- E(X_n+1) = (1/(n+1))[n E(X_n) + x_n+1]
 * 	- E(X_n+1^2) = (1/(n+1))[ n E(X_n^2) + (x_n+1)^2 ]
 * @param value a value to take into account for computing mean, std,
 * min & max values.
 * @post the value has been taken into account for mean, std, min & max
 * computation and elements count has been increased by 1
 * @warning One should NOT use internal sum of values or internal
 * sum of square values of type T to compute mean and std since these
 * sums tends to grow to very large values as more values are added,
 * hence eventually triggering a value overflow which can lead to a
 * sign inversion. Maintaining E(X) and E(X^2) values instead of
 * sums can solve this problem.
 */
template<typename T, typename R>
void MeanValue<T, R>::operator ()(const T & value)
{
	/*
	 * TODO update internal attibutes in order to be able to provide
	 * 	- mean value of type R
	 * 	- standard deviation value ot type R
	 * 	- min value of type T
	 * 	- max value of type T
	 * 	- number of accumulated values
	 * 	Beware ! Type T could be "int" so operations on int could lead to
	 * 	overflow. Better use type R which will be float or double.
	 */
}

/*
 * Self increment operator
 * @param value a new value to be considered for computing mean, std,
 * min & max
 * @post internal attributes have been updated
 * @note does the same thing as Function call operator
 * @see #operator ()(const T & value)
 */
template<typename T, typename R>
void MeanValue<T, R>::operator +=(const T & value)
{
	// TODO Complete with previous operator ...
}

/*
 * Copy operator from another mean value
 * @param mv the mean value to copy
 * @return a reference to the current mean value
 */
template<typename T, typename R>
MeanValue<T, R> & MeanValue<T, R>::operator =(const MeanValue<T, R> & mv)
{
	// TODO Complete ...

	return *this;
}

/*
 * Move operator from another mean value
 * @param mv the mean value to move
 * @return a reference to the current mean value
 * @post the moved mean value "mv" has been reset
 */
template<typename T, typename R>
MeanValue<T, R> & MeanValue<T, R>::operator =(MeanValue<T, R> && mv)
{
	// TODO Complete ...

	return *this;
}

/*
 * Cast operator to result type (R)
 * @return the mean value as provided by mean() method
 * @see #mean()
 */
template<typename T, typename R>
MeanValue<T, R>::operator R() const
{
	// TODO Replace by correct implementation
	return R(0);
}

/*
 * Test the equality of the current mean value with another one
 * according to the tolerance percentage set by the
 * tolerance(const double) method
 * @param mv the other mean value to test
 * @return true if all mean, std, min & max values are in the range
 * of the tolerance
 * @see #tolerance(const double)
 * @see checkClose(const T & val1, const T & val2, const double relTol, const T & absTol)
 */
template<typename T, typename R>
bool MeanValue<T, R>::operator ==(const MeanValue<T, R> & mv) const
{
	/*
	 * TODO Replace with correct implementation :
	 * Compare values using checkClose function
	 */
	bool dummy = checkClose(mv._mean, mv._mean, _relTolerance, _absTolerance);
	return !dummy;
}

/*
 * Test the difference of the current mean value with another one
 * according to the tolerance percentage set by the tolerance(const double)
 * method
 * @param mv the other mean value to test
 * @return false if all mean, std, min & max values are in the range
 * of the tolerance
 * @post the negation of the previous operator has been used to ensure
 * symmetry
 * @see #tolerance(const double)
 */
template<typename T, typename R>
bool MeanValue<T, R>::operator !=(const MeanValue<T, R> & mv) const
{
	/*
	 * TODO Replace with correct implementation :
	 * Compare values using the previous operator
	 */
	return false;
}

/*
 * Output operator for MeanValue
 * @param out the output stream
 * @param mv the MeanValue to print on the output stream
 * @return a reference to the current output stream
 * @post put mean value ± std value on the stream
 * @code
 * 	MeanValue<int> m;
 * 	// Add 2500 values within [1..10]
 * 	...
 * 	cout << m << endl;
 * 	// should provide an output looking like the following line :
 * 	// 5.49998 ± 2.8723 [1...10](2500)
 * 	// |         |       |   |   |
 * 	// |         |       |   |   number values accumulated so far
 * 	// |         |       |   max recorded value
 * 	// |         |       min recorded value
 * 	// |         std value
 * 	// mean value
 * @endcode
 */
template<typename T, typename R>
ostream & operator <<(ostream & out, const MeanValue<T, R> & mv)
{
	// TODO Complete (without < or >) to get the following output
	// <mean value> ± <std value> [<min value>...<max value>](<count value>)
	// Please note that spaces are required

	return out;
}

/*
 * Compare two values equality within a tolerance expressed in percent
 * @param val1 the first value to compare
 * @param val2 the second value to compare
 * @param relTol relative tolerance for comparing two values
 * @param absTol Absolute tolerance for comparing two values
 * @return true if val1 == val2 evt within relative tolerance or absolute
 * tolerance when relative tolerance is not applicable (around 0)
 */
template<typename T>
bool checkClose(const T & val1,
                const T & val2,
                const double relTol,
                const T & absTol)
{
	if (val1 == val2)
	{
//		clog << "Comparing " << val1 << " == " << val2 << endl;
		return true;
	}

	double val1d = double(val1);
	double val2d = double(val2);

	double absoluteError = abs(val1d - val2d);

//	clog << "Comparing(" << val1 << ", " << val2 << ") absolute error = "
//	     << absoluteError << (absoluteError < absTol ? " < " : " >= ")
//	     << absTol << endl;

	if (absoluteError < double(absTol))
	{
		return true;
	}

	double relativeError;
	if (abs(val2d) > abs(val1d))
	{
		relativeError = abs((val1d - val2d) / val2d);
	}
	else
	{
		relativeError = abs((val1d - val2d) / val1d);
	}

//	clog << "Comparing(" << val1 << ", " << val2 << ") relative error = "
//	     << relativeError << (relativeError < relTol ? " < " : " >= ")
//	     << relTol << endl;

	if (relativeError <= relTol)
	{
		return true;
	}

	return false;
}

// --------------------------------------------------------------------
// Template specializations for known types
// --------------------------------------------------------------------

/**
 * Absolute tolerance when accumulating int values and providing double
 * mean and std values
 */
template<> double MeanValue<int, double>::_absTolerance = 1;

/**
 * Absolute tolerance when accumulating float values and providing double
 * mean and std values
 */
template<> double MeanValue<float, double>::_absTolerance = 0.01;

/**
 * Absolute tolerance when accumulating double values and providing double
 * mean and std values
 */
template<> double MeanValue<double, double>::_absTolerance = 0.001;

/**
 * Absolute tolerance when accumulating int values and providing float
 * mean and std values
 */
template<> float MeanValue<int, float>::_absTolerance = 1;

/**
 * Absolute tolerance when accumulating float values and providing float
 * mean and std values
 */
template<> float MeanValue<float, float>::_absTolerance = 0.01;

/**
 * Absolute tolerance when accumulating double values and providing float
 * mean and std values
 */
template<> float MeanValue<double, float>::_absTolerance = 0.001;

/**
 * Absolute tolerance when accumulating int values and providing int
 * mean and std values
 */
template<> int MeanValue<int, int>::_absTolerance = 1;

/**
 * Absolute tolerance when accumulating float values and providing int
 * mean and std values
 */
template<> int MeanValue<float, int>::_absTolerance = 1;

/**
 * Absolute tolerance when accumulating double values and providing int
 * mean and std values
 */
template<> int MeanValue<double, int>::_absTolerance = 1;

// ----------------------------------------------------------------------------
// Template protoinstanciations for
//	- int
//	- float
//	- double
// ----------------------------------------------------------------------------

// Proto instanciations
template class MeanValue<int>;
template class MeanValue<float>;
template class MeanValue<double>;
template class MeanValue<int, float>;
template class MeanValue<double, float>;

// Output operators proto-instanciations
template ostream & operator << (ostream &, const MeanValue<int> &);
template ostream & operator << (ostream &, const MeanValue<float> &);
template ostream & operator << (ostream &, const MeanValue<double> &);
template ostream & operator << (ostream &, const MeanValue<int, float> &);
template ostream & operator << (ostream &, const MeanValue<float, float> &);
template ostream & operator << (ostream &, const MeanValue<double, float> &);
