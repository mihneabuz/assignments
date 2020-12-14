function [L U] = LU_fact(A)
	[n n] = size(A);
	L = eye(n);
	U = zeros(n);
	
	for p = 1 : n
		for j = p : n
			s = 0;
			for k = 1 : p-1
				s += L(p, k) * U(k, j);
			end
			U(p, j) = A(p,j) - s;
		end
		for i = p+1 : n
			s = 0;
			for k = 1 : p-1
				s += L(i, k) * U(k, p);
			end
			L(i, p) = (A(i, p) - s) / U(p, p);
		end
	end
end
