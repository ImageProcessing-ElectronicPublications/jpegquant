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
./jpegquant -q 2 lena.q100.jpg lena.q100.jq02.jpg
```
![Quant=02](images/lena.q100.jq02.jpg)  
[176313]  
![butteraugli=2](images/lena.q100.jq02.butteraugli.jpg)  
butteraugli: 0.782326

```
./jpegquant -q 4 lena.q100.jpg lena.q100.jq04.jpg
```
![Quant=04](images/lena.q100.jq04.jpg)  
[110952]  
![butteraugli=4](images/lena.q100.jq04.butteraugli.jpg)  
butteraugli: 1.688544

```
./jpegquant -q 10 lena.q100.jpg lena.q100.jq10.jpg
```
![Quant=10](images/lena.q100.jq10.jpg)  
[45875]  
![butteraugli=10](images/lena.q100.jq10.butteraugli.jpg)  
butteraugli: 4.625142

```
./jpegquant -q 20 lena.q100.jpg lena.q100.jq20.jpg
```
![Quant=20](images/lena.q100.jq20.jpg)  
[26211]  
![butteraugli=20](images/lena.q100.jq20.butteraugli.jpg)  
butteraugli: 7.342572

See also
========

* https://github.com/kud/jpegrescan
* https://github.com/ImageProcessing-ElectronicPublications/jpeg-recompress

Authors
=======

Copyright (C) 2012, Owen Campbell-Moore.  
Modify 2019-2020, zvezdochiot  
Public Domain Mark 1.0

HomePage
========

https://github.com/ImageProcessing-ElectronicPublications/jpegquant
