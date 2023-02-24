#pragma once

#include <stdlib.h>
#include "../../Display/EPaperDisplay.h"

// The type of data the graph is showing
#define GraphDataType uint8_t
// How much space to the left of the graph is going to be left out for lables
#define FONT_PADDING_LEFT 32

class Graph 
{
private: 
    EPaperDisplay* Display;
    // PosX, PosY, Width and Height define the boundary for the graph
    uint16_t PosX, PosY;
    uint16_t Width, Height;

    // MaxValue is a constantly updated value which contains the highest value in the data-array
    // Used for displaying the data correctly
    GraphDataType MaxValue;
    GraphDataType MinValue;

    // Contains all of the data in the graph
    GraphDataType* Data;
    // Length of the data array
    uint16_t DataLength;
    // Index of the last added data point in the data array
    uint16_t Counter;
    
    // Method used for calculating the y position on the display of a certain data point
    uint16_t CalcDataPosY(GraphDataType dataPoint);

    // Calculates the x position on the display for a given index of a data point
    uint16_t CalcDataPosX(uint16_t index);

public: 
    Graph(EPaperDisplay* display, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    // Adds the next data point to the Data-Array, increases the index (CounteR) and updates MaxValue
    void AddNextValue(GraphDataType point);

    // Displays the graph at the given boundary box on the display
    void Draw();    
};