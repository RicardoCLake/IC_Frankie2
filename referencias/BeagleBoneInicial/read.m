filename = ls("-t frankie*.bin");
tst = [];
for(n=1:size(filename,1))
	printf("Reading %s\n", filename(n,:));
	fd = fopen(filename(n,:));
	tmp = fread(fd, "int32");
	raw = reshape(tmp, 32, length(tmp)/32);

	figure;plot(1:10000,raw(17,1:10000),1:10000,raw(18,1:10000)+2e6,1:10000,raw(19,1:10000)+4e6,raw(20,1:10000)+6e6)	%Analise Eletrodo 18
	%figure;plot(raw(18,1:8192));
	%tst = [tst ; raw(18,1:8192)];
endfor




