clear all

N=1024;
ne=32;
frame=1;
n_frame=1;
total = [];
for cur=1:n_frame
	total = [ total read_frame_binary("a.bin", frame, 1, ne, N)];
endfor
eletrodos = [1:ne];
M = zeros(ne,N*n_frame);
figure;

for cur=17:32
    M(cur,:) = total(cur:ne:end);
    subplot(4,4,cur-16)
    plot(M(cur,1:500)/2^23*10);
    %title(sprintf("Eletrodo %02d", cur));
    
endfor

