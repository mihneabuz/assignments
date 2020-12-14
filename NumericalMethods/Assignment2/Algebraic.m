function R = Algebraic(nume, d)
	% Functia care calculeaza vectorul PageRank folosind varianta algebrica de calcul.
	% Intrari:
	%	-> nume: numele fisierului in care se scrie;
	%	-> d: probabilitatea ca un anumit utilizator sa continue navigarea la o pagina urmatoare.
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
		M(:, i) = M(:, i) / L(i); %aduc matricea m la forma necesara
	endfor

	%calcularea PR cu formula algrebrica
	R = PR_Inv(eye(n) - d * M) * K;
end

