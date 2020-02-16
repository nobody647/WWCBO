    _       ____  _________   __   _       ________    __       ________    ___   __________    ____  ______   ____ _    ____________
    | |     / / / / / ____/ | / /  | |     / /  _/ /   / /      / ____/ /   /   | / ___/ ___/   / __ )/ ____/  / __ \ |  / / ____/ __ \
    | | /| / / /_/ / __/ /  |/ /   | | /| / // // /   / /      / /   / /   / /| | \__ \\__ \   / __  / __/    / / / / | / / __/ / /_/ /
    | |/ |/ / __  / /___/ /|  /    | |/ |/ // // /___/ /___   / /___/ /___/ ___ |___/ /__/ /  / /_/ / /___   / /_/ /| |/ / /___/ _, _/
    |__/|__/_/ /_/_____/_/ |_/     |__/|__/___/_____/_____/   \____/_____/_/  |_/____/____/  /_____/_____/   \____/ |___/_____/_/ |_|
  
  
In highschool, a question I found myself asking quite frequently was, "when will class be over?" Instead of learning how to read an analog clock and memorizing what time the bell rings like a sensible person, I decided to design, manufacture, and (attempt to) sell consumer electronics devices to answer the all important question of When Will Class Be Over, or WWCBO for short.

The code here is C++ using Arduino libraries designed to run on an ATMEGA328P on a custom board I designed. If you want to check out the hardware side of this project, head on over to https://imgur.com/a/ui9bz0x

If you want to get a good idea of my coding style, I'd honestly reccomend you **don't** take a look at this project. I wrote this when I was 16 and had no idea what I was doing. It's pooly commented, contains plenty of bad coding practices, and isn't really representative of the quality of code I'm capable of producing. I'm featuring this project because I think it's cool, not because it's well written.

The basic structure of the program is pretty simple. It has a main loop that displays the current time and time until a class ends. To conserve power, the main loop only runs every 8 seconds. This is acheived by using the LowPower library. At the end of every loop, I essentially tell the hardware to enter deep sleep for 8 seconds. Once it's done sleeping, it wakes up, runs the main loop, and falls back asleep.

When there is less than a minute remaining in a class, the main loop will only sleep for 1 second, updating the display with the number of seconds remaining until the bell rings and flashing the display on and off. The idea was to go for a cool final countdown effect, but the clock actually drifts by a second or two every day, so the final countdown to the bell doens't really work too well.

There is a pin interrupt for the single button on the front of the device. When pressed, the button will toggle the display backlight. The button I used is "bouncy", meaning one button press often causes multiple interrupts. I "debounce" it by keeping track of the number of milliseconds since the last button press and ignoring all presses that occur less than a second after the first press.

It reads from custom class schedule (classes.h) stored in flash memory. The way the class schedule is organized is confusing and a bit silly, but at the time I didn't know a better way to store the schedule data.
