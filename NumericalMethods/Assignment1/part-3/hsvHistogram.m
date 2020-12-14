function [sol] = hsvHistogram(path_to_image, count_bins)
map = imread(path_to_image);
	[n m d] = size(map);
	sol = zeros(1, count_bins*3);
	for i = 1 : n
		for j = 1 : m
		R = cast(map(i, j, 1), "double");
		G = cast(map(i, j, 2), "double");
		B = cast(map(i, j, 3), "double");
		[H S V] = RGB2HSV(R, G, B); 
		H = fix(H / 1.01 * count_bins);
		S = fix(S / 1.01 * count_bins);
		V = fix(V / 1.01 * count_bins);
		sol(H + 1)++;
		sol(S + count_bins + 1)++;
		sol(V + count_bins * 2 + 1)++;
		endfor
	endfor
end
