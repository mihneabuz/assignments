function [x iter] = bisectie(f, a, b, tol, max_iter)
	x = 0;
	if (feval(f, a) * feval(f, b) >= 0) 
		return;
	end

	iter = 0;
	while 1
		x = (a + b) / 2;
		if (feval(f, a) * feval(f, x) <= 0)
			b = x;
		else
			a = x;
		endif

		
		if (++iter == max_iter) || (b - a < tol)
			return;
		endif
	endwhile
endfunction
