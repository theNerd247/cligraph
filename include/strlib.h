/*
 * strlib.h is part of cligraph.
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
 * strlib.h 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: decleration file for strlib functions 
 */

#ifndef __STRLIB
#define __STRLIB

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
 * FUNCTION: strrpl
 * 
 * PARAMETERS: char* haystack, int strt, int end, char* needle
 *
 * RETURNS: char*  - new cstring containing replaced values
 * 
 * DESCRIPTION: replaces "needle" in "expr" from "str" to "end" indeces
 */
char*  strrpl(char* expr, int strt, int end, char* needle);

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
