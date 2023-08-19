#include "PlotConfiguration.h"

/*
	this function will serve all objects looking to plot using a fixed method of spacing,
	implemented as a global function in order to let all objects use this function independently of what type they are.
	any spacer plot can be simply called by using this function with any character delim, prefered using a defined
*/
void plotSpacerDelim(delimType delim)
{
	cout << string(PLOT_SPACER_NUM_OF_DELIMS, (char)delim) << endl;
}
