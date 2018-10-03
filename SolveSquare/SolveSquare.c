#include <stdio.h>
#include <math.h>

/**
        \This function finds roots of the square equation
        @param [in] a coefficient
        @param [in] b coefficient
        @param [in] c coeffitient
        @param [out] x1 pointer to the first root
        @param [out] x2 pointer to the second root
        @return roots of equation       
*/

int SolveSquare (double a, double b, double c, double* x1, double*x2);

/**
	\This function finds roots of the linear equation
	@param [in] b coefficient
	@param [in] c coeffitient
	@param [out] x1 pointer to the first root
	@param [out] x2 pointer to the second root
	@return roots of equation	
*/ 

int SolveLinear (double b, double c, double* x1, double* x2);

enum Errors
{
	ErrorScanf =  101,
	NullPtr = 102,
	NumberIsNan = 103,
	Infinity = 104
};

enum Solve
{
	NoRoots = 0,
	OneRoot = 1,
	TwoRoots = 2,
	InfinityRoots = 3
};

int main()
{
	double a = 0, b = 0, c = 0;
        int paramScanf = 0;
	printf ("\ninsert number please\n");
	paramScanf = scanf ("%lg %lg %lg", &a, &b, &c);
	if (paramScanf != 3)
	{
		printf ("\n Invalid value\n");
		return ErrorScanf;
	}
	if ((fabs (isinf(a)) == 1) || (fabs (isinf(b)) == 1) || (fabs (isinf(c)) == 1))
	{
		printf ("\n Value is infinity\n");
		return Infinity;
	}
	double x1 = 0, x2 = 0;
	int nRoots = SolveSquare (a, b, c, &x1, &x2);
	
	switch (nRoots)
	{
		case NoRoots:
			{
				printf ("there aren't roots in params:a = %lg, b = %lg, c = %lg\n", a, b, c);
				break;
			}
		case OneRoot: 
			{
				printf ("One Root in params:a = %lg, b = %lg, c = %lg,x1 = %lg\n", a, b, c, x1);
				break;
			}
		case TwoRoots:
			{
				printf ("find 2 roots in params:a = %lg, b = %lg, c = %lg, x1 = %lg, x2 = %lg\n", a, b, c, x1, x2);
				break;
			}
		case InfinityRoots:
			{
				printf ("there are a lot of roots in params:a = %lg, b = %lg, c = %lg\n", a, b, c);
				break;
			}
	}
	return 0;
}

int SolveSquare (double a, double b, double c, double* x1, double* x2)
{
	if ((isnan (a) != 0) || (isnan (b) != 0) || (isnan (c) != 0))
		return NumberIsNan;
	if ((x1 == NULL) || (x2 == NULL))
		return NullPtr;

	if (a == 0)
		return SolveLinear (b, c, x1, x2);
	else
		{
			double Discriminant = b * b - 4 * a * c;
			if (Discriminant >= 0)
				{
					*x1 = (- b - sqrt(Discriminant)) / (2 * a);
					*x2 = (- b + sqrt(Discriminant)) / (2 * a);
					return TwoRoots;
				}
			else 
				{
					*x1 = NAN;
					*x2 = NAN;
					return NoRoots;
				}
		}
}

int SolveLinear (double b, double c, double* x1, double * x2)
{
	if ((isnan (b) != 0) || (isnan (c) != 0))
		return NumberIsNan;
	if ((x2 == NULL) || (x2 == NULL))
		return NullPtr;

	if (b == 0)
		{
			if (c == 0)
				return InfinityRoots;
			else
				{
					*x1 = NAN;
					*x2 = NAN;
					return NoRoots;
				}
		}
	else
		{
			*x1 = - c / b;
			*x2 = NAN;
			return OneRoot;
		}
}
