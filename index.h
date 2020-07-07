#ifndef INDEX_H
#define INDEX_H


variable_pos** CreateIndex( int);

void SetIndex(int, int, variable_pos* []);

variable_pos* SetIndex_LH(link_node*,variable_pos** , int[],map*, map**);

void FreeIndex(variable_pos**);
void FreeSearch(variable_pos**,int );

int CountSet(variable_pos**);

void CreateGroup(link_node* ,variable_pos** , map**, map**);

link_node* FindMembers( int, variable_pos**, link_node* ,int[], map*);

bool CheckSet( int , link_node* );

link_node* AddMember( int , link_node* );

link_node* RemoveDuplicateMembers( link_node** );
link_node* RemoveLastMember( link_node** );

link_node* RemoveThisMember(int , link_node**);

link_node* CopySet(int, variable_pos**, link_node*);

link_node* JoinSet( link_node* , link_node* );

link_node* AddKnownVariables(link_node*);

void ResetSolve();

void DeleteIndex(variable_pos** ,int);

void RemoveGroup( variable_pos**, int);

link_node* AddConnections(link_node* ,bool []);

int ExistInSet(int , link_node* );

void DeleteSet(link_node** );

int CountUniqueVariables(link_node* );

link_node* CollectConnections ( link_node* );

int* CreateArray(int);

void DestroyArray(int []);

void DestroySet(link_node*);

link_node* RemoveUntil(int , link_node** );

void RemoveClauseFromIndex(int, variable_pos**);

void RemoveVariableFromClause(int , int , variable_pos** );

link_node* RemoveDisjoint(link_node* , int*, map*, variable_pos** );

link_node* RemoveKnownVariable( link_node* );

void reduce( int );

variable_pos* SetNewIndex(link_node* ,variable_pos** ,int [], map* , map** );

link_node* TestRest( link_node* );

void FreeSet( link_node** , int);

link_node* RemoveInSet(int , link_node* , link_node** , int );

void CheckFirstNode( link_node*);
	
#endif
