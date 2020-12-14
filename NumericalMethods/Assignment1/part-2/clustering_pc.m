function centroids = clustering_pc(points, NC)
[n d] = size(points);
centroids0 = [];
for i = 1 : NC
    aux = zeros(1, d);
    nr = 0;
    for j = i : NC : n
        aux = aux + points(j, :);
        nr++;
    endfor
    aux = aux/nr;
    centroids0 = [centroids0; aux];
endfor
iter = 0;
do
	if (iter)
		centroids0 = centroids;
	endif
	centroids = zeros(NC, d);
	nrpoints = zeros(NC, 1);
	for i = 1 : n
		min = sqrt(sum((points(i, :) - centroids0(1, :)).^2));
		minind = 1;
		for j = 2 : NC
			dist = sqrt(sum((points(i, :) - centroids0(j, :)).^2));
			if (dist < min)
				min = dist;
				minind = j;
			endif
		endfor
		centroids(minind, :) = centroids(minind, :) + points(i, :);
		nrpoints(minind)++;
	endfor
	for i = 1 : NC
		centroids(i, :) = centroids(i, :) / nrpoints(i);
	endfor
	iter++;
until( norm(centroids - centroids0) < 1e-5 )
endfunction
