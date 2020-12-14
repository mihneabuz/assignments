function [x iter] = Newton_Raphson(f, fderiv, x0, tol, max_iter)
	x = x0;
	iter = 0;
	while 1
		x = x - feval(f, x)/feval(fderiv, x);

		if ((++iter == max_iter))% || feval(f, x) <= tol)
			return;
		endif
	endwhile
endfunction
