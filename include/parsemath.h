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
#ifndef indep_var
#define indep_var 'x'
#endif

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
 * FUNCTION: insindpvar
 * 
 * PARAMETERS: char* expr, char* value
 *
 * RETURNS: char* - new cstring containing the inserted value
 * 
 * DESCRIPTION: inserts a value in the string for the independent variable
 */
//char* insindpvar(char* expr, char* value);

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
 * FUNCTION: mtchpar
 * 
 * PARAMETERS: char* expr, short pcnt
 *
 * RETURNS: char* - pointer to the matching parenthesis
 * 
 * DESCRIPTION: finds the matching parenthesis given a pointer to an initial parenthesis.
 *
 * NOTE: the second argument is either a 1 if the first argument is a pointer to a '('
 * and a -1 if it is a pointer to a ')'. If the given cstring doesn't contain any 
 * parenthesis then it will return the first argument
 */
char* mtchpar(char* expr, short pcnt);

#endif 
