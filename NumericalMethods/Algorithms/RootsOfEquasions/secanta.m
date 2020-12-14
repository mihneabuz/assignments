function [x iter] = secanta(f, x0, tol, max_iter)
	y = feval(f, x0);
	%gasesc o a doua aproximatie x1 plecand de la x0
	if (y > 0)
		if (feval(f, x0 - 0.1) < y)
			x1 = x0 - 0.1;
		else
			x1 = x0 + 0.1
		endif
	else
		if (feval(f, x0 + 0.1 < y))
			x1 = x0 + 0.1;
		else
			x1 = x0 - 0.1;
		endif
	endif

	%iteratia
	iter = 0;
	while 1
		x = x1 - ((x0 - x1) / (feval(f, x0) - feval(f, x1))) * feval(f, x1);

		x0 = x1;
		x1 = x;
		if ((++iter == max_iter) || (abs(x1 - x0) < tol))
			return
		endif
	endwhile
endfunction
