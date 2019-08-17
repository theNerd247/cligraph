/*
 * shunt.c is part of cligraph.
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

/**
 * @file shunt.c 
 *
 * @author Noah Harvey (noah.harvey247@gmail.com)
 * @copyright GNU Public License 2
 */

#include "shunt.h"

EXPRNODE* gettoken(char* expr)
{
	if(!expr) return NULL;

	//find start of next token 
}


num add(num a, num b)
{
	return a+b;
}

num sub(num a, num b)
{
	return a-b;
}

num mult(num a, num b)
{
	return a*b;
}

num div(num a, num b)
{
	return a/b;
}
