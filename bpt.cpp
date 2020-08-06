#include <bits/stdc++.h>

using namespace std;

#define MAX_DEGREE 4//max. item of each block

struct Chunk
{
	uint64_t key[MAX_DEGREE-1];
	int *val;
	Chunk (){
		for (int i = 0; i < MAX_DEGREE-1; ++i){
			key[i]=ULLONG_MAX;
		}
	}
};
struct Block
{
	bool is_leaf;
	int size;//size of block
	Chunk chunk;//include hash value and PBA
	struct Block* child[MAX_DEGREE];
	struct Block* parent;
	Block(){
		size=0;
		is_leaf=false;
		parent=nullptr;
		for (int i = 0; i < MAX_DEGREE; ++i){
			child[i]=nullptr;
		}
	}
};
Block *root=nullptr; //the B+ tree, declare as global variable
Block *search(uint64_t key){
	Block *p=root;
	if(root==nullptr){
		cout <<"tree is empty\n";
		return nullptr;
	}
	cout <<"path: ";
	bool fit=false;
	while(!p->is_leaf){
		for (int i=0;i<MAX_DEGREE-1;i++){
			cout <<p->chunk.key[i]<<">";
			if(key<p->chunk.key[i] || key==p->chunk.key[i] ){
				fit=true;
					p=p->child[i];
				break;
			}
		}
		if(!fit)
			p=p->child[MAX_DEGREE-1];
		//not in the tree -> return a nullptr
		if(p==nullptr)
			break;
	}
			return p;
	//return nullptr;
}
void insert(uint64_t key,int val){
	//case:size of tree is 0 ->build a new block
	
	if (root==nullptr){
		//in this case, root is the leaf node
		root=new Block();
		root->size++;
		root->is_leaf=true;
		root->chunk.key[0]=key;
		root->chunk.val=new int[MAX_DEGREE-1]();
		root->chunk.val[0]=val;
		return;
	}
	//case:block is not full
	if(root->size!=MAX_DEGREE){

	}
	//case:block is full
	else{

	}
	root->size++;








}
void printBlock(Block *p){
	if(p==nullptr)
		return;
	if(p->is_leaf){
		for (int i=0;i<MAX_DEGREE-1;i++)
			if (p->chunk.key[i]!=ULLONG_MAX)
				cout << p->chunk.key[i]<<"&"<<p->chunk.val[i]<<" ";
			else 
				cout << "__ ";
	}
	else{
		for (int i=0;i<MAX_DEGREE-1;i++){
			if (p->chunk.key[i]!=ULLONG_MAX)
				cout << p->chunk.key[i]<<" ";
			else
				cout<<"__ ";
		}
	}
	cout <<"| ";
}
void printTree(queue<Block *> q,int level){
	queue<Block *>tmp;
	cout <<"\n*****level : "<<level<<"*****\n | ";
	while(q.size()!=0){
		printBlock(q.front());
		for(int i=0;i<MAX_DEGREE;i++){
			if(q.front()->child[i]!=nullptr)
				tmp.push(q.front()->child[i]);
		}
		q.pop();
	}
	if(tmp.size()>0)
		printTree(tmp,level+1);
}

int main(){
	Block bb;
	cout <<"size of block is "<<sizeof(bb )<<endl;
	cout <<"size of chunk is "<<sizeof(bb.chunk )<<endl;
	cout <<"size of block* is "<<sizeof(bb.parent )<<endl;
	cout <<"size of block* is "<<sizeof(bb.child )<<endl;
	cout <<"size of bool is "<<sizeof(bb.is_leaf )<<endl;

	/*build a tree artificially*/
	root=new Block();
	root->is_leaf=false;
	root->chunk.key[0]=12;
	root->chunk.key[1]=15;
	root->chunk.key[2]=18;
	root->child[0]=new Block();
	root->child[1]=new Block();
	root->child[2]=new Block();
	root->child[3]=new Block();
	root->child[0]->chunk.key[0]=10;
	root->child[0]->chunk.key[1]=11;
	root->child[0]->chunk.key[2]=12;
	root->child[1]->chunk.key[0]=13;
	root->child[1]->chunk.key[1]=14;
	root->child[1]->chunk.key[2]=15;
	root->child[2]->chunk.key[0]=16;
	root->child[2]->chunk.key[1]=17;
	root->child[2]->chunk.key[2]=18;
	root->child[3]->chunk.key[0]=19;
	root->child[3]->chunk.key[1]=20;
	root->child[3]->chunk.key[2]=21;
	root->child[0]->child[0]=new Block();
	root->child[0]->child[0]->is_leaf=true;
	root->child[0]->child[0]->chunk.val=new int[MAX_DEGREE-1]();
	root->child[0]->child[0]->chunk.val[0]=9;
	root->child[0]->child[0]->chunk.key[0]=9;
	root->child[0]->child[0]->chunk.val[1]=8;
	root->child[0]->child[0]->chunk.key[1]=8;
	

	/*queue<Block *>q;
	q.push(root);
	printTree(q,1);*/
	//insert(1,1);
	Block *find=search(5);
	for (int i=0;i<MAX_DEGREE-1;i++){
		if(find!=nullptr){
			if(find->chunk.key[i]==ULLONG_MAX)
				continue;
			cout <<find->chunk.key[i]<<" "<<find->chunk.val[i]<<" ";
		}
	}
}










