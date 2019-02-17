#include<stdio.h>
#include<malloc.h>
//��������� ���� ������
typedef  struct treeNode {
	int key;
	unsigned char treeHeight;
	struct treeNode* left;
	struct treeNode* right;
} treeNode;
//�������� ������ ����
treeNode *newTreeNode(int key){
	treeNode *node = (treeNode*)malloc(sizeof(treeNode));
	if (node == NULL){
		return NULL;
	}
	node->key = key;
	node->treeHeight = 1;
	node->left = 0;
	node->right = 0;
	return node;
}
//������ ������
unsigned char treeHeight(treeNode *tree){
	if (tree) return tree->treeHeight;
	else return 0;
}

//�������� ������
void treeHeightRecount(treeNode *node){
	unsigned char hLeft = treeHeight(node->left);
	unsigned char hRight = treeHeight(node->right);
	node->treeHeight = ((hLeft > hRight) ? (hLeft) : (hRight)) + 1;
}
//����� �������
treeNode* treeRotateLeft(treeNode* node){
	treeNode* tempNode = node->right;
	node->right = tempNode->left;
	tempNode->left = node;
	treeHeightRecount(node);
	treeHeightRecount(tempNode);
	return tempNode;
}
//������ �������
treeNode* treeRotateRight(treeNode* node){
	treeNode* tempNode = node->left;
	node->left = tempNode->right;
	tempNode->right = node;
	treeHeightRecount(node);
	treeHeightRecount(tempNode);
	return tempNode;
}
//������� ������-�������
int countBalanceFactor(treeNode* node){
	return treeHeight(node->right) - treeHeight(node->left);
}
//������������ ������
treeNode* treeBalance(treeNode* node){
	treeHeightRecount(node);
	if (countBalanceFactor(node) == 2){
		if (countBalanceFactor(node->right) < 0)
			node->right = treeRotateRight(node->right);
		return treeRotateLeft(node);
	}
	if (countBalanceFactor(node) == -2){
		if (countBalanceFactor(node->left)> 0)
			node->left = treeRotateLeft(node->left);
		return treeRotateRight(node);
	}
	return node;
}
//���������� ���� � ������
treeNode* treeInsert(treeNode* node, int key){
	if (!node) return newTreeNode(key);
	if (key < node->key)
		node->left = treeInsert(node->left, key);
	else
		node->right = treeInsert(node->right, key);
	return treeBalance(node);
}
//������������ ������
void treeFree(treeNode *node){
	if (node == NULL) return;
	treeFree(node->left);
	treeFree(node->right);
	free(node);
}
int main(){
	struct treeNode *tree = NULL;
	int N = 0;
	int node = 0;
	scanf("%d", &N);
	if (N >= 0 && N <= 2000000){
		while (N--){
			scanf("%d", &node);
			tree = treeInsert(tree, node);
		}
	}
	int height = treeHeight(tree);
	printf("%d", height);
	treeFree(tree);
	//_getch();
	return 0;
}