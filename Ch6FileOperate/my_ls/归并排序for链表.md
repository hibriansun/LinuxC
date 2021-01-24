# 链表的归并排序

## 思路

[![r1u77D.md.png](https://s3.ax1x.com/2020/12/16/r1u77D.md.png)](https://imgchr.com/i/r1u77D)

对链表不断递归重复：找中点，两边二分

直到分到子链表只有两个节点，进行比较大小判断交换与否，之后自底向上进行合并

> 递归：
>
> ​	写在递归调用之前的式子随着递归自顶向下到递归终点地执行
>
> ​	写在递归调用之后的式子随着递归自底向上到第一次调用地执行

* **难点：合并**

  双指针法合并，建立辅助ListNode h 作为头部。
  设置两指针 left, right 分别指向两链表头部，比较两指针处节点值大小，由小到大加入合并链表头部，指针交替前进，直至添加完两个链表。
  返回辅助ListNode h 作为头部的下个节点 h.next。
  时间复杂度 O(l + r)，l, r 分别代表两个链表长度。

## 视频演示

https://www.youtube.com/watch?v=JSceec-wEyw

## 代码

```java
class Solution {
    public ListNode sortList(ListNode head) {
        if (head == null || head.next == null)
            return head;
        ListNode fast = head.next, slow = head;
        while (fast != null && fast.next != null) {
            slow = slow.next;
            fast = fast.next.next;
        }
        ListNode tmp = slow.next;
        slow.next = null;
        ListNode left = sortList(head);
        ListNode right = sortList(tmp);
        ListNode h = new ListNode(0);
        ListNode res = h;
        while (left != null && right != null) {
            // 谁小h.next连谁
            if (left.val < right.val) {
                h.next = left;	 
                left = left.next;
            } else {
                h.next = right;
                right = right.next;
            }
            h = h.next;
        }
        h.next = left != null ? left : right;
        return res.next;
    }
}
```