/*
 * shunt.h is part of cligraph.
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
 * @file shunt.h 
 *
 * these are all the functions that are required to process an expression into
 * an expression tree. The shunting yard algorithm is used to parse the
 * expressions.
 *
 * @author Noah Harvey (noah.harvey247@gmail.com)
 * @copyright GNU Public License 2
 */

#ifndef __shunt__
#define __shunt__

/** numerical type for all number computations 
 *
 * Use this as the data type for all plugins __(don't rely on all numbers to be
 * double)__.
 */
typedef long double num;

/** nodes for expression tree */
typedef struct exprnode_st
{
	num (*func)(num a,num b); /**< function pointer to operation function */
	num value; /**< a constant be held by this node, if the node is to represent a constant */
	struct exprnode_st* parent; /**< pointer to parent node, NULL if node is top of tree */
	struct exprnode_st* lchild; /**< pointer to left child node */
	struct exprnode_st* rchild; /**< pointer to right child node */
} EXPRNODE;

/**
 * @brief converts a string expression to its equivalent expression tree
 * 
 * This is the main interface between the UI and the plugins for functions. The
 * shunting yard algorithm is used to create the expression tree. Call this
 * function first before doing anything with functions (this function is the
 * core of the graphing plugin)
 *
 * @param expr - null terminated char array containing the expression tree
 *
 * @return EXPRNODE* - head of an expression tree
 * 
 */
EXPRNODE* mktree(char* expr);

/**
 * @brief fetches the next token in the given expression and creates an
 * EXPRNODE from it.
 *
 * @param expr - the expression to parse. This expression will be modified by
 * the function
 *
 * @return EXPRNODE* - a newly created EXPRNODE, if an error occures NULL is
 * returned
 * 
 */
EXPRNODE* gettoken(char* expr);

//mapping functions for EXPRNODE
num add(num a, num b);
num sub(num a, num b);
num mult(num a, num b);
num div(num a, num b);

#endif 

