/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{    
    setSize (500, 400);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff001F36));
    
    g.setColour(Colours::white);
    g.drawRect(getBounds().reduced(50));
    
    g.drawFittedText("Drop 'stills' folder here...", getBounds().reduced(50), Justification::centred, 1, 1);
}

void MainContentComponent::resized()
{

}

bool MainContentComponent::isInterestedInFileDrag (const StringArray& files)
{
    for (int i = 0; i < files.size(); i++ )
    {
        if (File(files[i]).isDirectory())
            return true;
    }
    
    return false;
}

void MainContentComponent::filesDropped (const StringArray& files, int x, int y)
{
    Array<int> scales;
    scales.add(368);
    scales.add(176);
    scales.add(132);
    scales.add(107);
    scales.add(65);
 
    for (int i = 0; i < files.size(); i++ )
    {
        File file = File(files[i]);
        if (file.isDirectory())
        {
            //get the source files
            Array<File> sourceFiles;
            file.findChildFiles( sourceFiles, File::TypesOfFileToFind::findFiles + File::TypesOfFileToFind::ignoreHiddenFiles , false );
            
            //cycle through the sourceFiles
            for ( int i = 0; i < sourceFiles.size(); i++ )
            {
                //get the image
                Image image = ImageFileFormat::loadFrom( sourceFiles[i] );
                
                //premultiply the pixels
                for ( int x = 0; x < image.getWidth(); x++ )
                {
                    for( int y = 0; y < image.getHeight(); y++ )
                    {
                        Colour c = image.getPixelAt(x, y);
                        PixelARGB p = c.getPixelARGB();
                        p.premultiply();
                        c = Colour::fromRGBA(p.getRed(), p.getGreen(), p.getBlue(), p.getAlpha());
                        image.setPixelAt(x, y, c.withAlpha(1.0f));
                    }
                }
                
                //once for each scaling
                for ( int s = 0; s < scales.size(); s++ )
                {
                    
                    //first resize
                    //image is always 4 x 3
                    image = image.rescaled( scales[s], (scales[s] / 4) * 3);
                    
                    //save to jpeg
                    JPEGImageFormat jpgff;
                    jpgff.setQuality( 1.0 );
                    
                    //create file to save as
                    File f = File(file.getParentDirectory().getFullPathName() + "/" + String(scales[s]) + "/" + sourceFiles[i].getFileNameWithoutExtension() + ".jpg");
                    DBG(f.getFullPathName());
                    if ( !f.exists() )
                        f.create();
                    
                    //create a stream to write to
                    FileOutputStream fos ( f, 16384);
                    
                    //write the stream to file
                    if ( fos.openedOk() )
                    {
                        jpgff.writeImageToStream (image, fos);
                        fos.flush();
                    }
                }
            }
        }
    }
}
