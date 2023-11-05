# Disintegration-Art
A C++ module that allows you to manipulate art pieces. This module uses wxWidgets and openCV libraries.

This module allows you to select images in the photos directory. The code automatically pulls up photos, at a maximum of 4, so you may have to move some files around in the case that the files you want are listed in the 5th or later position.

This application allows you to select from 1 to 10 disintegration points, which are used as an origin point to disintegrate it and points around it. Under the threshold of ignoring the pixel intensity values in the form of (RGB), the algorithm will either advance the spread of disintegration or stop there. Any pixel values found under the threshold value will be ignored and that point of expansion will discontinue.

<img width="1189" alt="Screenshot 2023-11-05 at 2 02 38 AM" src="https://github.com/Thanh-sies/Disintegration-Art/assets/7445307/ff02d26d-bbea-4625-8225-8f14a198196f">

After you input the parameters, you can then click on one of the photos that you choose to run the disintegration on. You will then click on the spots on the photo for where you would like the origin of disintegration to spawn. The text below the photo will keep track of where you have placed the points. Once you have selected the required number of points, which you set in the previous page, a new window will spawn showing the disintegration animation. Additionally, when the disintegration terminates, you can find number of iterations needed for the process to terminate.

![Screenshot 2023-11-05 at 2 06 14 AM](https://github.com/Thanh-sies/Disintegration-Art/assets/7445307/2cbc2422-e13e-4a1e-a6e5-ad7ba4d83119)

![Screenshot 2023-11-05 at 2 09 27 AM](https://github.com/Thanh-sies/Disintegration-Art/assets/7445307/2af85385-0486-493b-97b7-b9806d31c97b)

After the algorithm terminates, you can find the iterations in the original window. Here is a side by side comparison which took 888 iterations for the process to terminate.
![Screenshot 2023-11-05 at 2 11 18 AM](https://github.com/Thanh-sies/Disintegration-Art/assets/7445307/586b1273-28c3-412f-ba9c-46c6a760456c)




https://github.com/Thanh-sies/Disintegration-Art/assets/7445307/ac322ff9-796f-46c7-83cb-81b3cff9be06


