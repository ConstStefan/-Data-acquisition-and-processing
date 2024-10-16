#include <formatio.h>
#include <advanlys.h>
#include <cvirte.h>		
#include <userint.h>
#include <ansi_c.h>
#include "Proiect.h"

// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1

static int panelHandle;
static int histogramPanel;
static int acqPanel;
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double alpha;
double max = 0.0;
double min = 0.0;
double *waveData = 0;
double *filt =0;
double *filtredData2=0;
double *anvelopa=0;
int ok=0;
int okey=0;
int stopVal = 0;
int startVal = 0;
int nrpoints = 0.0;
int tipFereastra=0;


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Proiect.uir", PANEL)) < 0)
		return -1;
	if ((acqPanel = LoadPanel (0, "Proiect.uir", FREQ_Panel)) < 0)
		return -1;
	histogramPanel=LoadPanel(panelHandle,"Proiect.uir",PANEL_HIST);
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK OnPanelExit (int panel, int event, void *callbackData,
							 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK BTTLoad (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{

	double medie=0.0;
	double mediana=0.0;
	double dispersia=0.0;
	double contor=0.0;
	int minIndex = 0;
	int maxIndex = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("waveInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//afisare 
			PlotY(panel, PANEL_GRAPH, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			ok=1;
			//dispersie
			//StdDev (waveData, npoints, &medie, &dispersia);
			Variance(waveData,npoints,&medie,&dispersia);
			//medie
			Mean(waveData, npoints, &medie);
			SetCtrlVal(panel,PANEL_IDC_BTT_MEAN,medie);
			//mediana
			Median(waveData,npoints,&mediana);
			SetCtrlVal(panel,PANEL_IDC_BTT_MEDIANA,mediana);
			SetCtrlVal(panel,PANEL_IDC_BTT_DISPERSIE,dispersia);
			
			
			//maxim si minim
			MaxMin1D(waveData,npoints,&max,&maxIndex,&min,&minIndex);
			SetCtrlVal(panel,PANEL_IDC_BTT_MIN,min);
			MaxMin1D(waveData,npoints,&max,&maxIndex,&min,&minIndex);
			SetCtrlVal(panel,PANEL_IDC_BTT_MAX,max);
			
			//de cate ori se trece prin 0
			for(int i=0;i<npoints-1;i++){
				if((waveData[i]>0 && waveData[i+1]<0) ||(waveData[i]<0 && waveData[i+1]>0) ){
					contor++;
				}
			}
			SetCtrlVal(panel,PANEL_BTT_Treceri,contor);
			
			break;
	}
	return 0;
}

int CVICALLBACK HistogramBTTCB (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

				DisplayPanel(histogramPanel);
		
			break;
	}
	return 0;
}

int CVICALLBACK HistogramPanelCB (int panel, int event, void *callbackData,int eventData1, int eventData2)
{
	int x=100;
	switch (event)
	{
		case EVENT_GOT_FOCUS:
		{
			if(ok){
				ssize_t histogramPoints [x];
				double axisPoints[x];
				Histogram(waveData,npoints,min,max,histogramPoints,axisPoints,x);
				DeleteGraphPlot(panel,PANEL_HIST_HISTOGRAM,-1,VAL_IMMEDIATE_DRAW);
				PlotXY(panel,PANEL_HIST_HISTOGRAM,axisPoints,histogramPoints,x,VAL_DOUBLE,VAL_SIZE_T,VAL_VERTICAL_BAR,VAL_EMPTY_SQUARE,VAL_SOLID,1,VAL_BLUE);
				break;
			}
		}
		 case EVENT_LOST_FOCUS:
		{
			HidePanel(histogramPanel);
			break;
		}
		case EVENT_CLOSE:
		{
			
			break;
		}
		case EVENT_LEFT_CLICK:
		{
			
			break;
		}
	}
	return 0;
}

int CVICALLBACK FiltruButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	//filt este vectorul cu valorile filtrare
	filt = (double*)calloc(npoints,sizeof(double));
	int filtru = 0;
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel,PANEL_FILTRUBTT,&filtru);
			switch(filtru)
			{
				case 1:		
					//filtrare prin mediere pe 32 de elemente
					DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED,-1,VAL_DELAYED_DRAW);
					MedianFilter(waveData,npoints,32,0,filt);
					//MedianFilter(waveData,npoints,16,0,filtredData);
					PlotY(panel,PANEL_GRAPH_FILTERED,filt,npoints,VAL_DOUBLE, VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS, VAL_DK_BLUE);
					
					break;
				case 2:		
					//filtrare prin mediere cu un element de ordin 1
					DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED,-1,VAL_DELAYED_DRAW); 
					filt[0]=waveData[0];
					for(int i=1;i<npoints;i++)
					{
						filt[i] = (1-alpha)*filt[i-1] + alpha*waveData[i];
					}
					PlotY(panel,PANEL_GRAPH_FILTERED,filt,npoints,VAL_DOUBLE, VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS, VAL_DK_BLUE);
			}

			
			if(okey == 1) {
			GetCtrlVal(panel, PANEL_Start, &startVal);
			GetCtrlVal(panel, PANEL_Stop, &stopVal); 
					
			double *vector = filt + (int)(startVal * sampleRate);
			
			//afisare pe secunde	
			DeleteGraphPlot(panel, PANEL_GRAPH_FILTERED, -1, VAL_DELAYED_DRAW);
			PlotY(panel, PANEL_GRAPH_FILTERED, vector,sampleRate, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_MAGENTA);
			okey =0;
			}
			
	}
	return 0;
}	

int CVICALLBACK AlphaCB (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				GetCtrlVal(panel,PANEL_AlphaBTT,&alpha);
			break;
	}
	return 0;
}

int CVICALLBACK EnvelopeCB (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//afisarea anvelopei semnalului
			anvelopa = (double *) calloc(npoints, sizeof(double));   
			FileToArray("anvelopa.txt", anvelopa, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			PlotY(panel, PANEL_GRAPH, anvelopa, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLACK); 
		
			break;
	}
	return 0;
}

int CVICALLBACK Prev_NextBTT (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	//butoanele de prev si next
	
	
	int maxSecunde =(int) npoints/sampleRate; 
	int valStop = 0;
	int valStart = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			switch(control)
			{
				case PANEL_PrevBTT:
					switch(event)
					{
						case EVENT_COMMIT:
							okey = 1;
							GetCtrlVal(panel, PANEL_Start, &valStart);
							GetCtrlVal(panel, PANEL_Stop, &valStop);
							if(valStart > 0 && valStop <= maxSecunde)
							{
								SetCtrlVal(panel, PANEL_Start, valStart-1);
								SetCtrlVal(panel, PANEL_Stop, valStop-1);
								FiltruButtonCB(panel,PANEL_PrevBTT, EVENT_COMMIT,0,0,0);
							}
							break;		   
					}
					break;
		
				case PANEL_NextBTT:
					switch(event)
					{
						case EVENT_COMMIT:
							okey = 1;
							GetCtrlVal(panel, PANEL_Start, &valStart);
							GetCtrlVal(panel, PANEL_Stop, &valStop);
							if(valStart >= 0 && valStop < maxSecunde)
							{
								SetCtrlVal(panel, PANEL_Start, valStart+1);
								SetCtrlVal(panel, PANEL_Stop, valStop+1);
								FiltruButtonCB(panel,PANEL_NextBTT, EVENT_COMMIT,0,0,0);
							}
							break;
					}
					break;
			}
			break;
	}
	
	return 0;
}

int CVICALLBACK OnSwitchpanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel==panelHandle)
			{
				SetCtrlVal(acqPanel,FREQ_Panel_SWITCH_PANEL,1);
				DisplayPanel(acqPanel);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(panelHandle, PANEL_SWITCH_PANEL, 0);
				DisplayPanel(panelHandle);
				HidePanel(panel);
			}

			break;
	}
	return 0;
}

int CVICALLBACK OnSpectru (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	GetCtrlVal(panel, FREQ_Panel_NUMERIC_NPOINTS, &nrpoints); 
	double temp[nrpoints];
	double autoSpectrum[nrpoints/2];
	double dt = 1.0/sampleRate;
	double convertedSpectrum[nrpoints/2]; 
	double powerPeak = 0.0;
	double freqPeak = 0.0;  
	double df = 0.0;
	int i;
	char unit[32] = "V";
	switch (event)
	{
		case EVENT_COMMIT:
			for(i=0; i<nrpoints;i++)
			{
				temp[i] = waveData[i];
			}
			WindowConst winConst;
			GetCtrlVal(acqPanel, FREQ_Panel_RING_TIP_FER, &tipFereastra);
			if(tipFereastra == 1) //WELCH
			{
				ScaledWindowEx(temp, nrpoints,WELCH, 0, &winConst); 
			}
			if(tipFereastra == 2)//Hamming
			{
				ScaledWindowEx(temp, nrpoints,HAMMING, 0, &winConst); 
			}
			AutoPowerSpectrum(temp, nrpoints, dt, autoSpectrum, &df);
			PowerFrequencyEstimate(autoSpectrum, nrpoints/2, -1, winConst, df, 7, &freqPeak, &powerPeak);
			//afisez pe interfata valorile determinate
			SetCtrlVal( acqPanel, FREQ_Panel_NUMERIC_FREQ_PEAK, freqPeak);
			SetCtrlVal( acqPanel, FREQ_Panel_NUMERIC_POWER_PEAK, powerPeak);
			//Functia converteste spectrul de intrare (care este puterea, amplitudinea sau amplificarea) 

			SpectrumUnitConversion(autoSpectrum, nrpoints/2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);

			//sterg graph-ul unde urmeaza sa plotez spectrul semnalului
			DeleteGraphPlot (acqPanel, FREQ_Panel_GRAPH_FREQ, -1, VAL_IMMEDIATE_DRAW);	
			//plotez spectrul semnalului
			PlotWaveform( acqPanel, FREQ_Panel_GRAPH_FREQ, convertedSpectrum, nrpoints/4 ,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
                                VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_BLUE);
			
		
			break;
	}
	return 0;
}

int CVICALLBACK OnAplicareFiltre (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
GetCtrlVal(panel, FREQ_Panel_NUMERIC_NPOINTS, &nrpoints); 
	double temp[nrpoints];
	double autoSpectrum[nrpoints/2];
	double dt = 1.0/sampleRate;
	double convertedSpectrum[nrpoints/2]; 
	double powerPeak = 0.0;
	double freqPeak = 0.0;  
	double df = 0.0;
	int i;
	char unit[32] = "V";
    int filtru = 0;
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			filtredData2 = (double *) calloc(npoints, sizeof(double));
	
			 GetCtrlVal(acqPanel, FREQ_Panel_RING_FILTRU_FREQ, &filtru);
			 
			 if(filtru == 1) //ButterWorth ordin 4 trece jos
			{
				Bw_LPF(waveData,nrpoints,sampleRate,500,4,filtredData2);
				//DeleteGraphPlot(acqPanel,FREQ_Panel_GRAPH_FREQ_2,-1,VAL_IMMEDIATE_DRAW);
				//PlotY(acqPanel,FREQ_Panel_GRAPH_FREQ_2, filtredData2, nrpoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_RED); 
			}
			if(filtru == 2) //Chebyshev 2 ordin 4 trece jos
			{
				
				InvCh_LPF(waveData,nrpoints,sampleRate,500,40.0,4,filtredData2);
				//DeleteGraphPlot(acqPanel,FREQ_Panel_GRAPH_FREQ_2,-1,VAL_IMMEDIATE_DRAW);
				//PlotY(acqPanel,FREQ_Panel_GRAPH_FREQ_2, filtredData2, nrpoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_RED); 
			}
			
			for(i=0; i<nrpoints;i++)
			{
				temp[i] = filtredData2[i];
			}
			
			WindowConst winConst;
			
			
			GetCtrlVal(acqPanel, FREQ_Panel_RING_TIP_FER, &tipFereastra);
			
			if(tipFereastra == 1) //Welch
			{
				ScaledWindowEx(temp, nrpoints,WELCH, 0, &winConst); 
				DeleteGraphPlot(acqPanel,FREQ_Panel_GRAPH_FREQ_2,-1,VAL_IMMEDIATE_DRAW);
				PlotY(acqPanel,FREQ_Panel_GRAPH_FREQ_2, temp, nrpoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_RED);
			}
			
			if(tipFereastra == 2) //Hamming
			{
				ScaledWindowEx(temp, nrpoints, HAMMING, 0, &winConst); 
				DeleteGraphPlot(acqPanel,FREQ_Panel_GRAPH_FREQ_2,-1,VAL_IMMEDIATE_DRAW);
				PlotY(acqPanel,FREQ_Panel_GRAPH_FREQ_2, temp, nrpoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_RED);
			}

			AutoPowerSpectrum(temp, nrpoints, dt, autoSpectrum, &df);
			
			PowerFrequencyEstimate(autoSpectrum, nrpoints/2, -1, winConst, df, 7, &freqPeak, &powerPeak);
	
			//afisez pe interfata valorile determinate
			SetCtrlVal( acqPanel, FREQ_Panel_NUMERIC_FREQ_PEAK, freqPeak);
			SetCtrlVal( acqPanel, FREQ_Panel_NUMERIC_POWER_PEAK, powerPeak);
			//Functia converteste spectrul de intrare (care este puterea, amplitudinea sau amplificarea) 
			//în formate alternative (linear, logarithmic, dB) ce permit o reprezentare grafica mai convenabila.

			
			SpectrumUnitConversion(autoSpectrum, nrpoints/2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);

			//sterg graph-ul unde urmeaza sa plotez spectrul semnalului
			DeleteGraphPlot (acqPanel, FREQ_Panel_GRAPH_FREQ_FILTRED, -1, VAL_IMMEDIATE_DRAW);	
			//plotez spectrul semnalului
			PlotWaveform( acqPanel, FREQ_Panel_GRAPH_FREQ_FILTRED, convertedSpectrum, nrpoints/4 ,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
                                VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_RED);
			
			
			break;
	}
	return 0;
}

int CVICALLBACK ExitBTN (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface(0);
			break;
	}
	return 0;
}
