# Creating HDR video without ghost artifacts

The program takes video(burst of images), where each frame has different exposure. Then, we're merging every two frames to gather single HDR tone mapped
frame(Exposure Fusion). To remove ghost artifacts, we have to find movement areas and merge most exposed areas of frames.

 
## Results

#### Car example

Input exposure 1             |  Input exposure 2
:----------------------------|-------------------------------------:
![](/rpz/tex/img/exposure1_car.png)|![](/rpz/tex/img/exposure2_car.png)

Merged frames(Ghost artifacts)              |  Labeled movement map
:----------------------------|-------------------------------------:
![](/rpz/tex/img/hdr_car.png)|![](/rpz/tex/img/car_movementMap.png)

Merged frames(Ghost artifacts)              |  Merged frames(Deghosted frame)
:----------------------------|-------------------------------------:
![](/rpz/tex/img/hdr_car.png)|![](/rpz/tex/img/hdr_car_deghost.png)




### Links

* [RPZ Tex](http://git.iu7.bmstu.ru/IU7-Projects/latex-g7-32)
* [CyberLeninka](https://cyberleninka.ru/)
* [Google Scholar](https://scholar.google.ru/)
* [GpuGems](https://developer.nvidia.com/gpugems/GPUGems/gpugems_pref01.html)
* [SigGraph](https://www.youtube.com/results?search_query=siggraph)
* [Zotero](https://www.zotero.org/)
* [UmbertoEco](http://urss.ru/cgi-bin/db.pl?lang=Ru&blang=ru&page=Book&id=184562)

* [IUS](http://projects.ius.edu.ba/ComputerGraphics/HDR/hdri2014.html)
* [Cambridge DATASETS](https://www.repository.cam.ac.uk/handle/1810/261766)