function [R1 R2] = PageRank(nume, d, eps)
	% Calculeaza indicii PageRank pentru cele 3 cerinte
	% Scrie fisierul de iesire nume.out

	numeout = strcat(nume, '.out');
	outfile = fopen(numeout, 'wt');
	infile = fopen(nume, 'rt');

	n = fscanf(infile, '%d', 1);
	fprintf(outfile, '%d\n\n', n);

	%Metoda iterativa
	R1 = Iterative(nume, d, eps);
	for i = 1 : n
		fprintf(outfile, '%.6f\n', R1(i));
	endfor
	fprintf(outfile, '\n');

	%Metoda algebraica
	R2 = Algebraic(nume, d);
	for i = 1 : n
		fprintf(outfile, '%.6f\n', R2(i));
	endfor
	fprintf(outfile, '\n');

	%Apartenenta
	R2 = [zeros(n, 1), R2];
	v = fgetl(infile);
	for i = 1 : n
		R2(i, 1) = i; %adaug o coloana cu indicele paginii in fata coloanei PR
		v = fgetl(infile); %parcurg liniile fisierului pt a ajunge la val1 si val2
	endfor
	val1 = fscanf(infile, '%f', 1);
	val2 = fscanf(infile, '%f', 1);
	R2 = sortrows(R2, [-2, -1]); %sortez dupa coloana PR descrescator
	for i = 1 : n
		fprintf(outfile, '%d %d %.6f\n', i,
				R2(i, 1),
				Apartenenta(R2(i,2), val1, val2));
	endfor
	R2 = R2(:, 2); %elimin coloana indicelor

	fclose(outfile);
	fclose(infile);
end
