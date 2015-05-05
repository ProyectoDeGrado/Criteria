/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/



class MainContentComponent   : public Component, public MenuBarModel
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();
	
	void medwin();
	void importar();
	void showAudioSettings();


	StringArray getMenuBarNames();
	PopupMenu getMenuForIndex(int index,const String& name);
	enum MenuIDs {medicion,import,exportar,propiedadesmed,propiedades,salir,decaimiento,senalfilt,paracus,espectrofreq};
	void menuItemSelected(int menuID, int index);


private:

	MenuBarComponent menuBar;
	ApplicationProperties appProperties;
	//audio setup
	AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    AudioDeviceManager deviceManager;

	//Variables
	ScopedPointer<AudioSampleBuffer> IRbuffer;
	DocumentWindow *nada;
	int LongitudT;

    double sRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED

class ventanamedicioncomponentes : public Component
{
public:
	
	ventanamedicioncomponentes()
	{
		GroupComponent* groupfuente = addToList (new GroupComponent ("group", "Fuente"));
        groupfuente->setBounds (5, 5, 250, 300);
		GroupComponent* groupsenal = addToList (new GroupComponent ("group", "Senal"));
        groupsenal->setBounds (15, 20, 230, 125);
		GroupComponent* groupgain = addToList (new GroupComponent ("group", "Nivel"));
        groupgain->setBounds (15, 150, 230, 145);
		


		setSize(100,100);
		addAndMakeVisible(botonsweep=new TextButton("Interna"));
		botonsweep->setBounds(35,40,80,25);
		botonsweep->setColour (TextButton::buttonColourId, Colour(216,216,216));
		botonsweep->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
		
		addAndMakeVisible(externalbtn=new TextButton("Externa"));
		externalbtn->setBounds(145,40,80,25);
		externalbtn->setColour (TextButton::buttonColourId, Colour(216,216,216));
		externalbtn->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight );

		addAndMakeVisible(testbtn=new TextButton("Probar"));
		testbtn->setBounds(getHeight()+50,210,70,25);
		testbtn->setColour (TextButton::buttonColourId, Colour(216,216,216));
		testbtn->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);

		addAndMakeVisible(starbtn=new TextButton("Iniciar Medicion"));
		starbtn->setBounds(getHeight()-35,315,120,30);
		starbtn->setColour (TextButton::buttonColourId, Colour(183,183,183));
		starbtn->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);

		addAndMakeVisible(gainsweep);
		gainsweep.setBounds(100,160,60,110);
		gainsweep.setSliderStyle(Slider::LinearVertical);
		gainsweep.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
		gainsweep.setRange(0.0,100,1.0);
		gainsweep.setDoubleClickReturnValue (true, 50.0);
        gainsweep.setValue (50.0);		
		
		
		addAndMakeVisible(LinLog=new ComboBox("combobox"));
		LinLog->setBounds(110,80,100,20);
		LinLog->setEditableText (false);
		LinLog->setJustificationType (Justification::centredLeft);
		LinLog->setTextWhenNothingSelected (String::empty);
		LinLog->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
	    LinLog->addItem (TRANS("Lineal"), 1);
	    LinLog->addItem (TRANS("Logaritmico"), 2);
	    LinLog->setText("Logaritmico");
	
		addAndMakeVisible(duracionsweep = new TextEditor("Tiempo T"));
		duracionsweep->setBounds(110,110,100,20);
		duracionsweep->setMultiLine (false);
		duracionsweep->setReturnKeyStartsNewLine (false);
		duracionsweep->setReadOnly (false);
		duracionsweep->setScrollbarsShown (true);
		duracionsweep->setCaretVisible (true);
		duracionsweep->setPopupMenuEnabled (true);
		duracionsweep->setText (String::empty);
		duracionsweep->setText (TRANS("1"));
					
	}
	~ventanamedicioncomponentes()
	{
		deleteAndZero(botonsweep);
		deleteAndZero(externalbtn);
		deleteAndZero(testbtn);
		deleteAndZero(starbtn);
	}
	void paint (Graphics& g)
	{
		g.setFont (juce::Font (14.0f));
		g.drawText ("Tipo:", 30, 79, 30, 25, Justification::centred, true);

		g.setFont (juce::Font (14.0f));
		g.drawText ("Duracion[s]:", 30, 109, 80, 25, Justification::centredLeft, true);

		g.setFont (juce::Font (14.0f));
		g.drawText ("Ganancia", 100, 265, 80, 25, Justification::centredLeft, true);
		
		
	}

	void resized(){
	}
private:
	 

	OwnedArray<Component> components;
	template <typename ComponentType>
    ComponentType* addToList (ComponentType* newComp)
    {
        components.add (newComp);
        addAndMakeVisible (newComp);
        return newComp;
    }

	Slider gainsweep;
	TextButton *botonsweep;
	TextButton *externalbtn;
	TextButton *testbtn;
	TextButton *starbtn;
	ScopedPointer<TextEditor>duracionsweep;
	ScopedPointer<ComboBox> LinLog;
	
	

};



class ventanamedicion :public DocumentWindow
{
public:
	 ventanamedicion (const String& name, Colour backgroundColour, int buttonsNeeded)
        : DocumentWindow (name, backgroundColour, buttonsNeeded)
	 {
		 //setContentComponentSize(800,600);
		
		setContentOwned(new ventanamedicioncomponentes,false);
		     Rectangle<int> area (0, 0, 260, 360);
        const RectanglePlacement placement (RectanglePlacement::xLeft+ RectanglePlacement::yTop + RectanglePlacement::doNotResize);
        Rectangle<int> result (placement.appliedTo (area, Desktop::getInstance().getDisplays().getMainDisplay().userArea.reduced (100)));
        setBounds (result);
        
        setResizable (false, false);
        setUsingNativeTitleBar (true);
        setVisible (true);
    }
    
    void closeButtonPressed(){
        delete this;
    }
			
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ventanamedicion)
};