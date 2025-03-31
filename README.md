this is a very basic "game" about a spaceship flying through space shooting asteroids. there is no winning or losing and no score is kept.
however this game is written with a custom rendering engine using openGL.

this was a learning project, this is not how you should write a rendering engine with openGL, it is highly unscalable.

I made a lot of mistakes making this project, and a lot of them got me too deep in technical debt that its going to be very hard to try and expand this project in its current form.
But, im still very happy with how it turned out, I learned a lot from my mistakes, and next time i could make my rendering engine a lot more flexible.

i learned a lot from `https://learnopengl.com`, it is a great source and I highly recommend it. however if you want to create something more scalable you shouldnt just copy directly from there.

if anyone ever plans on trying this "game" for themselves, you can build it using CMake and VCPKG for the dependecies. keep in mind there isnt really a Release version, you can only build it for debug.