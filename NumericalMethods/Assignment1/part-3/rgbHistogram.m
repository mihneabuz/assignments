function [sol] = rgbHistogram(path_to_image, count_bins)
	map = imread(path_to_image);
	[n m d] = size(map);
	x = 0;
	sol = zeros(1, count_bins*3);
	for i = 1 : n
		for j = 1 : m
			for color = 1 : 3
				x = cast(map(i, j, color), "double");
				x = fix(x * count_bins / 256);
				sol(x + (color - 1)*count_bins + 1)++;
			endfor
		endfor
	endfor
end
