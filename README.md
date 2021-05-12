![GitHub release (latest by date)](https://img.shields.io/github/v/release/ImageProcessing-ElectronicPublications/jpegquant)
![GitHub Release Date](https://img.shields.io/github/release-date/ImageProcessing-ElectronicPublications/jpegquant)
![GitHub repo size](https://img.shields.io/github/repo-size/ImageProcessing-ElectronicPublications/jpegquant)
![GitHub all releases](https://img.shields.io/github/downloads/ImageProcessing-ElectronicPublications/jpegquant/total)
![GitHub](https://img.shields.io/github/license/ImageProcessing-ElectronicPublications/jpegquant)

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
[220516]  
![butteraugli=2](images/lena.q100.jq02.butteraugli.jpg)  
butteraugli: 0.751521

```
./jpegquant -q 4 lena.q100.jpg lena.q100.jq04.jpg
```
![Quant=04](images/lena.q100.jq04.jpg)  
[215382]  
![butteraugli=4](images/lena.q100.jq04.butteraugli.jpg)  
butteraugli: 0.973014

```
./jpegquant -q 10 lena.q100.jpg lena.q100.jq10.jpg
```
![Quant=10](images/lena.q100.jq10.jpg)  
[179893]  
![butteraugli=10](images/lena.q100.jq10.butteraugli.jpg)  
butteraugli: 1.955633

```
./jpegquant -q 20 lena.q100.jpg lena.q100.jq20.jpg
```
![Quant=20](images/lena.q100.jq20.jpg)  
[163137]  
![butteraugli=20](images/lena.q100.jq20.butteraugli.jpg)  
butteraugli: 2.807457

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
