/*
 * Filename: CDKMatrix.cc
 * Date: November 25, 2020
 * Author: Anh Nguyen
 * Email: nxn190023@utdallas.edu
 * Course CS 3377.002 Fall 2020
 * Version 1.0 (or correct version)
 * Copyright 2020, All Rights Reserved
 *
 * Description
 *
 * Build and displace a matrix with binary file contents using
 * curses and CDK.
 */

#include <iostream>
#include "cdk.h"
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdint.h>

#define MATRIX_ROWS 5
#define MATRIX_COLS 3
#define BOX_WIDTH 25
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;
int main()
{

  WINDOW    *window;
  CDKSCREEN *cdkscreen;
  CDKMATRIX *myMatrix; 

  const char *rowTitles[MATRIX_ROWS+1] = {"IGNORE", "a", "b", "c", "d", "e"};
  const char *columnTitles[MATRIX_COLS+1] = {"IGNORE", "a", "b", "c"};
  int colWidths[MATRIX_COLS+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int boxTypes[MATRIX_COLS+1] = {vMIXED, vMIXED, vMIXED, vMIXED};

  window = initscr();
  cdkscreen = initCDKScreen(window); 
  initCDKColor();

  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_ROWS, MATRIX_COLS, MATRIX_ROWS, MATRIX_COLS,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, colWidths,
			  boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      cout << "Error creating matrix." << endl;
      _exit(1);
    }

  drawCDKMatrix(myMatrix, true);

  const int maxRecordStringLength = 25;

  class BinaryFileHeader
  {
  public:
    uint32_t magicNumber;
    uint32_t versionNumber;
    uint64_t numRecords;
  };
  
  class BinaryFileRecord
  {
  public:
    uint8_t strLength;
    char stringBuffer[maxRecordStringLength];
  };
  
  BinaryFileRecord *myRecord = new BinaryFileRecord();
  BinaryFileHeader *myHeader = new BinaryFileHeader();

  ifstream binInFile ("/scratch/perkins/cs3377.bin", ios::in | ios::binary);
  if(binInFile)
    {
      binInFile.read((char*)myHeader, sizeof(BinaryFileHeader));
      stringstream magics;
      magics  << "Magic: 0x" << uppercase << hex << myHeader->magicNumber << endl;
      setCDKMatrixCell(myMatrix, 1, 1, (magics.str()).c_str());;

      stringstream vers;
      vers << "Version: "<< myHeader->versionNumber << endl;
      setCDKMatrixCell(myMatrix, 1, 2, (vers.str()).c_str());
      
      stringstream nums;
      nums << "NumRecords: " << myHeader->numRecords << endl;
      setCDKMatrixCell(myMatrix, 1, 3, (nums.str()).c_str());
      
      int i = 2;
      int j = 1;

      while(!binInFile.eof())
	{
	  binInFile.read((char*)myRecord, sizeof(BinaryFileRecord));
	  stringstream buffs;
	  buffs << myRecord->stringBuffer << endl;
	  setCDKMatrixCell(myMatrix, i, j+1, (buffs.str()).c_str());
	  
	  stringstream buffs2;
	  buffs2 << "strlen: " << strlen(myRecord->stringBuffer)<< endl;
	  setCDKMatrixCell(myMatrix, i, j, (buffs2.str()).c_str());

	  i++;
	}
      binInFile.close();
    }
  else 
    cout<< "ERROR: No binary file found." << endl;
  

  drawCDKMatrix(myMatrix, true);    
 
  sleep(10);
  endCDK();
  return 0;
}
