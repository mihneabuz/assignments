function [x iter] = Jacobi(A, b, tol, max_iter)
	[n n] = size(A);

	N = diag(diag(A));
	P = N - A;

	G = inv(N) * P;
	c = inv(N) * b;
	x0 = zeros(n, 1);

	if (max(abs(eig(G))) >= 1)
		x = NaN;
		iter= NaN;
		return;
	endif

	iter = 0;
	while 1
		x = G * x0 + c;

		if (++iter == max_iter) || (norm(x - x0) < tol)
			return;
		endif

		x0 = x;
	endwhile
endfunction
