function [x iter] = SOR(A, b, w, tol, max_iter)
	
	[n n] = size(A);
	if w == 0
		w = optimalW(A)
	endif

	N = diag(diag(A)) + tril(A, -1) * w;
	P = diag(diag(A)) * (1 - w) - triu(A, 1) * w;
	N = inv(N);
	
	G = N * P;
	c = w * N * b;

	x = zeros(n, 1);
	iter = 0;
	while 1
		x0 = x;
		x = G * x0 + c;

		if (++iter == max_iter) || (norm(x - x0) < tol)
			return;
		endif
	endwhile
endfunction
