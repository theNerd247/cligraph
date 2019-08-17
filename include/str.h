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
 * DESCRIPTION: extension of string.h
 */

#ifndef __STRLIB
#define __STRLIB

/*
 * FUNCTION: strins
 * 
 * DESCRIPTION: inserts needle into haystack at given index
 *
 * PARAMETERS: char* haystack, char* needle, size_t index, char* buff
 *
 * RETURNS: char* - buff containing the string with inserted characters. If error occurs NULL is returned. 
 *
 * NOTE: if buff is not sized properly then buffer overflow can occur (this string makes use of
 * strcpy).
 */
char* strins(char* haystack, char* needle, size_t index, char* buff);

/*
 * FUNCTION: strrpl
 * 
 * DESCRIPTION: replaces characters from strt to end (inclusive) with needle in haystack. 
 *
 * PARAMETERS: char* haystack, char* needle, size_t strt, size_t end, char* buff
 *
 * RETURNS: char* - the newly formed cstring stored in buff. If an error occurs NULL is returned
 *
 * NOTE: make sure that buff contains the proper amount of memory other wise behavior can be
 * unpredicatble (this function makes use of strcpy).
 * 
 */
char* strrpl(char* haystack, char* needle, size_t strt, size_t end, char* buff);

/*
 * FUNCTION: strsub
 * 
 * DESCRIPTION: fetches the substring found between the given indeces
 *
 * PARAMETERS: char* expr, size_t strt, size_t end, char* buff
 *
 * RETURNS: char* - buffer containing substring. NULL is returned if error occurs. 
 *
 * NOTE: if buff is not sized properly then a buffer overflow can occur (this string makes use of 
 * strcpy).
 */
char* strsub(char* expr, size_t strt, size_t end, char* buff);

#endif 
