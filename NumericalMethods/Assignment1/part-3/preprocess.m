function [X, y] = preprocess(path_to_dataset, histogram, count_bins)
	path_to_cats = strcat(path_to_dataset, "cats/*.jpg");
	path_to_notcats = strcat(path_to_dataset, "not_cats/*.jpg");
	cats = glob(path_to_cats);
	not_cats = glob(path_to_notcats);
endfunction
