/*
 * cligraph.c is part of cligraph.
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
 * cligraph.h 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: header for plugin management prototypes
 */

//--PLUGIN CONFIG------------------------------

#define PLUGIN_PATH "plugin"
#define DL_LOAD_FLAG (RTLD_LAZY || RTLD_GLOBAL)

//--END PLUGIN CONFIG---------------------------


/*
 * FUNCTION: getfuncref
 * 
 * DESCRIPTION: returns a reference to a function located in another plugin
 *
 * PARAMETERS: char* libname, char* funcname
 *
 * RETURNS: void* - void pointer to function. NULL is returned if error occurs
 * (funcname or libname are invalid names or trouble getting funcrefrence).
 * 
 */
void* getfuncref(char* libname, char* funcname);
