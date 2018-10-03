#include <math,h>
#include <stdio.h>


void UnitTest ();
int CompareNumber (double num1, double num2);

int main ()
{
	UnitTest ();
	return 0;
}

void UnitTest ()
{//                                           a  b  c  nRoots  x1  x2
        struct CheckSolveSquare Tests[] = {{ 0, 0,  0,  3,     0,  0},
                                            { 1, 4,  3,  2,    -3, -1},
                                            { 0, 1,  1,  1,    -1, NAN},
                                            { 1, 0, -1,  2,    -1,  1},
                                            { 1, 2, 1,   1,    -1, -1},
                                            { 2, 5, 9,   0,     0,  0},
                                            { 1, 4, 4,   1,    -2, -2}};
        int i = 0;
        for (; i < sizeof (Tests) / sizeof (Tests[0]); i++)
        {
            int RealnRoots = Tests[i].nRoots;
            double Realx1 = Tests[i].x1;
            double Realx2 = Tests[i].x2;

            int nRoots = SolveSquare (Tests[i].a, Tests[i].b, Tests[i].c, &Tests[i].x1, &Tests[i].x2);
            if (CompareNumber (Tests[i].nRoots, RealnRoots) || CompareNumber(Tests[i].x1, Realx1) || CompareNumber(Tests[i].x2, Realx2))
                {
                    printf ("\n Error in test %d\n", i);
                    printf ("\n Roots:         x1 = %lg,x2 = %lg\n", Tests[i].x1, Tests[i].x2);
                    printf ("\n Expected roots:x1 = %lg,x2 = %lg\n", Realx1, Realx2);
                    printf ("\n nRoots:         %d\n", Tests[i].nRoots);
                    printf ("\n Expected nRoots:%d\n", RealnRoots);
                }
        }
}


int CompareNumber (double num1, double num2)
{
	if ((isnan (num1) != 0) || (isnan (num2) != 0))
		return NumberIsNAN;
	double eps = 1e-7;
	if (fabs (num1 - num2) < eps)
		return 0;
	else 
		return 1;
}
