# 5toryline

5toryline is an exploration into our relationship with machines. With the rapid development of artificial intelligence and machine learning, we have never been closer to being able to co-create with the machines around us. 5toryline explores this idea by creating a platform in which you can collaboratively generate stories with your machine.

You can find more information about this project here. (add CIID link)

## How it Works

When the openFrameworks app is opened you will see a large set of images laid out. With your mouse, click and drag through the images according to the story you want to make. Once you have selected your images you must wait while your machine creates the story for you.



## How to Run It

### Python Script
You'll need to clone the <a href='https://github.com/karpathy/neuraltalk'>NeuralTalk2</a> and <a href='https://github.com/karpathy/char-rnn'>Char-RNN</a> repos into the main folder. 

Follow the instructions included in NeuralTalk2 and Char-RNN to install all the necessary dependencies.

Go to the `/story-creator` directory and run the command `python storycreator.py` from terminal.

### Add Models
The models used for this project were created by Ross Goodwin and can be found at <a href='https://github.com/rossgoodwin/neuralsnap'>NeuralSnap</a>. Follow his instructions on how to download his models and place the files in the included models folder.

### openFrameworks
Download the dataset of images at https://drive.google.com/drive/folders/0BwJRQX2ryWZ_bGpnVmZiZFkyUTg?usp=sharing

Place the images in `bin/data/Dataset500`

Move the entirety of the openFrameworks folder to your desired openFrameworks app destination, then use the project generator to update the project.

You will need to install some dependencies including: ofxBox2d, ofxGui, ofxJSON, ofxKDTree, ofxOsc

If you want to use your own dataset to generate stories — the json file is generated using Gene Kogan's <a href='https://github.com/genekogan/ofxTSNE'>ofxTSNE</a> app. Using this app you can upload your own images and output a json file. Replace the json file `dataset500.json` found in `bin/data` with your newly created on. Also in the openFrameworks code find line 24, `string inputFile = "dataset500.json";` and replace `dataset500.json` with your file.

## Credits

Special thanks goes out to <a href='http://rossgoodwin.com/'>Ross Goodwin</a> — this code is an adapation of his <a href='https://github.com/rossgoodwin/neuralsnap'>NeuralSnap</a> app.

The models included in this repository are also courtesy of Ross Goodwin.

This project was developed during a one week course at <a href='http://ciid.dk/'>Copenhagen Institute of Interaction Design</a> called Intro to Machine Learning. This course was taught by <a href='https://andreasrefsgaard.dk/'>Andreas Refsgaard</a> and <a href='http://genekogan.com/'>Gene Kogan</a>.
