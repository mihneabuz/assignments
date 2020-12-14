function y = functiederiv(x)
	y = 3 * x .^ 2 - 2 * sin(x * 2) + e .^ (x - 2) + 1 / (2 * sqrt(x + 15));
endfunction
