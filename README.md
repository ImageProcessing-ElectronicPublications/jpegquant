JPEG DCT coefficients access in C
=================================

This provides access via an array to the DCT coefficients
which can then be modified before being re-saved as another JPEG.
It depends on the IJG JPEG library (or libjpeg) which can be
found here: http://www.ijg.org/

USAGE
=====

Origin:  
![Origin](images/lena.q100.jpg)  
[211044]

```
./jpegquant lena.q100.jpg lena.q100.jq50.jpg 50
```
![Quant=50](images/lena.q100.jq50.jpg)  
[176313]  
![butteraugli=50](images/lena.q100.jq50.butteraugli.jpg)  
butteraugli: 0.782326

```
./jpegquant lena.q100.jpg lena.q100.jq25.jpg 25
```
![Quant=25](images/lena.q100.jq25.jpg)  
[110952]  
![butteraugli=25](images/lena.q100.jq25.butteraugli.jpg)  
butteraugli: 1.688544

```
./jpegquant lena.q100.jpg lena.q100.jq10.jpg 10
```
![Quant=10](images/lena.q100.jq10.jpg)  
[45875]  
![butteraugli=10](images/lena.q100.jq10.butteraugli.jpg)  
butteraugli: 4.625142

```
./jpegquant lena.q100.jpg lena.q100.jq05.jpg 5
```
![Quant=05](images/lena.q100.jq05.jpg)  
[26211]  
![butteraugli=05](images/lena.q100.jq05.butteraugli.jpg)  
butteraugli: 7.342572

See also
========

* https://github.com/kud/jpegrescan
* https://github.com/ImageProcessing-ElectronicPublications/jpeg-recompress

Authors
=======

Copyright (C) 2012, Owen Campbell-Moore.  
Modify 2019, zvezdochiot

HomePage
========

https://github.com/ImageProcessing-ElectronicPublications/jpegquant
