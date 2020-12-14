function [values, colind, rowptr] = matrix_to_csr(A)
[n n] = size(A);
values = [];
colind = [];
rowptr = [];
for i = 1 : n
	ok = 1;
	for j = 1 : n
		if (A(i, j) != 0)
			values = [values A(i, j)];
			colind = [colind j];
			if (ok)
				rowptr = [rowptr length(values)];
				ok = 0;
			endif
		endif
	endfor
endfor
rowptr = [rowptr length(values)+1];
endfunction
