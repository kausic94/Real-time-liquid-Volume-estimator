<center><h1>Real-Time liquid Volume Estimator</h1></center>

---

I did this project when I was in college for my sister who was in the 8th grade then. They had science fair in school for which my sister had chosen electronics. I had to come up with something that could blow them teachers' minds. At the same time I had to have it completed in as little time as possible as I was overwhelmed with my various other college work. That is when I decided to come up with this project.

The <i><u>Real-Time Liquid volume Estimator </u></i> computes the volume of the liquid in a container in Real time. I had a broken joystick, an ultasonic sensor and a microcontroller developement board at that time. Hence, this idea.

Basically, you have the ultrasonic at the brim of the container and facing down. The liquid is poured into the container. Ultrasonic waves from the sensor are continually transmitted into the container and the reflection are picked up by the sensor. This helps in estimating the volume in real time.

To make it more interesting I added a simple user interface for the user. There is a 16x2 LCD which is used for display. The input is handled using a single analog joystick and a button which happend to be on the joystick it self.

I gave the user three option, cuboid, cube and cylindrical containers. The first layer of the UI would be to choose the type of the container. This can be done using the joystick and pressing the button to select it. Based on the container , the controller will ask for the inputs of the other dimensions which can be entered using the same joy-stick and the button.

Once the parameters are set and the button is clicked the system starts monitoring the height of the liquid and calculates the volume according to the parameters entered.

Make sure you set up the apparatus accordingly.