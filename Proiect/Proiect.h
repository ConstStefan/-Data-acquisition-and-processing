/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  FREQ_Panel                       1
#define  FREQ_Panel_SWITCH_PANEL          2       /* control type: binary, callback function: OnSwitchpanelCB */
#define  FREQ_Panel_GRAPH_FREQ_2          3       /* control type: graph, callback function: (none) */
#define  FREQ_Panel_GRAPH_FREQ            4       /* control type: graph, callback function: (none) */
#define  FREQ_Panel_GRAPH_FREQ_FILTRED    5       /* control type: graph, callback function: (none) */
#define  FREQ_Panel_NUMERIC_FREQ_PEAK     6       /* control type: numeric, callback function: (none) */
#define  FREQ_Panel_NUMERIC_POWER_PEAK    7       /* control type: numeric, callback function: (none) */
#define  FREQ_Panel_BTN_GENERARE_SPECTRU  8       /* control type: command, callback function: OnSpectru */
#define  FREQ_Panel_BTN_EXIT              9       /* control type: command, callback function: ExitBTN */
#define  FREQ_Panel_RING_TIP_FER          10      /* control type: ring, callback function: (none) */
#define  FREQ_Panel_RING_FILTRU_FREQ      11      /* control type: ring, callback function: (none) */
#define  FREQ_Panel_COMMANDBUTTON_FILTRE  12      /* control type: command, callback function: OnAplicareFiltre */
#define  FREQ_Panel_NUMERIC_NPOINTS       13      /* control type: ring, callback function: (none) */

#define  PANEL                            2       /* callback function: OnPanelExit */
#define  PANEL_IDC_BTT_LOAD               2       /* control type: command, callback function: BTTLoad */
#define  PANEL_GRAPH_FILTERED             3       /* control type: graph, callback function: (none) */
#define  PANEL_GRAPH                      4       /* control type: graph, callback function: (none) */
#define  PANEL_BTT_Treceri                5       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_BTT_MEDIANA            6       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_BTT_MEAN               7       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_BTT_DISPERSIE          8       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_BTT_MIN                9       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_BTT_MAX                10      /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_HISTOGRAM_BTT          11      /* control type: command, callback function: HistogramBTTCB */
#define  PANEL_BTT_FILTRU                 12      /* control type: command, callback function: FiltruButtonCB */
#define  PANEL_AlphaBTT                   13      /* control type: numeric, callback function: AlphaCB */
#define  PANEL_FILTRUBTT                  14      /* control type: ring, callback function: (none) */
#define  PANEL_EnvelopeBTT                15      /* control type: command, callback function: EnvelopeCB */
#define  PANEL_PrevBTT                    16      /* control type: command, callback function: Prev_NextBTT */
#define  PANEL_NextBTT                    17      /* control type: command, callback function: Prev_NextBTT */
#define  PANEL_Start                      18      /* control type: numeric, callback function: (none) */
#define  PANEL_Stop                       19      /* control type: numeric, callback function: (none) */
#define  PANEL_SWITCH_PANEL               20      /* control type: binary, callback function: OnSwitchpanelCB */

#define  PANEL_HIST                       3       /* callback function: HistogramPanelCB */
#define  PANEL_HIST_HISTOGRAM             2       /* control type: graph, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK AlphaCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BTTLoad(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK EnvelopeCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ExitBTN(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FiltruButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HistogramBTTCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HistogramPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnAplicareFiltre(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanelExit(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSpectru(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchpanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Prev_NextBTT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif