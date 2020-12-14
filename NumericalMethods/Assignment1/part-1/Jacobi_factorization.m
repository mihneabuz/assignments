function [G_J, c_J] = Jacobi_factorization(A, b)
[n n] = size(A);
G_J = zeros(n);
c_J = zeros(n, 1);
for i = 1 : n
	G_J(i, :) = A(i, :)/A(i, i);
	c_J(i) = b(i)/A(i, i) * -1;
	G_J(i, i) = 0;
endfor
G_J = G_J * -1;
c_J = c_J * -1;
endfunction
