/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h" //JUCE
#include "fftw3.h" //Librería FFTW
#include <fstream> //Para poder exportar archivos .txt
#include <iostream>

using namespace drow;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
//==============================================================================

//Variables Globales del proyecto (Se utilizan en .h .cpp)
extern AudioSampleBuffer *IRbuffer;
extern int Length;
extern double *IR;

const double w1=2.0 * double_Pi * 1;//Frecuencia de Inicio de Sweep
const double w2=2.0 * double_Pi * 23000;//Frecuencia Final del Sweep

extern DocumentWindow* ConfAudiowindow;
extern DocumentWindow* medwindow;

extern juce::AudioDeviceManager::AudioDeviceSetup result;
extern double sRate;

//==============================================================================

class MainContentComponent   : public Component, public MenuBarModel, public ChangeListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();
    void changeListenerCallback (ChangeBroadcaster*);
	//void showAudioSettings(); SE TIENE QUE HACER UNA CLASE PARA ESTA VENTANA ventanaConfiguracionesAudio
	//void medwin(); SE PASA PARA LA VENTANA DE CONFIGURACIONES DE AUDIO!!
    void medicionWin();
    void ImportarWAV();
    void normalizar(double *Signal, int Len);
    double sumar(double *Inbuffer, int Len);
    void encontrarTodosLosParametros();
    double maximo(double *InSignal, int Len);
    double minimo(double *InSignal, int Len);
    void filtrar(double *InDoubleA, int bandas, int Len);
    double getValorParametroTemporal(double *EdBj, double dB1, double dB2, int Longitud); //j es la banda
    int encontrarMuestraN(double value, double *InBuffer, int Len);
    void exportarParametros(int band);
    static AudioDeviceManager& getAudioDeviceManagerCompartido();
    //void escribirWav(AudioSampleBuffer &AudioBuffer, int FS);
    void inicializarPunteros(String descripcion);
    void eliminarPunteros(String descripcion);
    
    //static void alertBoxResultChosen (int result, DialogsDemo*);
    
	StringArray getMenuBarNames();
	PopupMenu getMenuForIndex(int index,const String& name);
	enum MenuIDs {medicion,import,exportarIR, exportarP,propiedadesmed,propiedades,salir,decaimiento,octava,tercio,verEDT, verT10, verT20, verT30, verC50,verC80, verD50, verTs, verSTe, verSTl, formaOnda, verTrBR};
	void menuItemSelected(int menuID, int index);
    
private:
    
	MenuBarComponent menuBar;
	ApplicationProperties appProperties;

	//Variables
	int LongitudT;

    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    AudioDeviceManager& deviceManager;
    AudioTransportSource transportSource;
    AudioSourcePlayer sourcePlayer;
    TimeSliceThread thread;
    File soundfile;
    
    double *IRcopy;
    double *hFinal;
    double **irFil;
    //double **EdBmm;
    int bandas;
    double *EDT;
    double *T10;
    double *T20;
    double *T30;
    double *C50;
    double *C80;
    double *D50;
    double *Ts;
    double *STe;
    double *STl;
    double Tr;//Brillo
    double BR;//Calidéz
    
    //Variables
    double *pos;
    double *picos;
    
    double *posprom;
    double *picosprom;
    
    double *env;
    double *tail;
    double *yy;
    double *yyE;
    double *y1;
    double *ET;
    double *h1;
    double *E;
    double *EdB;
    //double *EdBcopy;
    double *groupdelay;
    double *ycuadE;
    double *xcopia;
    double *h1f;
    double *y1f;
    double *EdBabs;
    double *y30;
    double *xx;
    double *xy30;
    double *xpw2;
    double *x1;
    double *regresion;
    double *regresionAbs;
    int Longitud;
    
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;
    //float level;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED

//Elemntos de la ventana de medición
class ventanamedicioncomponentes : public Component, public AudioIODeviceCallback, public ButtonListener/*, public Timer*/
{
public:
	
    ventanamedicioncomponentes():threadMed("audio file preview"),deviceManager(MainContentComponent::getAudioDeviceManagerCompartido()), meterThread ("Meter Thread")
	{
		GroupComponent *groupfuente = addToList (new GroupComponent ("group", "Fuente"));
        groupfuente->setBounds (5, 5, 250, 300);
		GroupComponent *groupsenal = addToList (new GroupComponent ("group", "Barrido/Sweep"));
        groupsenal->setBounds (15, 20, 230, 125);
		GroupComponent *groupgain = addToList (new GroupComponent ("group", "Nivel"));
        groupgain->setBounds (15, 150, 230, 145);
		
		setSize(100,100);
        
     
		addAndMakeVisible(botonsweep=new TextButton("Interno"));
		botonsweep->setBounds(35,40,80,25);
		botonsweep->setColour (TextButton::buttonColourId, Colour(216,216,216));
		botonsweep->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
        botonsweep->addListener(this);
		
		addAndMakeVisible(externalbtn=new TextButton("Externo"));
		externalbtn->setBounds(145,40,80,25);
		externalbtn->setColour (TextButton::buttonColourId, Colour(216,216,216));
		externalbtn->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight );
        externalbtn->setEnabled(true);
        externalbtn->addListener(this);
        
		addAndMakeVisible(testbtn=new TextButton("Probar"));
		testbtn->setBounds(getHeight()+50,210,70,25);
		testbtn->setColour (TextButton::buttonColourId, Colour(216,216,216));
		testbtn->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
        testbtn->addListener(this);
        
		addAndMakeVisible(startbtn=new TextButton(CharPointer_UTF8 ("Iniciar medici\xc3\xb3n")));
		startbtn->setBounds(getHeight()-35,315,120,30);
		startbtn->setColour (TextButton::buttonColourId, Colour(183,183,183));
		startbtn->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
        startbtn->addListener(this);
        startbtn->setTooltip("empezar la medicion");
        
        pararCallback=new TextButton("quitarCallback");
        pararCallback->setBounds(getHeight()-35,200,12,30);
        pararCallback->setColour (TextButton::buttonColourId, Colour(183,183,183));
        pararCallback->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
        pararCallback->addListener(this);
        
		addAndMakeVisible(gainsweep);
		gainsweep.setBounds(100,160,60,110);
		gainsweep.setSliderStyle(Slider::LinearVertical);
		gainsweep.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
		gainsweep.setRange(0.0,1.0,0.001);
		gainsweep.setDoubleClickReturnValue (true, 0.5);
        gainsweep.setValue (0.5);
        gainsweep.setSkewFactor(0.5);
		
		
		addAndMakeVisible(LinLog=new ComboBox("combobox"));
		LinLog->setBounds(115,72,100,18);
		LinLog->setEditableText (false);
		LinLog->setJustificationType (Justification::centredLeft);
		LinLog->setTextWhenNothingSelected (String::empty);
		LinLog->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
	    LinLog->addItem (TRANS("Lineal"), 1);
	    LinLog->addItem (TRANS(CharPointer_UTF8 ("Logar\xc3\xadtmico ")), 2);
	    LinLog->setText(CharPointer_UTF8 ("Logar\xc3\xadtmico "));
	
		addAndMakeVisible(duracionsweep = new TextEditor("Tiempo T"));
		duracionsweep->setBounds(115,94,100,18);
		duracionsweep->setMultiLine (false);
		duracionsweep->setReturnKeyStartsNewLine (false);
		duracionsweep->setReadOnly (false);
		duracionsweep->setScrollbarsShown (true);
		duracionsweep->setCaretVisible (true);
		duracionsweep->setPopupMenuEnabled (true);
		duracionsweep->setText (String::empty);
		duracionsweep->setText (TRANS("1"));
        
        addAndMakeVisible(duracionRTestimado = new TextEditor("RT estimado"));
        duracionRTestimado->setBounds(115,116,100,18);
        duracionRTestimado->setMultiLine (false);
        duracionRTestimado->setReturnKeyStartsNewLine (false);
        duracionRTestimado->setReadOnly (false);
        duracionRTestimado->setText ("2");
        duracionRTestimado->setEnabled(true);
        
        deconv=false;
        senalExterna=false;
        definirFFT(1);
        SweepBuffer=new AudioSampleBuffer(1,1);
        ResBuffer=new AudioSampleBuffer(1,1);
        SenalExternaBuffer=new AudioSampleBuffer(1,1);
        
        fManagerMed.registerBasicFormats();
        sPlayerMed.setSource(&tSourceMed);
        threadMed.startThread(3);
        
        deviceManager.enableInputLevelMeasurement (true);
        //startTimer (50); //Timer para el medidor
        
        addAndMakeVisible (&meterL);
        meterThread.addTimeSliceClient (&meterL);
        meterThread.startThread (1);
        
					
	}
	~ventanamedicioncomponentes()
	{
		deleteAndZero(botonsweep);
		deleteAndZero(externalbtn);
		deleteAndZero(testbtn);
		deleteAndZero(startbtn);
        deleteAndZero(pararCallback);
        
        tSourceMed.stop();
        tSourceMed.setSource(nullptr);
        rSourceMed=nullptr;
        threadMed.stopThread (500);
        meterThread.stopThread (500);
        
        deviceManager.enableInputLevelMeasurement (false);
        deviceManager.removeAudioCallback(this);
        
        fftw_destroy_plan(transformadaX);
        fftw_destroy_plan(transformadaY);
        fftw_destroy_plan(transformadah);
        fftw_free(h);
        fftw_free(H);
        fftw_free(x);
        fftw_free(X);
        fftw_free(y);
        fftw_free(Y);
        
        SweepBuffer->~AudioSampleBuffer();
        ResBuffer->~AudioSampleBuffer();
        SenalExternaBuffer->~AudioSampleBuffer();
        
	}
	void paint (Graphics& g)
	{
		g.setFont (juce::Font (12.0f));
		g.drawText ("Tipo", 30, 70, 30, 25, Justification::centredLeft, true);
		g.drawText (CharPointer_UTF8 ("Duraci\xc3\xb3n[s]:"), 30, 92, 80, 25, Justification::centredLeft, true);
        g.drawText ("RT estimado[s]:", 30, 112, 80, 25, Justification::centredLeft, true);

		g.drawText ("Ganancia", 110, 265, 80, 25, Justification::centredLeft, true);
        //g.setOrigin(50, 209);
        //getLookAndFeel().drawLevelMeter(g, 50, 25, (float) exp (log (level) / 3.0));
	}

	void resized(){
        meterL.setBounds(68,178,15,85);
	}
    
    void escribirWav(AudioSampleBuffer &AudioBuffer, int FS){
        
        FileChooser chooser ("Guardar respuesta al impulso (IR)",File::nonexistent, "*.wav");
        if (chooser.browseForFileToSave(true)) {
            file=chooser.getResult();
            file.deleteFile();
            ScopedPointer<FileOutputStream> fileStream (file.createOutputStream());
            
            WavAudioFormat wavFormat;
            ScopedPointer<AudioFormatWriter> writer;
            writer = wavFormat.createWriterFor (fileStream, FS, 1, 16, StringPairArray(), 0);
            
            fileStream.release(); // Se pasa la responsabilidad de borrar el stream al writerObject que ahora lo esta usando
            writer->writeFromAudioSampleBuffer(AudioBuffer, 0, AudioBuffer.getNumSamples());
            
            //        delete writer; 
            //        writer=nullptr;
            
        }else{
            escribirWav(*IRbuffer, sRate);
        }
    }
    
    void buttonClicked(Button* buttonThatWasClicked){
        if (buttonThatWasClicked==startbtn) {
            if (startbtn->getButtonText()==CharPointer_UTF8 ("Iniciar medici\xc3\xb3n")) {
                startbtn->setButtonText(CharPointer_UTF8 ("Parar medici\xc3\xb3n"));
                testbtn->setEnabled(false);
                botonsweep->setEnabled(false);
                externalbtn->setEnabled(false);
                LinLog->setEnabled(false);
                gainsweep.setEnabled(false);
                duracionsweep->setEnabled(false);
                deconv=false;
                amplitude=gainsweep.getValue();
                ampExt=1.0/maxExterna; //NORMALIZAR SEÑALES EXTERNAS!!!!
                T=duracionsweep->getText().getDoubleValue();
                valorRTestimado=duracionRTestimado->getText().getDoubleValue();
                
                deviceManager.getAudioDeviceSetup(result);
                sRate=result.sampleRate;
                
                t=0.0;
                scount=0;
                SweepSize = T*sRate;
                fftSize=1.11*SweepSize+1.11*valorRTestimado*sRate;//RT+Sweep+RT/9+Sweep/9->RT(1+1/9)+Sweep(1+1/9)
                
                definirFFT(fftSize);
                //        deviceManager.getAudioDeviceSetup(result);
                //        sRate=result.sampleRate;
                
                deviceManager.addAudioCallback(this);
            }else{
                pararCallback->triggerClick();
            }

        }else if (buttonThatWasClicked==testbtn){
            if (testbtn->getButtonText()=="Probar") {
                testbtn->setButtonText("Parar");
                botonsweep->setEnabled(false);
                externalbtn->setEnabled(false);
                LinLog->setEnabled(false);
                startbtn->setEnabled(false);
                duracionsweep->setEnabled(false);
                duracionRTestimado->setEnabled(false);
                
                deconv=false;
                amplitude=gainsweep.getValue();
                ampExt=1.0/maxExterna;
                T=duracionsweep->getText().getFloatValue();
                t=0.0;
                scount=0;
                SweepSize = T*sRate;
                
                definirFFT(SweepSize);
                
                //        deviceManager.getAudioDeviceSetup(result);
                //        sRate=result.sampleRate;
                
                deviceManager.addAudioCallback(this);
            }else{
                pararCallback->triggerClick();
            }
        }else if (buttonThatWasClicked==pararCallback){
            testbtn->setEnabled(true);
            botonsweep->setEnabled(true);
            externalbtn->setEnabled(true);
            if (senalExterna) {
                LinLog->setEnabled(false);
                duracionsweep->setEnabled(false);
            }else{
                LinLog->setEnabled(true);
                duracionsweep->setEnabled(true);
            }
            
            gainsweep.setEnabled(true);
            startbtn->setEnabled(true);
            
            amplitude=0.0;
            if (startbtn->getButtonText()==CharPointer_UTF8 ("Parar medici\xc3\xb3n")) {
                startbtn->setButtonText(CharPointer_UTF8("Iniciar medici\xc3\xb3n"));
                if (deconv) {
                    deconvolucion();
                    //medwindow->closeButtonPressed();
                    escribirWav(*IRbuffer, sRate);
                    AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                                          CharPointer_UTF8 ("Medici\xc3\xb3n finalizada"),
                                                          "La respuesta al impulso " + file.getFullPathName() + " puede ser importada en la ventana principal.",
                                                          "Ok");
                    
                }
            }else if (testbtn->getButtonText()=="Parar"){
                testbtn->setButtonText("Probar");
            }
        }else if (buttonThatWasClicked==botonsweep){
            //botonsweep->setEnabled(false);
            externalbtn->setEnabled(true);
            LinLog->setEnabled(true);
            duracionsweep->setEnabled(true);
            duracionRTestimado->setEnabled(true);
            senalExterna=false;
        }else if (buttonThatWasClicked==externalbtn){
            //externalbtn->setEnabled(false);
            scount=0;
            botonsweep->setEnabled(true);
            LinLog->setEnabled(false);
            duracionsweep->setEnabled(false);
            ImportarSenalExterna();
            duracionsweep->setText(String(Lexterna/sRate));
            maxExterna=0.0f;
            for (int i=0; i<Lexterna; ++i) {
                if (SenalExternaBuffer->getSample(0, i)>maxExterna) {
                    maxExterna=SenalExternaBuffer->getSample(0, i);
                }
            }
        }
    }
    
    void ImportarSenalExterna(){

        FileChooser chooser ("Seleccione una respuesta al impulso",File::nonexistent, "*.wav");
        if (chooser.browseForFileToOpen()) {
            File soundfile(chooser.getResult());
            FileInputSource wavRead(chooser.getResult());
            AudioFormatReader* aFormatReaderMed = fManagerMed.createReaderFor(soundfile);
            rSourceMed = new AudioFormatReaderSource(aFormatReaderMed, true);
            Lexterna=aFormatReaderMed->lengthInSamples;
            
            SenalExternaBuffer->~AudioSampleBuffer();
            SenalExternaBuffer = new AudioSampleBuffer(1, Lexterna); //Buffer donde están las muestras del .wav
            SenalExternaBuffer->clear();
            
            aFormatReaderMed->read(SenalExternaBuffer, 0, Lexterna, 0, true, false);
            sRate=aFormatReaderMed->sampleRate;
            
            //Aqui se cambia la frecuencia de muestreo de la sesión a la del .wav
            juce::AudioDeviceManager::AudioDeviceSetup newAudioSetup;
            deviceManager.getAudioDeviceSetup(newAudioSetup);
            newAudioSetup.sampleRate=sRate;
            deviceManager.setAudioDeviceSetup(newAudioSetup, true);
            
            senalExterna=true;
        }
    }
    
    void audioDeviceIOCallback(const float** inputData,int InputChannels,float** outputData,int OutputChannels,int numSamples){
        //meterL.copySamples (inputData[0], numSamples);
        
        const AudioSampleBuffer RespBuff (const_cast<float**> (inputData), 1, numSamples);
        const AudioSampleBuffer SweepBuff (const_cast<float**> (outputData), 1, numSamples);
        const float originalt = t;
        for(int i = 0; i < OutputChannels; i++){
            t = originalt;
            for(int j = 0; j < numSamples; j++){
                if (senalExterna) {
                    if (scount+j<Lexterna) {
                        outputData[i][j]=amplitude*ampExt*SenalExternaBuffer->getSample(0, scount+j);
                    }else{
                        outputData[i][j]=0.0;
                    }
                    
                }else{
                    if (LinLog->getText() == "Lineal") {
                        phase = std::fmod((w1*t) + (((w2-w1)*(1/T))*(t*t)/2), 2.0f*float_Pi); //Lineal
                    }else{
                        phase = std::fmod(((w1*T)/(std::log(w2/w1)))*((std::exp((t/T)*(std::log(w2/w1))))-1), 2 * float_Pi);//Logaritmico
                    }
                    outputData[i][j]=amplitude*std::sin(phase);
                }

                if (testbtn->getButtonText()=="Parar") {
                    amplitude=gainsweep.getValue();
                }
                if (startbtn->getButtonText()==CharPointer_UTF8 ("Parar medici\xc3\xb3n")) {
                    if ((scount+j)<fftSize) {
                        x[scount+j][0]=SweepBuff.getSample(0, j);
                        y[scount+j][0]=RespBuff.getSample(0, j);
                    }else{
                        deconv=true;
                        pararCallback->triggerClick();
                    }
                }
                if (t>T) {
                    amplitude=0;
                    if (testbtn->getButtonText()=="Parar") {
                        pararCallback->triggerClick();
                    }
                }else{
                    t+=1.0/sRate;
                }
            }
        }
        scount+=numSamples;
    }
    
    void audioDeviceAboutToStart (AudioIODevice* device){
        //transportSource.prepareToPlay(device->getCurrentBufferSizeSamples(),device->getCurrentSampleRate());
    }
    void audioDeviceStopped(){};

    void definirFFT(int fftSize){
        
        x  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * fftSize );
        X  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * fftSize );
        y  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * fftSize );
        Y  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * fftSize );
        h  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * fftSize );
        H  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * fftSize );
        
        transformadaX  = fftw_plan_dft_1d( fftSize , x, X, FFTW_FORWARD, FFTW_ESTIMATE );
        transformadaY  = fftw_plan_dft_1d( fftSize , y, Y, FFTW_FORWARD, FFTW_ESTIMATE );
        transformadah  = fftw_plan_dft_1d( fftSize , H, h, FFTW_BACKWARD, FFTW_ESTIMATE );
        
    }
    
    void deconvolucion(){
        
        fftw_execute(transformadaX);
        fftw_execute(transformadaY);
        
        for (int k=0; k<fftSize; ++k) {
            float a=X[k][0]*X[k][0]+X[k][1]*X[k][1];
            H[k][0]=(100/a)*(Y[k][0]*X[k][0]+Y[k][1]*X[k][1]);
            H[k][1]=(100/a)*(Y[k][1]*X[k][0]-Y[k][0]*X[k][1]);
        }
        fftw_execute(transformadah);
        Length=(fftSize-SweepSize);
        
        SweepBuffer->~AudioSampleBuffer();
        ResBuffer->~AudioSampleBuffer();
        
        SweepBuffer= new AudioSampleBuffer(1,Length);
        SweepBuffer->clear();
        IRbuffer = new AudioSampleBuffer(1,Length);
        IRbuffer->clear();
        ResBuffer = new AudioSampleBuffer(1,fftSize);
        ResBuffer->clear();
        delete [] IR;
        IR=new double[Length];
//        Sweep=new double[Length];
//        Respuesta=new double[fftSize];
        
        for (int n=0; n<fftSize ; n++) {
            if (n<Length) {
                IRbuffer->addSample(0, n, h[n][0]/fftSize);
                SweepBuffer->addSample(0, n, x[n][0]);
                IR[n]=h[n][0]/fftSize;
//                Sweep[n]=x[n][0];
            }
            ResBuffer->addSample(0, n, y[n][0]);
        }

    }
    
    //timer para el medidor de juce
//    void timerCallback(){
//        const float newLevel = (float) deviceManager.getCurrentInputLevel();
//        if (std::abs (level - newLevel) > 0.005f){
//            level = newLevel;
//            repaint();
//        }else{
//            level = 0;
//        }
//    }
    
private:
	 
	OwnedArray<Component> components;
	template <typename ComponentType>
    ComponentType* addToList (ComponentType* newComp)
    {
        components.add (newComp);
        addAndMakeVisible (newComp);
        return newComp;
    }
    
    AudioFormatManager fManagerMed;
    ScopedPointer<AudioFormatReaderSource> rSourceMed;
    AudioTransportSource tSourceMed;
    AudioSourcePlayer sPlayerMed;
    TimeSliceThread threadMed;
    AudioDeviceManager& deviceManager;
     
	Slider gainsweep;
	TextButton *botonsweep;
	TextButton *externalbtn;
	TextButton *testbtn;
	TextButton *startbtn;
    TextButton *pararCallback;
	ScopedPointer<TextEditor> duracionsweep;
    ScopedPointer<TextEditor> duracionRTestimado;
	ScopedPointer<ComboBox> LinLog;
    File file;
    
    int fftSize;
    int scount;
    int SweepSize;
    int Lexterna;
    float maxExterna;
    
    double t; //Vector de Tiempo
    double amplitude;
    double ampExt;
    double phase; //Argumento para sin(ph)
    double T; //Duración del sweep
    double valorRTestimado;
    bool deconv;
    
    AudioSampleBuffer *SweepBuffer;
    AudioSampleBuffer *ResBuffer;
    AudioSampleBuffer *SenalExternaBuffer;
    
    
    fftw_complex *x, *X, *y, *Y, *h, *H;
    fftw_plan transformadaX;
    fftw_plan transformadaY;
    fftw_plan transformadah;
    bool senalExterna;
    
    //float level;
    
    TimeSliceThread meterThread;
    SegmentedMeter meterL;//, meterR;
    
};

class ventanamedicion :public DocumentWindow
{
public:
	 ventanamedicion (const String& name, Colour backgroundColour, int buttonsNeeded)
        : DocumentWindow (name, backgroundColour, buttonsNeeded)
	 {
         
        setContentComponentSize(800,600);
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

class ventanaConfiguracionesAudioComponentes : public Component, public ButtonListener{
public:
    ventanaConfiguracionesAudioComponentes() : deviceManager(MainContentComponent::getAudioDeviceManagerCompartido()), settingsComp(deviceManager, 1, 1, 1, 1, false, false, false, false)
    {
        //CONSTRUCTOR
        addAndMakeVisible(botonSiguiente=new TextButton("Siguiente"));
        botonSiguiente->setBounds(480/2-40,280,80,25);
        botonSiguiente->setColour (TextButton::buttonColourId, Colour(216,216,216));
        botonSiguiente->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
        botonSiguiente->addListener(this);
        
        
        addAndMakeVisible(settingsComp);
        settingsComp.setSize (480, 280);
    
    }
    ~ventanaConfiguracionesAudioComponentes(){
        //DESTRUCTOR
        deviceManager.getAudioDeviceSetup(result);
        sRate=result.sampleRate;
        deleteAndZero(botonSiguiente);
        medwin();
    }
    
    void paint (Graphics& g) {
    }
    
    void resized(){
    }
    
    void buttonClicked(Button* buttonThatWasClicked){
        if (buttonThatWasClicked==botonSiguiente){
            delete this;
            delete ConfAudiowindow;
        }
        
    }
    void medwin()
    {
        medwindow = new ventanamedicion(CharPointer_UTF8 ("Medici\xc3\xb3n"),Colour(241,241,241),4);
        medwindow->centreWithSize(260,360);
        
    }
    
private:
    AudioDeviceManager& deviceManager;
    TextButton *botonSiguiente;
    AudioDeviceSelectorComponent settingsComp;
};

class ventanaConfiguracionesAudio : public DocumentWindow
{
public:
    ventanaConfiguracionesAudio(const String& name, Colour backgroundColour, int buttonsNeeded) : DocumentWindow (name, backgroundColour, buttonsNeeded){
        
        setContentComponentSize(800,600);
        setContentOwned(new ventanaConfiguracionesAudioComponentes,false);
        Rectangle<int> area (0, 0, 480, 310);
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


};



