#include<iostream>
using namespace std;
#define mod 1000000007

//Korzystam z https://en.wikipedia.org/wiki/Splay_tree
struct splay{
	splay *parent;
	splay *left, *right;
	int lsize = 0, rsize = 0;
	bool reversed;
	char value;
	unsigned long long GG, GR, RG, RR;
};

splay *newNode(char c){
	splay* node = new splay();
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	node->reversed = false;
	node->value = c;
	node->lsize = 0;
	node->rsize = 0;
	if(c == 'G')
		node->GG = 1;
	else
		node->GG = 0;
	node->GR = 0;
	node->RG = 0;
	if(c == 'R')
		node->RR = 1;
	else
		node->RR = 0;

	return node;
}

void swapSons(splay * node){
	if(!node)
		return;
	if(node->reversed){
		splay *x = node->left;
		int size = node->lsize;
		int rg = node->RG;
		node->left = node->right;
		node->right = x;
		node->lsize = node->rsize;
		node->rsize = size;
		if(node->left)
			node->left->reversed = !node->left->reversed;
		if(node->right)
			node->right->reversed = !node->right->reversed;
		node->reversed = false;
		node->RG = node->GR;
		node->GR = rg;
	}
}

void countValue(splay *node){
	if(!node)
		return;
	splay *left = node->left;
	splay *right = node->right;

	unsigned long long lgr = 0, lgg = 0, lrg = 0, lrr = 0;
	unsigned long long rgr = 0, rgg = 0, rrg = 0, rrr = 0;
	int llsize = -1, lrsize = 0, rlsize = -1, rrsize = 0;
	if(left){
		lgr = left->GR;
		lgg = left->GG;
		lrg = left->RG;
		lrr = left->RR;
		llsize = left->lsize;
		lrsize = left->rsize;
	}
	if(right){
		rgr = right->GR;
		rgg = right->GG;
		rrg = right->RG;
		rrr = right->RR;
		rrsize = right->rsize;
		rlsize = right->lsize;
	}

	if(node->value == 'G'){
		node->GG = lgr*rrg*2 + lgg + rgg + 1 + lgr + rrg + lgr*rgg + lgg*rrg;
		node->GR = lgr*rgr + lgr + rgr + lgg*rrr + rrr + lgr*rrr*2 ;
		node->RG = lrg*rrg + lrg + rrg + lrr*rrg*2 + lrr*rgg + lrr;
		node->RR = lrg*rrr + lrr + rrr + lrr*rrr*2 + lrr*rgr;
	}
	else{
		node->GG = ((lgr*rrg) + (lgr*rgg) + (lgg*rrg))*2 + (lgg*rgg) + lgg + rgg;
		node->GR = (lgr + lgr*(rgr + rrr) + (lgg*rrr))*2 + (lgg*rgr) + rgr + lgg;
		node->RG = lrg + (rrg + lrr*(rrg + rgg) + (lrg*rrg)) * 2 + (lrg * rgg) + rgg;
		node->RR = (lrr + rrr + lrr*(rgr + rrr) + (lrg*rrr)) * 2 + (lrg*rgr) + lrg + rgr + 1;
	}

	node->GG%=mod;
	node->GR%=mod;
	node->RG%=mod;
	node->RR%=mod;
	node->lsize = llsize + lrsize + 1;
	node->rsize = rrsize + rlsize + 1;
}

splay* rightRotate(splay *node){
	swapSons(node);
	splay *left = node->left;
	swapSons(left);
	swapSons(node->right);
	if(left){
		swapSons(left->right);
		swapSons(left->left);
		node->left = left->right;
		if(left->right)
			left->right->parent = node;
		left->parent = node->parent;
		if(node->parent){
			if(node->parent->left == node)
				node->parent->left = left;
			else
				node->parent->right = left;
		}
		node->parent = left;
		left->right = node;
		countValue(node);
		countValue(left);

		return left;
	}

	return node;
}

splay* leftRotate(splay *node){
	swapSons(node);
	splay *right = node->right;
	swapSons(right);
	swapSons(node->left);
	if(right){
		swapSons(right->left);
		swapSons(right->right);
		node->right = right->left;
		if(right->left)
			right->left->parent = node;
		right->parent = node->parent;
		if(node->parent){
			if(node->parent->left == node)
				node->parent->left = right;
			else
				node->parent->right = right;
		}
		node->parent = right;
		right->left = node;
		countValue(node);
		countValue(right);

		return right;
	}

	return node;
}

splay* splay2(splay *node){
	while(node->parent){
		swapSons(node->parent);
		swapSons(node->parent->parent);
		if(!node->parent->parent){
			if(node->parent->left == node)
				rightRotate(node->parent);
			else
				leftRotate(node->parent);
		}
		else if(node->parent->left == node && node->parent->parent->left == node->parent){
			rightRotate(node->parent->parent);
			rightRotate(node->parent);
		}
		else if(node->parent->right == node && node->parent->parent->right == node->parent){
			leftRotate(node->parent->parent);
			leftRotate(node->parent);
		}
		else if(node->parent->right == node && node->parent->parent->left == node->parent){
			leftRotate(node->parent);
			rightRotate(node->parent);
		}
		else{
			rightRotate(node->parent);
			leftRotate(node->parent);
		}
	}
	return node;
}

splay* find(int i, splay *node){
	splay * result = NULL;
	while(node){
		swapSons(node);
		if(i == node->lsize + 1 || (!node->right && i > node->lsize)){
			result = node;
			break;
		}
		else if(i > node->lsize){
			i = i - node->lsize - 1;
			node = node->right;
		}
		else{
			node = node->left;
		}
	}
	splay2(result);
	return result;
}

void insert(splay *node, splay *root){
	while(root->right){
		root->rsize++;
		root = root->right;
	}

	root->rsize++;
	root->right = node;
	node->parent = root;
	splay2(node);
}

splay* splitAndJoin(splay *root, int i, int j, bool reverse){
	splay *node = find(i, root);
	splay *left = node->left;
	if(left){
		left->parent = NULL;
		node->left = NULL;
		countValue(node);
	}

	splay *node2 = find(j-i+1, node);
	splay *right = node2->right;

	if(right){
		right->parent = NULL;
		node2->right = NULL;
		countValue(node2);
	}

	if(reverse)
		node2->reversed = true;
	else
		cout<<(node2->GR + node2->RG + node2->GG + node2->RR)%mod<<endl;

	splay *jNode, *jNode2;
	if(left){
		jNode = find(left->lsize+ left->rsize +1, left);
		jNode->right = node2;
		node2->parent = jNode;
		countValue(jNode);
	}

	if(right){
		jNode2 = find(1, right);
		if(left){
			jNode2->left = jNode;
			jNode->parent = jNode2;
		}
		else{
			jNode2->left = node2;
			node2->parent = jNode2;
		}
		countValue(jNode2);
		return jNode2;
	}

	if(left)
		return jNode;

	return node2;
}

splay* query(splay *root, int i, int j){
	return splitAndJoin(root, i, j, false);
}

splay* reverse(splay * node, int i, int j){
	return splitAndJoin(node, i, j, true);
}

int main(){
	int n,m;
	cin>>n>>m;
	string szereg;
	cin>>szereg;

	splay *node = newNode(szereg[0]);
	for(unsigned i = 1; i < szereg.size(); i++){
		splay* node2 = newNode(szereg[i]);
		insert(node2, node);
		node = node2;
	}

	char z;
	int i,j;

	for(int w = 0; w < m; w++){
		cin>>z>>i>>j;
		if(z == 'O')
			node = reverse(node, i, j);
		else
			node = query(node, i, j);
	}
}
