#pragma once
#include "includer.h"
#include "Figure.h"

bool CrashCheck_AABB(float a_midx, float a_midy, float a_midz, float a_sizex, float a_sizey, float a_sizez,
	float b_midx, float b_midy, float b_midz, float b_sizex, float b_sizey, float b_sizez); // 기본 충돌 체크 함수

bool Shuffle_Cube(Cube* cube, int cubeSize);

class Data {
private:
public:
	int val[100000000];
};

class MyNode {
private:
	Data* data;
	MyNode* next;
	MyNode* prev;
public:
	MyNode() {
		data = NULL;
		next = NULL;
		prev = NULL;
	}
	void SetData(Data* d) { data = d; }
	void SetNext(MyNode* n) { next = n; }
	void SetPrev(MyNode* p) { prev = p; }
	Data* GetData() { return data; }
	MyNode* GetNext() { return next; }
	MyNode* GetPrev() { return prev; }

	~MyNode() {
		delete data;
	}
};

class LinkedList {
private:
	MyNode* node;

public:
	LinkedList() {
		node = NULL;
	}

	void push(int val) {
		MyNode* NewNode = new MyNode;
		Data* NewData = new Data;
		NewData->val[0] = val;
		NewNode->SetData(NewData);
		if (node == NULL) {
			node = NewNode;
			NewNode->SetNext(NULL);
			NewNode->SetPrev(NULL);
		}
		else {
			NewNode->SetNext(NULL);
			NewNode->SetPrev(node);
			node->SetNext(NewNode);
			node = NewNode;
		}
	}
	int pop() {
		if (node != NULL) {
			MyNode* temp = node;
			node = node->GetPrev();
			if (node != NULL) { node->SetNext(NULL); }
			int t = temp->GetData()->val[0];
			delete temp;
			return t;
		}
		else {
			return -1;
		}
	}
};