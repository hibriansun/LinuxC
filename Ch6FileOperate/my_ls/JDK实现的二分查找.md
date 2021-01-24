```java
public static int binarySearch(int[] a, int fromIndex, int toIndex, int key) {
    rangeCheck(a.length, fromIndex, toIndex);
    return binarySearch0(a, fromIndex, toIndex, key);
}

private static int binarySearch0(int[] a, int fromIndex, int toIndex, int key) {
    int low = fromIndex;
    int high = toIndex - 1;

    while(low <= high) {
        int mid = low + high >>> 1;		// >>>表示无符号右移，也叫逻辑右移，即若该数为正，则高位补0，而若该数为负数，则右移后高位同样补0
        int midVal = a[mid];
        if (midVal < key) {
            low = mid + 1;
        } else {
            if (midVal <= key) {
                return mid;
            }

            high = mid - 1;
        }
    }

    return -(low + 1);
}
```

