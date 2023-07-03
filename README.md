![GitHub release (latest by date)](https://img.shields.io/github/v/release/ImageProcessing-ElectronicPublications/jpegquant)
![GitHub Release Date](https://img.shields.io/github/release-date/ImageProcessing-ElectronicPublications/jpegquant)
![GitHub repo size](https://img.shields.io/github/repo-size/ImageProcessing-ElectronicPublications/jpegquant)
![GitHub all releases](https://img.shields.io/github/downloads/ImageProcessing-ElectronicPublications/jpegquant/total)
![GitHub](https://img.shields.io/github/license/ImageProcessing-ElectronicPublications/jpegquant)

# JPEGQUANT

JPEG DCT coefficients access in C

This provides access via an array to the DCT coefficients
which can then be modified before being re-saved as another JPEG.
It depends on the IJG JPEG library (or libjpeg) which can be
found here: http://www.ijg.org/

### USAGE

Origin:  
![Origin](https://raw.githubusercontent.com/ImageProcessing-ElectronicPublications/jpegquant-samples/main/images/lena.q100.jpg)  
[211044]

```
./jpegquant -q 2 lena.q100.jpg lena.q100.jq02.jpg
```
![Quant=02](https://raw.githubusercontent.com/ImageProcessing-ElectronicPublications/jpegquant-samples/main/images/lena.q100.jq02.jpg)  
[175851]  
![butteraugli=2](https://raw.githubusercontent.com/ImageProcessing-ElectronicPublications/jpegquant-samples/main/images/lena.q100.jq02.butteraugli.jpg)  
butteraugli: 0.776085
```sh
jpeg-compare -m sum -n lena.q100.jpg lena.q100.jq02.jpg 
SUM: 0.945429 (UM) # > 0.93 (HIGH)
```

```
./jpegquant -q 4 lena.q100.jpg lena.q100.jq04.jpg
```
![Quant=04](https://raw.githubusercontent.com/ImageProcessing-ElectronicPublications/jpegquant-samples/main/images/lena.q100.jq04.jpg)  
[127572]  
![butteraugli=4](https://raw.githubusercontent.com/ImageProcessing-ElectronicPublications/jpegquant-samples/main/images/lena.q100.jq04.butteraugli.jpg)  
butteraugli: 1.300308
```sh
jpeg-compare -m sum -n lena.q100.jpg lena.q100.jq04.jpg 
SUM: 0.833405 (UM) # > 0.76 (MEDIUM)
```

```
./jpegquant -q 10 lena.q100.jpg lena.q100.jq10.jpg
```
![Quant=10](https://raw.githubusercontent.com/ImageProcessing-ElectronicPublications/jpegquant-samples/main/images/lena.q100.jq10.jpg)  
[60209]  
![butteraugli=10](https://raw.githubusercontent.com/ImageProcessing-ElectronicPublications/jpegquant-samples/main/images/lena.q100.jq10.butteraugli.jpg)  
butteraugli: 2.399572
```sh
jpeg-compare -m sum -n lena.q100.jpg lena.q100.jq10.jpg 
SUM: 0.574876 (UM) # > 0.50 (LOW)
```

```
./jpegquant -q 20 lena.q100.jpg lena.q100.jq20.jpg
```
![Quant=20](https://raw.githubusercontent.com/ImageProcessing-ElectronicPublications/jpegquant-samples/main/images/lena.q100.jq20.jpg)  
[30029]  
![butteraugli=20](https://raw.githubusercontent.com/ImageProcessing-ElectronicPublications/jpegquant-samples/main/images/lena.q100.jq20.butteraugli.jpg)  
butteraugli: 4.421662
```sh
jpeg-compare -m sum -n lena.q100.jpg lena.q100.jq20.jpg 
SUM: 0.367628 (UM) # < 0.50 (DIRTY)
```

See all [sample files for JPEGquant](https://github.com/ImageProcessing-ElectronicPublications/jpegquant-samples).

### See also

* https://github.com/kud/jpegrescan
* https://github.com/ImageProcessing-ElectronicPublications/jpeg-recompress

### Authors

Copyright (C) 2012, Owen Campbell-Moore.  
Modify 2019-2023, zvezdochiot  
Public Domain Mark 1.0

### HomePage

https://github.com/ImageProcessing-ElectronicPublications/jpegquant
