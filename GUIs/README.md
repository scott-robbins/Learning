# Graphical User Interfaces 
GUIs are kind of a pain to make. Here I will try to learn how to use Qt5, a tool to help automate the process. 

## Getting started 
Once installed, you can make GUIs in Qt5 with designer using ```pyqt5-tools designer```. 


## Simple GUI
OK, so I'll build a simple GUI using some of the other code from this repository. In this case I will be calling the project test, and PyQt5 saves it with the `.ui` extension. 

## Running the GUI
Convert the UI to runnable python using the following command:
```pyuic5 -x -o main.py test.ui```

## Making the Loading bar tick
We need to 'connect' the progress bar to a function, and call that function, in order to update the progress bar over time. I've defined a small function in the `main.py` file to do this. The function is `update_progress`. As this is purely for demonstration, the progress bar simply progresses as time updates (and nothing happens at 100%). 

To do so I've also added a `QTimer` definition within the `setupUi` function that will callback to the `update_progress` function every 10 milliseconds:
```
self.timer = QTimer()
self.timer.timeout.connect(self.update_progress)
self.timer.start(10)
```