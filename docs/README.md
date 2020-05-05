# 5x5 and Gaussian Elimination

 I am going to show why the game 5x5 can be solved using Gaussian Elimination. 

## Recall the rules

First, let us recall the rules. We have a 5x5 board, where each entry is either black (0) or white (1)

- each entry is initially black
- the goal is to do a series of operations so that every entry is white.
- Every operation has a center, and the result of the operation is that the center itself and the entries around it will flip color:
	- here we choose the boxed entry as the center of the operation, and as a result, the center itself and the four entries around it has their color flipped.
  $$
  \begin{bmatrix}
  0 & 0 & 0 &0 &0\\
  0 & 0 & 0 &0 &0\\
  0 & 0 & \boxed{0} &0 &0\\
  0 & 0 & 0 &0 &0\\
  0 & 0 & 0 &0 &0\\
  \end{bmatrix}

  \Longrightarrow

  \begin{bmatrix}
  0 & 0 & 0 &0 &0\\
  0 & 0 & 1 &0 &0\\
  0 & 1 & \boxed{1} &1 &0\\
  0 & 0 & 1 &0 &0\\
  0 & 0 & 0 &0 &0\\
  \end{bmatrix}
  $$
  
  - we can go on to do another operation, whose center is boxed in the figure below. 

  $$
  \begin{bmatrix}
  0 & 0 & 0 &0 &0\\
  0 & 0 & 1 &0 &0\\
  0 & 1 & 1 &1 &\boxed0\\
  0 & 0 & 1 &0 &0\\
  0 & 0 & 0 &0 &0\\
  \end{bmatrix}
  \Longrightarrow
  

  \begin{bmatrix}
  0 & 0 & 0 &0 &0\\
  0 & 0 & 1 &0 &1\\
  0 & 1 & 1 &0 &\boxed1\\
  0 & 0 & 1 &0 &1\\
  0 & 0 & 0 &0 &0\\
  \end{bmatrix}
  $$
  
  - Note that one entry has its color flipped back to zero. And this center has only 3 adjacent entries, because the center is on the boundary.

## Key Observations

1. We can use 0 and 1 to indicate black and white (which we already did)

2. **The order of the operation does not matter.** (toggling pos1 then pos2 is the same as toggling pos2 and then pos1)

3. We can use a variable $X_{ij}$ to indicate the number of times we toggle the entry $(i,j)$. There're 25 variables, one for each entry.

4. **Toggling twice on the same location cancels the effect.**

5. Therefore all $X_{ij} \in \{0,1\}$. Now we have 25 boolean variables. 

6. We can also formulate the goal as 25 equations! Each equation is for an entry, and the equation says that entry must be 1 at the end. Note that one entry $(i,j)$ is affected by (at most) 5 variables

   1. $(i,j)$ is affected by $X_{i,j}$,$X_{i-1,j}$, $X_{i+1,j}$, $X_{i,j-1}$, $X_{i,j+1}$
   2. I said "at least" because for entries on the boundary, there're only 3 or 4 related variables.
      1. corner: 3 variables
      2. Side: 4 variables

7. Variables add up through $XOR$

8. Therefore for a non-boundary entry $(i,j)$, the equation is

   $$
   X_{i,j} \oplus X_{i-1,j} \oplus X_{i+1,j} \oplus X_{i,j-1} \oplus X_{i,j+1} = 1
   $$
   
9. We can also take in to account the current color of an entry $(i,j)$, denoted as $C_{i,j} \in \{0,1\}$. Then the equation becomes

	$$
	\begin{aligned}
	C_{i,j}\oplus X_{i,j} \oplus X_{i-1,j} \oplus X_{i+1,j} \oplus X_{i,j-1} \oplus X_{i,j+1} 
	&= 1\\
	X_{i,j} \oplus X_{i-1,j} \oplus X_{i+1,j} \oplus X_{i,j-1} \oplus X_{i,j+1} 
	&= 1 \oplus C_{i,j}\\
	X_{i,j} \oplus X_{i-1,j} \oplus X_{i+1,j} \oplus X_{i,j-1} \oplus X_{i,j+1} 
	&= \neg C_{i,j}\\
	\end{aligned}
	$$

10. We have 25 variables and 25 equations. We can put this into a matrix form and solve it using Gaussian Elimination under $\Z_2$

## 2x2 Example

Let me use the 2x2 example to show the full matrix. 5x5 and nxn are generally the same.

In 2x2, we have 4 variables
$$
\begin{bmatrix}
X_{11}&X_{12}\\
X_{21}&X_{22}
\end{bmatrix}
$$
Let us rename the variable using row-major order
$$
\begin{bmatrix}
X_{1}&X_{2}\\
X_{3}&X_{4}
\end{bmatrix}
$$
And the equations are
$$
\begin{aligned}
X_1 &\oplus X_2 &\oplus X_3 & &= \neg C_1\\
X_1 &\oplus X_2 &&\oplus X_4 &= \neg C_2\\
X_1 &&\oplus X_4 &\oplus X_4 &= \neg C_3\\
    &X_2&\oplus X_3 &\oplus X_4 &= \neg C_4\\

\end{aligned}
$$
Its matrix form is
$$
\begin{bmatrix}
1 & 1 & 1 & 0\\
1 & 1 & 0 & 1\\
1 & 0 & 1 & 1\\
0 & 1 & 1 & 1\\
\end{bmatrix} \cdot
\begin{bmatrix}
X_1 \\
X_2 \\
X_3 \\
X_4 
\end{bmatrix} = \neg
\begin{bmatrix}
C_1\\
C_2\\
C_3\\
C_4
\end{bmatrix}
$$
Note that all multiplications are $AND$ and additions are $XOR$.

Let us solve this with an empty board (that is, all C are 0). The augmented form is:
$$
\begin{bmatrix}
1 & 1 & 1 & 0&1\\
1 & 1 & 0 & 1&1\\
1 & 0 & 1 & 1&1\\
0 & 1 & 1 & 1&1\\
\end{bmatrix}
$$
We apply **Gauss-Jordan Elimination** to get a reduced row echelon form:

1. First choose the first row as the pivot and reduce the 2nd and 3rd row
   $$
   \begin{bmatrix}
   1 & 1 & 1 & 0&1\\
   0 & 0 & 1 & 1&0\\
   0 & 1 & 0 & 1&0\\
   0 & 1 & 1 & 1&1\\
   \end{bmatrix}
   $$
2. Choose the 3rd row as the pivot and swap it with the 2nd row

   $$
   \begin{bmatrix}
   1 & 1 & 1 & 0&1\\
   0 & 1 & 0 & 1&0\\
   0 & 0 & 1 & 1&0\\
   0 & 1 & 1 & 1&1\\
   \end{bmatrix}
   $$

3. Reduce the 1st and 4th row using 2nd row as the pivot

   $$
   \begin{bmatrix}
   1 & 0 & 1 & 1&1\\
   0 & 1 & 0 & 1&0\\
   0 & 0 & 1 & 1&0\\
   0 & 0 & 1 & 0&1\\
   \end{bmatrix}
   $$

4. Choose the 3rd row as the pivot and reduce 1st and 4th row

   $$
   \begin{bmatrix}
   1 & 0 & 0 & 0&1\\
   0 & 1 & 0 & 1&0\\
   0 & 0 & 1 & 1&0\\
   0 & 0 & 0 & 1&1\\
   \end{bmatrix}
   $$

5. Choose the 4th row as the pivot and reduce the 2nd and 4rd row

   $$
   \begin{bmatrix}
   1 & 0 & 0 & 0&1\\
   0 & 1 & 0 & 0&1\\
   0 & 0 & 1 & 0&1\\
   0 & 0 & 0 & 1&1\\
   \end{bmatrix}
   $$

This gave us a nice identity matrix on the left, which might not always be the case (I will explain how to handle this later). But for now, the reduced row-echelon form tells us the unique solution is
$$
X_1=X_2=X_3=X_4=1
$$
Which means, if the initial 2x2 board is empty, we should toggle everything exactly once.

## Optimization

Note that in the procedures above, given a 2x2 board, the 4x4 matrix on the left is always the same no matter what the current board (the C vector) is. 

That is to say, when the current board changes a little bit and we apply the solver, the only thing that's different is the rightmost column, and the matrix on the left always undergoes the same operations.

We can reduce the matrix, only once, at the beginning of the game, and keep track of what operations it undergoes, and then every time a C vector comes in, we directly apply those operations on C.

There are only 2 types of operation

- swap i j:   swap row i with row j
- reduce i j:  row j = row j   xor. row i

The significance in doing this is that

- for nxn board, we have $n^2$ variables
- Gaussian Elimination takes $\mathcal O(n^6)$ time
- yet it only emits $\mathcal O(n^4)$ operations
- therefore when a C vector comes in, we have reduce the time to $\mathcal O(n^4)$ 

## What if the matrix is singular

We need to consider 2 more cases:

- no solution

  $$
  \begin{bmatrix}
  1 & 0 & 0 & 0&1\\
  0 & 1 & 0 & 0&1\\
  0 & 0 & 1 & 0&1\\
  0 & 0 & 0 & \boxed0&\boxed1\\
  \end{bmatrix}
  $$
	
	In this case the solver should return no solution. This might happen if a board is initially not all-empty.
	
- non-unique solutions / free variables

  $$
  \begin{bmatrix}
  1 & 0 & 0 & 0&1\\
  0 & 1 & 0 & 0&1\\
  0 & 0 & 1 & 0&1\\
  0 & 0 & 0 & \boxed0&\boxed0\\
  \end{bmatrix}
  $$
  
	In this case $X_4$ is a free variable, and can be either 0 or 1. We can choose to minimize the number of toggles (one of my week 3 goal), so we can assign all free variable zero. 