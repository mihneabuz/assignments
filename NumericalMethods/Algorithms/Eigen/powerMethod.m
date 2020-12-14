function [lambda x iter] = powerMethod(A, x0, tol, max_iter)
	iter = 0;
	while 1
		x = A * x0;
		x = x / norm(x);
		if (++iter == max_iter) || (norm(x - x0) < tol)
			lambda = x' * A * x;
			return;
		endif
		x0 = x;
	endwhile
endfunction
