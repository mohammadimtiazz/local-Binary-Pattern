# local-Binary-Pattern
LBP tutorial links:
https://www.youtube.com/watch?v=wpAwdsubl1w

https://www.youtube.com/watch?v=Q1Oo8nIda_Y&t=210s

https://www.youtube.com/watch?v=f070XReddQU

lbpTest01 ->> has cpp file which perfrom local binary pattern on gray scale image from scale 1 to 3. As example two sample image is provided "filename.png" and "lena512.bmp". 
  "filename.png"--> is a 9c9 matrix with 8-bit values. This image provide a great example about how the LBP mechanism works. 
  "lena512.bmp" --> is 512x512 standred lena image whihc illustates how the lbp images looks like in different scale.
  
  
  lbpTest02 ->> is as same as lbpTest01. The only difference is that the lbp functionality has been moved to a function and later on called in the main(). 
  
  
  medianLbp ->> has source file that can perform median LBP operation. it is bit slow as it is using the median pixel value as a threshold. STL container median operation can make htis program faster. For more info look at the main code

  extendedLBP ->> has source file that can perform extended LBP operation. 
       niLBP -- neighbourhood intensity LBP. use neighbouring pixels intesity average as threshold for binarization
       ciLBP -- central intensity LBP. use entire image pixels intesity average as a threshold for binarization of central pixel
