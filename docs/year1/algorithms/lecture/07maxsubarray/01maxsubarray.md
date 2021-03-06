# The Maximum Sub-array Problem <!-- omit in toc -->

[~~Previous Chapter~~][prev] | [~~Next Chapter~~][next] | [Chapter Contents][index]

[prev]: ../05invariants/index
[next]: ../07maxsubarray/index
[index]: ../index

- [Definition of the problem](#definition-of-the-problem)
  - [Trivial Solution](#trivial-solution)
- [Divide and Conquer algorithm](#divide-and-conquer-algorithm)
  - [Code](#code)

## Definition of the problem

The maximum sub-array $S$ of an array $A$ is a subset of an array that satisfies the following property:

Select indices $i$ and $j$ of array $A$ of length $n$ such that $0\leq i\leq j\leq n-1$. Then, $\sum_{l=i}^{j} A[l]$ must be the maximum possible sum.

### Trivial Solution

Compute the sub-arrays for every pair possible of $i$ and $j$. There are $n^2$ possible pairs (ignoring order). Also, each sub-array would take $O(n)$ time to calculate Therefore the trivial runtime is $\sum_{pairs}O(n) = n^2\cdotp O(n) = O(n^3)$.

## Divide and Conquer algorithm

Consider that array $A$ can be considered the composition of two smaller arrays:

$$ A = L\circ R $$
$$\text{ where } L := \{A[k] : 0 \leq k \leq \lfloor n/2 \rfloor \}$$
$$\text{ and } R := \{ A[k] : \lfloor n/2 \rfloor + 1 \leq k \leq n - 1 \}$$

Three things can happen here. Either $S\subseteq L$, $S \subseteq R$ or $S \subseteq (L \circ R)$. In words, if the sub-array is completely in the left sub-array, completely in the right sub-array, or partially in both sub-arrays (or, if $i$ describes an index in $L$ and $j$ describes an index in $R$).

```python
algorithm max_sub_arr

Require an integer array `A` of length `n`

if n is 1 then
  return A

sub_one = max_sub_arr(A[0, n//2])
sub_two = max_sub_arr(A[n//2 + 1, n-1])
sub_thr = max_sub_arr(minimize_left(A) + minimize_right(A))

return heaviest(sub_one, sub_two, sub_thr)
```

### Code 

Non-optimised example code [here](./maxsubarr.py)
