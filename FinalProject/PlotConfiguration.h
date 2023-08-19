#pragma once

#include <string>
#include <iostream>
using namespace std;
#define PLOT_SPACER_NUM_OF_DELIMS 100
#define PLOT_SPACER_MAIN_DELIM '='
#define PLOT_SPACER_SUB_DELIM '_'
#define PLOT_SPACER_REM_DELIM 'X'
#define PLOT_SPACER_SLOT_DELIM '-'

enum class delimType { Main = '=', Sub = '_', Rem = 'X', Slot = '-' };

void plotSpacerDelim(delimType delim);
