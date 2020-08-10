int split=(MAX_DEGREE)/2;
Block * current=node;
Block * prev=node->parent;	
while(current->size==MAX_DEGREE-1){
		uint64_t oldkey[MAX_DEGREE]={};
		uint64_t oldval[MAX_DEGREE]={};
		uint64_t target;
		for (int i=0;i<MAX_DEGREE;i++)
			oldval[i]=oldkey[i]=ULLONG_MAX;

		Block * oldBlock[MAX_DEGREE]={nullptr};

		for (int j=0,i=0;i<MAX_DEGREE-1;i++){
			if((key<current->chunk.key[i] ||key==current->chunk.key[i]) && j==0){
				oldkey[i]=key;
				oldval[i]=val;
				j++;
			}
			oldBlock[i]=current->child[i];
			oldval[i+j]=current->chunk.val[i];
			oldkey[i+j]=current->chunk.key[i];
			if(i==MAX_DEGREE-2&&j==0){
				oldkey[i+1]=key;
				oldval[i+1]=val;
			}
		}
		oldBlock[MAX_DEGREE-1]=current->child[MAX_DEGREE-1];

		for (int i=0;i<MAX_DEGREE;i++){
			cout <<oldkey[i]<<" ";
		}

		target=oldkey[split];

		if (prev==nullptr){
			cout <<"creat new root\n";
			root=prev=new Block();
			prev->chunk.key[0]=target;			
			current->parent=prev;
			prev->child[0]=current;
			prev->child[1]=new Block();
			prev->child[1]->parent=prev;
			if(prev->child[0]->is_leaf){
				prev->child[1]->is_leaf=true;
				prev->child[1]->chunk.val=new int[MAX_DEGREE];
				for (int i=0;i<MAX_DEGREE-1;i++){
					prev->child[0]->chunk.val[i]=0;
				}
			}
			prev->child[1]->size=prev->child[0]->size=0;
			for (int i=0;i<MAX_DEGREE-1;i++){
				prev->child[0]->chunk.key[i]=ULLONG_MAX;
			}
			prev->size++;
		}
		else{
			int pos=-1;
			for (i=0;i<MAX_DEGREE;i++)
				if(prev->chunk.key[i]<target || prev->chunk.key[i]==target){
					pos=i;
					break;
				}
			for (int i=0,j=0;i<MAX_DEGREE;i++){
				if(pos==i){
					prev->chunk.key[i]=target;
					j++;
				}
				prev->chunk.key[i+j]=prev->chunk.key[i];
			}
			prev->child[prev->size-1]=current;
			prev->child[prev->size]=new Block();
			prev->child[prev->size]->parent=prev;
			if(prev->child[prev->size-1]->is_leaf){
				prev->child[prev->size]->is_leaf=true;
				prev->child[prev->size]->chunk.val=new int[MAX_DEGREE];
				for (int i=0;i<MAX_DEGREE-1;i++){
					prev->child[prev->size-1]->chunk.val[i]=0;
				}
			}
			for (int i=0;i<MAX_DEGREE-1;i++){
				prev->child[prev->size-1]->chunk.val[i]=0;
				prev->child[prev->size-1]->chunk.key[i]=ULLONG_MAX;
			}
			prev->child[prev->size]->size=prev->child[prev->size-1]->size=0;
			prev->size++;
		}

			/******************/
			
			/******************/
			if(prev->child[prev->size-1]->is_leaf){
				for(int i=0;i<MAX_DEGREE;i++){
					//old block
					if(i < split){
						prev->child[prev->size-1]->chunk.key[prev->child[prev->size-1]->size]=oldkey[i];
						prev->child[prev->size-1]->chunk.val[prev->child[prev->size-1]->size]=oldval[i];
						prev->child[prev->size-1]->size++;
					}
					//new block
					else{
						prev->child[prev->size]->chunk.key[prev->child[prev->size]->size]=oldkey[i];
						prev->child[prev->size]->chunk.val[prev->child[prev->size]->size]=oldval[i];
						prev->child[prev->size]->size++;
					}
				}
			}
			else{
				for(int i=0;i<MAX_DEGREE-1;i++){
						//old block
						if(i<split){
							prev->child[prev->size-1]->chunk.key[prev->child[prev->size-1]->size]=oldkey[i];
							prev->child[prev->size-1]->size++;
						}
						else if(i==split)
							continue;
						//new block
						else{
							prev->child[prev->size]->chunk.key[prev->child[prev->size]->size]=oldkey[i];
							prev->child[prev->size]->size++;
						}
					}
			}
		current=current->parent;
		prev=prev->parent;
	}