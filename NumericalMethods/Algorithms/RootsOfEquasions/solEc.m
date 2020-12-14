function x = solEc(f, a, b)
	max_iter = fix(log2(b - a));
	tol = 1e-15;
	x = bisectie(f, a, b, tol, max_iter);
	x = secanta(f, x, tol, 20);
endfunction
