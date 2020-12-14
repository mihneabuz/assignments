function [A, b] = generate_probabilities_system(rows)
n = rows * (rows + 1) / 2;
A = zeros(n);
b = zeros(n, 1);
for i = n : -1 : n-rows+1
	b(i) = 1;
endfor
A(1, 1) = 4;
A(1, 2) = -1;
A(1, 3) = -1;
AUX = [1 zeros(1, rows-1)];
s = 1;
for i = 2 : rows
	line = [(s + (1 : i)) zeros(1, rows-i)];
	s = s + i;
	AUX = [AUX; line];
endfor
for i = 2 : rows-1
	A(AUX(i, 1), AUX(i-1, 1)) = -1;
	A(AUX(i, 1), AUX(i+1, 1)) = -1;
	A(AUX(i, 1), AUX(i+1, 2)) = -1;
	A(AUX(i, 1), AUX(i, 2)) = -1;
	A(AUX(i, 1), AUX(i, 1)) = 5;
	for j = 2 : i-1
		A(AUX(i, j), AUX(i-1, j-1)) = -1;
		A(AUX(i, j), AUX(i-1, j)) = -1;
		A(AUX(i, j), AUX(i+1, j)) = -1;
		A(AUX(i, j), AUX(i+1, j+1)) = -1;
		A(AUX(i, j), AUX(i, j-1)) = -1;
		A(AUX(i, j), AUX(i, j+1)) = -1;
		A(AUX(i, j), AUX(i, j)) = 6;
	endfor
	j = i;
	A(AUX(i, j), AUX(i-1, j-1)) = -1;
	A(AUX(i, j), AUX(i+1, j)) = -1;
	A(AUX(i, j), AUX(i+1, j+1)) = -1;
	A(AUX(i, j), AUX(i, j-1)) = -1;
	A(AUX(i, j), AUX(i, j)) = 5;
endfor
i = rows;
A(AUX(i, 1), AUX(i-1, 1)) = -1;
A(AUX(i, 1), AUX(i, 2)) = -1;
A(AUX(i, 1), AUX(i, 1)) = 4;
for j = 2 : i-1
	A(AUX(i, j), AUX(i-1, j-1)) = -1;
	A(AUX(i, j), AUX(i-1, j)) = -1;
	A(AUX(i, j), AUX(i, j-1)) = -1;
	A(AUX(i, j), AUX(i, j+1)) = -1;
	A(AUX(i, j), AUX(i, j)) = 5;
endfor
j = i;
A(AUX(i, j), AUX(i-1, j-1)) = -1;
A(AUX(i, j), AUX(i, j-1)) = -1;
A(AUX(i, j), AUX(i, j)) = 4;
endfunction
