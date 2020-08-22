
using namespace std;

#define MAX_DEGREE 35//max. item of each block

struct Chunk
{
	uint64_t key[MAX_DEGREE]={0};
	int val[MAX_DEGREE] = { 0 };
	Chunk() {
		for (int i = 0; i < MAX_DEGREE; ++i) {
			key[i] = ULLONG_MAX;
		}
	}
};
struct Block
{
	bool is_leaf;
	int size;//size of block
	Chunk chunk;//include hash value and PBA
	struct Block* child[MAX_DEGREE + 1];
	struct Block* parent;
	struct Block* next;
	Block() {
		size = 0;
		is_leaf = false;
		parent = nullptr;
		next = nullptr;
		for (int i = 0; i < MAX_DEGREE+1; ++i) {
			child[i] = nullptr;
		}
	}
};
Block *root = nullptr; //the B+ tree, declare as global variable
void printBlock(Block *p) {
	if (p == nullptr)
		return;
	if (p->is_leaf) {
		for (int i = 0; i < MAX_DEGREE ; i++)
			if (i<p->size)
				cout << p->chunk.key[i] << "&" << p->chunk.val[i] << " ";
			else
				cout << "__ ";
	}
	else {
		for (int i = 0; i < MAX_DEGREE ; i++) {
			if (i<p->size)
				cout << p->chunk.key[i] << " ";
			else
				cout << "__ ";
		}
	}
	cout << "| ";
}
void printTree(queue<Block *> q, int level) {
	if (q.front() == nullptr) {
		cout << "is empty\n";
		return;
	}
	queue<Block *>tmp;
	cout << "\n*****level: " << level << "*****\n | ";
	while (q.size() != 0) {
		cout << "size" << q.front()->size << " ";
		printBlock(q.front());
		for (int i = 0; i < MAX_DEGREE+1; i++) {
			if (q.front()->child[i] != nullptr) {
				tmp.push(q.front()->child[i]);
			}
		}
		q.pop();
	}
	if (tmp.size() > 0)
		printTree(tmp, level + 1);
}

Block *search(uint64_t key) {
	Block *p = root;
	if (root == nullptr) {
		cout << "tree is empty\n";
		return nullptr;
	}
	//cout <<"path: ";
	bool fit = false;
	while (!p->is_leaf) {
		for (int i = 0; i < MAX_DEGREE ; i++) {
			//cout <<p->chunk.key[i]<<">";
			if (key < p->chunk.key[i] || key == p->chunk.key[i]) {
				fit = true;
				p = p->child[i];
				break;
			}
		}
		if (!fit)
			p = p->child[MAX_DEGREE - 1];
		//not in the tree -> return a parent (is_leaf==0)
		if (p == nullptr) {
			p = p->parent;
			break;
		}
	}
	return p;
	//return nullptr;
}
void adjust(Block * node, uint64_t key, int val) {
	int split = (MAX_DEGREE) / 2;
	Block * current = node;
	Block * prev = node->parent;
	while (true ) {
		uint64_t oldkey[MAX_DEGREE+1] ;
		uint64_t oldval[MAX_DEGREE+1] ;
		uint64_t target;
		Block ** oldBlock = new Block*[MAX_DEGREE + 2];
		for (int i = 0; i < MAX_DEGREE + 1; i++)
			oldBlock[i] = nullptr;

		int pos = -1;
		int ppos = -1;
	
		if (prev == nullptr) {
			//cout << "creat new root\n";
			root = prev = new Block();
			current->parent = prev;
			prev->child[0] = current;
		}

		for (int j = 0, i = 0; i < MAX_DEGREE; i++) {
			if ((key < current->chunk.key[i] || key == current->chunk.key[i]) && j == 0 && node==current) {
				pos = i;
				oldkey[i] = key;
				oldval[i] = val;
				j++;
			}
			oldval[i + j] = current->chunk.val[i];
			oldkey[i + j] = current->chunk.key[i];
		}
		target = oldkey[split];
		for (int i = 0; i < MAX_DEGREE; i++) {
			if (target < prev->chunk.key[i] || target == prev->chunk.key[i]) {
				ppos = i;
				break;
			}
		}
		//cout << "pos: " << pos << endl;
		//cout << "ppos: " << ppos << endl;
		/*for (int i = 0; i < MAX_DEGREE+1; i++) {
			cout << oldkey[i] << " ";
		}*/
		for (int j = 0, i = 0; i < MAX_DEGREE+1; i++) {
				oldBlock[i+j] = prev->child[i];
			if (i == ppos) {
				oldBlock[i + 1] = new Block();
				if (current->is_leaf)
					oldBlock[i + 1]->is_leaf = true;
				else {
					for (int k=0,j = split+1; j < MAX_DEGREE+1; k++,j++) {
						oldBlock[i + 1]->child[k] = current->child[j];
						current->child[j]->parent = oldBlock[i + 1];
						current->child[j] = nullptr;
					}
				}
				oldBlock[i + 1]->parent = prev;
				oldBlock[ppos + 1]->next = oldBlock[i]->next;
				oldBlock[i]->next = oldBlock[i + 1];

				j++;
			}
		}

		for (int i = 0; i< MAX_DEGREE+1; i++) {
  			if (i == ppos) {
				for (int j = MAX_DEGREE + 1; j >=i; j--)
					prev->chunk.key[j] = prev->chunk.key[j - 1];
				prev->chunk.key[i] = target;
			}
		}
		for (int i = 0; i < MAX_DEGREE + 1; i++) {
			prev->child[i] = oldBlock[i];
		}
			current->size = 0;
		if (current->is_leaf) {
			for (int i = 0; i < MAX_DEGREE; i++) {
				//old block
				if (i < split) {
					current->chunk.key[current->size] = oldkey[i];
					current->chunk.val[current->size] = oldval[i];
					current->size++;
				}
				//new block
				else {
					oldBlock[ppos + 1]->chunk.key[oldBlock[ppos + 1]->size] = oldkey[i];
					oldBlock[ppos + 1]->chunk.val[oldBlock[ppos + 1]->size] = oldval[i];
					oldBlock[ppos + 1]->size++;
				}
			}
		}
		else {
			for (int j=0,i = 0; i < MAX_DEGREE ; i++) {
				//old block
				if (i < split) {
					prev->child[ppos]->chunk.key[prev->child[ppos]->size] = oldkey[i];
					prev->child[ppos]->size++;
					if(oldkey[i] == target)
						j++;
				}
				else if (i==split)
					continue;
				//new block
				else {
					prev->child[ppos + 1]->chunk.key[prev->child[ppos + 1]->size] = oldkey[i];
					prev->child[ppos + 1]->size++;
				}
			}
		}
		prev->size++;
		if (prev->size == MAX_DEGREE) {
			key = prev->chunk.key[split];
			current = current->parent;
			prev = prev->parent;
		}
		else
			break;
	}
}
void insert(uint64_t key, int val) {
	//case:size of tree is 0 ->build a new block
	//cout << "\n=================insert=================" << key << endl << endl;
	if (root == nullptr) {
		//in this case, root is the leaf node
		root = new Block();
		root->size++;
		root->is_leaf = true;
		root->chunk.key[0] = key;

		root->chunk.val[0] = val;
		return;
	}
	Block *position = search(key);
	//whether the position is leaf
	if (!position->is_leaf) {
		int i = 0;
		for (i = 0; i < MAX_DEGREE - 1; i++) {
			if (key < position->chunk.key[i] || key == position->chunk.key[i])
				break;
		}
		if (position->child[i] == nullptr) {
			position->child[i] = new Block();
			if (i != 0)
				position->child[i - 1]->next = position->child[i];
			position->child[i]->parent = position->child[i];
		}
		position = position->child[i];
	}
	//whether the leaf is not full
	if (position->size != MAX_DEGREE - 1) {
		for (int i = 0; i < MAX_DEGREE - 1; i++) {
			if (key < position->chunk.key[i] || key == position->chunk.key[i]) {
				for (int j = MAX_DEGREE - 1; j > i; j--) {
					position->chunk.key[j] = position->chunk.key[j - 1];
					position->chunk.val[j] = position->chunk.val[j - 1];
				}
				position->chunk.key[i] = key;
				position->chunk.val[i] = val;
				break;
			}
		}

		position->size++;
	}
	//whether the leaf is full -> adjust
	else {
		//position->size++;
		//cout << "adjust\n";
		adjust(position, key, val);
	}

}
