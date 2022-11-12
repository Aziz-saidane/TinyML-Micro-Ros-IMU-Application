# TinyML-Micro-Ros-IMU-Application

![image](https://user-images.githubusercontent.com/85931327/201472095-910f5cbd-7ff6-484e-8e0c-5e4172ac57ba.png)  ![image](https://user-images.githubusercontent.com/85931327/201472105-6eef5b30-7602-4fef-9ef5-2caf0b2bf24d.png) ![Screenshot from 2022-11-12 12-39-01](https://user-images.githubusercontent.com/85931327/201472243-35635a26-b666-443b-97bf-1123574a2746.png)



This prject works with Ubuntu 20 and ROS2 foxy.

This project is inspired from the great work of billiyz: https://github.com/billiyz/tinyml-on-the-edge and ShawnHymel:   https://github.com/ShawnHymel/tinyml-example-anomaly-detection .

The main goal of this project is to be able to detect the movement done by the IMU, either UP or RIGHT using Machine learning directly on the ESP32 microcontroller and send the result of the classification to my Laptop using Micro-Ros and ROS2. 

This project will follow the following procedure.

0. Materials used
1. Setting up arduino IDE with the necessary libraries
2. Look into accelerometer and gyroscope operation
3. Write a sketch to collect accelerometer and gyroscope data
4. Upload the data to the google colaboratory platform
6. Extract Features to be used for the training
5. Train a neural network and Convert the trained model into tensorflow lite model
6. Write a sketch that do the inference in the ESP32 and send data to the Laptop
7. Uploading the code and testing the project


# Materials used

We will work mainly with 2 materials:

1. ESP32 DEVKIT V1 

![image](https://user-images.githubusercontent.com/85931327/201471394-395f5722-785e-4cda-9f3a-09751490d679.png)

2. MPU9250 

![image](https://user-images.githubusercontent.com/85931327/201471480-475dea82-fb94-4b24-a05f-098a2f5a2316.png)

# Setting up arduino IDE 

To work on this project you need to have 4 important libraries

## 1. EloquentTinyML 

This Arduino library is here to simplify the deployment of Tensorflow Lite for Microcontrollers models to Arduino boards using the Arduino IDE and was mainly insired from __Arduino_TensorFlowLite library version 2.4.0-ALPHA__ and __TensorFlowLite_ESP32 version 0.9.0__.

## 2. Micro_ros_arduino

This is a micro-ROS library for baremetal projects based on Arduino IDE or Arduino CLI.

## 3. MPU9250

Arduino library for MPU9250 Nine-Axis (Gyro + Accelerometer + Compass) MEMS MotionTracking™ Device.

-> To install this 3 libraries, you just need to get this repository where you will find zip files with the names of the 3 libraries. 

Include the 3 of them in your project using ``` Sketch -> Include library -> Add .ZIP Library... ```

Finally you need you arduino IDE to be able to recognize the esp32 so you just need to go to:
``` tools -> board -> boards manager ``` then write ESP32 and install the library.


# Accelerometer and gyroscope operation.

## Accelerometer

In IMU 3d accelerometer sensor, the sensing mechanism for all three accelerometers is capacitive. The accelerometer measures linear acceleration along the x, y, and z axis. It has three sensors. The three sensors are mounted orthogonally.

![accel](https://user-images.githubusercontent.com/85931327/201472399-eed2e187-e3a8-43c3-947c-91203898a14f.png)

## Gyroscope

The gyroscpe measures the rate of change of angular velocity over time (angular changes). Many of these sensors are also manufactured using MEMS. Gyroscope also relies on the varying capacitance between silicon and mechanical elements, but with this configuration, the sensor generates capacitive changes with angular velocity changes. A 3D gyroscope also has 3 gyroscopic sensors mounted orthogonally to each other.

![gyro](https://user-images.githubusercontent.com/85931327/201472403-7f3dcfcb-9580-410c-bd23-b914e253453a.png)


Accelerometer and gyrometer sensors face position and angular errors respectively, but fussion process in which they are made and manufactured, aims at perfecting on each other errors thus accuracy is reached.

# Collect sensor data

To capture the accelerometer and gyroscope data, A sketch is written on arduino IDE and uploaded to the board. The sketch in this repository is labelled __collectDataESP32__.
To upload the code:
```
1- Plug the ESP32 Sense board to the computer, select the board type and the port.
2- Open a new terminal 
3- Enter: sudo chmod -R 777 /dev/ttyUSB0
4- Click on upload
```

The sketch will indicate Done uploading. Turn to Tools. and click Serial Monitor to see the accelerometer and gyroscope data.

![ide](https://user-images.githubusercontent.com/85931327/201473893-469d9e42-7a44-48db-a1e0-53a061f71d9b.png)

Make sure you have aX,aY,aZ,gX,gY,gZ on the serial monitor. With the MPU9250 in your hand, make a Move upward to signify 'UP'.Make 20-30 Up movement. On the Serial monitor, uncheck Autoscrol button and copy the data. Save the data on your pc as Up.csv.Make a Move to the right 20-30 times too while checking Serial Monitor and copy the data again, but this time save as Right.csv on your PC.

# Google colaboratory data upload

Open [google Colaboratory platorm](https://colab.research.google.com/drive/1gDI2YYIm0uTYM01g0vLmI6K0hMiy_V9C), Move to Files. Drag and drop your Up.csv and Right.csv files in the sample_data folder. The Google colaboratory provides a Jupyter notebook that allows us to run our TensorFlow training in a web browser.


# Extract Features to be used for the training

To learn how to decide the best features to use for your training to have the best accuracy, you may walk through this tutoriel done by ShawnHymel which was really inspiring for me: [Feature-Extraction](https://www.digikey.com/en/maker/projects/edge-ai-anomaly-detection-part-2-feature-extraction-and-model-training/70927a6e439b49bea7305953a3c9bfff).

For my work, I decided too work with __Median absolute deviation(MAD__ which is a robust measure of how spread out a set of data is. It's the best choice when your data has __extremely high or extremely low values and non normality__

![image](https://user-images.githubusercontent.com/85931327/201474596-25b591f6-c556-491c-9720-d09e5101512d.png)


# Train neural network, build and train a model

While on the Google colaboratory platform, go trough the notebook till the end. Convertion of the trained model into tensorflow lite model is done near the end. When the model.h file is created under Files, click on the model.h file to download to your pc. If you open the model.h file, it will look like the one below.

![finale](https://user-images.githubusercontent.com/85931327/201474851-4e019deb-6da9-48e6-83c6-b9b31ee1be73.png)

![model](https://user-images.githubusercontent.com/85931327/201475118-c1c936cd-0787-4733-94fe-5ec11e51264a.png)


# Inference in ESP32

Open accelerometer and gyroscope clasifier sketch on this repository named as __Inference_IMU_Micro_ROS.ino__.
On the far top right, click on the drop down button and click New Tab. Name the file as model.h when you save it. Open the downloaded model.h file on your favourite editor and copy the content. Paste in your new model.h file you created alongside the classifier sketch and save.

To calculate the Median absolute deviation a C code was written by ShawnHymel and you may find it as __utils.h__ and __utils.c__
You also need to add it to your arduino project.

Finally your project needs to be similar to this one:

![ide](https://user-images.githubusercontent.com/85931327/201475201-51993295-5419-4a7e-84e8-9dfae7f6f27d.png)


# Testing the project

 First we need to upload the project so like last time we do:
 ```
1- Plug the ESP32 Sense board to the computer, select the board type and the port.
2- Open a new terminal 
3- Enter: sudo chmod -R 777 /dev/ttyUSB0
4- Click on upload
```

Next we need to launch the Micro_Ros agent on our laptop

If you're not familiar with Micro_ROS and didn't already install the micro-ROS build system then you may look up to my other repo [First-Steps-with-Micro-ROS-ESP32](https://github.com/Aziz-saidane/First-Steps-with-Micro-ROS-ESP32). 
For this project, you just need to execute the first step "Installing the micro-ROS build system" which will build a new Micro_Ros repository for you.
But if you want to discover how to work with esp32 and Micro-ROS without the arduino IDE, you may finish the repo till the end.

Now that your Micro_Ros repository is created, open a new terminal and click:
```
$ cd microros_ws
$ source /opt/ros/foxy/setup.bash
$ source install/local_setup.bash
$ ros2 run micro_ros_agent micro_ros_agent serial --dev /dev/ttyUSB0
```
![terminal](https://user-images.githubusercontent.com/85931327/201475949-cfad4628-5ffb-486f-8f19-d4727c6cb288.png)

Now that everything is done just open another terminal and run
```
$ source /opt/ros/foxy/setup.bash
$ ros2 topic echo /imu_info_topic 
```

There we are! If everything went OK than you need to see something like this if you do 3 UP and 3 RIGHT:

![up](https://user-images.githubusercontent.com/85931327/201476129-cb5efe10-e242-4845-9d3f-c21a25b3df69.png)









