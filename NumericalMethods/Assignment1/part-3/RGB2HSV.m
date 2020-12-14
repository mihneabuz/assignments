function [H S V] = RGB2HSV(R, G, B)
R = R / 256;
G = G / 256;
B = B / 256;	
Cmax = max([R G B]);
Cmin = min([R G B]);
D = Cmax - Cmin;
if (D == 0)
	H = 0;
else
	if (Cmax == R)
		H = 60 * mod((G-B)/D, 6);
	else if (Cmax == G)
		H = 60 * ((B-R)/D + 2);
	else if (Cmax == B)
		H = 60 * ((R-G)/D + 4);
	endif
	endif
	endif
endif
H = H / 360;
if (Cmax == 0)
	S = 0;
else
	S = D/Cmax;
endif
V = Cmax;
endfunction
