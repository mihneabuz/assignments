function B = PR_Inv(A)
	% Functia care calculeaza inversa matricii A folosind factorizari Gram-Schmidt
	% Aplic algoritmul Gran-Schmidt modificat pentru a descompune matricea A
	% in Q R, care verifica proprietatile:
	% 1) Q^-1 = Q'
	% 2) R matrice superior triunghiulara

	[n n]=size(A);
	Q = A;
	R = zeros(n, n);
	for i = 1 : n
		R(i, i) = norm(Q(1 : n, i));
		Q(1 : n, i) = Q(1 : n, i) / R(i, i);
		for j = i + 1 : n
			R(i, j) = Q(1 : n, i)' * Q(1 : n, j);
			Q(1 : n, j) = Q(1 : n, j) - Q(1 : n, i) * R(i, j);
		endfor
	endfor

	% Ecuatia devine:
	% A * xi = ei -> Q * R * xi = ei -> R * xi = Q^t * ei
	Q = Q';
	E = eye(n);
	B = zeros(n);
	for i = 1 : n
		B(:, i) = SST(R, Q*E(:, i));
	endfor
end
