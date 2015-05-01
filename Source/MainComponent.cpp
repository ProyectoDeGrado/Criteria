/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent():menuBar(this)
{
	addAndMakeVisible(&menuBar);
	centreWithSize(800,600);
   
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);


}

void MainContentComponent::resized()
{
	menuBar.setBounds(0, 0,	getWidth(), 20);
}

StringArray MainContentComponent::getMenuBarNames()
{
	const char* menuNames[] = {
		"Archivo", "Calcular", 0 };
	return StringArray(menuNames);
}

PopupMenu MainContentComponent::getMenuForIndex(int index, const String& name)
{
	PopupMenu menu;
	if (name == "Archivo")
	{				
		menu.addItem(propiedadesmed, "Medicion");
		menu.addItem(import, "Importar");
		menu.addItem(exportar, "Exportar");
		menu.addItem(propiedades, "Propiedades de Audio");
		menu.addItem(salir, "salir");
	}
	else if (name == "Calcular")
	{
		menu.addItem(senalfilt, "Respuesta al impulso filtrada");
		menu.addItem(decaimiento, "Curva de Decaimiento");
		menu.addItem(paracus, "Parametros Acusticos");
		menu.addItem(espectrofreq, "Espectro en frecuencia");
		}
	
	return menu;
}

void MainContentComponent::menuItemSelected(int menuID,  int index)
{
	if (menuID==propiedadesmed)
	{
		medwin();
	}
	else if (menuID==import)
	{
		importar();
	}
	else if (menuID==propiedades)
	{
		showAudioSettings();
	}
	else if (menuID==salir)
	{
		JUCEApplication::quit();
	}
	
	
}

void MainContentComponent::medwin()
{
   DocumentWindow* medwin= new ventanamedicion("Configuracion de medicion",Colour(241,241,241),4);
   medwin->centreWithSize(260,360);
}

void MainContentComponent::importar()
{
	FileChooser chooser("Select a Wave file to play...", File::nonexistent, "*.wav");
	if (chooser.browseForFileToOpen())
	{
		File soundfile(chooser.getResult());
		FileInputSource wavRead(chooser.getResult());
		formatManager.registerBasicFormats();
		AudioFormatReader* audioFormatReader = formatManager.createReaderFor(soundfile);
		//thumbnail.setSource (new FileInputSource (soundfile));
		readerSource = new AudioFormatReaderSource(audioFormatReader, true);
		LongitudT = audioFormatReader->lengthInSamples;
		IRbuffer = new AudioSampleBuffer(1, LongitudT);
		IRbuffer->clear();
		audioFormatReader->read(IRbuffer, 0, LongitudT, 0, true, false);
		sRate = audioFormatReader->sampleRate;
	}
}

void MainContentComponent::showAudioSettings()
{    
    bool showMidiInputOptions = false;
	bool showMidiOutputSelector = false;
	bool showChannelsAsStereoPairs = true;
	bool hideAdvancedOptions = false;
	
	AudioDeviceSelectorComponent settings(deviceManager,0, 0, 1, 2,showMidiInputOptions,showMidiOutputSelector,showChannelsAsStereoPairs,hideAdvancedOptions);
	settings.setSize (480, 250);
	DialogWindow::showModalDialog(String("Audio Settings"),&settings,TopLevelWindow::getTopLevelWindow (0),Colours::white,true);//darkslategrey   
}

