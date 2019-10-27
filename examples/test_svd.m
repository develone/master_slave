clear
close all

%A = [1.0 2.0 1.0 22.0 11.0; 3.0 4.0 2.0 41.0 21.0; 5.0 6.0 3.0 63.0 33.0;7.00  8.00  4.00 82.00 42.00;7.0 8.0 4.0 256.0 242.0;7.0 8.0 4.0 182.0 142.0]
A = imread('red.pgm');
[row col] = size(A);
N = row * col;
figure
imagesc(A);
colorbar;
colormap 'gray';
[U,S,V] = svd(A);
S(128:256,128:256) = 0;
AR = U*S*V';
V;
V'; 
figure
imagesc(AR);
colorbar;
colormap 'gray';
diff = (A - AR);
diffsq = (double(diff)*double(diff))/N;
m = sum(diffsq);
mse = sum(m);
mse
%https://www.mathworks.com/matlabcentral/answers/81048-mse-mean-square-error
%S(18:256,18:256) = 0;
%mse =  5602.6
%S(28:256,28:256) = 0;
%mse =  2985.9
%S(48:256,48:256) = 0;
%mse =  1405.4
%S(68:256,68:256) = 0;
%mse =  708.77
%S(128:256,128:256) = 0;
%mse =  104.05
