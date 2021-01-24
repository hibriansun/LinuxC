#include <stdio.h>
#include <stdlib.h>

struct node{
	int data;
	struct node* next;
};


// 归并排序，无头结点，打头的就是首元节点
struct node*
sort(struct node* head){
    struct node* slow = head, *fast = head->next;   // slow位于第一个节点 fast位于**第二个**节点
	// 快慢指针找中点
    while(fast != NULL && fast != NULL){    // 这个条件都杜绝了fast->next为空而还要实现fast->next->next的段错误
        slow = slow->next;      // 走一步
        fast = fast->next->next;    // 走两步
    }

    // 此时：链表节点为奇数个时：slow正好在中心，fast为NULL       <以总数为3想象>
    // 链表节点为偶数个时：slow在中间两个的左边那个，fast在最后一个    <以总数为4想象>

    // 暂存中点下一个节点，作为二分后右半部这个新子链
    struct node* tmp = slow->next;
    
    // 二分！
    slow->next = NULL;
    struct node* left = sort(head);
    struct node* right = sort(tmp);

    // 排序交换

}

int
main()
{
	struct node* head = (struct node*)malloc(sizeof(struct node));
	struct node* ptr1, *ptr2;

	int datatmp;
	while(scanf("%d", &datatmp)){
		struct node* newnode = (struct node*)malloc(sizeof(struct node));
		newnode->data = datatmp;

		newnode->next = head->next;
		head->next = newnode;
	}

	head->next = sort(head->next);

    print(head->next);

}
