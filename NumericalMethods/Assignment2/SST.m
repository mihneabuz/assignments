function x = SST(U, b)
	%Functie care rezolva sistemul Ux = b
	%unde U este matrice superior triunghiulara
    [n n] = size(U);
    x = zeros(n, 1);
    for i = n : -1 : 1
        s = 0;
        for j = i + 1 : n
            s = s + U(i, j) * x(j);
        endfor
        x(i) = (b(i) - s) / U(i, i);
    endfor
endfunction
