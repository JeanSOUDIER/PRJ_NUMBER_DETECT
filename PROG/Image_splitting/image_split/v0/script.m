clear;
hold off;
close all;
clc;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if(exist('OCTAVE_VERSION', 'builtin'))

  disp("Environnement detected : Octave.");
  disp("Please ensure that the following packages are installed :");
  disp("image package : https://octave.sourceforge.io/image/");
  disp("control package : https://octave.sourceforge.io/control/");
  disp("signal package : https://octave.sourceforge.io/signal/");
  disp("\n\n\n");
  disp("*********************************************************");
  disp("\n\n\n");
  pkg load image;
  pkg load signal;

  else

  disp('Environnement detected : MatLab')
  disp('Some features may not be supported due to identifiers being different in MatLab and in Octave.');
  fprintf("\n\n\n");
  disp("*********************************************************");
  fprintf("\n\n\n");
  end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
data = imread('mnist.jpg');

figure(1);
subplot(1,2,1);
imshow(data);

data = bwlabel(removeColours(data) < 128);

  
  
parts = regionprops(data, 'BoundingBox', 'Area');
%returns a list of all items connected found from bwconncomp(data)


subplot(1,2,2);
imshow(data);

for k = 1 : length(parts)
  thisBB = parts(k).BoundingBox;
  rectangle('Position', [thisBB(1),thisBB(2),thisBB(3),thisBB(4)],...
  'EdgeColor','r','LineWidth',2 )
end

%[sorted_parts , sorted_ind] = sort([parts.Area] ,'descend');