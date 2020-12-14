function R = Iterative(nume, d, eps)
	% Functia care calculeaza matricea R folosind algoritmul iterativ.
	% Intrari:
	%	-> nume: numele fisierului din care se citeste;
	%	-> d: coeficentul d, adica probabilitatea ca un anumit navigator sa continue navigarea (0.85 in cele mai multe cazuri)
	%	-> eps: eruarea care apare in algoritm.
	% Iesiri:
	%	-> R: vectorul de PageRank-uri acordat pentru fiecare pagina.

	infile = fopen(nume, 'rt');
	n = fscanf(infile, '%d', 1);
	M = zeros(n);
	L = zeros(n, 1);
	K = ones(n, 1) * ((1 - d) / n);
	%M - matricea de adiacenta
	%L - L(i) = numarul linkurilor paginii i
	%K - coloana unitate * (1-d)/n

	%determinarea matricei de adiacenta
	for i = 1 : n
		curent = fscanf(infile, '%d', 1);
		L(curent) = fscanf(infile, '%d', 1);
		for j = 1 : L(curent)
			x = fscanf(infile, '%d', 1);
			M(x, curent) = 1;
		endfor
	endfor
	for i = 1 : n
		L(i) -= M(i, i); %eliminarea legaturilor i -> i
		M(i, i) = 0;
		M(:, i) = M(:, i) / L(i); %aduc M la forma necesara pt metoda iterativa
	endfor

	%calcularea iterativa a vectorul PR
	R_cur = ones(n, 1) * (1 / n);
	do
		R = R_cur;
		R_cur = d * M * R + K;
	until (norm(R - R_cur) < eps)
end
