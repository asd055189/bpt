#include <bits/stdc++.h>
#include "bpt.h"
using namespace std;
int main() {
	vector <int> arr;
	srand(time(NULL));
	for (int i = 0; i < 5000; i++) {
		int random = rand() % 3000 ;
		arr.push_back(random);
		insert(random, random);

	}
	Block *B = search(0);
	int index = 0;
	sort(arr.begin(),arr.end());
	while (B != nullptr) {
		for (int i = 0; i < MAX_DEGREE; i++) {
			if (i<B->size) {
				if(B->chunk.key[i]!= arr[index])
					cout << B->chunk.key[i] << " " << arr[index]<<endl;
					index++;
			}
		}
		B = B->next;
	}
	queue<Block *>q;
	q.push(root);
	printTree(q,0);
	cout << endl;
	if(index==5000)
		cout << "index" << index<<endl<<"============correct============";
	
}