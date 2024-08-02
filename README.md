# Algorithms for finding the k-th order statistic

A k-th order statistic of a sequence of elements with a total ordering
is the element which would be k-th if this sequence was to be sorted according to this ordering.

This problem could be solved by sorting (in time O(n log n) where n is the number of elements),
but faster algorithms exist (in time O(n), on average or even in the worst case). 

An important special case is finding the median, which corresponds to k being equal to n/2.

Classic algorithms, such as median of medians or quickselect, are often defined as _non-destructive_ ones,
that is, after execution they leave the sequence in a permuted state,
and they are often _in-place_, that is, they require less than a linear amount of additional memory,
often O(1) or O(log n).

This project aims at exploring what speed-ups are possible if we lift 
these two requirements: namely, we allow up to O(n) of additional memory to be pre-allocated and used,
and we also allow the original sequence to be completely destroyed.

The latter is not a problem in certain applications, where the sequence is generated
from some other structure, and the k-th order statistic is used to guide other operations.
An example of this approach is multi-dimensional divide-and-conquer algorithms.
