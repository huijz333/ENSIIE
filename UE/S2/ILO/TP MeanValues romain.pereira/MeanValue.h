#ifndef MEANVALUE_H
#define MEANVALUE_H

#include <iostream>			// for input/output
#include <limits>			// for numeric limits
using namespace std;

/**
 * Mean and std value for type T values expressed in type R.
 * @tparam T the type of values to accummulate
 * @tparam R the type of value of mean and std computation [defaulted to double]
 * @code
 *	MeanValue<clock_t, float> mvcf;
 *	MeanValue<int> mvid; // second argument is defaulted to "double"
 *	// Accumulating values to mvcf
 *	while (...)
 *	{
 *		mcvf += <a new clock_t value>;
 *	}
 *	float cfmean = mcvf.mean();
 *	float cfstd = mcvf.std();
 *	clock_t cfmin = mcvf.min();
 *	clock_t cfmax = mcvf.max();
 *	// Accumulating values to mvid
 *	while (...)
 *	{
 *		mvid(<a new int value>);
 *	}
 *	double idmean = mvid.mean();
 *	double idstd = mvid.std();
 *	int idmin = mvid.min();
 *	int idmax = mvid.max();
 * @endcode
 * @warning One should NOT use internal sum of values or internal
 * sum of square values of type T to compute mean and std since these
 * sums tends to grow to very large values as more values are added,
 * hence eventually triggering a value overflow which can lead to a
 * sign inversion. Maintaining E(X) and E(X^2) values instead of
 * sums can solve this problem.
 * @author David Roussel
 * @date 2014/05/31
 */
template<typename T, typename R = double>
class MeanValue
{
	private:
		/**
		 * Current mean value: \f$E(X)\f$
		 */
		R _mean;

		/**
		 * Current mean of square: \f$E(X^2)\f$
		 */
		R _meanSquare;

		/**
		 * Number of elements counted so far
		 */
		long _count;

		/**
		 * Minimum recorded value
		 */
		T _min;

		/**
		 * Maximum recorded value
		 */
		T _max;

		/**
		 * Relative Tolerance for comparing two values expressed in percent of
		 * these values [initialized to 1%]
		 */
		static double _relTolerance;

		/**
		 * Absolute tolerance for comparing two values [initialized to 0.001]
		 */
		static R _absTolerance;

		/**
		 * Default value to reset minimum value to
		 * (a high value so that next value will have reasonable chances to be
		 * less than this value), so the maximum values of type T should fit.
		 */
		static const T _minDefault;

		/**
		 * Default value to reset maximum value to
		 * (a low value so that next value will have reasonable chances to be
		 * greater than this value), so the lowest value of type T should fit
		 */
		static const T _maxDefault;

		/**
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
		MeanValue(const R & mean,
		          const R & std,
		          const T & min,
		          const T & max,
		          const long count);

	public:
		/**
		 * Default Constructor.
		 * @post Attributes for computing mean & std are set to 0, min & max
		 * attributes are set to their default values, count is set to 0
		 * @code
		 * 	MeanValue<int> mv;
		 * 	assert(mv.mean() == 0.0);
		 * 	assert(mv.std() == 0.0);
		 * 	assert(mv.min() == 0);
		 * 	assert(mv.max() == 0);
		 * 	assert(mv.count() == 0);
		 * @endcode
		 */
		MeanValue();

		/**
		 * Copy constructor
		 * @param mv the other mean value to copy
		 * @code
		 * 	MeanValue<int> m1(...);
		 * 	...
		 * 	MeanValue<int> m2(m1);
		 * @endcode
		 */
		MeanValue(const MeanValue<T, R> & mv);

		/**
		 * Move constructor
		 * @param mv the other mean value to move
		 * @post reset has been performed on the moved mean value
		 * @code
		 * 	MeanValue<int> m2(MeanValue<int>(3, 4, 5, 6));
		 * @endcode
		 */
		MeanValue(MeanValue<T, R> && mv);

		/**
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
		 * @code
		 * 	const MeanValue<int> reference = MeanValue<int>::get(5.5, 2.59808, 1, 10);
		 * 	MeanValue<int> test;
		 * 	// add some random values to test in the range[1..10]
		 * 	...
		 * 	assert(test == reference);
		 * @endcode
		 */
		static const MeanValue<T, R> get(const R & mean,
		                                 const R & std,
		                                 const T & min,
		                                 const T & max);

		/**
		 * Destructor
		 * Reset all values to default before destruction
		 * @code
		 * 	MeanValue<int> * pm;
		 * 	{
		 * 		MeanValue<int> m;
		 *
		 * 		// Do some work with m & pm
		 *
		 * 	} // <-- m has been implicitly destroyed
		 * 	delete pm; // <-- explicitly destroy pm
		 * @endcode
		 */
		virtual ~MeanValue();

		/**
		 * Compute mean value : \f$E(X)\f$.
		 * @return the mean value of all elements added to this MeanValue.
		 * @code
		 * 	MeanValue<int> m;
		 * 	double meanValueInit = m.mean(); // should be 0.0
		 * 	// Add some values to m
		 * 	...
		 * 	double meanValueFinal = m.mean();
		 * @endcode
		 */
		R mean() const;

		/**
		 * Compute standard deviation of values : \f$\sqrt{E(X^2) - E(X)^2}\f$.
		 * @return the standard deviation of all elements added to this MeanValue.
		 * @code
		 * 	MeanValue<int> m;
		 * 	double stdValueInit = m.std(); // should be 0.0
		 * 	// Add some values to m
		 * 	...
		 * 	double stdValueFinal = m.std();
		 * @endcode
		 */
		R std() const;

		/**
		 * Minimum recorded value accessor
		 * @return the minimum recorded value (until reset)
		 * @note if no value has been taken into account yet (_count == 0)
		 * then return 0;
		 * @code
		 * 	MeanValue<int> m;
		 * 	int minValueInit = m.min(); // should be 0 (even if internal value is not)
		 * 	// Add some values to m
		 * 	...
		 * 	int minValueFinal = m.min();
		 * @endcode
		 */
		T min() const;

		/**
		 * Maximum recorded value accessor
		 * @return the maximum recorded value (until reset)
		 * @note if no value has been taken into account yet (_count == 0)
		 * then return 0;
		 * @code
		 * 	MeanValue<int> m;
		 * 	int maxValueInit = m.max(); // should be 0 (even if internal value is not)
		 * 	// Add some values to m
		 * 	...
		 * 	int maxValueFinal = m.max();
		 * @endcode
		 */
		T max() const;

		/**
		 * Number of values added to this mean value so far (until reset)
		 * @return the number of values taken into account in this mean value
		 * @note constant MeanValues provided by #get method have a -1 count
		 * @code
		 * 	MeanValue<int> m;
		 * 	long countValueInit = m.count(); // should be 0
		 * 	// Add 18 values to m
		 * 	...
		 * 	int countValueFinal = m.count(); // should be 18
		 * 	const MeanValue<int> cm = MeanValue<int>::get(0,0,0,0);
		 * 	int countValueConstant = cm.count(); // should be -1
		 * @endcode
		 */
		long count() const;

		/**
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
		 * @code
		 * 	MeanValue<int> m;
		 * 	// Add some non null values to m
		 * 	...
		 * 	double meanValueFinal = m.mean();
		 * 	m.reset();
		 * 	assert(m.mean() != meanValueFinal);
		 * @endcode
		 */
		void reset();

		/**
		 * Sets a new relative tolerance percentage to be used in values
		 * comparison in order to decide if theses values are close enough to
		 * be considered equal
		 * @param tol the new tolerance percentage to set
		 * @code
		 *
		 * @endcode
		 */
		static void tolerance(const double tol);

		/**
		 * Function call operator with one value.
		 * Computes un updated value of the mean:
		 * \f[
		 * E(X_{n+1}) = \frac{1}{n+1}\left[
		 * 	\sum_{i=1}^{n} x_i + x_{n+1}
		 * 	\right] =
		 * 	\frac{1}{n+1}\left[n E(X_n) + x_{n+1} \right]
		 * \f]
		 * and an updated value of the mean of squares
		 * \f[
		 * E(X_{n+1}^{2}) = \frac{1}{n+1}\left[ n E(X_{n}^{2}) +
		 * \left(x_{n+1}\right)^{2} \right]
		 * \f].
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
		 * @code
		 * 	MeanValue<int> m;
		 * 	int n = 0;
		 * 	while (n < 1000)
		 * 	{
		 * 		m(n++);
		 * 	}
		 * @endcode
		 */
		void operator ()(const T & value);

		/**
		 * Variadic function call operator with multiple values
		 * @tparam U the types of other arguments
		 * @param value a value to take into account for computing mean, std,
		 * min & max values
		 * @param others other values to take into account computing mean, std,
		 * min & max values
		 * @post all the values (value & others) have been taken into account
		 * for mean, std, min & max computation and elements count has been
		 * increased by 1 + sizeof...(others)
		 * @note this method should be implemented in the header, otherwise
		 * all its possible proto instanciation should be described in the
		 * proto-instanciation section at the end of MeanValue.cpp
		 * @code
		 * 	MeanValue<int> m;
		 * 	m(4,3,5,2,6);
		 * 	assert(m.mean() == 4.0);
		 * @endcode
		 */
		template<typename ...U>
		void operator ()(const T & value, const U & ... others)
		{
			operator ()(value);
			operator ()(others...);
		}

		/**
		 * Self increment operator
		 * @param value a new value to be considered for computing mean, std,
		 * min & max
		 * @post internal attributes have been updated
		 * @note does the same thing as Function call operator
		 * @see #operator ()(const T & value)
		 * @code
		 * 	MeanValue<int> m;
		 * 	int n = 0;
		 * 	while (n < 1000)
		 * 	{
		 * 		m += n++;
		 * 	}
		 * @endcode
		 */
		void operator +=(const T & value);

		/**
		 * Copy operator from another mean value
		 * @param mv the mean value to copy
		 * @return a reference to the current mean value
		 * @code
		 * 	MeanValue<int> m1;
		 * 	MeanValue<int> m2;
		 * 	// Add some values to m1
		 * 	...
		 * 	m2 = m1;
		 * 	assert(m2 == m1);
		 * @endcode
		 */
		MeanValue<T, R> & operator =(const MeanValue<T, R> & mv);

		/**
		 * Move operator from another mean value
		 * @param mv the mean value to move
		 * @return a reference to the current mean value
		 * @post the moved mean value "mv" has been reset
		 * @code
		 * 	MeanValue<int> m1;
		 * 	MeanValue<int> m2;
		 * 	// Add some values to m1
		 * 	...
		 * 	double mean = m1.mean();
		 * 	double std = m1.std();
		 * 	int min = m1.min();
		 * 	int max = m1.max();
		 * 	m2 = std::move(m1);
		 * 	assert(m2 != m1); // m1 has been reset
		 * 	assert(mean == m2.mean());
		 * 	assert(std == m2.std());
		 * 	...
		 * @endcode
		 */
		MeanValue<T, R> & operator =(MeanValue<T, R> && mv);

		/**
		 * Cast operator to result type (R)
		 * @return the mean value as provided by mean() method
		 * @see #mean()
		 * @code
		 * 	MeanValue<int> m;
		 * 	// Add some values to m
		 * 	...
		 * 	double computedMean = m;
		 * @endcode
		 */
		operator R() const;

		/**
		 * Test the equality of the current mean value with another one
		 * according to the tolerance percentage set by the
		 * tolerance(const double) method
		 * @param mv the other mean value to test
		 * @return true if all mean, std, min & max values are in the range
		 * of the tolerance
		 * @see #tolerance(const double)
		 * @see checkClose(const T & val1, const T & val2, const double relTol, const T & absTol)
		 * @code
		 * 	const MeanValue<int> expected = MeanValue<int>::get(5.5, 2.59808, 1, 10);
		 * 	MeanValue<int> test;
		 * 	// add some random values to test in the range[1..10]
		 * 	...
		 * 	// should be true if enough values have been added
		 * 	assert(test == expected);
		 * @endcode
		 */
		bool operator ==(const MeanValue<T, R> & mv) const;

		/**
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
		bool operator !=(const MeanValue<T, R> & mv) const;
};

/**
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
ostream & operator <<(ostream & out, const MeanValue<T, R> & mv);

/**
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
                const T & absTol);

#endif // MEANVALUE_H
