#ifndef INDEX_H
#define INDEX_H


variable_pos** CreateIndex( int);

void SetIndex(int, int, variable_pos* []);

variable_pos* SetIndex_LH(link_node*,variable_pos** , int[],map*);

void FreeIndex(variable_pos**);
void FreeSet(variable_pos**);
void FreeSearch(variable_pos**);

int CountSet(variable_pos**);

void CreateGroup(link_node* ,variable_pos** , int[], map**);

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

#endif
