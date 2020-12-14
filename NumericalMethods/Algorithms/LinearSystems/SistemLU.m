function [x] = SistemLU(A, b)
	if (det(A) == 0)
		return;
	endif
	[L U] = LU_fact(A);
	x = SIT(L, b); 
	x = SST(U, x);
endfunction
