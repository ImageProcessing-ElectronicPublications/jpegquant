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

```
./jpegquant lena.q100.jpg lena.q100.jq50.jpg 50
```
![Quant=50](images/lena.q100.jq50.jpg) ![butteraugli=50](images/lena.q100.jq50.butteraugli.jpg)

```
./jpegquant lena.q100.jpg lena.q100.jq25.jpg 25
```
![Quant=25](images/lena.q100.jq25.jpg) ![butteraugli=25](images/lena.q100.jq25.butteraugli.jpg)

```
./jpegquant lena.q100.jpg lena.q100.jq10.jpg 10
```
![Quant=10](images/lena.q100.jq10.jpg) ![butteraugli=10](images/lena.q100.jq10.butteraugli.jpg)

```
./jpegquant lena.q100.jpg lena.q100.jq05.jpg 5
```
![Quant=05](images/lena.q100.jq05.jpg) ![butteraugli=05](images/lena.q100.jq05.butteraugli.jpg)

HomePage
========

https://github.com/ImageProcessing-ElectronicPublications/jpegquant
