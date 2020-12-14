function [x] = Jacobi_sparse(G_values, G_colind, G_rowptr, c, tol)
n = length(c);
x0 = zeros(n, 1);
x = c;
while( abs(x - x0) > tol )
	x0 = x;
	x = csr_multiplication(G_values, G_colind, G_rowptr, x) + c;
endwhile
endfunction
