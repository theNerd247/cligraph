/*
 * stringmath.h is part of cligraph.
 * 
 * cligraph is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * cligraph is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with cligraph.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * stringmath.h 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: functions that mix math.h and string.h (mainly to compute mathematical string
 * expressions). 
 */

#ifndef __STRINGMATH
#define __STRINGMATH

#include <llist.h>

//structure to contain function value pairs
typedef struct funcpoint 
{
	double y;
	double x;
} POINT;

#define newPOINT() (POINT*)malloc(sizeof(POINT))

typedef LList FuncValues;

/*
 * FUNCTION: getvalue
 * 
<<<<<<< HEAD
 * PARAMETERS: char* expr
 *
 * RETURNS: double - evaluated expression value
=======
 * PARAMETERS: char* expr, double value
 *
 * RETURNS: double - evaluated function value
>>>>>>> compute
 *
 * DESCRIPTION: computes the value of the given mathematical expression (with indep var replaced
 * with a value
 */
double getvalue(char* expr, double value);

/*
 * FUNCTION: getvalues
 * 
 * PARAMETERS: char*  expr, double strt, double end, const double step
 *
 * RETURNS: FuncValues* - linked list of function values
 * 
 * DESCRIPTION: computes function values starting at "str" and ending at "end" with step size of "step"
 */
FuncValues* getfuncvalues(char*  expr, double strt, double end, const double step);

/*
 * FUNCTION: chknum
 * 
 * PARAMETERS: char* expr
 *
 * RETURNS: char - bool value (0 or 1)
 * 
 * DESCRIPTION: checks if the given cstring is a number (contains only numeric characters)
 */
char chknum(char* expr);

/*
 * FUNCTION: strtonum
 * 
 * PARAMETERS: char* expr
 *
 * RETURNS: double - double value of numerical expression
 * 
 * DESCRIPTION: converts a given string (assuming to contain only numbers) to the numerical
 * expression
 */
double strtonum(char* expr);

#endif 
