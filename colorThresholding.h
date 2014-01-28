#ifndef COLORTHRESHOLDING_H
#define COLORTHRESHOLDING_H

#define NSAMPLES 7
class Image;
class GuiParameters;

//Performs color thresholding using thresholding bounds stored in guiParameters. A binary segmented image is produced in the Image class.
void produceBinariesHSV(Image* img, GuiParameters* guiParameters);


#endif // COLORTHRESHOLDING_H
