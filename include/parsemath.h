/*
 * parsemath.h is part of cligraph.
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
 * parsemath.h 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: parser functions for mathmatical expressions
 */

#ifndef __PARSEMATH
#define __PARSEMATH

//define what the independent variable is
char indep_var = 'x';

/*
 * FUNCTION: eval
 * 
 * PARAMETERS: char* expr, int value
 *
 * RETURNS: int - value of evaluated expression
 * 
 * DESCRIPTION: evaluates the given expression with the given value for the independent variable
 */
int eval(char* expr, int value);

/*
 * FUNCTION: expndexpr
 * 
 * PARAMETERS: char* expr
 *
 * RETURNS: char*  - expanded expression
 * 
 * DESCRIPTION: takes a mathmatical expression and expands the implied operations for easier
 * computation
 *
 * EXAMPLE: expndexpr("4x^2(3x-2)") returns "(4*(x^2))*((3*x)-2)"
 *
 */
char*  expndexpr(char* expr);

/*
 * FUNCTION: strins
 * 
 * PARAMETERS: char* dest, const char* source, const int index
 *
 * RETURNS: char*  - cstring w/. inserted characters
 * 
 * DESCRIPTION: inserts source into dest at the given index (with 0 being the first character in
 * dest)
 */
char*  strins(char* dest, const char* source, const int index);

/*
 * FUNCTION: inspare
 * 
 * PARAMETERS: char* expr
 *
 * RETURNS: char* - new string containing parenthesis
 * 
 * DESCRIPTION: expands the given mathematical expression with parenthesis
 * to override order of operations
 *
 */
char* inspare(char* expr);

/*
 * FUNCTION: strsub
 * 
 * PARAMETERS: char* expr, const int start, const int end
 *
 * RETURNS: char* - sub cstring
 * 
 * DESCRIPTION: returns a substring (cstring style) within given expr. 
 */
char* strsub(char* expr, const int start, const int end);

/*
 * FUNCTION: expndcoef
 * 
 * PARAMETERS: char* expr
 *
 * RETURNS: char* - expanded cstring
 * 
 * DESCRIPTION: expands the multiplication coefficients with operators
 *
 * EXAMPLE: expndcoef("3x^2+2x") returns "3*x^2+2*x"
 *
 * NOTE: recursive function
 */
char* expndcoef(char* expr);

#endif 
