function y = Apartenenta(x, val1, val2)
	% Functia care primeste ca parametrii x, val1, val2 si care calculeaza valoarea functiei membru in punctul x.
	% Stim ca 0 <= x <= 1
	if x < val1
		y = 0;
   	else if x > val2
 		y = 1;
	else
		a = 1/(val2 - val1);
		b = 1 - a * val2;
		y = a * x + b;
	endif
end
