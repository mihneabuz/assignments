function [x] = SIT(A, b)
	[n n] = size(A);
	x = zeros(n, 1);
	x(1) = b(1) / A(1, 1);
	for i = 2 : n
		sum = A(i, 1 : i-1) * x(1 : i-1);
		x(i) = (b(i) - sum) / A(i, i);
	endfor
endfunction
