function data = removeColours(data)

  if(ndims(data) >= 3)
  data = rgb2gray(data);
  end

end
