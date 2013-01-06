/* 
 * Author: Sandeep Singh
 * Date  : Jan 6, 2013
 * 
 * Copyright 2013 Sandeep Singh (sandeepsinghmails@gmail.com)
 *
 * This file is part of Dictionary Search Application.
 * Fragmentation Remover is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Dictionary Search Application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Dictionary Search Application. If not, see <http://www.gnu.org/licenses/>.
 *
 * To Compile:
 * $ gcc Dictionary_Search_Application.c -ansi -Wall -pedantic -o Dictionary_Search_Application
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ALPHABETS	26		/* Permitted Characters: 'a' to 'z' */
#define MAX_BUFFLEN		15
#define SUCCESS 		1
#define FAILURE			0
#define SET				1
#define QUIT_SEARCH_OP  2

typedef struct node
{
	char 	data;
	bool 	end_of_word_flag;				
	int 	num_word_occurrences;			/* For Use in the Next Release */
	struct node *next_node[MAX_ALPHABETS];
} TrieNode;

typedef struct 
{
	char 			search_result;
	unsigned int 	last_match_buff_index;
	TrieNode		*last_match_trienode_ptr;
} trie_search_result;

void 				build_dictionary (TrieNode **root);

void				insert_operation_trie (	TrieNode **root,
											char 	*input_word,
											int		index);

trie_search_result 	search_dictionary (TrieNode *root);


trie_search_result  search_operation_trie (	TrieNode *root,
											char 	 *buffer,
											int 	 index);

int main()
{
	TrieNode *root = NULL;
	
	trie_search_result search_output;

	/* Build Up the Dictionary */
	build_dictionary(&root);

	/* Word Search in Trie */
	while (1)
	{
		search_output = search_dictionary (root);

		if (SUCCESS == search_output.search_result)
		{
			printf ("\nMatch Found !\n");
		}
		else if (QUIT_SEARCH_OP == search_output.search_result)
		{
			printf ("\nDictionary Search Complete !\n");
			break;
		}
		else
		{
			printf ("\nNo Match !\n");
		}
	}

	return 0;
}

void build_dictionary (TrieNode **root)
{
	char 	buffer [MAX_BUFFLEN];

	/* Request for User Input */	
	while(1)
	{
		printf ("\nEnter Dictionary Word (or ':q' to exit)\n");
		scanf  ("%s", buffer);

		if (0 == strncmp(buffer, ":q", 2))
		{
			break;
		}

		insert_operation_trie (root, buffer, 0);
	}
}

void insert_operation_trie (TrieNode **root,
							char 	*input_word,
							int		index)
{
	TrieNode *newnode = NULL;
	TrieNode *currentnode = *root;

	trie_search_result search_output;

	printf ("\nIndex = %d\n", index);

	if (NULL == *root)
	{
		*root = malloc (sizeof(TrieNode));
		if (NULL == *root)
		{	
			fprintf (stdout, "\nMemory Allocation Failed. File:%s, Line:%d\n",
								__FILE__, __LINE__);

			exit (FAILURE);
		}
		currentnode = *root;

	}
	else if ((0 == index)	
		   &&(NULL != currentnode->next_node [input_word[0]%'a'])) 
	{
		search_output = search_operation_trie 
								(currentnode->next_node[input_word[0]%'a'], input_word+1, 1);
	
		index = search_output.last_match_buff_index;
		currentnode  = search_output.last_match_trienode_ptr;

		input_word += index;
	}

	if (input_word[0] == '\0')
	{
		return;
	}

	newnode = malloc (sizeof(TrieNode));
	if (NULL == newnode)
	{	
		fprintf (stdout, "\nMemory Allocation Failed. File:%s, Line:%d\n",
							__FILE__, __LINE__);

		exit (FAILURE);
	}

	newnode->data = input_word[0];
	printf ("\nInserted: %c\n", newnode->data);

	if (input_word[1] == '\0')
	{
		newnode->end_of_word_flag = SET;
		printf ("\nEnd-of-Word Flag = SET\n");
	}

	currentnode->next_node[(newnode->data)%'a'] = newnode;

	if (index+1 <= MAX_BUFFLEN)
	{
		insert_operation_trie (&(currentnode->next_node[(newnode->data)%'a']), input_word+1, index+1); 
	}
}

trie_search_result search_dictionary (TrieNode *root)
{
	char buffer [MAX_BUFFLEN];
	trie_search_result search_output;

	printf ("\nEnter the Word To Search (or ':q' to Quit): " );
	scanf  ("%s", buffer);

	if (0 == strncmp(buffer, ":q", 2))
	{
		search_output.search_result = QUIT_SEARCH_OP;
		return search_output;			
	}
	else
	{
		return search_operation_trie (root, buffer, 0);
	}
}

trie_search_result search_operation_trie (TrieNode *root,
							char 	 *buffer,
							int 	 index)
{
	trie_search_result search_output;

	if (NULL == root)
	{
		search_output.search_result 			= FAILURE;
		search_output.last_match_buff_index 	= index;
		search_output.last_match_trienode_ptr	= root;
		return search_output;										
	}

	if (buffer[1] == '\0')
	{
		if (	(NULL != root->next_node[buffer[0]%'a']) 
			&&	(root->next_node[buffer[0]%'a']->end_of_word_flag == SET)
			&&  (root->next_node[buffer[0]%'a']->data == buffer[0]))
		{
			printf ("\nAlphabet Match: %c\n", buffer[0]);
			search_output.search_result 			= SUCCESS;
			search_output.last_match_buff_index 	= index;
			search_output.last_match_trienode_ptr	= root;
			return search_output;								
		}
		else
		{
			search_output.search_result				 = FAILURE;
			search_output.last_match_buff_index 	 = index;
			search_output.last_match_trienode_ptr	 = root;
			return search_output;									
		}
	}
	else if (index+1 <= MAX_BUFFLEN)
	{
		if (NULL != root->next_node [buffer[0]%'a'])
		{
			printf ("\nAlphabet Match: %c\n", buffer[0]);
			return search_operation_trie((root->next_node [buffer[0]%'a']), buffer+1, index+1);
		}
		else  
		{
			search_output.search_result 				= FAILURE;
			search_output.last_match_buff_index 		= index;
			search_output.last_match_trienode_ptr		= root;
			return search_output;										
			
		}
	}
	else	
	{
		search_output.search_result 				= FAILURE;
		search_output.last_match_buff_index 		= index;
		search_output.last_match_trienode_ptr		= root;
		return search_output;										
	}
}







	
