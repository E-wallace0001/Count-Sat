#ifndef INDEX_H
#define INDEX_H

#include "map.h"

struct variable_pos** CreateIndex( int);

void SetIndex(int, int, struct variable_pos* []);


void FreeIndex(struct variable_pos**);
void FreeSearch(struct variable_pos**,int );

int CountSet(struct variable_pos**);

void CreateGroup(link_node* ,struct variable_pos** ,  struct map**,struct map**,com_line*);

link_node* FindMembers( int, struct variable_pos**, link_node* ,int[],struct map*,com_line*);

bool CheckSet( int , link_node* );

link_node* AddMember( int , link_node*,com_line*);

link_node* RemoveDuplicateMembers( link_node**,com_line*);
link_node* RemoveLastMember( link_node**, com_line* );

link_node* RemoveThisMember(int , link_node**, com_line*);

link_node* CopySet(int, struct variable_pos**, link_node*,com_line*);

link_node* JoinSet( link_node* , link_node* );

link_node* AddKnownVariables(link_node*, com_line*);

void ResetSolve(com_line*);

void DeleteIndex(struct variable_pos** ,int);

void RemoveGroup( struct variable_pos**, int);

link_node* AddConnections(link_node* ,bool [],com_line*);

int ExistInSet(int , link_node* );

void DeleteSet(link_node**, com_line* );

int CountUniqueVariables(link_node* ,com_line* );

link_node* CollectConnections ( link_node*, com_line*);

link_node* ConnectedVariables ( int variable, com_line* Coms);

int* CreateArray(int);

void DestroyArray(int [],com_line*);

void DestroySet(link_node*, com_line*);

link_node* RemoveUntil(int , link_node**, com_line*);

void RemoveClauseFromIndex(int, struct variable_pos**, com_line*);

void RemoveVariableFromClause(int , int , struct variable_pos** );

link_node* RemoveDisjoint(link_node* , int*,struct map*, struct variable_pos** );

link_node* RemoveKnownVariable( link_node*, com_line* );

void reduce( int );

struct variable_pos* SetNewIndex(link_node* , struct variable_pos** ,int [],struct map* , struct map**, com_line*);

link_node* TestRest( link_node*,com_line*);

void FreeSet( link_node** , int,com_line*);

link_node* RemoveInSet(int , link_node* , link_node** , int,com_line* );

void CheckFirstNode( link_node*);

link_node** CreateSet(int, com_line* );
	
#endif
