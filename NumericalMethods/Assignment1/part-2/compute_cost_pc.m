function [cost] = compute_cost_pc(points, centroids)
[n d] = size(points);
[c d] = size(centroids);
cost = 0;
for i = 1 : n
    min = sqrt(sum((points(i, :) - centroids(1, :)).^2));
    for j = 2 : c
        aux = sqrt(sum((points(i, :) - centroids(j, :)).^2));
        if (aux < min) 
            min = aux;
        endif
    endfor
    cost = cost + min;
endfor
endfunction

