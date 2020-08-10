#include <bits/stdc++.h>

using namespace std;

#define MAX_DEGREE 4//max. item of each block

struct Chunk
{
	uint64_t key[MAX_DEGREE];
	int val[MAX_DEGREE]={0};
	Chunk (){
		for (int i = 0; i < MAX_DEGREE; ++i){
			key[i]=ULLONG_MAX;
		}
	}
};
struct Block
{
	bool is_leaf;
	int size;//size of block
	Chunk chunk;//include hash value and PBA
	struct Block* child[MAX_DEGREE+1];
	struct Block* parent;
	struct Block* next;
	Block(){
		size=0;
		is_leaf=false;
		parent=nullptr;
		next=nullptr;
		for (int i = 0; i < MAX_DEGREE; ++i){
			child[i]=nullptr;
		}
	}
};
Block *root=nullptr; //the B+ tree, declare as global variable
void printBlock(Block *p){
	if(p==nullptr)
		return;
	if(p->is_leaf){
		for (int i=0;i<MAX_DEGREE-1;i++)
			if (p->chunk.key[i]!=ULLONG_MAX)
				cout <<p->chunk.key[i]<<"&"<<p->chunk.val[i]<<" ";
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
	if(q.front()==nullptr){
		cout << "is empty\n";
		return;
	}
	queue<Block *>tmp;
	cout <<"\n*****level: "<<level<<"*****\n | ";
	while(q.size()!=0){
		cout <<"size"<<q.front()->size<<" ";
		printBlock(q.front());
		for(int i=0;i<MAX_DEGREE;i++){
			if(q.front()->child[i]!=nullptr){
				tmp.push(q.front()->child[i]);
			}
		}
		q.pop();
	}
	if(tmp.size()>0)
		printTree(tmp,level+1);
}

Block *search(uint64_t key){
	Block *p=root;
	if(root==nullptr){
		cout <<"tree is empty\n";
		return nullptr;
	}
	//cout <<"path: ";
	bool fit=false;
	while(!p->is_leaf){
		for (int i=0;i<MAX_DEGREE-1;i++){
			//cout <<p->chunk.key[i]<<">";
			if(key<p->chunk.key[i] || key==p->chunk.key[i] ){
				fit=true;
					p=p->child[i];
				break;
			}
		}
		if(!fit)
			p=p->child[MAX_DEGREE-1];
		//not in the tree -> return a parent (is_leaf==0)
		if(p==nullptr){
			p=p->parent;
			break;
		}
	}
			return p;
	//return nullptr;
}
void adjust(Block * node,uint64_t key,int val){
	int split=(MAX_DEGREE)/2;
	Block * current=node;
	Block * prev=node->parent;	
	while(current->size==MAX_DEGREE-1){
		uint64_t oldkey[MAX_DEGREE]={ULLONG_MAX};
		uint64_t oldval[MAX_DEGREE]={ULLONG_MAX};
		uint64_t target;
		Block ** oldBlock=new Block*[MAX_DEGREE+1];

		int pos=-1;

		target=oldkey[split];
		if(prev==nullptr){
			cout <<"creat new root\n";
			root=prev=new Block();			
			current->parent=prev;
		}
		for (int j=0,i=0;i<MAX_DEGREE-1;i++){
			if((key<current->chunk.key[i] ||key==current->chunk.key[i]) && j==0){
				pos=i;
				oldkey[i]=key;
				oldval[i]=val;
				j++;
			}
			oldval[i+j]=current->chunk.val[i];
			oldkey[i+j]=current->chunk.key[i];
			if(i==MAX_DEGREE-2&&j==0){
				oldkey[i+1]=key;
				oldval[i+1]=val;
			}
		}
		cout<<"pos: "<<pos<<endl;
		for (int i=0;i<MAX_DEGREE;i++){
			cout <<oldkey[i]<<" ";
		}
		for (int j=0,i=0;i<MAX_DEGREE;i++){
			oldBlock[i]=prev->child[i];
			if(i==pos){
				oldBlock[i+1]=new Block();
				if(current->is_leaf)
					oldBlock[i+1]->is_leaf=true;
				oldBlock[i+1]->parent=prev;
				oldBlock[i]->next=oldBlock[i+1];
				j++;
			}
		}
		for(int i=0;i<MAX_DEGREE;i++)
			prev->child[i]=oldBlock[i];
		if(current->is_leaf){
			for(int i=0;i<MAX_DEGREE;i++){
				//old block
				if(i < split){
					current->chunk.key[current->size]=oldkey[i];
					current->chunk.val[current->size]=oldval[i];
					current->size++;
				}
				//new block
				else{
					prev->child[pos+1]->chunk.key[prev->child[pos+1]->size]=oldkey[i];
					prev->child[pos+1]->chunk.val[prev->child[pos+1]->size]=oldval[i];
					prev->child[pos+1]->size++;
				}
			}
		}
		else{
			for(int i=0;i<MAX_DEGREE-1;i++){
					//old block
					if(i<split){
						prev->child[pos]->chunk.key[prev->child[pos]->size]=oldkey[i];
						prev->child[pos]->size++;
					}
					else if(i==split)
						continue;
					//new block
					else{
						prev->child[pos+1]->chunk.key[prev->child[pos+1]->size]=oldkey[i];
						prev->child[pos+1]->size++;
					}
				}
		}
		prev->size++;
		current=current->parent;
		prev=prev->parent;
	}
}
void insert(uint64_t key,int val){
	//case:size of tree is 0 ->build a new block
	cout <<"\n============insert============"<<key<<endl<<endl;
	if (root==nullptr){
		//in this case, root is the leaf node
		root=new Block();
		root->size++;
		root->is_leaf=true;
		root->chunk.key[0]=key;

		root->chunk.val[0]=val;
		return;
	}
	Block *position=search(key);
	//whether the position is leaf
	if(!position->is_leaf){
		int i=0;
		for (i=0;i<MAX_DEGREE-1;i++){
			if(key<position->chunk.key[i] || key==position->chunk.key[i])
				break;
		}
		if(position->child[i]==nullptr){
			position->child[i]=new Block();
			if(i!=0)
				position->child[i-1]->next=position->child[i];
			position->child[i]->parent=position->child[i];
		}
		position=position->child[i];
	}
	//whether the leaf is not full
	if(position->size!=MAX_DEGREE-1){
		for (int i=0;i<MAX_DEGREE-1;i++){
			if(key<position->chunk.key[i] || key==position->chunk.key[i]){
				for (int j=MAX_DEGREE-1;j>i;j--){
					position->chunk.key[j]=position->chunk.key[j-1];
					position->chunk.val[j]=position->chunk.val[j-1];
				}
				position->chunk.key[i]=key;
				position->chunk.val[i]=val;
				break;
			}
		}

		position->size++;
	}
	//whether the leaf is full -> adjust
	else{
		//position->size++;
		cout <<"adjust\n";
		adjust(position,key,val);
	}
	queue<Block *> q;
	q.push(root);
	printTree(q,1);
	cout <<endl;
}

int main(){
	/*
	Block bb;
	cout <<"size of block is "<<sizeof(bb )<<endl;
	cout <<"size of chunk is "<<sizeof(bb.chunk )<<endl;
	cout <<"size of block* is "<<sizeof(bb.parent )<<endl;
	cout <<"size of block* is "<<sizeof(bb.child )<<endl;
	cout <<"size of bool is "<<sizeof(bb.is_leaf )<<endl;
	*/
	queue<Block *>q;
	q.push(root);
	printTree(q,1);
	insert(1,1);
	insert(2,2);
	insert(3,3);
	insert(2,2);
	insert(3,3);
	insert(4,4);
	insert(5,5);
	insert(1,1);

	insert(1,1);


	/*build a tree artificially
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
	*/

	/*
	queue<Block *>q;
	q.push(root);
	printTree(q,1);
	*/

	//insert(1,1);
	/*
	Block *find=search(5);
	for (int i=0;i<MAX_DEGREE-1;i++){
		if(find!=nullptr){
			if(find->chunk.key[i]==ULLONG_MAX)
				continue;
			cout <<find->chunk.key[i]<<" "<<find->chunk.val[i]<<" ";
		}
	}
	*/
}










