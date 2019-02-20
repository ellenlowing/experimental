# alternative typing interface with opencv.js

<a href="http://www.youtube.com/watch?feature=player_embedded&v=Lka2lDtg0As" target="_blank"><img src="http://img.youtube.com/vi/Lka2lDtg0As/0.jpg" alt="Type interface" width="500px" /></a>

Inspired by Google Creative Lab's projects such as Teachable Machine and Move Mirror, we hope to make computer vision technology accessible by making it live in the browser. 
In this project, we decided to create a text entry interface by tracking hand movements with computer vision and machine learning for text recognition.

This repo contains only the computer vision part of the project.

The setup of experiment consists of a camera looking at a scene with the user holding their phone with flashlight feature. 
The movements of phone will be traced while flashlight is turned on. The user will perform a quick swipe gesture to represent termination of character. Upon detection of the gesture, the image of a traced character path will be fed into the neural network for character recognization task.

The workflow of the computer vision component can be described in the following image:
<img src="https://raw.githubusercontent.com/j2nelson/585_Project/master/report/img/workflow.png" alt="Workflow of computer vision system" style="width: 70vw; display:block; margin-left:auto; margin-right:auto;">

The computer vision system takes in the camera input and converts it to a grayscale image. The grayscale image is then thresholded to create a binary image. All the pixels with a brightness value below 254 are set to 0 and the rest are 1. The largest blob in binary image is detected, and the blob's centroid is tracked to draw the character path on the canvas. To detect swipe gesture, the system also keeps track of the position of blob in previous frame in order to get velocity and acceleration of the bright spot in scene. Upon detection of a swipe gesture, the canvas is saved, the drawing is cleared for the next character, and the saved canvas is displayed on a separate section of the webpage.
