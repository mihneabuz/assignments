function w = optimalW(A)
	%returneaza w optim pentru matricea A, sau 1 daca nu este poate fi calculat
	%daca w = 1 practic transforma metoda SOR in G-S
		
	N = diag(diag(A));
	P = N - A;
	Gj = inv(N) * P;
	e = eig(Gj);
	w = 1.0;
	
	if (max(abs(e)) >= 1)
		return;
	endif

	ok = 1;
	n = length(e);
	for i = 1 : n
		if (imag(e(i)) > 1e-5)
			ok = 0;
			break;
		endif
	endfor

	if ok
		u = max(abs(e));
		w = 1 + (u / (1 + sqrt(1 - u^2))) ^ 2;
	endif
endfunction
