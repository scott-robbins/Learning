# Graphical User Interfaces 
GUIs are kind of a pain to make. Here I will try to learn how to use Qt5, a tool to help automate the process. 

## Getting started 
Once installed, you can make GUIs in Qt5 with designer using ```pyqt5-tools designer```. 


## Simple GUI
OK, so I'll build a simple GUI using some of the other code from this repository. In this case I will be calling the project test, and PyQt5 saves it with the `.ui` extension. 

## Running the GUI
Convert the UI to runnable python using the following command:
```pyuic5 -x -o main.py test.ui```